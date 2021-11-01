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
 * \file SERP_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SERP_REGMASKS_H_
#define _SERP_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SERP (SERP)
/** \brief Register descriptions for the SERP block.*/
//
//====================================================================

//====================================================================
//Register: SERP Configuration Register (SCR)
/** \brief This register configures the general operation of the SERP block. If SERP is bypassed, the states of the internal state machines are reset, but register settings are preserved.*/
//====================================================================

#define  SERP_SCR_TLB_SRAM_ACCESS_ENABLE_MASK 0x80000000
#define  SERP_SCR_TLB_SRAM_ACCESS_ENABLE_SHIFT 31
#define  SERP_SCR_TLB_SRAM_ACCESS_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_TLB_SRAM_ACCESS_ENABLE_MASK) >> SERP_SCR_TLB_SRAM_ACCESS_ENABLE_SHIFT)
#define  SERP_SCR_TLB_SRAM_ACCESS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_TLB_SRAM_ACCESS_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_TLB_SRAM_ACCESS_ENABLE_SHIFT))

#define  SERP_SCR_OBJ_ENCODED_SELECT_MASK 0x70000000
#define  SERP_SCR_OBJ_ENCODED_SELECT_SHIFT 28
#define  SERP_SCR_OBJ_ENCODED_SELECT_MASK_SHIFT(reg) (((reg) & SERP_SCR_OBJ_ENCODED_SELECT_MASK) >> SERP_SCR_OBJ_ENCODED_SELECT_SHIFT)
#define  SERP_SCR_OBJ_ENCODED_SELECT_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_OBJ_ENCODED_SELECT_MASK) | (((uint32_t)val) << SERP_SCR_OBJ_ENCODED_SELECT_SHIFT))

#define  SERP_SCR_RESERVED1_MASK 0xc000000
#define  SERP_SCR_RESERVED1_SHIFT 26
#define  SERP_SCR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SCR_RESERVED1_MASK) >> SERP_SCR_RESERVED1_SHIFT)
#define  SERP_SCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_RESERVED1_MASK) | (((uint32_t)val) << SERP_SCR_RESERVED1_SHIFT))

#define  SERP_SCR_OBJ_MERGE_ENABLE_MASK 0x3000000
#define  SERP_SCR_OBJ_MERGE_ENABLE_SHIFT 24
#define  SERP_SCR_OBJ_MERGE_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_OBJ_MERGE_ENABLE_MASK) >> SERP_SCR_OBJ_MERGE_ENABLE_SHIFT)
#define  SERP_SCR_OBJ_MERGE_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_OBJ_MERGE_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_OBJ_MERGE_ENABLE_SHIFT))

#define  SERP_SCR_RESERVED2_MASK 0xe00000
#define  SERP_SCR_RESERVED2_SHIFT 21
#define  SERP_SCR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SCR_RESERVED2_MASK) >> SERP_SCR_RESERVED2_SHIFT)
#define  SERP_SCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_RESERVED2_MASK) | (((uint32_t)val) << SERP_SCR_RESERVED2_SHIFT))

#define  SERP_SCR_EDGE_STRENGTH_ENABLE_MASK 0x100000
#define  SERP_SCR_EDGE_STRENGTH_ENABLE_SHIFT 20
#define  SERP_SCR_EDGE_STRENGTH_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_EDGE_STRENGTH_ENABLE_MASK) >> SERP_SCR_EDGE_STRENGTH_ENABLE_SHIFT)
#define  SERP_SCR_EDGE_STRENGTH_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_EDGE_STRENGTH_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_EDGE_STRENGTH_ENABLE_SHIFT))

#define  SERP_SCR_RESERVED3_MASK 0xe0000
#define  SERP_SCR_RESERVED3_SHIFT 17
#define  SERP_SCR_RESERVED3_MASK_SHIFT(reg) (((reg) & SERP_SCR_RESERVED3_MASK) >> SERP_SCR_RESERVED3_SHIFT)
#define  SERP_SCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_RESERVED3_MASK) | (((uint32_t)val) << SERP_SCR_RESERVED3_SHIFT))

#define  SERP_SCR_BLACK_CORE_ENABLE_MASK 0x10000
#define  SERP_SCR_BLACK_CORE_ENABLE_SHIFT 16
#define  SERP_SCR_BLACK_CORE_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_BLACK_CORE_ENABLE_MASK) >> SERP_SCR_BLACK_CORE_ENABLE_SHIFT)
#define  SERP_SCR_BLACK_CORE_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_BLACK_CORE_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_BLACK_CORE_ENABLE_SHIFT))

#define  SERP_SCR_RESERVED4_MASK 0xc000
#define  SERP_SCR_RESERVED4_SHIFT 14
#define  SERP_SCR_RESERVED4_MASK_SHIFT(reg) (((reg) & SERP_SCR_RESERVED4_MASK) >> SERP_SCR_RESERVED4_SHIFT)
#define  SERP_SCR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_RESERVED4_MASK) | (((uint32_t)val) << SERP_SCR_RESERVED4_SHIFT))

#define  SERP_SCR_HR1_ENABLE_MASK 0x2000
#define  SERP_SCR_HR1_ENABLE_SHIFT 13
#define  SERP_SCR_HR1_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_HR1_ENABLE_MASK) >> SERP_SCR_HR1_ENABLE_SHIFT)
#define  SERP_SCR_HR1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_HR1_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_HR1_ENABLE_SHIFT))

#define  SERP_SCR_HR0_ENABLE_MASK 0x1000
#define  SERP_SCR_HR0_ENABLE_SHIFT 12
#define  SERP_SCR_HR0_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_HR0_ENABLE_MASK) >> SERP_SCR_HR0_ENABLE_SHIFT)
#define  SERP_SCR_HR0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_HR0_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_HR0_ENABLE_SHIFT))

#define  SERP_SCR_RESERVED5_MASK 0xe00
#define  SERP_SCR_RESERVED5_SHIFT 9
#define  SERP_SCR_RESERVED5_MASK_SHIFT(reg) (((reg) & SERP_SCR_RESERVED5_MASK) >> SERP_SCR_RESERVED5_SHIFT)
#define  SERP_SCR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_RESERVED5_MASK) | (((uint32_t)val) << SERP_SCR_RESERVED5_SHIFT))

#define  SERP_SCR_TEXT_MERGE_ENABLE_MASK 0x100
#define  SERP_SCR_TEXT_MERGE_ENABLE_SHIFT 8
#define  SERP_SCR_TEXT_MERGE_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_TEXT_MERGE_ENABLE_MASK) >> SERP_SCR_TEXT_MERGE_ENABLE_SHIFT)
#define  SERP_SCR_TEXT_MERGE_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_TEXT_MERGE_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_TEXT_MERGE_ENABLE_SHIFT))

#define  SERP_SCR_RESERVED6_MASK 0xc0
#define  SERP_SCR_RESERVED6_SHIFT 6
#define  SERP_SCR_RESERVED6_MASK_SHIFT(reg) (((reg) & SERP_SCR_RESERVED6_MASK) >> SERP_SCR_RESERVED6_SHIFT)
#define  SERP_SCR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_RESERVED6_MASK) | (((uint32_t)val) << SERP_SCR_RESERVED6_SHIFT))

#define  SERP_SCR_SERP_START_DIR_MASK 0x20
#define  SERP_SCR_SERP_START_DIR_SHIFT 5
#define  SERP_SCR_SERP_START_DIR_MASK_SHIFT(reg) (((reg) & SERP_SCR_SERP_START_DIR_MASK) >> SERP_SCR_SERP_START_DIR_SHIFT)
#define  SERP_SCR_SERP_START_DIR_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_SERP_START_DIR_MASK) | (((uint32_t)val) << SERP_SCR_SERP_START_DIR_SHIFT))

