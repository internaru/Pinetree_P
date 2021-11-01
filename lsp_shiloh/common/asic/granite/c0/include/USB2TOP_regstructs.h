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
 * \file USB2TOP_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _USB2TOP_REGSTRUCTS_H_
#define _USB2TOP_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: USB2TOP (USB2TOP)
//
//====================================================================

//====================================================================
//
//Register File: USB Device (USB2D)
//
//====================================================================

typedef struct USB2D_REGS_s
{
  volatile uint32_t ID;  ///< 0x0 [R]: Identification Register
  volatile uint32_t HWGENERAL;  ///< 0x4 [R]: ChipIdea General Hardware Parameters Register
  volatile uint32_t reserved0;
  volatile uint32_t HWDEVICE;  ///< 0xc [R]: Device Hardware Parameters Register
  volatile uint32_t HWTXBUF;  ///< 0x10 [R]: TX Buffer Parameters Register
  volatile uint32_t HWRXBUF;  ///< 0x14 [R]: RX Buffer Parameters Register
  volatile uint32_t reserved1[26];
  volatile uint32_t GPTIMER0LD;  ///< 0x80 [R/W]: Timer 0 Duration Register
  volatile uint32_t GPTIMER0CTRL;  ///< 0x84 [R/W]: Timer 0 Control Register
  volatile uint32_t GPTIMER1LD;  ///< 0x88 [R/W]: Timer 1 Duration Register
  volatile uint32_t GPTIMER1CTRL;  ///< 0x8c [R/W]: Timer 1 Control Register
  volatile uint32_t SBUSCFG;  ///< 0x90 [R/W]: System Bus Control Register
  volatile uint32_t reserved2[27];
  volatile uint32_t HCIV_CAPLENGTH;  ///< 0x100 [R]: EHCI Version / Capability Length Register
  volatile uint32_t reserved3[7];
  volatile uint32_t DCIVERSION;  ///< 0x120 [R]: Device Controller Version Register
  volatile uint32_t DCCPARAMS;  ///< 0x124 [R]: Device Capability Register
  volatile uint32_t reserved4[6];
  volatile uint32_t USBCMD;  ///< 0x140 [R/W]: USB Command Register
  volatile uint32_t USBSTS;  ///< 0x144 [R/W]: USB Status Register
  volatile uint32_t USBINTR;  ///< 0x148 [R/W]: USB Interrupt Register
  volatile uint32_t FRINDEX;  ///< 0x14c [R]: Frame Index Register
  volatile uint32_t reserved5;
  volatile uint32_t DEVICEADDR;  ///< 0x154 [R/W]: Device Address Register
  volatile uint32_t EPLISTADDR;  ///< 0x158 [R/W]: Endpoint List Address Register
  volatile uint32_t reserved6;
  volatile uint32_t BURSTSIZE;  ///< 0x160 [R/W]: Burst Size Register
  volatile uint32_t reserved7[5];
  volatile uint32_t ENDPTNAK;  ///< 0x178 [R/W]: Endpoint NAK Register
  volatile uint32_t ENDPTNAKEN;  ///< 0x17c [R/W]: Endpoint NAK Enable Register
  volatile uint32_t reserved8;
  volatile uint32_t PORTSC;  ///< 0x184 [R/W]: Port Status Control Register
  volatile uint32_t reserved9[8];
  volatile uint32_t USBMODE;  ///< 0x1a8 [R/W]: USB Device Mode Register
  volatile uint32_t ENDPTSETUPSTAT;  ///< 0x1ac [R/W]: Endpoint Setup Status Register
  volatile uint32_t ENDPTPRIME;  ///< 0x1b0 [R/W]: Endpoint Initialization Register
  volatile uint32_t ENDPTFLUSH;  ///< 0x1b4 [R/W]: Endpoint De-Initialize Register
  volatile uint32_t ENDPTSTAT;  ///< 0x1b8 [R]: Endpoint Status Register
  volatile uint32_t ENDPTCOMPLETE;  ///< 0x1bc [R/W]: Endpoint Complete Register
  volatile uint32_t ENDPTCTRL_0;  ///< 0x1c0 [R/W]: Endpoint Control 0 Register
  volatile uint32_t ENDPTCTRL1;  ///< 0x1c4 [R/W]: Endpoint Control Register (Instance 1 of 9)
  volatile uint32_t ENDPTCTRL2;  ///< 0x1c8 [R/W]: Endpoint Control Register (Instance 2 of 9)
  volatile uint32_t ENDPTCTRL3;  ///< 0x1cc [R/W]: Endpoint Control Register (Instance 3 of 9)
  volatile uint32_t ENDPTCTRL4;  ///< 0x1d0 [R/W]: Endpoint Control Register (Instance 4 of 9)
  volatile uint32_t ENDPTCTRL5;  ///< 0x1d4 [R/W]: Endpoint Control Register (Instance 5 of 9)
  volatile uint32_t ENDPTCTRL6;  ///< 0x1d8 [R/W]: Endpoint Control Register (Instance 6 of 9)
  volatile uint32_t ENDPTCTRL7;  ///< 0x1dc [R/W]: Endpoint Control Register (Instance 7 of 9)
  volatile uint32_t ENDPTCTRL8;  ///< 0x1e0 [R/W]: Endpoint Control Register (Instance 8 of 9)
  volatile uint32_t ENDPTCTRL9;  ///< 0x1e4 [R/W]: Endpoint Control Register (Instance 9 of 9)
} USB2D_REGS_t;

