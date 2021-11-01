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
 * \file CSSCG_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CSSCG_REGMASKS_H_
#define _CSSCG_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CSSCG Configuration Registers (CSSCG)
/** \brief Top-level register file for the CSSCG block*/
//
//====================================================================

//====================================================================
//Register: CSSCG Mode Configuration Register (MODE_CONFIG)
/** \brief This register configures the CSSCG modes of operation.*/
//====================================================================

#define  CSSCG_MODE_CONFIG_RESERVED1_MASK 0xffe00000
#define  CSSCG_MODE_CONFIG_RESERVED1_SHIFT 21
#define  CSSCG_MODE_CONFIG_RESERVED1_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_RESERVED1_MASK) >> CSSCG_MODE_CONFIG_RESERVED1_SHIFT)
#define  CSSCG_MODE_CONFIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_RESERVED1_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_RESERVED1_SHIFT))

#define  CSSCG_MODE_CONFIG_MAX_ADDR_MASK 0x1f0000
#define  CSSCG_MODE_CONFIG_MAX_ADDR_SHIFT 16
#define  CSSCG_MODE_CONFIG_MAX_ADDR_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_MAX_ADDR_MASK) >> CSSCG_MODE_CONFIG_MAX_ADDR_SHIFT)
#define  CSSCG_MODE_CONFIG_MAX_ADDR_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_MAX_ADDR_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_MAX_ADDR_SHIFT))

#define  CSSCG_MODE_CONFIG_RESERVED2_MASK 0x8000
#define  CSSCG_MODE_CONFIG_RESERVED2_SHIFT 15
#define  CSSCG_MODE_CONFIG_RESERVED2_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_RESERVED2_MASK) >> CSSCG_MODE_CONFIG_RESERVED2_SHIFT)
#define  CSSCG_MODE_CONFIG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_RESERVED2_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_RESERVED2_SHIFT))

#define  CSSCG_MODE_CONFIG_INTERP_SEGMENTS_MASK 0x7000
#define  CSSCG_MODE_CONFIG_INTERP_SEGMENTS_SHIFT 12
#define  CSSCG_MODE_CONFIG_INTERP_SEGMENTS_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_INTERP_SEGMENTS_MASK) >> CSSCG_MODE_CONFIG_INTERP_SEGMENTS_SHIFT)
#define  CSSCG_MODE_CONFIG_INTERP_SEGMENTS_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_INTERP_SEGMENTS_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_INTERP_SEGMENTS_SHIFT))

#define  CSSCG_MODE_CONFIG_RESERVED3_MASK 0xe00
#define  CSSCG_MODE_CONFIG_RESERVED3_SHIFT 9
#define  CSSCG_MODE_CONFIG_RESERVED3_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_RESERVED3_MASK) >> CSSCG_MODE_CONFIG_RESERVED3_SHIFT)
#define  CSSCG_MODE_CONFIG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_RESERVED3_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_RESERVED3_SHIFT))

#define  CSSCG_MODE_CONFIG_RATE_DIV_M1_MASK 0x1f0
#define  CSSCG_MODE_CONFIG_RATE_DIV_M1_SHIFT 4
#define  CSSCG_MODE_CONFIG_RATE_DIV_M1_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_RATE_DIV_M1_MASK) >> CSSCG_MODE_CONFIG_RATE_DIV_M1_SHIFT)
#define  CSSCG_MODE_CONFIG_RATE_DIV_M1_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_RATE_DIV_M1_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_RATE_DIV_M1_SHIFT))

#define  CSSCG_MODE_CONFIG_RESERVED4_MASK 0xc
#define  CSSCG_MODE_CONFIG_RESERVED4_SHIFT 2
#define  CSSCG_MODE_CONFIG_RESERVED4_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_RESERVED4_MASK) >> CSSCG_MODE_CONFIG_RESERVED4_SHIFT)
#define  CSSCG_MODE_CONFIG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_RESERVED4_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_RESERVED4_SHIFT))

#define  CSSCG_MODE_CONFIG_APPLY_CORRECTION_MASK 0x2
#define  CSSCG_MODE_CONFIG_APPLY_CORRECTION_SHIFT 1
#define  CSSCG_MODE_CONFIG_APPLY_CORRECTION_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_APPLY_CORRECTION_MASK) >> CSSCG_MODE_CONFIG_APPLY_CORRECTION_SHIFT)
#define  CSSCG_MODE_CONFIG_APPLY_CORRECTION_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_APPLY_CORRECTION_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_APPLY_CORRECTION_SHIFT))