#define  SERP_SCR_SERPENTINE_DATA_MASK 0x10
#define  SERP_SCR_SERPENTINE_DATA_SHIFT 4
#define  SERP_SCR_SERPENTINE_DATA_MASK_SHIFT(reg) (((reg) & SERP_SCR_SERPENTINE_DATA_MASK) >> SERP_SCR_SERPENTINE_DATA_SHIFT)
#define  SERP_SCR_SERPENTINE_DATA_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_SERPENTINE_DATA_MASK) | (((uint32_t)val) << SERP_SCR_SERPENTINE_DATA_SHIFT))

#define  SERP_SCR_RESERVED7_MASK 0xc
#define  SERP_SCR_RESERVED7_SHIFT 2
#define  SERP_SCR_RESERVED7_MASK_SHIFT(reg) (((reg) & SERP_SCR_RESERVED7_MASK) >> SERP_SCR_RESERVED7_SHIFT)
#define  SERP_SCR_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_RESERVED7_MASK) | (((uint32_t)val) << SERP_SCR_RESERVED7_SHIFT))

#define  SERP_SCR_SAVE_RESTORE_MODE_MASK 0x2
#define  SERP_SCR_SAVE_RESTORE_MODE_SHIFT 1
#define  SERP_SCR_SAVE_RESTORE_MODE_MASK_SHIFT(reg) (((reg) & SERP_SCR_SAVE_RESTORE_MODE_MASK) >> SERP_SCR_SAVE_RESTORE_MODE_SHIFT)
#define  SERP_SCR_SAVE_RESTORE_MODE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_SAVE_RESTORE_MODE_MASK) | (((uint32_t)val) << SERP_SCR_SAVE_RESTORE_MODE_SHIFT))

#define  SERP_SCR_BYPASS_ENABLE_MASK 0x1
#define  SERP_SCR_BYPASS_ENABLE_SHIFT 0
#define  SERP_SCR_BYPASS_ENABLE_MASK_SHIFT(reg) (((reg) & SERP_SCR_BYPASS_ENABLE_MASK) >> SERP_SCR_BYPASS_ENABLE_SHIFT)
#define  SERP_SCR_BYPASS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SCR_BYPASS_ENABLE_MASK) | (((uint32_t)val) << SERP_SCR_BYPASS_ENABLE_SHIFT))

//====================================================================
//Register: SERP Halftone Replace Register (SHRR)
/** \brief This register configures the operation of halftone replace0
      and halftone replace1 by setting the input data channel used as the source for replacement, the bit width and position of the data within that channel, as well as the output channel. <b>Please note that the output contone channel cannot be yellow (ch2) or text black (ch4).</b>*/
//====================================================================

#define  SERP_SHRR_RESERVED1_MASK 0xf8000000
#define  SERP_SHRR_RESERVED1_SHIFT 27
#define  SERP_SHRR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SHRR_RESERVED1_MASK) >> SERP_SHRR_RESERVED1_SHIFT)
#define  SERP_SHRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_RESERVED1_MASK) | (((uint32_t)val) << SERP_SHRR_RESERVED1_SHIFT))

#define  SERP_SHRR_HR1_OUTCHAN_MASK 0x7000000
#define  SERP_SHRR_HR1_OUTCHAN_SHIFT 24
#define  SERP_SHRR_HR1_OUTCHAN_MASK_SHIFT(reg) (((reg) & SERP_SHRR_HR1_OUTCHAN_MASK) >> SERP_SHRR_HR1_OUTCHAN_SHIFT)
#define  SERP_SHRR_HR1_OUTCHAN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_HR1_OUTCHAN_MASK) | (((uint32_t)val) << SERP_SHRR_HR1_OUTCHAN_SHIFT))

#define  SERP_SHRR_RESERVED2_MASK 0xc00000
#define  SERP_SHRR_RESERVED2_SHIFT 22
#define  SERP_SHRR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SHRR_RESERVED2_MASK) >> SERP_SHRR_RESERVED2_SHIFT)
#define  SERP_SHRR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_RESERVED2_MASK) | (((uint32_t)val) << SERP_SHRR_RESERVED2_SHIFT))

#define  SERP_SHRR_HR1_INCONFIG_MASK 0x3f0000
#define  SERP_SHRR_HR1_INCONFIG_SHIFT 16
#define  SERP_SHRR_HR1_INCONFIG_MASK_SHIFT(reg) (((reg) & SERP_SHRR_HR1_INCONFIG_MASK) >> SERP_SHRR_HR1_INCONFIG_SHIFT)
#define  SERP_SHRR_HR1_INCONFIG_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_HR1_INCONFIG_MASK) | (((uint32_t)val) << SERP_SHRR_HR1_INCONFIG_SHIFT))

#define  SERP_SHRR_RESERVED3_MASK 0xf800
#define  SERP_SHRR_RESERVED3_SHIFT 11
#define  SERP_SHRR_RESERVED3_MASK_SHIFT(reg) (((reg) & SERP_SHRR_RESERVED3_MASK) >> SERP_SHRR_RESERVED3_SHIFT)
#define  SERP_SHRR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_RESERVED3_MASK) | (((uint32_t)val) << SERP_SHRR_RESERVED3_SHIFT))

#define  SERP_SHRR_HR0_OUTCHAN_MASK 0x700
#define  SERP_SHRR_HR0_OUTCHAN_SHIFT 8
#define  SERP_SHRR_HR0_OUTCHAN_MASK_SHIFT(reg) (((reg) & SERP_SHRR_HR0_OUTCHAN_MASK) >> SERP_SHRR_HR0_OUTCHAN_SHIFT)
#define  SERP_SHRR_HR0_OUTCHAN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_HR0_OUTCHAN_MASK) | (((uint32_t)val) << SERP_SHRR_HR0_OUTCHAN_SHIFT))

#define  SERP_SHRR_RESERVED4_MASK 0xc0
#define  SERP_SHRR_RESERVED4_SHIFT 6
#define  SERP_SHRR_RESERVED4_MASK_SHIFT(reg) (((reg) & SERP_SHRR_RESERVED4_MASK) >> SERP_SHRR_RESERVED4_SHIFT)
#define  SERP_SHRR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_RESERVED4_MASK) | (((uint32_t)val) << SERP_SHRR_RESERVED4_SHIFT))

#define  SERP_SHRR_HR0_INCONFIG_MASK 0x3f
#define  SERP_SHRR_HR0_INCONFIG_SHIFT 0
#define  SERP_SHRR_HR0_INCONFIG_MASK_SHIFT(reg) (((reg) & SERP_SHRR_HR0_INCONFIG_MASK) >> SERP_SHRR_HR0_INCONFIG_SHIFT)
#define  SERP_SHRR_HR0_INCONFIG_REPLACE_VAL(reg,val) (((reg) & ~SERP_SHRR_HR0_INCONFIG_MASK) | (((uint32_t)val) << SERP_SHRR_HR0_INCONFIG_SHIFT))

//====================================================================
//Register: SERP Blank Threshold A Register (SBTAR)
/** \brief This register sets the threshold values for contone channels 0 and 1. For a given line, if all contone values for each channel are equal to or below their respective thresholds, the line is tagged as being blank.*/
//====================================================================

#define  SERP_SBTAR_RESERVED1_MASK 0xfc000000
#define  SERP_SBTAR_RESERVED1_SHIFT 26
#define  SERP_SBTAR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SBTAR_RESERVED1_MASK) >> SERP_SBTAR_RESERVED1_SHIFT)
#define  SERP_SBTAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTAR_RESERVED1_MASK) | (((uint32_t)val) << SERP_SBTAR_RESERVED1_SHIFT))

