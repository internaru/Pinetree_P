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
 * \file BLOOM_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BLOOM_REGMASKS_H_
#define _BLOOM_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BLOOM (BLOOM)
/** \brief Register descriptions for the BLOOM block.*/
//
//====================================================================

//====================================================================
//Register: BLOOM Configuration Register (BCR)
/** \brief This register configures the general operation of the BLOOM block. If BLOOM is bypassed, the states of the internal state machines are reset, but register settings are preserved.*/
//====================================================================

#define  BLOOM_BCR_SRAM_ACCESS_ENABLE_MASK 0x80000000
#define  BLOOM_BCR_SRAM_ACCESS_ENABLE_SHIFT 31
#define  BLOOM_BCR_SRAM_ACCESS_ENABLE_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_SRAM_ACCESS_ENABLE_MASK) >> BLOOM_BCR_SRAM_ACCESS_ENABLE_SHIFT)
#define  BLOOM_BCR_SRAM_ACCESS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_SRAM_ACCESS_ENABLE_MASK) | (((uint32_t)val) << BLOOM_BCR_SRAM_ACCESS_ENABLE_SHIFT))

#define  BLOOM_BCR_SAVE_RESTORE_MODE_MASK 0x40000000
#define  BLOOM_BCR_SAVE_RESTORE_MODE_SHIFT 30
#define  BLOOM_BCR_SAVE_RESTORE_MODE_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_SAVE_RESTORE_MODE_MASK) >> BLOOM_BCR_SAVE_RESTORE_MODE_SHIFT)
#define  BLOOM_BCR_SAVE_RESTORE_MODE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_SAVE_RESTORE_MODE_MASK) | (((uint32_t)val) << BLOOM_BCR_SAVE_RESTORE_MODE_SHIFT))

#define  BLOOM_BCR_RESERVED1_MASK 0x3e000000
#define  BLOOM_BCR_RESERVED1_SHIFT 25
#define  BLOOM_BCR_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_RESERVED1_MASK) >> BLOOM_BCR_RESERVED1_SHIFT)
#define  BLOOM_BCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BCR_RESERVED1_SHIFT))

#define  BLOOM_BCR_V_OUT_WINDOW_MASK 0x1000000
#define  BLOOM_BCR_V_OUT_WINDOW_SHIFT 24
#define  BLOOM_BCR_V_OUT_WINDOW_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_V_OUT_WINDOW_MASK) >> BLOOM_BCR_V_OUT_WINDOW_SHIFT)
#define  BLOOM_BCR_V_OUT_WINDOW_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_V_OUT_WINDOW_MASK) | (((uint32_t)val) << BLOOM_BCR_V_OUT_WINDOW_SHIFT))

#define  BLOOM_BCR_RESERVED2_MASK 0xe00000
#define  BLOOM_BCR_RESERVED2_SHIFT 21
#define  BLOOM_BCR_RESERVED2_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_RESERVED2_MASK) >> BLOOM_BCR_RESERVED2_SHIFT)
#define  BLOOM_BCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_RESERVED2_MASK) | (((uint32_t)val) << BLOOM_BCR_RESERVED2_SHIFT))

#define  BLOOM_BCR_H_OUT_WINDOW_MASK 0x100000
#define  BLOOM_BCR_H_OUT_WINDOW_SHIFT 20
#define  BLOOM_BCR_H_OUT_WINDOW_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_H_OUT_WINDOW_MASK) >> BLOOM_BCR_H_OUT_WINDOW_SHIFT)
#define  BLOOM_BCR_H_OUT_WINDOW_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_H_OUT_WINDOW_MASK) | (((uint32_t)val) << BLOOM_BCR_H_OUT_WINDOW_SHIFT))

