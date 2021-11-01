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
 * \file LAN_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _LAN_REGSTRUCTS_H_
#define _LAN_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: LAN TOP Instantiation of Register Sets (LAN)
//
//====================================================================

//====================================================================
//
//Register File: LAN external-to-DWIP Registers (EXT)
/** \brief A handful of LAN external-to-DWIP registers exist namely: FTDLL, FTDLL_OBSV, DEL_TEST, SB. These registers are documented here.
    Also, if integrated, the Fast Ethernet PHY config and status registers, as well as a dual interface option with RGMII also.
    Note: the FTDLL, PHY and DUAL_IF registers are built by defualt, at build a -D 0=0, -D 0=0, and 0=0 will delete them.*/
//
//====================================================================

typedef struct EXT_REGS_s
{
  volatile uint32_t reserved0[3];
  volatile uint32_t SB;  ///< 0xc [R/W]: Sideband Register
} EXT_REGS_t;

//====================================================================
//
//Register File: LAN DWIP MAC Registers (MAC)
/** \brief TBD - more to add but this RegSet comment fields don't seem to be showing up.
    This section defines the bits for each MAC register.*/
//
//====================================================================

typedef struct MAC_REGS_s
{
  volatile uint32_t r0;  ///< 0x0 [R/W]: MAC Configuration Register
  volatile uint32_t r1;  ///< 0x4 [R/W]: MAC Frame Filter
  volatile uint32_t r2;  ///< 0x8 [R/W]: Hast Table High Register
  volatile uint32_t r3;  ///< 0xc [R/W]: Hash Table Low Register
  volatile uint32_t r4;  ///< 0x10 [R/W]: GMII Address Register
  volatile uint32_t r5;  ///< 0x14 [R/W]: GMII Data Register
  volatile uint32_t r6;  ///< 0x18 [R/W]: Flow Control Register
  volatile uint32_t r7;  ///< 0x1c [R/W]: VLAN Tag Register
  volatile uint32_t r8;  ///< 0x20 [R]: Version Register
  volatile uint32_t reserved0;
  volatile uint32_t r10;  ///< 0x28 [R/W]: Remote Wake-Up Frame Filter
  volatile uint32_t r11;  ///< 0x2c [R/W]: PMT Control and Status
  volatile uint32_t reserved1[2];
  volatile uint32_t r14;  ///< 0x38 [R]: Interrrupt Status Register
  volatile uint32_t r15;  ///< 0x3c [R/W]: Interrupt Mask Register
  volatile uint32_t AddrHi_MAC0;  ///< 0x40 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC0;  ///< 0x44 [R/W]: MAC Address Low Register (Instance 1 of 16)
  volatile uint32_t AddrHi_MAC1;  ///< 0x48 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC1;  ///< 0x4c [R/W]: MAC Address Low Register (Instance 2 of 16)
  volatile uint32_t AddrHi_MAC2;  ///< 0x50 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC2;  ///< 0x54 [R/W]: MAC Address Low Register (Instance 3 of 16)
  volatile uint32_t AddrHi_MAC3;  ///< 0x58 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC3;  ///< 0x5c [R/W]: MAC Address Low Register (Instance 4 of 16)
  volatile uint32_t AddrHi_MAC4;  ///< 0x60 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC4;  ///< 0x64 [R/W]: MAC Address Low Register (Instance 5 of 16)
  volatile uint32_t AddrHi_MAC5;  ///< 0x68 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC5;  ///< 0x6c [R/W]: MAC Address Low Register (Instance 6 of 16)
  volatile uint32_t AddrHi_MAC6;  ///< 0x70 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC6;  ///< 0x74 [R/W]: MAC Address Low Register (Instance 7 of 16)
  volatile uint32_t AddrHi_MAC7;  ///< 0x78 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC7;  ///< 0x7c [R/W]: MAC Address Low Register (Instance 8 of 16)
  volatile uint32_t AddrHi_MAC8;  ///< 0x80 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC8;  ///< 0x84 [R/W]: MAC Address Low Register (Instance 9 of 16)
  volatile uint32_t AddrHi_MAC9;  ///< 0x88 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC9;  ///< 0x8c [R/W]: MAC Address Low Register (Instance 10 of 16)
  volatile uint32_t AddrHi_MAC10;  ///< 0x90 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC10;  ///< 0x94 [R/W]: MAC Address Low Register (Instance 11 of 16)
  volatile uint32_t AddrHi_MAC11;  ///< 0x98 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC11;  ///< 0x9c [R/W]: MAC Address Low Register (Instance 12 of 16)
  volatile uint32_t AddrHi_MAC12;  ///< 0xa0 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC12;  ///< 0xa4 [R/W]: MAC Address Low Register (Instance 13 of 16)
  volatile uint32_t AddrHi_MAC13;  ///< 0xa8 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC13;  ///< 0xac [R/W]: MAC Address Low Register (Instance 14 of 16)
  volatile uint32_t AddrHi_MAC14;  ///< 0xb0 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC14;  ///< 0xb4 [R/W]: MAC Address Low Register (Instance 15 of 16)
  volatile uint32_t AddrHi_MAC15;  ///< 0xb8 [R/W]: MAC Address High Register
  volatile uint32_t AddrLo_MAC15;  ///< 0xbc [R/W]: MAC Address Low Register (Instance 16 of 16)
  volatile uint32_t reserved2[6];
  volatile uint32_t r54;  ///< 0xd8 [R]: SGMII/RGMII Status Register
} MAC_REGS_t;