#define  SERP_SBTAR_CH1_BLANKTHRESH_MASK 0x3ff0000
#define  SERP_SBTAR_CH1_BLANKTHRESH_SHIFT 16
#define  SERP_SBTAR_CH1_BLANKTHRESH_MASK_SHIFT(reg) (((reg) & SERP_SBTAR_CH1_BLANKTHRESH_MASK) >> SERP_SBTAR_CH1_BLANKTHRESH_SHIFT)
#define  SERP_SBTAR_CH1_BLANKTHRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTAR_CH1_BLANKTHRESH_MASK) | (((uint32_t)val) << SERP_SBTAR_CH1_BLANKTHRESH_SHIFT))

#define  SERP_SBTAR_RESERVED2_MASK 0xfc00
#define  SERP_SBTAR_RESERVED2_SHIFT 10
#define  SERP_SBTAR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SBTAR_RESERVED2_MASK) >> SERP_SBTAR_RESERVED2_SHIFT)
#define  SERP_SBTAR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTAR_RESERVED2_MASK) | (((uint32_t)val) << SERP_SBTAR_RESERVED2_SHIFT))

#define  SERP_SBTAR_CH0_BLANKTHRESH_MASK 0x3ff
#define  SERP_SBTAR_CH0_BLANKTHRESH_SHIFT 0
#define  SERP_SBTAR_CH0_BLANKTHRESH_MASK_SHIFT(reg) (((reg) & SERP_SBTAR_CH0_BLANKTHRESH_MASK) >> SERP_SBTAR_CH0_BLANKTHRESH_SHIFT)
#define  SERP_SBTAR_CH0_BLANKTHRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTAR_CH0_BLANKTHRESH_MASK) | (((uint32_t)val) << SERP_SBTAR_CH0_BLANKTHRESH_SHIFT))

//====================================================================
//Register: SERP Blank Threshold B Register (SBTBR)
/** \brief This register sets the threshold values for contone channels 2 and 3. For a given line, if all contone values for each channel are equal to or below their respective thresholds, the line is tagged as being blank.*/
//====================================================================

#define  SERP_SBTBR_RESERVED1_MASK 0xfc000000
#define  SERP_SBTBR_RESERVED1_SHIFT 26
#define  SERP_SBTBR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SBTBR_RESERVED1_MASK) >> SERP_SBTBR_RESERVED1_SHIFT)
#define  SERP_SBTBR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTBR_RESERVED1_MASK) | (((uint32_t)val) << SERP_SBTBR_RESERVED1_SHIFT))

#define  SERP_SBTBR_CH3_BLANKTHRESH_MASK 0x3ff0000
#define  SERP_SBTBR_CH3_BLANKTHRESH_SHIFT 16
#define  SERP_SBTBR_CH3_BLANKTHRESH_MASK_SHIFT(reg) (((reg) & SERP_SBTBR_CH3_BLANKTHRESH_MASK) >> SERP_SBTBR_CH3_BLANKTHRESH_SHIFT)
#define  SERP_SBTBR_CH3_BLANKTHRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTBR_CH3_BLANKTHRESH_MASK) | (((uint32_t)val) << SERP_SBTBR_CH3_BLANKTHRESH_SHIFT))

#define  SERP_SBTBR_RESERVED2_MASK 0xfc00
#define  SERP_SBTBR_RESERVED2_SHIFT 10
#define  SERP_SBTBR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SBTBR_RESERVED2_MASK) >> SERP_SBTBR_RESERVED2_SHIFT)
#define  SERP_SBTBR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTBR_RESERVED2_MASK) | (((uint32_t)val) << SERP_SBTBR_RESERVED2_SHIFT))

#define  SERP_SBTBR_CH2_BLANKTHRESH_MASK 0x3ff
#define  SERP_SBTBR_CH2_BLANKTHRESH_SHIFT 0
#define  SERP_SBTBR_CH2_BLANKTHRESH_MASK_SHIFT(reg) (((reg) & SERP_SBTBR_CH2_BLANKTHRESH_MASK) >> SERP_SBTBR_CH2_BLANKTHRESH_SHIFT)
#define  SERP_SBTBR_CH2_BLANKTHRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTBR_CH2_BLANKTHRESH_MASK) | (((uint32_t)val) << SERP_SBTBR_CH2_BLANKTHRESH_SHIFT))

//====================================================================
//Register: SERP Blank Threshold C Register (SBTCR)
/** \brief This register sets the threshold values for contone channels 4 and 5. For a given line, if all contone values for each channel are equal to or below their respective thresholds, the line is tagged as being blank.*/
//====================================================================

#define  SERP_SBTCR_RESERVED1_MASK 0xfc000000
#define  SERP_SBTCR_RESERVED1_SHIFT 26
#define  SERP_SBTCR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SBTCR_RESERVED1_MASK) >> SERP_SBTCR_RESERVED1_SHIFT)
#define  SERP_SBTCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTCR_RESERVED1_MASK) | (((uint32_t)val) << SERP_SBTCR_RESERVED1_SHIFT))

#define  SERP_SBTCR_CH5_BLANKTHRESH_MASK 0x3ff0000
#define  SERP_SBTCR_CH5_BLANKTHRESH_SHIFT 16
#define  SERP_SBTCR_CH5_BLANKTHRESH_MASK_SHIFT(reg) (((reg) & SERP_SBTCR_CH5_BLANKTHRESH_MASK) >> SERP_SBTCR_CH5_BLANKTHRESH_SHIFT)
#define  SERP_SBTCR_CH5_BLANKTHRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTCR_CH5_BLANKTHRESH_MASK) | (((uint32_t)val) << SERP_SBTCR_CH5_BLANKTHRESH_SHIFT))

#define  SERP_SBTCR_RESERVED2_MASK 0xfc00
#define  SERP_SBTCR_RESERVED2_SHIFT 10
#define  SERP_SBTCR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SBTCR_RESERVED2_MASK) >> SERP_SBTCR_RESERVED2_SHIFT)
#define  SERP_SBTCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTCR_RESERVED2_MASK) | (((uint32_t)val) << SERP_SBTCR_RESERVED2_SHIFT))

#define  SERP_SBTCR_CH4_BLANKTHRESH_MASK 0x3ff
#define  SERP_SBTCR_CH4_BLANKTHRESH_SHIFT 0
#define  SERP_SBTCR_CH4_BLANKTHRESH_MASK_SHIFT(reg) (((reg) & SERP_SBTCR_CH4_BLANKTHRESH_MASK) >> SERP_SBTCR_CH4_BLANKTHRESH_SHIFT)
#define  SERP_SBTCR_CH4_BLANKTHRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBTCR_CH4_BLANKTHRESH_MASK) | (((uint32_t)val) << SERP_SBTCR_CH4_BLANKTHRESH_SHIFT))

//====================================================================
//Register: SERP Scale Register (SSR)
/** \brief This register allows for independent horizontal and vertical integer scaling for both the contone and text data. The contone and text data must have matching resolutions after leaving SERP.*/
//====================================================================

#define  SERP_SSR_RESERVED1_MASK 0xe0000000
#define  SERP_SSR_RESERVED1_SHIFT 29
#define  SERP_SSR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SSR_RESERVED1_MASK) >> SERP_SSR_RESERVED1_SHIFT)
#define  SERP_SSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_RESERVED1_MASK) | (((uint32_t)val) << SERP_SSR_RESERVED1_SHIFT))

