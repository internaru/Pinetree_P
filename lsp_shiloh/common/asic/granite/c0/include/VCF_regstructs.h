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
 * \file VCF_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_REGSTRUCTS_H_
#define _VCF_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF Registers (VCF)
//
//====================================================================

//====================================================================
//
//Register File: CORE (CORE)
//
//====================================================================

typedef struct CORE_REGS_s
{
  volatile uint32_t VCF_STATUS;  ///< 0x0 [R]: VCF_STATUS
  volatile uint32_t reserved0[15];
  volatile uint32_t VCF_ENFORCE;  ///< 0x40 [R]: VCF_ENFORCE
  volatile uint32_t reserved1[15];
  volatile uint32_t VCF_PRE_ENFORCE;  ///< 0x80 [R]: VCF_PRE_ENFORCE
  volatile uint32_t reserved2[15];
  volatile uint32_t VCF_TRIGGERS;  ///< 0xc0 [R]: VCF_TRIGGERS
  volatile uint32_t reserved3[15];
  volatile uint32_t VCF_ERROR;  ///< 0x100 [R]: VCF_ERROR
  volatile uint32_t reserved4[15];
  volatile uint32_t VCF_IRQ_SOURCE;  ///< 0x140 [R]: VCF_IRQ_SOURCE
  volatile uint32_t reserved5[15];
  volatile uint32_t VCF_PIN_MIRROR;  ///< 0x180 [R]: VCF_PIN_MIRROR
  volatile uint32_t reserved6[15];
  volatile uint32_t VCF_PRNG;  ///< 0x1c0 [R]: VCF_PRNG
  volatile uint32_t reserved7[15];
  volatile uint32_t VCF_FUSE0;  ///< 0x200 [R]: VCF_FUSE (Instance 1 of 4)
  volatile uint32_t VCF_FUSE1;  ///< 0x204 [R]: VCF_FUSE (Instance 2 of 4)
  volatile uint32_t VCF_FUSE2;  ///< 0x208 [R]: VCF_FUSE (Instance 3 of 4)
  volatile uint32_t VCF_FUSE3;  ///< 0x20c [R]: VCF_FUSE (Instance 4 of 4)
  volatile uint32_t reserved8[12];
  volatile uint32_t VCF_EVENT;  ///< 0x240 [R]: VCF_EVENT
  volatile uint32_t reserved9[15];
  volatile uint32_t VCF_TIMERS;  ///< 0x280 [R]: VCF_TIMERS
  volatile uint32_t reserved10[15];
  volatile uint32_t VCF_VERSION;  ///< 0x2c0 [R]: VCF_VERSION
  volatile uint32_t reserved11[15];
  volatile uint32_t VCF_RECON_DATA0;  ///< 0x300 [R]: VCF_RECON_DATA (Instance 1 of 3)
  volatile uint32_t VCF_RECON_DATA1;  ///< 0x304 [R]: VCF_RECON_DATA (Instance 2 of 3)
  volatile uint32_t VCF_RECON_DATA2;  ///< 0x308 [R]: VCF_RECON_DATA (Instance 3 of 3)
  volatile uint32_t reserved12[61];
  volatile uint32_t VCF_INPUT;  ///< 0x400 [R/W]: VCF_INPUT
  volatile uint32_t reserved13[15];
  volatile uint32_t VCF_OUTPUT;  ///< 0x440 [R/W]: VCF_OUTPUT
  volatile uint32_t reserved14[15];
  volatile uint32_t VCF_IRQ_MASK;  ///< 0x480 [R/W]: VCF_IRQ_MASK
  volatile uint32_t reserved15[15];
  volatile uint32_t VCF_IRQ_STATE;  ///< 0x4c0 [R/W]: VCF_IRQ_STATE
  volatile uint32_t reserved16[15];
  volatile uint32_t VCF_MAINT_WINDOW;  ///< 0x500 [R/W]: VCF_MAINT_WINDOW
  volatile uint32_t reserved17[15];
  volatile uint32_t VCF_WARN_ADJ;  ///< 0x540 [R/W]: VCF_WARN_ADJ
  volatile uint32_t reserved18[47];
  volatile uint32_t VCF_DIAG;  ///< 0x600 [R/W]: VCF_DIAG
  volatile uint32_t reserved19[15];
  volatile uint32_t VCF_FCERT00;  ///< 0x640 [R]: VCF_FCERT0 (Instance 1 of 3)
  volatile uint32_t VCF_FCERT01;  ///< 0x644 [R]: VCF_FCERT0 (Instance 2 of 3)
  volatile uint32_t VCF_FCERT02;  ///< 0x648 [R]: VCF_FCERT0 (Instance 3 of 3)
  volatile uint32_t reserved20[13];
  volatile uint32_t VCF_FCERT1;  ///< 0x680 [R]: VCF_FCERT1
  volatile uint32_t reserved21[15];
  volatile uint32_t VCF_CCERT00;  ///< 0x6c0 [R]: VCF_CCERT0 (Instance 1 of 7)
  volatile uint32_t VCF_CCERT01;  ///< 0x6c4 [R]: VCF_CCERT0 (Instance 2 of 7)
  volatile uint32_t VCF_CCERT02;  ///< 0x6c8 [R]: VCF_CCERT0 (Instance 3 of 7)
  volatile uint32_t VCF_CCERT03;  ///< 0x6cc [R]: VCF_CCERT0 (Instance 4 of 7)
  volatile uint32_t VCF_CCERT04;  ///< 0x6d0 [R]: VCF_CCERT0 (Instance 5 of 7)
  volatile uint32_t VCF_CCERT05;  ///< 0x6d4 [R]: VCF_CCERT0 (Instance 6 of 7)
  volatile uint32_t VCF_CCERT06;  ///< 0x6d8 [R]: VCF_CCERT0 (Instance 7 of 7)
  volatile uint32_t reserved22[9];
  volatile uint32_t VCF_CCERT10;  ///< 0x700 [R]: VCF_CCERT1 (Instance 1 of 13)
  volatile uint32_t VCF_CCERT11;  ///< 0x704 [R]: VCF_CCERT1 (Instance 2 of 13)
  volatile uint32_t VCF_CCERT12;  ///< 0x708 [R]: VCF_CCERT1 (Instance 3 of 13)
  volatile uint32_t VCF_CCERT13;  ///< 0x70c [R]: VCF_CCERT1 (Instance 4 of 13)
  volatile uint32_t VCF_CCERT14;  ///< 0x710 [R]: VCF_CCERT1 (Instance 5 of 13)
  volatile uint32_t VCF_CCERT15;  ///< 0x714 [R]: VCF_CCERT1 (Instance 6 of 13)
  volatile uint32_t VCF_CCERT16;  ///< 0x718 [R]: VCF_CCERT1 (Instance 7 of 13)
  volatile uint32_t VCF_CCERT17;  ///< 0x71c [R]: VCF_CCERT1 (Instance 8 of 13)
  volatile uint32_t VCF_CCERT18;  ///< 0x720 [R]: VCF_CCERT1 (Instance 9 of 13)
  volatile uint32_t VCF_CCERT19;  ///< 0x724 [R]: VCF_CCERT1 (Instance 10 of 13)
  volatile uint32_t VCF_CCERT110;  ///< 0x728 [R]: VCF_CCERT1 (Instance 11 of 13)
  volatile uint32_t VCF_CCERT111;  ///< 0x72c [R]: VCF_CCERT1 (Instance 12 of 13)
  volatile uint32_t VCF_CCERT112;  ///< 0x730 [R]: VCF_CCERT1 (Instance 13 of 13)
  volatile uint32_t reserved23[3];
  volatile uint32_t VCF_CCERT2;  ///< 0x740 [R]: VCF_CCERT2
  volatile uint32_t reserved24[15];
  volatile uint32_t VCF_VCERT00;  ///< 0x780 [R]: VCF_VCERT0 (Instance 1 of 9)
  volatile uint32_t VCF_VCERT01;  ///< 0x784 [R]: VCF_VCERT0 (Instance 2 of 9)
  volatile uint32_t VCF_VCERT02;  ///< 0x788 [R]: VCF_VCERT0 (Instance 3 of 9)
  volatile uint32_t VCF_VCERT03;  ///< 0x78c [R]: VCF_VCERT0 (Instance 4 of 9)
  volatile uint32_t VCF_VCERT04;  ///< 0x790 [R]: VCF_VCERT0 (Instance 5 of 9)
  volatile uint32_t VCF_VCERT05;  ///< 0x794 [R]: VCF_VCERT0 (Instance 6 of 9)
  volatile uint32_t VCF_VCERT06;  ///< 0x798 [R]: VCF_VCERT0 (Instance 7 of 9)
  volatile uint32_t VCF_VCERT07;  ///< 0x79c [R]: VCF_VCERT0 (Instance 8 of 9)
  volatile uint32_t VCF_VCERT08;  ///< 0x7a0 [R]: VCF_VCERT0 (Instance 9 of 9)
  volatile uint32_t reserved25[7];
  volatile uint32_t VCF_VCERT_COMPAT0;  ///< 0x7c0 [R]: VCF_VCERT_COMPAT (Instance 1 of 3)
  volatile uint32_t VCF_VCERT_COMPAT1;  ///< 0x7c4 [R]: VCF_VCERT_COMPAT (Instance 2 of 3)
  volatile uint32_t VCF_VCERT_COMPAT2;  ///< 0x7c8 [R]: VCF_VCERT_COMPAT (Instance 3 of 3)
  volatile uint32_t reserved26[13];
  volatile uint32_t VCF_VCERT_TIMER;  ///< 0x800 [R]: VCF_VCERT_TIMER
  volatile uint32_t reserved27[15];
  volatile uint32_t VCF_VCERT_MULT_CL0;  ///< 0x840 [R]: VCF_VCERT_MULT_CL (Instance 1 of 4)
  volatile uint32_t VCF_VCERT_MULT_CL1;  ///< 0x844 [R]: VCF_VCERT_MULT_CL (Instance 2 of 4)
  volatile uint32_t VCF_VCERT_MULT_CL2;  ///< 0x848 [R]: VCF_VCERT_MULT_CL (Instance 3 of 4)
  volatile uint32_t VCF_VCERT_MULT_CL3;  ///< 0x84c [R]: VCF_VCERT_MULT_CL (Instance 4 of 4)
  volatile uint32_t reserved28[12];
  volatile uint32_t VCF_VCERT_MULT_CH0;  ///< 0x880 [R]: VCF_VCERT_MULT_CH (Instance 1 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH1;  ///< 0x884 [R]: VCF_VCERT_MULT_CH (Instance 2 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH2;  ///< 0x888 [R]: VCF_VCERT_MULT_CH (Instance 3 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH3;  ///< 0x88c [R]: VCF_VCERT_MULT_CH (Instance 4 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH4;  ///< 0x890 [R]: VCF_VCERT_MULT_CH (Instance 5 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH5;  ///< 0x894 [R]: VCF_VCERT_MULT_CH (Instance 6 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH6;  ///< 0x898 [R]: VCF_VCERT_MULT_CH (Instance 7 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH7;  ///< 0x89c [R]: VCF_VCERT_MULT_CH (Instance 8 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH8;  ///< 0x8a0 [R]: VCF_VCERT_MULT_CH (Instance 9 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH9;  ///< 0x8a4 [R]: VCF_VCERT_MULT_CH (Instance 10 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH10;  ///< 0x8a8 [R]: VCF_VCERT_MULT_CH (Instance 11 of 12)
  volatile uint32_t VCF_VCERT_MULT_CH11;  ///< 0x8ac [R]: VCF_VCERT_MULT_CH (Instance 12 of 12)
  volatile uint32_t reserved29[4];
  volatile uint32_t VCF_VCERT_USE_THRESH_CL0;  ///< 0x8c0 [R]: VCF_VCERT_USE_THRESH_CL (Instance 1 of 4)
  volatile uint32_t VCF_VCERT_USE_THRESH_CL1;  ///< 0x8c4 [R]: VCF_VCERT_USE_THRESH_CL (Instance 2 of 4)
  volatile uint32_t VCF_VCERT_USE_THRESH_CL2;  ///< 0x8c8 [R]: VCF_VCERT_USE_THRESH_CL (Instance 3 of 4)
  volatile uint32_t VCF_VCERT_USE_THRESH_CL3;  ///< 0x8cc [R]: VCF_VCERT_USE_THRESH_CL (Instance 4 of 4)
  volatile uint32_t reserved30[12];
  volatile uint32_t VCF_VCERT_USE_THRESH_CH0;  ///< 0x900 [R]: VCF_VCERT_USE_THRESH_CH (Instance 1 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH1;  ///< 0x904 [R]: VCF_VCERT_USE_THRESH_CH (Instance 2 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH2;  ///< 0x908 [R]: VCF_VCERT_USE_THRESH_CH (Instance 3 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH3;  ///< 0x90c [R]: VCF_VCERT_USE_THRESH_CH (Instance 4 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH4;  ///< 0x910 [R]: VCF_VCERT_USE_THRESH_CH (Instance 5 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH5;  ///< 0x914 [R]: VCF_VCERT_USE_THRESH_CH (Instance 6 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH6;  ///< 0x918 [R]: VCF_VCERT_USE_THRESH_CH (Instance 7 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH7;  ///< 0x91c [R]: VCF_VCERT_USE_THRESH_CH (Instance 8 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH8;  ///< 0x920 [R]: VCF_VCERT_USE_THRESH_CH (Instance 9 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH9;  ///< 0x924 [R]: VCF_VCERT_USE_THRESH_CH (Instance 10 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH10;  ///< 0x928 [R]: VCF_VCERT_USE_THRESH_CH (Instance 11 of 12)
  volatile uint32_t VCF_VCERT_USE_THRESH_CH11;  ///< 0x92c [R]: VCF_VCERT_USE_THRESH_CH (Instance 12 of 12)
  volatile uint32_t reserved31[4];
  volatile uint32_t VCF_VCERT_DATA1_CL0;  ///< 0x940 [R]: VCF_VCERT_DATA1_CL (Instance 1 of 4)
  volatile uint32_t VCF_VCERT_DATA1_CL1;  ///< 0x944 [R]: VCF_VCERT_DATA1_CL (Instance 2 of 4)
  volatile uint32_t VCF_VCERT_DATA1_CL2;  ///< 0x948 [R]: VCF_VCERT_DATA1_CL (Instance 3 of 4)
  volatile uint32_t VCF_VCERT_DATA1_CL3;  ///< 0x94c [R]: VCF_VCERT_DATA1_CL (Instance 4 of 4)
  volatile uint32_t reserved32[12];
  volatile uint32_t VCF_VCERT_DATA1_CH0;  ///< 0x980 [R]: VCF_VCERT_DATA1_CH (Instance 1 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH1;  ///< 0x984 [R]: VCF_VCERT_DATA1_CH (Instance 2 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH2;  ///< 0x988 [R]: VCF_VCERT_DATA1_CH (Instance 3 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH3;  ///< 0x98c [R]: VCF_VCERT_DATA1_CH (Instance 4 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH4;  ///< 0x990 [R]: VCF_VCERT_DATA1_CH (Instance 5 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH5;  ///< 0x994 [R]: VCF_VCERT_DATA1_CH (Instance 6 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH6;  ///< 0x998 [R]: VCF_VCERT_DATA1_CH (Instance 7 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH7;  ///< 0x99c [R]: VCF_VCERT_DATA1_CH (Instance 8 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH8;  ///< 0x9a0 [R]: VCF_VCERT_DATA1_CH (Instance 9 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH9;  ///< 0x9a4 [R]: VCF_VCERT_DATA1_CH (Instance 10 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH10;  ///< 0x9a8 [R]: VCF_VCERT_DATA1_CH (Instance 11 of 12)
  volatile uint32_t VCF_VCERT_DATA1_CH11;  ///< 0x9ac [R]: VCF_VCERT_DATA1_CH (Instance 12 of 12)
  volatile uint32_t reserved33[4];
  volatile uint32_t VCF_VCERT_DATA2_CL0;  ///< 0x9c0 [R]: VCF_VCERT_DATA2_CL (Instance 1 of 4)
  volatile uint32_t VCF_VCERT_DATA2_CL1;  ///< 0x9c4 [R]: VCF_VCERT_DATA2_CL (Instance 2 of 4)
  volatile uint32_t VCF_VCERT_DATA2_CL2;  ///< 0x9c8 [R]: VCF_VCERT_DATA2_CL (Instance 3 of 4)
  volatile uint32_t VCF_VCERT_DATA2_CL3;  ///< 0x9cc [R]: VCF_VCERT_DATA2_CL (Instance 4 of 4)
  volatile uint32_t reserved34[12];
  volatile uint32_t VCF_VCERT_DATA2_CH0;  ///< 0xa00 [R]: VCF_VCERT_DATA2_CH (Instance 1 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH1;  ///< 0xa04 [R]: VCF_VCERT_DATA2_CH (Instance 2 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH2;  ///< 0xa08 [R]: VCF_VCERT_DATA2_CH (Instance 3 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH3;  ///< 0xa0c [R]: VCF_VCERT_DATA2_CH (Instance 4 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH4;  ///< 0xa10 [R]: VCF_VCERT_DATA2_CH (Instance 5 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH5;  ///< 0xa14 [R]: VCF_VCERT_DATA2_CH (Instance 6 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH6;  ///< 0xa18 [R]: VCF_VCERT_DATA2_CH (Instance 7 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH7;  ///< 0xa1c [R]: VCF_VCERT_DATA2_CH (Instance 8 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH8;  ///< 0xa20 [R]: VCF_VCERT_DATA2_CH (Instance 9 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH9;  ///< 0xa24 [R]: VCF_VCERT_DATA2_CH (Instance 10 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH10;  ///< 0xa28 [R]: VCF_VCERT_DATA2_CH (Instance 11 of 12)
  volatile uint32_t VCF_VCERT_DATA2_CH11;  ///< 0xa2c [R]: VCF_VCERT_DATA2_CH (Instance 12 of 12)
  volatile uint32_t reserved35[4];
  volatile uint32_t VCF_WARN_USE_THRESH_CL0;  ///< 0xa40 [R/W]: VCF_WARN_USE_THRESH_CL (Instance 1 of 4)
  volatile uint32_t VCF_WARN_USE_THRESH_CL1;  ///< 0xa44 [R/W]: VCF_WARN_USE_THRESH_CL (Instance 2 of 4)
  volatile uint32_t VCF_WARN_USE_THRESH_CL2;  ///< 0xa48 [R/W]: VCF_WARN_USE_THRESH_CL (Instance 3 of 4)
  volatile uint32_t VCF_WARN_USE_THRESH_CL3;  ///< 0xa4c [R/W]: VCF_WARN_USE_THRESH_CL (Instance 4 of 4)
  volatile uint32_t reserved36[108];
  volatile uint32_t VCF_WARN_USE_THRESH_CH0;  ///< 0xc00 [R]: VCF_WARN_USE_THRESH_CH (Instance 1 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH1;  ///< 0xc04 [R]: VCF_WARN_USE_THRESH_CH (Instance 2 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH2;  ///< 0xc08 [R]: VCF_WARN_USE_THRESH_CH (Instance 3 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH3;  ///< 0xc0c [R]: VCF_WARN_USE_THRESH_CH (Instance 4 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH4;  ///< 0xc10 [R]: VCF_WARN_USE_THRESH_CH (Instance 5 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH5;  ///< 0xc14 [R]: VCF_WARN_USE_THRESH_CH (Instance 6 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH6;  ///< 0xc18 [R]: VCF_WARN_USE_THRESH_CH (Instance 7 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH7;  ///< 0xc1c [R]: VCF_WARN_USE_THRESH_CH (Instance 8 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH8;  ///< 0xc20 [R]: VCF_WARN_USE_THRESH_CH (Instance 9 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH9;  ///< 0xc24 [R]: VCF_WARN_USE_THRESH_CH (Instance 10 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH10;  ///< 0xc28 [R]: VCF_WARN_USE_THRESH_CH (Instance 11 of 12)
  volatile uint32_t VCF_WARN_USE_THRESH_CH11;  ///< 0xc2c [R]: VCF_WARN_USE_THRESH_CH (Instance 12 of 12)
  volatile uint32_t reserved37[4];
  volatile uint32_t VCF_AUTH_CH0;  ///< 0xc40 [R]: VCF_AUTH_CH (Instance 1 of 12)
  volatile uint32_t VCF_AUTH_CH1;  ///< 0xc44 [R]: VCF_AUTH_CH (Instance 2 of 12)
  volatile uint32_t VCF_AUTH_CH2;  ///< 0xc48 [R]: VCF_AUTH_CH (Instance 3 of 12)
  volatile uint32_t VCF_AUTH_CH3;  ///< 0xc4c [R]: VCF_AUTH_CH (Instance 4 of 12)
  volatile uint32_t VCF_AUTH_CH4;  ///< 0xc50 [R]: VCF_AUTH_CH (Instance 5 of 12)
  volatile uint32_t VCF_AUTH_CH5;  ///< 0xc54 [R]: VCF_AUTH_CH (Instance 6 of 12)
  volatile uint32_t VCF_AUTH_CH6;  ///< 0xc58 [R]: VCF_AUTH_CH (Instance 7 of 12)
  volatile uint32_t VCF_AUTH_CH7;  ///< 0xc5c [R]: VCF_AUTH_CH (Instance 8 of 12)
  volatile uint32_t VCF_AUTH_CH8;  ///< 0xc60 [R]: VCF_AUTH_CH (Instance 9 of 12)
  volatile uint32_t VCF_AUTH_CH9;  ///< 0xc64 [R]: VCF_AUTH_CH (Instance 10 of 12)
  volatile uint32_t VCF_AUTH_CH10;  ///< 0xc68 [R]: VCF_AUTH_CH (Instance 11 of 12)
  volatile uint32_t VCF_AUTH_CH11;  ///< 0xc6c [R]: VCF_AUTH_CH (Instance 12 of 12)
  volatile uint32_t reserved38[4];
  volatile uint32_t VCF_AUTHUSE_LO_CH0;  ///< 0xc80 [R]: VCF_AUTHUSE_LO_CH (Instance 1 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH1;  ///< 0xc84 [R]: VCF_AUTHUSE_LO_CH (Instance 2 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH2;  ///< 0xc88 [R]: VCF_AUTHUSE_LO_CH (Instance 3 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH3;  ///< 0xc8c [R]: VCF_AUTHUSE_LO_CH (Instance 4 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH4;  ///< 0xc90 [R]: VCF_AUTHUSE_LO_CH (Instance 5 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH5;  ///< 0xc94 [R]: VCF_AUTHUSE_LO_CH (Instance 6 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH6;  ///< 0xc98 [R]: VCF_AUTHUSE_LO_CH (Instance 7 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH7;  ///< 0xc9c [R]: VCF_AUTHUSE_LO_CH (Instance 8 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH8;  ///< 0xca0 [R]: VCF_AUTHUSE_LO_CH (Instance 9 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH9;  ///< 0xca4 [R]: VCF_AUTHUSE_LO_CH (Instance 10 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH10;  ///< 0xca8 [R]: VCF_AUTHUSE_LO_CH (Instance 11 of 12)
  volatile uint32_t VCF_AUTHUSE_LO_CH11;  ///< 0xcac [R]: VCF_AUTHUSE_LO_CH (Instance 12 of 12)
  volatile uint32_t reserved39[4];
  volatile uint32_t VCF_AUTHUSE_HI_CH0;  ///< 0xcc0 [R]: VCF_AUTHUSE_HI_CH (Instance 1 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH1;  ///< 0xcc4 [R]: VCF_AUTHUSE_HI_CH (Instance 2 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH2;  ///< 0xcc8 [R]: VCF_AUTHUSE_HI_CH (Instance 3 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH3;  ///< 0xccc [R]: VCF_AUTHUSE_HI_CH (Instance 4 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH4;  ///< 0xcd0 [R]: VCF_AUTHUSE_HI_CH (Instance 5 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH5;  ///< 0xcd4 [R]: VCF_AUTHUSE_HI_CH (Instance 6 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH6;  ///< 0xcd8 [R]: VCF_AUTHUSE_HI_CH (Instance 7 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH7;  ///< 0xcdc [R]: VCF_AUTHUSE_HI_CH (Instance 8 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH8;  ///< 0xce0 [R]: VCF_AUTHUSE_HI_CH (Instance 9 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH9;  ///< 0xce4 [R]: VCF_AUTHUSE_HI_CH (Instance 10 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH10;  ///< 0xce8 [R]: VCF_AUTHUSE_HI_CH (Instance 11 of 12)
  volatile uint32_t VCF_AUTHUSE_HI_CH11;  ///< 0xcec [R]: VCF_AUTHUSE_HI_CH (Instance 12 of 12)
  volatile uint32_t reserved40[4];
  volatile uint32_t VCF_OP_METER_LO;  ///< 0xd00 [R]: VCF_OP_METER_LO
  volatile uint32_t reserved41[15];
  volatile uint32_t VCF_OP_METER_HI;  ///< 0xd40 [R]: VCF_OP_METER_HI
  volatile uint32_t reserved42[15];
  volatile uint32_t VCF_SCERT0;  ///< 0xd80 [R]: VCF_SCERT (Instance 1 of 10)
  volatile uint32_t VCF_SCERT1;  ///< 0xd84 [R]: VCF_SCERT (Instance 2 of 10)
  volatile uint32_t VCF_SCERT2;  ///< 0xd88 [R]: VCF_SCERT (Instance 3 of 10)
  volatile uint32_t VCF_SCERT3;  ///< 0xd8c [R]: VCF_SCERT (Instance 4 of 10)
  volatile uint32_t VCF_SCERT4;  ///< 0xd90 [R]: VCF_SCERT (Instance 5 of 10)
  volatile uint32_t VCF_SCERT5;  ///< 0xd94 [R]: VCF_SCERT (Instance 6 of 10)
  volatile uint32_t VCF_SCERT6;  ///< 0xd98 [R]: VCF_SCERT (Instance 7 of 10)
  volatile uint32_t VCF_SCERT7;  ///< 0xd9c [R]: VCF_SCERT (Instance 8 of 10)
  volatile uint32_t VCF_SCERT8;  ///< 0xda0 [R]: VCF_SCERT (Instance 9 of 10)
  volatile uint32_t VCF_SCERT9;  ///< 0xda4 [R]: VCF_SCERT (Instance 10 of 10)
  volatile uint32_t reserved43[22];
  volatile uint32_t VCF_CERT0;  ///< 0xe00 [R]: VCF_CERT (Instance 1 of 4)
  volatile uint32_t VCF_CERT1;  ///< 0xe04 [R]: VCF_CERT (Instance 2 of 4)
  volatile uint32_t VCF_CERT2;  ///< 0xe08 [R]: VCF_CERT (Instance 3 of 4)
  volatile uint32_t VCF_CERT3;  ///< 0xe0c [R]: VCF_CERT (Instance 4 of 4)
} CORE_REGS_t;

