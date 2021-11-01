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
 * \file LCD2_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _LCD2_REGSTRUCTS_H_
#define _LCD2_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: LCD2 Display Controller (LCD2)
/** \brief Registers for the enhanced LCD Display Controller Block are described in this
section. Regarding the register naming convention, registers for configuring
the video DMA are called LCD_DMA*, and registers for configuring the graphic DMA
are class LCD_GRA*.*/
//
//====================================================================

typedef struct LCD2_REGS_s
{
  volatile uint32_t reserved0[39];
  volatile uint32_t LCD_SOFT_RESET;  ///< 0x9c [R/W]: LCD Software Reset
  volatile uint32_t reserved1[8];
  volatile uint32_t LCD_DMA_START_ADDR_Y0;  ///< 0xc0 [R/W]: Panel Video Frame 0 Y Starting Address Register
  volatile uint32_t LCD_DMA_START_ADDR_U0;  ///< 0xc4 [R/W]: Panel Video Frame 0 U Starting Address Register
  volatile uint32_t LCD_DMA_START_ADDR_V0;  ///< 0xc8 [R/W]: Panel Video Frame 0 V Starting Address Register
  volatile uint32_t LCD_DMA_START_ADDR_C0;  ///< 0xcc [R/W]: Panel Video Frame 0 Command Starting Address Register
  volatile uint32_t LCD_DMA_START_ADDR_Y1;  ///< 0xd0 [R/W]: Panel Video Frame 1 Y Starting Address Register
  volatile uint32_t LCD_DMA_START_ADDR_U1;  ///< 0xd4 [R/W]: Panel Video Frame 1 U Starting Address Register
  volatile uint32_t LCD_DMA_START_ADDR_V1;  ///< 0xd8 [R/W]: Panel Video Frame 1 V Starting Address Register
  volatile uint32_t LCD_DMA_START_ADDR_C1;  ///< 0xdc [R/W]: Panel Video Frame 1 Command Starting Address Register
  volatile uint32_t LCD_DMA_PITCH_YC;  ///< 0xe0 [R/W]: Panel Video Y and C Line Length (Pitch) Register
  volatile uint32_t LCD_DMA_PITCH_UV;  ///< 0xe4 [R/W]: Panel Video U and V Line Length (Pitch) Register
  volatile uint32_t LCD_DMA_OVSA_HPXL_VLN;  ///< 0xe8 [R/W]: Panel Video Starting Point on Screen Register
  volatile uint32_t LCD_DMA_HPXL_VLN;  ///< 0xec [R/W]: Panel Video Source Size Register
  volatile uint32_t LCD_DMAZM_HPXL_VLN;  ///< 0xf0 [R/W]: Panel Video Destination Size (After Zooming) Register
  volatile uint32_t LCD_GRA_START_ADDR0;  ///< 0xf4 [R/W]: Panel Graphic Frame 0 Starting Address Register
  volatile uint32_t LCD_GRA_START_ADDR1;  ///< 0xf8 [R/W]: Panel Graphic Frame 1 Starting Address Register
  volatile uint32_t LCD_GRA_PITCH;  ///< 0xfc [R/W]: Panel Graphic Line Length (Pitch) Register
  volatile uint32_t LCD_GRA_OVSA_HPXL_VLN;  ///< 0x100 [R/W]: Panel Graphic Starting Point on Screen Register
  volatile uint32_t LCD_GRA_HPXL_VLN;  ///< 0x104 [R/W]: Panel Graphic Source Size Register
  volatile uint32_t LCD_GRAZM_HPXL_VLN;  ///< 0x108 [R/W]: Panel Graphic Destination Size (after Zooming) Register
  volatile uint32_t reserved2[2];
  volatile uint32_t LCD_PN_V_H_TOTAL;  ///< 0x114 [R/W]: Panel Total Screen Size Register
  volatile uint32_t LCD_PN_V_H_ACTIVE;  ///< 0x118 [R/W]: Panel Screen Active Size Register
  volatile uint32_t LCD_PN_H_PORCH;  ///< 0x11c [R/W]: Panel Screen Horizontal Porch Register
  volatile uint32_t LCD_PN_V_PORCH;  ///< 0x120 [R/W]: Panel Screen Vertical Porch Register
  volatile uint32_t LCD_PN_BLANKCOLOR;  ///< 0x124 [R/W]: Panel Screen Blank Color Register
  volatile uint32_t reserved3[2];
  volatile uint32_t LCD_PN_COLORKEY_Y;  ///< 0x130 [R/W]: Panel Video Y Color Key Control
  volatile uint32_t LCD_PN_COLORKEY_U;  ///< 0x134 [R/W]: Panel Video U Color Key Control Register
  volatile uint32_t LCD_PN_COLORKEY_V;  ///< 0x138 [R/W]: Panel Video V Color Key Control Register
  volatile uint32_t LCD_PN_SEPXLCNT;  ///< 0x13c [R/W]: Panel VSYNC Pulse Pixel Edge Control Register
  volatile uint32_t reserved4;
  volatile uint32_t LCD_ISA_RXDATA;  ///< 0x144 [R]: Smart Panel Read Data Register
  volatile uint32_t LCD_READ_IOPAD;  ///< 0x148 [R]: Panel Path Panel Interface I/O Pad Read Register
  volatile uint32_t LCD_DMAVLD_YC;  ///< 0x14c [R]: Panel Video Actual Line Length in Memory Register
  volatile uint32_t LCD_DMAVLD_UV;  ///< 0x150 [R]: Panel Video U and V Actual Line Length in Memory Register
  volatile uint32_t LCD_TVGGRAVLD_HLEN;  ///< 0x154 [R]: Graphic Actual Line Length (Pitch) in System Memory
  volatile uint32_t reserved5;
  volatile uint32_t LCD_PN_GAMMA_RDDAT;  ///< 0x15c [R]: Panel Gamma Table SRAM Read Data Register
  volatile uint32_t LCD_PN_PALETTE_RDDAT;  ///< 0x160 [R]: Panel Palette Table SRAM Read Data Register
  volatile uint32_t LCD_SLV_DBG;  ///< 0x164 [R]: Panel Slave Path Status and Debug Register
  volatile uint32_t reserved6[5];
  volatile uint32_t LCD_FRAME_CNT;  ///< 0x17c [R]: Display Frame Number Read Only Registers
  volatile uint32_t reserved7[2];
  volatile uint32_t LCD_SMPN_CTRL;  ///< 0x188 [R/W]: Smart Panel 8-bit Bus Control Register
  volatile uint32_t LCD_SLV_PORT;  ///< 0x18c [W]: AHB Slave Path Data Port Register
  volatile uint32_t LCD_PN_CTRL0;  ///< 0x190 [R/W]: Panel Path DMA Control 0 Register
  volatile uint32_t LCD_PN_CTRL1;  ///< 0x194 [R/W]: Panel Path DMA Control 1 Register
  volatile uint32_t LCD_SRAM_CTRL;  ///< 0x198 [R/W]: SRAM Control Register
  volatile uint32_t LCD_SRAM_WRDAT;  ///< 0x19c [W]: SRAM Write Data Register
  volatile uint32_t reserved8;
  volatile uint32_t LCD_SRAM_PARA1;  ///< 0x1a4 [R/W]: SRAM Power Down Control Register
  volatile uint32_t LCD_SCLK_DIV;  ///< 0x1a8 [R/W]: Smart Panel or Dumb Panel Clock Divider
  volatile uint32_t LCD_PN_CONTRAST;  ///< 0x1ac [R/W]: Panel Path Video Contrast Register
  volatile uint32_t LCD_PN_SATURATION;  ///< 0x1b0 [R/W]: Panel Path Video Saturation Register
  volatile uint32_t LCD_PN_CBSH_HUE;  ///< 0x1b4 [R/W]: Panel Path Video Hue Adjust Register
  volatile uint32_t LCD_DUMB_CTRL;  ///< 0x1b8 [R/W]: Panel Path Dumb LCD Panel Control Register
  volatile uint32_t PN_IOPAD_CONTROL;  ///< 0x1bc [R/W]: Panel Path I/O Pads Control Register
  volatile uint32_t SPU_IRQ_ENA;  ///< 0x1c0 [R/W]: LCD Interrupt Control Register
  volatile uint32_t SPU_IRQ_ISR;  ///< 0x1c4 [R/W]: LCD Interrupt Status Register
  volatile uint32_t SPU_IRQ_RSR;  ///< 0x1c8 [R/W]: LCD Interrupt Reset Status Register
  volatile uint32_t LCD_GRA_CUTHPXL;  ///< 0x1cc [R/W]: Panel Path Graphic Partial Display Horizontal Control Register
  volatile uint32_t LCD_GRA_CUTVLN;  ///< 0x1d0 [R/W]: Panel Path Graphic Partial Display Vertical Control Register
  volatile uint32_t reserved9[2];
  volatile uint32_t LCD_TOP_CTRL;  ///< 0x1dc [R/W]: LCD Global Control Register
  volatile uint32_t reserved10[2];
  volatile uint32_t LCD_AFA_ALL2ONE;  ///< 0x1e8 [R/W]: LCD Mixed Overlay Control Register
  volatile uint32_t reserved11[5];
  volatile uint32_t MISR_CONTROL;  ///< 0x200 [R/W]: MISR Testability Control Register
  volatile uint32_t MISR_STATUS;  ///< 0x204 [R]: MISR Testability Status Register
} LCD2_REGS_t;

#endif // LCD2