#define  SERP_SSR_TEXT_VSCALE_MASK 0x1f000000
#define  SERP_SSR_TEXT_VSCALE_SHIFT 24
#define  SERP_SSR_TEXT_VSCALE_MASK_SHIFT(reg) (((reg) & SERP_SSR_TEXT_VSCALE_MASK) >> SERP_SSR_TEXT_VSCALE_SHIFT)
#define  SERP_SSR_TEXT_VSCALE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_TEXT_VSCALE_MASK) | (((uint32_t)val) << SERP_SSR_TEXT_VSCALE_SHIFT))

#define  SERP_SSR_RESERVED2_MASK 0xe00000
#define  SERP_SSR_RESERVED2_SHIFT 21
#define  SERP_SSR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SSR_RESERVED2_MASK) >> SERP_SSR_RESERVED2_SHIFT)
#define  SERP_SSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_RESERVED2_MASK) | (((uint32_t)val) << SERP_SSR_RESERVED2_SHIFT))

#define  SERP_SSR_TEXT_HSCALE_MASK 0x1f0000
#define  SERP_SSR_TEXT_HSCALE_SHIFT 16
#define  SERP_SSR_TEXT_HSCALE_MASK_SHIFT(reg) (((reg) & SERP_SSR_TEXT_HSCALE_MASK) >> SERP_SSR_TEXT_HSCALE_SHIFT)
#define  SERP_SSR_TEXT_HSCALE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_TEXT_HSCALE_MASK) | (((uint32_t)val) << SERP_SSR_TEXT_HSCALE_SHIFT))

#define  SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_MASK 0x8000
#define  SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_SHIFT 15
#define  SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_MASK_SHIFT(reg) (((reg) & SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_MASK) >> SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_SHIFT)
#define  SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_MASK) | (((uint32_t)val) << SERP_SSR_CONTONE_VSCALE_BILINEAR_EN_SHIFT))

#define  SERP_SSR_RESERVED3_MASK 0x6000
#define  SERP_SSR_RESERVED3_SHIFT 13
#define  SERP_SSR_RESERVED3_MASK_SHIFT(reg) (((reg) & SERP_SSR_RESERVED3_MASK) >> SERP_SSR_RESERVED3_SHIFT)
#define  SERP_SSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_RESERVED3_MASK) | (((uint32_t)val) << SERP_SSR_RESERVED3_SHIFT))

#define  SERP_SSR_CONTONE_VSCALE_MASK 0x1f00
#define  SERP_SSR_CONTONE_VSCALE_SHIFT 8
#define  SERP_SSR_CONTONE_VSCALE_MASK_SHIFT(reg) (((reg) & SERP_SSR_CONTONE_VSCALE_MASK) >> SERP_SSR_CONTONE_VSCALE_SHIFT)
#define  SERP_SSR_CONTONE_VSCALE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_CONTONE_VSCALE_MASK) | (((uint32_t)val) << SERP_SSR_CONTONE_VSCALE_SHIFT))

#define  SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_MASK 0x80
#define  SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_SHIFT 7
#define  SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_MASK_SHIFT(reg) (((reg) & SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_MASK) >> SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_SHIFT)
#define  SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_MASK) | (((uint32_t)val) << SERP_SSR_CONTONE_HSCALE_BILINEAR_EN_SHIFT))

#define  SERP_SSR_RESERVED4_MASK 0x60
#define  SERP_SSR_RESERVED4_SHIFT 5
#define  SERP_SSR_RESERVED4_MASK_SHIFT(reg) (((reg) & SERP_SSR_RESERVED4_MASK) >> SERP_SSR_RESERVED4_SHIFT)
#define  SERP_SSR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_RESERVED4_MASK) | (((uint32_t)val) << SERP_SSR_RESERVED4_SHIFT))

#define  SERP_SSR_CONTONE_HSCALE_MASK 0x1f
#define  SERP_SSR_CONTONE_HSCALE_SHIFT 0
#define  SERP_SSR_CONTONE_HSCALE_MASK_SHIFT(reg) (((reg) & SERP_SSR_CONTONE_HSCALE_MASK) >> SERP_SSR_CONTONE_HSCALE_SHIFT)
#define  SERP_SSR_CONTONE_HSCALE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSR_CONTONE_HSCALE_MASK) | (((uint32_t)val) << SERP_SSR_CONTONE_HSCALE_SHIFT))

//====================================================================
//Register: SERP Text Merge Register (STMCR)
/** \brief This register allows for configuration of the text-merge operation. The location of the incoming text is specified, as is the bit depth and the mode for text merging.*/
//====================================================================

#define  SERP_STMCR_RESERVED1_MASK 0xffff0000
#define  SERP_STMCR_RESERVED1_SHIFT 16
#define  SERP_STMCR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_STMCR_RESERVED1_MASK) >> SERP_STMCR_RESERVED1_SHIFT)
#define  SERP_STMCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMCR_RESERVED1_MASK) | (((uint32_t)val) << SERP_STMCR_RESERVED1_SHIFT))

#define  SERP_STMCR_TEXT_THRESHOLD_MASK 0xf000
#define  SERP_STMCR_TEXT_THRESHOLD_SHIFT 12
#define  SERP_STMCR_TEXT_THRESHOLD_MASK_SHIFT(reg) (((reg) & SERP_STMCR_TEXT_THRESHOLD_MASK) >> SERP_STMCR_TEXT_THRESHOLD_SHIFT)
#define  SERP_STMCR_TEXT_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMCR_TEXT_THRESHOLD_MASK) | (((uint32_t)val) << SERP_STMCR_TEXT_THRESHOLD_SHIFT))

#define  SERP_STMCR_RESERVED2_MASK 0xe00
#define  SERP_STMCR_RESERVED2_SHIFT 9
#define  SERP_STMCR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_STMCR_RESERVED2_MASK) >> SERP_STMCR_RESERVED2_SHIFT)
#define  SERP_STMCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMCR_RESERVED2_MASK) | (((uint32_t)val) << SERP_STMCR_RESERVED2_SHIFT))

#define  SERP_STMCR_TEXT_REPLACE_MODE_MASK 0x100
#define  SERP_STMCR_TEXT_REPLACE_MODE_SHIFT 8
#define  SERP_STMCR_TEXT_REPLACE_MODE_MASK_SHIFT(reg) (((reg) & SERP_STMCR_TEXT_REPLACE_MODE_MASK) >> SERP_STMCR_TEXT_REPLACE_MODE_SHIFT)
#define  SERP_STMCR_TEXT_REPLACE_MODE_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMCR_TEXT_REPLACE_MODE_MASK) | (((uint32_t)val) << SERP_STMCR_TEXT_REPLACE_MODE_SHIFT))

#define  SERP_STMCR_RESERVED3_MASK 0xf0
#define  SERP_STMCR_RESERVED3_SHIFT 4
#define  SERP_STMCR_RESERVED3_MASK_SHIFT(reg) (((reg) & SERP_STMCR_RESERVED3_MASK) >> SERP_STMCR_RESERVED3_SHIFT)
#define  SERP_STMCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMCR_RESERVED3_MASK) | (((uint32_t)val) << SERP_STMCR_RESERVED3_SHIFT))

#define  SERP_STMCR_TEXT_SOURCE_MASK 0xf
#define  SERP_STMCR_TEXT_SOURCE_SHIFT 0
#define  SERP_STMCR_TEXT_SOURCE_MASK_SHIFT(reg) (((reg) & SERP_STMCR_TEXT_SOURCE_MASK) >> SERP_STMCR_TEXT_SOURCE_SHIFT)
#define  SERP_STMCR_TEXT_SOURCE_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMCR_TEXT_SOURCE_MASK) | (((uint32_t)val) << SERP_STMCR_TEXT_SOURCE_SHIFT))

