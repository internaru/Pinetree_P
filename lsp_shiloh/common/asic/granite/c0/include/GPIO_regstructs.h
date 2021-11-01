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
 * \file GPIO_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _GPIO_REGSTRUCTS_H_
#define _GPIO_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: General Purpose I/O (GPIO)
/** \brief The GPIO block registers provide the means to configure the characteristics of each GPIO pin,
    as well as the means for firmware to control/observe these pins.*/
//
//====================================================================

typedef struct GPIO_REGS_s
{
  volatile uint32_t InData;  ///< 0x0 [R]: Input Data Register
  volatile uint32_t OutData;  ///< 0x4 [R/W]: Output Data Register
  volatile uint32_t OE;  ///< 0x8 [R/W]: GPIO Output Enable Register
  volatile uint32_t RawData;  ///< 0xc [R]: GPIO Raw DataIn Register
  volatile uint32_t IntEn;  ///< 0x10 [R/W]: GPIO Interrupt Enable Register
  volatile uint32_t IntAck;  ///< 0x14 [W]: GPIO Interrupt Acknowledge Register
  volatile uint32_t IntStat;  ///< 0x18 [R]: GPIO Interrupt Status Register
  volatile uint32_t IntTest;  ///< 0x1c [W]: GPIO Interrupt Test Register
  volatile uint32_t SemDataSet;  ///< 0x20 [W]: GPIO Semaphore Data Set Register
  volatile uint32_t SemDataClear;  ///< 0x24 [W]: GPIO Semaphore Data Clear Register
  volatile uint32_t SemOESet;  ///< 0x28 [W]: GPIO Semaphore Output Enable Set Register
  volatile uint32_t SemOEClear;  ///< 0x2c [W]: GPIO Semaphore Output Enable Clear Register
  volatile uint32_t SemIESet;  ///< 0x30 [W]: GPIO Semaphore Interrupt Enable Set Register
  volatile uint32_t SemIEClear;  ///< 0x34 [W]: GPIO Semaphore Interrupt Enable Clear Register
  volatile uint32_t reserved0[18];
  volatile uint32_t PinCfg0;  ///< 0x80 [R/W]: GPIO Pin Configuration Register (Instance 1 of 32)
  volatile uint32_t PinCfg1;  ///< 0x84 [R/W]: GPIO Pin Configuration Register (Instance 2 of 32)
  volatile uint32_t PinCfg2;  ///< 0x88 [R/W]: GPIO Pin Configuration Register (Instance 3 of 32)
  volatile uint32_t PinCfg3;  ///< 0x8c [R/W]: GPIO Pin Configuration Register (Instance 4 of 32)
  volatile uint32_t PinCfg4;  ///< 0x90 [R/W]: GPIO Pin Configuration Register (Instance 5 of 32)
  volatile uint32_t PinCfg5;  ///< 0x94 [R/W]: GPIO Pin Configuration Register (Instance 6 of 32)
  volatile uint32_t PinCfg6;  ///< 0x98 [R/W]: GPIO Pin Configuration Register (Instance 7 of 32)
  volatile uint32_t PinCfg7;  ///< 0x9c [R/W]: GPIO Pin Configuration Register (Instance 8 of 32)
  volatile uint32_t PinCfg8;  ///< 0xa0 [R/W]: GPIO Pin Configuration Register (Instance 9 of 32)
  volatile uint32_t PinCfg9;  ///< 0xa4 [R/W]: GPIO Pin Configuration Register (Instance 10 of 32)
  volatile uint32_t PinCfg10;  ///< 0xa8 [R/W]: GPIO Pin Configuration Register (Instance 11 of 32)
  volatile uint32_t PinCfg11;  ///< 0xac [R/W]: GPIO Pin Configuration Register (Instance 12 of 32)
  volatile uint32_t PinCfg12;  ///< 0xb0 [R/W]: GPIO Pin Configuration Register (Instance 13 of 32)
  volatile uint32_t PinCfg13;  ///< 0xb4 [R/W]: GPIO Pin Configuration Register (Instance 14 of 32)
  volatile uint32_t PinCfg14;  ///< 0xb8 [R/W]: GPIO Pin Configuration Register (Instance 15 of 32)
  volatile uint32_t PinCfg15;  ///< 0xbc [R/W]: GPIO Pin Configuration Register (Instance 16 of 32)
  volatile uint32_t PinCfg16;  ///< 0xc0 [R/W]: GPIO Pin Configuration Register (Instance 17 of 32)
  volatile uint32_t PinCfg17;  ///< 0xc4 [R/W]: GPIO Pin Configuration Register (Instance 18 of 32)
  volatile uint32_t PinCfg18;  ///< 0xc8 [R/W]: GPIO Pin Configuration Register (Instance 19 of 32)
  volatile uint32_t PinCfg19;  ///< 0xcc [R/W]: GPIO Pin Configuration Register (Instance 20 of 32)
  volatile uint32_t PinCfg20;  ///< 0xd0 [R/W]: GPIO Pin Configuration Register (Instance 21 of 32)
  volatile uint32_t PinCfg21;  ///< 0xd4 [R/W]: GPIO Pin Configuration Register (Instance 22 of 32)
  volatile uint32_t PinCfg22;  ///< 0xd8 [R/W]: GPIO Pin Configuration Register (Instance 23 of 32)
  volatile uint32_t PinCfg23;  ///< 0xdc [R/W]: GPIO Pin Configuration Register (Instance 24 of 32)
  volatile uint32_t PinCfg24;  ///< 0xe0 [R/W]: GPIO Pin Configuration Register (Instance 25 of 32)
  volatile uint32_t PinCfg25;  ///< 0xe4 [R/W]: GPIO Pin Configuration Register (Instance 26 of 32)
  volatile uint32_t PinCfg26;  ///< 0xe8 [R/W]: GPIO Pin Configuration Register (Instance 27 of 32)
  volatile uint32_t PinCfg27;  ///< 0xec [R/W]: GPIO Pin Configuration Register (Instance 28 of 32)
  volatile uint32_t PinCfg28;  ///< 0xf0 [R/W]: GPIO Pin Configuration Register (Instance 29 of 32)
  volatile uint32_t PinCfg29;  ///< 0xf4 [R/W]: GPIO Pin Configuration Register (Instance 30 of 32)
  volatile uint32_t PinCfg30;  ///< 0xf8 [R/W]: GPIO Pin Configuration Register (Instance 31 of 32)
  volatile uint32_t PinCfg31;  ///< 0xfc [R/W]: GPIO Pin Configuration Register (Instance 32 of 32)
} GPIO_REGS_t;

#endif // GPIO