#define  CSSCG_MODE_CONFIG_DOWN_SPREAD_MASK 0x1
#define  CSSCG_MODE_CONFIG_DOWN_SPREAD_SHIFT 0
#define  CSSCG_MODE_CONFIG_DOWN_SPREAD_MASK_SHIFT(reg) (((reg) & CSSCG_MODE_CONFIG_DOWN_SPREAD_MASK) >> CSSCG_MODE_CONFIG_DOWN_SPREAD_SHIFT)
#define  CSSCG_MODE_CONFIG_DOWN_SPREAD_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_MODE_CONFIG_DOWN_SPREAD_MASK) | (((uint32_t)val) << CSSCG_MODE_CONFIG_DOWN_SPREAD_SHIFT))

//====================================================================
//Register: CSSCG Halt Configuration Register (HALT_CONFIG)
/** \brief This register defines the place on the dither curve where the block will stop upon a halt request. Note that the block will continue to send offset codes to the PLL*/
//====================================================================

#define  CSSCG_HALT_CONFIG_RESERVED1_MASK 0xfffc0000
#define  CSSCG_HALT_CONFIG_RESERVED1_SHIFT 18
#define  CSSCG_HALT_CONFIG_RESERVED1_MASK_SHIFT(reg) (((reg) & CSSCG_HALT_CONFIG_RESERVED1_MASK) >> CSSCG_HALT_CONFIG_RESERVED1_SHIFT)
#define  CSSCG_HALT_CONFIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_HALT_CONFIG_RESERVED1_MASK) | (((uint32_t)val) << CSSCG_HALT_CONFIG_RESERVED1_SHIFT))

#define  CSSCG_HALT_CONFIG_HALT_PHASE_MASK 0x30000
#define  CSSCG_HALT_CONFIG_HALT_PHASE_SHIFT 16
#define  CSSCG_HALT_CONFIG_HALT_PHASE_MASK_SHIFT(reg) (((reg) & CSSCG_HALT_CONFIG_HALT_PHASE_MASK) >> CSSCG_HALT_CONFIG_HALT_PHASE_SHIFT)
#define  CSSCG_HALT_CONFIG_HALT_PHASE_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_HALT_CONFIG_HALT_PHASE_MASK) | (((uint32_t)val) << CSSCG_HALT_CONFIG_HALT_PHASE_SHIFT))

#define  CSSCG_HALT_CONFIG_RESERVED2_MASK 0xffe0
#define  CSSCG_HALT_CONFIG_RESERVED2_SHIFT 5
#define  CSSCG_HALT_CONFIG_RESERVED2_MASK_SHIFT(reg) (((reg) & CSSCG_HALT_CONFIG_RESERVED2_MASK) >> CSSCG_HALT_CONFIG_RESERVED2_SHIFT)
#define  CSSCG_HALT_CONFIG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_HALT_CONFIG_RESERVED2_MASK) | (((uint32_t)val) << CSSCG_HALT_CONFIG_RESERVED2_SHIFT))

#define  CSSCG_HALT_CONFIG_HALT_ADDR_MASK 0x1f
#define  CSSCG_HALT_CONFIG_HALT_ADDR_SHIFT 0
#define  CSSCG_HALT_CONFIG_HALT_ADDR_MASK_SHIFT(reg) (((reg) & CSSCG_HALT_CONFIG_HALT_ADDR_MASK) >> CSSCG_HALT_CONFIG_HALT_ADDR_SHIFT)
#define  CSSCG_HALT_CONFIG_HALT_ADDR_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_HALT_CONFIG_HALT_ADDR_MASK) | (((uint32_t)val) << CSSCG_HALT_CONFIG_HALT_ADDR_SHIFT))

//====================================================================
//Register: CSSCG Sign Bit Override Register (SB_OVER)
/** \brief This register is used to override the sign bit to the PLL.*/
//====================================================================

#define  CSSCG_SB_OVER_RESERVED1_MASK 0xfffffff0
#define  CSSCG_SB_OVER_RESERVED1_SHIFT 4
#define  CSSCG_SB_OVER_RESERVED1_MASK_SHIFT(reg) (((reg) & CSSCG_SB_OVER_RESERVED1_MASK) >> CSSCG_SB_OVER_RESERVED1_SHIFT)
#define  CSSCG_SB_OVER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_SB_OVER_RESERVED1_MASK) | (((uint32_t)val) << CSSCG_SB_OVER_RESERVED1_SHIFT))

#define  CSSCG_SB_OVER_SB_OVERRIDE_MASK 0xf
#define  CSSCG_SB_OVER_SB_OVERRIDE_SHIFT 0
#define  CSSCG_SB_OVER_SB_OVERRIDE_MASK_SHIFT(reg) (((reg) & CSSCG_SB_OVER_SB_OVERRIDE_MASK) >> CSSCG_SB_OVER_SB_OVERRIDE_SHIFT)
#define  CSSCG_SB_OVER_SB_OVERRIDE_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_SB_OVER_SB_OVERRIDE_MASK) | (((uint32_t)val) << CSSCG_SB_OVER_SB_OVERRIDE_SHIFT))

