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
 * \file CRE_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CRE_REGSTRUCTS_H_
#define _CRE_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Color Registration Enhancment (CRE)
/** \brief Top-level register file for CRE*/
//
//====================================================================

typedef struct CRE_REGS_s
{
  volatile uint32_t STATUS;  ///< 0x0 [R]: Status Register
  volatile uint32_t CLR_STATUS;  ///< 0x4 [W]: CLEAR STATUS REG
  volatile uint32_t CNTRL;  ///< 0x8 [R/W]: CRE CONTROL REG
  volatile uint32_t TRAP_CNTRL;  ///< 0xc [R/W]: TRAPPING CONTROL REG
  volatile uint32_t TRAP_T_C;  ///< 0x10 [R/W]: TRAP CYAN THRESHOLD
  volatile uint32_t TRAP_T_M;  ///< 0x14 [R/W]: TRAP MAGENTA THRESHOLD
  volatile uint32_t TRAP_T_Y;  ///< 0x18 [R/W]: TRAP YELLOW THRESHOLD
  volatile uint32_t TRAP_T_K;  ///< 0x1c [R/W]: TRAP BLACK TRESHOLD
  volatile uint32_t TRAP_IPLIM;  ///< 0x20 [R/W]: TRAP INTENSITY PERCENTAGE LIMIT
  volatile uint32_t TRAP_I_T;  ///< 0x24 [R/W]: TRAP INTENSITY THRESHOLD
  volatile uint32_t TRAP_KLINE_T;  ///< 0x28 [R/W]: TRAP BLACK LINE THRESHOLD
  volatile uint32_t TRAP_KPLINE;  ///< 0x2c [R/W]: ADJACENT BLACK PIXELS PER LINE
  volatile uint32_t TRAP_MAX;  ///< 0x30 [R/W]: TRAP MAXIMUM
  volatile uint32_t reserved0[3];
  volatile uint32_t TRAP_LUTCI[16];  ///< 0x40 [R/W]: TRAP CYAN INTENSITY LUT
  volatile uint32_t TRAP_LUTMI[16];  ///< 0x80 [R/W]: TRAP MAGENTA INTENSITY LUT
  volatile uint32_t TRAP_LUTYI[8];  ///< 0xc0 [R/W]: TRAP YELLOW INTENSITY LUT
  volatile uint32_t TRAP_LUTKI[32];  ///< 0xe0 [R/W]: TRAP BLACK INTENSITY LUT
  volatile uint32_t reserved1[40];
  volatile uint32_t TRAP_LUTCF[16];  ///< 0x200 [R/W]: TRAP CYAN FADE LUT
  volatile uint32_t TRAP_LUTMF[16];  ///< 0x240 [R/W]: TRAP MAGENTA FADE LUT
  volatile uint32_t TRAP_LUTYF[8];  ///< 0x280 [R/W]: TRAP YELLOW FADE LUT
  volatile uint32_t reserved2[24];
  volatile uint32_t TRAP_LUTKF[32];  ///< 0x300 [R/W]: TRAP BLACK FADE LUT
  volatile uint32_t reserved3[32];
  volatile uint32_t SHARP_CNTRL;  ///< 0x400 [R/W]: SHARPEN CONTROL REG
  volatile uint32_t SHARP_SCALE_C;  ///< 0x404 [R/W]: SHARPEN SCALE CYAN
  volatile uint32_t SHARP_SCALE_M;  ///< 0x408 [R/W]: SHARPEN SCALE MAGENTA
  volatile uint32_t SHARP_SCALE_Y;  ///< 0x40c [R/W]: SHARPEN SCALE YELLOW
  volatile uint32_t SHARP_SCALE_K;  ///< 0x410 [R/W]: SHARPEN SCALE BLACK
  volatile uint32_t SHARP_OFFSET_C;  ///< 0x414 [R/W]: SHARPEN OFFSET CYAN
  volatile uint32_t SHARP_OFFSET_M;  ///< 0x418 [R/W]: SHARPEN OFFSET MAGENTA
  volatile uint32_t SHARP_OFFSET_Y;  ///< 0x41c [R/W]: SHARPEN OFFSET YELLOW
  volatile uint32_t SHARP_OFFSET_K;  ///< 0x420 [R/W]: SHARPEN OFFSET BLACK
  volatile uint32_t reserved4[3];
  volatile uint32_t EDGE_CNTRL;  ///< 0x430 [R/W]: EDGE STRENGTH CONTROL REG
  volatile uint32_t EDGE_T_C;  ///< 0x434 [R/W]: CYAN EDGE STRENGTH THRESHOLD
  volatile uint32_t EDGE_T_M;  ///< 0x438 [R/W]: MAGENTA EDGE STRENGTH THRESHOLD
  volatile uint32_t EDGE_T_Y;  ///< 0x43c [R/W]: YELLOW EDGE STRENGTH THRESHOLD
  volatile uint32_t EDGE_T_K;  ///< 0x440 [R/W]: BLACK EDGE STRENGTH THRESHOLD
  volatile uint32_t EDGE_SCALE_C;  ///< 0x444 [R/W]: CYAN EDGE STRENGTH SCALE FACTOR
  volatile uint32_t EDGE_SCALE_M;  ///< 0x448 [R/W]: MAGENTA EDGE STRENGTH SCALE FACTOR
  volatile uint32_t EDGE_SCALE_Y;  ///< 0x44c [R/W]: YELLOW EDGE STRENGTH SCALE FACTOR
  volatile uint32_t EDGE_SCALE_K;  ///< 0x450 [R/W]: BLACK EDGE STRENGTH SCALE FACTOR
  volatile uint32_t EDGE_OFFSET_C;  ///< 0x454 [R/W]: CYAN EDGE STRENGTH OFFSET
  volatile uint32_t EDGE_OFFSET_M;  ///< 0x458 [R/W]: MAGENTA EDGE STRENGTH OFFSET
  volatile uint32_t EDGE_OFFSET_Y;  ///< 0x45c [R/W]: YELLOW EDGE STRENGTH OFFSET
  volatile uint32_t EDGE_OFFSET_K;  ///< 0x460 [R/W]: BLACK EDGE STRENGTH OFFSET
  volatile uint32_t reserved5[3];
  volatile uint32_t CORE_CNTRL;  ///< 0x470 [R/W]: CORING CONTROL REG
  volatile uint32_t CORE_T_C;  ///< 0x474 [R/W]: CORE CYAN THRESHOLD
  volatile uint32_t CORE_T_M;  ///< 0x478 [R/W]: CORE MAGENTA THRESHOLD
  volatile uint32_t CORE_T_Y;  ///< 0x47c [R/W]: CORE YELLOW THRESHOLD
  volatile uint32_t CORE_T_K;  ///< 0x480 [R/W]: CORE BLACK THRESHOLD
  volatile uint32_t CORE_T_CMYK;  ///< 0x484 [R/W]: CORING CMYK THRESHOLD
  volatile uint32_t CORE_SCALE_C;  ///< 0x488 [R/W]: CORE CYAN SCALE FACTOR
  volatile uint32_t CORE_SCALE_M;  ///< 0x48c [R/W]: CORE MAGENTA SCALE FACTOR
  volatile uint32_t CORE_SCALE_Y;  ///< 0x490 [R/W]: CORE YELLOW SCALE FACTOR
  volatile uint32_t CORE_SCALE_K;  ///< 0x494 [R/W]: CORE BLACK SCALE FACTOR
  volatile uint32_t reserved6[2];
  volatile uint32_t PILE_CNTRL;  ///< 0x4a0 [R/W]: PILING CONTROL REG
  volatile uint32_t PILE_ET;  ///< 0x4a4 [R/W]: PILE EDGE THRESHOLD
  volatile uint32_t PILE_T_C;  ///< 0x4a8 [R/W]: PILE CYAN THRESHOLD
  volatile uint32_t PILE_T_M;  ///< 0x4ac [R/W]: PILE MAGENTA THRESHOLD
  volatile uint32_t PILE_T_Y;  ///< 0x4b0 [R/W]: PILE YELLOW THRESHOLD
  volatile uint32_t PILE_T_K;  ///< 0x4b4 [R/W]: PILE BLACK THRESHOLD
  volatile uint32_t PILE_SCALE_LE_C;  ///< 0x4b8 [R/W]: CYAN LEADING EDGE PILE SCALE
  volatile uint32_t PILE_SCALE_LE_M;  ///< 0x4bc [R/W]: MAGENTA LEADING EDGE PILE SCALE 
  volatile uint32_t PILE_SCALE_LE_Y;  ///< 0x4c0 [R/W]: YELLOW LEADING EDGE PILE SCALE
  volatile uint32_t PILE_SCALE_LE_K;  ///< 0x4c4 [R/W]: BLACK LEADING EDGE PILE SCALE
  volatile uint32_t PILE_SCALE_TE_C;  ///< 0x4c8 [R/W]: CYAN TRAILING EDGE PILE SCALE
  volatile uint32_t PILE_SCALE_TE_M;  ///< 0x4cc [R/W]: MAGENTA TRAILING EDGE PILE SCALE 
  volatile uint32_t PILE_SCALE_TE_Y;  ///< 0x4d0 [R/W]: YELLOW TRAILING EDGE PILE SCALE
  volatile uint32_t PILE_SCALE_TE_K;  ///< 0x4d4 [R/W]: BLACK TRAILING EDGE PILE SCALE
  volatile uint32_t reserved7[2];
  volatile uint32_t Window0;  ///< 0x4e0 [R/W]: 5x5 window pixels (Instance 1 of 40)
  volatile uint32_t Window1;  ///< 0x4e4 [R/W]: 5x5 window pixels (Instance 2 of 40)
  volatile uint32_t Window2;  ///< 0x4e8 [R/W]: 5x5 window pixels (Instance 3 of 40)
  volatile uint32_t Window3;  ///< 0x4ec [R/W]: 5x5 window pixels (Instance 4 of 40)
  volatile uint32_t Window4;  ///< 0x4f0 [R/W]: 5x5 window pixels (Instance 5 of 40)
  volatile uint32_t Window5;  ///< 0x4f4 [R/W]: 5x5 window pixels (Instance 6 of 40)
  volatile uint32_t Window6;  ///< 0x4f8 [R/W]: 5x5 window pixels (Instance 7 of 40)
  volatile uint32_t Window7;  ///< 0x4fc [R/W]: 5x5 window pixels (Instance 8 of 40)
  volatile uint32_t Window8;  ///< 0x500 [R/W]: 5x5 window pixels (Instance 9 of 40)
  volatile uint32_t Window9;  ///< 0x504 [R/W]: 5x5 window pixels (Instance 10 of 40)
  volatile uint32_t Window10;  ///< 0x508 [R/W]: 5x5 window pixels (Instance 11 of 40)
  volatile uint32_t Window11;  ///< 0x50c [R/W]: 5x5 window pixels (Instance 12 of 40)
  volatile uint32_t Window12;  ///< 0x510 [R/W]: 5x5 window pixels (Instance 13 of 40)
  volatile uint32_t Window13;  ///< 0x514 [R/W]: 5x5 window pixels (Instance 14 of 40)
  volatile uint32_t Window14;  ///< 0x518 [R/W]: 5x5 window pixels (Instance 15 of 40)
  volatile uint32_t Window15;  ///< 0x51c [R/W]: 5x5 window pixels (Instance 16 of 40)
  volatile uint32_t Window16;  ///< 0x520 [R/W]: 5x5 window pixels (Instance 17 of 40)
  volatile uint32_t Window17;  ///< 0x524 [R/W]: 5x5 window pixels (Instance 18 of 40)
  volatile uint32_t Window18;  ///< 0x528 [R/W]: 5x5 window pixels (Instance 19 of 40)
  volatile uint32_t Window19;  ///< 0x52c [R/W]: 5x5 window pixels (Instance 20 of 40)
  volatile uint32_t Window20;  ///< 0x530 [R/W]: 5x5 window pixels (Instance 21 of 40)
  volatile uint32_t Window21;  ///< 0x534 [R/W]: 5x5 window pixels (Instance 22 of 40)
  volatile uint32_t Window22;  ///< 0x538 [R/W]: 5x5 window pixels (Instance 23 of 40)
  volatile uint32_t Window23;  ///< 0x53c [R/W]: 5x5 window pixels (Instance 24 of 40)
  volatile uint32_t Window24;  ///< 0x540 [R/W]: 5x5 window pixels (Instance 25 of 40)
  volatile uint32_t Window25;  ///< 0x544 [R/W]: 5x5 window pixels (Instance 26 of 40)
  volatile uint32_t Window26;  ///< 0x548 [R/W]: 5x5 window pixels (Instance 27 of 40)
  volatile uint32_t Window27;  ///< 0x54c [R/W]: 5x5 window pixels (Instance 28 of 40)
  volatile uint32_t Window28;  ///< 0x550 [R/W]: 5x5 window pixels (Instance 29 of 40)
  volatile uint32_t Window29;  ///< 0x554 [R/W]: 5x5 window pixels (Instance 30 of 40)
  volatile uint32_t Window30;  ///< 0x558 [R/W]: 5x5 window pixels (Instance 31 of 40)
  volatile uint32_t Window31;  ///< 0x55c [R/W]: 5x5 window pixels (Instance 32 of 40)
  volatile uint32_t Window32;  ///< 0x560 [R/W]: 5x5 window pixels (Instance 33 of 40)
  volatile uint32_t Window33;  ///< 0x564 [R/W]: 5x5 window pixels (Instance 34 of 40)
  volatile uint32_t Window34;  ///< 0x568 [R/W]: 5x5 window pixels (Instance 35 of 40)
  volatile uint32_t Window35;  ///< 0x56c [R/W]: 5x5 window pixels (Instance 36 of 40)
  volatile uint32_t Window36;  ///< 0x570 [R/W]: 5x5 window pixels (Instance 37 of 40)
  volatile uint32_t Window37;  ///< 0x574 [R/W]: 5x5 window pixels (Instance 38 of 40)
  volatile uint32_t Window38;  ///< 0x578 [R/W]: 5x5 window pixels (Instance 39 of 40)
  volatile uint32_t Window39;  ///< 0x57c [R/W]: 5x5 window pixels (Instance 40 of 40)
  volatile uint32_t WinCntrl0;  ///< 0x580 [R/W]: window control storage (Instance 1 of 4)
  volatile uint32_t WinCntrl1;  ///< 0x584 [R/W]: window control storage (Instance 2 of 4)
  volatile uint32_t WinCntrl2;  ///< 0x588 [R/W]: window control storage (Instance 3 of 4)
  volatile uint32_t WinCntrl3;  ///< 0x58c [R/W]: window control storage (Instance 4 of 4)
  volatile uint32_t LBAR;  ///< 0x590 [W]: Line buffer SRAM Address and control
  volatile uint32_t LBDW3;  ///< 0x594 [R/W]: Line buffer Data Write 3
  volatile uint32_t LBDW2;  ///< 0x598 [R/W]: Write Data
  volatile uint32_t LBDW1;  ///< 0x59c [R/W]: Write Data
  volatile uint32_t LBDW0;  ///< 0x5a0 [R/W]: Write Data
  volatile uint32_t LBDR3;  ///< 0x5a4 [R]: Line Buffer Data Read 3
  volatile uint32_t LBDR2;  ///< 0x5a8 [R]: Line Buffer Data Read 2
  volatile uint32_t LBDR1;  ///< 0x5ac [R]: Line Buffer Data Read 1
  volatile uint32_t LBDR0;  ///< 0x5b0 [R]: Line Buffer Data Read 0
} CRE_REGS_t;

#endif // CRE
