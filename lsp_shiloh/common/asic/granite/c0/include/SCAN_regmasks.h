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
 * \file SCAN_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SCAN_REGMASKS_H_
#define _SCAN_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SCAN (SCAN)
/** \brief Top-level register file for SCAN superblock*/
//
//====================================================================

//====================================================================
//Register: Scan Config1 (SCFG1)
/** \brief Register used to program Clock and Signal Enables, Polarity.*/
//====================================================================

#define  SCAN_SCFG1_P1CLKEN_MASK 0x80000000
#define  SCAN_SCFG1_P1CLKEN_SHIFT 31
#define  SCAN_SCFG1_P1CLKEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_P1CLKEN_MASK) >> SCAN_SCFG1_P1CLKEN_SHIFT)
#define  SCAN_SCFG1_P1CLKEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_P1CLKEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_P1CLKEN_SHIFT))

#define  SCAN_SCFG1_P2CLKEN_MASK 0x40000000
#define  SCAN_SCFG1_P2CLKEN_SHIFT 30
#define  SCAN_SCFG1_P2CLKEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_P2CLKEN_MASK) >> SCAN_SCFG1_P2CLKEN_SHIFT)
#define  SCAN_SCFG1_P2CLKEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_P2CLKEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_P2CLKEN_SHIFT))

#define  SCAN_SCFG1_RSCLKEN_MASK 0x20000000
#define  SCAN_SCFG1_RSCLKEN_SHIFT 29
#define  SCAN_SCFG1_RSCLKEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_RSCLKEN_MASK) >> SCAN_SCFG1_RSCLKEN_SHIFT)
#define  SCAN_SCFG1_RSCLKEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_RSCLKEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_RSCLKEN_SHIFT))

#define  SCAN_SCFG1_CPCLKEN_MASK 0x10000000
#define  SCAN_SCFG1_CPCLKEN_SHIFT 28
#define  SCAN_SCFG1_CPCLKEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_CPCLKEN_MASK) >> SCAN_SCFG1_CPCLKEN_SHIFT)
#define  SCAN_SCFG1_CPCLKEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_CPCLKEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_CPCLKEN_SHIFT))

#define  SCAN_SCFG1_MCLKEN_MASK 0x8000000
#define  SCAN_SCFG1_MCLKEN_SHIFT 27
#define  SCAN_SCFG1_MCLKEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_MCLKEN_MASK) >> SCAN_SCFG1_MCLKEN_SHIFT)
#define  SCAN_SCFG1_MCLKEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_MCLKEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_MCLKEN_SHIFT))

#define  SCAN_SCFG1_VSCLKEN_MASK 0x4000000
#define  SCAN_SCFG1_VSCLKEN_SHIFT 26
#define  SCAN_SCFG1_VSCLKEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_VSCLKEN_MASK) >> SCAN_SCFG1_VSCLKEN_SHIFT)
#define  SCAN_SCFG1_VSCLKEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_VSCLKEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_VSCLKEN_SHIFT))

#define  SCAN_SCFG1_CLCLKEN_MASK 0x2000000
#define  SCAN_SCFG1_CLCLKEN_SHIFT 25
#define  SCAN_SCFG1_CLCLKEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_CLCLKEN_MASK) >> SCAN_SCFG1_CLCLKEN_SHIFT)
#define  SCAN_SCFG1_CLCLKEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_CLCLKEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_CLCLKEN_SHIFT))

#define  SCAN_SCFG1_LCLMPEN_MASK 0x1000000
#define  SCAN_SCFG1_LCLMPEN_SHIFT 24
#define  SCAN_SCFG1_LCLMPEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_LCLMPEN_MASK) >> SCAN_SCFG1_LCLMPEN_SHIFT)
#define  SCAN_SCFG1_LCLMPEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_LCLMPEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_LCLMPEN_SHIFT))

#define  SCAN_SCFG1_RESERVED1_MASK 0xe00000
#define  SCAN_SCFG1_RESERVED1_SHIFT 21
#define  SCAN_SCFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_RESERVED1_MASK) >> SCAN_SCFG1_RESERVED1_SHIFT)
#define  SCAN_SCFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_RESERVED1_MASK) | (((uint32_t)val) << SCAN_SCFG1_RESERVED1_SHIFT))

#define  SCAN_SCFG1_BULBEN_MASK 0x100000
#define  SCAN_SCFG1_BULBEN_SHIFT 20
#define  SCAN_SCFG1_BULBEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_BULBEN_MASK) >> SCAN_SCFG1_BULBEN_SHIFT)
#define  SCAN_SCFG1_BULBEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_BULBEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_BULBEN_SHIFT))

#define  SCAN_SCFG1_SSMEN_MASK 0xf0000
#define  SCAN_SCFG1_SSMEN_SHIFT 16
#define  SCAN_SCFG1_SSMEN_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_SSMEN_MASK) >> SCAN_SCFG1_SSMEN_SHIFT)
#define  SCAN_SCFG1_SSMEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_SSMEN_MASK) | (((uint32_t)val) << SCAN_SCFG1_SSMEN_SHIFT))

#define  SCAN_SCFG1_SMPOL_MASK 0xf000
#define  SCAN_SCFG1_SMPOL_SHIFT 12
#define  SCAN_SCFG1_SMPOL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_SMPOL_MASK) >> SCAN_SCFG1_SMPOL_SHIFT)
#define  SCAN_SCFG1_SMPOL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_SMPOL_MASK) | (((uint32_t)val) << SCAN_SCFG1_SMPOL_SHIFT))

#define  SCAN_SCFG1_P1APOL_MASK 0x800
#define  SCAN_SCFG1_P1APOL_SHIFT 11
#define  SCAN_SCFG1_P1APOL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_P1APOL_MASK) >> SCAN_SCFG1_P1APOL_SHIFT)
#define  SCAN_SCFG1_P1APOL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_P1APOL_MASK) | (((uint32_t)val) << SCAN_SCFG1_P1APOL_SHIFT))

#define  SCAN_SCFG1_RESERVED2_MASK 0x400
#define  SCAN_SCFG1_RESERVED2_SHIFT 10
#define  SCAN_SCFG1_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_RESERVED2_MASK) >> SCAN_SCFG1_RESERVED2_SHIFT)
#define  SCAN_SCFG1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_RESERVED2_MASK) | (((uint32_t)val) << SCAN_SCFG1_RESERVED2_SHIFT))

#define  SCAN_SCFG1_SPTOG_MASK 0x200
#define  SCAN_SCFG1_SPTOG_SHIFT 9
#define  SCAN_SCFG1_SPTOG_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_SPTOG_MASK) >> SCAN_SCFG1_SPTOG_SHIFT)
#define  SCAN_SCFG1_SPTOG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_SPTOG_MASK) | (((uint32_t)val) << SCAN_SCFG1_SPTOG_SHIFT))

#define  SCAN_SCFG1_SPPOL_MASK 0x100
#define  SCAN_SCFG1_SPPOL_SHIFT 8
#define  SCAN_SCFG1_SPPOL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_SPPOL_MASK) >> SCAN_SCFG1_SPPOL_SHIFT)
#define  SCAN_SCFG1_SPPOL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_SPPOL_MASK) | (((uint32_t)val) << SCAN_SCFG1_SPPOL_SHIFT))

#define  SCAN_SCFG1_SCPOL_MASK 0xf0
#define  SCAN_SCFG1_SCPOL_SHIFT 4
#define  SCAN_SCFG1_SCPOL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_SCPOL_MASK) >> SCAN_SCFG1_SCPOL_SHIFT)
#define  SCAN_SCFG1_SCPOL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_SCPOL_MASK) | (((uint32_t)val) << SCAN_SCFG1_SCPOL_SHIFT))

#define  SCAN_SCFG1_ACPOL_MASK 0xf
#define  SCAN_SCFG1_ACPOL_SHIFT 0
#define  SCAN_SCFG1_ACPOL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG1_ACPOL_MASK) >> SCAN_SCFG1_ACPOL_SHIFT)
#define  SCAN_SCFG1_ACPOL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG1_ACPOL_MASK) | (((uint32_t)val) << SCAN_SCFG1_ACPOL_SHIFT))

//====================================================================
//Register: Scan Config2 (SCFG2)
/** \brief This register sets the Color Mode, Sensor Type and Selects, AFE Line-by-line Mode, and the Pixel Period.<br/>
<br/>
Programming EXPDIV should allow for the maximum value for the Exposure count for a given scan line period. The Exposure is measured using a 16-bit counter clocked by ExpClk/(EXPDIV+1). The formula for the max. available Exposure Count is <br/>
<br/>
Exposure Count (max.) = 2^16 * (EXPDIV+1)/ExpClk <br/>
<br/>
Example: ExpClk = 240MHz. Using a CCD sensor with 5500 elements scanning each pixel at a 10MHz CLK, a scan line time is 550us. If EXPDIV=0 the maximum Exposure count is 2^16 * 1/ExpClk = ~273us. To cover the range of 550us, set EXPDIV=2 to get a maximum range of = 2^16 * (EXPDIV+1)/ExpClk = ~819us. <br/>
<br/>*/
//====================================================================

#define  SCAN_SCFG2_CMODE_MASK 0x80000000
#define  SCAN_SCFG2_CMODE_SHIFT 31
#define  SCAN_SCFG2_CMODE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_CMODE_MASK) >> SCAN_SCFG2_CMODE_SHIFT)
#define  SCAN_SCFG2_CMODE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_CMODE_MASK) | (((uint32_t)val) << SCAN_SCFG2_CMODE_SHIFT))

#define  SCAN_SCFG2_STYPE_MASK 0x40000000
#define  SCAN_SCFG2_STYPE_SHIFT 30
#define  SCAN_SCFG2_STYPE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_STYPE_MASK) >> SCAN_SCFG2_STYPE_SHIFT)
#define  SCAN_SCFG2_STYPE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_STYPE_MASK) | (((uint32_t)val) << SCAN_SCFG2_STYPE_SHIFT))

#define  SCAN_SCFG2_SSEL_MASK 0x30000000
#define  SCAN_SCFG2_SSEL_SHIFT 28
#define  SCAN_SCFG2_SSEL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_SSEL_MASK) >> SCAN_SCFG2_SSEL_SHIFT)
#define  SCAN_SCFG2_SSEL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_SSEL_MASK) | (((uint32_t)val) << SCAN_SCFG2_SSEL_SHIFT))

#define  SCAN_SCFG2_AFEWIDTH_MASK 0xc000000
#define  SCAN_SCFG2_AFEWIDTH_SHIFT 26
#define  SCAN_SCFG2_AFEWIDTH_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_AFEWIDTH_MASK) >> SCAN_SCFG2_AFEWIDTH_SHIFT)
#define  SCAN_SCFG2_AFEWIDTH_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_AFEWIDTH_MASK) | (((uint32_t)val) << SCAN_SCFG2_AFEWIDTH_SHIFT))

#define  SCAN_SCFG2_EXPMASK_MASK 0x2000000
#define  SCAN_SCFG2_EXPMASK_SHIFT 25
#define  SCAN_SCFG2_EXPMASK_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_EXPMASK_MASK) >> SCAN_SCFG2_EXPMASK_SHIFT)
#define  SCAN_SCFG2_EXPMASK_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_EXPMASK_MASK) | (((uint32_t)val) << SCAN_SCFG2_EXPMASK_SHIFT))

#define  SCAN_SCFG2_RESERVED1_MASK 0x1c00000
#define  SCAN_SCFG2_RESERVED1_SHIFT 22
#define  SCAN_SCFG2_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_RESERVED1_MASK) >> SCAN_SCFG2_RESERVED1_SHIFT)
#define  SCAN_SCFG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_RESERVED1_MASK) | (((uint32_t)val) << SCAN_SCFG2_RESERVED1_SHIFT))

#define  SCAN_SCFG2_LEDEN1_MASK 0x380000
#define  SCAN_SCFG2_LEDEN1_SHIFT 19
#define  SCAN_SCFG2_LEDEN1_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_LEDEN1_MASK) >> SCAN_SCFG2_LEDEN1_SHIFT)
#define  SCAN_SCFG2_LEDEN1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_LEDEN1_MASK) | (((uint32_t)val) << SCAN_SCFG2_LEDEN1_SHIFT))

#define  SCAN_SCFG2_LEDEN0_MASK 0x70000
#define  SCAN_SCFG2_LEDEN0_SHIFT 16
#define  SCAN_SCFG2_LEDEN0_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_LEDEN0_MASK) >> SCAN_SCFG2_LEDEN0_SHIFT)
#define  SCAN_SCFG2_LEDEN0_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_LEDEN0_MASK) | (((uint32_t)val) << SCAN_SCFG2_LEDEN0_SHIFT))

#define  SCAN_SCFG2_EXPDIV_MASK 0xff00
#define  SCAN_SCFG2_EXPDIV_SHIFT 8
#define  SCAN_SCFG2_EXPDIV_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_EXPDIV_MASK) >> SCAN_SCFG2_EXPDIV_SHIFT)
#define  SCAN_SCFG2_EXPDIV_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_EXPDIV_MASK) | (((uint32_t)val) << SCAN_SCFG2_EXPDIV_SHIFT))

#define  SCAN_SCFG2_PIXPER_MASK 0xff
#define  SCAN_SCFG2_PIXPER_SHIFT 0
#define  SCAN_SCFG2_PIXPER_MASK_SHIFT(reg) (((reg) & SCAN_SCFG2_PIXPER_MASK) >> SCAN_SCFG2_PIXPER_SHIFT)
#define  SCAN_SCFG2_PIXPER_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG2_PIXPER_MASK) | (((uint32_t)val) << SCAN_SCFG2_PIXPER_SHIFT))

//====================================================================
//Register: Scan Config3 (SCFG3)
/** \brief Register used to program Delays for each rising/falling edge of the Sensor and AFE Clocks.<br/>
<br/>
Resolution is ExpClk cycles. The frequency of ExpClk is implementation specific.<br/>
<br/>
This register contains 2-bit fields for delaying each edge (rising/falling) of the Sensor and AFE Clocks for fine adjustment of each Clock edge. The "coarse" edges are selected within a system clock period or a pixel clock period by the Sensor and AFE Clock registers, and the Start Config Registers.<br/>
<br/>
The 2-bit Delay fields are connected to the ScanDelay block instantiated at Scan top-level.<br/>*/
//====================================================================

#define  SCAN_SCFG3_P1RISE_MASK 0xc0000000
#define  SCAN_SCFG3_P1RISE_SHIFT 30
#define  SCAN_SCFG3_P1RISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_P1RISE_MASK) >> SCAN_SCFG3_P1RISE_SHIFT)
#define  SCAN_SCFG3_P1RISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_P1RISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_P1RISE_SHIFT))

#define  SCAN_SCFG3_P1FALL_MASK 0x30000000
#define  SCAN_SCFG3_P1FALL_SHIFT 28
#define  SCAN_SCFG3_P1FALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_P1FALL_MASK) >> SCAN_SCFG3_P1FALL_SHIFT)
#define  SCAN_SCFG3_P1FALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_P1FALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_P1FALL_SHIFT))

#define  SCAN_SCFG3_P2RISE_MASK 0xc000000
#define  SCAN_SCFG3_P2RISE_SHIFT 26
#define  SCAN_SCFG3_P2RISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_P2RISE_MASK) >> SCAN_SCFG3_P2RISE_SHIFT)
#define  SCAN_SCFG3_P2RISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_P2RISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_P2RISE_SHIFT))

#define  SCAN_SCFG3_P2FALL_MASK 0x3000000
#define  SCAN_SCFG3_P2FALL_SHIFT 24
#define  SCAN_SCFG3_P2FALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_P2FALL_MASK) >> SCAN_SCFG3_P2FALL_SHIFT)
#define  SCAN_SCFG3_P2FALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_P2FALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_P2FALL_SHIFT))

#define  SCAN_SCFG3_RSRISE_MASK 0xc00000
#define  SCAN_SCFG3_RSRISE_SHIFT 22
#define  SCAN_SCFG3_RSRISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_RSRISE_MASK) >> SCAN_SCFG3_RSRISE_SHIFT)
#define  SCAN_SCFG3_RSRISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_RSRISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_RSRISE_SHIFT))

#define  SCAN_SCFG3_RSFALL_MASK 0x300000
#define  SCAN_SCFG3_RSFALL_SHIFT 20
#define  SCAN_SCFG3_RSFALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_RSFALL_MASK) >> SCAN_SCFG3_RSFALL_SHIFT)
#define  SCAN_SCFG3_RSFALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_RSFALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_RSFALL_SHIFT))

#define  SCAN_SCFG3_CPRISE_MASK 0xc0000
#define  SCAN_SCFG3_CPRISE_SHIFT 18
#define  SCAN_SCFG3_CPRISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_CPRISE_MASK) >> SCAN_SCFG3_CPRISE_SHIFT)
#define  SCAN_SCFG3_CPRISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_CPRISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_CPRISE_SHIFT))

#define  SCAN_SCFG3_CPFALL_MASK 0x30000
#define  SCAN_SCFG3_CPFALL_SHIFT 16
#define  SCAN_SCFG3_CPFALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_CPFALL_MASK) >> SCAN_SCFG3_CPFALL_SHIFT)
#define  SCAN_SCFG3_CPFALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_CPFALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_CPFALL_SHIFT))

#define  SCAN_SCFG3_MCRISE_MASK 0xc000
#define  SCAN_SCFG3_MCRISE_SHIFT 14
#define  SCAN_SCFG3_MCRISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_MCRISE_MASK) >> SCAN_SCFG3_MCRISE_SHIFT)
#define  SCAN_SCFG3_MCRISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_MCRISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_MCRISE_SHIFT))

#define  SCAN_SCFG3_MCFALL_MASK 0x3000
#define  SCAN_SCFG3_MCFALL_SHIFT 12
#define  SCAN_SCFG3_MCFALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_MCFALL_MASK) >> SCAN_SCFG3_MCFALL_SHIFT)
#define  SCAN_SCFG3_MCFALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_MCFALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_MCFALL_SHIFT))

#define  SCAN_SCFG3_VSRISE_MASK 0xc00
#define  SCAN_SCFG3_VSRISE_SHIFT 10
#define  SCAN_SCFG3_VSRISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_VSRISE_MASK) >> SCAN_SCFG3_VSRISE_SHIFT)
#define  SCAN_SCFG3_VSRISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_VSRISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_VSRISE_SHIFT))

#define  SCAN_SCFG3_VSFALL_MASK 0x300
#define  SCAN_SCFG3_VSFALL_SHIFT 8
#define  SCAN_SCFG3_VSFALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_VSFALL_MASK) >> SCAN_SCFG3_VSFALL_SHIFT)
#define  SCAN_SCFG3_VSFALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_VSFALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_VSFALL_SHIFT))

#define  SCAN_SCFG3_CD1RISE_MASK 0xc0
#define  SCAN_SCFG3_CD1RISE_SHIFT 6
#define  SCAN_SCFG3_CD1RISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_CD1RISE_MASK) >> SCAN_SCFG3_CD1RISE_SHIFT)
#define  SCAN_SCFG3_CD1RISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_CD1RISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_CD1RISE_SHIFT))

#define  SCAN_SCFG3_CD1FALL_MASK 0x30
#define  SCAN_SCFG3_CD1FALL_SHIFT 4
#define  SCAN_SCFG3_CD1FALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_CD1FALL_MASK) >> SCAN_SCFG3_CD1FALL_SHIFT)
#define  SCAN_SCFG3_CD1FALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_CD1FALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_CD1FALL_SHIFT))

#define  SCAN_SCFG3_SPRISE_MASK 0xc
#define  SCAN_SCFG3_SPRISE_SHIFT 2
#define  SCAN_SCFG3_SPRISE_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_SPRISE_MASK) >> SCAN_SCFG3_SPRISE_SHIFT)
#define  SCAN_SCFG3_SPRISE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_SPRISE_MASK) | (((uint32_t)val) << SCAN_SCFG3_SPRISE_SHIFT))

#define  SCAN_SCFG3_SPFALL_MASK 0x3
#define  SCAN_SCFG3_SPFALL_SHIFT 0
#define  SCAN_SCFG3_SPFALL_MASK_SHIFT(reg) (((reg) & SCAN_SCFG3_SPFALL_MASK) >> SCAN_SCFG3_SPFALL_SHIFT)
#define  SCAN_SCFG3_SPFALL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCFG3_SPFALL_MASK) | (((uint32_t)val) << SCAN_SCFG3_SPFALL_SHIFT))

//====================================================================
//Register: Scan Control (SCTRL)
/** \brief Register controls overall scan operation - Scan Enable, Run, Interrupt Enables, etc.<br/>
<br/>
This register controls the overall scan operation. SCANEN is an overall enable for the sensor and AFE clocks, Start pulses, and illumination signals. CMDQEN is an enable bit for queuing the SCMD register values. RUN is an overall enable for the commands in the SCMD command queue register. See section 6.2.10 for a description of the overall sequence control for the Scan block, including Scan Control, Scan Command, and Scan Cycle. <br/>
<br/>
Note: When using an external motor for Linestarts, the Motor Configuration Register, MCFG, must be written before Scan Control bits that enable the Scan.<br/>*/
//====================================================================

#define  SCAN_SCTRL_RESERVED1_MASK 0xffffff00
#define  SCAN_SCTRL_RESERVED1_SHIFT 8
#define  SCAN_SCTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_RESERVED1_MASK) >> SCAN_SCTRL_RESERVED1_SHIFT)
#define  SCAN_SCTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_RESERVED1_MASK) | (((uint32_t)val) << SCAN_SCTRL_RESERVED1_SHIFT))

#define  SCAN_SCTRL_UMSEN_MASK 0x80
#define  SCAN_SCTRL_UMSEN_SHIFT 7
#define  SCAN_SCTRL_UMSEN_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_UMSEN_MASK) >> SCAN_SCTRL_UMSEN_SHIFT)
#define  SCAN_SCTRL_UMSEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_UMSEN_MASK) | (((uint32_t)val) << SCAN_SCTRL_UMSEN_SHIFT))

#define  SCAN_SCTRL_CMDQEN_MASK 0x40
#define  SCAN_SCTRL_CMDQEN_SHIFT 6
#define  SCAN_SCTRL_CMDQEN_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_CMDQEN_MASK) >> SCAN_SCTRL_CMDQEN_SHIFT)
#define  SCAN_SCTRL_CMDQEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_CMDQEN_MASK) | (((uint32_t)val) << SCAN_SCTRL_CMDQEN_SHIFT))

#define  SCAN_SCTRL_TERMCMD_MASK 0x20
#define  SCAN_SCTRL_TERMCMD_SHIFT 5
#define  SCAN_SCTRL_TERMCMD_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_TERMCMD_MASK) >> SCAN_SCTRL_TERMCMD_SHIFT)
#define  SCAN_SCTRL_TERMCMD_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_TERMCMD_MASK) | (((uint32_t)val) << SCAN_SCTRL_TERMCMD_SHIFT))

#define  SCAN_SCTRL_SRESET_MASK 0x10
#define  SCAN_SCTRL_SRESET_SHIFT 4
#define  SCAN_SCTRL_SRESET_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_SRESET_MASK) >> SCAN_SCTRL_SRESET_SHIFT)
#define  SCAN_SCTRL_SRESET_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_SRESET_MASK) | (((uint32_t)val) << SCAN_SCTRL_SRESET_SHIFT))

#define  SCAN_SCTRL_RESERVED2_MASK 0x8
#define  SCAN_SCTRL_RESERVED2_SHIFT 3
#define  SCAN_SCTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_RESERVED2_MASK) >> SCAN_SCTRL_RESERVED2_SHIFT)
#define  SCAN_SCTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_RESERVED2_MASK) | (((uint32_t)val) << SCAN_SCTRL_RESERVED2_SHIFT))

#define  SCAN_SCTRL_SCMDIE_MASK 0x4
#define  SCAN_SCTRL_SCMDIE_SHIFT 2
#define  SCAN_SCTRL_SCMDIE_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_SCMDIE_MASK) >> SCAN_SCTRL_SCMDIE_SHIFT)
#define  SCAN_SCTRL_SCMDIE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_SCMDIE_MASK) | (((uint32_t)val) << SCAN_SCTRL_SCMDIE_SHIFT))

#define  SCAN_SCTRL_RUN_MASK 0x2
#define  SCAN_SCTRL_RUN_SHIFT 1
#define  SCAN_SCTRL_RUN_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_RUN_MASK) >> SCAN_SCTRL_RUN_SHIFT)
#define  SCAN_SCTRL_RUN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_RUN_MASK) | (((uint32_t)val) << SCAN_SCTRL_RUN_SHIFT))

#define  SCAN_SCTRL_SCANEN_MASK 0x1
#define  SCAN_SCTRL_SCANEN_SHIFT 0
#define  SCAN_SCTRL_SCANEN_MASK_SHIFT(reg) (((reg) & SCAN_SCTRL_SCANEN_MASK) >> SCAN_SCTRL_SCANEN_SHIFT)
#define  SCAN_SCTRL_SCANEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCTRL_SCANEN_MASK) | (((uint32_t)val) << SCAN_SCTRL_SCANEN_SHIFT))

