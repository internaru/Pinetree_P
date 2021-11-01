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
 * \file UED_advanced_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _UED_ADVANCED_REGSTRUCTS_H_
#define _UED_ADVANCED_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Universal Error Diffusion Halftone Registers (UED_advanced)
/** \brief Register descriptions for the Univeral Error Diffusion halfTone block (UED).*/
//
//====================================================================

typedef struct UED_ADVANCED_REGS_s
{
  volatile uint32_t CLUT0[256];  ///< 0x0 [R/W]: TDS LUT SRAM (Instance 1 of 6)
  volatile uint32_t CLUT1[256];  ///< 0x400 [R/W]: TDS LUT SRAM (Instance 2 of 6)
  volatile uint32_t CLUT2[256];  ///< 0x800 [R/W]: TDS LUT SRAM (Instance 3 of 6)
  volatile uint32_t CLUT3[256];  ///< 0xc00 [R/W]: TDS LUT SRAM (Instance 4 of 6)
  volatile uint32_t CLUT4[256];  ///< 0x1000 [R/W]: TDS LUT SRAM (Instance 5 of 6)
  volatile uint32_t CLUT5[256];  ///< 0x1400 [R/W]: TDS LUT SRAM (Instance 6 of 6)
  volatile uint32_t WLUT0[128];  ///< 0x1800 [R/W]: TDW LUT SRAM (Instance 1 of 6)
  volatile uint32_t WLUT1[128];  ///< 0x1a00 [R/W]: TDW LUT SRAM (Instance 2 of 6)
  volatile uint32_t WLUT2[128];  ///< 0x1c00 [R/W]: TDW LUT SRAM (Instance 3 of 6)
  volatile uint32_t WLUT3[128];  ///< 0x1e00 [R/W]: TDW LUT SRAM (Instance 4 of 6)
  volatile uint32_t WLUT4[128];  ///< 0x2000 [R/W]: TDW LUT SRAM (Instance 5 of 6)
  volatile uint32_t WLUT5[128];  ///< 0x2200 [R/W]: TDW LUT SRAM (Instance 6 of 6)
  volatile uint32_t BLAC0;  ///< 0x2400 [R/W]: Bias LUT Access Command (Instance 1 of 6)
  volatile uint32_t BLAC1;  ///< 0x2404 [R/W]: Bias LUT Access Command (Instance 2 of 6)
  volatile uint32_t BLAC2;  ///< 0x2408 [R/W]: Bias LUT Access Command (Instance 3 of 6)
  volatile uint32_t BLAC3;  ///< 0x240c [R/W]: Bias LUT Access Command (Instance 4 of 6)
  volatile uint32_t BLAC4;  ///< 0x2410 [R/W]: Bias LUT Access Command (Instance 5 of 6)
  volatile uint32_t BLAC5;  ///< 0x2414 [R/W]: Bias LUT Access Command (Instance 6 of 6)
  volatile uint32_t BLAA0;  ///< 0x2418 [R/W]: Bias LUT Access Address (Instance 1 of 6)
  volatile uint32_t BLAA1;  ///< 0x241c [R/W]: Bias LUT Access Address (Instance 2 of 6)
  volatile uint32_t BLAA2;  ///< 0x2420 [R/W]: Bias LUT Access Address (Instance 3 of 6)
  volatile uint32_t BLAA3;  ///< 0x2424 [R/W]: Bias LUT Access Address (Instance 4 of 6)
  volatile uint32_t BLAA4;  ///< 0x2428 [R/W]: Bias LUT Access Address (Instance 5 of 6)
  volatile uint32_t BLAA5;  ///< 0x242c [R/W]: Bias LUT Access Address (Instance 6 of 6)
  volatile uint32_t BLAD0;  ///< 0x2430 [R/W]: Bias LUT Access Data (Instance 1 of 6)
  volatile uint32_t BLAD1;  ///< 0x2434 [R/W]: Bias LUT Access Data (Instance 2 of 6)
  volatile uint32_t BLAD2;  ///< 0x2438 [R/W]: Bias LUT Access Data (Instance 3 of 6)
  volatile uint32_t BLAD3;  ///< 0x243c [R/W]: Bias LUT Access Data (Instance 4 of 6)
  volatile uint32_t BLAD4;  ///< 0x2440 [R/W]: Bias LUT Access Data (Instance 5 of 6)
  volatile uint32_t BLAD5;  ///< 0x2444 [R/W]: Bias LUT Access Data (Instance 6 of 6)
  volatile uint32_t CFGA;  ///< 0x2448 [R/W]: Configuration Register
  volatile uint32_t CFGB;  ///< 0x244c [R/W]: Configuration Register for ED PAIR mode
  volatile uint32_t LW;  ///< 0x2450 [R/W]: Image Line Width
  volatile uint32_t L1;  ///< 0x2454 [R/W]: First Line Error Control
  volatile uint32_t PSA;  ///< 0x2458 [R/W]: Pixel Stacking Avoidance Control
  volatile uint32_t EWC;  ///< 0x245c [R/W]: Tone Dependent Error Weight Control
  volatile uint32_t EDC;  ///< 0x2460 [R/W]: Error Decay Control
  volatile uint32_t EDD1;  ///< 0x2464 [R/W]: Decay Error Weights
  volatile uint32_t EDD2;  ///< 0x2468 [R/W]: Decay Error Weights
  volatile uint32_t EWD1;  ///< 0x246c [R/W]: Default Error Weights
  volatile uint32_t EWD2;  ///< 0x2470 [R/W]: Default Error Weights
  volatile uint32_t B1;  ///< 0x2474 [R/W]: Bias Control 1
  volatile uint32_t B2;  ///< 0x2478 [R/W]: Bias Control 2
  volatile uint32_t B3;  ///< 0x247c [R/W]: Bias Control 3
  volatile uint32_t UBS;  ///< 0x2480 [R]: UED Build Spec
  volatile uint32_t BWS0;  ///< 0x2484 [R/W]: Bias Window Shape (Instance 1 of 6)
  volatile uint32_t BWS1;  ///< 0x2488 [R/W]: Bias Window Shape (Instance 2 of 6)
  volatile uint32_t BWS2;  ///< 0x248c [R/W]: Bias Window Shape (Instance 3 of 6)
  volatile uint32_t BWS3;  ///< 0x2490 [R/W]: Bias Window Shape (Instance 4 of 6)
  volatile uint32_t BWS4;  ///< 0x2494 [R/W]: Bias Window Shape (Instance 5 of 6)
  volatile uint32_t BWS5;  ///< 0x2498 [R/W]: Bias Window Shape (Instance 6 of 6)
  volatile uint32_t IR;  ///< 0x249c [R/W]: Image Row Odd Even
  volatile uint32_t BWR0;  ///< 0x24a0 [R/W]: Bias Window Row Access (Instance 1 of 6)
  volatile uint32_t BWR1;  ///< 0x24a4 [R/W]: Bias Window Row Access (Instance 2 of 6)
  volatile uint32_t BWR2;  ///< 0x24a8 [R/W]: Bias Window Row Access (Instance 3 of 6)
  volatile uint32_t BWR3;  ///< 0x24ac [R/W]: Bias Window Row Access (Instance 4 of 6)
  volatile uint32_t BWR4;  ///< 0x24b0 [R/W]: Bias Window Row Access (Instance 5 of 6)
  volatile uint32_t BWR5;  ///< 0x24b4 [R/W]: Bias Window Row Access (Instance 6 of 6)
  volatile uint32_t LFSRX0;  ///< 0x24b8 [R/W]: LFSR Context Switch (Instance 1 of 6)
  volatile uint32_t LFSRX1;  ///< 0x24bc [R/W]: LFSR Context Switch (Instance 2 of 6)
  volatile uint32_t LFSRX2;  ///< 0x24c0 [R/W]: LFSR Context Switch (Instance 3 of 6)
  volatile uint32_t LFSRX3;  ///< 0x24c4 [R/W]: LFSR Context Switch (Instance 4 of 6)
  volatile uint32_t LFSRX4;  ///< 0x24c8 [R/W]: LFSR Context Switch (Instance 5 of 6)
  volatile uint32_t LFSRX5;  ///< 0x24cc [R/W]: LFSR Context Switch (Instance 6 of 6)
  volatile uint32_t LFSRS0;  ///< 0x24d0 [R/W]: LFSR Start Value (Instance 1 of 6)
  volatile uint32_t LFSRS1;  ///< 0x24d4 [R/W]: LFSR Start Value (Instance 2 of 6)
  volatile uint32_t LFSRS2;  ///< 0x24d8 [R/W]: LFSR Start Value (Instance 3 of 6)
  volatile uint32_t LFSRS3;  ///< 0x24dc [R/W]: LFSR Start Value (Instance 4 of 6)
  volatile uint32_t LFSRS4;  ///< 0x24e0 [R/W]: LFSR Start Value (Instance 5 of 6)
  volatile uint32_t LFSRS5;  ///< 0x24e4 [R/W]: LFSR Start Value (Instance 6 of 6)
  volatile uint32_t THA0;  ///< 0x24e8 [R/W]: Thresholding Registers (Instance 1 of 30)
  volatile uint32_t THA1;  ///< 0x24ec [R/W]: Thresholding Registers (Instance 2 of 30)
  volatile uint32_t THA2;  ///< 0x24f0 [R/W]: Thresholding Registers (Instance 3 of 30)
  volatile uint32_t THA3;  ///< 0x24f4 [R/W]: Thresholding Registers (Instance 4 of 30)
  volatile uint32_t THA4;  ///< 0x24f8 [R/W]: Thresholding Registers (Instance 5 of 30)
  volatile uint32_t THA5;  ///< 0x24fc [R/W]: Thresholding Registers (Instance 6 of 30)
  volatile uint32_t THA6;  ///< 0x2500 [R/W]: Thresholding Registers (Instance 7 of 30)
  volatile uint32_t THA7;  ///< 0x2504 [R/W]: Thresholding Registers (Instance 8 of 30)
  volatile uint32_t THA8;  ///< 0x2508 [R/W]: Thresholding Registers (Instance 9 of 30)
  volatile uint32_t THA9;  ///< 0x250c [R/W]: Thresholding Registers (Instance 10 of 30)
  volatile uint32_t THA10;  ///< 0x2510 [R/W]: Thresholding Registers (Instance 11 of 30)
  volatile uint32_t THA11;  ///< 0x2514 [R/W]: Thresholding Registers (Instance 12 of 30)
  volatile uint32_t THA12;  ///< 0x2518 [R/W]: Thresholding Registers (Instance 13 of 30)
  volatile uint32_t THA13;  ///< 0x251c [R/W]: Thresholding Registers (Instance 14 of 30)
  volatile uint32_t THA14;  ///< 0x2520 [R/W]: Thresholding Registers (Instance 15 of 30)
  volatile uint32_t THA15;  ///< 0x2524 [R/W]: Thresholding Registers (Instance 16 of 30)
  volatile uint32_t THA16;  ///< 0x2528 [R/W]: Thresholding Registers (Instance 17 of 30)
  volatile uint32_t THA17;  ///< 0x252c [R/W]: Thresholding Registers (Instance 18 of 30)
  volatile uint32_t THA18;  ///< 0x2530 [R/W]: Thresholding Registers (Instance 19 of 30)
  volatile uint32_t THA19;  ///< 0x2534 [R/W]: Thresholding Registers (Instance 20 of 30)
  volatile uint32_t THA20;  ///< 0x2538 [R/W]: Thresholding Registers (Instance 21 of 30)
  volatile uint32_t THA21;  ///< 0x253c [R/W]: Thresholding Registers (Instance 22 of 30)
  volatile uint32_t THA22;  ///< 0x2540 [R/W]: Thresholding Registers (Instance 23 of 30)
  volatile uint32_t THA23;  ///< 0x2544 [R/W]: Thresholding Registers (Instance 24 of 30)
  volatile uint32_t THA24;  ///< 0x2548 [R/W]: Thresholding Registers (Instance 25 of 30)
  volatile uint32_t THA25;  ///< 0x254c [R/W]: Thresholding Registers (Instance 26 of 30)
  volatile uint32_t THA26;  ///< 0x2550 [R/W]: Thresholding Registers (Instance 27 of 30)
  volatile uint32_t THA27;  ///< 0x2554 [R/W]: Thresholding Registers (Instance 28 of 30)
  volatile uint32_t THA28;  ///< 0x2558 [R/W]: Thresholding Registers (Instance 29 of 30)
  volatile uint32_t THA29;  ///< 0x255c [R/W]: Thresholding Registers (Instance 30 of 30)
  volatile uint32_t THB0;  ///< 0x2560 [R/W]: Thresholding Registers for ED Pair (Instance 1 of 30)
  volatile uint32_t THB1;  ///< 0x2564 [R/W]: Thresholding Registers for ED Pair (Instance 2 of 30)
  volatile uint32_t THB2;  ///< 0x2568 [R/W]: Thresholding Registers for ED Pair (Instance 3 of 30)
  volatile uint32_t THB3;  ///< 0x256c [R/W]: Thresholding Registers for ED Pair (Instance 4 of 30)
  volatile uint32_t THB4;  ///< 0x2570 [R/W]: Thresholding Registers for ED Pair (Instance 5 of 30)
  volatile uint32_t THB5;  ///< 0x2574 [R/W]: Thresholding Registers for ED Pair (Instance 6 of 30)
  volatile uint32_t THB6;  ///< 0x2578 [R/W]: Thresholding Registers for ED Pair (Instance 7 of 30)
  volatile uint32_t THB7;  ///< 0x257c [R/W]: Thresholding Registers for ED Pair (Instance 8 of 30)
  volatile uint32_t THB8;  ///< 0x2580 [R/W]: Thresholding Registers for ED Pair (Instance 9 of 30)
  volatile uint32_t THB9;  ///< 0x2584 [R/W]: Thresholding Registers for ED Pair (Instance 10 of 30)
  volatile uint32_t THB10;  ///< 0x2588 [R/W]: Thresholding Registers for ED Pair (Instance 11 of 30)
  volatile uint32_t THB11;  ///< 0x258c [R/W]: Thresholding Registers for ED Pair (Instance 12 of 30)
  volatile uint32_t THB12;  ///< 0x2590 [R/W]: Thresholding Registers for ED Pair (Instance 13 of 30)
  volatile uint32_t THB13;  ///< 0x2594 [R/W]: Thresholding Registers for ED Pair (Instance 14 of 30)
  volatile uint32_t THB14;  ///< 0x2598 [R/W]: Thresholding Registers for ED Pair (Instance 15 of 30)
  volatile uint32_t THB15;  ///< 0x259c [R/W]: Thresholding Registers for ED Pair (Instance 16 of 30)
  volatile uint32_t THB16;  ///< 0x25a0 [R/W]: Thresholding Registers for ED Pair (Instance 17 of 30)
  volatile uint32_t THB17;  ///< 0x25a4 [R/W]: Thresholding Registers for ED Pair (Instance 18 of 30)
  volatile uint32_t THB18;  ///< 0x25a8 [R/W]: Thresholding Registers for ED Pair (Instance 19 of 30)
  volatile uint32_t THB19;  ///< 0x25ac [R/W]: Thresholding Registers for ED Pair (Instance 20 of 30)
  volatile uint32_t THB20;  ///< 0x25b0 [R/W]: Thresholding Registers for ED Pair (Instance 21 of 30)
  volatile uint32_t THB21;  ///< 0x25b4 [R/W]: Thresholding Registers for ED Pair (Instance 22 of 30)
  volatile uint32_t THB22;  ///< 0x25b8 [R/W]: Thresholding Registers for ED Pair (Instance 23 of 30)
  volatile uint32_t THB23;  ///< 0x25bc [R/W]: Thresholding Registers for ED Pair (Instance 24 of 30)
  volatile uint32_t THB24;  ///< 0x25c0 [R/W]: Thresholding Registers for ED Pair (Instance 25 of 30)
  volatile uint32_t THB25;  ///< 0x25c4 [R/W]: Thresholding Registers for ED Pair (Instance 26 of 30)
  volatile uint32_t THB26;  ///< 0x25c8 [R/W]: Thresholding Registers for ED Pair (Instance 27 of 30)
  volatile uint32_t THB27;  ///< 0x25cc [R/W]: Thresholding Registers for ED Pair (Instance 28 of 30)
  volatile uint32_t THB28;  ///< 0x25d0 [R/W]: Thresholding Registers for ED Pair (Instance 29 of 30)
  volatile uint32_t THB29;  ///< 0x25d4 [R/W]: Thresholding Registers for ED Pair (Instance 30 of 30)
  volatile uint32_t WLupW;  ///< 0x25d8 [R/W]: Write Window for Weight SRAM (upper 32 bits [63:32])
  volatile uint32_t WLupR0;  ///< 0x25dc [R]: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 1 of 6)
  volatile uint32_t WLupR1;  ///< 0x25e0 [R]: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 2 of 6)
  volatile uint32_t WLupR2;  ///< 0x25e4 [R]: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 3 of 6)
  volatile uint32_t WLupR3;  ///< 0x25e8 [R]: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 4 of 6)
  volatile uint32_t WLupR4;  ///< 0x25ec [R]: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 5 of 6)
  volatile uint32_t WLupR5;  ///< 0x25f0 [R]: Read Window for Weight SRAM (upper 32 bits[63:32]) (Instance 6 of 6)
  volatile uint32_t ECMP1;  ///< 0x25f4 [R/W]: Error Compression Control
  volatile uint32_t ECMP3;  ///< 0x25f8 [R/W]: Error Decompression Shift Settings
  volatile uint32_t ECA0;  ///< 0x25fc [R/W]: Error Current Access (Instance 1 of 6)
  volatile uint32_t ECA1;  ///< 0x2600 [R/W]: Error Current Access (Instance 2 of 6)
  volatile uint32_t ECA2;  ///< 0x2604 [R/W]: Error Current Access (Instance 3 of 6)
  volatile uint32_t ECA3;  ///< 0x2608 [R/W]: Error Current Access (Instance 4 of 6)
  volatile uint32_t ECA4;  ///< 0x260c [R/W]: Error Current Access (Instance 5 of 6)
  volatile uint32_t ECA5;  ///< 0x2610 [R/W]: Error Current Access (Instance 6 of 6)
  volatile uint32_t THP0;  ///< 0x2614 [R/W]: PSA Thresholding Registers (Instance 1 of 9)
  volatile uint32_t THP1;  ///< 0x2618 [R/W]: PSA Thresholding Registers (Instance 2 of 9)
  volatile uint32_t THP2;  ///< 0x261c [R/W]: PSA Thresholding Registers (Instance 3 of 9)
  volatile uint32_t THP3;  ///< 0x2620 [R/W]: PSA Thresholding Registers (Instance 4 of 9)
  volatile uint32_t THP4;  ///< 0x2624 [R/W]: PSA Thresholding Registers (Instance 5 of 9)
  volatile uint32_t THP5;  ///< 0x2628 [R/W]: PSA Thresholding Registers (Instance 6 of 9)
  volatile uint32_t THP6;  ///< 0x262c [R/W]: PSA Thresholding Registers (Instance 7 of 9)
  volatile uint32_t THP7;  ///< 0x2630 [R/W]: PSA Thresholding Registers (Instance 8 of 9)
  volatile uint32_t THP8;  ///< 0x2634 [R/W]: PSA Thresholding Registers (Instance 9 of 9)
} UED_ADVANCED_REGS_t;

#endif // UED_ADVANCED
