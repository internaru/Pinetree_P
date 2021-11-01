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
 * \file asela_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ASELA_REGSTRUCTS_H_
#define _ASELA_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Application Specific ELA (ASELA) (asela)
/** \brief Top-level register file for the Asela block*/
//
//====================================================================

typedef struct ASELA_REGS_s
{
  volatile uint32_t AselaCtrl;  ///< 0x0 [R/W]: Asela Control Register
  volatile uint32_t AselaPreTrigSample;  ///< 0x4 [R/W]: Number of Pre-Trigger Samples
  volatile uint32_t AselaPreTrigSamplesTaken;  ///< 0x8 [R/W]: Number of Pre-Trigger Samples Taken
  volatile uint32_t AselaMaxSampleDepth;  ///< 0xc [R/W]: Maximum Sample Depth
  volatile uint32_t AselaNumSamplesTaken;  ///< 0x10 [R/W]: Number of Samples Taken
  volatile uint32_t AselaBufferDepth;  ///< 0x14 [R/W]: Buffer Depth
  volatile uint32_t AselaTrigStartAddr;  ///< 0x18 [R/W]: Trigger Start Point Address
  volatile uint32_t AselaIRQEnable;  ///< 0x1c [R/W]: Interrupt Enable Register
  volatile uint32_t AselaIRQPend;  ///< 0x20 [R]: Interrupt Pending Register
  volatile uint32_t AselaIRQAck;  ///< 0x24 [W]: Interrupt Acknowledge Register
  volatile uint32_t AselaIRQForce;  ///< 0x28 [W]: Interrupt Force Register
  volatile uint32_t reserved0;
  volatile uint32_t AselaFWStateReg0;  ///< 0x30 [R/W]: Firmware State Register n (Instance 1 of 4)
  volatile uint32_t AselaFWStateReg1;  ///< 0x34 [R/W]: Firmware State Register n (Instance 2 of 4)
  volatile uint32_t AselaFWStateReg2;  ///< 0x38 [R/W]: Firmware State Register n (Instance 3 of 4)
  volatile uint32_t AselaFWStateReg3;  ///< 0x3c [R/W]: Firmware State Register n (Instance 4 of 4)
  volatile uint32_t AselaSampSigSelN0;  ///< 0x40 [R/W]: Select Nibble 2n and 2n+1 (Instance 1 of 8)
  volatile uint32_t AselaSampSigSelN1;  ///< 0x44 [R/W]: Select Nibble 2n and 2n+1 (Instance 2 of 8)
  volatile uint32_t AselaSampSigSelN2;  ///< 0x48 [R/W]: Select Nibble 2n and 2n+1 (Instance 3 of 8)
  volatile uint32_t AselaSampSigSelN3;  ///< 0x4c [R/W]: Select Nibble 2n and 2n+1 (Instance 4 of 8)
  volatile uint32_t AselaSampSigSelN4;  ///< 0x50 [R/W]: Select Nibble 2n and 2n+1 (Instance 5 of 8)
  volatile uint32_t AselaSampSigSelN5;  ///< 0x54 [R/W]: Select Nibble 2n and 2n+1 (Instance 6 of 8)
  volatile uint32_t AselaSampSigSelN6;  ///< 0x58 [R/W]: Select Nibble 2n and 2n+1 (Instance 7 of 8)
  volatile uint32_t AselaSampSigSelN7;  ///< 0x5c [R/W]: Select Nibble 2n and 2n+1 (Instance 8 of 8)
  volatile uint32_t AselaSampSigSelB0;  ///< 0x60 [R/W]: Select Byte 2n and 2n+1 (Instance 1 of 4)
  volatile uint32_t AselaSampSigSelB1;  ///< 0x64 [R/W]: Select Byte 2n and 2n+1 (Instance 2 of 4)
  volatile uint32_t AselaSampSigSelB2;  ///< 0x68 [R/W]: Select Byte 2n and 2n+1 (Instance 3 of 4)
  volatile uint32_t AselaSampSigSelB3;  ///< 0x6c [R/W]: Select Byte 2n and 2n+1 (Instance 4 of 4)
  volatile uint32_t AselaSampSigSelW0;  ///< 0x70 [R/W]: Select Word 2n and 2n+1 (Instance 1 of 2)
  volatile uint32_t AselaSampSigSelW1;  ///< 0x74 [R/W]: Select Word 2n and 2n+1 (Instance 2 of 2)
  volatile uint32_t AselaPclkSel;  ///< 0x78 [R/W]: PCLK Select for Nozzle Detect Function
  volatile uint32_t AselaPloadnSel;  ///< 0x7c [R/W]: PLOAD_N Select for Nozzle Detect Function
  volatile uint32_t AselaAdataSel;  ///< 0x80 [R/W]: ADATA Select for Nozzle Detect Function
  volatile uint32_t AselaPdataSel0;  ///< 0x84 [R/W]: PDATA Select for Nozzle Detect Function (Instance 1 of 8)
  volatile uint32_t AselaPdataSel1;  ///< 0x88 [R/W]: PDATA Select for Nozzle Detect Function (Instance 2 of 8)
  volatile uint32_t AselaPdataSel2;  ///< 0x8c [R/W]: PDATA Select for Nozzle Detect Function (Instance 3 of 8)
  volatile uint32_t AselaPdataSel3;  ///< 0x90 [R/W]: PDATA Select for Nozzle Detect Function (Instance 4 of 8)
  volatile uint32_t AselaPdataSel4;  ///< 0x94 [R/W]: PDATA Select for Nozzle Detect Function (Instance 5 of 8)
  volatile uint32_t AselaPdataSel5;  ///< 0x98 [R/W]: PDATA Select for Nozzle Detect Function (Instance 6 of 8)
  volatile uint32_t AselaPdataSel6;  ///< 0x9c [R/W]: PDATA Select for Nozzle Detect Function (Instance 7 of 8)
  volatile uint32_t AselaPdataSel7;  ///< 0xa0 [R/W]: PDATA Select for Nozzle Detect Function (Instance 8 of 8)
  volatile uint32_t reserved1[7];
  volatile uint32_t Asela8BitEventSigSel0;  ///< 0xc0 [R/W]: 8-Bit Event Module n Signal Select (Instance 1 of 8)
  volatile uint32_t Asela8BitEventCntrl0;  ///< 0xc4 [R/W]: 8-Bit Event Module n Control (Instance 1 of 8)
  volatile uint32_t Asela8BitEventSigSel1;  ///< 0xc8 [R/W]: 8-Bit Event Module n Signal Select (Instance 2 of 8)
  volatile uint32_t Asela8BitEventCntrl1;  ///< 0xcc [R/W]: 8-Bit Event Module n Control (Instance 2 of 8)
  volatile uint32_t Asela8BitEventSigSel2;  ///< 0xd0 [R/W]: 8-Bit Event Module n Signal Select (Instance 3 of 8)
  volatile uint32_t Asela8BitEventCntrl2;  ///< 0xd4 [R/W]: 8-Bit Event Module n Control (Instance 3 of 8)
  volatile uint32_t Asela8BitEventSigSel3;  ///< 0xd8 [R/W]: 8-Bit Event Module n Signal Select (Instance 4 of 8)
  volatile uint32_t Asela8BitEventCntrl3;  ///< 0xdc [R/W]: 8-Bit Event Module n Control (Instance 4 of 8)
  volatile uint32_t Asela8BitEventSigSel4;  ///< 0xe0 [R/W]: 8-Bit Event Module n Signal Select (Instance 5 of 8)
  volatile uint32_t Asela8BitEventCntrl4;  ///< 0xe4 [R/W]: 8-Bit Event Module n Control (Instance 5 of 8)
  volatile uint32_t Asela8BitEventSigSel5;  ///< 0xe8 [R/W]: 8-Bit Event Module n Signal Select (Instance 6 of 8)
  volatile uint32_t Asela8BitEventCntrl5;  ///< 0xec [R/W]: 8-Bit Event Module n Control (Instance 6 of 8)
  volatile uint32_t Asela8BitEventSigSel6;  ///< 0xf0 [R/W]: 8-Bit Event Module n Signal Select (Instance 7 of 8)
  volatile uint32_t Asela8BitEventCntrl6;  ///< 0xf4 [R/W]: 8-Bit Event Module n Control (Instance 7 of 8)
  volatile uint32_t Asela8BitEventSigSel7;  ///< 0xf8 [R/W]: 8-Bit Event Module n Signal Select (Instance 8 of 8)
  volatile uint32_t Asela8BitEventCntrl7;  ///< 0xfc [R/W]: 8-Bit Event Module n Control (Instance 8 of 8)
  volatile uint32_t Asela32BitEventSigSelA0;  ///< 0x100 [R/W]: 32-Bit Event Module n Signal Select A (Instance 1 of 4)
  volatile uint32_t Asela32BitEventSigSelB0;  ///< 0x104 [R/W]: 32-Bit Event Module n Signal Select B (Instance 1 of 4)
  volatile uint32_t reserved2[2];
  volatile uint32_t Asela32BitEventComp0;  ///< 0x110 [R/W]: 32-Bit Event Module n Compare Value (Instance 1 of 4)
  volatile uint32_t Asela32BitEventCntrl0;  ///< 0x114 [R/W]: 32-Bit Event Module n Control (Instance 1 of 4)
  volatile uint32_t Asela32BitEventPMatchA0;  ///< 0x118 [R/W]: 32-Bit Event Module n Pattern Match A (Instance 1 of 4)
  volatile uint32_t Asela32BitEventPMatchB0;  ///< 0x11c [R/W]: 32-Bit Event Module n Pattern Match B (Instance 1 of 4)
  volatile uint32_t Asela32BitEventSigSelA1;  ///< 0x120 [R/W]: 32-Bit Event Module n Signal Select A (Instance 2 of 4)
  volatile uint32_t Asela32BitEventSigSelB1;  ///< 0x124 [R/W]: 32-Bit Event Module n Signal Select B (Instance 2 of 4)
  volatile uint32_t reserved3[2];
  volatile uint32_t Asela32BitEventComp1;  ///< 0x130 [R/W]: 32-Bit Event Module n Compare Value (Instance 2 of 4)
  volatile uint32_t Asela32BitEventCntrl1;  ///< 0x134 [R/W]: 32-Bit Event Module n Control (Instance 2 of 4)
  volatile uint32_t Asela32BitEventPMatchA1;  ///< 0x138 [R/W]: 32-Bit Event Module n Pattern Match A (Instance 2 of 4)
  volatile uint32_t Asela32BitEventPMatchB1;  ///< 0x13c [R/W]: 32-Bit Event Module n Pattern Match B (Instance 2 of 4)
  volatile uint32_t Asela32BitEventSigSelA2;  ///< 0x140 [R/W]: 32-Bit Event Module n Signal Select A (Instance 3 of 4)
  volatile uint32_t Asela32BitEventSigSelB2;  ///< 0x144 [R/W]: 32-Bit Event Module n Signal Select B (Instance 3 of 4)
  volatile uint32_t reserved4[2];
  volatile uint32_t Asela32BitEventComp2;  ///< 0x150 [R/W]: 32-Bit Event Module n Compare Value (Instance 3 of 4)
  volatile uint32_t Asela32BitEventCntrl2;  ///< 0x154 [R/W]: 32-Bit Event Module n Control (Instance 3 of 4)
  volatile uint32_t Asela32BitEventPMatchA2;  ///< 0x158 [R/W]: 32-Bit Event Module n Pattern Match A (Instance 3 of 4)
  volatile uint32_t Asela32BitEventPMatchB2;  ///< 0x15c [R/W]: 32-Bit Event Module n Pattern Match B (Instance 3 of 4)
  volatile uint32_t Asela32BitEventSigSelA3;  ///< 0x160 [R/W]: 32-Bit Event Module n Signal Select A (Instance 4 of 4)
  volatile uint32_t Asela32BitEventSigSelB3;  ///< 0x164 [R/W]: 32-Bit Event Module n Signal Select B (Instance 4 of 4)
  volatile uint32_t reserved5[2];
  volatile uint32_t Asela32BitEventComp3;  ///< 0x170 [R/W]: 32-Bit Event Module n Compare Value (Instance 4 of 4)
  volatile uint32_t Asela32BitEventCntrl3;  ///< 0x174 [R/W]: 32-Bit Event Module n Control (Instance 4 of 4)
  volatile uint32_t Asela32BitEventPMatchA3;  ///< 0x178 [R/W]: 32-Bit Event Module n Pattern Match A (Instance 4 of 4)
  volatile uint32_t Asela32BitEventPMatchB3;  ///< 0x17c [R/W]: 32-Bit Event Module n Pattern Match B (Instance 4 of 4)
  volatile uint32_t Asela64BitEventSigSel0;  ///< 0x180 [R/W]: 64-Bit Event Module n Signal Select (Instance 1 of 2)
  volatile uint32_t reserved6;
  volatile uint32_t Asela64BitEventCntrl0;  ///< 0x188 [R/W]: 64-Bit Event Module n Control (Instance 1 of 2)
  volatile uint32_t Asela64BitEventPMatchA0;  ///< 0x18c [R/W]: 64-Bit Event Module n Pattern Match A (Instance 1 of 2)
  volatile uint32_t Asela64BitEventPMatchB0;  ///< 0x190 [R/W]: 64-Bit Event Module n Pattern Match B (Instance 1 of 2)
  volatile uint32_t Asela64BitEventPMatchC0;  ///< 0x194 [R/W]: 64-Bit Event Module n Pattern Match C (Instance 1 of 2)
  volatile uint32_t Asela64BitEventPMatchD0;  ///< 0x198 [R/W]: 64-Bit Event Module n Pattern Match D (Instance 1 of 2)
  volatile uint32_t reserved7;
  volatile uint32_t Asela64BitEventSigSel1;  ///< 0x1a0 [R/W]: 64-Bit Event Module n Signal Select (Instance 2 of 2)
  volatile uint32_t reserved8;
  volatile uint32_t Asela64BitEventCntrl1;  ///< 0x1a8 [R/W]: 64-Bit Event Module n Control (Instance 2 of 2)
  volatile uint32_t Asela64BitEventPMatchA1;  ///< 0x1ac [R/W]: 64-Bit Event Module n Pattern Match A (Instance 2 of 2)
  volatile uint32_t Asela64BitEventPMatchB1;  ///< 0x1b0 [R/W]: 64-Bit Event Module n Pattern Match B (Instance 2 of 2)
  volatile uint32_t Asela64BitEventPMatchC1;  ///< 0x1b4 [R/W]: 64-Bit Event Module n Pattern Match C (Instance 2 of 2)
  volatile uint32_t Asela64BitEventPMatchD1;  ///< 0x1b8 [R/W]: 64-Bit Event Module n Pattern Match D (Instance 2 of 2)
  volatile uint32_t reserved9;
  volatile uint32_t AselaIFCntrl0;  ///< 0x1c0 [R/W]: IF Statement n Control (Instance 1 of 10)
  volatile uint32_t AselaIFCntrSetup0;  ///< 0x1c4 [R/W]: IF Statement Counter Setup (Instance 1 of 10)
  volatile uint32_t AselaIFCntrl1;  ///< 0x1c8 [R/W]: IF Statement n Control (Instance 2 of 10)
  volatile uint32_t AselaIFCntrSetup1;  ///< 0x1cc [R/W]: IF Statement Counter Setup (Instance 2 of 10)
  volatile uint32_t AselaIFCntrl2;  ///< 0x1d0 [R/W]: IF Statement n Control (Instance 3 of 10)
  volatile uint32_t AselaIFCntrSetup2;  ///< 0x1d4 [R/W]: IF Statement Counter Setup (Instance 3 of 10)
  volatile uint32_t AselaIFCntrl3;  ///< 0x1d8 [R/W]: IF Statement n Control (Instance 4 of 10)
  volatile uint32_t AselaIFCntrSetup3;  ///< 0x1dc [R/W]: IF Statement Counter Setup (Instance 4 of 10)
  volatile uint32_t AselaIFCntrl4;  ///< 0x1e0 [R/W]: IF Statement n Control (Instance 5 of 10)
  volatile uint32_t AselaIFCntrSetup4;  ///< 0x1e4 [R/W]: IF Statement Counter Setup (Instance 5 of 10)
  volatile uint32_t AselaIFCntrl5;  ///< 0x1e8 [R/W]: IF Statement n Control (Instance 6 of 10)
  volatile uint32_t AselaIFCntrSetup5;  ///< 0x1ec [R/W]: IF Statement Counter Setup (Instance 6 of 10)
  volatile uint32_t AselaIFCntrl6;  ///< 0x1f0 [R/W]: IF Statement n Control (Instance 7 of 10)
  volatile uint32_t AselaIFCntrSetup6;  ///< 0x1f4 [R/W]: IF Statement Counter Setup (Instance 7 of 10)
  volatile uint32_t AselaIFCntrl7;  ///< 0x1f8 [R/W]: IF Statement n Control (Instance 8 of 10)
  volatile uint32_t AselaIFCntrSetup7;  ///< 0x1fc [R/W]: IF Statement Counter Setup (Instance 8 of 10)
  volatile uint32_t AselaIFCntrl8;  ///< 0x200 [R/W]: IF Statement n Control (Instance 9 of 10)
  volatile uint32_t AselaIFCntrSetup8;  ///< 0x204 [R/W]: IF Statement Counter Setup (Instance 9 of 10)
  volatile uint32_t AselaIFCntrl9;  ///< 0x208 [R/W]: IF Statement n Control (Instance 10 of 10)
  volatile uint32_t AselaIFCntrSetup9;  ///< 0x20c [R/W]: IF Statement Counter Setup (Instance 10 of 10)
  volatile uint32_t AselaCntrTimer0;  ///< 0x210 [R/W]: Counter / Timer n (Instance 1 of 4)
  volatile uint32_t AselaCntrTimer1;  ///< 0x214 [R/W]: Counter / Timer n (Instance 2 of 4)
  volatile uint32_t AselaCntrTimer2;  ///< 0x218 [R/W]: Counter / Timer n (Instance 3 of 4)
  volatile uint32_t AselaCntrTimer3;  ///< 0x21c [R/W]: Counter / Timer n (Instance 4 of 4)
  volatile uint32_t AselaBWAccSetup0;  ///< 0x220 [R/W]: BW Profiler Accumulator Setup (Instance 1 of 2)
  volatile uint32_t AselaBWAccSetup1;  ///< 0x224 [R/W]: BW Profiler Accumulator Setup (Instance 2 of 2)
  volatile uint32_t AselaBWAcc0;  ///< 0x228 [R/W]: BW Profiler Accumulator (Instance 1 of 2)
  volatile uint32_t AselaBWAcc1;  ///< 0x22c [R/W]: BW Profiler Accumulator (Instance 2 of 2)
  volatile uint32_t AselaPrintNozDet0;  ///< 0x230 [R/W]: Print Nozzle Detect (Instance 1 of 4)
  volatile uint32_t AselaPrintNozDet1;  ///< 0x234 [R/W]: Print Nozzle Detect (Instance 2 of 4)
  volatile uint32_t AselaPrintNozDet2;  ///< 0x238 [R/W]: Print Nozzle Detect (Instance 3 of 4)
  volatile uint32_t AselaPrintNozDet3;  ///< 0x23c [R/W]: Print Nozzle Detect (Instance 4 of 4)
  volatile uint32_t AselaMisrTestSelA;  ///< 0x240 [R/W]: MISR Test Signal Select A
  volatile uint32_t AselaMisrTestSelB;  ///< 0x244 [R/W]: MISR Test Signal Select B
  volatile uint32_t AselaMisrLoadSelA;  ///< 0x248 [R/W]: MISR Load Signal Select A
  volatile uint32_t AselaMisrLoadSelB;  ///< 0x24c [R/W]: MISR Load Signal Select B
  volatile uint32_t AselaMisrTstMskLdData;  ///< 0x250 [R/W]: MISR Test Data Mask / Load Data Register
  volatile uint32_t AselaMisrResult;  ///< 0x254 [R]: MISR Result Register
} ASELA_REGS_t;

#endif // ASELA