//====================================================================
//Register: Scan Command (SCMD)
/** \brief This register is the tail of a 2-deep command queue when SCTRL.CMDQEN = 1. Otherwise it is a R/W register only. If read when SCTRL.CMDQEN = 1, it will return the last value written, which may be different from the command value currently executing. Two values may be queued at the beginning of a scan operation. When the SCTRL.RUN bit is set, the first command begins execution. Command status is read in the SSTAT register with the SSTAT.QEMPTY and QFULL bits. When the first command finishes execution, the next queued command starts to execute, the SSTAT.SCMDCOMP bit is set, and also SSTAT.QFULL is cleared, indicating another command may be queued. SSTAT.SCMDCOMP must be cleared before the second command finishes execution. In general, another command may be queued whenever SSTAT.QFULL is clear. Operation continues in this manner until the last command completes execution. <br/>
<br/>
Each command in the queue is associated with the corresponding SCANY value. SCANY decrements for every scan line or every 3 lines for CIS Color. The command is considered complete when the local copy of SCANY decrements to 0, which is the condition for SSTAT.SCMDCOMP = 1. The local copy of SCANY can be read in SSTAT.SCANYR. The SCTRL.TERMCMD bit clears the local SCANY copy which causes the current command to terminate at the next line boundary, without affecting the Command Queue.<br/>
<br/>
The SCTRL.RUN bit is checked at each line boundary regardless of the current line count value. If it is cleared before the command queue completes, the scan will terminate and the command queue will be cleared. Data transfer will stop. It is an overriding enable bit for the scan.<br/>
<br/>
See the Scan Control Sequences section for a more detailed description of the interaction with SCTRL.<br/>*/
//====================================================================

#define  SCAN_SCMD_EOI_MASK 0x80000000
#define  SCAN_SCMD_EOI_SHIFT 31
#define  SCAN_SCMD_EOI_MASK_SHIFT(reg) (((reg) & SCAN_SCMD_EOI_MASK) >> SCAN_SCMD_EOI_SHIFT)
#define  SCAN_SCMD_EOI_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCMD_EOI_MASK) | (((uint32_t)val) << SCAN_SCMD_EOI_SHIFT))

#define  SCAN_SCMD_SDATA_MASK 0x40000000
#define  SCAN_SCMD_SDATA_SHIFT 30
#define  SCAN_SCMD_SDATA_MASK_SHIFT(reg) (((reg) & SCAN_SCMD_SDATA_MASK) >> SCAN_SCMD_SDATA_SHIFT)
#define  SCAN_SCMD_SDATA_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCMD_SDATA_MASK) | (((uint32_t)val) << SCAN_SCMD_SDATA_SHIFT))

#define  SCAN_SCMD_SPTOGVAL_MASK 0x20000000
#define  SCAN_SCMD_SPTOGVAL_SHIFT 29
#define  SCAN_SCMD_SPTOGVAL_MASK_SHIFT(reg) (((reg) & SCAN_SCMD_SPTOGVAL_MASK) >> SCAN_SCMD_SPTOGVAL_SHIFT)
#define  SCAN_SCMD_SPTOGVAL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCMD_SPTOGVAL_MASK) | (((uint32_t)val) << SCAN_SCMD_SPTOGVAL_SHIFT))

#define  SCAN_SCMD_SCLKPER_MASK 0x1fff0000
#define  SCAN_SCMD_SCLKPER_SHIFT 16
#define  SCAN_SCMD_SCLKPER_MASK_SHIFT(reg) (((reg) & SCAN_SCMD_SCLKPER_MASK) >> SCAN_SCMD_SCLKPER_SHIFT)
#define  SCAN_SCMD_SCLKPER_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCMD_SCLKPER_MASK) | (((uint32_t)val) << SCAN_SCMD_SCLKPER_SHIFT))

#define  SCAN_SCMD_SCANY_MASK 0xffff
#define  SCAN_SCMD_SCANY_SHIFT 0
#define  SCAN_SCMD_SCANY_MASK_SHIFT(reg) (((reg) & SCAN_SCMD_SCANY_MASK) >> SCAN_SCMD_SCANY_SHIFT)
#define  SCAN_SCMD_SCANY_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCMD_SCANY_MASK) | (((uint32_t)val) << SCAN_SCMD_SCANY_SHIFT))

//====================================================================
//Register: Scan Status (SSTAT)
/** \brief This register provides information about the command queue and the currently executing command. Note that if SCTRL.SRESET is set, the CMDSTAT, QEMPTY, QFULL, QOVR, IOVR, and SCMDCOMP bits are set to their default values. SCANYR is set to the current value of SCMD.SCANY. AFEPCOMP, if set, is not cleared by SCTRL.SRESET, but can be cleared by setting SIACK.AFEPACK = 1.<br/>*/
//====================================================================

#define  SCAN_SSTAT_SCANYR_MASK 0xffff0000
#define  SCAN_SSTAT_SCANYR_SHIFT 16
#define  SCAN_SSTAT_SCANYR_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_SCANYR_MASK) >> SCAN_SSTAT_SCANYR_SHIFT)
#define  SCAN_SSTAT_SCANYR_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_SCANYR_MASK) | (((uint32_t)val) << SCAN_SSTAT_SCANYR_SHIFT))

#define  SCAN_SSTAT_RESERVED1_MASK 0xf000
#define  SCAN_SSTAT_RESERVED1_SHIFT 12
#define  SCAN_SSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_RESERVED1_MASK) >> SCAN_SSTAT_RESERVED1_SHIFT)
#define  SCAN_SSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_RESERVED1_MASK) | (((uint32_t)val) << SCAN_SSTAT_RESERVED1_SHIFT))

#define  SCAN_SSTAT_EXTINTSTAT_MASK 0xf00
#define  SCAN_SSTAT_EXTINTSTAT_SHIFT 8
#define  SCAN_SSTAT_EXTINTSTAT_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_EXTINTSTAT_MASK) >> SCAN_SSTAT_EXTINTSTAT_SHIFT)
#define  SCAN_SSTAT_EXTINTSTAT_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_EXTINTSTAT_MASK) | (((uint32_t)val) << SCAN_SSTAT_EXTINTSTAT_SHIFT))

#define  SCAN_SSTAT_UNEXPSYNC_MASK 0x80
#define  SCAN_SSTAT_UNEXPSYNC_SHIFT 7
#define  SCAN_SSTAT_UNEXPSYNC_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_UNEXPSYNC_MASK) >> SCAN_SSTAT_UNEXPSYNC_SHIFT)
#define  SCAN_SSTAT_UNEXPSYNC_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_UNEXPSYNC_MASK) | (((uint32_t)val) << SCAN_SSTAT_UNEXPSYNC_SHIFT))

#define  SCAN_SSTAT_CMDSTAT_MASK 0x40
#define  SCAN_SSTAT_CMDSTAT_SHIFT 6
#define  SCAN_SSTAT_CMDSTAT_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_CMDSTAT_MASK) >> SCAN_SSTAT_CMDSTAT_SHIFT)
#define  SCAN_SSTAT_CMDSTAT_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_CMDSTAT_MASK) | (((uint32_t)val) << SCAN_SSTAT_CMDSTAT_SHIFT))

#define  SCAN_SSTAT_QEMPTY_MASK 0x20
#define  SCAN_SSTAT_QEMPTY_SHIFT 5
#define  SCAN_SSTAT_QEMPTY_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_QEMPTY_MASK) >> SCAN_SSTAT_QEMPTY_SHIFT)
#define  SCAN_SSTAT_QEMPTY_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_QEMPTY_MASK) | (((uint32_t)val) << SCAN_SSTAT_QEMPTY_SHIFT))

#define  SCAN_SSTAT_QFULL_MASK 0x10
#define  SCAN_SSTAT_QFULL_SHIFT 4
#define  SCAN_SSTAT_QFULL_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_QFULL_MASK) >> SCAN_SSTAT_QFULL_SHIFT)
#define  SCAN_SSTAT_QFULL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_QFULL_MASK) | (((uint32_t)val) << SCAN_SSTAT_QFULL_SHIFT))

#define  SCAN_SSTAT_QOVR_MASK 0x8
#define  SCAN_SSTAT_QOVR_SHIFT 3
#define  SCAN_SSTAT_QOVR_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_QOVR_MASK) >> SCAN_SSTAT_QOVR_SHIFT)
#define  SCAN_SSTAT_QOVR_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_QOVR_MASK) | (((uint32_t)val) << SCAN_SSTAT_QOVR_SHIFT))

#define  SCAN_SSTAT_IOVR_MASK 0x4
#define  SCAN_SSTAT_IOVR_SHIFT 2
#define  SCAN_SSTAT_IOVR_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_IOVR_MASK) >> SCAN_SSTAT_IOVR_SHIFT)
#define  SCAN_SSTAT_IOVR_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_IOVR_MASK) | (((uint32_t)val) << SCAN_SSTAT_IOVR_SHIFT))

#define  SCAN_SSTAT_RESERVED2_MASK 0x2
#define  SCAN_SSTAT_RESERVED2_SHIFT 1
#define  SCAN_SSTAT_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_RESERVED2_MASK) >> SCAN_SSTAT_RESERVED2_SHIFT)
#define  SCAN_SSTAT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_RESERVED2_MASK) | (((uint32_t)val) << SCAN_SSTAT_RESERVED2_SHIFT))

#define  SCAN_SSTAT_SCMDCOMP_MASK 0x1
#define  SCAN_SSTAT_SCMDCOMP_SHIFT 0
#define  SCAN_SSTAT_SCMDCOMP_MASK_SHIFT(reg) (((reg) & SCAN_SSTAT_SCMDCOMP_MASK) >> SCAN_SSTAT_SCMDCOMP_SHIFT)
#define  SCAN_SSTAT_SCMDCOMP_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SSTAT_SCMDCOMP_MASK) | (((uint32_t)val) << SCAN_SSTAT_SCMDCOMP_SHIFT))

//====================================================================
//Register: Scan Interrupt Acknowledge (SIACK)
/** \brief Scan Interrupt Acknowledge Register.*/
//====================================================================

#define  SCAN_SIACK_RESERVED1_MASK 0xffffffe0
#define  SCAN_SIACK_RESERVED1_SHIFT 5
#define  SCAN_SIACK_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_SIACK_RESERVED1_MASK) >> SCAN_SIACK_RESERVED1_SHIFT)
#define  SCAN_SIACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SIACK_RESERVED1_MASK) | (((uint32_t)val) << SCAN_SIACK_RESERVED1_SHIFT))

#define  SCAN_SIACK_UMSACK_MASK 0x10
#define  SCAN_SIACK_UMSACK_SHIFT 4
#define  SCAN_SIACK_UMSACK_MASK_SHIFT(reg) (((reg) & SCAN_SIACK_UMSACK_MASK) >> SCAN_SIACK_UMSACK_SHIFT)
#define  SCAN_SIACK_UMSACK_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SIACK_UMSACK_MASK) | (((uint32_t)val) << SCAN_SIACK_UMSACK_SHIFT))

#define  SCAN_SIACK_QOVRACK_MASK 0x8
#define  SCAN_SIACK_QOVRACK_SHIFT 3
#define  SCAN_SIACK_QOVRACK_MASK_SHIFT(reg) (((reg) & SCAN_SIACK_QOVRACK_MASK) >> SCAN_SIACK_QOVRACK_SHIFT)
#define  SCAN_SIACK_QOVRACK_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SIACK_QOVRACK_MASK) | (((uint32_t)val) << SCAN_SIACK_QOVRACK_SHIFT))

#define  SCAN_SIACK_CIOVRACK_MASK 0x4
#define  SCAN_SIACK_CIOVRACK_SHIFT 2
#define  SCAN_SIACK_CIOVRACK_MASK_SHIFT(reg) (((reg) & SCAN_SIACK_CIOVRACK_MASK) >> SCAN_SIACK_CIOVRACK_SHIFT)
#define  SCAN_SIACK_CIOVRACK_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SIACK_CIOVRACK_MASK) | (((uint32_t)val) << SCAN_SIACK_CIOVRACK_SHIFT))

#define  SCAN_SIACK_RESERVED2_MASK 0x2
#define  SCAN_SIACK_RESERVED2_SHIFT 1
#define  SCAN_SIACK_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_SIACK_RESERVED2_MASK) >> SCAN_SIACK_RESERVED2_SHIFT)
#define  SCAN_SIACK_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SIACK_RESERVED2_MASK) | (((uint32_t)val) << SCAN_SIACK_RESERVED2_SHIFT))

#define  SCAN_SIACK_SCMDACK_MASK 0x1
#define  SCAN_SIACK_SCMDACK_SHIFT 0
#define  SCAN_SIACK_SCMDACK_MASK_SHIFT(reg) (((reg) & SCAN_SIACK_SCMDACK_MASK) >> SCAN_SIACK_SCMDACK_SHIFT)
#define  SCAN_SIACK_SCMDACK_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SIACK_SCMDACK_MASK) | (((uint32_t)val) << SCAN_SIACK_SCMDACK_SHIFT))

//====================================================================
//Register: Scan Pixels/Line (SCANX)
/** \brief This register contains the number of data pixels to transfer in a scanline. SCANX is loaded into a counter at the beginning of a scanline after the Start/SP/TG pulse deasserts. The counter then decrements at every data pixel as determined by the P bit in the PSEQ1 and PSEQ2 registers. SCANX includes the dummy pixels at the beginning of a scanline and the data pixels. It does not include the dummy pixels at the end of the scanline after the last good data pixel.<br/>*/
//====================================================================

#define  SCAN_SCANX_EXPVAL_MASK 0xffff0000
#define  SCAN_SCANX_EXPVAL_SHIFT 16
#define  SCAN_SCANX_EXPVAL_MASK_SHIFT(reg) (((reg) & SCAN_SCANX_EXPVAL_MASK) >> SCAN_SCANX_EXPVAL_SHIFT)
#define  SCAN_SCANX_EXPVAL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCANX_EXPVAL_MASK) | (((uint32_t)val) << SCAN_SCANX_EXPVAL_SHIFT))

#define  SCAN_SCANX_SCANX_MASK 0xffff
#define  SCAN_SCANX_SCANX_SHIFT 0
#define  SCAN_SCANX_SCANX_MASK_SHIFT(reg) (((reg) & SCAN_SCANX_SCANX_MASK) >> SCAN_SCANX_SCANX_SHIFT)
#define  SCAN_SCANX_SCANX_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCANX_SCANX_MASK) | (((uint32_t)val) << SCAN_SCANX_SCANX_SHIFT))

//====================================================================
//Register: Scan Cycle (SCYCLE)
/** \brief This register, in conjunction with the Pixel Period programmed in the SCFG2 register, and the Scan Clock Period set in the SCMD register, sets the scanline period for internal scanline generation. Scanline timing for external motor line syncs is described below. The EXP field in the register is the current value of exposure period measured. See Scan Config2 Register for details on how the exposure is measured. <br/>
<br/>
Two modes of operation are supported: DC/External Motor Scanline generation and Internal Scanline generation. Scanlines are delineated by a signal called "Linestart," which is generated by the external or internal scanline generator.<br/>
<br/>
DC Motor/External Motor Mode:<br/>
<br/>
If using a DC Motor or other external Motor for scanning pages, Linestart is derived from the Motor1 or Motor2 line sync pulses from the motor controllers. The MCFG register selects which line sync pulse is enabled (see below). SCFG2.PIXPER is still used to determine the pixel period for generating all clocks, LED's etc. but each new scanline is started by the motor line sync pulse.<br/>
<br/>
Two Line Sync modes are available, selected by bits in the Motor Configuration Register (MCFG): <br/>
<br/>
Linestart generated for the first Scanline only with Internal ScanLine generation thereafter ("Sync First"); or <br/>
<br/>
Linestart generated for every Scanline ("Sync Every").<br/>
<br/>
When the "Sync Every" mode is selected, the external motor controller must provide enough line sync pulses for all the scan commands to complete. For CIS mono or CCD sensors, this is (Total Scanlines + 2). The additional 2 sync pulses are needed due to the way the internal state machines start and end a series of scan commands. For CIS Color sensors the number of sync pulses = (Total Scanlines + 6), since a line is scanned once for each color, or 3x the mono case. For more details, refer to the Scan Control Sequences section.<br/>
<br/>
Internal Scanline Generator Mode:<br/>
<br/>
The timing diagram below illustrates an example of Internal Scanline timing. The register setting: SCYCLE = 15. SCLKPER = 1 is shown for illustration only; a realistic number would be much greater (see example below).<br/>
<br/>
<img src="../../internal/scanline_timing.jpg"></img>
<br/>
<br/>
From SCFG2, Pixel Clock period = Pix Period = (PIXPER + 1) * ClkPeriod<br/>
<br/>
In addition to SCFG2.PIXPER, two fields are used to create the scanline period: SCMD.SCLKPER creates a basic Scan Clock and SCYCLE.SCYCLE (referred to as SCLKPER and SCYCLE below) creates the Linestart pulses:<br/>
<br/>
Scan Clock = 1/((SCLKPER+1)*(Pix Period)) Hz Linestart = (Scan Clock)*(1/ (SCYCLE+1)) Hz<br/>
<br/>
Since the Scan Clock and Linestart are derived from the Pix Period, the total # pixels in a Scanline determines the selection of SCLKPER and SCYCLE as follows:<br/>
<br/>
Scanline pixels = (SCLKPER+1) *( SCYCLE+1) <br/>
<br/>
For example, for a 1200 dpi CCD sensor with 10,680 active pixels (~10,834 total pixels), SCLKPER = 338 and SCYCLE = 31 could be used. The total pixels in a Scanline includes active pixels and dummy or inactive pixels.<br/>
<br/>
Note: The number of Scanline pixels should be programmed to an even value. The number of Scanline pixels is the total number from Linestart to Linestart. If programmed to an odd value, sensor and/or AFE clocks programmed to toggle at SCFG2.PIXPER will re-initialize their phase at the beginning of each ScanLine. For some CIS sensors, this can cause difficulty in meeting the P1Clk to SP edge timing.<br/>
<br/>
For either mode of scanline generation, the Linestart pulse is used to delineate the beginning and end of each sensor scanline. It is used to trigger all events that occur within a scanline; for example, the Start (Transfer) pulse, CIS LED turn on and off times, data transfer start and stop, and to decrement the internal line counter = SSTAT.SCANYR or reload a new value from the SCANY register. Note that for CIS sensors in color mode, an internal divide-by-3 counter is used to decrement the SSTAT.SCANYR counter, since each scanline is read once for each color. For more details, refer to the Scan Control Sequences section.<br/>*/
//====================================================================

#define  SCAN_SCYCLE_EXPPER_MASK 0xffff0000
#define  SCAN_SCYCLE_EXPPER_SHIFT 16
#define  SCAN_SCYCLE_EXPPER_MASK_SHIFT(reg) (((reg) & SCAN_SCYCLE_EXPPER_MASK) >> SCAN_SCYCLE_EXPPER_SHIFT)
#define  SCAN_SCYCLE_EXPPER_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCYCLE_EXPPER_MASK) | (((uint32_t)val) << SCAN_SCYCLE_EXPPER_SHIFT))

#define  SCAN_SCYCLE_RESERVED1_MASK 0xffc0
#define  SCAN_SCYCLE_RESERVED1_SHIFT 6
#define  SCAN_SCYCLE_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_SCYCLE_RESERVED1_MASK) >> SCAN_SCYCLE_RESERVED1_SHIFT)
#define  SCAN_SCYCLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCYCLE_RESERVED1_MASK) | (((uint32_t)val) << SCAN_SCYCLE_RESERVED1_SHIFT))

#define  SCAN_SCYCLE_SCYCLE_MASK 0x3f
#define  SCAN_SCYCLE_SCYCLE_SHIFT 0
#define  SCAN_SCYCLE_SCYCLE_MASK_SHIFT(reg) (((reg) & SCAN_SCYCLE_SCYCLE_MASK) >> SCAN_SCYCLE_SCYCLE_SHIFT)
#define  SCAN_SCYCLE_SCYCLE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCYCLE_SCYCLE_MASK) | (((uint32_t)val) << SCAN_SCYCLE_SCYCLE_SHIFT))

//====================================================================
//Register: Scan Start Config1 (STCFG1)
/** \brief Scan Start Pulse Conguration 1 Register.<br/>*/
//====================================================================

#define  SCAN_STCFG1_SPGEN_MASK 0x80000000
#define  SCAN_STCFG1_SPGEN_SHIFT 31
#define  SCAN_STCFG1_SPGEN_MASK_SHIFT(reg) (((reg) & SCAN_STCFG1_SPGEN_MASK) >> SCAN_STCFG1_SPGEN_SHIFT)
#define  SCAN_STCFG1_SPGEN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG1_SPGEN_MASK) | (((uint32_t)val) << SCAN_STCFG1_SPGEN_SHIFT))

#define  SCAN_STCFG1_RESERVED1_MASK 0x70000000
#define  SCAN_STCFG1_RESERVED1_SHIFT 28
#define  SCAN_STCFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_STCFG1_RESERVED1_MASK) >> SCAN_STCFG1_RESERVED1_SHIFT)
#define  SCAN_STCFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG1_RESERVED1_MASK) | (((uint32_t)val) << SCAN_STCFG1_RESERVED1_SHIFT))

#define  SCAN_STCFG1_MSKEXTD_MASK 0xf000000
#define  SCAN_STCFG1_MSKEXTD_SHIFT 24
#define  SCAN_STCFG1_MSKEXTD_MASK_SHIFT(reg) (((reg) & SCAN_STCFG1_MSKEXTD_MASK) >> SCAN_STCFG1_MSKEXTD_SHIFT)
#define  SCAN_STCFG1_MSKEXTD_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG1_MSKEXTD_MASK) | (((uint32_t)val) << SCAN_STCFG1_MSKEXTD_SHIFT))

#define  SCAN_STCFG1_SSME1_MASK 0xff0000
#define  SCAN_STCFG1_SSME1_SHIFT 16
#define  SCAN_STCFG1_SSME1_MASK_SHIFT(reg) (((reg) & SCAN_STCFG1_SSME1_MASK) >> SCAN_STCFG1_SSME1_SHIFT)
#define  SCAN_STCFG1_SSME1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG1_SSME1_MASK) | (((uint32_t)val) << SCAN_STCFG1_SSME1_SHIFT))

#define  SCAN_STCFG1_RESERVED2_MASK 0xff00
#define  SCAN_STCFG1_RESERVED2_SHIFT 8
#define  SCAN_STCFG1_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_STCFG1_RESERVED2_MASK) >> SCAN_STCFG1_RESERVED2_SHIFT)
#define  SCAN_STCFG1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG1_RESERVED2_MASK) | (((uint32_t)val) << SCAN_STCFG1_RESERVED2_SHIFT))

#define  SCAN_STCFG1_SSME2_MASK 0xff
#define  SCAN_STCFG1_SSME2_SHIFT 0
#define  SCAN_STCFG1_SSME2_MASK_SHIFT(reg) (((reg) & SCAN_STCFG1_SSME2_MASK) >> SCAN_STCFG1_SSME2_SHIFT)
#define  SCAN_STCFG1_SSME2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG1_SSME2_MASK) | (((uint32_t)val) << SCAN_STCFG1_SSME2_SHIFT))

//====================================================================
//Register: Scan Start Config2 (STCFG2)
/** \brief Scan Start Pulse Conguration 2 Register.<br/>*/
//====================================================================

#define  SCAN_STCFG2_RESERVED1_MASK 0xff000000
#define  SCAN_STCFG2_RESERVED1_SHIFT 24
#define  SCAN_STCFG2_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_STCFG2_RESERVED1_MASK) >> SCAN_STCFG2_RESERVED1_SHIFT)
#define  SCAN_STCFG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG2_RESERVED1_MASK) | (((uint32_t)val) << SCAN_STCFG2_RESERVED1_SHIFT))

#define  SCAN_STCFG2_SPE1_MASK 0xff0000
#define  SCAN_STCFG2_SPE1_SHIFT 16
#define  SCAN_STCFG2_SPE1_MASK_SHIFT(reg) (((reg) & SCAN_STCFG2_SPE1_MASK) >> SCAN_STCFG2_SPE1_SHIFT)
#define  SCAN_STCFG2_SPE1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG2_SPE1_MASK) | (((uint32_t)val) << SCAN_STCFG2_SPE1_SHIFT))

#define  SCAN_STCFG2_RESERVED2_MASK 0xff00
#define  SCAN_STCFG2_RESERVED2_SHIFT 8
#define  SCAN_STCFG2_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_STCFG2_RESERVED2_MASK) >> SCAN_STCFG2_RESERVED2_SHIFT)
#define  SCAN_STCFG2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG2_RESERVED2_MASK) | (((uint32_t)val) << SCAN_STCFG2_RESERVED2_SHIFT))