#define  BLOOM_BCR_V_INPUT_CONDENSE_M1_MASK 0xff000
#define  BLOOM_BCR_V_INPUT_CONDENSE_M1_SHIFT 12
#define  BLOOM_BCR_V_INPUT_CONDENSE_M1_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_V_INPUT_CONDENSE_M1_MASK) >> BLOOM_BCR_V_INPUT_CONDENSE_M1_SHIFT)
#define  BLOOM_BCR_V_INPUT_CONDENSE_M1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_V_INPUT_CONDENSE_M1_MASK) | (((uint32_t)val) << BLOOM_BCR_V_INPUT_CONDENSE_M1_SHIFT))

#define  BLOOM_BCR_H_INPUT_CONDENSE_M1_MASK 0xff0
#define  BLOOM_BCR_H_INPUT_CONDENSE_M1_SHIFT 4
#define  BLOOM_BCR_H_INPUT_CONDENSE_M1_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_H_INPUT_CONDENSE_M1_MASK) >> BLOOM_BCR_H_INPUT_CONDENSE_M1_SHIFT)
#define  BLOOM_BCR_H_INPUT_CONDENSE_M1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_H_INPUT_CONDENSE_M1_MASK) | (((uint32_t)val) << BLOOM_BCR_H_INPUT_CONDENSE_M1_SHIFT))

#define  BLOOM_BCR_RESERVED3_MASK 0xe
#define  BLOOM_BCR_RESERVED3_SHIFT 1
#define  BLOOM_BCR_RESERVED3_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_RESERVED3_MASK) >> BLOOM_BCR_RESERVED3_SHIFT)
#define  BLOOM_BCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_RESERVED3_MASK) | (((uint32_t)val) << BLOOM_BCR_RESERVED3_SHIFT))

#define  BLOOM_BCR_BYPASS_ENABLE_MASK 0x1
#define  BLOOM_BCR_BYPASS_ENABLE_SHIFT 0
#define  BLOOM_BCR_BYPASS_ENABLE_MASK_SHIFT(reg) (((reg) & BLOOM_BCR_BYPASS_ENABLE_MASK) >> BLOOM_BCR_BYPASS_ENABLE_SHIFT)
#define  BLOOM_BCR_BYPASS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BCR_BYPASS_ENABLE_MASK) | (((uint32_t)val) << BLOOM_BCR_BYPASS_ENABLE_SHIFT))

//====================================================================
//Register: Input Image Configuration Register (BIIC)
/** \brief This register provides information about the input image. Specifically, these values are used to find the end of the SRAM for a line.*/
//====================================================================

#define  BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_MASK 0xff000000
#define  BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_SHIFT 24
#define  BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_MASK_SHIFT(reg) (((reg) & BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_MASK) >> BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_SHIFT)
#define  BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_MASK) | (((uint32_t)val) << BLOOM_BIIC_WIDTH_M1_MOD_CONDENSE_SHIFT))

#define  BLOOM_BIIC_RESERVED1_MASK 0xff0000
#define  BLOOM_BIIC_RESERVED1_SHIFT 16
#define  BLOOM_BIIC_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BIIC_RESERVED1_MASK) >> BLOOM_BIIC_RESERVED1_SHIFT)
#define  BLOOM_BIIC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BIIC_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BIIC_RESERVED1_SHIFT))

#define  BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_MASK 0xffff
#define  BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_SHIFT 0
#define  BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_MASK_SHIFT(reg) (((reg) & BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_MASK) >> BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_SHIFT)
#define  BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_MASK) | (((uint32_t)val) << BLOOM_BIIC_WIDTH_M1_DIV_CONDENSE_SHIFT))

//====================================================================
//Register: Line Buffer Access Control Register (BLBC)
/** \brief This register enables/disables access to the Line Buffer SRAMs.*/
//====================================================================

#define  BLOOM_BLBC_RESERVED1_MASK 0xfffffffc
#define  BLOOM_BLBC_RESERVED1_SHIFT 2
#define  BLOOM_BLBC_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BLBC_RESERVED1_MASK) >> BLOOM_BLBC_RESERVED1_SHIFT)
#define  BLOOM_BLBC_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBC_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BLBC_RESERVED1_SHIFT))