//====================================================================
//Register: CSSCG Control Register (CONTROL)
/** \brief This register is used to start/stop the CSSCG block.*/
//====================================================================

#define  CSSCG_CONTROL_NRESET_PLL_IF_MASK 0x80000000
#define  CSSCG_CONTROL_NRESET_PLL_IF_SHIFT 31
#define  CSSCG_CONTROL_NRESET_PLL_IF_MASK_SHIFT(reg) (((reg) & CSSCG_CONTROL_NRESET_PLL_IF_MASK) >> CSSCG_CONTROL_NRESET_PLL_IF_SHIFT)
#define  CSSCG_CONTROL_NRESET_PLL_IF_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_CONTROL_NRESET_PLL_IF_MASK) | (((uint32_t)val) << CSSCG_CONTROL_NRESET_PLL_IF_SHIFT))

#define  CSSCG_CONTROL_RESERVED1_MASK 0x7ffffff8
#define  CSSCG_CONTROL_RESERVED1_SHIFT 3
#define  CSSCG_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & CSSCG_CONTROL_RESERVED1_MASK) >> CSSCG_CONTROL_RESERVED1_SHIFT)
#define  CSSCG_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << CSSCG_CONTROL_RESERVED1_SHIFT))

#define  CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_MASK 0x4
#define  CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_SHIFT 2
#define  CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_MASK_SHIFT(reg) (((reg) & CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_MASK) >> CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_SHIFT)
#define  CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_MASK) | (((uint32_t)val) << CSSCG_CONTROL_CSSCG_EXTERNAL_MUX_SEL_SHIFT))

#define  CSSCG_CONTROL_HALT_EN_MASK 0x2
#define  CSSCG_CONTROL_HALT_EN_SHIFT 1
#define  CSSCG_CONTROL_HALT_EN_MASK_SHIFT(reg) (((reg) & CSSCG_CONTROL_HALT_EN_MASK) >> CSSCG_CONTROL_HALT_EN_SHIFT)
#define  CSSCG_CONTROL_HALT_EN_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_CONTROL_HALT_EN_MASK) | (((uint32_t)val) << CSSCG_CONTROL_HALT_EN_SHIFT))

#define  CSSCG_CONTROL_CSSCG_ENABLE_MASK 0x1
#define  CSSCG_CONTROL_CSSCG_ENABLE_SHIFT 0
#define  CSSCG_CONTROL_CSSCG_ENABLE_MASK_SHIFT(reg) (((reg) & CSSCG_CONTROL_CSSCG_ENABLE_MASK) >> CSSCG_CONTROL_CSSCG_ENABLE_SHIFT)
#define  CSSCG_CONTROL_CSSCG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_CONTROL_CSSCG_ENABLE_MASK) | (((uint32_t)val) << CSSCG_CONTROL_CSSCG_ENABLE_SHIFT))

//====================================================================
//Register: CSSCG Status Register (STATUS)
/** \brief This register provides the internal status of the block. It can be used to determine when a halt has completed. It can also be used for debug purposes.*/
//====================================================================

#define  CSSCG_STATUS_SYNCH_FIFO_EMPTY_MASK 0x80000000
#define  CSSCG_STATUS_SYNCH_FIFO_EMPTY_SHIFT 31
#define  CSSCG_STATUS_SYNCH_FIFO_EMPTY_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_SYNCH_FIFO_EMPTY_MASK) >> CSSCG_STATUS_SYNCH_FIFO_EMPTY_SHIFT)
#define  CSSCG_STATUS_SYNCH_FIFO_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_SYNCH_FIFO_EMPTY_MASK) | (((uint32_t)val) << CSSCG_STATUS_SYNCH_FIFO_EMPTY_SHIFT))

#define  CSSCG_STATUS_NEWCODE_VALID_MASK 0x40000000
#define  CSSCG_STATUS_NEWCODE_VALID_SHIFT 30
#define  CSSCG_STATUS_NEWCODE_VALID_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_NEWCODE_VALID_MASK) >> CSSCG_STATUS_NEWCODE_VALID_SHIFT)
#define  CSSCG_STATUS_NEWCODE_VALID_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_NEWCODE_VALID_MASK) | (((uint32_t)val) << CSSCG_STATUS_NEWCODE_VALID_SHIFT))

#define  CSSCG_STATUS_RESERVED1_MASK 0x20000000
#define  CSSCG_STATUS_RESERVED1_SHIFT 29
#define  CSSCG_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_RESERVED1_MASK) >> CSSCG_STATUS_RESERVED1_SHIFT)
#define  CSSCG_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_RESERVED1_MASK) | (((uint32_t)val) << CSSCG_STATUS_RESERVED1_SHIFT))