#define  SCAN_STCFG2_SPE2_MASK 0xff
#define  SCAN_STCFG2_SPE2_SHIFT 0
#define  SCAN_STCFG2_SPE2_MASK_SHIFT(reg) (((reg) & SCAN_STCFG2_SPE2_MASK) >> SCAN_STCFG2_SPE2_SHIFT)
#define  SCAN_STCFG2_SPE2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG2_SPE2_MASK) | (((uint32_t)val) << SCAN_STCFG2_SPE2_SHIFT))

//====================================================================
//Register: Scan Start Config3 (STCFG3)
/** \brief STCFG3 and STCFG4 determine up to 8 additional edges, APH1E1-8, for the Phase1 Clock. The edge times are referenced to the beginning of the scanline. The APH1E1-8 Edges generate an internal signal, P1ClkAdd, that is OR'd with the normal Phase1 Clock signal. Therefore, when using these Phase1 Edges the SCFG1.SSMEN[3] = SCFG1 bit 19 must be set = 1, and the SCFG1.SMPOL[3] = SCFG1 bit 15 must be clear = 0. The Polarity/Initial Value of P1ClkAdd is set by SCFG1.P1APOL. STCFG1.SPGEN controls whether edge values are counted in pixel periods or system clock periods. <br/>
<br/>
If not used, APH1E1-8 should be programmed to 'hff.<br/>*/
//====================================================================

#define  SCAN_STCFG3_APH1E1_MASK 0xff000000
#define  SCAN_STCFG3_APH1E1_SHIFT 24
#define  SCAN_STCFG3_APH1E1_MASK_SHIFT(reg) (((reg) & SCAN_STCFG3_APH1E1_MASK) >> SCAN_STCFG3_APH1E1_SHIFT)
#define  SCAN_STCFG3_APH1E1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG3_APH1E1_MASK) | (((uint32_t)val) << SCAN_STCFG3_APH1E1_SHIFT))

#define  SCAN_STCFG3_APH1E2_MASK 0xff0000
#define  SCAN_STCFG3_APH1E2_SHIFT 16
#define  SCAN_STCFG3_APH1E2_MASK_SHIFT(reg) (((reg) & SCAN_STCFG3_APH1E2_MASK) >> SCAN_STCFG3_APH1E2_SHIFT)
#define  SCAN_STCFG3_APH1E2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG3_APH1E2_MASK) | (((uint32_t)val) << SCAN_STCFG3_APH1E2_SHIFT))

#define  SCAN_STCFG3_APH1E3_MASK 0xff00
#define  SCAN_STCFG3_APH1E3_SHIFT 8
#define  SCAN_STCFG3_APH1E3_MASK_SHIFT(reg) (((reg) & SCAN_STCFG3_APH1E3_MASK) >> SCAN_STCFG3_APH1E3_SHIFT)
#define  SCAN_STCFG3_APH1E3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG3_APH1E3_MASK) | (((uint32_t)val) << SCAN_STCFG3_APH1E3_SHIFT))

#define  SCAN_STCFG3_APH1E4_MASK 0xff
#define  SCAN_STCFG3_APH1E4_SHIFT 0
#define  SCAN_STCFG3_APH1E4_MASK_SHIFT(reg) (((reg) & SCAN_STCFG3_APH1E4_MASK) >> SCAN_STCFG3_APH1E4_SHIFT)
#define  SCAN_STCFG3_APH1E4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG3_APH1E4_MASK) | (((uint32_t)val) << SCAN_STCFG3_APH1E4_SHIFT))

//====================================================================
//Register: Scan Start Config4 (STCFG4)
/** \brief Scan Start Pulse Conguration 4 Register - Additional Phase1 Clock Edges.<br/>*/
//====================================================================

#define  SCAN_STCFG4_APH1E5_MASK 0xff000000
#define  SCAN_STCFG4_APH1E5_SHIFT 24
#define  SCAN_STCFG4_APH1E5_MASK_SHIFT(reg) (((reg) & SCAN_STCFG4_APH1E5_MASK) >> SCAN_STCFG4_APH1E5_SHIFT)
#define  SCAN_STCFG4_APH1E5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG4_APH1E5_MASK) | (((uint32_t)val) << SCAN_STCFG4_APH1E5_SHIFT))

#define  SCAN_STCFG4_APH1E6_MASK 0xff0000
#define  SCAN_STCFG4_APH1E6_SHIFT 16
#define  SCAN_STCFG4_APH1E6_MASK_SHIFT(reg) (((reg) & SCAN_STCFG4_APH1E6_MASK) >> SCAN_STCFG4_APH1E6_SHIFT)
#define  SCAN_STCFG4_APH1E6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG4_APH1E6_MASK) | (((uint32_t)val) << SCAN_STCFG4_APH1E6_SHIFT))

#define  SCAN_STCFG4_APH1E7_MASK 0xff00
#define  SCAN_STCFG4_APH1E7_SHIFT 8
#define  SCAN_STCFG4_APH1E7_MASK_SHIFT(reg) (((reg) & SCAN_STCFG4_APH1E7_MASK) >> SCAN_STCFG4_APH1E7_SHIFT)
#define  SCAN_STCFG4_APH1E7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG4_APH1E7_MASK) | (((uint32_t)val) << SCAN_STCFG4_APH1E7_SHIFT))

#define  SCAN_STCFG4_APH1E8_MASK 0xff
#define  SCAN_STCFG4_APH1E8_SHIFT 0
#define  SCAN_STCFG4_APH1E8_MASK_SHIFT(reg) (((reg) & SCAN_STCFG4_APH1E8_MASK) >> SCAN_STCFG4_APH1E8_SHIFT)
#define  SCAN_STCFG4_APH1E8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_STCFG4_APH1E8_MASK) | (((uint32_t)val) << SCAN_STCFG4_APH1E8_SHIFT))

//====================================================================
//Register: Clamp Config (CCFG)
/** \brief This register controls the Clamp mode and on-off edges for Sensor and AFE Clamp signals in Line Clamp mode. See the timing diagram above for example timing. Note that the Edge values are counted in pixel periods only. Also note that the CCD and AFE Clamp signals are not mutually exclusive. While the CCD Clamp signal, generated when CPMODE = 1, is independent of the CLMODE setting, the AFE Clamp signal, generated when CLMODE = 1, must also have CPMODE = 1. In other words, if AFE Clamp mode is enabled, then the CCD Clamp mode must also be enabled. The CCD Clamp signal is enabled by SCFG1 bit 28; polarity is set by SCFG1 bit 4. The AFE Clamp signal is enabled by SCFG1 bit 24; polarity is set by SCFG1 bit 0.<br/>*/
//====================================================================

#define  SCAN_CCFG_CPMODE_MASK 0x80000000
#define  SCAN_CCFG_CPMODE_SHIFT 31
#define  SCAN_CCFG_CPMODE_MASK_SHIFT(reg) (((reg) & SCAN_CCFG_CPMODE_MASK) >> SCAN_CCFG_CPMODE_SHIFT)
#define  SCAN_CCFG_CPMODE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CCFG_CPMODE_MASK) | (((uint32_t)val) << SCAN_CCFG_CPMODE_SHIFT))

#define  SCAN_CCFG_CLMODE_MASK 0x40000000
#define  SCAN_CCFG_CLMODE_SHIFT 30
#define  SCAN_CCFG_CLMODE_MASK_SHIFT(reg) (((reg) & SCAN_CCFG_CLMODE_MASK) >> SCAN_CCFG_CLMODE_SHIFT)
#define  SCAN_CCFG_CLMODE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CCFG_CLMODE_MASK) | (((uint32_t)val) << SCAN_CCFG_CLMODE_SHIFT))

#define  SCAN_CCFG_RESERVED1_MASK 0x3f000000
#define  SCAN_CCFG_RESERVED1_SHIFT 24
#define  SCAN_CCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_CCFG_RESERVED1_MASK) >> SCAN_CCFG_RESERVED1_SHIFT)
#define  SCAN_CCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CCFG_RESERVED1_MASK) | (((uint32_t)val) << SCAN_CCFG_RESERVED1_SHIFT))

#define  SCAN_CCFG_CLE1_MASK 0xff0000
#define  SCAN_CCFG_CLE1_SHIFT 16
#define  SCAN_CCFG_CLE1_MASK_SHIFT(reg) (((reg) & SCAN_CCFG_CLE1_MASK) >> SCAN_CCFG_CLE1_SHIFT)
#define  SCAN_CCFG_CLE1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CCFG_CLE1_MASK) | (((uint32_t)val) << SCAN_CCFG_CLE1_SHIFT))

#define  SCAN_CCFG_CCLE2_MASK 0xff00
#define  SCAN_CCFG_CCLE2_SHIFT 8
#define  SCAN_CCFG_CCLE2_MASK_SHIFT(reg) (((reg) & SCAN_CCFG_CCLE2_MASK) >> SCAN_CCFG_CCLE2_SHIFT)
#define  SCAN_CCFG_CCLE2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CCFG_CCLE2_MASK) | (((uint32_t)val) << SCAN_CCFG_CCLE2_SHIFT))

#define  SCAN_CCFG_ACLE2_MASK 0xff
#define  SCAN_CCFG_ACLE2_SHIFT 0
#define  SCAN_CCFG_ACLE2_MASK_SHIFT(reg) (((reg) & SCAN_CCFG_ACLE2_MASK) >> SCAN_CCFG_ACLE2_SHIFT)
#define  SCAN_CCFG_ACLE2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CCFG_ACLE2_MASK) | (((uint32_t)val) << SCAN_CCFG_ACLE2_SHIFT))

//====================================================================
//Register: Sensor Clocks 1 (SCLK1)
/** \brief The Sensor Clocks Registers and AFE Clocks Registers contain Edge values for P1Clk, P2Clk, RSClk, CPClk, AFE MClk, VSClk, and CLClk. The number of programmable Edges varies for each clock: P1Clk and P2Clk have up to 8 Edges, RSClk, CPClk, VSClk and CLClk have 4 Edges each. The AFE MClk has up to 12 programmable Edge values. If the clock is enabled in SCFG1, the polarity is determined by SCFG1.SCPOL and SCFG1.ACPOL. For the Sensor Clocks1 and Sensor Clocks2 Registers, which define the P1Clk, the clock is enabled by SCFG1 bit 31 and polarity is set by SCFG1 bit 7. Note that each clock must have an even number of edges within a pixel period.<br/>
<br/>
Unused Edges must be programmed to 'hFF.<br/>
<br/>
The programmable clocks are generated from hardware counters using the PIXPER field in the SCFG2 register. Valid values for each Clk's EDGEi value are from 0 -> SCFG2.PIXPER. Example timing for SCFG2. PIXPER = 9, and P1E1 = 0, P1E2 = 5, MCE1 = 1, MCE2 = 5, VSE1 = 1, VSE2 = 3, is shown in the timing diagram below. The Pixel Period start is referenced to PixPer_Cnt = 0.<br/>
<br/>
<br/>
<img src="../../internal/scanclock_timing.jpg"></img><br/>
<br/>*/
//====================================================================

#define  SCAN_SCLK1_P1E1_MASK 0xff000000
#define  SCAN_SCLK1_P1E1_SHIFT 24
#define  SCAN_SCLK1_P1E1_MASK_SHIFT(reg) (((reg) & SCAN_SCLK1_P1E1_MASK) >> SCAN_SCLK1_P1E1_SHIFT)
#define  SCAN_SCLK1_P1E1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK1_P1E1_MASK) | (((uint32_t)val) << SCAN_SCLK1_P1E1_SHIFT))

#define  SCAN_SCLK1_P1E2_MASK 0xff0000
#define  SCAN_SCLK1_P1E2_SHIFT 16
#define  SCAN_SCLK1_P1E2_MASK_SHIFT(reg) (((reg) & SCAN_SCLK1_P1E2_MASK) >> SCAN_SCLK1_P1E2_SHIFT)
#define  SCAN_SCLK1_P1E2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK1_P1E2_MASK) | (((uint32_t)val) << SCAN_SCLK1_P1E2_SHIFT))

#define  SCAN_SCLK1_P1E3_MASK 0xff00
#define  SCAN_SCLK1_P1E3_SHIFT 8
#define  SCAN_SCLK1_P1E3_MASK_SHIFT(reg) (((reg) & SCAN_SCLK1_P1E3_MASK) >> SCAN_SCLK1_P1E3_SHIFT)
#define  SCAN_SCLK1_P1E3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK1_P1E3_MASK) | (((uint32_t)val) << SCAN_SCLK1_P1E3_SHIFT))

#define  SCAN_SCLK1_P1E4_MASK 0xff
#define  SCAN_SCLK1_P1E4_SHIFT 0
#define  SCAN_SCLK1_P1E4_MASK_SHIFT(reg) (((reg) & SCAN_SCLK1_P1E4_MASK) >> SCAN_SCLK1_P1E4_SHIFT)
#define  SCAN_SCLK1_P1E4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK1_P1E4_MASK) | (((uint32_t)val) << SCAN_SCLK1_P1E4_SHIFT))

//====================================================================
//Register: Sensor Clocks 2 (SCLK2)
/** \brief Sensor Clocks 2 Register.*/
//====================================================================

#define  SCAN_SCLK2_P1E5_MASK 0xff000000
#define  SCAN_SCLK2_P1E5_SHIFT 24
#define  SCAN_SCLK2_P1E5_MASK_SHIFT(reg) (((reg) & SCAN_SCLK2_P1E5_MASK) >> SCAN_SCLK2_P1E5_SHIFT)
#define  SCAN_SCLK2_P1E5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK2_P1E5_MASK) | (((uint32_t)val) << SCAN_SCLK2_P1E5_SHIFT))

#define  SCAN_SCLK2_P1E6_MASK 0xff0000
#define  SCAN_SCLK2_P1E6_SHIFT 16
#define  SCAN_SCLK2_P1E6_MASK_SHIFT(reg) (((reg) & SCAN_SCLK2_P1E6_MASK) >> SCAN_SCLK2_P1E6_SHIFT)
#define  SCAN_SCLK2_P1E6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK2_P1E6_MASK) | (((uint32_t)val) << SCAN_SCLK2_P1E6_SHIFT))

#define  SCAN_SCLK2_P1E7_MASK 0xff00
#define  SCAN_SCLK2_P1E7_SHIFT 8
#define  SCAN_SCLK2_P1E7_MASK_SHIFT(reg) (((reg) & SCAN_SCLK2_P1E7_MASK) >> SCAN_SCLK2_P1E7_SHIFT)
#define  SCAN_SCLK2_P1E7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK2_P1E7_MASK) | (((uint32_t)val) << SCAN_SCLK2_P1E7_SHIFT))

#define  SCAN_SCLK2_P1E8_MASK 0xff
#define  SCAN_SCLK2_P1E8_SHIFT 0
#define  SCAN_SCLK2_P1E8_MASK_SHIFT(reg) (((reg) & SCAN_SCLK2_P1E8_MASK) >> SCAN_SCLK2_P1E8_SHIFT)
#define  SCAN_SCLK2_P1E8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK2_P1E8_MASK) | (((uint32_t)val) << SCAN_SCLK2_P1E8_SHIFT))

//====================================================================
//Register: Sensor Clocks 3 (SCLK3)
/** \brief This Register and the Sensor Clocks4 Register contain the 8 Edge values for P2Clk. This clock is enabled by SCFG1 bit 30; polarity is set by SCFG1 bit 6. Note that the clock must have an even number of edges within a pixel period.<br/>*/
//====================================================================

#define  SCAN_SCLK3_P2E1_MASK 0xff000000
#define  SCAN_SCLK3_P2E1_SHIFT 24
#define  SCAN_SCLK3_P2E1_MASK_SHIFT(reg) (((reg) & SCAN_SCLK3_P2E1_MASK) >> SCAN_SCLK3_P2E1_SHIFT)
#define  SCAN_SCLK3_P2E1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK3_P2E1_MASK) | (((uint32_t)val) << SCAN_SCLK3_P2E1_SHIFT))

#define  SCAN_SCLK3_P2E2_MASK 0xff0000
#define  SCAN_SCLK3_P2E2_SHIFT 16
#define  SCAN_SCLK3_P2E2_MASK_SHIFT(reg) (((reg) & SCAN_SCLK3_P2E2_MASK) >> SCAN_SCLK3_P2E2_SHIFT)
#define  SCAN_SCLK3_P2E2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK3_P2E2_MASK) | (((uint32_t)val) << SCAN_SCLK3_P2E2_SHIFT))

#define  SCAN_SCLK3_P2E3_MASK 0xff00
#define  SCAN_SCLK3_P2E3_SHIFT 8
#define  SCAN_SCLK3_P2E3_MASK_SHIFT(reg) (((reg) & SCAN_SCLK3_P2E3_MASK) >> SCAN_SCLK3_P2E3_SHIFT)
#define  SCAN_SCLK3_P2E3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK3_P2E3_MASK) | (((uint32_t)val) << SCAN_SCLK3_P2E3_SHIFT))

#define  SCAN_SCLK3_P2E4_MASK 0xff
#define  SCAN_SCLK3_P2E4_SHIFT 0
#define  SCAN_SCLK3_P2E4_MASK_SHIFT(reg) (((reg) & SCAN_SCLK3_P2E4_MASK) >> SCAN_SCLK3_P2E4_SHIFT)
#define  SCAN_SCLK3_P2E4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK3_P2E4_MASK) | (((uint32_t)val) << SCAN_SCLK3_P2E4_SHIFT))

//====================================================================
//Register: Sensor Clocks 4 (SCLK4)
/** \brief Sensor Clocks 4 Register.*/
//====================================================================

#define  SCAN_SCLK4_P2E5_MASK 0xff000000
#define  SCAN_SCLK4_P2E5_SHIFT 24
#define  SCAN_SCLK4_P2E5_MASK_SHIFT(reg) (((reg) & SCAN_SCLK4_P2E5_MASK) >> SCAN_SCLK4_P2E5_SHIFT)
#define  SCAN_SCLK4_P2E5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK4_P2E5_MASK) | (((uint32_t)val) << SCAN_SCLK4_P2E5_SHIFT))

#define  SCAN_SCLK4_P2E6_MASK 0xff0000
#define  SCAN_SCLK4_P2E6_SHIFT 16
#define  SCAN_SCLK4_P2E6_MASK_SHIFT(reg) (((reg) & SCAN_SCLK4_P2E6_MASK) >> SCAN_SCLK4_P2E6_SHIFT)
#define  SCAN_SCLK4_P2E6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK4_P2E6_MASK) | (((uint32_t)val) << SCAN_SCLK4_P2E6_SHIFT))

#define  SCAN_SCLK4_P2E7_MASK 0xff00
#define  SCAN_SCLK4_P2E7_SHIFT 8
#define  SCAN_SCLK4_P2E7_MASK_SHIFT(reg) (((reg) & SCAN_SCLK4_P2E7_MASK) >> SCAN_SCLK4_P2E7_SHIFT)
#define  SCAN_SCLK4_P2E7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK4_P2E7_MASK) | (((uint32_t)val) << SCAN_SCLK4_P2E7_SHIFT))

#define  SCAN_SCLK4_P2E8_MASK 0xff
#define  SCAN_SCLK4_P2E8_SHIFT 0
#define  SCAN_SCLK4_P2E8_MASK_SHIFT(reg) (((reg) & SCAN_SCLK4_P2E8_MASK) >> SCAN_SCLK4_P2E8_SHIFT)
#define  SCAN_SCLK4_P2E8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK4_P2E8_MASK) | (((uint32_t)val) << SCAN_SCLK4_P2E8_SHIFT))

//====================================================================
//Register: Sensor Clocks 5 (SCLK5)
/** \brief This register is used for up to 4 programmable RSClk Edges. This clock is enabled by SCFG1 bit 29; polarity is set by SCFG1 bit 5. Note that the clock must have an even number of edges within a pixel period.<br/>*/
//====================================================================

#define  SCAN_SCLK5_RSE1_MASK 0xff000000
#define  SCAN_SCLK5_RSE1_SHIFT 24
#define  SCAN_SCLK5_RSE1_MASK_SHIFT(reg) (((reg) & SCAN_SCLK5_RSE1_MASK) >> SCAN_SCLK5_RSE1_SHIFT)
#define  SCAN_SCLK5_RSE1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK5_RSE1_MASK) | (((uint32_t)val) << SCAN_SCLK5_RSE1_SHIFT))

#define  SCAN_SCLK5_RSE2_MASK 0xff0000
#define  SCAN_SCLK5_RSE2_SHIFT 16
#define  SCAN_SCLK5_RSE2_MASK_SHIFT(reg) (((reg) & SCAN_SCLK5_RSE2_MASK) >> SCAN_SCLK5_RSE2_SHIFT)
#define  SCAN_SCLK5_RSE2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK5_RSE2_MASK) | (((uint32_t)val) << SCAN_SCLK5_RSE2_SHIFT))

#define  SCAN_SCLK5_RSE3_MASK 0xff00
#define  SCAN_SCLK5_RSE3_SHIFT 8
#define  SCAN_SCLK5_RSE3_MASK_SHIFT(reg) (((reg) & SCAN_SCLK5_RSE3_MASK) >> SCAN_SCLK5_RSE3_SHIFT)
#define  SCAN_SCLK5_RSE3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK5_RSE3_MASK) | (((uint32_t)val) << SCAN_SCLK5_RSE3_SHIFT))

#define  SCAN_SCLK5_RSE4_MASK 0xff
#define  SCAN_SCLK5_RSE4_SHIFT 0
#define  SCAN_SCLK5_RSE4_MASK_SHIFT(reg) (((reg) & SCAN_SCLK5_RSE4_MASK) >> SCAN_SCLK5_RSE4_SHIFT)
#define  SCAN_SCLK5_RSE4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK5_RSE4_MASK) | (((uint32_t)val) << SCAN_SCLK5_RSE4_SHIFT))

//====================================================================
//Register: Sensor Clocks 6 (SCLK6)
/** \brief This register is used for up to 4 programmable CPClk Edges. This clock is enabled by SCFG1 bit 28; polarity is set by SCFG1 bit 4.<br/>*/
//====================================================================

#define  SCAN_SCLK6_CPE1_MASK 0xff000000
#define  SCAN_SCLK6_CPE1_SHIFT 24
#define  SCAN_SCLK6_CPE1_MASK_SHIFT(reg) (((reg) & SCAN_SCLK6_CPE1_MASK) >> SCAN_SCLK6_CPE1_SHIFT)
#define  SCAN_SCLK6_CPE1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK6_CPE1_MASK) | (((uint32_t)val) << SCAN_SCLK6_CPE1_SHIFT))

#define  SCAN_SCLK6_CPE2_MASK 0xff0000
#define  SCAN_SCLK6_CPE2_SHIFT 16
#define  SCAN_SCLK6_CPE2_MASK_SHIFT(reg) (((reg) & SCAN_SCLK6_CPE2_MASK) >> SCAN_SCLK6_CPE2_SHIFT)
#define  SCAN_SCLK6_CPE2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK6_CPE2_MASK) | (((uint32_t)val) << SCAN_SCLK6_CPE2_SHIFT))

#define  SCAN_SCLK6_CPE3_MASK 0xff00
#define  SCAN_SCLK6_CPE3_SHIFT 8
#define  SCAN_SCLK6_CPE3_MASK_SHIFT(reg) (((reg) & SCAN_SCLK6_CPE3_MASK) >> SCAN_SCLK6_CPE3_SHIFT)
#define  SCAN_SCLK6_CPE3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK6_CPE3_MASK) | (((uint32_t)val) << SCAN_SCLK6_CPE3_SHIFT))

#define  SCAN_SCLK6_CPE4_MASK 0xff
#define  SCAN_SCLK6_CPE4_SHIFT 0
#define  SCAN_SCLK6_CPE4_MASK_SHIFT(reg) (((reg) & SCAN_SCLK6_CPE4_MASK) >> SCAN_SCLK6_CPE4_SHIFT)
#define  SCAN_SCLK6_CPE4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_SCLK6_CPE4_MASK) | (((uint32_t)val) << SCAN_SCLK6_CPE4_SHIFT))

//====================================================================
//Register: AFE Clocks 1 (ACLK1)
/** \brief This Register and the AFE Clocks2 and AFE Clocks3 Registers contain the 12 Edge values for MClk. The AFE Clocks 4 and 5 Registers contain the 4 Edge values for VSClk and CD1Clk. This clock is enabled by SCFG1 bit 27; polarity is set by SCFG1 bit 3. Note that the clock must have an even number of edges within a pixel period.<br/>*/
//====================================================================

#define  SCAN_ACLK1_MCE1_MASK 0xff000000
#define  SCAN_ACLK1_MCE1_SHIFT 24
#define  SCAN_ACLK1_MCE1_MASK_SHIFT(reg) (((reg) & SCAN_ACLK1_MCE1_MASK) >> SCAN_ACLK1_MCE1_SHIFT)
#define  SCAN_ACLK1_MCE1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK1_MCE1_MASK) | (((uint32_t)val) << SCAN_ACLK1_MCE1_SHIFT))

