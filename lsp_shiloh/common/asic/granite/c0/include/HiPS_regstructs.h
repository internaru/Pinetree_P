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
 * \file HiPS_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HIPS_REGSTRUCTS_H_
#define _HIPS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: HiPS (HiPS)
/** \brief Top-level register file for HiPS block*/
//
//====================================================================

typedef struct HIPS_REGS_s
{
  volatile uint32_t Chan_Enable;  ///< 0x0 [R/W]: HiPS Channel Enable
  volatile uint32_t FWBIST_Control;  ///< 0x4 [R/W]: HiPS FWBIST Control
  volatile uint32_t FWBIST_Enable;  ///< 0x8 [R/W]: HiPS FWBIST Enable
  volatile uint32_t FWBIST_Error;  ///< 0xc [R]: HiPS FWBIST Error
  volatile uint32_t FWBIST_Done;  ///< 0x10 [R]: HiPS FWBIST Done
  volatile uint32_t FWBIST_PassnFail;  ///< 0x14 [R]: HiPS FWBIST PassnFail
  volatile uint32_t reserved0[2];
  volatile uint32_t FWBIST_Diagnostic_Table0;  ///< 0x20 [R]: HiPS FWBIST Diagnostic Values Table (Instance 1 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table1;  ///< 0x24 [R]: HiPS FWBIST Diagnostic Values Table (Instance 2 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table2;  ///< 0x28 [R]: HiPS FWBIST Diagnostic Values Table (Instance 3 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table3;  ///< 0x2c [R]: HiPS FWBIST Diagnostic Values Table (Instance 4 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table4;  ///< 0x30 [R]: HiPS FWBIST Diagnostic Values Table (Instance 5 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table5;  ///< 0x34 [R]: HiPS FWBIST Diagnostic Values Table (Instance 6 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table6;  ///< 0x38 [R]: HiPS FWBIST Diagnostic Values Table (Instance 7 of 8)
  volatile uint32_t FWBIST_Diagnostic_Table7;  ///< 0x3c [R]: HiPS FWBIST Diagnostic Values Table (Instance 8 of 8)
} HIPS_REGS_t;

#endif // HIPS