#define  CSSCG_STATUS_NEWCODE_MASK 0x1ffff000
#define  CSSCG_STATUS_NEWCODE_SHIFT 12
#define  CSSCG_STATUS_NEWCODE_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_NEWCODE_MASK) >> CSSCG_STATUS_NEWCODE_SHIFT)
#define  CSSCG_STATUS_NEWCODE_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_NEWCODE_MASK) | (((uint32_t)val) << CSSCG_STATUS_NEWCODE_SHIFT))

#define  CSSCG_STATUS_RESERVED2_MASK 0xe00
#define  CSSCG_STATUS_RESERVED2_SHIFT 9
#define  CSSCG_STATUS_RESERVED2_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_RESERVED2_MASK) >> CSSCG_STATUS_RESERVED2_SHIFT)
#define  CSSCG_STATUS_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_RESERVED2_MASK) | (((uint32_t)val) << CSSCG_STATUS_RESERVED2_SHIFT))

#define  CSSCG_STATUS_CURRENT_ADDR_MASK 0x1f0
#define  CSSCG_STATUS_CURRENT_ADDR_SHIFT 4
#define  CSSCG_STATUS_CURRENT_ADDR_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_CURRENT_ADDR_MASK) >> CSSCG_STATUS_CURRENT_ADDR_SHIFT)
#define  CSSCG_STATUS_CURRENT_ADDR_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_CURRENT_ADDR_MASK) | (((uint32_t)val) << CSSCG_STATUS_CURRENT_ADDR_SHIFT))

#define  CSSCG_STATUS_CURRENT_PHASE_MASK 0xc
#define  CSSCG_STATUS_CURRENT_PHASE_SHIFT 2
#define  CSSCG_STATUS_CURRENT_PHASE_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_CURRENT_PHASE_MASK) >> CSSCG_STATUS_CURRENT_PHASE_SHIFT)
#define  CSSCG_STATUS_CURRENT_PHASE_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_CURRENT_PHASE_MASK) | (((uint32_t)val) << CSSCG_STATUS_CURRENT_PHASE_SHIFT))

#define  CSSCG_STATUS_HALTED_MASK 0x2
#define  CSSCG_STATUS_HALTED_SHIFT 1
#define  CSSCG_STATUS_HALTED_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_HALTED_MASK) >> CSSCG_STATUS_HALTED_SHIFT)
#define  CSSCG_STATUS_HALTED_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_HALTED_MASK) | (((uint32_t)val) << CSSCG_STATUS_HALTED_SHIFT))

#define  CSSCG_STATUS_ACTIVE_MASK 0x1
#define  CSSCG_STATUS_ACTIVE_SHIFT 0
#define  CSSCG_STATUS_ACTIVE_MASK_SHIFT(reg) (((reg) & CSSCG_STATUS_ACTIVE_MASK) >> CSSCG_STATUS_ACTIVE_SHIFT)
#define  CSSCG_STATUS_ACTIVE_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_STATUS_ACTIVE_MASK) | (((uint32_t)val) << CSSCG_STATUS_ACTIVE_SHIFT))

//====================================================================
//Register: CSSCG RAM Access Register (CSSCG_RAM)
/** \brief The table used to define the dither shape. The values are unsigned "deltas." Therefore, the curve must be monotonic.*/
//====================================================================

#define  CSSCG_CSSCG_RAM_RESERVED1_MASK 0xffff0000
#define  CSSCG_CSSCG_RAM_RESERVED1_SHIFT 16
#define  CSSCG_CSSCG_RAM_RESERVED1_MASK_SHIFT(reg) (((reg) & CSSCG_CSSCG_RAM_RESERVED1_MASK) >> CSSCG_CSSCG_RAM_RESERVED1_SHIFT)
#define  CSSCG_CSSCG_RAM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_CSSCG_RAM_RESERVED1_MASK) | (((uint32_t)val) << CSSCG_CSSCG_RAM_RESERVED1_SHIFT))

#define  CSSCG_CSSCG_RAM_VAL_MASK 0xffff
#define  CSSCG_CSSCG_RAM_VAL_SHIFT 0
#define  CSSCG_CSSCG_RAM_VAL_MASK_SHIFT(reg) (((reg) & CSSCG_CSSCG_RAM_VAL_MASK) >> CSSCG_CSSCG_RAM_VAL_SHIFT)
#define  CSSCG_CSSCG_RAM_VAL_REPLACE_VAL(reg,val) (((reg) & ~CSSCG_CSSCG_RAM_VAL_MASK) | (((uint32_t)val) << CSSCG_CSSCG_RAM_VAL_SHIFT))

#endif // CSSCG
