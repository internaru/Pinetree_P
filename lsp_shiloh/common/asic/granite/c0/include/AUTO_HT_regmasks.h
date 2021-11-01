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
 * \file AUTO_HT_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _AUTO_HT_REGMASKS_H_
#define _AUTO_HT_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: AUTO_HT (AUTO_HT)
/** \brief Register descriptions for the AUTO_HT block.*/
//
//====================================================================

//====================================================================
//Register: AUTO_HT Configuration Register (ACR)
/** \brief This register configures the general operation of the AUTO_HT block. If AUTO_HT is bypassed, the states of the internal state machines are reset, but register settings are preserved. The CPU access bit must be enabled to write the auto halftone LUT, and must be disabled before starting the block.*/
//====================================================================

#define  AUTO_HT_ACR_RESERVED1_MASK 0xffffe000
#define  AUTO_HT_ACR_RESERVED1_SHIFT 13
#define  AUTO_HT_ACR_RESERVED1_MASK_SHIFT(reg) (((reg) & AUTO_HT_ACR_RESERVED1_MASK) >> AUTO_HT_ACR_RESERVED1_SHIFT)
#define  AUTO_HT_ACR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_ACR_RESERVED1_MASK) | (((uint32_t)val) << AUTO_HT_ACR_RESERVED1_SHIFT))

#define  AUTO_HT_ACR_CPU_ACCESS_MASK 0x1000
#define  AUTO_HT_ACR_CPU_ACCESS_SHIFT 12
#define  AUTO_HT_ACR_CPU_ACCESS_MASK_SHIFT(reg) (((reg) & AUTO_HT_ACR_CPU_ACCESS_MASK) >> AUTO_HT_ACR_CPU_ACCESS_SHIFT)
#define  AUTO_HT_ACR_CPU_ACCESS_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_ACR_CPU_ACCESS_MASK) | (((uint32_t)val) << AUTO_HT_ACR_CPU_ACCESS_SHIFT))

#define  AUTO_HT_ACR_RESERVED2_MASK 0xfc0
#define  AUTO_HT_ACR_RESERVED2_SHIFT 6
#define  AUTO_HT_ACR_RESERVED2_MASK_SHIFT(reg) (((reg) & AUTO_HT_ACR_RESERVED2_MASK) >> AUTO_HT_ACR_RESERVED2_SHIFT)
#define  AUTO_HT_ACR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_ACR_RESERVED2_MASK) | (((uint32_t)val) << AUTO_HT_ACR_RESERVED2_SHIFT))

#define  AUTO_HT_ACR_AH1_ENABLE_MASK 0x20
#define  AUTO_HT_ACR_AH1_ENABLE_SHIFT 5
#define  AUTO_HT_ACR_AH1_ENABLE_MASK_SHIFT(reg) (((reg) & AUTO_HT_ACR_AH1_ENABLE_MASK) >> AUTO_HT_ACR_AH1_ENABLE_SHIFT)
#define  AUTO_HT_ACR_AH1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_ACR_AH1_ENABLE_MASK) | (((uint32_t)val) << AUTO_HT_ACR_AH1_ENABLE_SHIFT))

#define  AUTO_HT_ACR_AH0_ENABLE_MASK 0x10
#define  AUTO_HT_ACR_AH0_ENABLE_SHIFT 4
#define  AUTO_HT_ACR_AH0_ENABLE_MASK_SHIFT(reg) (((reg) & AUTO_HT_ACR_AH0_ENABLE_MASK) >> AUTO_HT_ACR_AH0_ENABLE_SHIFT)
#define  AUTO_HT_ACR_AH0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_ACR_AH0_ENABLE_MASK) | (((uint32_t)val) << AUTO_HT_ACR_AH0_ENABLE_SHIFT))

#define  AUTO_HT_ACR_RESERVED3_MASK 0xe
#define  AUTO_HT_ACR_RESERVED3_SHIFT 1
#define  AUTO_HT_ACR_RESERVED3_MASK_SHIFT(reg) (((reg) & AUTO_HT_ACR_RESERVED3_MASK) >> AUTO_HT_ACR_RESERVED3_SHIFT)
#define  AUTO_HT_ACR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_ACR_RESERVED3_MASK) | (((uint32_t)val) << AUTO_HT_ACR_RESERVED3_SHIFT))