//====================================================================
//Register: SERP Text Mapping A Register (STMAR)
/** \brief Incoming text data may be 1-bpp or 2-bpp but needs to be converted to 10-bpp. An incoming "0" or "1" on the text channel will be translated to the 10-bit values programmed here. Note that these are the values that will be used as the inputs to the text-merge process.
      4-bpp text will always be mapped to the following: {text[3:0],text[3:0],text[3:2]}.*/
//====================================================================

#define  SERP_STMAR_RESERVED1_MASK 0xfc000000
#define  SERP_STMAR_RESERVED1_SHIFT 26
#define  SERP_STMAR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_STMAR_RESERVED1_MASK) >> SERP_STMAR_RESERVED1_SHIFT)
#define  SERP_STMAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMAR_RESERVED1_MASK) | (((uint32_t)val) << SERP_STMAR_RESERVED1_SHIFT))

#define  SERP_STMAR_TEXT1_MAPPING_MASK 0x3ff0000
#define  SERP_STMAR_TEXT1_MAPPING_SHIFT 16
#define  SERP_STMAR_TEXT1_MAPPING_MASK_SHIFT(reg) (((reg) & SERP_STMAR_TEXT1_MAPPING_MASK) >> SERP_STMAR_TEXT1_MAPPING_SHIFT)
#define  SERP_STMAR_TEXT1_MAPPING_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMAR_TEXT1_MAPPING_MASK) | (((uint32_t)val) << SERP_STMAR_TEXT1_MAPPING_SHIFT))

#define  SERP_STMAR_RESERVED2_MASK 0xfc00
#define  SERP_STMAR_RESERVED2_SHIFT 10
#define  SERP_STMAR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_STMAR_RESERVED2_MASK) >> SERP_STMAR_RESERVED2_SHIFT)
#define  SERP_STMAR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMAR_RESERVED2_MASK) | (((uint32_t)val) << SERP_STMAR_RESERVED2_SHIFT))

#define  SERP_STMAR_TEXT0_MAPPING_MASK 0x3ff
#define  SERP_STMAR_TEXT0_MAPPING_SHIFT 0
#define  SERP_STMAR_TEXT0_MAPPING_MASK_SHIFT(reg) (((reg) & SERP_STMAR_TEXT0_MAPPING_MASK) >> SERP_STMAR_TEXT0_MAPPING_SHIFT)
#define  SERP_STMAR_TEXT0_MAPPING_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMAR_TEXT0_MAPPING_MASK) | (((uint32_t)val) << SERP_STMAR_TEXT0_MAPPING_SHIFT))

//====================================================================
//Register: SERP Text Mapping B Register (STMBR)
/** \brief Incoming text data may be 1-bpp or 2-bpp but needs to be converted to 10-bpp. An incoming "2" or "3" on the text channel will be translated to the 10-bit values programmed here. Note that these are the values that will be used as the inputs to the text-merge process.
      4-bpp text will always be mapped to the following: {text[3:0],text[3:0],text[3:2]}.*/
//====================================================================

#define  SERP_STMBR_RESERVED1_MASK 0xfc000000
#define  SERP_STMBR_RESERVED1_SHIFT 26
#define  SERP_STMBR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_STMBR_RESERVED1_MASK) >> SERP_STMBR_RESERVED1_SHIFT)
#define  SERP_STMBR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMBR_RESERVED1_MASK) | (((uint32_t)val) << SERP_STMBR_RESERVED1_SHIFT))

#define  SERP_STMBR_TEXT3_MAPPING_MASK 0x3ff0000
#define  SERP_STMBR_TEXT3_MAPPING_SHIFT 16
#define  SERP_STMBR_TEXT3_MAPPING_MASK_SHIFT(reg) (((reg) & SERP_STMBR_TEXT3_MAPPING_MASK) >> SERP_STMBR_TEXT3_MAPPING_SHIFT)
#define  SERP_STMBR_TEXT3_MAPPING_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMBR_TEXT3_MAPPING_MASK) | (((uint32_t)val) << SERP_STMBR_TEXT3_MAPPING_SHIFT))

#define  SERP_STMBR_RESERVED2_MASK 0xfc00
#define  SERP_STMBR_RESERVED2_SHIFT 10
#define  SERP_STMBR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_STMBR_RESERVED2_MASK) >> SERP_STMBR_RESERVED2_SHIFT)
#define  SERP_STMBR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMBR_RESERVED2_MASK) | (((uint32_t)val) << SERP_STMBR_RESERVED2_SHIFT))

#define  SERP_STMBR_TEXT2_MAPPING_MASK 0x3ff
#define  SERP_STMBR_TEXT2_MAPPING_SHIFT 0
#define  SERP_STMBR_TEXT2_MAPPING_MASK_SHIFT(reg) (((reg) & SERP_STMBR_TEXT2_MAPPING_MASK) >> SERP_STMBR_TEXT2_MAPPING_SHIFT)
#define  SERP_STMBR_TEXT2_MAPPING_REPLACE_VAL(reg,val) (((reg) & ~SERP_STMBR_TEXT2_MAPPING_MASK) | (((uint32_t)val) << SERP_STMBR_TEXT2_MAPPING_SHIFT))

//====================================================================
//Register: SERP Black Core Register (SBCR)
/** \brief This register configures the operation of the black core function.*/
//====================================================================

#define  SERP_SBCR_RESERVED1_MASK 0xe0000000
#define  SERP_SBCR_RESERVED1_SHIFT 29
#define  SERP_SBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SBCR_RESERVED1_MASK) >> SERP_SBCR_RESERVED1_SHIFT)
#define  SERP_SBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_RESERVED1_MASK) | (((uint32_t)val) << SERP_SBCR_RESERVED1_SHIFT))

#define  SERP_SBCR_BLACK_CORE_CH5_EN_MASK 0x10000000
#define  SERP_SBCR_BLACK_CORE_CH5_EN_SHIFT 28
#define  SERP_SBCR_BLACK_CORE_CH5_EN_MASK_SHIFT(reg) (((reg) & SERP_SBCR_BLACK_CORE_CH5_EN_MASK) >> SERP_SBCR_BLACK_CORE_CH5_EN_SHIFT)
#define  SERP_SBCR_BLACK_CORE_CH5_EN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_BLACK_CORE_CH5_EN_MASK) | (((uint32_t)val) << SERP_SBCR_BLACK_CORE_CH5_EN_SHIFT))

#define  SERP_SBCR_RESERVED2_MASK 0xe000000
#define  SERP_SBCR_RESERVED2_SHIFT 25
#define  SERP_SBCR_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SBCR_RESERVED2_MASK) >> SERP_SBCR_RESERVED2_SHIFT)
#define  SERP_SBCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_RESERVED2_MASK) | (((uint32_t)val) << SERP_SBCR_RESERVED2_SHIFT))

#define  SERP_SBCR_BLACK_CORE_CH3_EN_MASK 0x1000000
#define  SERP_SBCR_BLACK_CORE_CH3_EN_SHIFT 24
#define  SERP_SBCR_BLACK_CORE_CH3_EN_MASK_SHIFT(reg) (((reg) & SERP_SBCR_BLACK_CORE_CH3_EN_MASK) >> SERP_SBCR_BLACK_CORE_CH3_EN_SHIFT)
#define  SERP_SBCR_BLACK_CORE_CH3_EN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_BLACK_CORE_CH3_EN_MASK) | (((uint32_t)val) << SERP_SBCR_BLACK_CORE_CH3_EN_SHIFT))