#define  BLOOM_BLBC_LB_INCR_ADDR_MODE_MASK 0x2
#define  BLOOM_BLBC_LB_INCR_ADDR_MODE_SHIFT 1
#define  BLOOM_BLBC_LB_INCR_ADDR_MODE_MASK_SHIFT(reg) (((reg) & BLOOM_BLBC_LB_INCR_ADDR_MODE_MASK) >> BLOOM_BLBC_LB_INCR_ADDR_MODE_SHIFT)
#define  BLOOM_BLBC_LB_INCR_ADDR_MODE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBC_LB_INCR_ADDR_MODE_MASK) | (((uint32_t)val) << BLOOM_BLBC_LB_INCR_ADDR_MODE_SHIFT))

#define  BLOOM_BLBC_LB_RNW_MASK 0x1
#define  BLOOM_BLBC_LB_RNW_SHIFT 0
#define  BLOOM_BLBC_LB_RNW_MASK_SHIFT(reg) (((reg) & BLOOM_BLBC_LB_RNW_MASK) >> BLOOM_BLBC_LB_RNW_SHIFT)
#define  BLOOM_BLBC_LB_RNW_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBC_LB_RNW_MASK) | (((uint32_t)val) << BLOOM_BLBC_LB_RNW_SHIFT))

//====================================================================
//Register:  Line Buffer Address Register (BLBA)
/** \brief Write of the Line Buffer Address to access.*/
//====================================================================

#define  BLOOM_BLBA_RESERVED1_MASK 0xfffff800
#define  BLOOM_BLBA_RESERVED1_SHIFT 11
#define  BLOOM_BLBA_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BLBA_RESERVED1_MASK) >> BLOOM_BLBA_RESERVED1_SHIFT)
#define  BLOOM_BLBA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBA_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BLBA_RESERVED1_SHIFT))

#define  BLOOM_BLBA_LB_ADDR_MASK 0x7ff
#define  BLOOM_BLBA_LB_ADDR_SHIFT 0
#define  BLOOM_BLBA_LB_ADDR_MASK_SHIFT(reg) (((reg) & BLOOM_BLBA_LB_ADDR_MASK) >> BLOOM_BLBA_LB_ADDR_SHIFT)
#define  BLOOM_BLBA_LB_ADDR_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBA_LB_ADDR_MASK) | (((uint32_t)val) << BLOOM_BLBA_LB_ADDR_SHIFT))

//====================================================================
//Register: Line Buffer Address Status Register (BLBAS)
/** \brief Read of the Line Buffer SRAM address being accessed.*/
//====================================================================

#define  BLOOM_BLBAS_RESERVED1_MASK 0xfffff800
#define  BLOOM_BLBAS_RESERVED1_SHIFT 11
#define  BLOOM_BLBAS_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BLBAS_RESERVED1_MASK) >> BLOOM_BLBAS_RESERVED1_SHIFT)
#define  BLOOM_BLBAS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBAS_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BLBAS_RESERVED1_SHIFT))

#define  BLOOM_BLBAS_LB_ADDR_STATUS_MASK 0x7ff
#define  BLOOM_BLBAS_LB_ADDR_STATUS_SHIFT 0
#define  BLOOM_BLBAS_LB_ADDR_STATUS_MASK_SHIFT(reg) (((reg) & BLOOM_BLBAS_LB_ADDR_STATUS_MASK) >> BLOOM_BLBAS_LB_ADDR_STATUS_SHIFT)
#define  BLOOM_BLBAS_LB_ADDR_STATUS_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBAS_LB_ADDR_STATUS_MASK) | (((uint32_t)val) << BLOOM_BLBAS_LB_ADDR_STATUS_SHIFT))

//====================================================================
//Register: Line Buffer Write Data (BLBWD)
/** \brief For register writes to the Line Buffer SRAMs. When not in "increment address mode", data must be written into BLBWD prior to
        writing the address into BLBA.
        When in "increment address mode", the starting address must be written to BLBA prior to performing consecutive writes to this register.*/