#define  SCAN_ACLK1_MCE2_MASK 0xff0000
#define  SCAN_ACLK1_MCE2_SHIFT 16
#define  SCAN_ACLK1_MCE2_MASK_SHIFT(reg) (((reg) & SCAN_ACLK1_MCE2_MASK) >> SCAN_ACLK1_MCE2_SHIFT)
#define  SCAN_ACLK1_MCE2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK1_MCE2_MASK) | (((uint32_t)val) << SCAN_ACLK1_MCE2_SHIFT))

#define  SCAN_ACLK1_MCE3_MASK 0xff00
#define  SCAN_ACLK1_MCE3_SHIFT 8
#define  SCAN_ACLK1_MCE3_MASK_SHIFT(reg) (((reg) & SCAN_ACLK1_MCE3_MASK) >> SCAN_ACLK1_MCE3_SHIFT)
#define  SCAN_ACLK1_MCE3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK1_MCE3_MASK) | (((uint32_t)val) << SCAN_ACLK1_MCE3_SHIFT))

#define  SCAN_ACLK1_MCE4_MASK 0xff
#define  SCAN_ACLK1_MCE4_SHIFT 0
#define  SCAN_ACLK1_MCE4_MASK_SHIFT(reg) (((reg) & SCAN_ACLK1_MCE4_MASK) >> SCAN_ACLK1_MCE4_SHIFT)
#define  SCAN_ACLK1_MCE4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK1_MCE4_MASK) | (((uint32_t)val) << SCAN_ACLK1_MCE4_SHIFT))

//====================================================================
//Register: AFE Clocks 2 (ACLK2)
/** \brief AFE Clocks 2 Register.*/
//====================================================================

#define  SCAN_ACLK2_MCE5_MASK 0xff000000
#define  SCAN_ACLK2_MCE5_SHIFT 24
#define  SCAN_ACLK2_MCE5_MASK_SHIFT(reg) (((reg) & SCAN_ACLK2_MCE5_MASK) >> SCAN_ACLK2_MCE5_SHIFT)
#define  SCAN_ACLK2_MCE5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK2_MCE5_MASK) | (((uint32_t)val) << SCAN_ACLK2_MCE5_SHIFT))

#define  SCAN_ACLK2_MCE6_MASK 0xff0000
#define  SCAN_ACLK2_MCE6_SHIFT 16
#define  SCAN_ACLK2_MCE6_MASK_SHIFT(reg) (((reg) & SCAN_ACLK2_MCE6_MASK) >> SCAN_ACLK2_MCE6_SHIFT)
#define  SCAN_ACLK2_MCE6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK2_MCE6_MASK) | (((uint32_t)val) << SCAN_ACLK2_MCE6_SHIFT))

#define  SCAN_ACLK2_MCE7_MASK 0xff00
#define  SCAN_ACLK2_MCE7_SHIFT 8
#define  SCAN_ACLK2_MCE7_MASK_SHIFT(reg) (((reg) & SCAN_ACLK2_MCE7_MASK) >> SCAN_ACLK2_MCE7_SHIFT)
#define  SCAN_ACLK2_MCE7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK2_MCE7_MASK) | (((uint32_t)val) << SCAN_ACLK2_MCE7_SHIFT))

#define  SCAN_ACLK2_MCE8_MASK 0xff
#define  SCAN_ACLK2_MCE8_SHIFT 0
#define  SCAN_ACLK2_MCE8_MASK_SHIFT(reg) (((reg) & SCAN_ACLK2_MCE8_MASK) >> SCAN_ACLK2_MCE8_SHIFT)
#define  SCAN_ACLK2_MCE8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK2_MCE8_MASK) | (((uint32_t)val) << SCAN_ACLK2_MCE8_SHIFT))

//====================================================================
//Register: AFE Clocks 3 (ACLK3)
/** \brief AFE Clocks 3 Register.*/
//====================================================================

#define  SCAN_ACLK3_MCE9_MASK 0xff000000
#define  SCAN_ACLK3_MCE9_SHIFT 24
#define  SCAN_ACLK3_MCE9_MASK_SHIFT(reg) (((reg) & SCAN_ACLK3_MCE9_MASK) >> SCAN_ACLK3_MCE9_SHIFT)
#define  SCAN_ACLK3_MCE9_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK3_MCE9_MASK) | (((uint32_t)val) << SCAN_ACLK3_MCE9_SHIFT))

#define  SCAN_ACLK3_MCE10_MASK 0xff0000
#define  SCAN_ACLK3_MCE10_SHIFT 16
#define  SCAN_ACLK3_MCE10_MASK_SHIFT(reg) (((reg) & SCAN_ACLK3_MCE10_MASK) >> SCAN_ACLK3_MCE10_SHIFT)
#define  SCAN_ACLK3_MCE10_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK3_MCE10_MASK) | (((uint32_t)val) << SCAN_ACLK3_MCE10_SHIFT))

#define  SCAN_ACLK3_MCE11_MASK 0xff00
#define  SCAN_ACLK3_MCE11_SHIFT 8
#define  SCAN_ACLK3_MCE11_MASK_SHIFT(reg) (((reg) & SCAN_ACLK3_MCE11_MASK) >> SCAN_ACLK3_MCE11_SHIFT)
#define  SCAN_ACLK3_MCE11_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK3_MCE11_MASK) | (((uint32_t)val) << SCAN_ACLK3_MCE11_SHIFT))

#define  SCAN_ACLK3_MCE12_MASK 0xff
#define  SCAN_ACLK3_MCE12_SHIFT 0
#define  SCAN_ACLK3_MCE12_MASK_SHIFT(reg) (((reg) & SCAN_ACLK3_MCE12_MASK) >> SCAN_ACLK3_MCE12_SHIFT)
#define  SCAN_ACLK3_MCE12_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK3_MCE12_MASK) | (((uint32_t)val) << SCAN_ACLK3_MCE12_SHIFT))

//====================================================================
//Register: AFE Clocks 6 (ACLK6)
/** \brief AFE Clocks 6 Register.*/
//====================================================================

#define  SCAN_ACLK6_MCE13_MASK 0xff000000
#define  SCAN_ACLK6_MCE13_SHIFT 24
#define  SCAN_ACLK6_MCE13_MASK_SHIFT(reg) (((reg) & SCAN_ACLK6_MCE13_MASK) >> SCAN_ACLK6_MCE13_SHIFT)
#define  SCAN_ACLK6_MCE13_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK6_MCE13_MASK) | (((uint32_t)val) << SCAN_ACLK6_MCE13_SHIFT))

#define  SCAN_ACLK6_MCE14_MASK 0xff0000
#define  SCAN_ACLK6_MCE14_SHIFT 16
#define  SCAN_ACLK6_MCE14_MASK_SHIFT(reg) (((reg) & SCAN_ACLK6_MCE14_MASK) >> SCAN_ACLK6_MCE14_SHIFT)
#define  SCAN_ACLK6_MCE14_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK6_MCE14_MASK) | (((uint32_t)val) << SCAN_ACLK6_MCE14_SHIFT))

#define  SCAN_ACLK6_MCE15_MASK 0xff00
#define  SCAN_ACLK6_MCE15_SHIFT 8
#define  SCAN_ACLK6_MCE15_MASK_SHIFT(reg) (((reg) & SCAN_ACLK6_MCE15_MASK) >> SCAN_ACLK6_MCE15_SHIFT)
#define  SCAN_ACLK6_MCE15_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK6_MCE15_MASK) | (((uint32_t)val) << SCAN_ACLK6_MCE15_SHIFT))

#define  SCAN_ACLK6_MCE16_MASK 0xff
#define  SCAN_ACLK6_MCE16_SHIFT 0
#define  SCAN_ACLK6_MCE16_MASK_SHIFT(reg) (((reg) & SCAN_ACLK6_MCE16_MASK) >> SCAN_ACLK6_MCE16_SHIFT)
#define  SCAN_ACLK6_MCE16_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK6_MCE16_MASK) | (((uint32_t)val) << SCAN_ACLK6_MCE16_SHIFT))

//====================================================================
//Register: AFE Clocks 4 (ACLK4)
/** \brief This Register contains the 4 Edge values for the AFE VSClk. This clock is enabled by SCFG1 bit 26; polarity is set by SCFG1 bit 2. Note that the clock must have an even number of edges within a pixel period.<br/>*/
//====================================================================

#define  SCAN_ACLK4_VSE1_MASK 0xff000000
#define  SCAN_ACLK4_VSE1_SHIFT 24
#define  SCAN_ACLK4_VSE1_MASK_SHIFT(reg) (((reg) & SCAN_ACLK4_VSE1_MASK) >> SCAN_ACLK4_VSE1_SHIFT)
#define  SCAN_ACLK4_VSE1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK4_VSE1_MASK) | (((uint32_t)val) << SCAN_ACLK4_VSE1_SHIFT))

#define  SCAN_ACLK4_VSE2_MASK 0xff0000
#define  SCAN_ACLK4_VSE2_SHIFT 16
#define  SCAN_ACLK4_VSE2_MASK_SHIFT(reg) (((reg) & SCAN_ACLK4_VSE2_MASK) >> SCAN_ACLK4_VSE2_SHIFT)
#define  SCAN_ACLK4_VSE2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK4_VSE2_MASK) | (((uint32_t)val) << SCAN_ACLK4_VSE2_SHIFT))

#define  SCAN_ACLK4_VSE3_MASK 0xff00
#define  SCAN_ACLK4_VSE3_SHIFT 8
#define  SCAN_ACLK4_VSE3_MASK_SHIFT(reg) (((reg) & SCAN_ACLK4_VSE3_MASK) >> SCAN_ACLK4_VSE3_SHIFT)
#define  SCAN_ACLK4_VSE3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK4_VSE3_MASK) | (((uint32_t)val) << SCAN_ACLK4_VSE3_SHIFT))

#define  SCAN_ACLK4_VSE4_MASK 0xff
#define  SCAN_ACLK4_VSE4_SHIFT 0
#define  SCAN_ACLK4_VSE4_MASK_SHIFT(reg) (((reg) & SCAN_ACLK4_VSE4_MASK) >> SCAN_ACLK4_VSE4_SHIFT)
#define  SCAN_ACLK4_VSE4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK4_VSE4_MASK) | (((uint32_t)val) << SCAN_ACLK4_VSE4_SHIFT))

//====================================================================
//Register: AFE Clocks 5 (ACLK5)
/** \brief This Register contains the 4 Edge values for the AFE CD1Clk. This clock is enabled by SCFG1 bit 25; polarity is set by SCFG1 bit 1. Note that the clock must have an even number of edges within a pixel period.<br/>*/
//====================================================================

#define  SCAN_ACLK5_CD1E1_MASK 0xff000000
#define  SCAN_ACLK5_CD1E1_SHIFT 24
#define  SCAN_ACLK5_CD1E1_MASK_SHIFT(reg) (((reg) & SCAN_ACLK5_CD1E1_MASK) >> SCAN_ACLK5_CD1E1_SHIFT)
#define  SCAN_ACLK5_CD1E1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK5_CD1E1_MASK) | (((uint32_t)val) << SCAN_ACLK5_CD1E1_SHIFT))

#define  SCAN_ACLK5_CD1E2_MASK 0xff0000
#define  SCAN_ACLK5_CD1E2_SHIFT 16
#define  SCAN_ACLK5_CD1E2_MASK_SHIFT(reg) (((reg) & SCAN_ACLK5_CD1E2_MASK) >> SCAN_ACLK5_CD1E2_SHIFT)
#define  SCAN_ACLK5_CD1E2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK5_CD1E2_MASK) | (((uint32_t)val) << SCAN_ACLK5_CD1E2_SHIFT))

#define  SCAN_ACLK5_CD1E3_MASK 0xff00
#define  SCAN_ACLK5_CD1E3_SHIFT 8
#define  SCAN_ACLK5_CD1E3_MASK_SHIFT(reg) (((reg) & SCAN_ACLK5_CD1E3_MASK) >> SCAN_ACLK5_CD1E3_SHIFT)
#define  SCAN_ACLK5_CD1E3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK5_CD1E3_MASK) | (((uint32_t)val) << SCAN_ACLK5_CD1E3_SHIFT))

#define  SCAN_ACLK5_CD1E4_MASK 0xff
#define  SCAN_ACLK5_CD1E4_SHIFT 0
#define  SCAN_ACLK5_CD1E4_MASK_SHIFT(reg) (((reg) & SCAN_ACLK5_CD1E4_MASK) >> SCAN_ACLK5_CD1E4_SHIFT)
#define  SCAN_ACLK5_CD1E4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_ACLK5_CD1E4_MASK) | (((uint32_t)val) << SCAN_ACLK5_CD1E4_SHIFT))

