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
 * \file SIZAR_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SIZAR_REGSTRUCTS_H_
#define _SIZAR_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Sizar Calculations (SIZAR)
/** \brief Top-level register file for the Sizar block*/
//
//====================================================================

typedef struct SIZAR_REGS_s
{
  volatile uint32_t SizarCSR;  ///< 0x0 [R/W]: Status Control Register
  volatile uint32_t SizarOutSize;  ///< 0x4 [R/W]: Output Size Register
  volatile uint32_t SizarXPos;  ///< 0x8 [R/W]: X Position Register
  volatile uint32_t SizarYPos;  ///< 0xc [R/W]: Y Position Register
  volatile uint32_t SizarXStep;  ///< 0x10 [R/W]: X Step Register
  volatile uint32_t SizarYStep;  ///< 0x14 [R/W]: Y Step Register
  volatile uint32_t SizarInSize;  ///< 0x18 [R/W]: Input Size Register
  volatile uint32_t SizarBandLineNum;  ///< 0x1c [R/W]: Band and Line Number Register
  volatile uint32_t SizarCurPtr;  ///< 0x20 [R/W]: Current Pointer Register
  volatile uint32_t SizarBandPtr;  ///< 0x24 [R/W]: Band Structure Pointer Register
  volatile uint32_t SizarOutPtr;  ///< 0x28 [R/W]: Output Pointer Register
  volatile uint32_t SizarOutStep;  ///< 0x2c [R/W]: Output Step Register
  volatile uint32_t SizarValBand;  ///< 0x30 [R/W]: Valid Band Register
  volatile uint32_t reserved0;
  volatile uint32_t SizarDBG7;  ///< 0x38 [R]: Sizar Debug 7 Register
  volatile uint32_t SizarIRQ;  ///< 0x3c [R/W]: Interrupt Register
  volatile uint32_t SizarDBG1;  ///< 0x40 [R]: Sizar Debug 1 Register
  volatile uint32_t SizarDBG2;  ///< 0x44 [R]: Sizar Debug 2 Register
  volatile uint32_t SizarDBG3;  ///< 0x48 [R]: Sizar Debug 3 Register
  volatile uint32_t SizarDBG4;  ///< 0x4c [R]: Sizar Debug 4 Register
  volatile uint32_t SizarDBG5;  ///< 0x50 [R]: Sizar Debug 5 Register
  volatile uint32_t SizarDBG6;  ///< 0x54 [R/W]: Sizar Debug 6 Register
  volatile uint32_t AltXStep;  ///< 0x58 [R/W]: Alternate X Step Register
  volatile uint32_t AltYStep;  ///< 0x5c [R/W]: Alternate Y Step Register
  volatile uint32_t MapCtrl;  ///< 0x60 [R/W]: Map Control Register
  volatile uint32_t MapBaseAddr;  ///< 0x64 [R/W]: Map Base Address Register
  volatile uint32_t MapLineSize;  ///< 0x68 [R/W]: Map Line Size Register
  volatile uint32_t reserved1[4];
  volatile uint32_t ReverseOfst;  ///< 0x7c [R/W]: Reverse Offset Register
} SIZAR_REGS_t;

#endif // SIZAR