//====================================================================

#define  BLOOM_BLBWD_LB_WR_DATA_MASK 0xffffffff
#define  BLOOM_BLBWD_LB_WR_DATA_SHIFT 0
#define  BLOOM_BLBWD_LB_WR_DATA_MASK_SHIFT(reg) (((reg) & BLOOM_BLBWD_LB_WR_DATA_MASK) >> BLOOM_BLBWD_LB_WR_DATA_SHIFT)
#define  BLOOM_BLBWD_LB_WR_DATA_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBWD_LB_WR_DATA_MASK) | (((uint32_t)val) << BLOOM_BLBWD_LB_WR_DATA_SHIFT))

//====================================================================
//Register: Line Buffer Read Data (BLBRD)
/** \brief For register reads to the Line Buffer SRAMs. When not in "increment address mode", the address must be written to BLBA prior to reading data from this register.
        When in "increment address mode", the starting address must be written to BLBA prior to performing consecutive reads from this register.*/
//====================================================================

#define  BLOOM_BLBRD_LB_RD_DATA0_MASK 0xffffffff
#define  BLOOM_BLBRD_LB_RD_DATA0_SHIFT 0
#define  BLOOM_BLBRD_LB_RD_DATA0_MASK_SHIFT(reg) (((reg) & BLOOM_BLBRD_LB_RD_DATA0_MASK) >> BLOOM_BLBRD_LB_RD_DATA0_SHIFT)
#define  BLOOM_BLBRD_LB_RD_DATA0_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BLBRD_LB_RD_DATA0_MASK) | (((uint32_t)val) << BLOOM_BLBRD_LB_RD_DATA0_SHIFT))

//====================================================================
//Register: BLOOM Save Restore Address (BSRA)
/** \brief This register is used to set the address of the save/restore register to access. There are currently 3 32-bit registers that must be saved and restored, therefore valid addresses are 0x0 to 0x2.*/
//====================================================================

#define  BLOOM_BSRA_RESERVED1_MASK 0xfffffffc
#define  BLOOM_BSRA_RESERVED1_SHIFT 2
#define  BLOOM_BSRA_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BSRA_RESERVED1_MASK) >> BLOOM_BSRA_RESERVED1_SHIFT)
#define  BLOOM_BSRA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSRA_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BSRA_RESERVED1_SHIFT))

#define  BLOOM_BSRA_SR_ADDR_MASK 0x3
#define  BLOOM_BSRA_SR_ADDR_SHIFT 0
#define  BLOOM_BSRA_SR_ADDR_MASK_SHIFT(reg) (((reg) & BLOOM_BSRA_SR_ADDR_MASK) >> BLOOM_BSRA_SR_ADDR_SHIFT)
#define  BLOOM_BSRA_SR_ADDR_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSRA_SR_ADDR_MASK) | (((uint32_t)val) << BLOOM_BSRA_SR_ADDR_SHIFT))

//====================================================================
//Register: BLOOM Save Restore Read (BSRR)
/** \brief This read-only register is provided for the save portion of save/restore functionality. The "save_restore_mode" bit of the configuration register must be set to a 1 to access this register. The address for this value is controlled by the BSRA register.*/
//====================================================================

#define  BLOOM_BSRR_SAVE_VAL_MASK 0xffffffff
#define  BLOOM_BSRR_SAVE_VAL_SHIFT 0
#define  BLOOM_BSRR_SAVE_VAL_MASK_SHIFT(reg) (((reg) & BLOOM_BSRR_SAVE_VAL_MASK) >> BLOOM_BSRR_SAVE_VAL_SHIFT)
#define  BLOOM_BSRR_SAVE_VAL_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSRR_SAVE_VAL_MASK) | (((uint32_t)val) << BLOOM_BSRR_SAVE_VAL_SHIFT))