//====================================================================
//
//Register File: WRAPPER (WRAPPER)
//
//====================================================================

typedef struct WRAPPER_REGS_s
{
  volatile uint32_t VALR0;  ///< 0x0 [R]: VCF Accumulator Low Register (Instance 1 of 12)
  volatile uint32_t VAHR0;  ///< 0x4 [R]: VCF Accumulator High Register (Instance 1 of 12)
  volatile uint32_t VALR1;  ///< 0x8 [R]: VCF Accumulator Low Register (Instance 2 of 12)
  volatile uint32_t VAHR1;  ///< 0xc [R]: VCF Accumulator High Register (Instance 2 of 12)
  volatile uint32_t VALR2;  ///< 0x10 [R]: VCF Accumulator Low Register (Instance 3 of 12)
  volatile uint32_t VAHR2;  ///< 0x14 [R]: VCF Accumulator High Register (Instance 3 of 12)
  volatile uint32_t VALR3;  ///< 0x18 [R]: VCF Accumulator Low Register (Instance 4 of 12)
  volatile uint32_t VAHR3;  ///< 0x1c [R]: VCF Accumulator High Register (Instance 4 of 12)
  volatile uint32_t VALR4;  ///< 0x20 [R]: VCF Accumulator Low Register (Instance 5 of 12)
  volatile uint32_t VAHR4;  ///< 0x24 [R]: VCF Accumulator High Register (Instance 5 of 12)
  volatile uint32_t VALR5;  ///< 0x28 [R]: VCF Accumulator Low Register (Instance 6 of 12)
  volatile uint32_t VAHR5;  ///< 0x2c [R]: VCF Accumulator High Register (Instance 6 of 12)
  volatile uint32_t VALR6;  ///< 0x30 [R]: VCF Accumulator Low Register (Instance 7 of 12)
  volatile uint32_t VAHR6;  ///< 0x34 [R]: VCF Accumulator High Register (Instance 7 of 12)
  volatile uint32_t VALR7;  ///< 0x38 [R]: VCF Accumulator Low Register (Instance 8 of 12)
  volatile uint32_t VAHR7;  ///< 0x3c [R]: VCF Accumulator High Register (Instance 8 of 12)
  volatile uint32_t VALR8;  ///< 0x40 [R]: VCF Accumulator Low Register (Instance 9 of 12)
  volatile uint32_t VAHR8;  ///< 0x44 [R]: VCF Accumulator High Register (Instance 9 of 12)
  volatile uint32_t VALR9;  ///< 0x48 [R]: VCF Accumulator Low Register (Instance 10 of 12)
  volatile uint32_t VAHR9;  ///< 0x4c [R]: VCF Accumulator High Register (Instance 10 of 12)
  volatile uint32_t VALR10;  ///< 0x50 [R]: VCF Accumulator Low Register (Instance 11 of 12)
  volatile uint32_t VAHR10;  ///< 0x54 [R]: VCF Accumulator High Register (Instance 11 of 12)
  volatile uint32_t VALR11;  ///< 0x58 [R]: VCF Accumulator Low Register (Instance 12 of 12)
  volatile uint32_t VAHR11;  ///< 0x5c [R]: VCF Accumulator High Register (Instance 12 of 12)
  volatile uint32_t reserved0[8];
  volatile uint32_t VAIR0;  ///< 0x80 [W]: VCF Accumulator Increment Register (Instance 1 of 12)
  volatile uint32_t VAIR1;  ///< 0x84 [W]: VCF Accumulator Increment Register (Instance 2 of 12)
  volatile uint32_t VAIR2;  ///< 0x88 [W]: VCF Accumulator Increment Register (Instance 3 of 12)
  volatile uint32_t VAIR3;  ///< 0x8c [W]: VCF Accumulator Increment Register (Instance 4 of 12)
  volatile uint32_t VAIR4;  ///< 0x90 [W]: VCF Accumulator Increment Register (Instance 5 of 12)
  volatile uint32_t VAIR5;  ///< 0x94 [W]: VCF Accumulator Increment Register (Instance 6 of 12)
  volatile uint32_t VAIR6;  ///< 0x98 [W]: VCF Accumulator Increment Register (Instance 7 of 12)
  volatile uint32_t VAIR7;  ///< 0x9c [W]: VCF Accumulator Increment Register (Instance 8 of 12)
  volatile uint32_t VAIR8;  ///< 0xa0 [W]: VCF Accumulator Increment Register (Instance 9 of 12)
  volatile uint32_t VAIR9;  ///< 0xa4 [W]: VCF Accumulator Increment Register (Instance 10 of 12)
  volatile uint32_t VAIR10;  ///< 0xa8 [W]: VCF Accumulator Increment Register (Instance 11 of 12)
  volatile uint32_t VAIR11;  ///< 0xac [W]: VCF Accumulator Increment Register (Instance 12 of 12)
  volatile uint32_t reserved1[4];
  volatile uint32_t VMHR0;  ///< 0xc0 [R]: VCF MCPU Hash Register (Instance 1 of 6)
  volatile uint32_t VMHR1;  ///< 0xc4 [R]: VCF MCPU Hash Register (Instance 2 of 6)
  volatile uint32_t VMHR2;  ///< 0xc8 [R]: VCF MCPU Hash Register (Instance 3 of 6)
  volatile uint32_t VMHR3;  ///< 0xcc [R]: VCF MCPU Hash Register (Instance 4 of 6)
  volatile uint32_t VMHR4;  ///< 0xd0 [R]: VCF MCPU Hash Register (Instance 5 of 6)
  volatile uint32_t VMHR5;  ///< 0xd4 [R]: VCF MCPU Hash Register (Instance 6 of 6)
  volatile uint32_t reserved2[2];
  volatile uint32_t VIHR00;  ///< 0xe0 [R]: VCF ICPU0 Hash Register (Instance 1 of 6)
  volatile uint32_t VIHR01;  ///< 0xe4 [R]: VCF ICPU0 Hash Register (Instance 2 of 6)
  volatile uint32_t VIHR02;  ///< 0xe8 [R]: VCF ICPU0 Hash Register (Instance 3 of 6)
  volatile uint32_t VIHR03;  ///< 0xec [R]: VCF ICPU0 Hash Register (Instance 4 of 6)
  volatile uint32_t VIHR04;  ///< 0xf0 [R]: VCF ICPU0 Hash Register (Instance 5 of 6)
  volatile uint32_t VIHR05;  ///< 0xf4 [R]: VCF ICPU0 Hash Register (Instance 6 of 6)
  volatile uint32_t reserved3[2];
  volatile uint32_t VIHR10;  ///< 0x100 [R]: VCF ICPU1 Hash Register (Instance 1 of 6)
  volatile uint32_t VIHR11;  ///< 0x104 [R]: VCF ICPU1 Hash Register (Instance 2 of 6)
  volatile uint32_t VIHR12;  ///< 0x108 [R]: VCF ICPU1 Hash Register (Instance 3 of 6)
  volatile uint32_t VIHR13;  ///< 0x10c [R]: VCF ICPU1 Hash Register (Instance 4 of 6)
  volatile uint32_t VIHR14;  ///< 0x110 [R]: VCF ICPU1 Hash Register (Instance 5 of 6)
  volatile uint32_t VIHR15;  ///< 0x114 [R]: VCF ICPU1 Hash Register (Instance 6 of 6)
  volatile uint32_t reserved4[2];
  volatile uint32_t VWSR;  ///< 0x120 [R]: VCF Wrapper Status Register
  volatile uint32_t VMFR;  ///< 0x124 [W]: VCF Minimum Function Register
  volatile uint32_t VLMR;  ///< 0x128 [R/W]: VCF Laser Map Register
  volatile uint32_t VLER;  ///< 0x12c [R]: VCF Laser Enforce Register
  volatile uint32_t VIER0;  ///< 0x130 [R]: VCF Ink Enforce Register 0
  volatile uint32_t VIER1;  ///< 0x134 [R]: VCF Ink Enforce Register 1
} WRAPPER_REGS_t;

//====================================================================
//
//Register File: ROM (ROM)
//
//====================================================================

typedef struct ROM_REGS_s
{
  volatile uint32_t ROM[4096];  ///< 0x0 [R/W]: ROM
} ROM_REGS_t;

#endif // VCF