//====================================================================
//Register: AFE Load Data 1 (Instance 1 of 2) (LDATA10)
/** \brief The AFE Load Data 1-6 Registers contain 12 8-bit Data Samples, DS1-DS12 and corresponding 5-bit Data Code values, DC1-DC12. The Data Sample and Data Code values are logically combined into "Load Data" values, LDi, as follows:<br/>
<br/>
LDi = (DSi, DCi) where <br/>
DSi = Clk Number of Data Sample (8-bits)<br/>
DCi = (Sequence Complete, Load Complete, Load Code) (1-bit, 1-bit, 3-bit)<br/>
Therefore,<br/>
<br/>
LD1 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
LD2 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
LD3 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
...<br/>
LD12 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
<br/>
where Load Code is<br/>
000 clear all 16 bits (can be used to clear unused bits)<br/>
001 load 8 MSbits (AB)<br/>
010 load 8 LSbits (CD)<br/>
011 load all 16-bits (ABCD) (not implemented)<br/>
100 load 4 MSbits (A)<br/>
101 load next 4 bits (B)<br/>
110 load next 4 bits (C)<br/>
111 load 4 LSbits (D)<br/><br/>
Notes:<br/>
<br/>
1. The sequence complete and load complete codes must be set on the highest numbered LDi used to complete the load sequence. For example, if 2 data nibbles or bytes are received from the AFE, the load/sequence complete must be set for LD2; or LD4 for 4 nibbles; or LD8 for 8 nibbles; etc.<br/>
2. AFE data should be loaded in the order received from the AFE; e.g., ABCD nibbles.<br/>
3. The Clock n sequence need not be in ascending order, although that case is shown in the Examples below. For example, if the pixel period is 16 clocks with 4 AFE nibbles, LD1 (A) could be at Clock 16, LD2 (B) at Clock 4, LD3 (C) at Clock 8, LD4 (D) at Clock 12. This may be the case when AFE data is delayed such that the LDi's span 2 pixel periods.<br/>
<br/>
Examples:<br/>
For a Mono CCD or Color CIS scan with an 8-bit AFE that outputs a hi-byte:lo-byte sequence for every pixel, an example set of LDi values are<br/>
<br/>
LD1 = (2,0,0,001) Load 8-bit data at Clock 2 to AB<br/>
LD2 = (6,1,1,010) Load 8-bit data at Clock 6 to CD; Sequence complete and Load Complete.<br/>
LD3-LD12 = (255,0,0,000) Nop<br/>
<br/>
For the same scan with a 4-bit AFE:<br/>
<br/>
LD1 = (4,0,0,100) Load 4-bit data at Clock 4 to A<br/>
LD2 = (7,0,0,101) Load 4-bit data at Clock 7 to B<br/>
LD3 = (9,0,0,110) Load 4-bit data at Clock 9 to C<br/>
LD4 = (12,1,1,111) Load 4-bit data at Clock 12 to D; Sequence Complete, Load Complete.<br/>
LD5-LD12 = (255,0,0,000) Nop<br/>
<br/>
For a Color CCD scan with an 8-bit AFE that outputs 3 hi-byte:lo-byte data values for every pixel:<br/>
<br/>
LD1 = (2,0,0,001) Load 8-bit data at Clock 2 to AB<br/>
LD2 = (6,0,1,010) Load 8-bit data at Clock 6 to CD; Load Complete.<br/>
LD3 = (9,0,0,001) Load 8-bit data at Clock 9 to AB <br/>
LD4 = (13,0,1,010) Load 8-bit data at Clock 13 to CD; Load Complete.<br/>
LD5 = (17,0,0,001) Load 8-bit data at Clock 17 to AB <br/>
LD6 = (21,1,1,010) Load 8-bit data at Clock 21 to CD; Sequence Complete, Load Complete.<br/>
LD7-LD12 = (255,0,0,000) Nop<br/>
<br/>
A CCD Color Scan with a 4-bit AFE uses all 12 LD values.<br/>
<br/>
AFE Load Data 1 Register.<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LDATA10_DS1_MASK 0xff000000
#define  SCAN_LDATA10_DS1_SHIFT 24
#define  SCAN_LDATA10_DS1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA10_DS1_MASK) >> SCAN_LDATA10_DS1_SHIFT)
#define  SCAN_LDATA10_DS1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA10_DS1_MASK) | (((uint32_t)val) << SCAN_LDATA10_DS1_SHIFT))

#define  SCAN_LDATA10_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA10_RESERVED1_SHIFT 21
#define  SCAN_LDATA10_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA10_RESERVED1_MASK) >> SCAN_LDATA10_RESERVED1_SHIFT)
#define  SCAN_LDATA10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA10_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA10_RESERVED1_SHIFT))

#define  SCAN_LDATA10_DC1_MASK 0x1f0000
#define  SCAN_LDATA10_DC1_SHIFT 16
#define  SCAN_LDATA10_DC1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA10_DC1_MASK) >> SCAN_LDATA10_DC1_SHIFT)
#define  SCAN_LDATA10_DC1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA10_DC1_MASK) | (((uint32_t)val) << SCAN_LDATA10_DC1_SHIFT))

#define  SCAN_LDATA10_DS2_MASK 0xff00
#define  SCAN_LDATA10_DS2_SHIFT 8
#define  SCAN_LDATA10_DS2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA10_DS2_MASK) >> SCAN_LDATA10_DS2_SHIFT)
#define  SCAN_LDATA10_DS2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA10_DS2_MASK) | (((uint32_t)val) << SCAN_LDATA10_DS2_SHIFT))

#define  SCAN_LDATA10_RESERVED2_MASK 0xe0
#define  SCAN_LDATA10_RESERVED2_SHIFT 5
#define  SCAN_LDATA10_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA10_RESERVED2_MASK) >> SCAN_LDATA10_RESERVED2_SHIFT)
#define  SCAN_LDATA10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA10_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA10_RESERVED2_SHIFT))

#define  SCAN_LDATA10_DC2_MASK 0x1f
#define  SCAN_LDATA10_DC2_SHIFT 0
#define  SCAN_LDATA10_DC2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA10_DC2_MASK) >> SCAN_LDATA10_DC2_SHIFT)
#define  SCAN_LDATA10_DC2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA10_DC2_MASK) | (((uint32_t)val) << SCAN_LDATA10_DC2_SHIFT))

//====================================================================
//Register: AFE Load Data 1 (Instance 2 of 2) (LDATA11)
/** \brief The AFE Load Data 1-6 Registers contain 12 8-bit Data Samples, DS1-DS12 and corresponding 5-bit Data Code values, DC1-DC12. The Data Sample and Data Code values are logically combined into "Load Data" values, LDi, as follows:<br/>
<br/>
LDi = (DSi, DCi) where <br/>
DSi = Clk Number of Data Sample (8-bits)<br/>
DCi = (Sequence Complete, Load Complete, Load Code) (1-bit, 1-bit, 3-bit)<br/>
Therefore,<br/>
<br/>
LD1 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
LD2 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
LD3 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
...<br/>
LD12 = (Clk Number, Sequence Complete, Load Complete, Load Code)<br/>
<br/>
where Load Code is<br/>
000 clear all 16 bits (can be used to clear unused bits)<br/>
001 load 8 MSbits (AB)<br/>
010 load 8 LSbits (CD)<br/>
011 load all 16-bits (ABCD) (not implemented)<br/>
100 load 4 MSbits (A)<br/>
101 load next 4 bits (B)<br/>
110 load next 4 bits (C)<br/>
111 load 4 LSbits (D)<br/><br/>
Notes:<br/>
<br/>
1. The sequence complete and load complete codes must be set on the highest numbered LDi used to complete the load sequence. For example, if 2 data nibbles or bytes are received from the AFE, the load/sequence complete must be set for LD2; or LD4 for 4 nibbles; or LD8 for 8 nibbles; etc.<br/>
2. AFE data should be loaded in the order received from the AFE; e.g., ABCD nibbles.<br/>
3. The Clock n sequence need not be in ascending order, although that case is shown in the Examples below. For example, if the pixel period is 16 clocks with 4 AFE nibbles, LD1 (A) could be at Clock 16, LD2 (B) at Clock 4, LD3 (C) at Clock 8, LD4 (D) at Clock 12. This may be the case when AFE data is delayed such that the LDi's span 2 pixel periods.<br/>
<br/>
Examples:<br/>
For a Mono CCD or Color CIS scan with an 8-bit AFE that outputs a hi-byte:lo-byte sequence for every pixel, an example set of LDi values are<br/>
<br/>
LD1 = (2,0,0,001) Load 8-bit data at Clock 2 to AB<br/>
LD2 = (6,1,1,010) Load 8-bit data at Clock 6 to CD; Sequence complete and Load Complete.<br/>
LD3-LD12 = (255,0,0,000) Nop<br/>
<br/>
For the same scan with a 4-bit AFE:<br/>
<br/>
LD1 = (4,0,0,100) Load 4-bit data at Clock 4 to A<br/>
LD2 = (7,0,0,101) Load 4-bit data at Clock 7 to B<br/>
LD3 = (9,0,0,110) Load 4-bit data at Clock 9 to C<br/>
LD4 = (12,1,1,111) Load 4-bit data at Clock 12 to D; Sequence Complete, Load Complete.<br/>
LD5-LD12 = (255,0,0,000) Nop<br/>
<br/>
For a Color CCD scan with an 8-bit AFE that outputs 3 hi-byte:lo-byte data values for every pixel:<br/>
<br/>
LD1 = (2,0,0,001) Load 8-bit data at Clock 2 to AB<br/>
LD2 = (6,0,1,010) Load 8-bit data at Clock 6 to CD; Load Complete.<br/>
LD3 = (9,0,0,001) Load 8-bit data at Clock 9 to AB <br/>
LD4 = (13,0,1,010) Load 8-bit data at Clock 13 to CD; Load Complete.<br/>
LD5 = (17,0,0,001) Load 8-bit data at Clock 17 to AB <br/>
LD6 = (21,1,1,010) Load 8-bit data at Clock 21 to CD; Sequence Complete, Load Complete.<br/>
LD7-LD12 = (255,0,0,000) Nop<br/>
<br/>
A CCD Color Scan with a 4-bit AFE uses all 12 LD values.<br/>
<br/>
AFE Load Data 1 Register.<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LDATA11_DS1_MASK 0xff000000
#define  SCAN_LDATA11_DS1_SHIFT 24
#define  SCAN_LDATA11_DS1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA11_DS1_MASK) >> SCAN_LDATA11_DS1_SHIFT)
#define  SCAN_LDATA11_DS1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA11_DS1_MASK) | (((uint32_t)val) << SCAN_LDATA11_DS1_SHIFT))

#define  SCAN_LDATA11_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA11_RESERVED1_SHIFT 21
#define  SCAN_LDATA11_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA11_RESERVED1_MASK) >> SCAN_LDATA11_RESERVED1_SHIFT)
#define  SCAN_LDATA11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA11_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA11_RESERVED1_SHIFT))

#define  SCAN_LDATA11_DC1_MASK 0x1f0000
#define  SCAN_LDATA11_DC1_SHIFT 16
#define  SCAN_LDATA11_DC1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA11_DC1_MASK) >> SCAN_LDATA11_DC1_SHIFT)
#define  SCAN_LDATA11_DC1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA11_DC1_MASK) | (((uint32_t)val) << SCAN_LDATA11_DC1_SHIFT))

#define  SCAN_LDATA11_DS2_MASK 0xff00
#define  SCAN_LDATA11_DS2_SHIFT 8
#define  SCAN_LDATA11_DS2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA11_DS2_MASK) >> SCAN_LDATA11_DS2_SHIFT)
#define  SCAN_LDATA11_DS2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA11_DS2_MASK) | (((uint32_t)val) << SCAN_LDATA11_DS2_SHIFT))

#define  SCAN_LDATA11_RESERVED2_MASK 0xe0
#define  SCAN_LDATA11_RESERVED2_SHIFT 5
#define  SCAN_LDATA11_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA11_RESERVED2_MASK) >> SCAN_LDATA11_RESERVED2_SHIFT)
#define  SCAN_LDATA11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA11_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA11_RESERVED2_SHIFT))

#define  SCAN_LDATA11_DC2_MASK 0x1f
#define  SCAN_LDATA11_DC2_SHIFT 0
#define  SCAN_LDATA11_DC2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA11_DC2_MASK) >> SCAN_LDATA11_DC2_SHIFT)
#define  SCAN_LDATA11_DC2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA11_DC2_MASK) | (((uint32_t)val) << SCAN_LDATA11_DC2_SHIFT))

//====================================================================
//Register: AFE Load Data 2 (Instance 1 of 2) (LDATA20)
/** \brief AFE Load Data 2 Register. (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LDATA20_DS3_MASK 0xff000000
#define  SCAN_LDATA20_DS3_SHIFT 24
#define  SCAN_LDATA20_DS3_MASK_SHIFT(reg) (((reg) & SCAN_LDATA20_DS3_MASK) >> SCAN_LDATA20_DS3_SHIFT)
#define  SCAN_LDATA20_DS3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA20_DS3_MASK) | (((uint32_t)val) << SCAN_LDATA20_DS3_SHIFT))

#define  SCAN_LDATA20_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA20_RESERVED1_SHIFT 21
#define  SCAN_LDATA20_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA20_RESERVED1_MASK) >> SCAN_LDATA20_RESERVED1_SHIFT)
#define  SCAN_LDATA20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA20_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA20_RESERVED1_SHIFT))

#define  SCAN_LDATA20_DC3_MASK 0x1f0000
#define  SCAN_LDATA20_DC3_SHIFT 16
#define  SCAN_LDATA20_DC3_MASK_SHIFT(reg) (((reg) & SCAN_LDATA20_DC3_MASK) >> SCAN_LDATA20_DC3_SHIFT)
#define  SCAN_LDATA20_DC3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA20_DC3_MASK) | (((uint32_t)val) << SCAN_LDATA20_DC3_SHIFT))

#define  SCAN_LDATA20_DS4_MASK 0xff00
#define  SCAN_LDATA20_DS4_SHIFT 8
#define  SCAN_LDATA20_DS4_MASK_SHIFT(reg) (((reg) & SCAN_LDATA20_DS4_MASK) >> SCAN_LDATA20_DS4_SHIFT)
#define  SCAN_LDATA20_DS4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA20_DS4_MASK) | (((uint32_t)val) << SCAN_LDATA20_DS4_SHIFT))

#define  SCAN_LDATA20_RESERVED2_MASK 0xe0
#define  SCAN_LDATA20_RESERVED2_SHIFT 5
#define  SCAN_LDATA20_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA20_RESERVED2_MASK) >> SCAN_LDATA20_RESERVED2_SHIFT)
#define  SCAN_LDATA20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA20_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA20_RESERVED2_SHIFT))

#define  SCAN_LDATA20_DC4_MASK 0x1f
#define  SCAN_LDATA20_DC4_SHIFT 0
#define  SCAN_LDATA20_DC4_MASK_SHIFT(reg) (((reg) & SCAN_LDATA20_DC4_MASK) >> SCAN_LDATA20_DC4_SHIFT)
#define  SCAN_LDATA20_DC4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA20_DC4_MASK) | (((uint32_t)val) << SCAN_LDATA20_DC4_SHIFT))

//====================================================================
//Register: AFE Load Data 2 (Instance 2 of 2) (LDATA21)
/** \brief AFE Load Data 2 Register. (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LDATA21_DS3_MASK 0xff000000
#define  SCAN_LDATA21_DS3_SHIFT 24
#define  SCAN_LDATA21_DS3_MASK_SHIFT(reg) (((reg) & SCAN_LDATA21_DS3_MASK) >> SCAN_LDATA21_DS3_SHIFT)
#define  SCAN_LDATA21_DS3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA21_DS3_MASK) | (((uint32_t)val) << SCAN_LDATA21_DS3_SHIFT))

#define  SCAN_LDATA21_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA21_RESERVED1_SHIFT 21
#define  SCAN_LDATA21_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA21_RESERVED1_MASK) >> SCAN_LDATA21_RESERVED1_SHIFT)
#define  SCAN_LDATA21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA21_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA21_RESERVED1_SHIFT))

#define  SCAN_LDATA21_DC3_MASK 0x1f0000
#define  SCAN_LDATA21_DC3_SHIFT 16
#define  SCAN_LDATA21_DC3_MASK_SHIFT(reg) (((reg) & SCAN_LDATA21_DC3_MASK) >> SCAN_LDATA21_DC3_SHIFT)
#define  SCAN_LDATA21_DC3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA21_DC3_MASK) | (((uint32_t)val) << SCAN_LDATA21_DC3_SHIFT))

#define  SCAN_LDATA21_DS4_MASK 0xff00
#define  SCAN_LDATA21_DS4_SHIFT 8
#define  SCAN_LDATA21_DS4_MASK_SHIFT(reg) (((reg) & SCAN_LDATA21_DS4_MASK) >> SCAN_LDATA21_DS4_SHIFT)
#define  SCAN_LDATA21_DS4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA21_DS4_MASK) | (((uint32_t)val) << SCAN_LDATA21_DS4_SHIFT))

#define  SCAN_LDATA21_RESERVED2_MASK 0xe0
#define  SCAN_LDATA21_RESERVED2_SHIFT 5
#define  SCAN_LDATA21_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA21_RESERVED2_MASK) >> SCAN_LDATA21_RESERVED2_SHIFT)
#define  SCAN_LDATA21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA21_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA21_RESERVED2_SHIFT))

#define  SCAN_LDATA21_DC4_MASK 0x1f
#define  SCAN_LDATA21_DC4_SHIFT 0
#define  SCAN_LDATA21_DC4_MASK_SHIFT(reg) (((reg) & SCAN_LDATA21_DC4_MASK) >> SCAN_LDATA21_DC4_SHIFT)
#define  SCAN_LDATA21_DC4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA21_DC4_MASK) | (((uint32_t)val) << SCAN_LDATA21_DC4_SHIFT))

//====================================================================
//Register: AFE Load Data 3 (Instance 1 of 2) (LDATA30)
/** \brief AFE Load Data 3 Register. (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LDATA30_DS5_MASK 0xff000000
#define  SCAN_LDATA30_DS5_SHIFT 24
#define  SCAN_LDATA30_DS5_MASK_SHIFT(reg) (((reg) & SCAN_LDATA30_DS5_MASK) >> SCAN_LDATA30_DS5_SHIFT)
#define  SCAN_LDATA30_DS5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA30_DS5_MASK) | (((uint32_t)val) << SCAN_LDATA30_DS5_SHIFT))

#define  SCAN_LDATA30_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA30_RESERVED1_SHIFT 21
#define  SCAN_LDATA30_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA30_RESERVED1_MASK) >> SCAN_LDATA30_RESERVED1_SHIFT)
#define  SCAN_LDATA30_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA30_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA30_RESERVED1_SHIFT))

#define  SCAN_LDATA30_DC5_MASK 0x1f0000
#define  SCAN_LDATA30_DC5_SHIFT 16
#define  SCAN_LDATA30_DC5_MASK_SHIFT(reg) (((reg) & SCAN_LDATA30_DC5_MASK) >> SCAN_LDATA30_DC5_SHIFT)
#define  SCAN_LDATA30_DC5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA30_DC5_MASK) | (((uint32_t)val) << SCAN_LDATA30_DC5_SHIFT))

#define  SCAN_LDATA30_DS6_MASK 0xff00
#define  SCAN_LDATA30_DS6_SHIFT 8
#define  SCAN_LDATA30_DS6_MASK_SHIFT(reg) (((reg) & SCAN_LDATA30_DS6_MASK) >> SCAN_LDATA30_DS6_SHIFT)
#define  SCAN_LDATA30_DS6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA30_DS6_MASK) | (((uint32_t)val) << SCAN_LDATA30_DS6_SHIFT))

#define  SCAN_LDATA30_RESERVED2_MASK 0xe0
#define  SCAN_LDATA30_RESERVED2_SHIFT 5
#define  SCAN_LDATA30_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA30_RESERVED2_MASK) >> SCAN_LDATA30_RESERVED2_SHIFT)
#define  SCAN_LDATA30_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA30_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA30_RESERVED2_SHIFT))

#define  SCAN_LDATA30_DC6_MASK 0x1f
#define  SCAN_LDATA30_DC6_SHIFT 0
#define  SCAN_LDATA30_DC6_MASK_SHIFT(reg) (((reg) & SCAN_LDATA30_DC6_MASK) >> SCAN_LDATA30_DC6_SHIFT)
#define  SCAN_LDATA30_DC6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA30_DC6_MASK) | (((uint32_t)val) << SCAN_LDATA30_DC6_SHIFT))

//====================================================================
//Register: AFE Load Data 3 (Instance 2 of 2) (LDATA31)
/** \brief AFE Load Data 3 Register. (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LDATA31_DS5_MASK 0xff000000
#define  SCAN_LDATA31_DS5_SHIFT 24
#define  SCAN_LDATA31_DS5_MASK_SHIFT(reg) (((reg) & SCAN_LDATA31_DS5_MASK) >> SCAN_LDATA31_DS5_SHIFT)
#define  SCAN_LDATA31_DS5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA31_DS5_MASK) | (((uint32_t)val) << SCAN_LDATA31_DS5_SHIFT))

#define  SCAN_LDATA31_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA31_RESERVED1_SHIFT 21
#define  SCAN_LDATA31_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA31_RESERVED1_MASK) >> SCAN_LDATA31_RESERVED1_SHIFT)
#define  SCAN_LDATA31_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA31_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA31_RESERVED1_SHIFT))

#define  SCAN_LDATA31_DC5_MASK 0x1f0000
#define  SCAN_LDATA31_DC5_SHIFT 16
#define  SCAN_LDATA31_DC5_MASK_SHIFT(reg) (((reg) & SCAN_LDATA31_DC5_MASK) >> SCAN_LDATA31_DC5_SHIFT)
#define  SCAN_LDATA31_DC5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA31_DC5_MASK) | (((uint32_t)val) << SCAN_LDATA31_DC5_SHIFT))

#define  SCAN_LDATA31_DS6_MASK 0xff00
#define  SCAN_LDATA31_DS6_SHIFT 8
#define  SCAN_LDATA31_DS6_MASK_SHIFT(reg) (((reg) & SCAN_LDATA31_DS6_MASK) >> SCAN_LDATA31_DS6_SHIFT)
#define  SCAN_LDATA31_DS6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA31_DS6_MASK) | (((uint32_t)val) << SCAN_LDATA31_DS6_SHIFT))

#define  SCAN_LDATA31_RESERVED2_MASK 0xe0
#define  SCAN_LDATA31_RESERVED2_SHIFT 5
#define  SCAN_LDATA31_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA31_RESERVED2_MASK) >> SCAN_LDATA31_RESERVED2_SHIFT)
#define  SCAN_LDATA31_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA31_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA31_RESERVED2_SHIFT))

#define  SCAN_LDATA31_DC6_MASK 0x1f
#define  SCAN_LDATA31_DC6_SHIFT 0
#define  SCAN_LDATA31_DC6_MASK_SHIFT(reg) (((reg) & SCAN_LDATA31_DC6_MASK) >> SCAN_LDATA31_DC6_SHIFT)
#define  SCAN_LDATA31_DC6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA31_DC6_MASK) | (((uint32_t)val) << SCAN_LDATA31_DC6_SHIFT))

//====================================================================
//Register: AFE Load Data 4 (Instance 1 of 2) (LDATA40)
/** \brief AFE Load Data 4 Register. (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LDATA40_DS7_MASK 0xff000000
#define  SCAN_LDATA40_DS7_SHIFT 24
#define  SCAN_LDATA40_DS7_MASK_SHIFT(reg) (((reg) & SCAN_LDATA40_DS7_MASK) >> SCAN_LDATA40_DS7_SHIFT)
#define  SCAN_LDATA40_DS7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA40_DS7_MASK) | (((uint32_t)val) << SCAN_LDATA40_DS7_SHIFT))

#define  SCAN_LDATA40_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA40_RESERVED1_SHIFT 21
#define  SCAN_LDATA40_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA40_RESERVED1_MASK) >> SCAN_LDATA40_RESERVED1_SHIFT)
#define  SCAN_LDATA40_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA40_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA40_RESERVED1_SHIFT))

#define  SCAN_LDATA40_DC7_MASK 0x1f0000
#define  SCAN_LDATA40_DC7_SHIFT 16
#define  SCAN_LDATA40_DC7_MASK_SHIFT(reg) (((reg) & SCAN_LDATA40_DC7_MASK) >> SCAN_LDATA40_DC7_SHIFT)
#define  SCAN_LDATA40_DC7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA40_DC7_MASK) | (((uint32_t)val) << SCAN_LDATA40_DC7_SHIFT))

#define  SCAN_LDATA40_DS8_MASK 0xff00
#define  SCAN_LDATA40_DS8_SHIFT 8
#define  SCAN_LDATA40_DS8_MASK_SHIFT(reg) (((reg) & SCAN_LDATA40_DS8_MASK) >> SCAN_LDATA40_DS8_SHIFT)
#define  SCAN_LDATA40_DS8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA40_DS8_MASK) | (((uint32_t)val) << SCAN_LDATA40_DS8_SHIFT))

#define  SCAN_LDATA40_RESERVED2_MASK 0xe0
#define  SCAN_LDATA40_RESERVED2_SHIFT 5
#define  SCAN_LDATA40_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA40_RESERVED2_MASK) >> SCAN_LDATA40_RESERVED2_SHIFT)
#define  SCAN_LDATA40_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA40_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA40_RESERVED2_SHIFT))

#define  SCAN_LDATA40_DC8_MASK 0x1f
#define  SCAN_LDATA40_DC8_SHIFT 0
#define  SCAN_LDATA40_DC8_MASK_SHIFT(reg) (((reg) & SCAN_LDATA40_DC8_MASK) >> SCAN_LDATA40_DC8_SHIFT)
#define  SCAN_LDATA40_DC8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA40_DC8_MASK) | (((uint32_t)val) << SCAN_LDATA40_DC8_SHIFT))

//====================================================================
//Register: AFE Load Data 4 (Instance 2 of 2) (LDATA41)
/** \brief AFE Load Data 4 Register. (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LDATA41_DS7_MASK 0xff000000
#define  SCAN_LDATA41_DS7_SHIFT 24
#define  SCAN_LDATA41_DS7_MASK_SHIFT(reg) (((reg) & SCAN_LDATA41_DS7_MASK) >> SCAN_LDATA41_DS7_SHIFT)
#define  SCAN_LDATA41_DS7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA41_DS7_MASK) | (((uint32_t)val) << SCAN_LDATA41_DS7_SHIFT))

#define  SCAN_LDATA41_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA41_RESERVED1_SHIFT 21
#define  SCAN_LDATA41_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA41_RESERVED1_MASK) >> SCAN_LDATA41_RESERVED1_SHIFT)
#define  SCAN_LDATA41_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA41_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA41_RESERVED1_SHIFT))

#define  SCAN_LDATA41_DC7_MASK 0x1f0000
#define  SCAN_LDATA41_DC7_SHIFT 16
#define  SCAN_LDATA41_DC7_MASK_SHIFT(reg) (((reg) & SCAN_LDATA41_DC7_MASK) >> SCAN_LDATA41_DC7_SHIFT)
#define  SCAN_LDATA41_DC7_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA41_DC7_MASK) | (((uint32_t)val) << SCAN_LDATA41_DC7_SHIFT))

#define  SCAN_LDATA41_DS8_MASK 0xff00
#define  SCAN_LDATA41_DS8_SHIFT 8
#define  SCAN_LDATA41_DS8_MASK_SHIFT(reg) (((reg) & SCAN_LDATA41_DS8_MASK) >> SCAN_LDATA41_DS8_SHIFT)
#define  SCAN_LDATA41_DS8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA41_DS8_MASK) | (((uint32_t)val) << SCAN_LDATA41_DS8_SHIFT))

#define  SCAN_LDATA41_RESERVED2_MASK 0xe0
#define  SCAN_LDATA41_RESERVED2_SHIFT 5
#define  SCAN_LDATA41_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA41_RESERVED2_MASK) >> SCAN_LDATA41_RESERVED2_SHIFT)
#define  SCAN_LDATA41_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA41_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA41_RESERVED2_SHIFT))

#define  SCAN_LDATA41_DC8_MASK 0x1f
#define  SCAN_LDATA41_DC8_SHIFT 0
#define  SCAN_LDATA41_DC8_MASK_SHIFT(reg) (((reg) & SCAN_LDATA41_DC8_MASK) >> SCAN_LDATA41_DC8_SHIFT)
#define  SCAN_LDATA41_DC8_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA41_DC8_MASK) | (((uint32_t)val) << SCAN_LDATA41_DC8_SHIFT))

//====================================================================
//Register: AFE Load Data 5 (Instance 1 of 2) (LDATA50)
/** \brief AFE Load Data 5 Register. (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LDATA50_DS9_MASK 0xff000000
#define  SCAN_LDATA50_DS9_SHIFT 24
#define  SCAN_LDATA50_DS9_MASK_SHIFT(reg) (((reg) & SCAN_LDATA50_DS9_MASK) >> SCAN_LDATA50_DS9_SHIFT)
#define  SCAN_LDATA50_DS9_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA50_DS9_MASK) | (((uint32_t)val) << SCAN_LDATA50_DS9_SHIFT))

#define  SCAN_LDATA50_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA50_RESERVED1_SHIFT 21
#define  SCAN_LDATA50_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA50_RESERVED1_MASK) >> SCAN_LDATA50_RESERVED1_SHIFT)
#define  SCAN_LDATA50_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA50_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA50_RESERVED1_SHIFT))

#define  SCAN_LDATA50_DC9_MASK 0x1f0000
#define  SCAN_LDATA50_DC9_SHIFT 16
#define  SCAN_LDATA50_DC9_MASK_SHIFT(reg) (((reg) & SCAN_LDATA50_DC9_MASK) >> SCAN_LDATA50_DC9_SHIFT)
#define  SCAN_LDATA50_DC9_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA50_DC9_MASK) | (((uint32_t)val) << SCAN_LDATA50_DC9_SHIFT))

#define  SCAN_LDATA50_DS10_MASK 0xff00
#define  SCAN_LDATA50_DS10_SHIFT 8
#define  SCAN_LDATA50_DS10_MASK_SHIFT(reg) (((reg) & SCAN_LDATA50_DS10_MASK) >> SCAN_LDATA50_DS10_SHIFT)
#define  SCAN_LDATA50_DS10_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA50_DS10_MASK) | (((uint32_t)val) << SCAN_LDATA50_DS10_SHIFT))

#define  SCAN_LDATA50_RESERVED2_MASK 0xe0
#define  SCAN_LDATA50_RESERVED2_SHIFT 5
#define  SCAN_LDATA50_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA50_RESERVED2_MASK) >> SCAN_LDATA50_RESERVED2_SHIFT)
#define  SCAN_LDATA50_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA50_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA50_RESERVED2_SHIFT))

#define  SCAN_LDATA50_DC10_MASK 0x1f
#define  SCAN_LDATA50_DC10_SHIFT 0
#define  SCAN_LDATA50_DC10_MASK_SHIFT(reg) (((reg) & SCAN_LDATA50_DC10_MASK) >> SCAN_LDATA50_DC10_SHIFT)
#define  SCAN_LDATA50_DC10_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA50_DC10_MASK) | (((uint32_t)val) << SCAN_LDATA50_DC10_SHIFT))

//====================================================================
//Register: AFE Load Data 5 (Instance 2 of 2) (LDATA51)
/** \brief AFE Load Data 5 Register. (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LDATA51_DS9_MASK 0xff000000
#define  SCAN_LDATA51_DS9_SHIFT 24
#define  SCAN_LDATA51_DS9_MASK_SHIFT(reg) (((reg) & SCAN_LDATA51_DS9_MASK) >> SCAN_LDATA51_DS9_SHIFT)
#define  SCAN_LDATA51_DS9_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA51_DS9_MASK) | (((uint32_t)val) << SCAN_LDATA51_DS9_SHIFT))

#define  SCAN_LDATA51_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA51_RESERVED1_SHIFT 21
#define  SCAN_LDATA51_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA51_RESERVED1_MASK) >> SCAN_LDATA51_RESERVED1_SHIFT)
#define  SCAN_LDATA51_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA51_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA51_RESERVED1_SHIFT))

#define  SCAN_LDATA51_DC9_MASK 0x1f0000
#define  SCAN_LDATA51_DC9_SHIFT 16
#define  SCAN_LDATA51_DC9_MASK_SHIFT(reg) (((reg) & SCAN_LDATA51_DC9_MASK) >> SCAN_LDATA51_DC9_SHIFT)
#define  SCAN_LDATA51_DC9_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA51_DC9_MASK) | (((uint32_t)val) << SCAN_LDATA51_DC9_SHIFT))

#define  SCAN_LDATA51_DS10_MASK 0xff00
#define  SCAN_LDATA51_DS10_SHIFT 8
#define  SCAN_LDATA51_DS10_MASK_SHIFT(reg) (((reg) & SCAN_LDATA51_DS10_MASK) >> SCAN_LDATA51_DS10_SHIFT)
#define  SCAN_LDATA51_DS10_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA51_DS10_MASK) | (((uint32_t)val) << SCAN_LDATA51_DS10_SHIFT))

#define  SCAN_LDATA51_RESERVED2_MASK 0xe0
#define  SCAN_LDATA51_RESERVED2_SHIFT 5
#define  SCAN_LDATA51_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA51_RESERVED2_MASK) >> SCAN_LDATA51_RESERVED2_SHIFT)
#define  SCAN_LDATA51_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA51_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA51_RESERVED2_SHIFT))

#define  SCAN_LDATA51_DC10_MASK 0x1f
#define  SCAN_LDATA51_DC10_SHIFT 0
#define  SCAN_LDATA51_DC10_MASK_SHIFT(reg) (((reg) & SCAN_LDATA51_DC10_MASK) >> SCAN_LDATA51_DC10_SHIFT)
#define  SCAN_LDATA51_DC10_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA51_DC10_MASK) | (((uint32_t)val) << SCAN_LDATA51_DC10_SHIFT))

//====================================================================
//Register: AFE Load Data 6 (Instance 1 of 2) (LDATA60)
/** \brief AFE Load Data 6 Register. (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LDATA60_DS11_MASK 0xff000000
#define  SCAN_LDATA60_DS11_SHIFT 24
#define  SCAN_LDATA60_DS11_MASK_SHIFT(reg) (((reg) & SCAN_LDATA60_DS11_MASK) >> SCAN_LDATA60_DS11_SHIFT)
#define  SCAN_LDATA60_DS11_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA60_DS11_MASK) | (((uint32_t)val) << SCAN_LDATA60_DS11_SHIFT))

#define  SCAN_LDATA60_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA60_RESERVED1_SHIFT 21
#define  SCAN_LDATA60_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA60_RESERVED1_MASK) >> SCAN_LDATA60_RESERVED1_SHIFT)
#define  SCAN_LDATA60_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA60_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA60_RESERVED1_SHIFT))

#define  SCAN_LDATA60_DC11_MASK 0x1f0000
#define  SCAN_LDATA60_DC11_SHIFT 16
#define  SCAN_LDATA60_DC11_MASK_SHIFT(reg) (((reg) & SCAN_LDATA60_DC11_MASK) >> SCAN_LDATA60_DC11_SHIFT)
#define  SCAN_LDATA60_DC11_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA60_DC11_MASK) | (((uint32_t)val) << SCAN_LDATA60_DC11_SHIFT))

#define  SCAN_LDATA60_DS12_MASK 0xff00
#define  SCAN_LDATA60_DS12_SHIFT 8
#define  SCAN_LDATA60_DS12_MASK_SHIFT(reg) (((reg) & SCAN_LDATA60_DS12_MASK) >> SCAN_LDATA60_DS12_SHIFT)
#define  SCAN_LDATA60_DS12_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA60_DS12_MASK) | (((uint32_t)val) << SCAN_LDATA60_DS12_SHIFT))

#define  SCAN_LDATA60_RESERVED2_MASK 0xe0
#define  SCAN_LDATA60_RESERVED2_SHIFT 5
#define  SCAN_LDATA60_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA60_RESERVED2_MASK) >> SCAN_LDATA60_RESERVED2_SHIFT)
#define  SCAN_LDATA60_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA60_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA60_RESERVED2_SHIFT))

#define  SCAN_LDATA60_DC12_MASK 0x1f
#define  SCAN_LDATA60_DC12_SHIFT 0
#define  SCAN_LDATA60_DC12_MASK_SHIFT(reg) (((reg) & SCAN_LDATA60_DC12_MASK) >> SCAN_LDATA60_DC12_SHIFT)
#define  SCAN_LDATA60_DC12_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA60_DC12_MASK) | (((uint32_t)val) << SCAN_LDATA60_DC12_SHIFT))

//====================================================================
//Register: AFE Load Data 6 (Instance 2 of 2) (LDATA61)
/** \brief AFE Load Data 6 Register. (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LDATA61_DS11_MASK 0xff000000
#define  SCAN_LDATA61_DS11_SHIFT 24
#define  SCAN_LDATA61_DS11_MASK_SHIFT(reg) (((reg) & SCAN_LDATA61_DS11_MASK) >> SCAN_LDATA61_DS11_SHIFT)
#define  SCAN_LDATA61_DS11_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA61_DS11_MASK) | (((uint32_t)val) << SCAN_LDATA61_DS11_SHIFT))

#define  SCAN_LDATA61_RESERVED1_MASK 0xe00000
#define  SCAN_LDATA61_RESERVED1_SHIFT 21
#define  SCAN_LDATA61_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LDATA61_RESERVED1_MASK) >> SCAN_LDATA61_RESERVED1_SHIFT)
#define  SCAN_LDATA61_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA61_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LDATA61_RESERVED1_SHIFT))

#define  SCAN_LDATA61_DC11_MASK 0x1f0000
#define  SCAN_LDATA61_DC11_SHIFT 16
#define  SCAN_LDATA61_DC11_MASK_SHIFT(reg) (((reg) & SCAN_LDATA61_DC11_MASK) >> SCAN_LDATA61_DC11_SHIFT)
#define  SCAN_LDATA61_DC11_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA61_DC11_MASK) | (((uint32_t)val) << SCAN_LDATA61_DC11_SHIFT))

#define  SCAN_LDATA61_DS12_MASK 0xff00
#define  SCAN_LDATA61_DS12_SHIFT 8
#define  SCAN_LDATA61_DS12_MASK_SHIFT(reg) (((reg) & SCAN_LDATA61_DS12_MASK) >> SCAN_LDATA61_DS12_SHIFT)
#define  SCAN_LDATA61_DS12_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA61_DS12_MASK) | (((uint32_t)val) << SCAN_LDATA61_DS12_SHIFT))

#define  SCAN_LDATA61_RESERVED2_MASK 0xe0
#define  SCAN_LDATA61_RESERVED2_SHIFT 5
#define  SCAN_LDATA61_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_LDATA61_RESERVED2_MASK) >> SCAN_LDATA61_RESERVED2_SHIFT)
#define  SCAN_LDATA61_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA61_RESERVED2_MASK) | (((uint32_t)val) << SCAN_LDATA61_RESERVED2_SHIFT))

#define  SCAN_LDATA61_DC12_MASK 0x1f
#define  SCAN_LDATA61_DC12_SHIFT 0
#define  SCAN_LDATA61_DC12_MASK_SHIFT(reg) (((reg) & SCAN_LDATA61_DC12_MASK) >> SCAN_LDATA61_DC12_SHIFT)
#define  SCAN_LDATA61_DC12_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LDATA61_DC12_MASK) | (((uint32_t)val) << SCAN_LDATA61_DC12_SHIFT))

//====================================================================
//Register: AFE Pixel Sequence 1 (PSEQ1)
/** \brief The purpose of these registers is to map the AFE data to the PIC Common Bus Interface for different combinations of sensors, AFE types, and AFE programming.
<br/>
The AFE Pixel Sequence1 and Pixel Sequence2 Registers contain 6 6-bit Pixel Sequence Codes, as follows:
<br/>
<br/>
PSEQi = {S, P, V, Color[2:0]}
<br/>
<br/>
where
<br/>
<br/>
S = Sequence Complete
<br/>
P = DataType Complete; also decrements the SCANX data count when P = 1.
<br/>
V = Output Valid
<br/>
<br/>
Color[2:0] = Specified by the PIC Common Bus Interface:
<br/>
000 = Color 0 non-staggered
<br/>
001 = Color 1 non-staggered
<br/>
010 = Color 2 non-staggered
<br/>
011 = Monochrome non-staggered
<br/>
100 = Color 0 staggered
<br/>
101 = Color 1 staggered
<br/>
110 = Color 2 staggered
<br/>
111 = Monochrome staggered
<br/>
<br/>
The meaning of Color 0-2 and Monochrome may correspond to the traditional Red, Green, Blue and Monochrome, but may have alternate meanings for some sensors. Ultimately the purpose is to map the sensor/AFE data to a "Color Channel" that ends up in memory via the PIC data path.
<br/>
<br/>
Examples:
<br/>
<br/>
For non-staggered Monochrome CCD or Color CIS:
<br/>
PSEQ1 = 111_011
<br/>
PSEQ2-6 = don't care
<br/>
<br/>
For non-staggered CCD Color:
<br/>
<br/>
PSEQ1 = 001_000
<br/>
PSEQ2 = 001_001
<br/>
PSEQ3 = 111_010
<br/>
PSEQ4-6 = don't care
<br/>
<br/>
For staggered CCD Color:
<br/>
<br/>
PSEQ1 = 001_000
<br/>
PSEQ2 = 001_001
<br/>
PSEQ3 = 001_010
<br/>
PSEQ4 = 001_100
<br/>
PSEQ5 = 001_101
<br/>
PSEQ6 = 111_110
<br/>
<br/>
For non-staggered Monochrome with a dual-output sensor and using a dual-channel output mode AFE:
<br/>
<br/>
PSEQ1 = 011_011
<br/>
PSEQ2 = 111_011
<br/>
PSEQ3-6 = don't care*/
//====================================================================