//====================================================================
//
//Register File: LAN DWIP MMC Registers (MMC)
/** \brief This section defines the bits for each MMC register.
MMC address range is 0100 to 02cf
The counters in the MAC Management Counters (MMC) module can be viewed as an extension of the register address space of the CSR module. The MMC module maintains a set of registers for gathering statistics on the received and transmitted frames. These include a control register for controlling the behavior of the registers, two 32-bit registers containing interrupts generated (receive and transmit), and two 32-bit registers containing masks for the Interrupt register (receive and transmit). These registers are accessible from the Application through the MAC Control Interface (MCI). Each register is 32 bits wide. The write data is qualified with the corresponding mci_be_i signals. Thus, non-32 bit accesses are allowed as long as the address is word-aligned.
The MMCs are accessed using transactions, in the same way the CSR address space is accessed. The following sections in the chapter describe the various counters and list the address for each of the statistics counters. This address will be used for Read/Write accesses to the desired transmit/receive counter.
The Receive MMC counters are updated for frames that are passed by the Address Filter (AFM) block. Statistics of frames that are dropped by the AFM module are not updated unless they are runt frames of less than 6 bytes (DA bytes are not received fully).
The MMC module gathers statistics on encapsulated IPv4, IPv6, TCP, UDP, or ICMP payloads in received Ethernet frames. This gathering is only enabled when Full Checksum Offload Engine is selected during RTL configuration. The address map of the corresponding registers is 0x0200 to 0x02FC.
The MMC register naming convention is as follows.
"tx" as a prefix or suffix indicates counters associated with transmission
"rx" as a prefix or suffix indicates counters associated with reception
"_g" as a suffix indicates registers that count good frames only
"_gb" as a suffix indicates registers that count frames regardless of whether they are good or bad
Transmitted frames are considered "Bad" (and are thus aborted) if one or more of the following conditions exists.
Jabber Timeout
No Carrier/Loss of Carrier
Late Collision
Frame Underflow error
Received frames are considered "Bad" if one of the following conditions exists.
CRC error
Length error
Watchdog timeout
Missed frame error
Maximum frame size is dependent on the frame type, as follows.
Untagged frame maxsize = 1518
VLAN Frame maxsize = 1522
Jumbo Frame maxsize = 9018
JumboVLAN Frame maxsize = 9022*/
//
//====================================================================