#define  SERP_SBCR_RESERVED3_MASK 0xe00000
#define  SERP_SBCR_RESERVED3_SHIFT 21
#define  SERP_SBCR_RESERVED3_MASK_SHIFT(reg) (((reg) & SERP_SBCR_RESERVED3_MASK) >> SERP_SBCR_RESERVED3_SHIFT)
#define  SERP_SBCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_RESERVED3_MASK) | (((uint32_t)val) << SERP_SBCR_RESERVED3_SHIFT))

#define  SERP_SBCR_BLACK_CORE_CH2_EN_MASK 0x100000
#define  SERP_SBCR_BLACK_CORE_CH2_EN_SHIFT 20
#define  SERP_SBCR_BLACK_CORE_CH2_EN_MASK_SHIFT(reg) (((reg) & SERP_SBCR_BLACK_CORE_CH2_EN_MASK) >> SERP_SBCR_BLACK_CORE_CH2_EN_SHIFT)
#define  SERP_SBCR_BLACK_CORE_CH2_EN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_BLACK_CORE_CH2_EN_MASK) | (((uint32_t)val) << SERP_SBCR_BLACK_CORE_CH2_EN_SHIFT))

#define  SERP_SBCR_RESERVED4_MASK 0xe0000
#define  SERP_SBCR_RESERVED4_SHIFT 17
#define  SERP_SBCR_RESERVED4_MASK_SHIFT(reg) (((reg) & SERP_SBCR_RESERVED4_MASK) >> SERP_SBCR_RESERVED4_SHIFT)
#define  SERP_SBCR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_RESERVED4_MASK) | (((uint32_t)val) << SERP_SBCR_RESERVED4_SHIFT))

#define  SERP_SBCR_BLACK_CORE_CH1_EN_MASK 0x10000
#define  SERP_SBCR_BLACK_CORE_CH1_EN_SHIFT 16
#define  SERP_SBCR_BLACK_CORE_CH1_EN_MASK_SHIFT(reg) (((reg) & SERP_SBCR_BLACK_CORE_CH1_EN_MASK) >> SERP_SBCR_BLACK_CORE_CH1_EN_SHIFT)
#define  SERP_SBCR_BLACK_CORE_CH1_EN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_BLACK_CORE_CH1_EN_MASK) | (((uint32_t)val) << SERP_SBCR_BLACK_CORE_CH1_EN_SHIFT))

#define  SERP_SBCR_RESERVED5_MASK 0xe000
#define  SERP_SBCR_RESERVED5_SHIFT 13
#define  SERP_SBCR_RESERVED5_MASK_SHIFT(reg) (((reg) & SERP_SBCR_RESERVED5_MASK) >> SERP_SBCR_RESERVED5_SHIFT)
#define  SERP_SBCR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_RESERVED5_MASK) | (((uint32_t)val) << SERP_SBCR_RESERVED5_SHIFT))

#define  SERP_SBCR_BLACK_CORE_CH0_EN_MASK 0x1000
#define  SERP_SBCR_BLACK_CORE_CH0_EN_SHIFT 12
#define  SERP_SBCR_BLACK_CORE_CH0_EN_MASK_SHIFT(reg) (((reg) & SERP_SBCR_BLACK_CORE_CH0_EN_MASK) >> SERP_SBCR_BLACK_CORE_CH0_EN_SHIFT)
#define  SERP_SBCR_BLACK_CORE_CH0_EN_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_BLACK_CORE_CH0_EN_MASK) | (((uint32_t)val) << SERP_SBCR_BLACK_CORE_CH0_EN_SHIFT))

#define  SERP_SBCR_RESERVED6_MASK 0xc00
#define  SERP_SBCR_RESERVED6_SHIFT 10
#define  SERP_SBCR_RESERVED6_MASK_SHIFT(reg) (((reg) & SERP_SBCR_RESERVED6_MASK) >> SERP_SBCR_RESERVED6_SHIFT)
#define  SERP_SBCR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_RESERVED6_MASK) | (((uint32_t)val) << SERP_SBCR_RESERVED6_SHIFT))

#define  SERP_SBCR_BLACK_CORE_THRESH_MASK 0x3ff
#define  SERP_SBCR_BLACK_CORE_THRESH_SHIFT 0
#define  SERP_SBCR_BLACK_CORE_THRESH_MASK_SHIFT(reg) (((reg) & SERP_SBCR_BLACK_CORE_THRESH_MASK) >> SERP_SBCR_BLACK_CORE_THRESH_SHIFT)
#define  SERP_SBCR_BLACK_CORE_THRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SBCR_BLACK_CORE_THRESH_MASK) | (((uint32_t)val) << SERP_SBCR_BLACK_CORE_THRESH_SHIFT))

//====================================================================
//Register: SERP Edge Strength Configuration A (SECA)
/** \brief This register is the first of two registers that contains edge strength configuration bits.*/
//====================================================================

#define  SERP_SECA_RESERVED1_MASK 0xfffe0000
#define  SERP_SECA_RESERVED1_SHIFT 17
#define  SERP_SECA_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SECA_RESERVED1_MASK) >> SERP_SECA_RESERVED1_SHIFT)
#define  SERP_SECA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_RESERVED1_MASK) | (((uint32_t)val) << SERP_SECA_RESERVED1_SHIFT))

#define  SERP_SECA_VSCALE_MASK 0x1f000
#define  SERP_SECA_VSCALE_SHIFT 12
#define  SERP_SECA_VSCALE_MASK_SHIFT(reg) (((reg) & SERP_SECA_VSCALE_MASK) >> SERP_SECA_VSCALE_SHIFT)
#define  SERP_SECA_VSCALE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_VSCALE_MASK) | (((uint32_t)val) << SERP_SECA_VSCALE_SHIFT))

#define  SERP_SECA_RESERVED2_MASK 0xe00
#define  SERP_SECA_RESERVED2_SHIFT 9
#define  SERP_SECA_RESERVED2_MASK_SHIFT(reg) (((reg) & SERP_SECA_RESERVED2_MASK) >> SERP_SECA_RESERVED2_SHIFT)
#define  SERP_SECA_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_RESERVED2_MASK) | (((uint32_t)val) << SERP_SECA_RESERVED2_SHIFT))

#define  SERP_SECA_HSCALE_MASK 0x1f0
#define  SERP_SECA_HSCALE_SHIFT 4
#define  SERP_SECA_HSCALE_MASK_SHIFT(reg) (((reg) & SERP_SECA_HSCALE_MASK) >> SERP_SECA_HSCALE_SHIFT)
#define  SERP_SECA_HSCALE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_HSCALE_MASK) | (((uint32_t)val) << SERP_SECA_HSCALE_SHIFT))

#define  SERP_SECA_EDGE_OUT_MSB_MASK 0x8
#define  SERP_SECA_EDGE_OUT_MSB_SHIFT 3
#define  SERP_SECA_EDGE_OUT_MSB_MASK_SHIFT(reg) (((reg) & SERP_SECA_EDGE_OUT_MSB_MASK) >> SERP_SECA_EDGE_OUT_MSB_SHIFT)
#define  SERP_SECA_EDGE_OUT_MSB_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_EDGE_OUT_MSB_MASK) | (((uint32_t)val) << SERP_SECA_EDGE_OUT_MSB_SHIFT))

