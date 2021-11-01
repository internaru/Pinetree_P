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
 * \file HiPS_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HIPS_REGMASKS_H_
#define _HIPS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: HiPS (HiPS)
/** \brief Top-level register file for HiPS block*/
//
//====================================================================

//====================================================================
//Register: HiPS Channel Enable (Chan_Enable)
/** \brief The HiPS Channel Enable Register is used to enable the HiPS clock to each HiPS channel inside the HiPS core.*/
//====================================================================

#define  HIPS_CHAN_ENABLE_RESERVED1_MASK 0xffffff00
#define  HIPS_CHAN_ENABLE_RESERVED1_SHIFT 8
#define  HIPS_CHAN_ENABLE_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPS_CHAN_ENABLE_RESERVED1_MASK) >> HIPS_CHAN_ENABLE_RESERVED1_SHIFT)
#define  HIPS_CHAN_ENABLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPS_CHAN_ENABLE_RESERVED1_MASK) | (((uint32_t)val) << HIPS_CHAN_ENABLE_RESERVED1_SHIFT))

#define  HIPS_CHAN_ENABLE_CH_ENABLE_MASK 0xff
#define  HIPS_CHAN_ENABLE_CH_ENABLE_SHIFT 0
#define  HIPS_CHAN_ENABLE_CH_ENABLE_MASK_SHIFT(reg) (((reg) & HIPS_CHAN_ENABLE_CH_ENABLE_MASK) >> HIPS_CHAN_ENABLE_CH_ENABLE_SHIFT)
#define  HIPS_CHAN_ENABLE_CH_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HIPS_CHAN_ENABLE_CH_ENABLE_MASK) | (((uint32_t)val) << HIPS_CHAN_ENABLE_CH_ENABLE_SHIFT))

//====================================================================
//Register: HiPS FWBIST Control (FWBIST_Control)
/** \brief The HiPS FWBIST Control Register is used to control the activation and diagnostic mode of HiPS FWBIST operation.*/
//====================================================================

#define  HIPS_FWBIST_CONTROL_RESERVED1_MASK 0xfffffffc
#define  HIPS_FWBIST_CONTROL_RESERVED1_SHIFT 2
#define  HIPS_FWBIST_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_CONTROL_RESERVED1_MASK) >> HIPS_FWBIST_CONTROL_RESERVED1_SHIFT)
#define  HIPS_FWBIST_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << HIPS_FWBIST_CONTROL_RESERVED1_SHIFT))

#define  HIPS_FWBIST_CONTROL_DIAGNOSTIC_MASK 0x2
#define  HIPS_FWBIST_CONTROL_DIAGNOSTIC_SHIFT 1
#define  HIPS_FWBIST_CONTROL_DIAGNOSTIC_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_CONTROL_DIAGNOSTIC_MASK) >> HIPS_FWBIST_CONTROL_DIAGNOSTIC_SHIFT)
#define  HIPS_FWBIST_CONTROL_DIAGNOSTIC_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_CONTROL_DIAGNOSTIC_MASK) | (((uint32_t)val) << HIPS_FWBIST_CONTROL_DIAGNOSTIC_SHIFT))

#define  HIPS_FWBIST_CONTROL_ACTIVATE_MASK 0x1
#define  HIPS_FWBIST_CONTROL_ACTIVATE_SHIFT 0
#define  HIPS_FWBIST_CONTROL_ACTIVATE_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_CONTROL_ACTIVATE_MASK) >> HIPS_FWBIST_CONTROL_ACTIVATE_SHIFT)
#define  HIPS_FWBIST_CONTROL_ACTIVATE_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_CONTROL_ACTIVATE_MASK) | (((uint32_t)val) << HIPS_FWBIST_CONTROL_ACTIVATE_SHIFT))

//====================================================================
//Register: HiPS FWBIST Enable (FWBIST_Enable)
/** \brief The HiPS FWBIST Enable Register is used to enable HiPS FWBIST for each HiPS channel inside the HiPS core.*/
//====================================================================

#define  HIPS_FWBIST_ENABLE_RESERVED1_MASK 0xffffff00
#define  HIPS_FWBIST_ENABLE_RESERVED1_SHIFT 8
#define  HIPS_FWBIST_ENABLE_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_ENABLE_RESERVED1_MASK) >> HIPS_FWBIST_ENABLE_RESERVED1_SHIFT)
#define  HIPS_FWBIST_ENABLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_ENABLE_RESERVED1_MASK) | (((uint32_t)val) << HIPS_FWBIST_ENABLE_RESERVED1_SHIFT))

#define  HIPS_FWBIST_ENABLE_ENABLE_MASK 0xff
#define  HIPS_FWBIST_ENABLE_ENABLE_SHIFT 0
#define  HIPS_FWBIST_ENABLE_ENABLE_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_ENABLE_ENABLE_MASK) >> HIPS_FWBIST_ENABLE_ENABLE_SHIFT)
#define  HIPS_FWBIST_ENABLE_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_ENABLE_ENABLE_MASK) | (((uint32_t)val) << HIPS_FWBIST_ENABLE_ENABLE_SHIFT))

//====================================================================
//Register: HiPS FWBIST Error (FWBIST_Error)
/** \brief The HiPS FWBIST Error Register is used to determine if an error occurred during HiPS FWBIST diagnostic mode for each HiPS channel inside the HiPS core.*/
//====================================================================