typedef struct MMC_REGS_s
{
  volatile uint32_t reserved0[64];
  volatile uint32_t cntrl;  ///< 0x100 [R/W]: MMC Control Register
  volatile uint32_t intr_rx;  ///< 0x104 [R]: MMC Receive Interrupt Register
  volatile uint32_t intr_tx;  ///< 0x108 [R]: MMC Transmit Interrupt Register
  volatile uint32_t intr_mask_rx;  ///< 0x10c [R/W]: MMC Receive Interrupt Mask Register
  volatile uint32_t intr_mask_tx;  ///< 0x110 [R/W]: MMC Transmit Interrupt Mask Register
  volatile uint32_t txoctetcount_gb;  ///< 0x114 [R]: Transmit Byte Count Register
  volatile uint32_t txframecount_gb;  ///< 0x118 [R]: Transmit Frame Count Register
  volatile uint32_t txbroadcastframes_g;  ///< 0x11c [R]: Good Broadcast Transmit Frame Count Register
  volatile uint32_t txmulticastframes_g;  ///< 0x120 [R]: Good Multicast Transmit Frame Count Register
  volatile uint32_t tx64octets_gb;  ///< 0x124 [R]: 64 byte Transmit Frame Count Register
  volatile uint32_t tx65to127octets_gb;  ///< 0x128 [R]: 65 to 127 Byte Transmit Frame Count Register
  volatile uint32_t tx128to255octets_gb;  ///< 0x12c [R]: 128 to 255 Byte Transmit Frame Count Register
  volatile uint32_t tx256to511octets_gb;  ///< 0x130 [R]: 256 to 511 Byte Transmit Frame Count Register
  volatile uint32_t tx512to1023octets_gb;  ///< 0x134 [R]: 512 to 1023 Byte Transmit Frame Count Register
  volatile uint32_t tx1024tomaxoctets_gb;  ///< 0x138 [R]: 1024 to Max Byte Transmit Frame Count Register
  volatile uint32_t txunicastframes_gb;  ///< 0x13c [R]: Unicast Transmit Frame Count Register
  volatile uint32_t txmulticastframes_gb;  ///< 0x140 [R]: Multicast Transmit Frame Count Register
  volatile uint32_t txbroadcastframes_gb;  ///< 0x144 [R]: Broadcast Transmit Frame Count
  volatile uint32_t txunderflowerror;  ///< 0x148 [R]: Aborted Underflow Transmit Frame Count Register
  volatile uint32_t txsinglecol_g;  ///< 0x14c [R]: Single Collision Successful Transmit Frame Count Register
  volatile uint32_t txmulticol_g;  ///< 0x150 [R]: Multi-collision Successful Transmit Frame Count Register
  volatile uint32_t txdeferred;  ///< 0x154 [R]: Deferred Successful Half Duplex Transmit Frame Count Register
  volatile uint32_t txlatecol;  ///< 0x158 [R]: Late Collision Aborted Transmit Frame Count Register
  volatile uint32_t txexesscol;  ///< 0x15c [R]: Excessive Collision Aborted Transmit Frame Count Register
  volatile uint32_t txcarriererror;  ///< 0x160 [R]: Carrier Sense Error Aborted Transmit Frame Count Register
  volatile uint32_t txoctetcount_g;  ///< 0x164 [R]: Good Transmit Frame Byte Count Register
  volatile uint32_t txframecount_g;  ///< 0x168 [R]: Good Transmit Frame Count Register
  volatile uint32_t txexcessdef;  ///< 0x16c [R]: Excessive Deferral Error Aborted Transmit Frame Count Register
  volatile uint32_t txpauseframes;  ///< 0x170 [R]: Good PAUSE Frame Trasmit Count Register
  volatile uint32_t txvlanframes_g;  ///< 0x174 [R]: Good Transmit VLAN Frame Count Register
  volatile uint32_t reserved1[2];
  volatile uint32_t rxframecount_gb;  ///< 0x180 [R]: Receive Frame Count Register
  volatile uint32_t rxoctetcount_gb;  ///< 0x184 [R]: Receive Byte Count Register
  volatile uint32_t rxoctetcount_g;  ///< 0x188 [R]: Good Receive Frame Count Register
  volatile uint32_t rxbroadcastframes_g;  ///< 0x18c [R]: Good Broadcast Receive Frame Count Register
  volatile uint32_t rxmulticastframes_g;  ///< 0x190 [R]: Good Multicast Receive Frame Count Register
  volatile uint32_t rxcrcerror;  ///< 0x194 [R]: CRC Error Receive Frame Count Register
  volatile uint32_t rxalignmenterror;  ///< 0x198 [R]: Alignment Error Receive Frame Count Register
  volatile uint32_t rxrunterror;  ///< 0x19c [R]: Runt Error Receive Frame Count Register
  volatile uint32_t rxjabbererror;  ///< 0x1a0 [R]: CRC Error Giant Receive Frame Count Register
  volatile uint32_t rxundersize_g;  ///< 0x1a4 [R]: Undersized Receive Frame Count Register
  volatile uint32_t rxoversize_g;  ///< 0x1a8 [R]: Oversized Receive Frame Count Register
  volatile uint32_t rx64octets_gb;  ///< 0x1ac [R]: 64 Byte Receive Frame Count Register
  volatile uint32_t rx65to127octets_gb;  ///< 0x1b0 [R]: 65 to 127 Byte Receive Frame Count Register
  volatile uint32_t rx128to255octets_gb;  ///< 0x1b4 [R]: 128 to 255 Byte Receive Frame Count Register
  volatile uint32_t rx256to511octets_gb;  ///< 0x1b8 [R]: 256 to 511 Byte Receive Frame Count Register
  volatile uint32_t rx512to1023octets_gb;  ///< 0x1bc [R]: 512 to 1023 Byte Receive Frame Count Register
  volatile uint32_t rx1024tomaxoctets_gb;  ///< 0x1c0 [R]: 1024 to Max Byte Receive Frame Count Register
  volatile uint32_t rxunicastframes_g;  ///< 0x1c4 [R]: Good Unicast Receive Frame Count Register
  volatile uint32_t rxlengtherror;  ///< 0x1c8 [R]: Length Field Error Receive Frame Count Register
  volatile uint32_t rxoutofrangetype;  ///< 0x1cc [R]: 1501 to 1535 Length Mismatch Receive Frame Count Register
  volatile uint32_t rxpauseframes;  ///< 0x1d0 [R]: Good Valid PAUSE Receive Frame Count Register
  volatile uint32_t rxfifooverflow;  ///< 0x1d4 [R]: FIFO Overflow Missed Receive Frame Count Register
  volatile uint32_t rxvlanframes_gb;  ///< 0x1d8 [R]: VLAN Receive Frame Count Register
  volatile uint32_t rxwatchdogerror;  ///< 0x1dc [R]: Watchdog Timeout Error Receive Frame Count Register
  volatile uint32_t reserved2[8];
  volatile uint32_t ipc_intr_mask_rx;  ///< 0x200 [R/W]: IPC Receive Checksum Offload Interrupt Mask Register
  volatile uint32_t reserved3;
  volatile uint32_t ipc_intr_rx;  ///< 0x208 [R/W]: IPC Receive Checksum Offload Interrupt Register
  volatile uint32_t reserved4;
  volatile uint32_t rxipv4_gd_frms;  ///< 0x210 [R/W]: Good IPv4 Receive Datagram Count Register
  volatile uint32_t rxipv4_hdrerr_frms;  ///< 0x214 [R/W]: Header Error IPv4 Receive Datagram Count Register
  volatile uint32_t rxipv4_nopay_frms;  ///< 0x218 [R/W]: Non-TCP, UDP, ICMP payload IPv4 Receive Datagram Count Register
  volatile uint32_t rxipv4_frag_frms;  ///< 0x21c [R/W]: Fragmented IPv4 Receive Datagram Count Register
  volatile uint32_t rxipv4_udsbl_frms;  ///< 0x220 [R/W]: Disabled Checksum UDP Payload IPv4 Receive Datagram Count Register
  volatile uint32_t rxipv6_gd_frms;  ///< 0x224 [R/W]: Good TCP, UDP, or ICMP payload IPv6 Receive Datagram Count Register
  volatile uint32_t rxipv6_hdrerr_frms;  ///< 0x228 [R/W]: Header Error IPv6 Receiv Datagram Count Register
  volatile uint32_t rxipv6_nopay_frms;  ///< 0x22c [R/W]: Non-TCP, UDP or ICMP payload IPv6 Receive Datagram Count Register
  volatile uint32_t rxudp_gd_frms;  ///< 0x230 [R/W]: Good UDP Payload IP Receive Datagram Count Register
  volatile uint32_t rxudp_err_frms;  ///< 0x234 [R/W]: Checksum Error UDP Payload IP Receive Datagram Count Register
  volatile uint32_t rxtcp_gd_frms;  ///< 0x238 [R/W]: Good TCP Payload IP Receieve Datagram Count Register
  volatile uint32_t rxtcp_err_frms;  ///< 0x23c [R/W]: Checksum Error TCP Payload IP Receieve Datagram Count Register
  volatile uint32_t rxicmp_gd_frms;  ///< 0x240 [R/W]: Good ICMP Payload IP Receive Datagram Count Register
  volatile uint32_t rxicmp_err_frms;  ///< 0x244 [R/W]: Checksum Error ICMP Payload IP Receive Datagram Count Register
  volatile uint32_t reserved5[2];
  volatile uint32_t rxipv4_gd_octets;  ///< 0x250 [R/W]: Good TCP, UDP or ICMP (IPv4) Data Byte Count Register
  volatile uint32_t rxipv4_hdrerr_octets;  ///< 0x254 [R/W]: Header Error IPv4 Receive Datagram Byte Count Register
  volatile uint32_t rxipv4_nopay_octets;  ///< 0x258 [R/W]: Non-TCP, UDP, or ICMP Payload IPv4 Receive Datagram Accumulative Length Byte Count Register
  volatile uint32_t rxipv4_frag_octets;  ///< 0x25c [R/W]: Fragmented IPv4 Receive Datagram Accumulative Length Byte Count Register
  volatile uint32_t rxipv4_udsbl_octets;  ///< 0x260 [R/W]: UDP Checksum Disable Receive Byte Count Register
  volatile uint32_t rxipv6_gd_octets;  ///< 0x264 [R/W]: Good TCP, UDP or ICMPv6 IPv6 Receive Datagram Byte Count Register
  volatile uint32_t rxipv6_hdrerr_octets;  ///< 0x268 [R/W]: Header Error IPv6 Receive Datagram Byte Count Register
  volatile uint32_t rxipv6_nopay_octets;  ///< 0x26c [R/W]: Non-TCP, UDP or ICMP Payload IPv6 Receive Datagram Accumulative Length Count Register
  volatile uint32_t rxudp_gd_octets;  ///< 0x270 [R/W]: UDP Receive Byte Count Register
  volatile uint32_t rxudp_err_octets;  ///< 0x274 [R/W]: Checksum Error UDP Receive Byte Count Register
  volatile uint32_t rxtcp_gd_octets;  ///< 0x278 [R/W]: TCP Receive Byte Count Register
  volatile uint32_t rxtcp_err_octets;  ///< 0x27c [R/W]: Checksum Error TCP Receive Byte Count Register
  volatile uint32_t rxicmp_gd_octets;  ///< 0x280 [R/W]: ICMP Receive Byte Count Register
  volatile uint32_t rxicmp_err_octets;  ///< 0x284 [R/W]: Checksum Error ICMP Receive Byte Count Register
} MMC_REGS_t;