#define  SERP_SECA_EDGE_IN_MSB_MASK 0x4
#define  SERP_SECA_EDGE_IN_MSB_SHIFT 2
#define  SERP_SECA_EDGE_IN_MSB_MASK_SHIFT(reg) (((reg) & SERP_SECA_EDGE_IN_MSB_MASK) >> SERP_SECA_EDGE_IN_MSB_SHIFT)
#define  SERP_SECA_EDGE_IN_MSB_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_EDGE_IN_MSB_MASK) | (((uint32_t)val) << SERP_SECA_EDGE_IN_MSB_SHIFT))

#define  SERP_SECA_EDGE_ZERO_MASK 0x2
#define  SERP_SECA_EDGE_ZERO_SHIFT 1
#define  SERP_SECA_EDGE_ZERO_MASK_SHIFT(reg) (((reg) & SERP_SECA_EDGE_ZERO_MASK) >> SERP_SECA_EDGE_ZERO_SHIFT)
#define  SERP_SECA_EDGE_ZERO_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_EDGE_ZERO_MASK) | (((uint32_t)val) << SERP_SECA_EDGE_ZERO_SHIFT))

#define  SERP_SECA_INVERT_DATA_MASK 0x1
#define  SERP_SECA_INVERT_DATA_SHIFT 0
#define  SERP_SECA_INVERT_DATA_MASK_SHIFT(reg) (((reg) & SERP_SECA_INVERT_DATA_MASK) >> SERP_SECA_INVERT_DATA_SHIFT)
#define  SERP_SECA_INVERT_DATA_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECA_INVERT_DATA_MASK) | (((uint32_t)val) << SERP_SECA_INVERT_DATA_SHIFT))

//====================================================================
//Register: SERP Edge Strength Configuration B (SECB)
/** \brief This register is the second of two registers that contains edge strength configuration bits.*/
//====================================================================

#define  SERP_SECB_EDGE_THRESH_MASK 0xfff00000
#define  SERP_SECB_EDGE_THRESH_SHIFT 20
#define  SERP_SECB_EDGE_THRESH_MASK_SHIFT(reg) (((reg) & SERP_SECB_EDGE_THRESH_MASK) >> SERP_SECB_EDGE_THRESH_SHIFT)
#define  SERP_SECB_EDGE_THRESH_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECB_EDGE_THRESH_MASK) | (((uint32_t)val) << SERP_SECB_EDGE_THRESH_SHIFT))

#define  SERP_SECB_EDGE_SCALE_MASK 0xff000
#define  SERP_SECB_EDGE_SCALE_SHIFT 12
#define  SERP_SECB_EDGE_SCALE_MASK_SHIFT(reg) (((reg) & SERP_SECB_EDGE_SCALE_MASK) >> SERP_SECB_EDGE_SCALE_SHIFT)
#define  SERP_SECB_EDGE_SCALE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECB_EDGE_SCALE_MASK) | (((uint32_t)val) << SERP_SECB_EDGE_SCALE_SHIFT))

#define  SERP_SECB_EDGE_OFFSET_MASK 0xfff
#define  SERP_SECB_EDGE_OFFSET_SHIFT 0
#define  SERP_SECB_EDGE_OFFSET_MASK_SHIFT(reg) (((reg) & SERP_SECB_EDGE_OFFSET_MASK) >> SERP_SECB_EDGE_OFFSET_SHIFT)
#define  SERP_SECB_EDGE_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~SERP_SECB_EDGE_OFFSET_MASK) | (((uint32_t)val) << SERP_SECB_EDGE_OFFSET_SHIFT))

//====================================================================
//Register: Text Line Buffer Access Control Register (STLBC)
/** \brief This register enables/disables access to the Text Line Buffer SRAMs.*/
//====================================================================

#define  SERP_STLBC_RESERVED1_MASK 0xfffffffc
#define  SERP_STLBC_RESERVED1_SHIFT 2
#define  SERP_STLBC_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_STLBC_RESERVED1_MASK) >> SERP_STLBC_RESERVED1_SHIFT)
#define  SERP_STLBC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBC_RESERVED1_MASK) | (((uint32_t)val) << SERP_STLBC_RESERVED1_SHIFT))

#define  SERP_STLBC_TLB_INCR_ADDR_MODE_MASK 0x2
#define  SERP_STLBC_TLB_INCR_ADDR_MODE_SHIFT 1
#define  SERP_STLBC_TLB_INCR_ADDR_MODE_MASK_SHIFT(reg) (((reg) & SERP_STLBC_TLB_INCR_ADDR_MODE_MASK) >> SERP_STLBC_TLB_INCR_ADDR_MODE_SHIFT)
#define  SERP_STLBC_TLB_INCR_ADDR_MODE_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBC_TLB_INCR_ADDR_MODE_MASK) | (((uint32_t)val) << SERP_STLBC_TLB_INCR_ADDR_MODE_SHIFT))

#define  SERP_STLBC_TLB_RNW_MASK 0x1
#define  SERP_STLBC_TLB_RNW_SHIFT 0
#define  SERP_STLBC_TLB_RNW_MASK_SHIFT(reg) (((reg) & SERP_STLBC_TLB_RNW_MASK) >> SERP_STLBC_TLB_RNW_SHIFT)
#define  SERP_STLBC_TLB_RNW_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBC_TLB_RNW_MASK) | (((uint32_t)val) << SERP_STLBC_TLB_RNW_SHIFT))

//====================================================================
//Register: Text Line Buffer Address Register (STLBA)
/** \brief Write of the Text Line Buffer Address to access.*/
//====================================================================

#define  SERP_STLBA_RESERVED1_MASK 0xfffff800
#define  SERP_STLBA_RESERVED1_SHIFT 11
#define  SERP_STLBA_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_STLBA_RESERVED1_MASK) >> SERP_STLBA_RESERVED1_SHIFT)
#define  SERP_STLBA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBA_RESERVED1_MASK) | (((uint32_t)val) << SERP_STLBA_RESERVED1_SHIFT))

#define  SERP_STLBA_TLB_ADDR_MASK 0x7ff
#define  SERP_STLBA_TLB_ADDR_SHIFT 0
#define  SERP_STLBA_TLB_ADDR_MASK_SHIFT(reg) (((reg) & SERP_STLBA_TLB_ADDR_MASK) >> SERP_STLBA_TLB_ADDR_SHIFT)
#define  SERP_STLBA_TLB_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBA_TLB_ADDR_MASK) | (((uint32_t)val) << SERP_STLBA_TLB_ADDR_SHIFT))

//====================================================================
//Register: Text Line Buffer Address Status Register (STLBAS)
/** \brief Read of the Text Line Buffer SRAM address being accessed.*/
//====================================================================

#define  SERP_STLBAS_RESERVED1_MASK 0xfffff800
#define  SERP_STLBAS_RESERVED1_SHIFT 11
#define  SERP_STLBAS_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_STLBAS_RESERVED1_MASK) >> SERP_STLBAS_RESERVED1_SHIFT)
#define  SERP_STLBAS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBAS_RESERVED1_MASK) | (((uint32_t)val) << SERP_STLBAS_RESERVED1_SHIFT))

#define  SERP_STLBAS_TLB_ADDR_STATUS_MASK 0x7ff
#define  SERP_STLBAS_TLB_ADDR_STATUS_SHIFT 0
#define  SERP_STLBAS_TLB_ADDR_STATUS_MASK_SHIFT(reg) (((reg) & SERP_STLBAS_TLB_ADDR_STATUS_MASK) >> SERP_STLBAS_TLB_ADDR_STATUS_SHIFT)
#define  SERP_STLBAS_TLB_ADDR_STATUS_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBAS_TLB_ADDR_STATUS_MASK) | (((uint32_t)val) << SERP_STLBAS_TLB_ADDR_STATUS_SHIFT))

