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
 * \file SENSOR_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SENSOR_REGS_REGSTRUCTS_H_
#define _SENSOR_REGS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: sensor_regs (sensor_regs)
//
//====================================================================

typedef struct SENSOR_REGS_REGS_s
{
  volatile uint32_t cfg0;  ///< 0x0 [R/W]: Configuration register (Instance 1 of 19)
  volatile uint32_t thresh0;  ///< 0x4 [R/W]: Filter Threshold register (Instance 1 of 19)
  volatile uint32_t risew0;  ///< 0x8 [R/W]: Filter Rise Weight register (Instance 1 of 19)
  volatile uint32_t fallw0;  ///< 0xc [R/W]: Filter Fall Weight register (Instance 1 of 19)
  volatile uint32_t cfg1;  ///< 0x10 [R/W]: Configuration register (Instance 2 of 19)
  volatile uint32_t thresh1;  ///< 0x14 [R/W]: Filter Threshold register (Instance 2 of 19)
  volatile uint32_t risew1;  ///< 0x18 [R/W]: Filter Rise Weight register (Instance 2 of 19)
  volatile uint32_t fallw1;  ///< 0x1c [R/W]: Filter Fall Weight register (Instance 2 of 19)
  volatile uint32_t cfg2;  ///< 0x20 [R/W]: Configuration register (Instance 3 of 19)
  volatile uint32_t thresh2;  ///< 0x24 [R/W]: Filter Threshold register (Instance 3 of 19)
  volatile uint32_t risew2;  ///< 0x28 [R/W]: Filter Rise Weight register (Instance 3 of 19)
  volatile uint32_t fallw2;  ///< 0x2c [R/W]: Filter Fall Weight register (Instance 3 of 19)
  volatile uint32_t cfg3;  ///< 0x30 [R/W]: Configuration register (Instance 4 of 19)
  volatile uint32_t thresh3;  ///< 0x34 [R/W]: Filter Threshold register (Instance 4 of 19)
  volatile uint32_t risew3;  ///< 0x38 [R/W]: Filter Rise Weight register (Instance 4 of 19)
  volatile uint32_t fallw3;  ///< 0x3c [R/W]: Filter Fall Weight register (Instance 4 of 19)
  volatile uint32_t cfg4;  ///< 0x40 [R/W]: Configuration register (Instance 5 of 19)
  volatile uint32_t thresh4;  ///< 0x44 [R/W]: Filter Threshold register (Instance 5 of 19)
  volatile uint32_t risew4;  ///< 0x48 [R/W]: Filter Rise Weight register (Instance 5 of 19)
  volatile uint32_t fallw4;  ///< 0x4c [R/W]: Filter Fall Weight register (Instance 5 of 19)
  volatile uint32_t cfg5;  ///< 0x50 [R/W]: Configuration register (Instance 6 of 19)
  volatile uint32_t thresh5;  ///< 0x54 [R/W]: Filter Threshold register (Instance 6 of 19)
  volatile uint32_t risew5;  ///< 0x58 [R/W]: Filter Rise Weight register (Instance 6 of 19)
  volatile uint32_t fallw5;  ///< 0x5c [R/W]: Filter Fall Weight register (Instance 6 of 19)
  volatile uint32_t cfg6;  ///< 0x60 [R/W]: Configuration register (Instance 7 of 19)
  volatile uint32_t thresh6;  ///< 0x64 [R/W]: Filter Threshold register (Instance 7 of 19)
  volatile uint32_t risew6;  ///< 0x68 [R/W]: Filter Rise Weight register (Instance 7 of 19)
  volatile uint32_t fallw6;  ///< 0x6c [R/W]: Filter Fall Weight register (Instance 7 of 19)
  volatile uint32_t cfg7;  ///< 0x70 [R/W]: Configuration register (Instance 8 of 19)
  volatile uint32_t thresh7;  ///< 0x74 [R/W]: Filter Threshold register (Instance 8 of 19)
  volatile uint32_t risew7;  ///< 0x78 [R/W]: Filter Rise Weight register (Instance 8 of 19)
  volatile uint32_t fallw7;  ///< 0x7c [R/W]: Filter Fall Weight register (Instance 8 of 19)
  volatile uint32_t cfg8;  ///< 0x80 [R/W]: Configuration register (Instance 9 of 19)
  volatile uint32_t thresh8;  ///< 0x84 [R/W]: Filter Threshold register (Instance 9 of 19)
  volatile uint32_t risew8;  ///< 0x88 [R/W]: Filter Rise Weight register (Instance 9 of 19)
  volatile uint32_t fallw8;  ///< 0x8c [R/W]: Filter Fall Weight register (Instance 9 of 19)
  volatile uint32_t cfg9;  ///< 0x90 [R/W]: Configuration register (Instance 10 of 19)
  volatile uint32_t thresh9;  ///< 0x94 [R/W]: Filter Threshold register (Instance 10 of 19)
  volatile uint32_t risew9;  ///< 0x98 [R/W]: Filter Rise Weight register (Instance 10 of 19)
  volatile uint32_t fallw9;  ///< 0x9c [R/W]: Filter Fall Weight register (Instance 10 of 19)
  volatile uint32_t cfg10;  ///< 0xa0 [R/W]: Configuration register (Instance 11 of 19)
  volatile uint32_t thresh10;  ///< 0xa4 [R/W]: Filter Threshold register (Instance 11 of 19)
  volatile uint32_t risew10;  ///< 0xa8 [R/W]: Filter Rise Weight register (Instance 11 of 19)
  volatile uint32_t fallw10;  ///< 0xac [R/W]: Filter Fall Weight register (Instance 11 of 19)
  volatile uint32_t cfg11;  ///< 0xb0 [R/W]: Configuration register (Instance 12 of 19)
  volatile uint32_t thresh11;  ///< 0xb4 [R/W]: Filter Threshold register (Instance 12 of 19)
  volatile uint32_t risew11;  ///< 0xb8 [R/W]: Filter Rise Weight register (Instance 12 of 19)
  volatile uint32_t fallw11;  ///< 0xbc [R/W]: Filter Fall Weight register (Instance 12 of 19)
  volatile uint32_t cfg12;  ///< 0xc0 [R/W]: Configuration register (Instance 13 of 19)
  volatile uint32_t thresh12;  ///< 0xc4 [R/W]: Filter Threshold register (Instance 13 of 19)
  volatile uint32_t risew12;  ///< 0xc8 [R/W]: Filter Rise Weight register (Instance 13 of 19)
  volatile uint32_t fallw12;  ///< 0xcc [R/W]: Filter Fall Weight register (Instance 13 of 19)
  volatile uint32_t cfg13;  ///< 0xd0 [R/W]: Configuration register (Instance 14 of 19)
  volatile uint32_t thresh13;  ///< 0xd4 [R/W]: Filter Threshold register (Instance 14 of 19)
  volatile uint32_t risew13;  ///< 0xd8 [R/W]: Filter Rise Weight register (Instance 14 of 19)
  volatile uint32_t fallw13;  ///< 0xdc [R/W]: Filter Fall Weight register (Instance 14 of 19)
  volatile uint32_t cfg14;  ///< 0xe0 [R/W]: Configuration register (Instance 15 of 19)
  volatile uint32_t thresh14;  ///< 0xe4 [R/W]: Filter Threshold register (Instance 15 of 19)
  volatile uint32_t risew14;  ///< 0xe8 [R/W]: Filter Rise Weight register (Instance 15 of 19)
  volatile uint32_t fallw14;  ///< 0xec [R/W]: Filter Fall Weight register (Instance 15 of 19)
  volatile uint32_t cfg15;  ///< 0xf0 [R/W]: Configuration register (Instance 16 of 19)
  volatile uint32_t thresh15;  ///< 0xf4 [R/W]: Filter Threshold register (Instance 16 of 19)
  volatile uint32_t risew15;  ///< 0xf8 [R/W]: Filter Rise Weight register (Instance 16 of 19)
  volatile uint32_t fallw15;  ///< 0xfc [R/W]: Filter Fall Weight register (Instance 16 of 19)
  volatile uint32_t cfg16;  ///< 0x100 [R/W]: Configuration register (Instance 17 of 19)
  volatile uint32_t thresh16;  ///< 0x104 [R/W]: Filter Threshold register (Instance 17 of 19)
  volatile uint32_t risew16;  ///< 0x108 [R/W]: Filter Rise Weight register (Instance 17 of 19)
  volatile uint32_t fallw16;  ///< 0x10c [R/W]: Filter Fall Weight register (Instance 17 of 19)
  volatile uint32_t cfg17;  ///< 0x110 [R/W]: Configuration register (Instance 18 of 19)
  volatile uint32_t thresh17;  ///< 0x114 [R/W]: Filter Threshold register (Instance 18 of 19)
  volatile uint32_t risew17;  ///< 0x118 [R/W]: Filter Rise Weight register (Instance 18 of 19)
  volatile uint32_t fallw17;  ///< 0x11c [R/W]: Filter Fall Weight register (Instance 18 of 19)
  volatile uint32_t cfg18;  ///< 0x120 [R/W]: Configuration register (Instance 19 of 19)
  volatile uint32_t thresh18;  ///< 0x124 [R/W]: Filter Threshold register (Instance 19 of 19)
  volatile uint32_t risew18;  ///< 0x128 [R/W]: Filter Rise Weight register (Instance 19 of 19)
  volatile uint32_t fallw18;  ///< 0x12c [R/W]: Filter Fall Weight register (Instance 19 of 19)
  volatile uint32_t reserved0[52];
  volatile uint32_t int_en;  ///< 0x200 [R/W]: Interrupt Enable register
  volatile uint32_t int_st;  ///< 0x204 [R]: Interrupt Pending register
  volatile uint32_t int_cl;  ///< 0x208 [W]: Interrupt Clear register
  volatile uint32_t int_fo;  ///< 0x20c [W]: Interrupt Force register
  volatile uint32_t sensor_out;  ///< 0x210 [R]: Sensor Output register
} SENSOR_REGS_REGS_t;

#endif // SENSOR_REGS