#define  SCAN_PSEQ1_RESERVED1_MASK 0xc0000000
#define  SCAN_PSEQ1_RESERVED1_SHIFT 30
#define  SCAN_PSEQ1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_RESERVED1_MASK) >> SCAN_PSEQ1_RESERVED1_SHIFT)
#define  SCAN_PSEQ1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_RESERVED1_MASK) | (((uint32_t)val) << SCAN_PSEQ1_RESERVED1_SHIFT))

#define  SCAN_PSEQ1_PSEQ1_MASK 0x3f000000
#define  SCAN_PSEQ1_PSEQ1_SHIFT 24
#define  SCAN_PSEQ1_PSEQ1_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_PSEQ1_MASK) >> SCAN_PSEQ1_PSEQ1_SHIFT)
#define  SCAN_PSEQ1_PSEQ1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_PSEQ1_MASK) | (((uint32_t)val) << SCAN_PSEQ1_PSEQ1_SHIFT))

#define  SCAN_PSEQ1_RESERVED2_MASK 0xc00000
#define  SCAN_PSEQ1_RESERVED2_SHIFT 22
#define  SCAN_PSEQ1_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_RESERVED2_MASK) >> SCAN_PSEQ1_RESERVED2_SHIFT)
#define  SCAN_PSEQ1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_RESERVED2_MASK) | (((uint32_t)val) << SCAN_PSEQ1_RESERVED2_SHIFT))

#define  SCAN_PSEQ1_PSEQ2_MASK 0x3f0000
#define  SCAN_PSEQ1_PSEQ2_SHIFT 16
#define  SCAN_PSEQ1_PSEQ2_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_PSEQ2_MASK) >> SCAN_PSEQ1_PSEQ2_SHIFT)
#define  SCAN_PSEQ1_PSEQ2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_PSEQ2_MASK) | (((uint32_t)val) << SCAN_PSEQ1_PSEQ2_SHIFT))

#define  SCAN_PSEQ1_RESERVED3_MASK 0xc000
#define  SCAN_PSEQ1_RESERVED3_SHIFT 14
#define  SCAN_PSEQ1_RESERVED3_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_RESERVED3_MASK) >> SCAN_PSEQ1_RESERVED3_SHIFT)
#define  SCAN_PSEQ1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_RESERVED3_MASK) | (((uint32_t)val) << SCAN_PSEQ1_RESERVED3_SHIFT))

#define  SCAN_PSEQ1_PSEQ3_MASK 0x3f00
#define  SCAN_PSEQ1_PSEQ3_SHIFT 8
#define  SCAN_PSEQ1_PSEQ3_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_PSEQ3_MASK) >> SCAN_PSEQ1_PSEQ3_SHIFT)
#define  SCAN_PSEQ1_PSEQ3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_PSEQ3_MASK) | (((uint32_t)val) << SCAN_PSEQ1_PSEQ3_SHIFT))

#define  SCAN_PSEQ1_RESERVED4_MASK 0xc0
#define  SCAN_PSEQ1_RESERVED4_SHIFT 6
#define  SCAN_PSEQ1_RESERVED4_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_RESERVED4_MASK) >> SCAN_PSEQ1_RESERVED4_SHIFT)
#define  SCAN_PSEQ1_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_RESERVED4_MASK) | (((uint32_t)val) << SCAN_PSEQ1_RESERVED4_SHIFT))

#define  SCAN_PSEQ1_PSEQ4_MASK 0x3f
#define  SCAN_PSEQ1_PSEQ4_SHIFT 0
#define  SCAN_PSEQ1_PSEQ4_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ1_PSEQ4_MASK) >> SCAN_PSEQ1_PSEQ4_SHIFT)
#define  SCAN_PSEQ1_PSEQ4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ1_PSEQ4_MASK) | (((uint32_t)val) << SCAN_PSEQ1_PSEQ4_SHIFT))

//====================================================================
//Register: AFE Pixel Sequence 2 (PSEQ2)
/** \brief AFE Pixel Sequence 2 Register.*/
//====================================================================

#define  SCAN_PSEQ2_RESERVED1_MASK 0xffffc000
#define  SCAN_PSEQ2_RESERVED1_SHIFT 14
#define  SCAN_PSEQ2_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ2_RESERVED1_MASK) >> SCAN_PSEQ2_RESERVED1_SHIFT)
#define  SCAN_PSEQ2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ2_RESERVED1_MASK) | (((uint32_t)val) << SCAN_PSEQ2_RESERVED1_SHIFT))

#define  SCAN_PSEQ2_PSEQ5_MASK 0x3f00
#define  SCAN_PSEQ2_PSEQ5_SHIFT 8
#define  SCAN_PSEQ2_PSEQ5_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ2_PSEQ5_MASK) >> SCAN_PSEQ2_PSEQ5_SHIFT)
#define  SCAN_PSEQ2_PSEQ5_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ2_PSEQ5_MASK) | (((uint32_t)val) << SCAN_PSEQ2_PSEQ5_SHIFT))

#define  SCAN_PSEQ2_RESERVED2_MASK 0xc0
#define  SCAN_PSEQ2_RESERVED2_SHIFT 6
#define  SCAN_PSEQ2_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ2_RESERVED2_MASK) >> SCAN_PSEQ2_RESERVED2_SHIFT)
#define  SCAN_PSEQ2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ2_RESERVED2_MASK) | (((uint32_t)val) << SCAN_PSEQ2_RESERVED2_SHIFT))

#define  SCAN_PSEQ2_PSEQ6_MASK 0x3f
#define  SCAN_PSEQ2_PSEQ6_SHIFT 0
#define  SCAN_PSEQ2_PSEQ6_MASK_SHIFT(reg) (((reg) & SCAN_PSEQ2_PSEQ6_MASK) >> SCAN_PSEQ2_PSEQ6_SHIFT)
#define  SCAN_PSEQ2_PSEQ6_REPLACE_VAL(reg,val) (((reg) & ~SCAN_PSEQ2_PSEQ6_MASK) | (((uint32_t)val) << SCAN_PSEQ2_PSEQ6_SHIFT))

//====================================================================
//Register: Bulb PWM (BPWM)
/** \brief Bulb PWM Register.*/
//====================================================================

#define  SCAN_BPWM_RESERVED1_MASK 0xf0000000
#define  SCAN_BPWM_RESERVED1_SHIFT 28
#define  SCAN_BPWM_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_BPWM_RESERVED1_MASK) >> SCAN_BPWM_RESERVED1_SHIFT)
#define  SCAN_BPWM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_BPWM_RESERVED1_MASK) | (((uint32_t)val) << SCAN_BPWM_RESERVED1_SHIFT))

#define  SCAN_BPWM_BPWMPER_MASK 0xfff0000
#define  SCAN_BPWM_BPWMPER_SHIFT 16
#define  SCAN_BPWM_BPWMPER_MASK_SHIFT(reg) (((reg) & SCAN_BPWM_BPWMPER_MASK) >> SCAN_BPWM_BPWMPER_SHIFT)
#define  SCAN_BPWM_BPWMPER_REPLACE_VAL(reg,val) (((reg) & ~SCAN_BPWM_BPWMPER_MASK) | (((uint32_t)val) << SCAN_BPWM_BPWMPER_SHIFT))

#define  SCAN_BPWM_RESERVED2_MASK 0xf000
#define  SCAN_BPWM_RESERVED2_SHIFT 12
#define  SCAN_BPWM_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_BPWM_RESERVED2_MASK) >> SCAN_BPWM_RESERVED2_SHIFT)
#define  SCAN_BPWM_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_BPWM_RESERVED2_MASK) | (((uint32_t)val) << SCAN_BPWM_RESERVED2_SHIFT))

#define  SCAN_BPWM_BPWMDTY_MASK 0xfff
#define  SCAN_BPWM_BPWMDTY_SHIFT 0
#define  SCAN_BPWM_BPWMDTY_MASK_SHIFT(reg) (((reg) & SCAN_BPWM_BPWMDTY_MASK) >> SCAN_BPWM_BPWMDTY_SHIFT)
#define  SCAN_BPWM_BPWMDTY_REPLACE_VAL(reg,val) (((reg) & ~SCAN_BPWM_BPWMDTY_MASK) | (((uint32_t)val) << SCAN_BPWM_BPWMDTY_SHIFT))

//====================================================================
//Register: Motor Config (MCFG)
/** \brief Motor Configuration Register.*/
//====================================================================

#define  SCAN_MCFG_PSMODE_MASK 0x80000000
#define  SCAN_MCFG_PSMODE_SHIFT 31
#define  SCAN_MCFG_PSMODE_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_PSMODE_MASK) >> SCAN_MCFG_PSMODE_SHIFT)
#define  SCAN_MCFG_PSMODE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_PSMODE_MASK) | (((uint32_t)val) << SCAN_MCFG_PSMODE_SHIFT))

#define  SCAN_MCFG_PSCFG_MASK 0x70000000
#define  SCAN_MCFG_PSCFG_SHIFT 28
#define  SCAN_MCFG_PSCFG_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_PSCFG_MASK) >> SCAN_MCFG_PSCFG_SHIFT)
#define  SCAN_MCFG_PSCFG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_PSCFG_MASK) | (((uint32_t)val) << SCAN_MCFG_PSCFG_SHIFT))

#define  SCAN_MCFG_RESERVED1_MASK 0xc000000
#define  SCAN_MCFG_RESERVED1_SHIFT 26
#define  SCAN_MCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_RESERVED1_MASK) >> SCAN_MCFG_RESERVED1_SHIFT)
#define  SCAN_MCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_RESERVED1_MASK) | (((uint32_t)val) << SCAN_MCFG_RESERVED1_SHIFT))

#define  SCAN_MCFG_PSCOUNT_MASK 0x3ff0000
#define  SCAN_MCFG_PSCOUNT_SHIFT 16
#define  SCAN_MCFG_PSCOUNT_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_PSCOUNT_MASK) >> SCAN_MCFG_PSCOUNT_SHIFT)
#define  SCAN_MCFG_PSCOUNT_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_PSCOUNT_MASK) | (((uint32_t)val) << SCAN_MCFG_PSCOUNT_SHIFT))

#define  SCAN_MCFG_RESERVED2_MASK 0x8000
#define  SCAN_MCFG_RESERVED2_SHIFT 15
#define  SCAN_MCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_RESERVED2_MASK) >> SCAN_MCFG_RESERVED2_SHIFT)
#define  SCAN_MCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_RESERVED2_MASK) | (((uint32_t)val) << SCAN_MCFG_RESERVED2_SHIFT))

#define  SCAN_MCFG_LSMODE_MASK 0x6000
#define  SCAN_MCFG_LSMODE_SHIFT 13
#define  SCAN_MCFG_LSMODE_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_LSMODE_MASK) >> SCAN_MCFG_LSMODE_SHIFT)
#define  SCAN_MCFG_LSMODE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_LSMODE_MASK) | (((uint32_t)val) << SCAN_MCFG_LSMODE_SHIFT))

#define  SCAN_MCFG_RESERVED3_MASK 0x1000
#define  SCAN_MCFG_RESERVED3_SHIFT 12
#define  SCAN_MCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_RESERVED3_MASK) >> SCAN_MCFG_RESERVED3_SHIFT)
#define  SCAN_MCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_RESERVED3_MASK) | (((uint32_t)val) << SCAN_MCFG_RESERVED3_SHIFT))

#define  SCAN_MCFG_MTYPE_MASK 0x800
#define  SCAN_MCFG_MTYPE_SHIFT 11
#define  SCAN_MCFG_MTYPE_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_MTYPE_MASK) >> SCAN_MCFG_MTYPE_SHIFT)
#define  SCAN_MCFG_MTYPE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_MTYPE_MASK) | (((uint32_t)val) << SCAN_MCFG_MTYPE_SHIFT))

#define  SCAN_MCFG_RESERVED4_MASK 0x7ff
#define  SCAN_MCFG_RESERVED4_SHIFT 0
#define  SCAN_MCFG_RESERVED4_MASK_SHIFT(reg) (((reg) & SCAN_MCFG_RESERVED4_MASK) >> SCAN_MCFG_RESERVED4_SHIFT)
#define  SCAN_MCFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MCFG_RESERVED4_MASK) | (((uint32_t)val) << SCAN_MCFG_RESERVED4_SHIFT))

//====================================================================
//Register: Min scanclk cycles between Motor pulses in sync every mode (MotorSyncMin)
//====================================================================

#define  SCAN_MOTORSYNCMIN_MSMIN_MASK 0xffffffff
#define  SCAN_MOTORSYNCMIN_MSMIN_SHIFT 0
#define  SCAN_MOTORSYNCMIN_MSMIN_MASK_SHIFT(reg) (((reg) & SCAN_MOTORSYNCMIN_MSMIN_MASK) >> SCAN_MOTORSYNCMIN_MSMIN_SHIFT)
#define  SCAN_MOTORSYNCMIN_MSMIN_REPLACE_VAL(reg,val) (((reg) & ~SCAN_MOTORSYNCMIN_MSMIN_MASK) | (((uint32_t)val) << SCAN_MOTORSYNCMIN_MSMIN_SHIFT))

//====================================================================
//Register: AFE Program Control (AFEPC)
/** \brief AFE Program Control Register.*/
//====================================================================

#define  SCAN_AFEPC_RESERVED1_MASK 0x80000000
#define  SCAN_AFEPC_RESERVED1_SHIFT 31
#define  SCAN_AFEPC_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_AFEPC_RESERVED1_MASK) >> SCAN_AFEPC_RESERVED1_SHIFT)
#define  SCAN_AFEPC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_AFEPC_RESERVED1_MASK) | (((uint32_t)val) << SCAN_AFEPC_RESERVED1_SHIFT))

#define  SCAN_AFEPC_AUTO_MASK 0x40000000
#define  SCAN_AFEPC_AUTO_SHIFT 30
#define  SCAN_AFEPC_AUTO_MASK_SHIFT(reg) (((reg) & SCAN_AFEPC_AUTO_MASK) >> SCAN_AFEPC_AUTO_SHIFT)
#define  SCAN_AFEPC_AUTO_REPLACE_VAL(reg,val) (((reg) & ~SCAN_AFEPC_AUTO_MASK) | (((uint32_t)val) << SCAN_AFEPC_AUTO_SHIFT))

#define  SCAN_AFEPC_RESERVED2_MASK 0x3fffffff
#define  SCAN_AFEPC_RESERVED2_SHIFT 0
#define  SCAN_AFEPC_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_AFEPC_RESERVED2_MASK) >> SCAN_AFEPC_RESERVED2_SHIFT)
#define  SCAN_AFEPC_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_AFEPC_RESERVED2_MASK) | (((uint32_t)val) << SCAN_AFEPC_RESERVED2_SHIFT))

//====================================================================
//Register: CBI Arbitration Config (CFGARB)
/** \brief CBI Arbitration Configuration Register*/
//====================================================================

#define  SCAN_CFGARB_RESERVED1_MASK 0xfff80000
#define  SCAN_CFGARB_RESERVED1_SHIFT 19
#define  SCAN_CFGARB_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_CFGARB_RESERVED1_MASK) >> SCAN_CFGARB_RESERVED1_SHIFT)
#define  SCAN_CFGARB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CFGARB_RESERVED1_MASK) | (((uint32_t)val) << SCAN_CFGARB_RESERVED1_SHIFT))

#define  SCAN_CFGARB_EXTENABLE_MASK 0x70000
#define  SCAN_CFGARB_EXTENABLE_SHIFT 16
#define  SCAN_CFGARB_EXTENABLE_MASK_SHIFT(reg) (((reg) & SCAN_CFGARB_EXTENABLE_MASK) >> SCAN_CFGARB_EXTENABLE_SHIFT)
#define  SCAN_CFGARB_EXTENABLE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CFGARB_EXTENABLE_MASK) | (((uint32_t)val) << SCAN_CFGARB_EXTENABLE_SHIFT))

#define  SCAN_CFGARB_RESERVED2_MASK 0xfffc
#define  SCAN_CFGARB_RESERVED2_SHIFT 2
#define  SCAN_CFGARB_RESERVED2_MASK_SHIFT(reg) (((reg) & SCAN_CFGARB_RESERVED2_MASK) >> SCAN_CFGARB_RESERVED2_SHIFT)
#define  SCAN_CFGARB_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CFGARB_RESERVED2_MASK) | (((uint32_t)val) << SCAN_CFGARB_RESERVED2_SHIFT))

#define  SCAN_CFGARB_PARENABLE_MASK 0x3
#define  SCAN_CFGARB_PARENABLE_SHIFT 0
#define  SCAN_CFGARB_PARENABLE_MASK_SHIFT(reg) (((reg) & SCAN_CFGARB_PARENABLE_MASK) >> SCAN_CFGARB_PARENABLE_SHIFT)
#define  SCAN_CFGARB_PARENABLE_REPLACE_VAL(reg,val) (((reg) & ~SCAN_CFGARB_PARENABLE_MASK) | (((uint32_t)val) << SCAN_CFGARB_PARENABLE_SHIFT))

