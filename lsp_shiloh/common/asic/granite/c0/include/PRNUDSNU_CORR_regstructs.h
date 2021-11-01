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
 * \file PRNUDSNU_CORR_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PRNUDSNU_CORR_REGSTRUCTS_H_
#define _PRNUDSNU_CORR_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PRNUDSNU Correction (PRNUDSNU_CORR)
/** \brief Register Descriptions for the PRNUDSNU Correction block*/
//
//====================================================================

typedef struct PRNUDSNU_CORR_REGS_s
{
  volatile uint32_t RLSCR;  ///< 0x0 [R/W]: Left and Right Starting Gain Configuration Register
  volatile uint32_t LSVCR;  ///< 0x4 [R/W]: Left Slope Value Configuration Register
  volatile uint32_t LMPCR0;  ///< 0x8 [R/W]: Left Max Pixel Configuration Register - Color0/Even
  volatile uint32_t LMPCR1;  ///< 0xc [R/W]: Left Max Pixel Configuration Register - Color1/Odd
  volatile uint32_t LMPCR2;  ///< 0x10 [R/W]: Left Max Pixel Configuration Register - Color2
  volatile uint32_t RSVCR;  ///< 0x14 [R/W]: Right Slope Value Configuration Register
  volatile uint32_t RSPCR0;  ///< 0x18 [R/W]: Right Start pixel Configuration Register - Color0/even
  volatile uint32_t RSPCR1;  ///< 0x1c [R/W]: Right Start pixel Configuration Register - Color1/odd
  volatile uint32_t RSPCR2;  ///< 0x20 [R/W]: Right Start pixel Configuration Register - Color2
  volatile uint32_t PDCR1;  ///< 0x24 [R/W]: PRNU/DSNU Configuration Register1
  volatile uint32_t reserved0[3];
  volatile uint32_t PDCR2;  ///< 0x34 [R/W]: PRNU/DSNU Configuration Register2
  volatile uint32_t PONCR;  ///< 0x38 [R/W]: Percentage of Nominal Configuration Register
  volatile uint32_t QUAD;  ///< 0x3c [R/W]: Quadratic Configuration
  volatile uint32_t DOFF;  ///< 0x40 [R/W]: DSNU Offset Register
  volatile uint32_t DMULT;  ///< 0x44 [R/W]: DSNU Mult/Scale Register
  volatile uint32_t POFF;  ///< 0x48 [R/W]: PSNU Offset Register
  volatile uint32_t PMULT;  ///< 0x4c [R/W]: PRNU Mult/Scale Register
} PRNUDSNU_CORR_REGS_t;

#endif // PRNUDSNU_CORR