//====================================================================
//
//Register File: LAN DWIP MAC 16 plus Registers (MAC_16plus)
/** \brief This section defines the bits for additional MAC Address registers beyond 16.*/
//
//====================================================================

typedef struct MAC_16PLUS_REGS_s
{
  volatile uint32_t reserved0[512];
  volatile uint32_t AddrHi_MAC16;  ///< 0x800 [R/W]: MAC Address High Register (Instance 1 of 8)
  volatile uint32_t AddrLo_MAC16;  ///< 0x804 [R/W]: MAC Address Low Register (Instance 1 of 8)
  volatile uint32_t AddrHi_MAC17;  ///< 0x808 [R/W]: MAC Address High Register (Instance 2 of 8)
  volatile uint32_t AddrLo_MAC17;  ///< 0x80c [R/W]: MAC Address Low Register (Instance 2 of 8)
  volatile uint32_t AddrHi_MAC18;  ///< 0x810 [R/W]: MAC Address High Register (Instance 3 of 8)
  volatile uint32_t AddrLo_MAC18;  ///< 0x814 [R/W]: MAC Address Low Register (Instance 3 of 8)
  volatile uint32_t AddrHi_MAC19;  ///< 0x818 [R/W]: MAC Address High Register (Instance 4 of 8)
  volatile uint32_t AddrLo_MAC19;  ///< 0x81c [R/W]: MAC Address Low Register (Instance 4 of 8)
  volatile uint32_t AddrHi_MAC20;  ///< 0x820 [R/W]: MAC Address High Register (Instance 5 of 8)
  volatile uint32_t AddrLo_MAC20;  ///< 0x824 [R/W]: MAC Address Low Register (Instance 5 of 8)
  volatile uint32_t AddrHi_MAC21;  ///< 0x828 [R/W]: MAC Address High Register (Instance 6 of 8)
  volatile uint32_t AddrLo_MAC21;  ///< 0x82c [R/W]: MAC Address Low Register (Instance 6 of 8)
  volatile uint32_t AddrHi_MAC22;  ///< 0x830 [R/W]: MAC Address High Register (Instance 7 of 8)
  volatile uint32_t AddrLo_MAC22;  ///< 0x834 [R/W]: MAC Address Low Register (Instance 7 of 8)
  volatile uint32_t AddrHi_MAC23;  ///< 0x838 [R/W]: MAC Address High Register (Instance 8 of 8)
  volatile uint32_t AddrLo_MAC23;  ///< 0x83c [R/W]: MAC Address Low Register (Instance 8 of 8)
} MAC_16PLUS_REGS_t;