//====================================================================
//Register: LED0 Gate (Instance 1 of 2) (LED00)
/** \brief LED0 Gate Register.
<br/>
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LED00_LED0POS_MASK 0xffff0000
#define  SCAN_LED00_LED0POS_SHIFT 16
#define  SCAN_LED00_LED0POS_MASK_SHIFT(reg) (((reg) & SCAN_LED00_LED0POS_MASK) >> SCAN_LED00_LED0POS_SHIFT)
#define  SCAN_LED00_LED0POS_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED00_LED0POS_MASK) | (((uint32_t)val) << SCAN_LED00_LED0POS_SHIFT))

#define  SCAN_LED00_LED0NEG_MASK 0xffff
#define  SCAN_LED00_LED0NEG_SHIFT 0
#define  SCAN_LED00_LED0NEG_MASK_SHIFT(reg) (((reg) & SCAN_LED00_LED0NEG_MASK) >> SCAN_LED00_LED0NEG_SHIFT)
#define  SCAN_LED00_LED0NEG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED00_LED0NEG_MASK) | (((uint32_t)val) << SCAN_LED00_LED0NEG_SHIFT))

//====================================================================
//Register: LED0 Gate (Instance 2 of 2) (LED01)
/** \brief LED0 Gate Register.
<br/>
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LED01_LED0POS_MASK 0xffff0000
#define  SCAN_LED01_LED0POS_SHIFT 16
#define  SCAN_LED01_LED0POS_MASK_SHIFT(reg) (((reg) & SCAN_LED01_LED0POS_MASK) >> SCAN_LED01_LED0POS_SHIFT)
#define  SCAN_LED01_LED0POS_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED01_LED0POS_MASK) | (((uint32_t)val) << SCAN_LED01_LED0POS_SHIFT))

#define  SCAN_LED01_LED0NEG_MASK 0xffff
#define  SCAN_LED01_LED0NEG_SHIFT 0
#define  SCAN_LED01_LED0NEG_MASK_SHIFT(reg) (((reg) & SCAN_LED01_LED0NEG_MASK) >> SCAN_LED01_LED0NEG_SHIFT)
#define  SCAN_LED01_LED0NEG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED01_LED0NEG_MASK) | (((uint32_t)val) << SCAN_LED01_LED0NEG_SHIFT))

//====================================================================
//Register: LED1 Gate (Instance 1 of 2) (LED10)
/** \brief LED1 Gate Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LED10_LED1POS_MASK 0xffff0000
#define  SCAN_LED10_LED1POS_SHIFT 16
#define  SCAN_LED10_LED1POS_MASK_SHIFT(reg) (((reg) & SCAN_LED10_LED1POS_MASK) >> SCAN_LED10_LED1POS_SHIFT)
#define  SCAN_LED10_LED1POS_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED10_LED1POS_MASK) | (((uint32_t)val) << SCAN_LED10_LED1POS_SHIFT))

#define  SCAN_LED10_LED1NEG_MASK 0xffff
#define  SCAN_LED10_LED1NEG_SHIFT 0
#define  SCAN_LED10_LED1NEG_MASK_SHIFT(reg) (((reg) & SCAN_LED10_LED1NEG_MASK) >> SCAN_LED10_LED1NEG_SHIFT)
#define  SCAN_LED10_LED1NEG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED10_LED1NEG_MASK) | (((uint32_t)val) << SCAN_LED10_LED1NEG_SHIFT))

//====================================================================
//Register: LED1 Gate (Instance 2 of 2) (LED11)
/** \brief LED1 Gate Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LED11_LED1POS_MASK 0xffff0000
#define  SCAN_LED11_LED1POS_SHIFT 16
#define  SCAN_LED11_LED1POS_MASK_SHIFT(reg) (((reg) & SCAN_LED11_LED1POS_MASK) >> SCAN_LED11_LED1POS_SHIFT)
#define  SCAN_LED11_LED1POS_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED11_LED1POS_MASK) | (((uint32_t)val) << SCAN_LED11_LED1POS_SHIFT))

#define  SCAN_LED11_LED1NEG_MASK 0xffff
#define  SCAN_LED11_LED1NEG_SHIFT 0
#define  SCAN_LED11_LED1NEG_MASK_SHIFT(reg) (((reg) & SCAN_LED11_LED1NEG_MASK) >> SCAN_LED11_LED1NEG_SHIFT)
#define  SCAN_LED11_LED1NEG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED11_LED1NEG_MASK) | (((uint32_t)val) << SCAN_LED11_LED1NEG_SHIFT))

//====================================================================
//Register: LED2 Gate (Instance 1 of 2) (LED20)
/** \brief LED2 Gate Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports, LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports, LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LED20_LED2POS_MASK 0xffff0000
#define  SCAN_LED20_LED2POS_SHIFT 16
#define  SCAN_LED20_LED2POS_MASK_SHIFT(reg) (((reg) & SCAN_LED20_LED2POS_MASK) >> SCAN_LED20_LED2POS_SHIFT)
#define  SCAN_LED20_LED2POS_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED20_LED2POS_MASK) | (((uint32_t)val) << SCAN_LED20_LED2POS_SHIFT))

#define  SCAN_LED20_LED2NEG_MASK 0xffff
#define  SCAN_LED20_LED2NEG_SHIFT 0
#define  SCAN_LED20_LED2NEG_MASK_SHIFT(reg) (((reg) & SCAN_LED20_LED2NEG_MASK) >> SCAN_LED20_LED2NEG_SHIFT)
#define  SCAN_LED20_LED2NEG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED20_LED2NEG_MASK) | (((uint32_t)val) << SCAN_LED20_LED2NEG_SHIFT))

//====================================================================
//Register: LED2 Gate (Instance 2 of 2) (LED21)
/** \brief LED2 Gate Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports, LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports, LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LED21_LED2POS_MASK 0xffff0000
#define  SCAN_LED21_LED2POS_SHIFT 16
#define  SCAN_LED21_LED2POS_MASK_SHIFT(reg) (((reg) & SCAN_LED21_LED2POS_MASK) >> SCAN_LED21_LED2POS_SHIFT)
#define  SCAN_LED21_LED2POS_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED21_LED2POS_MASK) | (((uint32_t)val) << SCAN_LED21_LED2POS_SHIFT))

#define  SCAN_LED21_LED2NEG_MASK 0xffff
#define  SCAN_LED21_LED2NEG_SHIFT 0
#define  SCAN_LED21_LED2NEG_MASK_SHIFT(reg) (((reg) & SCAN_LED21_LED2NEG_MASK) >> SCAN_LED21_LED2NEG_SHIFT)
#define  SCAN_LED21_LED2NEG_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LED21_LED2NEG_MASK) | (((uint32_t)val) << SCAN_LED21_LED2NEG_SHIFT))

//====================================================================
//Register: LED PWM Period Config (Instance 1 of 2) (LPWMConfig0)
/** \brief LED PWM Period Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports, LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports, LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LPWMCONFIG0_LPWMPER_MASK 0xffff0000
#define  SCAN_LPWMCONFIG0_LPWMPER_SHIFT 16
#define  SCAN_LPWMCONFIG0_LPWMPER_MASK_SHIFT(reg) (((reg) & SCAN_LPWMCONFIG0_LPWMPER_MASK) >> SCAN_LPWMCONFIG0_LPWMPER_SHIFT)
#define  SCAN_LPWMCONFIG0_LPWMPER_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWMCONFIG0_LPWMPER_MASK) | (((uint32_t)val) << SCAN_LPWMCONFIG0_LPWMPER_SHIFT))

#define  SCAN_LPWMCONFIG0_RESERVED1_MASK 0xfff8
#define  SCAN_LPWMCONFIG0_RESERVED1_SHIFT 3
#define  SCAN_LPWMCONFIG0_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LPWMCONFIG0_RESERVED1_MASK) >> SCAN_LPWMCONFIG0_RESERVED1_SHIFT)
#define  SCAN_LPWMCONFIG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWMCONFIG0_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LPWMCONFIG0_RESERVED1_SHIFT))

#define  SCAN_LPWMCONFIG0_LEDPOL_MASK 0x7
#define  SCAN_LPWMCONFIG0_LEDPOL_SHIFT 0
#define  SCAN_LPWMCONFIG0_LEDPOL_MASK_SHIFT(reg) (((reg) & SCAN_LPWMCONFIG0_LEDPOL_MASK) >> SCAN_LPWMCONFIG0_LEDPOL_SHIFT)
#define  SCAN_LPWMCONFIG0_LEDPOL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWMCONFIG0_LEDPOL_MASK) | (((uint32_t)val) << SCAN_LPWMCONFIG0_LEDPOL_SHIFT))

//====================================================================
//Register: LED PWM Period Config (Instance 2 of 2) (LPWMConfig1)
/** \brief LED PWM Period Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports, LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports, LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LPWMCONFIG1_LPWMPER_MASK 0xffff0000
#define  SCAN_LPWMCONFIG1_LPWMPER_SHIFT 16
#define  SCAN_LPWMCONFIG1_LPWMPER_MASK_SHIFT(reg) (((reg) & SCAN_LPWMCONFIG1_LPWMPER_MASK) >> SCAN_LPWMCONFIG1_LPWMPER_SHIFT)
#define  SCAN_LPWMCONFIG1_LPWMPER_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWMCONFIG1_LPWMPER_MASK) | (((uint32_t)val) << SCAN_LPWMCONFIG1_LPWMPER_SHIFT))

#define  SCAN_LPWMCONFIG1_RESERVED1_MASK 0xfff8
#define  SCAN_LPWMCONFIG1_RESERVED1_SHIFT 3
#define  SCAN_LPWMCONFIG1_RESERVED1_MASK_SHIFT(reg) (((reg) & SCAN_LPWMCONFIG1_RESERVED1_MASK) >> SCAN_LPWMCONFIG1_RESERVED1_SHIFT)
#define  SCAN_LPWMCONFIG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWMCONFIG1_RESERVED1_MASK) | (((uint32_t)val) << SCAN_LPWMCONFIG1_RESERVED1_SHIFT))

#define  SCAN_LPWMCONFIG1_LEDPOL_MASK 0x7
#define  SCAN_LPWMCONFIG1_LEDPOL_SHIFT 0
#define  SCAN_LPWMCONFIG1_LEDPOL_MASK_SHIFT(reg) (((reg) & SCAN_LPWMCONFIG1_LEDPOL_MASK) >> SCAN_LPWMCONFIG1_LEDPOL_SHIFT)
#define  SCAN_LPWMCONFIG1_LEDPOL_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWMCONFIG1_LEDPOL_MASK) | (((uint32_t)val) << SCAN_LPWMCONFIG1_LEDPOL_SHIFT))

//====================================================================
//Register: LED0 PWM Pulse Config (Instance 1 of 2) (LPWM00)
/** \brief LED PWM0 (Red) Pulse Config Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LPWM00_LED0ON_MASK 0xffff0000
#define  SCAN_LPWM00_LED0ON_SHIFT 16
#define  SCAN_LPWM00_LED0ON_MASK_SHIFT(reg) (((reg) & SCAN_LPWM00_LED0ON_MASK) >> SCAN_LPWM00_LED0ON_SHIFT)
#define  SCAN_LPWM00_LED0ON_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM00_LED0ON_MASK) | (((uint32_t)val) << SCAN_LPWM00_LED0ON_SHIFT))

#define  SCAN_LPWM00_LED0OFF_MASK 0xffff
#define  SCAN_LPWM00_LED0OFF_SHIFT 0
#define  SCAN_LPWM00_LED0OFF_MASK_SHIFT(reg) (((reg) & SCAN_LPWM00_LED0OFF_MASK) >> SCAN_LPWM00_LED0OFF_SHIFT)
#define  SCAN_LPWM00_LED0OFF_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM00_LED0OFF_MASK) | (((uint32_t)val) << SCAN_LPWM00_LED0OFF_SHIFT))

//====================================================================
//Register: LED0 PWM Pulse Config (Instance 2 of 2) (LPWM01)
/** \brief LED PWM0 (Red) Pulse Config Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LPWM01_LED0ON_MASK 0xffff0000
#define  SCAN_LPWM01_LED0ON_SHIFT 16
#define  SCAN_LPWM01_LED0ON_MASK_SHIFT(reg) (((reg) & SCAN_LPWM01_LED0ON_MASK) >> SCAN_LPWM01_LED0ON_SHIFT)
#define  SCAN_LPWM01_LED0ON_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM01_LED0ON_MASK) | (((uint32_t)val) << SCAN_LPWM01_LED0ON_SHIFT))

#define  SCAN_LPWM01_LED0OFF_MASK 0xffff
#define  SCAN_LPWM01_LED0OFF_SHIFT 0
#define  SCAN_LPWM01_LED0OFF_MASK_SHIFT(reg) (((reg) & SCAN_LPWM01_LED0OFF_MASK) >> SCAN_LPWM01_LED0OFF_SHIFT)
#define  SCAN_LPWM01_LED0OFF_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM01_LED0OFF_MASK) | (((uint32_t)val) << SCAN_LPWM01_LED0OFF_SHIFT))

//====================================================================
//Register: LED1 PWM Pulse Config (Instance 1 of 2) (LPWM10)
/** \brief LED PWM1 (Green) Pulse Config Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LPWM10_LED1ON_MASK 0xffff0000
#define  SCAN_LPWM10_LED1ON_SHIFT 16
#define  SCAN_LPWM10_LED1ON_MASK_SHIFT(reg) (((reg) & SCAN_LPWM10_LED1ON_MASK) >> SCAN_LPWM10_LED1ON_SHIFT)
#define  SCAN_LPWM10_LED1ON_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM10_LED1ON_MASK) | (((uint32_t)val) << SCAN_LPWM10_LED1ON_SHIFT))

#define  SCAN_LPWM10_LED1OFF_MASK 0xffff
#define  SCAN_LPWM10_LED1OFF_SHIFT 0
#define  SCAN_LPWM10_LED1OFF_MASK_SHIFT(reg) (((reg) & SCAN_LPWM10_LED1OFF_MASK) >> SCAN_LPWM10_LED1OFF_SHIFT)
#define  SCAN_LPWM10_LED1OFF_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM10_LED1OFF_MASK) | (((uint32_t)val) << SCAN_LPWM10_LED1OFF_SHIFT))

//====================================================================
//Register: LED1 PWM Pulse Config (Instance 2 of 2) (LPWM11)
/** \brief LED PWM1 (Green) Pulse Config Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LPWM11_LED1ON_MASK 0xffff0000
#define  SCAN_LPWM11_LED1ON_SHIFT 16
#define  SCAN_LPWM11_LED1ON_MASK_SHIFT(reg) (((reg) & SCAN_LPWM11_LED1ON_MASK) >> SCAN_LPWM11_LED1ON_SHIFT)
#define  SCAN_LPWM11_LED1ON_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM11_LED1ON_MASK) | (((uint32_t)val) << SCAN_LPWM11_LED1ON_SHIFT))

#define  SCAN_LPWM11_LED1OFF_MASK 0xffff
#define  SCAN_LPWM11_LED1OFF_SHIFT 0
#define  SCAN_LPWM11_LED1OFF_MASK_SHIFT(reg) (((reg) & SCAN_LPWM11_LED1OFF_MASK) >> SCAN_LPWM11_LED1OFF_SHIFT)
#define  SCAN_LPWM11_LED1OFF_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM11_LED1OFF_MASK) | (((uint32_t)val) << SCAN_LPWM11_LED1OFF_SHIFT))

//====================================================================
//Register: LED2 PWM Pulse Config (Instance 1 of 2) (LPWM20)
/** \brief LED PWM2 (Blue) Pulse Config Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 1 of 2)*/
//====================================================================

#define  SCAN_LPWM20_LED2ON_MASK 0xffff0000
#define  SCAN_LPWM20_LED2ON_SHIFT 16
#define  SCAN_LPWM20_LED2ON_MASK_SHIFT(reg) (((reg) & SCAN_LPWM20_LED2ON_MASK) >> SCAN_LPWM20_LED2ON_SHIFT)
#define  SCAN_LPWM20_LED2ON_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM20_LED2ON_MASK) | (((uint32_t)val) << SCAN_LPWM20_LED2ON_SHIFT))

#define  SCAN_LPWM20_LED2OFF_MASK 0xffff
#define  SCAN_LPWM20_LED2OFF_SHIFT 0
#define  SCAN_LPWM20_LED2OFF_MASK_SHIFT(reg) (((reg) & SCAN_LPWM20_LED2OFF_MASK) >> SCAN_LPWM20_LED2OFF_SHIFT)
#define  SCAN_LPWM20_LED2OFF_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM20_LED2OFF_MASK) | (((uint32_t)val) << SCAN_LPWM20_LED2OFF_SHIFT))

//====================================================================
//Register: LED2 PWM Pulse Config (Instance 2 of 2) (LPWM21)
/** \brief LED PWM2 (Blue) Pulse Config Register.
<br/>
LED00, LED10, LED20, LPWMCOnfig0, LWPM00, LPWM10 and LPWM20 configure the LED controller associated with ports,
LEDR[0], CCD_BulbGate_LEDG[0] and CCD_BulbPWM_LEDB[0]
<br/>
<br/>
LED01, LED11, LED21, LPWMCOnfig1, LWPM01, LPWM11 and LPWM21 configure the LED controller associated with ports,
LEDR[1], CCD_BulbGate_LEDG[1] and CCD_BulbPWM_LEDB[1]
<br/>
<br/> (Instance 2 of 2)*/
//====================================================================

#define  SCAN_LPWM21_LED2ON_MASK 0xffff0000
#define  SCAN_LPWM21_LED2ON_SHIFT 16
#define  SCAN_LPWM21_LED2ON_MASK_SHIFT(reg) (((reg) & SCAN_LPWM21_LED2ON_MASK) >> SCAN_LPWM21_LED2ON_SHIFT)
#define  SCAN_LPWM21_LED2ON_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM21_LED2ON_MASK) | (((uint32_t)val) << SCAN_LPWM21_LED2ON_SHIFT))

#define  SCAN_LPWM21_LED2OFF_MASK 0xffff
#define  SCAN_LPWM21_LED2OFF_SHIFT 0
#define  SCAN_LPWM21_LED2OFF_MASK_SHIFT(reg) (((reg) & SCAN_LPWM21_LED2OFF_MASK) >> SCAN_LPWM21_LED2OFF_SHIFT)
#define  SCAN_LPWM21_LED2OFF_REPLACE_VAL(reg,val) (((reg) & ~SCAN_LPWM21_LED2OFF_MASK) | (((uint32_t)val) << SCAN_LPWM21_LED2OFF_SHIFT))

//====================================================================
//
//Register File: LVDS AFE Interface (LVDSAFE)
/** \brief Top-level register file for the LVDS AFE Control block*/
//
//====================================================================

//====================================================================
//Register: LVDS AFE Control Register (rb4LVDSAFEControl)
/** \brief This register contains control bits for the LVDS AFE block.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_MASK 0x80000000
#define  LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_SHIFT 31
#define  LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_LVDSAFE_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_MASK 0x40000000
#define  LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_SHIFT 30
#define  LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_SCAN_LINESYNC_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_MASK 0x20000000
#define  LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_SHIFT 29
#define  LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_EOI_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_MASK 0x1f000000
#define  LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_SHIFT 24
#define  LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_OR_PIXEL_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_MASK 0x800000
#define  LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_SHIFT 23
#define  LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_DUPLEX_TAG_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_MASK 0x600000
#define  LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_SHIFT 21
#define  LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_ASELA_OUT_SEL_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_MASK 0x1f0000
#define  LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_AND_PIXEL_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_MASK 0x8000
#define  LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_SHIFT 15
#define  LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_MASK 0x7000
#define  LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_SHIFT 12
#define  LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_OUT_EDGE_SEL_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_MASK 0xe00
#define  LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_SHIFT 9
#define  LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_INP_EDGE_SEL_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_MASK 0x1f0
#define  LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_SHIFT 4
#define  LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_START_OF_SCAN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_MASK 0xf
#define  LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_MASK) >> LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL_AFE_TYPE_SHIFT))

//====================================================================
//Register: LVDS AFE Control Register 2 (rb4LVDSAFEControl2)
/** \brief This register contains control bits for the LVDS AFE block.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_MASK 0x80000000
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_SHIFT 31
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_MASK 0x7c000000
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_SHIFT 26
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_CH2_PXLTYPE1_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_MASK 0x3e00000
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_SHIFT 21
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_CH1_PXLTYPE1_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_MASK 0x1f0000
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_CH0_PXLTYPE1_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_MASK 0x8000
#define  LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_SHIFT 15
#define  LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_AC_COLOR_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_MASK 0x6000
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_SHIFT 13
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_RESERVED2_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_MASK 0x1000
#define  LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_SHIFT 12
#define  LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_ENDIAN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_MASK 0x800
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_SHIFT 11
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_RESERVED3_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_MASK 0x700
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_SHIFT 8
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_CH2_COLOR_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_MASK 0x80
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_SHIFT 7
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_RESERVED4_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_MASK 0x70
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_SHIFT 4
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_CH1_COLOR_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_MASK 0x8
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_SHIFT 3
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_RESERVED5_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_MASK 0x7
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_MASK) >> LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL2_CH0_COLOR_SHIFT))

//====================================================================
//Register: LVDS AFE Control Register 3 (rb4LVDSAFEControl3)
/** \brief This register contains control bits for the LVDS AFE block.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_MASK 0xe000
#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_SHIFT 13
#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_STAG_START_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_MASK 0x1000
#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_SHIFT 12
#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_STAG_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_MASK 0xf00
#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_SHIFT 8
#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_MASK 0x80
#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_SHIFT 7
#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_PLL_CLK_MASK_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_MASK 0x70
#define  LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_SHIFT 4
#define  LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_LAST_PXL_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_MASK 0x8
#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_SHIFT 3
#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_RESERVED2_SHIFT))

#define  LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_MASK 0x7
#define  LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_MASK) >> LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECONTROL3_FIRST_PXL_EN_SHIFT))

//====================================================================
//Register: LVDS AFE Alternate Pixel Encodings Register (rb4LVDSAFEAltPxl)
/** \brief This register contains alternate pixel encodings for the pixel type in the LVDS AFE data stream.
   If alternate encodings are not needed, the alternate values for each channel should be programmed the same as pixel type 1.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_MASK 0x80000000
#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_SHIFT 31
#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_MASK 0x7c000000
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_SHIFT 26
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE3_SHIFT))

#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_MASK 0x3e00000
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_SHIFT 21
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE3_SHIFT))

#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_MASK 0x1f0000
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_SHIFT 16
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE3_SHIFT))

#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_MASK 0x8000
#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_SHIFT 15
#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_RESERVED2_SHIFT))

#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_MASK 0x7c00
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_SHIFT 10
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_CH2_PXLTYPE2_SHIFT))

#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_MASK 0x3e0
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_SHIFT 5
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_CH1_PXLTYPE2_SHIFT))

#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_MASK 0x1f
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_SHIFT 0
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_MASK) >> LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_SHIFT)
#define  LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEALTPXL_CH0_PXLTYPE2_SHIFT))

//====================================================================
//Register: LVDS AFE Data Line Sync Register (rb4LVDSAFEDataLineSync)
/** \brief This register contains the enable and encoding for line sync via the data bus.
   The TI VSP56** AFEs are examples of AFEs that generate the line sync code via the data bits, rather than command bits.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_SHIFT 16
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_MASK) >> LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_SHIFT)
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_SHIFT))

#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_MASK 0xfffe
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_SHIFT 1
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEDATALINESYNC_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_MASK 0x1
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_SHIFT 0
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_MASK) >> LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEDATALINESYNC_DATA_LINE_SYNC_EN_SHIFT))

//====================================================================
//Register: LVDS AFE Clock Register (rb4LVDSAFEClk)
/** \brief This register contains settings related to the LVDS Clock output to the LVDS AFE.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_MASK 0x80000000
#define  LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_SHIFT 31
#define  LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_MASK) >> LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_SHIFT)
#define  LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECLK_LVDS_CLK_OUT_EN_SHIFT))

#define  LVDSAFE_RB4LVDSAFECLK_RESERVED1_MASK 0x7fff0000
#define  LVDSAFE_RB4LVDSAFECLK_RESERVED1_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECLK_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECLK_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFECLK_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFECLK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECLK_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECLK_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_MASK 0xff00
#define  LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_SHIFT 8
#define  LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_MASK) >> LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_SHIFT)
#define  LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECLK_CLK_HIGH_SHIFT))

#define  LVDSAFE_RB4LVDSAFECLK_CLK_LOW_MASK 0xff
#define  LVDSAFE_RB4LVDSAFECLK_CLK_LOW_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECLK_CLK_LOW_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECLK_CLK_LOW_MASK) >> LVDSAFE_RB4LVDSAFECLK_CLK_LOW_SHIFT)
#define  LVDSAFE_RB4LVDSAFECLK_CLK_LOW_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECLK_CLK_LOW_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECLK_CLK_LOW_SHIFT))

//====================================================================
//Register: LVDS AFE Channel 0 Line Count Register (rb4LVDSAFECh0LineCount)
/** \brief This register provides setting of the pixels/line and lines/image count values for channel 0.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_MASK) >> LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_LINE_IMG_CNT_SHIFT))

#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_MASK 0xffff
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_MASK) >> LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH0LINECOUNT_CH0_PXL_LINE_CNT_SHIFT))

//====================================================================
//Register: LVDS AFE Channel 0 Line Count Observation Register (rb4LVDSAFECh0LineCountObs)
/** \brief This register allows read-only observation of the pixels/line and lines/image current count values for channel 0.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_MASK) >> LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_LINE_IMG_CNT_OBS_SHIFT))

#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_MASK 0xffff
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_MASK) >> LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH0LINECOUNTOBS_CH0_PXL_LINE_CNT_OBS_SHIFT))

//====================================================================
//Register: LVDS AFE Channel 1 Line Count Register (rb4LVDSAFECh1LineCount)
/** \brief This register provides setting of the pixels/line and lines/image count values for channel 1.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_MASK) >> LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_LINE_IMG_CNT_SHIFT))

#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_MASK 0xffff
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_MASK) >> LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH1LINECOUNT_CH1_PXL_LINE_CNT_SHIFT))

//====================================================================
//Register: LVDS AFE Channel 1 Line Count Observation Register (rb4LVDSAFECh1LineCountObs)
/** \brief This register allows read-only observation of the pixels/line and lines/image current count values for channel 1.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_MASK) >> LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_LINE_IMG_CNT_OBS_SHIFT))

#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_MASK 0xffff
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_MASK) >> LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH1LINECOUNTOBS_CH1_PXL_LINE_CNT_OBS_SHIFT))

//====================================================================
//Register: LVDS AFE Channel 2 Line Count Register (rb4LVDSAFECh2LineCount)
/** \brief This register provides setting of the pixels/line and lines/image count values for channel 2.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_MASK) >> LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_LINE_IMG_CNT_SHIFT))

#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_MASK 0xffff
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_MASK) >> LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH2LINECOUNT_CH2_PXL_LINE_CNT_SHIFT))

//====================================================================
//Register: LVDS AFE Channel 2 Line Count Observation Register (rb4LVDSAFECh2LineCountObs)
/** \brief This register allows read-only observation of the pixels/line and lines/image current count values for channel 2.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_MASK 0xffff0000
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_SHIFT 16
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_MASK) >> LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_LINE_IMG_CNT_OBS_SHIFT))

#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_MASK 0xffff
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_SHIFT 0
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_MASK) >> LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_SHIFT)
#define  LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFECH2LINECOUNTOBS_CH2_PXL_LINE_CNT_OBS_SHIFT))

//====================================================================
//Register: LVDS AFE IRQ Enable Register (rb4LVDSAFEIrqEnb)
/** \brief The controller generates several interrupt conditions.<BR>
   The channel line interrupt is asserted when the pixel/line count reaches zero.<BR>
   The channel image interrupt is asserted when the line/image count reaches zero.<BR>
   The overflow interrupt is asserted when an incoming pixel arrives before the previous pixel has been accepted by the downstream block.<BR>
   Writing a '1' to an enable bit will enable that interrupt source. All interrupt sources are OR'd together for a single interrupt output from the block.
   Writing a '1' to the irq force bit will force the block to issue an irq to the interrupt controller until cleared. The force bit is used only for testing.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_MASK 0xffffff80
#define  LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_SHIFT 7
#define  LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_MASK 0x40
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_SHIFT 6
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_IRQ_OVFLW_ENB_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_MASK 0x20
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_SHIFT 5
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_IMAGE_ENB_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_MASK 0x10
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_SHIFT 4
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH2_LINE_ENB_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_MASK 0x8
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_SHIFT 3
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_IMAGE_ENB_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_MASK 0x4
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_SHIFT 2
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH1_LINE_ENB_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_MASK 0x2
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_SHIFT 1
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_IMAGE_ENB_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_MASK 0x1
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_SHIFT 0
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_MASK) >> LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQENB_IRQ_CH0_LINE_ENB_SHIFT))

//====================================================================
//Register: LVDS AFE IRQ Pending Register (rb4LVDSAFEIrqPnd)
/** \brief This register provides read-only bits for observing any pending interrupts.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_MASK 0xffffff80
#define  LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_SHIFT 7
#define  LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_MASK 0x40
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_SHIFT 6
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_IRQ_OVFLW_PND_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_MASK 0x20
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_SHIFT 5
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_IMAGE_PND_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_MASK 0x10
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_SHIFT 4
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH2_LINE_PND_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_MASK 0x8
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_SHIFT 3
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_IMAGE_PND_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_MASK 0x4
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_SHIFT 2
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH1_LINE_PND_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_MASK 0x2
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_SHIFT 1
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_IMAGE_PND_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_MASK 0x1
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_SHIFT 0
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_MASK) >> LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQPND_IRQ_CH0_LINE_PND_SHIFT))

//====================================================================
//Register: LVDS AFE IRQ Acknowledge Register (rb4LVDSAFEIrqAck)
/** \brief This register provides write-only bits for acknowledging(clearing) any pending interrupts.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_MASK 0xffffff80
#define  LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_SHIFT 7
#define  LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_MASK 0x40
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_SHIFT 6
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_IRQ_OVFLW_ACK_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_MASK 0x20
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_SHIFT 5
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_IMAGE_ACK_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_MASK 0x10
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_SHIFT 4
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH2_LINE_ACK_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_MASK 0x8
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_SHIFT 3
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_IMAGE_ACK_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_MASK 0x4
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_SHIFT 2
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH1_LINE_ACK_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_MASK 0x2
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_SHIFT 1
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_IMAGE_ACK_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_MASK 0x1
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_SHIFT 0
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_MASK) >> LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQACK_IRQ_CH0_LINE_ACK_SHIFT))

//====================================================================
//Register: LVDS AFE IRQ Force Register (rb4LVDSAFEIrqForce)
/** \brief This register provides write-only bits for forcing(for test purposes) any interrupts.*/
//====================================================================

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_MASK 0xffffff80
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_SHIFT 7
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_RESERVED1_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_MASK 0x40
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_SHIFT 6
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_OVFLW_FORCE_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_MASK 0x20
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_SHIFT 5
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_IMAGE_FORCE_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_MASK 0x10
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_SHIFT 4
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH2_LINE_FORCE_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_MASK 0x8
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_SHIFT 3
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_IMAGE_FORCE_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_MASK 0x4
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_SHIFT 2
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH1_LINE_FORCE_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_MASK 0x2
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_SHIFT 1
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_IMAGE_FORCE_SHIFT))

