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
 * \file PCIE_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PCIE_REGSTRUCTS_H_
#define _PCIE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PCIE (PCIE)
/** \brief Top-level register file for PCIE*/
//
//====================================================================

//====================================================================
//
//Register File: PCIE Dual Mode Core (PCIE_DM)
/** \brief Top-level register file for PCIE_DM*/
//
//====================================================================

typedef struct PCIE_DM_REGS_s
{
  volatile uint32_t TCfg00;  ///< 0x0 [R/W]: DM Core Target Configuration Register 00
  volatile uint32_t TCfg04;  ///< 0x4 [R/W]: DM Core Target Configuration Register 04
  volatile uint32_t TCfg08;  ///< 0x8 [R/W]: DM Core Target Configuration Register 08
  volatile uint32_t TCfg0C;  ///< 0xc [R/W]: DM Core Target Configuration Register 0C
  volatile uint32_t TCfg10;  ///< 0x10 [R/W]: DM Core Target Configuration Register 10 is BAR0
  volatile uint32_t TCfg14;  ///< 0x14 [R/W]: DM Core Target Configuration Register 14 is BAR1
  volatile uint32_t reserved0[5];
  volatile uint32_t TCfg2C;  ///< 0x2c [R/W]: DM Core Target Configuration Register 2C, Subsystem ID and Subsystem Vendor ID
  volatile uint32_t reserved1;
  volatile uint32_t TCfg34;  ///< 0x34 [R/W]: DM Core Target Configuration Register 34
  volatile uint32_t reserved2;
  volatile uint32_t TCfg3C;  ///< 0x3c [R/W]: DM Core Target Configuration Register 3C, Interrupt Pin  and Interrupt Line
  volatile uint32_t TPMCap;  ///< 0x40 [R/W]: DM Core Target Power Management Capabilities Register
  volatile uint32_t TPMCSR;  ///< 0x44 [R/W]: DM Core Target Power Management Control and Status Register
  volatile uint32_t reserved3[2];
  volatile uint32_t TMsiCtrl;  ///< 0x50 [R/W]: DM Core Target MSI Capabilities Control Register
  volatile uint32_t TMsiAddr;  ///< 0x54 [R/W]: DM Core Target MSI Capabilities  Address Register
  volatile uint32_t reserved4;
  volatile uint32_t TMsiData;  ///< 0x5c [R/W]: DM Core Target MSI Capabilities Data  Register
  volatile uint32_t reserved5[4];
  volatile uint32_t TPCIECap;  ///< 0x70 [R/W]: DM Core Target PCIE Capabilities Register
  volatile uint32_t TDevCap;  ///< 0x74 [R/W]: DM Core Target Device Capability Register
  volatile uint32_t TDevContStatus;  ///< 0x78 [R/W]: DM Core Target Device Control and Status Register
  volatile uint32_t TDevLinkCap;  ///< 0x7c [R/W]: DM Core Target Device Link Capabilities Register
  volatile uint32_t TLinkContStatus;  ///< 0x80 [R/W]: DM Core Target Link Control and Status Register
  volatile uint32_t reserved6[4];
  volatile uint32_t TDevCap2;  ///< 0x94 [R/W]: DM Core Target Device Capability2 Register
  volatile uint32_t TDevContStatus2;  ///< 0x98 [R/W]: DM Core Target Device Control and Status2 Register
  volatile uint32_t TDevLinkCap2;  ///< 0x9c [R/W]: DM Core Target Device Link Capabilities2 Register
  volatile uint32_t TLinkContStatus2;  ///< 0xa0 [R/W]: DM Core Target Link Control and Status2 Register
  volatile uint32_t reserved7[23];
  volatile uint32_t TAERCap;  ///< 0x100 [R/W]: DM Core Target AER Capabilities Register
  volatile uint32_t TUCorrStat;  ///< 0x104 [R/W]: DM Core Target Uncorrectable Error Status Register
  volatile uint32_t TUCorrMask;  ///< 0x108 [R/W]: DM Core Target Uncorrectable Error Mask Register
  volatile uint32_t TUCorrSev;  ///< 0x10c [R/W]: DM Core Target Uncorrectable Error Severity Register
  volatile uint32_t TCorrStat;  ///< 0x110 [R/W]: DM Core Target Correctable Error Status Register
  volatile uint32_t TCorrMask;  ///< 0x114 [R/W]: DM Core Target Correctable Error Mask Register
  volatile uint32_t TAERCapCtrl;  ///< 0x118 [R/W]: DM Core Target AER Capabilities and Control Register
  volatile uint32_t reserved8[8121];
  volatile uint32_t LCfg00;  ///< 0x8000 [R/W]: DM Core Local Configuration Register 00
  volatile uint32_t LCfg04;  ///< 0x8004 [R/W]: DM Core Local Configuration Register 04
  volatile uint32_t LCfg08;  ///< 0x8008 [R/W]: DM Core Local Configuration Register 08
  volatile uint32_t LCfg0C;  ///< 0x800c [R/W]: DM Core Local Configuration Register 0C
  volatile uint32_t LCfg10;  ///< 0x8010 [R/W]: DM Core Local Configuration Register 10 is BAR0
  volatile uint32_t LCfg14;  ///< 0x8014 [R/W]: DM Core Local Configuration Register 14 is BAR1
  volatile uint32_t reserved9[3];
  volatile uint32_t LCfg24;  ///< 0x8024 [R/W]: Prefetchable Memory Base and Limit Register
  volatile uint32_t reserved10;
  volatile uint32_t LCfg2C;  ///< 0x802c [R/W]: DM Core Local Configuration Register 2C, Subsystem ID and Subsystem Vendor ID
  volatile uint32_t reserved11;
  volatile uint32_t LCfg34;  ///< 0x8034 [R/W]: DM Core Local Configuration Register 34
  volatile uint32_t reserved12;
  volatile uint32_t LCfg3C;  ///< 0x803c [R/W]: DM Core Local Configuration Register 3C, Interrupt Pin  and Interrupt Line
  volatile uint32_t LPMCap;  ///< 0x8040 [R/W]: DM Core Local Power Management Capabilities Register
  volatile uint32_t LPMCSR;  ///< 0x8044 [R/W]: DM Core Local Power Management Control and Status Register
  volatile uint32_t reserved13[2];
  volatile uint32_t LMsiCtrl;  ///< 0x8050 [R/W]: DM Core Local MSI Capabilities Control Register
  volatile uint32_t LMsiAddr;  ///< 0x8054 [R/W]: DM Core Local MSI Capabilities  Address Register
  volatile uint32_t reserved14;
  volatile uint32_t LMsiData;  ///< 0x805c [R/W]: DM Core Local MSI Capabilities Data  Register
  volatile uint32_t reserved15[4];
  volatile uint32_t LPCIECap;  ///< 0x8070 [R/W]: DM Core Local PCIE Capabilities Register
  volatile uint32_t LDevCap;  ///< 0x8074 [R/W]: DM Core Local Device Capability Register
  volatile uint32_t LDevContStatus;  ///< 0x8078 [R/W]: DM Core Local Device Control and Status Register
  volatile uint32_t LDevLinkCap;  ///< 0x807c [R/W]: DM Core Local Device Link Capabilities Register
  volatile uint32_t LLinkContStatus;  ///< 0x8080 [R/W]: DM Core Local Link Control and Status Register
  volatile uint32_t reserved16[2];
  volatile uint32_t LRootCtrl;  ///< 0x808c [R/W]: DM Core Local Root Control Register
  volatile uint32_t LRootStat;  ///< 0x8090 [R/W]: DM Core Local Root Status Register
  volatile uint32_t LDevCap2;  ///< 0x8094 [R/W]: DM Core Local Device Capability2 Register
  volatile uint32_t LDevContStatus2;  ///< 0x8098 [R/W]: DM Core Local Device Control and Status2 Register
  volatile uint32_t LDevLinkCap2;  ///< 0x809c [R/W]: DM Core Local Device Link Capabilities2 Register
  volatile uint32_t LLinkContStatus2;  ///< 0x80a0 [R/W]: DM Core Local Link Control and Status2 Register
  volatile uint32_t reserved17[23];
  volatile uint32_t LAERCap;  ///< 0x8100 [R/W]: DM Core Local AER Capabilities Register
  volatile uint32_t LUCorrStat;  ///< 0x8104 [R/W]: DM Core Local Uncorrectable Error Status Register
  volatile uint32_t LUCorrMask;  ///< 0x8108 [R/W]: DM Core Local Uncorrectable Error Mask Register
  volatile uint32_t LUCorrSev;  ///< 0x810c [R/W]: DM Core Local Uncorrectable Error Severity Register
  volatile uint32_t LCorrStat;  ///< 0x8110 [R/W]: DM Core Local Correctable Error Status Register
  volatile uint32_t LCorrMask;  ///< 0x8114 [R/W]: DM Core Local Correctable Error Mask Register
  volatile uint32_t LAERCapCtrl;  ///< 0x8118 [R/W]: DM Core Local AER Capabilities and Control Register
  volatile uint32_t reserved18[4];
  volatile uint32_t LRootErrCmd;  ///< 0x812c [R/W]: DM Core Local Root Error Command Register
  volatile uint32_t LRootErrStat;  ///< 0x8130 [R/W]: DM Core Local Root Error Status Register
  volatile uint32_t LErrSrcID;  ///< 0x8134 [R]: DM Core Local Error Source Identification Register
  volatile uint32_t reserved19[370];
  volatile uint32_t LPortTimer;  ///< 0x8700 [R/W]: DM Core Local Port Logic Ack Latency and Replay Timer Register
  volatile uint32_t LMsgReg;  ///< 0x8704 [R/W]: DM Core Local Port Logic Message Register
  volatile uint32_t LForceLink;  ///< 0x8708 [R/W]: DM Core Local Port Logic Force Link Register
  volatile uint32_t LAckFreq;  ///< 0x870c [R/W]: DM Core Local Port Logic Ack Frequency Register
  volatile uint32_t LPortLink;  ///< 0x8710 [R/W]: DM Core Local Port Link Control Register
  volatile uint32_t reserved20[575];
  volatile uint32_t LCfg10M;  ///< 0x9010 [W]: DM Core Local Configuration BAR0 Mask Register
  volatile uint32_t LCfg14M;  ///< 0x9014 [W]: DM Core Local Configuration BAR1 Mask Register
  volatile uint32_t reserved21[7162];
  volatile uint32_t ACtrl;  ///< 0x10000 [R/W]: DM Application Control Register
  volatile uint32_t AStat;  ///< 0x10004 [R]: DM Application Status Register
  volatile uint32_t AIntP;  ///< 0x10008 [R]: DM Application Interrupt Pending Register
  volatile uint32_t AIntEn;  ///< 0x1000c [R/W]: DM Application Interrupt Enable Register
  volatile uint32_t AIntAck;  ///< 0x10010 [W]: DM Application Interrupt Acknowledge Register
  volatile uint32_t InAddrXlt0;  ///< 0x10014 [R/W]: DM Application Inbound Address Translation Register 0
  volatile uint32_t InAddrXlt1;  ///< 0x10018 [R/W]: DM Application Inbound Address Translation Register 1
  volatile uint32_t InAddrXlt2;  ///< 0x1001c [R/W]: DM Application Inbound Address Translation Register 2
  volatile uint32_t BAR0;  ///< 0x10020 [R/W]: DM Application BAR0
  volatile uint32_t WinMiss;  ///< 0x10024 [R/W]: DM Application Window Miss Register
  volatile uint32_t RC_MSI_IRQ0_Mask;  ///< 0x10028 [R/W]: DM Application RC MSI Interrupt0 Mask 
  volatile uint32_t RC_MSI_IRQ0;  ///< 0x1002c [R/W]: DM Application RC MSI Interrupt0
  volatile uint32_t RC_MSI_IRQ0_Clr;  ///< 0x10030 [W]: DM Application RC MSI Interrupt0 Clear
  volatile uint32_t RC_MSI_IRQ1_Mask;  ///< 0x10034 [R/W]: DM Application RC MSI Interrupt1 Mask 
  volatile uint32_t RC_MSI_IRQ1;  ///< 0x10038 [R/W]: DM Application RC MSI Interrupt1
  volatile uint32_t RC_MSI_IRQ1_Clr;  ///< 0x1003c [W]: DM Application RC MSI Interrupt1 Clear
  volatile uint32_t RC_MSI_IRQ2_Mask;  ///< 0x10040 [R/W]: DM Application RC MSI Interrupt2 Mask 
  volatile uint32_t RC_MSI_IRQ2;  ///< 0x10044 [R/W]: DM Application RC MSI Interrupt2
  volatile uint32_t RC_MSI_IRQ2_Clr;  ///< 0x10048 [W]: DM Application RC MSI Interrupt2 Clear
  volatile uint32_t RC_MSI_IRQ3_Mask;  ///< 0x1004c [R/W]: DM Application RC MSI Interrupt3 Mask 
  volatile uint32_t RC_MSI_IRQ3;  ///< 0x10050 [R/W]: DM Application RC MSI Interrupt3
  volatile uint32_t RC_MSI_IRQ3_Clr;  ///< 0x10054 [W]: DM Application RC MSI Interrupt3 Clear
  volatile uint32_t RC_INTA_IRQ_En;  ///< 0x10058 [R/W]: DM Application INTA Interrupt Enable
  volatile uint32_t RC_INTA_IRQ_Cnt;  ///< 0x1005c [R]: DM Application INTA Count Value
  volatile uint32_t RC_INTB_IRQ_En;  ///< 0x10060 [R/W]: DM Application INTB Interrupt Enable
  volatile uint32_t RC_INTB_IRQ_Cnt;  ///< 0x10064 [R]: DM Application INTB Count Value
  volatile uint32_t RC_INTC_IRQ_En;  ///< 0x10068 [R/W]: DM Application INTC Interrupt Enable
  volatile uint32_t RC_INTC_IRQ_Cnt;  ///< 0x1006c [R]: DM Application INTC Count Value
  volatile uint32_t RC_INTD_IRQ_En;  ///< 0x10070 [R/W]: DM Application INTD Interrupt Enable
  volatile uint32_t RC_INTD_IRQ_Cnt;  ///< 0x10074 [R]: DM Application INTD Count Value
  volatile uint32_t EP_INTA_Gen;  ///< 0x10078 [R/W]: DM Application - EP INTA Generate
  volatile uint32_t EP_MSI_Gen;  ///< 0x1007c [R/W]: DM Application - EP MSI Generate
  volatile uint32_t EP_MSI_Gen_En;  ///< 0x10080 [R]: DM Application - EP MSI Generate Enabled
  volatile uint32_t MB_WR_En0;  ///< 0x10084 [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 1 of 8)
  volatile uint32_t MB_WR_En1;  ///< 0x10088 [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 2 of 8)
  volatile uint32_t MB_WR_En2;  ///< 0x1008c [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 3 of 8)
  volatile uint32_t MB_WR_En3;  ///< 0x10090 [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 4 of 8)
  volatile uint32_t MB_WR_En4;  ///< 0x10094 [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 5 of 8)
  volatile uint32_t MB_WR_En5;  ///< 0x10098 [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 6 of 8)
  volatile uint32_t MB_WR_En6;  ///< 0x1009c [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 7 of 8)
  volatile uint32_t MB_WR_En7;  ///< 0x100a0 [R/W]: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 8 of 8)
  volatile uint32_t MB_WR0;  ///< 0x100a4 [W]: DM Application - Mailbox Write Register 0-7 (Instance 1 of 8)
  volatile uint32_t MB_WR1;  ///< 0x100a8 [W]: DM Application - Mailbox Write Register 0-7 (Instance 2 of 8)
  volatile uint32_t MB_WR2;  ///< 0x100ac [W]: DM Application - Mailbox Write Register 0-7 (Instance 3 of 8)
  volatile uint32_t MB_WR3;  ///< 0x100b0 [W]: DM Application - Mailbox Write Register 0-7 (Instance 4 of 8)
  volatile uint32_t MB_WR4;  ///< 0x100b4 [W]: DM Application - Mailbox Write Register 0-7 (Instance 5 of 8)
  volatile uint32_t MB_WR5;  ///< 0x100b8 [W]: DM Application - Mailbox Write Register 0-7 (Instance 6 of 8)
  volatile uint32_t MB_WR6;  ///< 0x100bc [W]: DM Application - Mailbox Write Register 0-7 (Instance 7 of 8)
  volatile uint32_t MB_WR7;  ///< 0x100c0 [W]: DM Application - Mailbox Write Register 0-7 (Instance 8 of 8)
  volatile uint32_t MB_WR_Stat0;  ///< 0x100c4 [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 1 of 8)
  volatile uint32_t MB_WR_Stat1;  ///< 0x100c8 [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 2 of 8)
  volatile uint32_t MB_WR_Stat2;  ///< 0x100cc [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 3 of 8)
  volatile uint32_t MB_WR_Stat3;  ///< 0x100d0 [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 4 of 8)
  volatile uint32_t MB_WR_Stat4;  ///< 0x100d4 [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 5 of 8)
  volatile uint32_t MB_WR_Stat5;  ///< 0x100d8 [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 6 of 8)
  volatile uint32_t MB_WR_Stat6;  ///< 0x100dc [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 7 of 8)
  volatile uint32_t MB_WR_Stat7;  ///< 0x100e0 [R]: DM Application - Mailbox Write Register Status 0-7 (Instance 8 of 8)
  volatile uint32_t MB_WR_Ack0;  ///< 0x100e4 [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 1 of 8)
  volatile uint32_t MB_WR_Ack1;  ///< 0x100e8 [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 2 of 8)
  volatile uint32_t MB_WR_Ack2;  ///< 0x100ec [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 3 of 8)
  volatile uint32_t MB_WR_Ack3;  ///< 0x100f0 [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 4 of 8)
  volatile uint32_t MB_WR_Ack4;  ///< 0x100f4 [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 5 of 8)
  volatile uint32_t MB_WR_Ack5;  ///< 0x100f8 [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 6 of 8)
  volatile uint32_t MB_WR_Ack6;  ///< 0x100fc [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 7 of 8)
  volatile uint32_t MB_WR_Ack7;  ///< 0x10100 [W]: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 8 of 8)
  volatile uint32_t reserved22[959];
  volatile uint32_t PhyRegAccess;  ///< 0x11000 [R/W]: DM Application PHY Register Access Control via serial interface
  volatile uint32_t PhyReadWrite;  ///< 0x11004 [R/W]: PHY Read or Write Register
  volatile uint32_t PhyReadWriteDone;  ///< 0x11008 [R]: PHY Read or Write Done Register
  volatile uint32_t PhyReadWriteStart;  ///< 0x1100c [W]: PHY Read or Write Start Register
  volatile uint32_t OutAddrXlt0;  ///< 0x11010 [R/W]: DM Application Outbound Address Translation Register 0
  volatile uint32_t OutAddrXlt1;  ///< 0x11014 [R/W]: DM Application Outbound Address Translation Register 1
  volatile uint32_t OutAddrXlt2;  ///< 0x11018 [R/W]: DM Application Outbound Address Translation Register 2
  volatile uint32_t OutAddrXlt3;  ///< 0x1101c [R/W]: DM Application Outbound Address Translation Register 3
  volatile uint32_t OutAddrXlt4;  ///< 0x11020 [R/W]: DM Application Outbound Address Translation Register 4
  volatile uint32_t OutAddrXlt5;  ///< 0x11024 [R/W]: DM Application Outbound Address Translation Register 5
  volatile uint32_t OutAddrXlt6;  ///< 0x11028 [R/W]: DM Application Outbound Address Translation Register 6
  volatile uint32_t OutAddrXlt7;  ///< 0x1102c [R/W]: DM Application Outbound Address Translation Register 7
  volatile uint32_t CfgControl;  ///< 0x11030 [R/W]: DM Application Configuration TLP Settings
} PCIE_DM_REGS_t;

#endif // PCIE