//====================================================================
//
//Register File: USB Host (USB2H)
//
//====================================================================

typedef struct USB2H_REGS_s
{
  volatile uint32_t ID;  ///< 0x0 [R]: Identification Register
  volatile uint32_t HWGENERAL;  ///< 0x4 [R]: ChipIdea General Hardware Parameters Register
  volatile uint32_t HWHOST;  ///< 0x8 [R]: Host Hardware Parameters Register
  volatile uint32_t reserved0;
  volatile uint32_t HWTXBUF;  ///< 0x10 [R]: TX Buffer Parameters Register
  volatile uint32_t HWRXBUF;  ///< 0x14 [R]: RX Buffer Parameters Register
  volatile uint32_t reserved1[26];
  volatile uint32_t GPTIMER0LD;  ///< 0x80 [R/W]: Timer 0 Duration Register
  volatile uint32_t GPTIMER0CTRL;  ///< 0x84 [R/W]: Timer 0 Control Register
  volatile uint32_t GPTIMER1LD;  ///< 0x88 [R/W]: Timer 1 Duration Register
  volatile uint32_t GPTIMER1CTRL;  ///< 0x8c [R/W]: Timer 1 Control Register
  volatile uint32_t SBUSCFG;  ///< 0x90 [R/W]: System Bus Control Register
  volatile uint32_t reserved2[27];
  volatile uint32_t HCIV_CAPLENGTH;  ///< 0x100 [R]: EHCI Version / Capability Length Register
  volatile uint32_t HCSPARAMS;  ///< 0x104 [R]: Host Control Structural Parameters Register
  volatile uint32_t HCCPARAMS;  ///< 0x108 [R]: Host Control Capability Parameters Register
  volatile uint32_t reserved3[13];
  volatile uint32_t USBCMD;  ///< 0x140 [R/W]: USB Command Register
  volatile uint32_t USBSTS;  ///< 0x144 [R/W]: USB Status Register
  volatile uint32_t USBINTR;  ///< 0x148 [R/W]: USB Interrupt Enable Register
  volatile uint32_t FRINDEX;  ///< 0x14c [R/W]: Frame Index Register
  volatile uint32_t reserved4;
  volatile uint32_t PLISTBASE;  ///< 0x154 [R/W]: Host Controller Frame List Base Address Register
  volatile uint32_t ASYNCLISTADDR;  ///< 0x158 [R/W]: Host Controller Next Asynch Address Register
  volatile uint32_t TTCTRL;  ///< 0x15c [R/W]: Host Controller Embedded TT Async Buffer Status Register
  volatile uint32_t BURSTSIZE;  ///< 0x160 [R/W]: Burst Size Register
  volatile uint32_t TXFILLTUNING;  ///< 0x164 [R/W]: TX Fill Tuning Register
  volatile uint32_t reserved5[6];
  volatile uint32_t CONFIGFLAG;  ///< 0x180 [R]: Config Flag Register
  volatile uint32_t PORTSC;  ///< 0x184 [R/W]: Port Status Control Register
  volatile uint32_t reserved6[8];
  volatile uint32_t USBMODE;  ///< 0x1a8 [R/W]: USB Device Mode Register
} USB2H_REGS_t;

