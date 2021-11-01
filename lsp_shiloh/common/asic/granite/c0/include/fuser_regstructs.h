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
 * \file fuser_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _FUSER_REGSTRUCTS_H_
#define _FUSER_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: FUSER block registers (fuser)
/** \brief The following registers are used to configure the
    FUSER block. Note that synchronous mode implies that a
    zeroCrossDetect signal is being used (either rising, falling, or both).
    Asynchronous mode implies that no zeroCrossDetect is available.*/
//
//====================================================================

typedef struct FUSER_REGS_s
{
  volatile uint32_t FEPR;  ///< 0x0 [R/W]: Fuser Enable and Polarity Register
  volatile uint32_t FCR0;  ///< 0x4 [R/W]: Fuser Configuration Register (Instance 1 of 3)
  volatile uint32_t FCR1;  ///< 0x8 [R/W]: Fuser Configuration Register (Instance 2 of 3)
  volatile uint32_t FCR2;  ///< 0xc [R/W]: Fuser Configuration Register (Instance 3 of 3)
  volatile uint32_t FPECR0;  ///< 0x10 [R/W]: Fuser PWM Mode Even Cycle Configuration Register (Instance 1 of 3)
  volatile uint32_t FPECR1;  ///< 0x14 [R/W]: Fuser PWM Mode Even Cycle Configuration Register (Instance 2 of 3)
  volatile uint32_t FPECR2;  ///< 0x18 [R/W]: Fuser PWM Mode Even Cycle Configuration Register (Instance 3 of 3)
  volatile uint32_t FPOCR0;  ///< 0x1c [R/W]: Fuser PWM Mode Odd Cycle Configuration Register (Instance 1 of 3)
  volatile uint32_t FPOCR1;  ///< 0x20 [R/W]: Fuser PWM Mode Odd Cycle Configuration Register (Instance 2 of 3)
  volatile uint32_t FPOCR2;  ///< 0x24 [R/W]: Fuser PWM Mode Odd Cycle Configuration Register (Instance 3 of 3)
  volatile uint32_t FDCR0;  ///< 0x28 [R/W]: Fuser DSM Mode Configuration Register (Instance 1 of 3)
  volatile uint32_t FDCR1;  ///< 0x2c [R/W]: Fuser DSM Mode Configuration Register (Instance 2 of 3)
  volatile uint32_t FDCR2;  ///< 0x30 [R/W]: Fuser DSM Mode Configuration Register (Instance 3 of 3)
  volatile uint32_t FPACR0;  ///< 0x34 [R/W]: Fuser Phase Angle Configuration Register (Instance 1 of 3)
  volatile uint32_t FPACR1;  ///< 0x38 [R/W]: Fuser Phase Angle Configuration Register (Instance 2 of 3)
  volatile uint32_t FPACR2;  ///< 0x3c [R/W]: Fuser Phase Angle Configuration Register (Instance 3 of 3)
  volatile uint32_t FSR;  ///< 0x40 [R]: Fuser Status Register
  volatile uint32_t FIER;  ///< 0x44 [R/W]: Fuser Interrupt Enable Register
  volatile uint32_t FIPR;  ///< 0x48 [R]: Fuser Interrupt Pending Register
  volatile uint32_t FIAR;  ///< 0x4c [W]: Fuser Interrupt Acknowledge Register
  volatile uint32_t FIFR;  ///< 0x50 [W]: Force Interrupt Force Register
  volatile uint32_t ZCFR;  ///< 0x54 [R/W]: Zerocrossing Detect Filter register
  volatile uint32_t ZCLR;  ///< 0x58 [R/W]: Zerocrossing Detect Lockout register
  volatile uint32_t TOCFG;  ///< 0x5c [R/W]: Timeout Configuration Register
} FUSER_REGS_t;

#endif // FUSER