#define  AUTO_HT_ACR_BYPASS_ENABLE_MASK 0x1
#define  AUTO_HT_ACR_BYPASS_ENABLE_SHIFT 0
#define  AUTO_HT_ACR_BYPASS_ENABLE_MASK_SHIFT(reg) (((reg) & AUTO_HT_ACR_BYPASS_ENABLE_MASK) >> AUTO_HT_ACR_BYPASS_ENABLE_SHIFT)
#define  AUTO_HT_ACR_BYPASS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_ACR_BYPASS_ENABLE_MASK) | (((uint32_t)val) << AUTO_HT_ACR_BYPASS_ENABLE_SHIFT))

//====================================================================
//Register: AH Configuration Register (AAHCR)
/** \brief This register configures the operation of the auto halftone (replacement ) function.*/
//====================================================================

#define  AUTO_HT_AAHCR_RESERVED1_MASK 0xfffff000
#define  AUTO_HT_AAHCR_RESERVED1_SHIFT 12
#define  AUTO_HT_AAHCR_RESERVED1_MASK_SHIFT(reg) (((reg) & AUTO_HT_AAHCR_RESERVED1_MASK) >> AUTO_HT_AAHCR_RESERVED1_SHIFT)
#define  AUTO_HT_AAHCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AAHCR_RESERVED1_MASK) | (((uint32_t)val) << AUTO_HT_AAHCR_RESERVED1_SHIFT))

#define  AUTO_HT_AAHCR_AH1_MODE_SELECT_MASK 0xf00
#define  AUTO_HT_AAHCR_AH1_MODE_SELECT_SHIFT 8
#define  AUTO_HT_AAHCR_AH1_MODE_SELECT_MASK_SHIFT(reg) (((reg) & AUTO_HT_AAHCR_AH1_MODE_SELECT_MASK) >> AUTO_HT_AAHCR_AH1_MODE_SELECT_SHIFT)
#define  AUTO_HT_AAHCR_AH1_MODE_SELECT_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AAHCR_AH1_MODE_SELECT_MASK) | (((uint32_t)val) << AUTO_HT_AAHCR_AH1_MODE_SELECT_SHIFT))

#define  AUTO_HT_AAHCR_RESERVED2_MASK 0xf0
#define  AUTO_HT_AAHCR_RESERVED2_SHIFT 4
#define  AUTO_HT_AAHCR_RESERVED2_MASK_SHIFT(reg) (((reg) & AUTO_HT_AAHCR_RESERVED2_MASK) >> AUTO_HT_AAHCR_RESERVED2_SHIFT)
#define  AUTO_HT_AAHCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AAHCR_RESERVED2_MASK) | (((uint32_t)val) << AUTO_HT_AAHCR_RESERVED2_SHIFT))

#define  AUTO_HT_AAHCR_AH0_MODE_SELECT_MASK 0xf
#define  AUTO_HT_AAHCR_AH0_MODE_SELECT_SHIFT 0
#define  AUTO_HT_AAHCR_AH0_MODE_SELECT_MASK_SHIFT(reg) (((reg) & AUTO_HT_AAHCR_AH0_MODE_SELECT_MASK) >> AUTO_HT_AAHCR_AH0_MODE_SELECT_SHIFT)
#define  AUTO_HT_AAHCR_AH0_MODE_SELECT_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AAHCR_AH0_MODE_SELECT_MASK) | (((uint32_t)val) << AUTO_HT_AAHCR_AH0_MODE_SELECT_SHIFT))

//====================================================================
//Register: AUTO_HT Debug 0 Register (AD0R)
/** \brief This register exposes internal states that are used in the operation of autohalftone.*/
//====================================================================

#define  AUTO_HT_AD0R_LUT1_NIBBLE_OUT_MASK 0xf0000000
#define  AUTO_HT_AD0R_LUT1_NIBBLE_OUT_SHIFT 28
#define  AUTO_HT_AD0R_LUT1_NIBBLE_OUT_MASK_SHIFT(reg) (((reg) & AUTO_HT_AD0R_LUT1_NIBBLE_OUT_MASK) >> AUTO_HT_AD0R_LUT1_NIBBLE_OUT_SHIFT)
#define  AUTO_HT_AD0R_LUT1_NIBBLE_OUT_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AD0R_LUT1_NIBBLE_OUT_MASK) | (((uint32_t)val) << AUTO_HT_AD0R_LUT1_NIBBLE_OUT_SHIFT))