//====================================================================
//
//Register File: USB OTG (USB2O)
//
//====================================================================

typedef struct USB2O_REGS_s
{
  volatile uint32_t ID;  ///< 0x0 [R]: Identification Register
  volatile uint32_t HWGENERAL;  ///< 0x4 [R]: ChipIdea General Hardware Parameters Register
  volatile uint32_t HWHOST;  ///< 0x8 [R]: Host Hardware Parameters Register
  volatile uint32_t HWDEVICE;  ///< 0xc [R]: Device Hardware Parameters Register
  volatile uint32_t HWTXBUF;  ///< 0x10 [R]: TX Buffer Parameters Register
  volatile uint32_t HWRXBUF;  ///< 0x14 [R]: RX Buffer Parameters Register
  volatile uint32_t reserved0[26];
  volatile uint32_t GPTIMER0LD;  ///< 0x80 [R/W]: Timer 0 Duration Register
  volatile uint32_t GPTIMER0CTRL;  ///< 0x84 [R/W]: Timer 0 Control Register
  volatile uint32_t GPTIMER1LD;  ///< 0x88 [R/W]: Timer 1 Duration Register
  volatile uint32_t GPTIMER1CTRL;  ///< 0x8c [R/W]: Timer 1 Control Register
  volatile uint32_t SBUSCFG;  ///< 0x90 [R/W]: System Bus Control Register
  volatile uint32_t reserved1[27];
  volatile uint32_t HCIV_CAPLENGTH;  ///< 0x100 [R]: EHCI Version / Capability Length Register
  volatile uint32_t HCSPARAMS;  ///< 0x104 [R]: Host Control Structural Parameters Register
  volatile uint32_t HCCPARAMS;  ///< 0x108 [R]: Host Control Capability Parameters Register
  volatile uint32_t reserved2[5];
  volatile uint32_t DCIVERSION;  ///< 0x120 [R]: Device Controller Version Register
  volatile uint32_t DCCPARAMS;  ///< 0x124 [R]: Device Capability Register
  volatile uint32_t reserved3[6];
  volatile uint32_t USBCMD;  ///< 0x140 [R/W]: USB Command Register
  volatile uint32_t USBSTS;  ///< 0x144 [R/W]: USB Status Register
  volatile uint32_t USBINTR;  ///< 0x148 [R/W]: USB Interrupt Enable Register
  volatile uint32_t FRINDEX;  ///< 0x14c [R/W]: Frame Index Register
  volatile uint32_t reserved4;
 union {
  volatile uint32_t DEVICEADDR;  ///< 0x154 [R/W]: Device Address Register
  volatile uint32_t PLISTBASE;  ///< 0x154 [R/W]: Host Controller Frame List Base Address Register
 } union0;
 union {
  volatile uint32_t EPLISTADDR;  ///< 0x158 [R/W]: Endpoint List Address Register
  volatile uint32_t ASYNCLISTADDR;  ///< 0x158 [R/W]: Host Controller Next Asynch Address Register
 } union1;
  volatile uint32_t TTCTRL;  ///< 0x15c [R/W]: Host Controller Embedded TT Async Buffer Status Register
  volatile uint32_t BURSTSIZE;  ///< 0x160 [R/W]: Burst Size Register
  volatile uint32_t TXFILLTUNING;  ///< 0x164 [R/W]: TX Fill Tuning Register
  volatile uint32_t reserved5[4];
  volatile uint32_t ENDPTNAK;  ///< 0x178 [R/W]: Endpoint NAK Register
  volatile uint32_t ENDPTNAKEN;  ///< 0x17c [R/W]: Endpoint NAK Enable Register
  volatile uint32_t CONFIGFLAG;  ///< 0x180 [R]: Config Flag Register
  volatile uint32_t PORTSC;  ///< 0x184 [R/W]: Port Status Control Register
  volatile uint32_t reserved6[8];
  volatile uint32_t USBMODE;  ///< 0x1a8 [R/W]: USB Device Mode Register
  volatile uint32_t ENDPTSETUPSTAT;  ///< 0x1ac [R/W]: Endpoint Setup Status Register
  volatile uint32_t ENDPTPRIME;  ///< 0x1b0 [R/W]: Endpoint Initialization Register
  volatile uint32_t ENDPTFLUSH;  ///< 0x1b4 [R/W]: Endpoint De-Initialize Register
  volatile uint32_t ENDPTSTAT;  ///< 0x1b8 [R]: Endpoint Status Register
  volatile uint32_t ENDPTCOMPLETE;  ///< 0x1bc [R/W]: Endpoint Complete Register
  volatile uint32_t ENDPTCTRL_0;  ///< 0x1c0 [R/W]: Endpoint Control 0 Register
  volatile uint32_t ENDPTCTRL1;  ///< 0x1c4 [R/W]: Endpoint Control Register (Instance 1 of 9)
  volatile uint32_t ENDPTCTRL2;  ///< 0x1c8 [R/W]: Endpoint Control Register (Instance 2 of 9)
  volatile uint32_t ENDPTCTRL3;  ///< 0x1cc [R/W]: Endpoint Control Register (Instance 3 of 9)
  volatile uint32_t ENDPTCTRL4;  ///< 0x1d0 [R/W]: Endpoint Control Register (Instance 4 of 9)
  volatile uint32_t ENDPTCTRL5;  ///< 0x1d4 [R/W]: Endpoint Control Register (Instance 5 of 9)
  volatile uint32_t ENDPTCTRL6;  ///< 0x1d8 [R/W]: Endpoint Control Register (Instance 6 of 9)
  volatile uint32_t ENDPTCTRL7;  ///< 0x1dc [R/W]: Endpoint Control Register (Instance 7 of 9)
  volatile uint32_t ENDPTCTRL8;  ///< 0x1e0 [R/W]: Endpoint Control Register (Instance 8 of 9)
  volatile uint32_t ENDPTCTRL9;  ///< 0x1e4 [R/W]: Endpoint Control Register (Instance 9 of 9)
} USB2O_REGS_t;

