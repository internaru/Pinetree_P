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
 * \file Xena_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _XENA_REGSTRUCTS_H_
#define _XENA_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Register descriptions for UPC module (Xena)
/** \brief Register Descriptions for the UPC block. Registers from offset starting at
0x0 are "Public", and registers with offsets starting at 0x1000 are "Private".*/
//
//====================================================================

typedef struct XENA_REGS_s
{
  volatile uint32_t reserved0[37];
  volatile uint32_t Phase0;  ///< 0x94 [R]: UPC Encoder Phase 0 Register
  volatile uint32_t Phase1;  ///< 0x98 [R]: UPC Encoder Phase 1 Register
  volatile uint32_t Phase2;  ///< 0x9c [R]: UPC Encoder Phase 2 Register
  volatile uint32_t Phase3;  ///< 0xa0 [R]: UPC Encoder Phase 3 Register
  volatile uint32_t reserved1[33];
  volatile uint32_t Mailbox0;  ///< 0x128 [R/W]: UPC Mailbox 0 Register
  volatile uint32_t Mailbox1;  ///< 0x12c [R/W]: UPC Mailbox 1 Register
  volatile uint32_t reserved2[8];
  volatile uint32_t GPStatus;  ///< 0x150 [R/W]: UPC General Purpose Status Register
  volatile uint32_t CPUSoftReset;  ///< 0x154 [R/W]: UPC CPU Soft Reset Register
  volatile uint32_t PhaseRef;  ///< 0x158 [R]: UPC Encoder Phase 0 Reference Register
  volatile uint32_t reserved3[41];
  volatile uint32_t PerfMon0FWEvent;  ///< 0x200 [W]: UPC Performance Monitor 0 Firmware Event Register
  volatile uint32_t PerfMon0Timer;  ///< 0x204 [R]: UPC Performance Monitor 0 Timer Register
  volatile uint32_t reserved4[6];
  volatile uint32_t PerfMon0StartEn[2];  ///< 0x220 [R/W]: UPC Performance Monitor 0 Start Enable (Instance 1 of 2)
  volatile uint32_t reserved5[6];
  volatile uint32_t PerfMon0StopEn[2];  ///< 0x240 [R/W]: UPC Performance Monitor 0 Stop Enable (Instance 1 of 2)
  volatile uint32_t reserved6[6];
  volatile uint32_t PerfMon1FWEvent;  ///< 0x260 [W]: UPC Performance Monitor 1 Firmware Event Register
  volatile uint32_t PerfMon1Timer;  ///< 0x264 [R]: UPC Performance Monitor 1 Timer Register
  volatile uint32_t reserved7[6];
  volatile uint32_t PerfMon1StartEn[2];  ///< 0x280 [R/W]: UPC Performance Monitor 1 Start Enable (Instance 1 of 2)
  volatile uint32_t reserved8[6];
  volatile uint32_t PerfMon1StopEn[2];  ///< 0x2a0 [R/W]: UPC Performance Monitor 1 Stop Enable (Instance 1 of 2)
  volatile uint32_t reserved9[854];
  volatile uint32_t XRR;  ///< 0x1000 [R/W]: UPC Prefetch Reset Register (Private)
  volatile uint32_t XPS;  ///< 0x1004 [W]: UPC Prefetch Start Register (Private)
  volatile uint32_t XCR;  ///< 0x1008 [R/W]: UPC Prefetch Configuration Register (Private)
  volatile uint32_t XMC;  ///< 0x100c [R/W]: UPC Prefetch Mask Configuration Register (Private)
  volatile uint32_t XMA;  ///< 0x1010 [R/W]: UPC Prefetch Mask Address Register (Private)
  volatile uint32_t XMJ;  ///< 0x1014 [R/W]: UPC Prefetch Mask Jump Register (Private)
  volatile uint32_t XMA2;  ///< 0x1018 [R/W]: UPC Prefetch Mask Address2 Register (Private)
  volatile uint32_t XML;  ///< 0x101c [R/W]: UPC Prefetch Mask Lines Register (Private)
  volatile uint32_t XDA;  ///< 0x1020 [R/W]: UPC Prefetch Data Address Register (Private)
  volatile uint32_t XDJ;  ///< 0x1024 [R/W]: UPC Prefetch Data Jump Register (Private)
  volatile uint32_t XDA2;  ///< 0x1028 [R/W]: UPC Prefetch Data Address2 Register (Private)
  volatile uint32_t XDL;  ///< 0x102c [R/W]: UPC Prefetch Data Lines Register (Private)
  volatile uint32_t MaskTileBurst;  ///< 0x1030 [R/W]: UPC Prefetch Mask Burst Tiling setup Register (Private)
  volatile uint32_t MaskTileWord;  ///< 0x1034 [R/W]: UPC Prefetch Mask Word Tiling setup Register (Private)
  volatile uint32_t XCA;  ///< 0x1038 [R/W]: UPC Prefetch Destination Address Register (Private)
  volatile uint32_t XCJ;  ///< 0x103c [R/W]: UPC Prefetch Destination Jump Register (Private)
  volatile uint32_t XNW;  ///< 0x1040 [R/W]: UPC Prefetch Bypass NumWords Register (Private)
  volatile uint32_t XST;  ///< 0x1044 [R]: UPC Prefetch Status Register (Private)
  volatile uint32_t XCA2;  ///< 0x1048 [R/W]: UPC Prefetch Destination Address Register2 (Private)
  volatile uint32_t XDest;  ///< 0x104c [R/W]: UPC Prefetch Destination Config Register (Private)
  volatile uint32_t reserved10[12];
  volatile uint32_t EncControl;  ///< 0x1080 [R/W]: UPC Encoder Control Register (Private)
  volatile uint32_t EncPos;  ///< 0x1084 [R/W]: UPC Encoder Position Register
  volatile uint32_t EncFilt;  ///< 0x1088 [R/W]: UPC Encoder Filter Register (Private)
  volatile uint32_t Coeff;  ///< 0x108c [R/W]: UPC Encoder Period Prediction Coefficient Register (Private)
  volatile uint32_t FWPredPeriod;  ///< 0x1090 [R/W]: UPC Firmware Predicted Period Register (Private)
  volatile uint32_t reserved11[4];
  volatile uint32_t LeftMargin0;  ///< 0x10a4 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 1 of 8)
  volatile uint32_t RightMargin0;  ///< 0x10a8 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 1 of 8)
  volatile uint32_t Res0;  ///< 0x10ac [R/W]: UPC Encoder Resolution Register (Private) (Instance 1 of 8)
  volatile uint32_t TOF0;  ///< 0x10b0 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 1 of 8)
  volatile uint32_t LeftMargin1;  ///< 0x10b4 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 2 of 8)
  volatile uint32_t RightMargin1;  ///< 0x10b8 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 2 of 8)
  volatile uint32_t Res1;  ///< 0x10bc [R/W]: UPC Encoder Resolution Register (Private) (Instance 2 of 8)
  volatile uint32_t TOF1;  ///< 0x10c0 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 2 of 8)
  volatile uint32_t LeftMargin2;  ///< 0x10c4 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 3 of 8)
  volatile uint32_t RightMargin2;  ///< 0x10c8 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 3 of 8)
  volatile uint32_t Res2;  ///< 0x10cc [R/W]: UPC Encoder Resolution Register (Private) (Instance 3 of 8)
  volatile uint32_t TOF2;  ///< 0x10d0 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 3 of 8)
  volatile uint32_t LeftMargin3;  ///< 0x10d4 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 4 of 8)
  volatile uint32_t RightMargin3;  ///< 0x10d8 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 4 of 8)
  volatile uint32_t Res3;  ///< 0x10dc [R/W]: UPC Encoder Resolution Register (Private) (Instance 4 of 8)
  volatile uint32_t TOF3;  ///< 0x10e0 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 4 of 8)
  volatile uint32_t LeftMargin4;  ///< 0x10e4 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 5 of 8)
  volatile uint32_t RightMargin4;  ///< 0x10e8 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 5 of 8)
  volatile uint32_t Res4;  ///< 0x10ec [R/W]: UPC Encoder Resolution Register (Private) (Instance 5 of 8)
  volatile uint32_t TOF4;  ///< 0x10f0 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 5 of 8)
  volatile uint32_t LeftMargin5;  ///< 0x10f4 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 6 of 8)
  volatile uint32_t RightMargin5;  ///< 0x10f8 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 6 of 8)
  volatile uint32_t Res5;  ///< 0x10fc [R/W]: UPC Encoder Resolution Register (Private) (Instance 6 of 8)
  volatile uint32_t TOF5;  ///< 0x1100 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 6 of 8)
  volatile uint32_t LeftMargin6;  ///< 0x1104 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 7 of 8)
  volatile uint32_t RightMargin6;  ///< 0x1108 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 7 of 8)
  volatile uint32_t Res6;  ///< 0x110c [R/W]: UPC Encoder Resolution Register (Private) (Instance 7 of 8)
  volatile uint32_t TOF6;  ///< 0x1110 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 7 of 8)
  volatile uint32_t LeftMargin7;  ///< 0x1114 [R/W]: UPC Encoder Left Margin Register (Private) (Instance 8 of 8)
  volatile uint32_t RightMargin7;  ///< 0x1118 [R/W]: UPC Encoder Right Margin Register (Private) (Instance 8 of 8)
  volatile uint32_t Res7;  ///< 0x111c [R/W]: UPC Encoder Resolution Register (Private) (Instance 8 of 8)
  volatile uint32_t TOF7;  ///< 0x1120 [R/W]: UPC Encoder Time-of-Flight Register (Private) (Instance 8 of 8)
  volatile uint32_t Status;  ///< 0x1124 [R/W]: UPC Encoder Status Register (Private)
  volatile uint32_t PseudoEncoder;  ///< 0x1128 [R/W]: UPC Pseudo Encoder Register (Private)
  volatile uint32_t reserved12;
  volatile uint32_t Mailbox2;  ///< 0x1130 [R/W]: UPC Mailbox 2 Register (Private)
  volatile uint32_t Mailbox3;  ///< 0x1134 [R/W]: UPC Mailbox 3 Register (Private)
  volatile uint32_t Mailbox4;  ///< 0x1138 [R/W]: UPC Mailbox 4 Register (Private)
  volatile uint32_t Mailbox5;  ///< 0x113c [R/W]: UPC Mailbox 5 Register (Private)
  volatile uint32_t Timer0;  ///< 0x1140 [R/W]: UPC Timer 0 Register (Private)
  volatile uint32_t Timer1;  ///< 0x1144 [R/W]: UPC Timer 1 Register (Private)
  volatile uint32_t Timer2;  ///< 0x1148 [R/W]: UPC Timer 2 Register (Private)
  volatile uint32_t Timer3;  ///< 0x114c [R/W]: UPC Timer 3 Register (Private)
  volatile uint32_t reserved13[4];
  volatile uint32_t XIEnSel0;  ///< 0x1160 [R/W]: UPC I/O Encoder Source Selection Register 0 (Private)
  volatile uint32_t XIEnSel1;  ///< 0x1164 [R/W]: UPC I/O Encoder Source Selection Register 1 (Private)
  volatile uint32_t reserved14[6];
  volatile uint32_t XCT;  ///< 0x1180 [W]: UPCIO Reset Register (Private)
  volatile uint32_t XIE0;  ///< 0x1184 [R/W]: UPCIO Clock Enable Register (Private)
  volatile uint32_t XIFire;  ///< 0x1188 [R/W]: UPCIO Fire pulse selection (Private)
  volatile uint32_t XISync;  ///< 0x118c [R/W]: UPCIO Sync pulse selection (Private)
  volatile uint32_t XIC[8];  ///< 0x1190 [R/W]: UPCIO ClkGen Configuration Register (Private) (Instance 1 of 8)
  volatile uint32_t XICST0;  ///< 0x11b0 [R]: UPCIO Status Register0 (Private)
  volatile uint32_t XICST;  ///< 0x11b4 [R]: UPCIO Status Register (Private)
  volatile uint32_t XIFC;  ///< 0x11b8 [R/W]: UPCIO FIFO Configuration Register (Private)
  volatile uint32_t XIFI;  ///< 0x11bc [R/W]: UPCIO FIFO Interrupt Level Register (Private)
  volatile uint32_t XIFD[8];  ///< 0x11c0 [W]: UPCIO FIFO Data Register (Private) (Instance 1 of 8)
  volatile uint32_t XIFSt[8];  ///< 0x11e0 [R/W]: UPCIO FIFO Ring Buffer Start address Register (Private) (Instance 1 of 8)
  volatile uint32_t XIFSp[8];  ///< 0x1200 [R/W]: UPCIO FIFO Ring Buffer Stop address Register (Private) (Instance 1 of 8)
  volatile uint32_t XIFBF[8];  ///< 0x1220 [W]: UPCIO FIFO Ring Buffer Count Increment Register (Private) (Instance 1 of 8)
  volatile uint32_t XIFBC[8];  ///< 0x1240 [R]: UPCIO FIFO Ring Buffer Current Count Register (Private) (Instance 1 of 8)
  volatile uint32_t XISI;  ///< 0x1260 [R/W]: UPCIO Serializer Interleave Configuration Register (Private)
  volatile uint32_t XISC;  ///< 0x1264 [R/W]: UPCIO Serializer Configuration Register (Private)
  volatile uint32_t XIIC[52];  ///< 0x1268 [R/W]: UPCIO IO Configuration Register (Private) (Instance 1 of 52)
  volatile uint32_t reserved15[12];
  volatile uint32_t XIP0[52];  ///< 0x1368 [R/W]: UPCIO IO PWM Config Register0 (Private) (Instance 1 of 52)
  volatile uint32_t reserved16[12];
  volatile uint32_t XIP1[52];  ///< 0x1468 [R/W]: UPCIO IO PWM Config Register1 (Private) (Instance 1 of 52)
  volatile uint32_t reserved17[12];
  volatile uint32_t XIP2[52];  ///< 0x1568 [R/W]: UPCIO IO PWM Config Register2 (Private) (Instance 1 of 52)
  volatile uint32_t reserved18[6];
  volatile uint32_t XIRW;  ///< 0x1650 [R/W]: UPCIO Bit-Bang Write Data Register (Private)
  volatile uint32_t reserved19;
  volatile uint32_t XIRD;  ///< 0x1658 [R]: UPCIO Bit-Bang Read Data Register (Private)
  volatile uint32_t XIRD1;  ///< 0x165c [R]: UPCIO Bit-Bang Read Data Register 1 (Private)
  volatile uint32_t reserved20[4];
  volatile uint32_t XIRS;  ///< 0x1670 [R/W]: UPCIO IO Read Sync select Register (Private)
  volatile uint32_t XIII;  ///< 0x1674 [R/W]: UPCIO IO Interrupt Config Register (Private)
  volatile uint32_t XISyncConfig[8];  ///< 0x1678 [R/W]: UPCIO ClkGen Sync Configuration Register (Private) (Instance 1 of 8)
  volatile uint32_t reserved21[2];
  volatile uint32_t SafetyData0;  ///< 0x16a0 [R/W]: UPC Safety Watchdog Safe Value (Private) (Instance 1 of 2)
  volatile uint32_t SafetyData1;  ///< 0x16a4 [R/W]: UPC Safety Watchdog Safe Value (Private) (Instance 2 of 2)
  volatile uint32_t reserved22[2];
  volatile uint32_t SafetyOE0;  ///< 0x16b0 [R/W]: UPC Safety Watchdog Safe Output Enable (Private) (Instance 1 of 2)
  volatile uint32_t SafetyOE1;  ///< 0x16b4 [R/W]: UPC Safety Watchdog Safe Output Enable (Private) (Instance 2 of 2)
  volatile uint32_t reserved23[2];
  volatile uint32_t SafetyWatchdog;  ///< 0x16c0 [R/W]: UPC Safety Watchdog Timer (Private)
  volatile uint32_t reserved24[15];
  volatile uint32_t LFSR[52];  ///< 0x1700 [R/W]: UPCIO IO LFSR (CRC) Register (Private)
  volatile uint32_t reserved25[12];
  volatile uint32_t IRQEnableFM[4];  ///< 0x1800 [R/W]: UPC Formatter CPU Regular Interrupt Enable (Private) (Instance 1 of 4)
  volatile uint32_t reserved26[4];
  volatile uint32_t FIQEnableFM[4];  ///< 0x1820 [R/W]: UPC Formatter CPU Fast Interrupt Enable (Private) (Instance 1 of 4)
  volatile uint32_t reserved27[4];
  volatile uint32_t IntPostFM[4];  ///< 0x1840 [R]: UPC Formatter CPU Interrupt Posted (Private) (Instance 1 of 4)
  volatile uint32_t reserved28[4];
  volatile uint32_t IntClearFM[4];  ///< 0x1860 [W]: UPC Formatter CPU Interrupt Clear (Private) (Instance 1 of 4)
  volatile uint32_t reserved29[4];
  volatile uint32_t IRQEnableIO[4];  ///< 0x1880 [R/W]: UPC I/O CPU Regular Interrupt Enable (Private) (Instance 1 of 4)
  volatile uint32_t reserved30[4];
  volatile uint32_t FIQEnableIO[4];  ///< 0x18a0 [R/W]: UPC I/O CPU Fast Interrupt Enable (Private) (Instance 1 of 4)
  volatile uint32_t reserved31[4];
  volatile uint32_t IntPostIO[4];  ///< 0x18c0 [R]: UPC I/O CPU Interrupt Posted (Private) (Instance 1 of 4)
  volatile uint32_t reserved32[4];
  volatile uint32_t IntClearIO[4];  ///< 0x18e0 [W]: UPC I/O CPU Interrupt Clear (Private) (Instance 1 of 4)
  volatile uint32_t reserved33[5572];
  volatile uint32_t APB[1024];  ///< 0x7000 [R/W]: APB Register (Private)
} XENA_REGS_t;

#endif // XENA