//====================================================================
//Register: BLOOM Save Restore Write (BSRW)
/** \brief This write-only register is provided for the restore portion of save/restore functionality. The "save_restore_mode" bit of the configuration register must be set to a 1 to access this register. The address for this value is controlled by the BSRA register.*/
//====================================================================

#define  BLOOM_BSRW_RESTORE_VAL_MASK 0xffffffff
#define  BLOOM_BSRW_RESTORE_VAL_SHIFT 0
#define  BLOOM_BSRW_RESTORE_VAL_MASK_SHIFT(reg) (((reg) & BLOOM_BSRW_RESTORE_VAL_MASK) >> BLOOM_BSRW_RESTORE_VAL_SHIFT)
#define  BLOOM_BSRW_RESTORE_VAL_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSRW_RESTORE_VAL_MASK) | (((uint32_t)val) << BLOOM_BSRW_RESTORE_VAL_SHIFT))

//====================================================================
//Register: BLOOM Error Register (BER)
/** \brief This read-only register is provides the status of possible "error" bits. They are cleared on a soft reset or when a start-of-image data type is seen.*/
//====================================================================

#define  BLOOM_BER_RESERVED1_MASK 0xfffffffe
#define  BLOOM_BER_RESERVED1_SHIFT 1
#define  BLOOM_BER_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BER_RESERVED1_MASK) >> BLOOM_BER_RESERVED1_SHIFT)
#define  BLOOM_BER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BER_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BER_RESERVED1_SHIFT))

#define  BLOOM_BER_EOL_ALIGNMENT_ERROR_MASK 0x1
#define  BLOOM_BER_EOL_ALIGNMENT_ERROR_SHIFT 0
#define  BLOOM_BER_EOL_ALIGNMENT_ERROR_MASK_SHIFT(reg) (((reg) & BLOOM_BER_EOL_ALIGNMENT_ERROR_MASK) >> BLOOM_BER_EOL_ALIGNMENT_ERROR_SHIFT)
#define  BLOOM_BER_EOL_ALIGNMENT_ERROR_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BER_EOL_ALIGNMENT_ERROR_MASK) | (((uint32_t)val) << BLOOM_BER_EOL_ALIGNMENT_ERROR_SHIFT))

//====================================================================
//Register: BLOOM State Debug (BSD)
/** \brief This read-only register is provided in order to debug the Bloom block.*/
//====================================================================

#define  BLOOM_BSD_RESERVED1_MASK 0xffff0000
#define  BLOOM_BSD_RESERVED1_SHIFT 16
#define  BLOOM_BSD_RESERVED1_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_RESERVED1_MASK) >> BLOOM_BSD_RESERVED1_SHIFT)
#define  BLOOM_BSD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_RESERVED1_MASK) | (((uint32_t)val) << BLOOM_BSD_RESERVED1_SHIFT))

#define  BLOOM_BSD_READY_OUT_MASK 0x8000
#define  BLOOM_BSD_READY_OUT_SHIFT 15
#define  BLOOM_BSD_READY_OUT_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_READY_OUT_MASK) >> BLOOM_BSD_READY_OUT_SHIFT)
#define  BLOOM_BSD_READY_OUT_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_READY_OUT_MASK) | (((uint32_t)val) << BLOOM_BSD_READY_OUT_SHIFT))

#define  BLOOM_BSD_VALID_OUT_MASK 0x4000
#define  BLOOM_BSD_VALID_OUT_SHIFT 14
#define  BLOOM_BSD_VALID_OUT_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_VALID_OUT_MASK) >> BLOOM_BSD_VALID_OUT_SHIFT)
#define  BLOOM_BSD_VALID_OUT_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_VALID_OUT_MASK) | (((uint32_t)val) << BLOOM_BSD_VALID_OUT_SHIFT))