#define  HIPS_FWBIST_ERROR_RESERVED1_MASK 0xffffff00
#define  HIPS_FWBIST_ERROR_RESERVED1_SHIFT 8
#define  HIPS_FWBIST_ERROR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_ERROR_RESERVED1_MASK) >> HIPS_FWBIST_ERROR_RESERVED1_SHIFT)
#define  HIPS_FWBIST_ERROR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_ERROR_RESERVED1_MASK) | (((uint32_t)val) << HIPS_FWBIST_ERROR_RESERVED1_SHIFT))

#define  HIPS_FWBIST_ERROR_ERROR_MASK 0xff
#define  HIPS_FWBIST_ERROR_ERROR_SHIFT 0
#define  HIPS_FWBIST_ERROR_ERROR_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_ERROR_ERROR_MASK) >> HIPS_FWBIST_ERROR_ERROR_SHIFT)
#define  HIPS_FWBIST_ERROR_ERROR_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_ERROR_ERROR_MASK) | (((uint32_t)val) << HIPS_FWBIST_ERROR_ERROR_SHIFT))

//====================================================================
//Register: HiPS FWBIST Done (FWBIST_Done)
/** \brief The HiPS FWBIST Done Register is used to determine if the HiPS FWBIST finished for each HiPS channel inside the HiPS core.*/
//====================================================================

#define  HIPS_FWBIST_DONE_RESERVED1_MASK 0xffffff00
#define  HIPS_FWBIST_DONE_RESERVED1_SHIFT 8
#define  HIPS_FWBIST_DONE_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DONE_RESERVED1_MASK) >> HIPS_FWBIST_DONE_RESERVED1_SHIFT)
#define  HIPS_FWBIST_DONE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DONE_RESERVED1_MASK) | (((uint32_t)val) << HIPS_FWBIST_DONE_RESERVED1_SHIFT))

#define  HIPS_FWBIST_DONE_DONE_MASK 0xff
#define  HIPS_FWBIST_DONE_DONE_SHIFT 0
#define  HIPS_FWBIST_DONE_DONE_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DONE_DONE_MASK) >> HIPS_FWBIST_DONE_DONE_SHIFT)
#define  HIPS_FWBIST_DONE_DONE_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DONE_DONE_MASK) | (((uint32_t)val) << HIPS_FWBIST_DONE_DONE_SHIFT))

//====================================================================
//Register: HiPS FWBIST PassnFail (FWBIST_PassnFail)
/** \brief The HiPS FWBIST PassnFail Register is used to determine if the HiPS FWBIST passed or failed for each HiPS channel inside the HiPS core.*/
//====================================================================

#define  HIPS_FWBIST_PASSNFAIL_RESERVED1_MASK 0xffffff00
#define  HIPS_FWBIST_PASSNFAIL_RESERVED1_SHIFT 8
#define  HIPS_FWBIST_PASSNFAIL_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_PASSNFAIL_RESERVED1_MASK) >> HIPS_FWBIST_PASSNFAIL_RESERVED1_SHIFT)
#define  HIPS_FWBIST_PASSNFAIL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_PASSNFAIL_RESERVED1_MASK) | (((uint32_t)val) << HIPS_FWBIST_PASSNFAIL_RESERVED1_SHIFT))

#define  HIPS_FWBIST_PASSNFAIL_PNF_MASK 0xff
#define  HIPS_FWBIST_PASSNFAIL_PNF_SHIFT 0
#define  HIPS_FWBIST_PASSNFAIL_PNF_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_PASSNFAIL_PNF_MASK) >> HIPS_FWBIST_PASSNFAIL_PNF_SHIFT)
#define  HIPS_FWBIST_PASSNFAIL_PNF_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_PASSNFAIL_PNF_MASK) | (((uint32_t)val) << HIPS_FWBIST_PASSNFAIL_PNF_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 1 of 8) (FWBIST_Diagnostic_Table0)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 1 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE0_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE0_EXPECTED_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 2 of 8) (FWBIST_Diagnostic_Table1)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 2 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE1_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE1_EXPECTED_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 3 of 8) (FWBIST_Diagnostic_Table2)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 3 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE2_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE2_EXPECTED_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 4 of 8) (FWBIST_Diagnostic_Table3)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 4 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE3_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE3_EXPECTED_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 5 of 8) (FWBIST_Diagnostic_Table4)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 5 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE4_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE4_EXPECTED_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 6 of 8) (FWBIST_Diagnostic_Table5)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 6 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE5_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE5_EXPECTED_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 7 of 8) (FWBIST_Diagnostic_Table6)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 7 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE6_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE6_EXPECTED_SHIFT))

//====================================================================
//Register: HiPS FWBIST Diagnostic Values Table (Instance 8 of 8) (FWBIST_Diagnostic_Table7)
/** \brief The HiPS FWBIST Diagnostic Values Table stores the diagnostic data for each HiPS channel when an error occurs. (Instance 8 of 8)*/
//====================================================================

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_MASK 0xffff0000
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_SHIFT 16
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE7_ACTUAL_SHIFT))

#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_MASK 0xffff
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_SHIFT 0
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_MASK_SHIFT(reg) (((reg) & HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_MASK) >> HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_SHIFT)
#define  HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_REPLACE_VAL(reg,val) (((reg) & ~HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_MASK) | (((uint32_t)val) << HIPS_FWBIST_DIAGNOSTIC_TABLE7_EXPECTED_SHIFT))

#endif // HIPS