//====================================================================
//
//Register File: LAN DWIP DMA Registers (MAD)
/** \brief This section defines the bits for each DMA register.*/
//
//====================================================================

typedef struct MAD_REGS_s
{
  volatile uint32_t reserved0[1024];
  volatile uint32_t r0;  ///< 0x1000 [R/W]: Bus Mode Register
  volatile uint32_t r1;  ///< 0x1004 [R]: Transmit Poll Demand Register
  volatile uint32_t r2;  ///< 0x1008 [R]: Recieve Poll Demand Register
  volatile uint32_t r3;  ///< 0x100c [R/W]: Receive Descriptor List Address Register
  volatile uint32_t r4;  ///< 0x1010 [R/W]: Transmit Descriptor List Address Register
  volatile uint32_t r5;  ///< 0x1014 [R/W]: Status Register
  volatile uint32_t r6;  ///< 0x1018 [R/W]: Operation Mode Register
  volatile uint32_t r7;  ///< 0x101c [R/W]: Interrupt Enable Register
  volatile uint32_t r8;  ///< 0x1020 [R]: Missed Frame and Buffer Overflow Counter Register
  volatile uint32_t reserved1[9];
  volatile uint32_t r18;  ///< 0x1048 [R]: Current Host Transmit Descriptor Register
  volatile uint32_t r19;  ///< 0x104c [R]: Current Host Receive Descriptor Register
  volatile uint32_t r20;  ///< 0x1050 [R]: Current Host Transmit Buffer Address Register
  volatile uint32_t r21;  ///< 0x1054 [R]: Current Host Receive Buffer Address Register
} MAD_REGS_t;

#endif // LAN
