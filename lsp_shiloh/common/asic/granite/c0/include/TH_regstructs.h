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
 * \file TH_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TH_REGSTRUCTS_H_
#define _TH_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Thresholding Registers and SRAMs (TH)
/** \brief Register and SRAM descriptions for the Thresholding block*/
//
//====================================================================

//====================================================================
//
//Register File: CORE (CORE)
/** \brief Register descriptions for the Thresholding block*/
//
//====================================================================

typedef struct CORE_REGS_s
{
  volatile uint32_t TCR;  ///< 0x0 [R/W]: TH Configuration Register
  volatile uint32_t TMCR0;  ///< 0x4 [R/W]: TH Micro Cell Register (Instance 1 of 2)
  volatile uint32_t TMCR1;  ///< 0x8 [R/W]: TH Micro Cell Register (Instance 2 of 2)
  volatile uint32_t TCSR0;  ///< 0xc [R/W]: TH Cell Start Register (Instance 1 of 2)
  volatile uint32_t TCSR1;  ///< 0x10 [R/W]: TH Cell Start Register (Instance 2 of 2)
  volatile uint32_t TSSR0;  ///< 0x14 [R/W]: TH Stochastic Size Register (Instance 1 of 2)
  volatile uint32_t TSSR1;  ///< 0x18 [R/W]: TH Stochastic Size Register (Instance 2 of 2)
  volatile uint32_t TTR0;  ///< 0x1c [R/W]: TH Thresholding Register (Instance 1 of 2)
  volatile uint32_t TTR1;  ///< 0x20 [R/W]: TH Thresholding Register (Instance 2 of 2)
  volatile uint32_t TTER;  ///< 0x24 [R/W]: TH Thresholding Edge Register
  volatile uint32_t TPGR0;  ///< 0x28 [R/W]: TH PosGen Save/Restore Register (Instance 1 of 2)
  volatile uint32_t TPGR1;  ///< 0x2c [R/W]: TH PosGen Save/Restore Register (Instance 2 of 2)
  volatile uint32_t TCGR0;  ///< 0x30 [R/W]: TH CellGen Save/Restore Register (Instance 1 of 2)
  volatile uint32_t TCGR1;  ///< 0x34 [R/W]: TH CellGen Save/Restore Register (Instance 2 of 2)
  volatile uint32_t TALR;  ///< 0x38 [R/W]: TH AltLut Save/Restore Register
  volatile uint32_t TSRR;  ///< 0x3c [R/W]: TH Save/Restore Register
} CORE_REGS_t;

//====================================================================
//
//Register File: SRAM (SRAM)
/** \brief SRAM descriptions for the Thresholding block*/
//
//====================================================================

typedef struct SRAM_REGS_s
{
  volatile uint32_t reserved0[1024];
  volatile uint32_t ILUT[512];  ///< 0x1000 [R/W]: InLUT SRAM
  volatile uint32_t CELL0[256];  ///< 0x1800 [R/W]: uCell SRAM
  volatile uint32_t reserved1[256];
  volatile uint32_t CELL1[256];  ///< 0x2000 [R/W]: uCell SRAM
  volatile uint32_t reserved2[256];
  volatile uint32_t ALUT[256];  ///< 0x2800 [R/W]: AltLUT SRAM
  volatile uint32_t reserved3[1280];
  volatile uint32_t STOC[4096];  ///< 0x4000 [R/W]: Stochastic SRAM
} SRAM_REGS_t;

#endif // TH