//====================================================================
//
//Register File: USB transceiver (USB2PHY)
//
//====================================================================

//====================================================================
//
//Register File: USB UTM PLL (USB2PHY_USB2_UTM_PLL)
//
//====================================================================

typedef struct USB2PHY_USB2_UTM_PLL_REGS_s
{
  volatile uint32_t PHY_ID;  ///< 0x0 [R]: PHY ID Register
  volatile uint32_t PLL0;  ///< 0x4 [R/W]: PLL Control Register 0
  volatile uint32_t PLL1;  ///< 0x8 [R/W]: PLL Control Register 1
  volatile uint32_t RESERVE;  ///< 0xc [R/W]: Reserve Register
  volatile uint32_t INT_EN;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t INT_STAT;  ///< 0x14 [R]: Interrupt Status Register
} USB2PHY_USB2_UTM_PLL_REGS_t;

//====================================================================
//
//Register File: USB UTM (Instance 1 of 3) (USB2PHY_USB2_UTM1)
//
//====================================================================

typedef struct USB2PHY_USB2_UTM1_REGS_s
{
  volatile uint32_t USB_CTL;  ///< 0x0 [R/W]: USB Control Register
  volatile uint32_t TX;  ///< 0x4 [R/W]: TX Channel Control Register
  volatile uint32_t RX0;  ///< 0x8 [R/W]: RX Channel Control Register 0
  volatile uint32_t RX1;  ///< 0xc [R/W]: Receive Channel Control Register 1
  volatile uint32_t IVREF;  ///< 0x10 [R/W]: PHY IVREF Control Register
  volatile uint32_t TEST0;  ///< 0x14 [R/W]: Test Group0 Register
  volatile uint32_t TEST1;  ///< 0x18 [R/W]: Test Group1 Register
  volatile uint32_t TEST2;  ///< 0x1c [R/W]: Test Group2 Register
  volatile uint32_t TEST3;  ///< 0x20 [R/W]: Test Group3 Register
  volatile uint32_t RESERVE;  ///< 0x24 [R/W]: Reserve Register
  volatile uint32_t ADDON;  ///< 0x28 [R/W]: Addon Register
  volatile uint32_t MON;  ///< 0x2c [R]: Monitor Register
} USB2PHY_USB2_UTM1_REGS_t;