//====================================================================
//Register: Text Line Buffer Write Data (STLBWD)
/** \brief For register writes to the Text Line Buffer SRAMs. Both 640x16 SRAMs are written simultaneously.
        When not in "increment address mode", data must be written into STLBWD prior to
        writing the address into STLBA.
        When in "increment address mode", the starting address must be written to STLBA prior to performing consecutive writes to this register.*/
//====================================================================

#define  SERP_STLBWD_TLB_WR_DATA1_MASK 0xffff0000
#define  SERP_STLBWD_TLB_WR_DATA1_SHIFT 16
#define  SERP_STLBWD_TLB_WR_DATA1_MASK_SHIFT(reg) (((reg) & SERP_STLBWD_TLB_WR_DATA1_MASK) >> SERP_STLBWD_TLB_WR_DATA1_SHIFT)
#define  SERP_STLBWD_TLB_WR_DATA1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBWD_TLB_WR_DATA1_MASK) | (((uint32_t)val) << SERP_STLBWD_TLB_WR_DATA1_SHIFT))

#define  SERP_STLBWD_TLB_WR_DATA0_MASK 0xffff
#define  SERP_STLBWD_TLB_WR_DATA0_SHIFT 0
#define  SERP_STLBWD_TLB_WR_DATA0_MASK_SHIFT(reg) (((reg) & SERP_STLBWD_TLB_WR_DATA0_MASK) >> SERP_STLBWD_TLB_WR_DATA0_SHIFT)
#define  SERP_STLBWD_TLB_WR_DATA0_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBWD_TLB_WR_DATA0_MASK) | (((uint32_t)val) << SERP_STLBWD_TLB_WR_DATA0_SHIFT))

//====================================================================
//Register: Text Line Buffer Read Data (STLBRD)
/** \brief For register reads to the Text Line Buffer SRAMs. Both 640x16 SRAMs are read simultaneously.
        When not in "increment address mode", the address must be written to STLBA prior to reading data from this register.
        When in "increment address mode", the starting address must be written to STLBA prior to performing consecutive reads from this register.*/
//====================================================================

#define  SERP_STLBRD_TLB_RD_DATA1_MASK 0xffff0000
#define  SERP_STLBRD_TLB_RD_DATA1_SHIFT 16
#define  SERP_STLBRD_TLB_RD_DATA1_MASK_SHIFT(reg) (((reg) & SERP_STLBRD_TLB_RD_DATA1_MASK) >> SERP_STLBRD_TLB_RD_DATA1_SHIFT)
#define  SERP_STLBRD_TLB_RD_DATA1_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBRD_TLB_RD_DATA1_MASK) | (((uint32_t)val) << SERP_STLBRD_TLB_RD_DATA1_SHIFT))

#define  SERP_STLBRD_TLB_RD_DATA0_MASK 0xffff
#define  SERP_STLBRD_TLB_RD_DATA0_SHIFT 0
#define  SERP_STLBRD_TLB_RD_DATA0_MASK_SHIFT(reg) (((reg) & SERP_STLBRD_TLB_RD_DATA0_MASK) >> SERP_STLBRD_TLB_RD_DATA0_SHIFT)
#define  SERP_STLBRD_TLB_RD_DATA0_REPLACE_VAL(reg,val) (((reg) & ~SERP_STLBRD_TLB_RD_DATA0_MASK) | (((uint32_t)val) << SERP_STLBRD_TLB_RD_DATA0_SHIFT))

//====================================================================
//Register: SERP Save Restore Address (SSRA)
/** \brief This register is provided for the save portion of save/restore functionality. This register is used to set the address of the save/restore register to access. There are currently 11 32-bit registers that must be saved and restored, therefore valid addresses are 0x0 to 0xA.*/
//====================================================================

#define  SERP_SSRA_RESERVED1_MASK 0xfffffff0
#define  SERP_SSRA_RESERVED1_SHIFT 4
#define  SERP_SSRA_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SSRA_RESERVED1_MASK) >> SERP_SSRA_RESERVED1_SHIFT)
#define  SERP_SSRA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSRA_RESERVED1_MASK) | (((uint32_t)val) << SERP_SSRA_RESERVED1_SHIFT))

#define  SERP_SSRA_SR_ADDR_MASK 0xf
#define  SERP_SSRA_SR_ADDR_SHIFT 0
#define  SERP_SSRA_SR_ADDR_MASK_SHIFT(reg) (((reg) & SERP_SSRA_SR_ADDR_MASK) >> SERP_SSRA_SR_ADDR_SHIFT)
#define  SERP_SSRA_SR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSRA_SR_ADDR_MASK) | (((uint32_t)val) << SERP_SSRA_SR_ADDR_SHIFT))

//====================================================================
//Register: SERP Save Restore Read (SSRR)
/** \brief This read-only register is provided for the save portion of save/restore functionality. The "save_restore_mode" bit of the configuration register must be set to a 1 to access this register.*/
//====================================================================

#define  SERP_SSRR_SAVE_VAL_MASK 0xffffffff
#define  SERP_SSRR_SAVE_VAL_SHIFT 0
#define  SERP_SSRR_SAVE_VAL_MASK_SHIFT(reg) (((reg) & SERP_SSRR_SAVE_VAL_MASK) >> SERP_SSRR_SAVE_VAL_SHIFT)
#define  SERP_SSRR_SAVE_VAL_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSRR_SAVE_VAL_MASK) | (((uint32_t)val) << SERP_SSRR_SAVE_VAL_SHIFT))

//====================================================================
//Register: SERP Save Restore Write (SSRW)
/** \brief This write-only register is provided for the restore portion of save/restore functionality.
      The "save_restore_mode" bit of the configuration register must be set to a 1 to access this register.*/
//====================================================================

#define  SERP_SSRW_RESTORE_VAL_MASK 0xffffffff
#define  SERP_SSRW_RESTORE_VAL_SHIFT 0
#define  SERP_SSRW_RESTORE_VAL_MASK_SHIFT(reg) (((reg) & SERP_SSRW_RESTORE_VAL_MASK) >> SERP_SSRW_RESTORE_VAL_SHIFT)
#define  SERP_SSRW_RESTORE_VAL_REPLACE_VAL(reg,val) (((reg) & ~SERP_SSRW_RESTORE_VAL_MASK) | (((uint32_t)val) << SERP_SSRW_RESTORE_VAL_SHIFT))

//====================================================================
//Register: SERP Edge Idle (SEI)
/** \brief This read-only register is provided in order to provide the state of the edge strength window block.*/
//====================================================================

#define  SERP_SEI_RESERVED1_MASK 0xfffffffe
#define  SERP_SEI_RESERVED1_SHIFT 1
#define  SERP_SEI_RESERVED1_MASK_SHIFT(reg) (((reg) & SERP_SEI_RESERVED1_MASK) >> SERP_SEI_RESERVED1_SHIFT)
#define  SERP_SEI_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SERP_SEI_RESERVED1_MASK) | (((uint32_t)val) << SERP_SEI_RESERVED1_SHIFT))

#define  SERP_SEI_EDGE_IDLE_MASK 0x1
#define  SERP_SEI_EDGE_IDLE_SHIFT 0
#define  SERP_SEI_EDGE_IDLE_MASK_SHIFT(reg) (((reg) & SERP_SEI_EDGE_IDLE_MASK) >> SERP_SEI_EDGE_IDLE_SHIFT)
#define  SERP_SEI_EDGE_IDLE_REPLACE_VAL(reg,val) (((reg) & ~SERP_SEI_EDGE_IDLE_MASK) | (((uint32_t)val) << SERP_SEI_EDGE_IDLE_SHIFT))

#endif // SERP