#define  BLOOM_BSD_READY_IN_MASK 0x2000
#define  BLOOM_BSD_READY_IN_SHIFT 13
#define  BLOOM_BSD_READY_IN_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_READY_IN_MASK) >> BLOOM_BSD_READY_IN_SHIFT)
#define  BLOOM_BSD_READY_IN_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_READY_IN_MASK) | (((uint32_t)val) << BLOOM_BSD_READY_IN_SHIFT))

#define  BLOOM_BSD_VALID_IN_MASK 0x1000
#define  BLOOM_BSD_VALID_IN_SHIFT 12
#define  BLOOM_BSD_VALID_IN_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_VALID_IN_MASK) >> BLOOM_BSD_VALID_IN_SHIFT)
#define  BLOOM_BSD_VALID_IN_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_VALID_IN_MASK) | (((uint32_t)val) << BLOOM_BSD_VALID_IN_SHIFT))

#define  BLOOM_BSD_RESERVED2_MASK 0x800
#define  BLOOM_BSD_RESERVED2_SHIFT 11
#define  BLOOM_BSD_RESERVED2_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_RESERVED2_MASK) >> BLOOM_BSD_RESERVED2_SHIFT)
#define  BLOOM_BSD_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_RESERVED2_MASK) | (((uint32_t)val) << BLOOM_BSD_RESERVED2_SHIFT))

#define  BLOOM_BSD_WINDOW_STATE_MASK 0x700
#define  BLOOM_BSD_WINDOW_STATE_SHIFT 8
#define  BLOOM_BSD_WINDOW_STATE_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_WINDOW_STATE_MASK) >> BLOOM_BSD_WINDOW_STATE_SHIFT)
#define  BLOOM_BSD_WINDOW_STATE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_WINDOW_STATE_MASK) | (((uint32_t)val) << BLOOM_BSD_WINDOW_STATE_SHIFT))

#define  BLOOM_BSD_RESERVED3_MASK 0x80
#define  BLOOM_BSD_RESERVED3_SHIFT 7
#define  BLOOM_BSD_RESERVED3_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_RESERVED3_MASK) >> BLOOM_BSD_RESERVED3_SHIFT)
#define  BLOOM_BSD_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_RESERVED3_MASK) | (((uint32_t)val) << BLOOM_BSD_RESERVED3_SHIFT))

#define  BLOOM_BSD_WINDOW_FULLNESS_MASK 0x70
#define  BLOOM_BSD_WINDOW_FULLNESS_SHIFT 4
#define  BLOOM_BSD_WINDOW_FULLNESS_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_WINDOW_FULLNESS_MASK) >> BLOOM_BSD_WINDOW_FULLNESS_SHIFT)
#define  BLOOM_BSD_WINDOW_FULLNESS_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_WINDOW_FULLNESS_MASK) | (((uint32_t)val) << BLOOM_BSD_WINDOW_FULLNESS_SHIFT))

#define  BLOOM_BSD_RESERVED4_MASK 0xc
#define  BLOOM_BSD_RESERVED4_SHIFT 2
#define  BLOOM_BSD_RESERVED4_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_RESERVED4_MASK) >> BLOOM_BSD_RESERVED4_SHIFT)
#define  BLOOM_BSD_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_RESERVED4_MASK) | (((uint32_t)val) << BLOOM_BSD_RESERVED4_SHIFT))

#define  BLOOM_BSD_CONDENSE_STATE_MASK 0x3
#define  BLOOM_BSD_CONDENSE_STATE_SHIFT 0
#define  BLOOM_BSD_CONDENSE_STATE_MASK_SHIFT(reg) (((reg) & BLOOM_BSD_CONDENSE_STATE_MASK) >> BLOOM_BSD_CONDENSE_STATE_SHIFT)
#define  BLOOM_BSD_CONDENSE_STATE_REPLACE_VAL(reg,val) (((reg) & ~BLOOM_BSD_CONDENSE_STATE_MASK) | (((uint32_t)val) << BLOOM_BSD_CONDENSE_STATE_SHIFT))

#endif // BLOOM