//====================================================================
//
//Register File: USB UTM (Instance 2 of 3) (USB2PHY_USB2_UTM2)
//
//====================================================================

typedef struct USB2PHY_USB2_UTM2_REGS_s
{
  volatile uint32_t USB_CTL;  ///< 0x0 [R/W]: USB Control Register
  volatile uint32_t TX;  ///< 0x4 [R/W]: TX Channel Control Register
  volatile uint32_t RX0;  ///< 0x8 [R/W]: RX Channel Control Register 0
  volatile uint32_t RX1;  ///< 0xc [R/W]: Receive Channel Control Register 1
  volatile uint32_t IVREF;  ///< 0x10 [R/W]: PHY IVREF Control Register
  volatile uint32_t TEST0;  ///< 0x14 [R/W]: Test Group0 Register
  volatile uint32_t TEST1;  ///< 0x18 [R/W]: Test Group1 Register
  volatile uint32_t TEST2;  ///< 0x1c [R/W]: Test Group2 Register
  volatile uint32_t TEST3;  ///< 0x20 [R/W]: Test Group3 Register
  volatile uint32_t RESERVE;  ///< 0x24 [R/W]: Reserve Register
  volatile uint32_t ADDON;  ///< 0x28 [R/W]: Addon Register
  volatile uint32_t MON;  ///< 0x2c [R]: Monitor Register
} USB2PHY_USB2_UTM2_REGS_t;

//====================================================================
//
//Register File: USB UTM (Instance 3 of 3) (USB2PHY_USB2_UTM3)
//
//====================================================================

typedef struct USB2PHY_USB2_UTM3_REGS_s
{
  volatile uint32_t USB_CTL;  ///< 0x0 [R/W]: USB Control Register
  volatile uint32_t TX;  ///< 0x4 [R/W]: TX Channel Control Register
  volatile uint32_t RX0;  ///< 0x8 [R/W]: RX Channel Control Register 0
  volatile uint32_t RX1;  ///< 0xc [R/W]: Receive Channel Control Register 1
  volatile uint32_t IVREF;  ///< 0x10 [R/W]: PHY IVREF Control Register
  volatile uint32_t TEST0;  ///< 0x14 [R/W]: Test Group0 Register
  volatile uint32_t TEST1;  ///< 0x18 [R/W]: Test Group1 Register
  volatile uint32_t TEST2;  ///< 0x1c [R/W]: Test Group2 Register
  volatile uint32_t TEST3;  ///< 0x20 [R/W]: Test Group3 Register
  volatile uint32_t RESERVE;  ///< 0x24 [R/W]: Reserve Register
  volatile uint32_t ADDON;  ///< 0x28 [R/W]: Addon Register
  volatile uint32_t MON;  ///< 0x2c [R]: Monitor Register
} USB2PHY_USB2_UTM3_REGS_t;

#endif // USB2TOP