#define  AUTO_HT_AD0R_LUT1_FULL_ADDR_MASK 0xfff0000
#define  AUTO_HT_AD0R_LUT1_FULL_ADDR_SHIFT 16
#define  AUTO_HT_AD0R_LUT1_FULL_ADDR_MASK_SHIFT(reg) (((reg) & AUTO_HT_AD0R_LUT1_FULL_ADDR_MASK) >> AUTO_HT_AD0R_LUT1_FULL_ADDR_SHIFT)
#define  AUTO_HT_AD0R_LUT1_FULL_ADDR_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AD0R_LUT1_FULL_ADDR_MASK) | (((uint32_t)val) << AUTO_HT_AD0R_LUT1_FULL_ADDR_SHIFT))

#define  AUTO_HT_AD0R_LUT0_NIBBLE_OUT_MASK 0xf000
#define  AUTO_HT_AD0R_LUT0_NIBBLE_OUT_SHIFT 12
#define  AUTO_HT_AD0R_LUT0_NIBBLE_OUT_MASK_SHIFT(reg) (((reg) & AUTO_HT_AD0R_LUT0_NIBBLE_OUT_MASK) >> AUTO_HT_AD0R_LUT0_NIBBLE_OUT_SHIFT)
#define  AUTO_HT_AD0R_LUT0_NIBBLE_OUT_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AD0R_LUT0_NIBBLE_OUT_MASK) | (((uint32_t)val) << AUTO_HT_AD0R_LUT0_NIBBLE_OUT_SHIFT))

#define  AUTO_HT_AD0R_LUT0_FULL_ADDR_MASK 0xfff
#define  AUTO_HT_AD0R_LUT0_FULL_ADDR_SHIFT 0
#define  AUTO_HT_AD0R_LUT0_FULL_ADDR_MASK_SHIFT(reg) (((reg) & AUTO_HT_AD0R_LUT0_FULL_ADDR_MASK) >> AUTO_HT_AD0R_LUT0_FULL_ADDR_SHIFT)
#define  AUTO_HT_AD0R_LUT0_FULL_ADDR_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_AD0R_LUT0_FULL_ADDR_MASK) | (((uint32_t)val) << AUTO_HT_AD0R_LUT0_FULL_ADDR_SHIFT))

//====================================================================
//Register: AUTO_HT LUT 0 (LUT0)
/** \brief To conserve addressable space, this 4-bit wide x 4096 deep LUT has been converted to a 32-bit wide x 512 deep LUT. As a result, 8 4-bit values are packed little-endian style onto each LUT word. For example, the base address of the LUT contains LUT entries 0-7, with entry 7 occupying bits 31-28, entry 6 in bits 27-24, ... , and entry 0 in bits 3-0.*/
//====================================================================

#define  AUTO_HT_LUT0_LE_PACKED_ENTRY_MASK 0xffffffff
#define  AUTO_HT_LUT0_LE_PACKED_ENTRY_SHIFT 0
#define  AUTO_HT_LUT0_LE_PACKED_ENTRY_MASK_SHIFT(reg) (((reg) & AUTO_HT_LUT0_LE_PACKED_ENTRY_MASK) >> AUTO_HT_LUT0_LE_PACKED_ENTRY_SHIFT)
#define  AUTO_HT_LUT0_LE_PACKED_ENTRY_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_LUT0_LE_PACKED_ENTRY_MASK) | (((uint32_t)val) << AUTO_HT_LUT0_LE_PACKED_ENTRY_SHIFT))

//====================================================================
//Register: AUTO_HT LUT 1 (LUT1)
/** \brief To conserve addressable space, this 4-bit wide x 4096 deep LUT has been converted to a 32-bit wide x 512 deep LUT. As a result, 8 4-bit values are packed little-endian style onto each LUT word. For example, the base address of the LUT contains LUT entries 0-7, with entry 7 occupying bits 31-28, entry 6 in bits 27-24, ... , and entry 0 in bits 3-0.*/
//====================================================================

#define  AUTO_HT_LUT1_LE_PACKED_ENTRY_MASK 0xffffffff
#define  AUTO_HT_LUT1_LE_PACKED_ENTRY_SHIFT 0
#define  AUTO_HT_LUT1_LE_PACKED_ENTRY_MASK_SHIFT(reg) (((reg) & AUTO_HT_LUT1_LE_PACKED_ENTRY_MASK) >> AUTO_HT_LUT1_LE_PACKED_ENTRY_SHIFT)
#define  AUTO_HT_LUT1_LE_PACKED_ENTRY_REPLACE_VAL(reg,val) (((reg) & ~AUTO_HT_LUT1_LE_PACKED_ENTRY_MASK) | (((uint32_t)val) << AUTO_HT_LUT1_LE_PACKED_ENTRY_SHIFT))

#endif // AUTO_HT
