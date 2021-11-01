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
 * \file DPWM_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DPWM_TOP_REGS_REGSTRUCTS_H_
#define _DPWM_TOP_REGS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: dpwm_top_regs (dpwm_top_regs)
//
//====================================================================

typedef struct DPWM_TOP_REGS_REGS_s
{
  volatile uint32_t en;  ///< 0x0 [R/W]: PWM Enable register
  volatile uint32_t intp;  ///< 0x4 [R]: PWM Interrupt Pending register
} DPWM_TOP_REGS_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 1 of 32) (dpwm_regs0)
//
//====================================================================

typedef struct DPWM_REGS0_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS0_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 2 of 32) (dpwm_regs1)
//
//====================================================================

typedef struct DPWM_REGS1_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS1_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 3 of 32) (dpwm_regs2)
//
//====================================================================

typedef struct DPWM_REGS2_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS2_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 4 of 32) (dpwm_regs3)
//
//====================================================================

typedef struct DPWM_REGS3_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS3_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 5 of 32) (dpwm_regs4)
//
//====================================================================

typedef struct DPWM_REGS4_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS4_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 6 of 32) (dpwm_regs5)
//
//====================================================================

typedef struct DPWM_REGS5_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS5_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 7 of 32) (dpwm_regs6)
//
//====================================================================

typedef struct DPWM_REGS6_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS6_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 8 of 32) (dpwm_regs7)
//
//====================================================================

typedef struct DPWM_REGS7_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS7_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 9 of 32) (dpwm_regs8)
//
//====================================================================

typedef struct DPWM_REGS8_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS8_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 10 of 32) (dpwm_regs9)
//
//====================================================================

typedef struct DPWM_REGS9_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS9_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 11 of 32) (dpwm_regs10)
//
//====================================================================

typedef struct DPWM_REGS10_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS10_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 12 of 32) (dpwm_regs11)
//
//====================================================================

typedef struct DPWM_REGS11_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS11_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 13 of 32) (dpwm_regs12)
//
//====================================================================

typedef struct DPWM_REGS12_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS12_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 14 of 32) (dpwm_regs13)
//
//====================================================================

typedef struct DPWM_REGS13_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS13_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 15 of 32) (dpwm_regs14)
//
//====================================================================

typedef struct DPWM_REGS14_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS14_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 16 of 32) (dpwm_regs15)
//
//====================================================================

typedef struct DPWM_REGS15_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS15_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 17 of 32) (dpwm_regs16)
//
//====================================================================

typedef struct DPWM_REGS16_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS16_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 18 of 32) (dpwm_regs17)
//
//====================================================================

typedef struct DPWM_REGS17_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS17_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 19 of 32) (dpwm_regs18)
//
//====================================================================

typedef struct DPWM_REGS18_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS18_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 20 of 32) (dpwm_regs19)
//
//====================================================================

typedef struct DPWM_REGS19_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS19_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 21 of 32) (dpwm_regs20)
//
//====================================================================

typedef struct DPWM_REGS20_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS20_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 22 of 32) (dpwm_regs21)
//
//====================================================================

typedef struct DPWM_REGS21_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS21_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 23 of 32) (dpwm_regs22)
//
//====================================================================

typedef struct DPWM_REGS22_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS22_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 24 of 32) (dpwm_regs23)
//
//====================================================================

typedef struct DPWM_REGS23_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS23_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 25 of 32) (dpwm_regs24)
//
//====================================================================

typedef struct DPWM_REGS24_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS24_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 26 of 32) (dpwm_regs25)
//
//====================================================================

typedef struct DPWM_REGS25_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS25_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 27 of 32) (dpwm_regs26)
//
//====================================================================

typedef struct DPWM_REGS26_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS26_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 28 of 32) (dpwm_regs27)
//
//====================================================================

typedef struct DPWM_REGS27_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS27_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 29 of 32) (dpwm_regs28)
//
//====================================================================

typedef struct DPWM_REGS28_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS28_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 30 of 32) (dpwm_regs29)
//
//====================================================================

typedef struct DPWM_REGS29_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS29_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 31 of 32) (dpwm_regs30)
//
//====================================================================

typedef struct DPWM_REGS30_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS30_REGS_t;

//====================================================================
//
//Register File: DPWM (Instance 32 of 32) (dpwm_regs31)
//
//====================================================================

typedef struct DPWM_REGS31_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_en;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_st;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_cl;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} DPWM_REGS31_REGS_t;

#endif // DPWM_TOP_REGS
