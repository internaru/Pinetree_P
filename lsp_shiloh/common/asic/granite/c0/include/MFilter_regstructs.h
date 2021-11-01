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
 * \file MFilter_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _MFILTER_REGSTRUCTS_H_
#define _MFILTER_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: MFilter (MFilter)
/** \brief Register Descriptions for the PIE MFilter block*/
//
//====================================================================

//====================================================================
//
//Register File: MFilter POGO registers (POGO_Aligner)
/** \brief Register Descriptions for the PIE POGO block*/
//
//====================================================================

typedef struct POGO_ALIGNER_REGS_s
{
  volatile uint32_t PACR;  ///< 0x0 [R/W]: Pogo Aligner Configuration Register
  volatile uint32_t PASTR;  ///< 0x4 [R]: Pogo Aligner Status Register
} POGO_ALIGNER_REGS_t;

//====================================================================
//
//Register File: MFilter Sculptor registers (Sculptor)
/** \brief Register Descriptions for the PIE Sculptor block*/
//
//====================================================================

typedef struct SCULPTOR_REGS_s
{
  volatile uint32_t AILR[64];  ///< 0x0 [R/W]: Activity Index LUT
  volatile uint32_t SCR1;  ///< 0x100 [R/W]: Sculptor Configuration 1 Register
  volatile uint32_t SCR2;  ///< 0x104 [R/W]: Sculptor Configuration 2 Register
  volatile uint32_t TEL1R;  ///< 0x108 [R/W]: Text Enhance Limit 1 Register
  volatile uint32_t TEL2R;  ///< 0x10c [R/W]: Text Enhance Limit 2 Register
  volatile uint32_t TEL3R;  ///< 0x110 [R/W]: Text Enhance Limit 3 Register
  volatile uint32_t EC;  ///< 0x114 [R/W]: Edge Count
  volatile uint32_t ECI;  ///< 0x118 [R/W]: Edge Count Indexes
  volatile uint32_t YVC00_01;  ///< 0x11c [R/W]: Y Variable Coefficient00_01
  volatile uint32_t YVC02_03;  ///< 0x120 [R/W]: Y Variable Coefficient02_03
  volatile uint32_t YVC04_10;  ///< 0x124 [R/W]: Y Variable Coefficient04_10
  volatile uint32_t YVC11_12;  ///< 0x128 [R/W]: Y Variable Coefficient11_12
  volatile uint32_t YVC13_14;  ///< 0x12c [R/W]: Y Variable Coefficient13_14
  volatile uint32_t YVC20_21;  ///< 0x130 [R/W]: Y Variable Coefficient20_21
  volatile uint32_t YVC22_23;  ///< 0x134 [R/W]: Y Variable Coefficient22_23
  volatile uint32_t YVC24;  ///< 0x138 [R/W]: Y Variable Coefficient24 and Shift
  volatile uint32_t SF;  ///< 0x13c [R/W]: Shift Factor
  volatile uint32_t CVC00_01;  ///< 0x140 [R/W]: Chroma Variable Coefficient00_01
  volatile uint32_t CVC02_03;  ///< 0x144 [R/W]: Chroma Variable Coefficient02_03
  volatile uint32_t CVC04_10;  ///< 0x148 [R/W]: Chroma Variable Coefficient04_10
  volatile uint32_t CVC11_12;  ///< 0x14c [R/W]: Chroma Variable Coefficient11_12
  volatile uint32_t CVC13_14;  ///< 0x150 [R/W]: Chroma Variable Coefficient13_14
  volatile uint32_t CVC20_21;  ///< 0x154 [R/W]: Chroma Variable Coefficient20_21
  volatile uint32_t CVC22_23;  ///< 0x158 [R/W]: Chroma Variable Coefficient22_23
  volatile uint32_t CVC24;  ///< 0x15c [R/W]: Chroma Variable Coefficient24 and Shift
} SCULPTOR_REGS_t;

#endif // MFILTER