#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_MASK 0x1
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_SHIFT 0
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_MASK_SHIFT(reg) (((reg) & LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_MASK) >> LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_SHIFT)
#define  LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_MASK) | (((uint32_t)val) << LVDSAFE_RB4LVDSAFEIRQFORCE_IRQ_CH0_LINE_FORCE_SHIFT))

//====================================================================
//
//Register File: Camera Capture Interface (VCAP)
/** \brief Top-level register file for the VCAP (Camera Capture) block*/
//
//====================================================================

//====================================================================
//Register: VCap Control Register (rb4VCapControl)
/** \brief This register contains control bits for the VCap Camera Capture block.*/
//====================================================================

#define  VCAP_RB4VCAPCONTROL_RESERVED1_MASK 0xfff80000
#define  VCAP_RB4VCAPCONTROL_RESERVED1_SHIFT 19
#define  VCAP_RB4VCAPCONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_RESERVED1_MASK) >> VCAP_RB4VCAPCONTROL_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPCONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPCONTROL_EOI_EN_MASK 0x40000
#define  VCAP_RB4VCAPCONTROL_EOI_EN_SHIFT 18
#define  VCAP_RB4VCAPCONTROL_EOI_EN_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_EOI_EN_MASK) >> VCAP_RB4VCAPCONTROL_EOI_EN_SHIFT)
#define  VCAP_RB4VCAPCONTROL_EOI_EN_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_EOI_EN_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_EOI_EN_SHIFT))

#define  VCAP_RB4VCAPCONTROL_SINGLE_FRAME_MASK 0x20000
#define  VCAP_RB4VCAPCONTROL_SINGLE_FRAME_SHIFT 17
#define  VCAP_RB4VCAPCONTROL_SINGLE_FRAME_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_SINGLE_FRAME_MASK) >> VCAP_RB4VCAPCONTROL_SINGLE_FRAME_SHIFT)
#define  VCAP_RB4VCAPCONTROL_SINGLE_FRAME_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_SINGLE_FRAME_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_SINGLE_FRAME_SHIFT))

#define  VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_MASK 0x10000
#define  VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_SHIFT 16
#define  VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_MASK) >> VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_SHIFT)
#define  VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_NEXT_FRAME_ARM_SHIFT))

#define  VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_MASK 0x8000
#define  VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_SHIFT 15
#define  VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_MASK) >> VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_SHIFT)
#define  VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_RISE_EDGE_SAMP_SHIFT))

#define  VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_MASK 0x4000
#define  VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_SHIFT 14
#define  VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_MASK) >> VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_SHIFT)
#define  VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_FALL_EDGE_SAMP_SHIFT))

#define  VCAP_RB4VCAPCONTROL_FAST_EDGE_MASK 0x2000
#define  VCAP_RB4VCAPCONTROL_FAST_EDGE_SHIFT 13
#define  VCAP_RB4VCAPCONTROL_FAST_EDGE_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_FAST_EDGE_MASK) >> VCAP_RB4VCAPCONTROL_FAST_EDGE_SHIFT)
#define  VCAP_RB4VCAPCONTROL_FAST_EDGE_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_FAST_EDGE_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_FAST_EDGE_SHIFT))

#define  VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_MASK 0x1000
#define  VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_SHIFT 12
#define  VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_MASK) >> VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_SHIFT)
#define  VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_ST_EDGE_SEL_SHIFT))

#define  VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_MASK 0x800
#define  VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_SHIFT 11
#define  VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_MASK) >> VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_SHIFT)
#define  VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_VS_EDGE_SEL_SHIFT))

#define  VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_MASK 0x400
#define  VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_SHIFT 10
#define  VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_MASK) >> VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_SHIFT)
#define  VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_HS_EDGE_SEL_SHIFT))

#define  VCAP_RB4VCAPCONTROL_VS_POL_MASK 0x200
#define  VCAP_RB4VCAPCONTROL_VS_POL_SHIFT 9
#define  VCAP_RB4VCAPCONTROL_VS_POL_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_VS_POL_MASK) >> VCAP_RB4VCAPCONTROL_VS_POL_SHIFT)
#define  VCAP_RB4VCAPCONTROL_VS_POL_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_VS_POL_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_VS_POL_SHIFT))

#define  VCAP_RB4VCAPCONTROL_HS_POL_MASK 0x100
#define  VCAP_RB4VCAPCONTROL_HS_POL_SHIFT 8
#define  VCAP_RB4VCAPCONTROL_HS_POL_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_HS_POL_MASK) >> VCAP_RB4VCAPCONTROL_HS_POL_SHIFT)
#define  VCAP_RB4VCAPCONTROL_HS_POL_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_HS_POL_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_HS_POL_SHIFT))

#define  VCAP_RB4VCAPCONTROL_SOFT_RST_MASK 0x80
#define  VCAP_RB4VCAPCONTROL_SOFT_RST_SHIFT 7
#define  VCAP_RB4VCAPCONTROL_SOFT_RST_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_SOFT_RST_MASK) >> VCAP_RB4VCAPCONTROL_SOFT_RST_SHIFT)
#define  VCAP_RB4VCAPCONTROL_SOFT_RST_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_SOFT_RST_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_SOFT_RST_SHIFT))

#define  VCAP_RB4VCAPCONTROL_RESERVED2_MASK 0x40
#define  VCAP_RB4VCAPCONTROL_RESERVED2_SHIFT 6
#define  VCAP_RB4VCAPCONTROL_RESERVED2_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_RESERVED2_MASK) >> VCAP_RB4VCAPCONTROL_RESERVED2_SHIFT)
#define  VCAP_RB4VCAPCONTROL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_RESERVED2_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_RESERVED2_SHIFT))

#define  VCAP_RB4VCAPCONTROL_RESERVED3_MASK 0x20
#define  VCAP_RB4VCAPCONTROL_RESERVED3_SHIFT 5
#define  VCAP_RB4VCAPCONTROL_RESERVED3_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_RESERVED3_MASK) >> VCAP_RB4VCAPCONTROL_RESERVED3_SHIFT)
#define  VCAP_RB4VCAPCONTROL_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_RESERVED3_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_RESERVED3_SHIFT))

#define  VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_MASK 0x10
#define  VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_SHIFT 4
#define  VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_MASK) >> VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_SHIFT)
#define  VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_PXL_CNTR_EN_SHIFT))

#define  VCAP_RB4VCAPCONTROL_RESERVED4_MASK 0x8
#define  VCAP_RB4VCAPCONTROL_RESERVED4_SHIFT 3
#define  VCAP_RB4VCAPCONTROL_RESERVED4_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_RESERVED4_MASK) >> VCAP_RB4VCAPCONTROL_RESERVED4_SHIFT)
#define  VCAP_RB4VCAPCONTROL_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_RESERVED4_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_RESERVED4_SHIFT))

#define  VCAP_RB4VCAPCONTROL_GATE_ON_VS_MASK 0x4
#define  VCAP_RB4VCAPCONTROL_GATE_ON_VS_SHIFT 2
#define  VCAP_RB4VCAPCONTROL_GATE_ON_VS_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_GATE_ON_VS_MASK) >> VCAP_RB4VCAPCONTROL_GATE_ON_VS_SHIFT)
#define  VCAP_RB4VCAPCONTROL_GATE_ON_VS_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_GATE_ON_VS_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_GATE_ON_VS_SHIFT))

#define  VCAP_RB4VCAPCONTROL_GATE_ON_HS_MASK 0x2
#define  VCAP_RB4VCAPCONTROL_GATE_ON_HS_SHIFT 1
#define  VCAP_RB4VCAPCONTROL_GATE_ON_HS_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_GATE_ON_HS_MASK) >> VCAP_RB4VCAPCONTROL_GATE_ON_HS_SHIFT)
#define  VCAP_RB4VCAPCONTROL_GATE_ON_HS_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_GATE_ON_HS_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_GATE_ON_HS_SHIFT))

#define  VCAP_RB4VCAPCONTROL_CAP_ENB_MASK 0x1
#define  VCAP_RB4VCAPCONTROL_CAP_ENB_SHIFT 0
#define  VCAP_RB4VCAPCONTROL_CAP_ENB_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCONTROL_CAP_ENB_MASK) >> VCAP_RB4VCAPCONTROL_CAP_ENB_SHIFT)
#define  VCAP_RB4VCAPCONTROL_CAP_ENB_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCONTROL_CAP_ENB_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCONTROL_CAP_ENB_SHIFT))

//====================================================================
//Register: VCap Counter Register (rb4VCapCounter)
/** \brief This register contains the pixel counter.
   If the Counter Enable bit = '1' in the rb2VCapControl register, then the counter counts down. This register should be pre-loaded with the desired number of pixels to capture. As each pixel is received into the data fifo, the counter is decremented. An interrupt can be generated when the count reaches zero.*/
//====================================================================

#define  VCAP_RB4VCAPCOUNTER_RESERVED1_MASK 0xff000000
#define  VCAP_RB4VCAPCOUNTER_RESERVED1_SHIFT 24
#define  VCAP_RB4VCAPCOUNTER_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCOUNTER_RESERVED1_MASK) >> VCAP_RB4VCAPCOUNTER_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPCOUNTER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCOUNTER_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCOUNTER_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPCOUNTER_PXL_CNTR_MASK 0xffffff
#define  VCAP_RB4VCAPCOUNTER_PXL_CNTR_SHIFT 0
#define  VCAP_RB4VCAPCOUNTER_PXL_CNTR_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCOUNTER_PXL_CNTR_MASK) >> VCAP_RB4VCAPCOUNTER_PXL_CNTR_SHIFT)
#define  VCAP_RB4VCAPCOUNTER_PXL_CNTR_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCOUNTER_PXL_CNTR_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCOUNTER_PXL_CNTR_SHIFT))

//====================================================================
//Register: VCap Counter Observation Register (rb4VCapCounterObs)
/** \brief This register allows read-only observation of the current pixel counter value.*/
//====================================================================

#define  VCAP_RB4VCAPCOUNTEROBS_RESERVED1_MASK 0xff000000
#define  VCAP_RB4VCAPCOUNTEROBS_RESERVED1_SHIFT 24
#define  VCAP_RB4VCAPCOUNTEROBS_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCOUNTEROBS_RESERVED1_MASK) >> VCAP_RB4VCAPCOUNTEROBS_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPCOUNTEROBS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCOUNTEROBS_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCOUNTEROBS_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_MASK 0xffffff
#define  VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_SHIFT 0
#define  VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_MASK) >> VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_SHIFT)
#define  VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCOUNTEROBS_PXL_CNTR_OBS_SHIFT))

//====================================================================
//Register: VCap Pixel Observation Register (rb4VCapPixelObs)
/** \brief This register allows read-only observation of the current pixel value.*/
//====================================================================

#define  VCAP_RB4VCAPPIXELOBS_RESERVED1_MASK 0xffe00000
#define  VCAP_RB4VCAPPIXELOBS_RESERVED1_SHIFT 21
#define  VCAP_RB4VCAPPIXELOBS_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPPIXELOBS_RESERVED1_MASK) >> VCAP_RB4VCAPPIXELOBS_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPPIXELOBS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPPIXELOBS_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPPIXELOBS_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPPIXELOBS_PXL_OBS_MASK 0x1fffff
#define  VCAP_RB4VCAPPIXELOBS_PXL_OBS_SHIFT 0
#define  VCAP_RB4VCAPPIXELOBS_PXL_OBS_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPPIXELOBS_PXL_OBS_MASK) >> VCAP_RB4VCAPPIXELOBS_PXL_OBS_SHIFT)
#define  VCAP_RB4VCAPPIXELOBS_PXL_OBS_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPPIXELOBS_PXL_OBS_MASK) | (((uint32_t)val) << VCAP_RB4VCAPPIXELOBS_PXL_OBS_SHIFT))

//====================================================================
//Register: VCap Line Count Register (rb4VCapLineCount)
/** \brief This register provides setting of the pixels/line and lines/image count values. This register is write only but values may be observed in rb4VCapLineCountObs register.*/
//====================================================================

#define  VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_MASK 0xffff0000
#define  VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_SHIFT 16
#define  VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_MASK) >> VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_SHIFT)
#define  VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_MASK) | (((uint32_t)val) << VCAP_RB4VCAPLINECOUNT_LINE_IMG_CNT_SHIFT))

#define  VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_MASK 0xffff
#define  VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_SHIFT 0
#define  VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_MASK) >> VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_SHIFT)
#define  VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_MASK) | (((uint32_t)val) << VCAP_RB4VCAPLINECOUNT_PXL_LINE_CNT_SHIFT))

//====================================================================
//Register: VCap Line Count Observation Register (rb4VCapLineCountObs)
/** \brief This register allows read-only observation of the pixels/line and lines/image current count values. These are decrementing counters.*/
//====================================================================

#define  VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_MASK 0xffff0000
#define  VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_SHIFT 16
#define  VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_MASK) >> VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_SHIFT)
#define  VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_MASK) | (((uint32_t)val) << VCAP_RB4VCAPLINECOUNTOBS_LINE_IMG_CNT_OBS_SHIFT))

#define  VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_MASK 0xffff
#define  VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_SHIFT 0
#define  VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_MASK) >> VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_SHIFT)
#define  VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_MASK) | (((uint32_t)val) << VCAP_RB4VCAPLINECOUNTOBS_PXL_LINE_CNT_OBS_SHIFT))

//====================================================================
//Register: VCap IRQ Enable Register (rb4VCapIrqEnb)
/** \brief The controller generates several interrupt conditions.
   The HS interrupt indicates a rising or falling edge on the HS input (edge selectable via control register). Similarly, for VS and ST edge interrupts.
   The Overflow interrupt indicates an incoming pixel was received prior to the previous pixel being accepted by the downstream block.
   The Counter interrupt is generated when the Countdown Enable bit ='1' in rb2VCapCtonrol, and the rb4VCapCounter register has counted down to zero.
   Writing a '1' to an enable bit will enable that interrupt source. All interrupt sources are OR'd together for a single interrupt output from the block.
   Writing a '1' to the irq force bit will force VCap to issue an irq to the interrupt controller until cleared. The force bit is used only for testing.*/
//====================================================================

#define  VCAP_RB4VCAPIRQENB_RESERVED1_MASK 0xffffffe0
#define  VCAP_RB4VCAPIRQENB_RESERVED1_SHIFT 5
#define  VCAP_RB4VCAPIRQENB_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQENB_RESERVED1_MASK) >> VCAP_RB4VCAPIRQENB_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPIRQENB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQENB_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQENB_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_MASK 0x10
#define  VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_SHIFT 4
#define  VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_MASK) >> VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_SHIFT)
#define  VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQENB_IRQ_CNTR_ENB_SHIFT))

#define  VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_MASK 0x8
#define  VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_SHIFT 3
#define  VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_MASK) >> VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_SHIFT)
#define  VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQENB_IRQ_OVFLW_ENB_SHIFT))

#define  VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_MASK 0x4
#define  VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_SHIFT 2
#define  VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_MASK) >> VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_SHIFT)
#define  VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQENB_IRQ_ST_ENB_SHIFT))

#define  VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_MASK 0x2
#define  VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_SHIFT 1
#define  VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_MASK) >> VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_SHIFT)
#define  VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQENB_IRQ_VS_ENB_SHIFT))

#define  VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_MASK 0x1
#define  VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_SHIFT 0
#define  VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_MASK) >> VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_SHIFT)
#define  VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQENB_IRQ_HS_ENB_SHIFT))

//====================================================================
//Register: VCap IRQ Pending Register (rb4VCapIrqPnd)
/** \brief This register provides read-only bits for observing any pending VCAP interrupts.*/
//====================================================================

#define  VCAP_RB4VCAPIRQPND_RESERVED1_MASK 0xffffffe0
#define  VCAP_RB4VCAPIRQPND_RESERVED1_SHIFT 5
#define  VCAP_RB4VCAPIRQPND_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQPND_RESERVED1_MASK) >> VCAP_RB4VCAPIRQPND_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPIRQPND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQPND_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQPND_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_MASK 0x10
#define  VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_SHIFT 4
#define  VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_MASK) >> VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_SHIFT)
#define  VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQPND_IRQ_CNTR_PNDG_SHIFT))

#define  VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_MASK 0x8
#define  VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_SHIFT 3
#define  VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_MASK) >> VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_SHIFT)
#define  VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQPND_IRQ_OVFLW_PNDG_SHIFT))

#define  VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_MASK 0x4
#define  VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_SHIFT 2
#define  VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_MASK) >> VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_SHIFT)
#define  VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQPND_IRQ_ST_PNDG_SHIFT))

#define  VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_MASK 0x2
#define  VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_SHIFT 1
#define  VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_MASK) >> VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_SHIFT)
#define  VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQPND_IRQ_VS_PNDG_SHIFT))

#define  VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_MASK 0x1
#define  VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_SHIFT 0
#define  VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_MASK) >> VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_SHIFT)
#define  VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQPND_IRQ_HS_PNDG_SHIFT))

//====================================================================
//Register: VCap IRQ Acknowledge Register (rb4VCapIrqAck)
/** \brief This register provides write-only bits for acknowledging(clearing) any pending VCAP interrupts.*/
//====================================================================

#define  VCAP_RB4VCAPIRQACK_RESERVED1_MASK 0xffffffe0
#define  VCAP_RB4VCAPIRQACK_RESERVED1_SHIFT 5
#define  VCAP_RB4VCAPIRQACK_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQACK_RESERVED1_MASK) >> VCAP_RB4VCAPIRQACK_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPIRQACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQACK_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQACK_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_MASK 0x10
#define  VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_SHIFT 4
#define  VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_MASK) >> VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_SHIFT)
#define  VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQACK_IRQ_CNTR_ACK_SHIFT))

#define  VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_MASK 0x8
#define  VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_SHIFT 3
#define  VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_MASK) >> VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_SHIFT)
#define  VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQACK_IRQ_OVFLW_ACK_SHIFT))

#define  VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_MASK 0x4
#define  VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_SHIFT 2
#define  VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_MASK) >> VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_SHIFT)
#define  VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQACK_IRQ_ST_ACK_SHIFT))

#define  VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_MASK 0x2
#define  VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_SHIFT 1
#define  VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_MASK) >> VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_SHIFT)
#define  VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQACK_IRQ_VS_ACK_SHIFT))

#define  VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_MASK 0x1
#define  VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_SHIFT 0
#define  VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_MASK) >> VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_SHIFT)
#define  VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQACK_IRQ_HS_ACK_SHIFT))

//====================================================================
//Register: VCap IRQ Force Register (rb4VCapIrqForce)
/** \brief This register provides write-only bits for forcing(for test purposes) any VCAP interrupts.*/
//====================================================================

#define  VCAP_RB4VCAPIRQFORCE_RESERVED1_MASK 0xffffffe0
#define  VCAP_RB4VCAPIRQFORCE_RESERVED1_SHIFT 5
#define  VCAP_RB4VCAPIRQFORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQFORCE_RESERVED1_MASK) >> VCAP_RB4VCAPIRQFORCE_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPIRQFORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQFORCE_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQFORCE_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_MASK 0x10
#define  VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_SHIFT 4
#define  VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_MASK) >> VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_SHIFT)
#define  VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQFORCE_IRQ_CNTR_FORCE_SHIFT))

#define  VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_MASK 0x8
#define  VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_SHIFT 3
#define  VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_MASK) >> VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_SHIFT)
#define  VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQFORCE_IRQ_OVFLW_FORCE_SHIFT))

#define  VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_MASK 0x4
#define  VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_SHIFT 2
#define  VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_MASK) >> VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_SHIFT)
#define  VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQFORCE_IRQ_ST_FORCE_SHIFT))

#define  VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_MASK 0x2
#define  VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_SHIFT 1
#define  VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_MASK) >> VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_SHIFT)
#define  VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQFORCE_IRQ_VS_FORCE_SHIFT))

#define  VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_MASK 0x1
#define  VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_SHIFT 0
#define  VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_MASK) >> VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_SHIFT)
#define  VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_MASK) | (((uint32_t)val) << VCAP_RB4VCAPIRQFORCE_IRQ_HS_FORCE_SHIFT))

//====================================================================
//Register: VCAP Clock Register (rb4VCapClk)
/** \brief This register contains settings related to the VCAP Clock output to the Camera sensor. The system clock source may be dithered (200MHz) or non-dithered (~209MHz) and is the same as selected for the SCAN block.*/
//====================================================================

#define  VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_MASK 0x80000000
#define  VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_SHIFT 31
#define  VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_MASK) >> VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_SHIFT)
#define  VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCLK_VCAP_CLK_OUT_EN_SHIFT))

#define  VCAP_RB4VCAPCLK_RESERVED1_MASK 0x7fff0000
#define  VCAP_RB4VCAPCLK_RESERVED1_SHIFT 16
#define  VCAP_RB4VCAPCLK_RESERVED1_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCLK_RESERVED1_MASK) >> VCAP_RB4VCAPCLK_RESERVED1_SHIFT)
#define  VCAP_RB4VCAPCLK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCLK_RESERVED1_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCLK_RESERVED1_SHIFT))

#define  VCAP_RB4VCAPCLK_CLK_HIGH_MASK 0xff00
#define  VCAP_RB4VCAPCLK_CLK_HIGH_SHIFT 8
#define  VCAP_RB4VCAPCLK_CLK_HIGH_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCLK_CLK_HIGH_MASK) >> VCAP_RB4VCAPCLK_CLK_HIGH_SHIFT)
#define  VCAP_RB4VCAPCLK_CLK_HIGH_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCLK_CLK_HIGH_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCLK_CLK_HIGH_SHIFT))

#define  VCAP_RB4VCAPCLK_CLK_LOW_MASK 0xff
#define  VCAP_RB4VCAPCLK_CLK_LOW_SHIFT 0
#define  VCAP_RB4VCAPCLK_CLK_LOW_MASK_SHIFT(reg) (((reg) & VCAP_RB4VCAPCLK_CLK_LOW_MASK) >> VCAP_RB4VCAPCLK_CLK_LOW_SHIFT)
#define  VCAP_RB4VCAPCLK_CLK_LOW_REPLACE_VAL(reg,val) (((reg) & ~VCAP_RB4VCAPCLK_CLK_LOW_MASK) | (((uint32_t)val) << VCAP_RB4VCAPCLK_CLK_LOW_SHIFT))

#endif // SCAN
