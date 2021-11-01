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
 * \file apb_config_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _APB_CONFIG_REGSTRUCTS_H_
#define _APB_CONFIG_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Chip configuration registers (apb_config)
/** \brief Configuration registers contained in apb_config.v*/
//
//====================================================================

typedef struct APB_CONFIG_REGS_s
{
  volatile uint32_t AIR;  ///< 0x0 [R]: ASIC Identification Register
  volatile uint32_t ASR;  ///< 0x4 [R]: ASIC Configuration/Boot Status Register
  volatile uint32_t OFCR;  ///< 0x8 [R/W]: Optional Feature Control Register
  volatile uint32_t OFSR;  ///< 0xc [R]: Optional Feature Status Register
  volatile uint32_t PINCR;  ///< 0x10 [R/W]: Pinout and MemMap Configuration Register
  volatile uint32_t reserved0[11];
  volatile uint32_t PDCR1;  ///< 0x40 [R/W]: Pad Drive Configuration Register 1
  volatile uint32_t PDCR2;  ///< 0x44 [R/W]: Pad Drive Configuration Register 2
  volatile uint32_t PDCR3;  ///< 0x48 [R/W]: Pad Drive Configuration Register 3
  volatile uint32_t reserved1;
  volatile uint32_t PVSCR;  ///< 0x50 [R/W]: Pad Voltage Sensor Configuration Register
  volatile uint32_t PVSSR;  ///< 0x54 [R]: Pad Voltage Sensor Status Register
  volatile uint32_t reserved2[10];
  volatile uint32_t BGCR;  ///< 0x80 [R/W]: Bandgap Configuration Register
  volatile uint32_t reserved3[31];
  volatile uint32_t PLL0_CR0;  ///< 0x100 [R/W]: PLL 0 Configuration Register 0
  volatile uint32_t PLL0_CR1;  ///< 0x104 [R/W]: PLL 0 Configuration Register 1
  volatile uint32_t PLL0_CR2;  ///< 0x108 [R/W]: PLL 0 Configuration Register 2
  volatile uint32_t PLL0_CR3;  ///< 0x10c [R/W]: PLL 0 Configuration Register 3
  volatile uint32_t PLL0_SR;  ///< 0x110 [R]: PLL 0 Status Register
  volatile uint32_t reserved4[3];
  volatile uint32_t PLL1_CR0;  ///< 0x120 [R/W]: PLL 1 Configuration Register 0
  volatile uint32_t PLL1_CR1;  ///< 0x124 [R/W]: PLL 1 Configuration Register 1
  volatile uint32_t PLL1_CR2;  ///< 0x128 [R/W]: PLL 1 Configuration Register 2
  volatile uint32_t PLL1_CR3;  ///< 0x12c [R/W]: PLL 1 Configuration Register 3
  volatile uint32_t PLL1_SR;  ///< 0x130 [R]: PLL 1 Status Register
  volatile uint32_t reserved5[3];
  volatile uint32_t PLL2_CR0;  ///< 0x140 [R/W]: PLL 2 Configuration Register 0
  volatile uint32_t PLL2_CR1;  ///< 0x144 [R/W]: PLL 2 Configuration Register 1
  volatile uint32_t reserved6[2];
  volatile uint32_t PLL2_SR;  ///< 0x150 [R]: PLL 2 Status Register
  volatile uint32_t reserved7[11];
  volatile uint32_t CDCR1;  ///< 0x180 [R/W]: Clock Divider Configuration Register 1
  volatile uint32_t reserved8[31];
  volatile uint32_t PWCR;  ///< 0x200 [R/W]: Powerdown Configuration Register
  volatile uint32_t PICR;  ///< 0x204 [R/W]: Power Island Configuration Register
  volatile uint32_t PISR;  ///< 0x208 [R]: Power Island Status Register
  volatile uint32_t reserved9[61];
  volatile uint32_t SRCR0;  ///< 0x300 [R/W]: SRAM WTC/RTC Configuration Register 0
  volatile uint32_t SRCR1;  ///< 0x304 [R/W]: SRAM WTC/RTC Configuration Register 1
  volatile uint32_t SRCR2;  ///< 0x308 [R/W]: SRAM WTC/RTC Configuration Register 2
  volatile uint32_t SRCR3;  ///< 0x30c [R/W]: SRAM WTC/RTC Configuration Register 3
  volatile uint32_t SRCR4;  ///< 0x310 [R/W]: SRAM WTC/RTC Configuration Register 4
  volatile uint32_t SRCR5;  ///< 0x314 [R/W]: SRAM WTC/RTC Configuration Register 5
  volatile uint32_t SRCR6;  ///< 0x318 [R/W]: SRAM WTC/RTC Configuration Register 6
  volatile uint32_t SRCR7;  ///< 0x31c [R/W]: SRAM WTC/RTC Configuration Register 7
  volatile uint32_t SRCR8;  ///< 0x320 [R/W]: SRAM WTC/RTC Configuration Register 8
  volatile uint32_t SRCR9;  ///< 0x324 [R/W]: EROM RTC Configuration Register 9
  volatile uint32_t reserved10[22];
  volatile uint32_t FCRR;  ///< 0x380 [R/W]: Firmware-controlled Reset Register
  volatile uint32_t reserved11[31];
  volatile uint32_t FBCR1;  ///< 0x400 [R/W]: FW BIST Control Register 1
  volatile uint32_t FBCR2;  ///< 0x404 [R/W]: FW BIST Control Register 2
  volatile uint32_t FBCR3;  ///< 0x408 [R/W]: FW BIST Control Register 3
  volatile uint32_t FBCR4;  ///< 0x40c [R/W]: FW BIST Control Register 4
  volatile uint32_t FBCR5;  ///< 0x410 [R/W]: FW BIST Control Register 5
  volatile uint32_t FBCR6;  ///< 0x414 [R/W]: FW BIST Control Register 6
  volatile uint32_t reserved12[2];
  volatile uint32_t FBFR1;  ///< 0x420 [R]: FW BIST Finish Register 1
  volatile uint32_t FBFR2;  ///< 0x424 [R]: FW BIST Finish Register 2
  volatile uint32_t FBFR3;  ///< 0x428 [R]: FW BIST Finish Register 3
  volatile uint32_t FBFR4;  ///< 0x42c [R]: FW BIST Finish Register 4
  volatile uint32_t FBFR5;  ///< 0x430 [R]: FW BIST Finish Register 5
  volatile uint32_t FBFR6;  ///< 0x434 [R]: FW BIST Finish Register 6
  volatile uint32_t reserved13[2];
  volatile uint32_t FBER1;  ///< 0x440 [R]: FW BIST Error Register 1
  volatile uint32_t FBER2;  ///< 0x444 [R]: FW BIST Error Register 2
  volatile uint32_t FBER3;  ///< 0x448 [R]: FW BIST Error Register 3
  volatile uint32_t FBER4;  ///< 0x44c [R]: FW BIST Error Register 4
  volatile uint32_t FBER5;  ///< 0x450 [R]: FW BIST Error Register 5
  volatile uint32_t FBER6;  ///< 0x454 [R]: FW BIST Error Register 6
  volatile uint32_t reserved14[42];
  volatile uint32_t GPR;  ///< 0x500 [R/W]: General Purpose Register
  volatile uint32_t reserved15[127];
  volatile uint32_t RTC_ATEST;  ///< 0x700 [R/W]: RTC Analog Test Pin Config
} APB_CONFIG_REGS_t;

#endif // APB_CONFIG
