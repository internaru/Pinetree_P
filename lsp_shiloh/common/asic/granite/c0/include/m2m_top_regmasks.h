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
 * \file m2m_top_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _M2M_TOP_REGMASKS_H_
#define _M2M_TOP_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: M2M TOP (M2M_TOP)
/** \brief M2M top level register set.*/
//
//====================================================================

//====================================================================
//
//Register File: M2M Registers (m2m0)
//
//====================================================================

//====================================================================
//Register: Link List Index Register (desc_start)
/** \brief Writing this register loads the first descriptor at Start Index.*/
//====================================================================

#define  M2M0_DESC_START_INDEX_MASK 0xfffffffc
#define  M2M0_DESC_START_INDEX_SHIFT 2
#define  M2M0_DESC_START_INDEX_MASK_SHIFT(reg) (((reg) & M2M0_DESC_START_INDEX_MASK) >> M2M0_DESC_START_INDEX_SHIFT)
#define  M2M0_DESC_START_INDEX_REPLACE_VAL(reg,val) (((reg) & ~M2M0_DESC_START_INDEX_MASK) | (((uint32_t)val) << M2M0_DESC_START_INDEX_SHIFT))

#define  M2M0_DESC_START_SOFT_RESET_MASK 0x2
#define  M2M0_DESC_START_SOFT_RESET_SHIFT 1
#define  M2M0_DESC_START_SOFT_RESET_MASK_SHIFT(reg) (((reg) & M2M0_DESC_START_SOFT_RESET_MASK) >> M2M0_DESC_START_SOFT_RESET_SHIFT)
#define  M2M0_DESC_START_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~M2M0_DESC_START_SOFT_RESET_MASK) | (((uint32_t)val) << M2M0_DESC_START_SOFT_RESET_SHIFT))

#define  M2M0_DESC_START_ID_CTL_MASK 0x1
#define  M2M0_DESC_START_ID_CTL_SHIFT 0
#define  M2M0_DESC_START_ID_CTL_MASK_SHIFT(reg) (((reg) & M2M0_DESC_START_ID_CTL_MASK) >> M2M0_DESC_START_ID_CTL_SHIFT)
#define  M2M0_DESC_START_ID_CTL_REPLACE_VAL(reg,val) (((reg) & ~M2M0_DESC_START_ID_CTL_MASK) | (((uint32_t)val) << M2M0_DESC_START_ID_CTL_SHIFT))

//====================================================================
//Register: Current Link List Address Register (current_desc_addr)
/** \brief The current link list address that the DMA block working on.*/
//====================================================================

#define  M2M0_CURRENT_DESC_ADDR_PTR_MASK 0xfffffffc
#define  M2M0_CURRENT_DESC_ADDR_PTR_SHIFT 2
#define  M2M0_CURRENT_DESC_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_CURRENT_DESC_ADDR_PTR_MASK) >> M2M0_CURRENT_DESC_ADDR_PTR_SHIFT)
#define  M2M0_CURRENT_DESC_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CURRENT_DESC_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_CURRENT_DESC_ADDR_PTR_SHIFT))

#define  M2M0_CURRENT_DESC_ADDR_SOFT_RESET_MASK 0x2
#define  M2M0_CURRENT_DESC_ADDR_SOFT_RESET_SHIFT 1
#define  M2M0_CURRENT_DESC_ADDR_SOFT_RESET_MASK_SHIFT(reg) (((reg) & M2M0_CURRENT_DESC_ADDR_SOFT_RESET_MASK) >> M2M0_CURRENT_DESC_ADDR_SOFT_RESET_SHIFT)
#define  M2M0_CURRENT_DESC_ADDR_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CURRENT_DESC_ADDR_SOFT_RESET_MASK) | (((uint32_t)val) << M2M0_CURRENT_DESC_ADDR_SOFT_RESET_SHIFT))

#define  M2M0_CURRENT_DESC_ADDR_ID_CTL_MASK 0x1
#define  M2M0_CURRENT_DESC_ADDR_ID_CTL_SHIFT 0
#define  M2M0_CURRENT_DESC_ADDR_ID_CTL_MASK_SHIFT(reg) (((reg) & M2M0_CURRENT_DESC_ADDR_ID_CTL_MASK) >> M2M0_CURRENT_DESC_ADDR_ID_CTL_SHIFT)
#define  M2M0_CURRENT_DESC_ADDR_ID_CTL_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CURRENT_DESC_ADDR_ID_CTL_MASK) | (((uint32_t)val) << M2M0_CURRENT_DESC_ADDR_ID_CTL_SHIFT))

//====================================================================
//Register: Status register (status)
/** \brief Status register for the m2m unit.*/
//====================================================================

#define  M2M0_STATUS_CHAN_BUSY_MASK 0xf0000000
#define  M2M0_STATUS_CHAN_BUSY_SHIFT 28
#define  M2M0_STATUS_CHAN_BUSY_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_CHAN_BUSY_MASK) >> M2M0_STATUS_CHAN_BUSY_SHIFT)
#define  M2M0_STATUS_CHAN_BUSY_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_CHAN_BUSY_MASK) | (((uint32_t)val) << M2M0_STATUS_CHAN_BUSY_SHIFT))

#define  M2M0_STATUS_CHAN_STATE_MASK 0xff00000
#define  M2M0_STATUS_CHAN_STATE_SHIFT 20
#define  M2M0_STATUS_CHAN_STATE_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_CHAN_STATE_MASK) >> M2M0_STATUS_CHAN_STATE_SHIFT)
#define  M2M0_STATUS_CHAN_STATE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_CHAN_STATE_MASK) | (((uint32_t)val) << M2M0_STATUS_CHAN_STATE_SHIFT))

#define  M2M0_STATUS_FIFO_ERROR_MASK 0x80000
#define  M2M0_STATUS_FIFO_ERROR_SHIFT 19
#define  M2M0_STATUS_FIFO_ERROR_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_FIFO_ERROR_MASK) >> M2M0_STATUS_FIFO_ERROR_SHIFT)
#define  M2M0_STATUS_FIFO_ERROR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_FIFO_ERROR_MASK) | (((uint32_t)val) << M2M0_STATUS_FIFO_ERROR_SHIFT))

#define  M2M0_STATUS_FIFO_ENABLE_MASK 0x40000
#define  M2M0_STATUS_FIFO_ENABLE_SHIFT 18
#define  M2M0_STATUS_FIFO_ENABLE_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_FIFO_ENABLE_MASK) >> M2M0_STATUS_FIFO_ENABLE_SHIFT)
#define  M2M0_STATUS_FIFO_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_FIFO_ENABLE_MASK) | (((uint32_t)val) << M2M0_STATUS_FIFO_ENABLE_SHIFT))

#define  M2M0_STATUS_FIFO_FULL_MASK 0x20000
#define  M2M0_STATUS_FIFO_FULL_SHIFT 17
#define  M2M0_STATUS_FIFO_FULL_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_FIFO_FULL_MASK) >> M2M0_STATUS_FIFO_FULL_SHIFT)
#define  M2M0_STATUS_FIFO_FULL_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_FIFO_FULL_MASK) | (((uint32_t)val) << M2M0_STATUS_FIFO_FULL_SHIFT))

#define  M2M0_STATUS_FIFO_EMPTY_MASK 0x10000
#define  M2M0_STATUS_FIFO_EMPTY_SHIFT 16
#define  M2M0_STATUS_FIFO_EMPTY_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_FIFO_EMPTY_MASK) >> M2M0_STATUS_FIFO_EMPTY_SHIFT)
#define  M2M0_STATUS_FIFO_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_FIFO_EMPTY_MASK) | (((uint32_t)val) << M2M0_STATUS_FIFO_EMPTY_SHIFT))

#define  M2M0_STATUS_IDLE_MASK 0x8000
#define  M2M0_STATUS_IDLE_SHIFT 15
#define  M2M0_STATUS_IDLE_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_IDLE_MASK) >> M2M0_STATUS_IDLE_SHIFT)
#define  M2M0_STATUS_IDLE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_IDLE_MASK) | (((uint32_t)val) << M2M0_STATUS_IDLE_SHIFT))

#define  M2M0_STATUS_RESERVED1_MASK 0x7c00
#define  M2M0_STATUS_RESERVED1_SHIFT 10
#define  M2M0_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_RESERVED1_MASK) >> M2M0_STATUS_RESERVED1_SHIFT)
#define  M2M0_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_RESERVED1_MASK) | (((uint32_t)val) << M2M0_STATUS_RESERVED1_SHIFT))

#define  M2M0_STATUS_BUFFER_ID_MASK 0x300
#define  M2M0_STATUS_BUFFER_ID_SHIFT 8
#define  M2M0_STATUS_BUFFER_ID_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_BUFFER_ID_MASK) >> M2M0_STATUS_BUFFER_ID_SHIFT)
#define  M2M0_STATUS_BUFFER_ID_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_BUFFER_ID_MASK) | (((uint32_t)val) << M2M0_STATUS_BUFFER_ID_SHIFT))

#define  M2M0_STATUS_BUFFER_LENGTH_MASK 0xff
#define  M2M0_STATUS_BUFFER_LENGTH_SHIFT 0
#define  M2M0_STATUS_BUFFER_LENGTH_MASK_SHIFT(reg) (((reg) & M2M0_STATUS_BUFFER_LENGTH_MASK) >> M2M0_STATUS_BUFFER_LENGTH_SHIFT)
#define  M2M0_STATUS_BUFFER_LENGTH_REPLACE_VAL(reg,val) (((reg) & ~M2M0_STATUS_BUFFER_LENGTH_MASK) | (((uint32_t)val) << M2M0_STATUS_BUFFER_LENGTH_SHIFT))

//====================================================================
//Register: Interrupt Enable (IEN)
/** \brief Enable for various interrupt sources.*/
//====================================================================

#define  M2M0_IEN_RESERVED1_MASK 0xfffffff0
#define  M2M0_IEN_RESERVED1_SHIFT 4
#define  M2M0_IEN_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M0_IEN_RESERVED1_MASK) >> M2M0_IEN_RESERVED1_SHIFT)
#define  M2M0_IEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IEN_RESERVED1_MASK) | (((uint32_t)val) << M2M0_IEN_RESERVED1_SHIFT))

#define  M2M0_IEN_CPU_OWNER_MASK 0x8
#define  M2M0_IEN_CPU_OWNER_SHIFT 3
#define  M2M0_IEN_CPU_OWNER_MASK_SHIFT(reg) (((reg) & M2M0_IEN_CPU_OWNER_MASK) >> M2M0_IEN_CPU_OWNER_SHIFT)
#define  M2M0_IEN_CPU_OWNER_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IEN_CPU_OWNER_MASK) | (((uint32_t)val) << M2M0_IEN_CPU_OWNER_SHIFT))

#define  M2M0_IEN_CHAIN_DONE_MASK 0x4
#define  M2M0_IEN_CHAIN_DONE_SHIFT 2
#define  M2M0_IEN_CHAIN_DONE_MASK_SHIFT(reg) (((reg) & M2M0_IEN_CHAIN_DONE_MASK) >> M2M0_IEN_CHAIN_DONE_SHIFT)
#define  M2M0_IEN_CHAIN_DONE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IEN_CHAIN_DONE_MASK) | (((uint32_t)val) << M2M0_IEN_CHAIN_DONE_SHIFT))

#define  M2M0_IEN_OUT_OF_ORDER_MASK 0x2
#define  M2M0_IEN_OUT_OF_ORDER_SHIFT 1
#define  M2M0_IEN_OUT_OF_ORDER_MASK_SHIFT(reg) (((reg) & M2M0_IEN_OUT_OF_ORDER_MASK) >> M2M0_IEN_OUT_OF_ORDER_SHIFT)
#define  M2M0_IEN_OUT_OF_ORDER_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IEN_OUT_OF_ORDER_MASK) | (((uint32_t)val) << M2M0_IEN_OUT_OF_ORDER_SHIFT))

#define  M2M0_IEN_TERM_CNT_MASK 0x1
#define  M2M0_IEN_TERM_CNT_SHIFT 0
#define  M2M0_IEN_TERM_CNT_MASK_SHIFT(reg) (((reg) & M2M0_IEN_TERM_CNT_MASK) >> M2M0_IEN_TERM_CNT_SHIFT)
#define  M2M0_IEN_TERM_CNT_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IEN_TERM_CNT_MASK) | (((uint32_t)val) << M2M0_IEN_TERM_CNT_SHIFT))

//====================================================================
//Register: Interrupt Pending (IPEND)
/** \brief Read-Only interrupt pending register.*/
//====================================================================

#define  M2M0_IPEND_RESERVED1_MASK 0xfffffff0
#define  M2M0_IPEND_RESERVED1_SHIFT 4
#define  M2M0_IPEND_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M0_IPEND_RESERVED1_MASK) >> M2M0_IPEND_RESERVED1_SHIFT)
#define  M2M0_IPEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IPEND_RESERVED1_MASK) | (((uint32_t)val) << M2M0_IPEND_RESERVED1_SHIFT))

#define  M2M0_IPEND_CPU_OWNER_PEND_MASK 0x8
#define  M2M0_IPEND_CPU_OWNER_PEND_SHIFT 3
#define  M2M0_IPEND_CPU_OWNER_PEND_MASK_SHIFT(reg) (((reg) & M2M0_IPEND_CPU_OWNER_PEND_MASK) >> M2M0_IPEND_CPU_OWNER_PEND_SHIFT)
#define  M2M0_IPEND_CPU_OWNER_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IPEND_CPU_OWNER_PEND_MASK) | (((uint32_t)val) << M2M0_IPEND_CPU_OWNER_PEND_SHIFT))

#define  M2M0_IPEND_CHAIN_DONE_PEND_MASK 0x4
#define  M2M0_IPEND_CHAIN_DONE_PEND_SHIFT 2
#define  M2M0_IPEND_CHAIN_DONE_PEND_MASK_SHIFT(reg) (((reg) & M2M0_IPEND_CHAIN_DONE_PEND_MASK) >> M2M0_IPEND_CHAIN_DONE_PEND_SHIFT)
#define  M2M0_IPEND_CHAIN_DONE_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IPEND_CHAIN_DONE_PEND_MASK) | (((uint32_t)val) << M2M0_IPEND_CHAIN_DONE_PEND_SHIFT))

#define  M2M0_IPEND_OUT_OF_ORDER_PEND_MASK 0x2
#define  M2M0_IPEND_OUT_OF_ORDER_PEND_SHIFT 1
#define  M2M0_IPEND_OUT_OF_ORDER_PEND_MASK_SHIFT(reg) (((reg) & M2M0_IPEND_OUT_OF_ORDER_PEND_MASK) >> M2M0_IPEND_OUT_OF_ORDER_PEND_SHIFT)
#define  M2M0_IPEND_OUT_OF_ORDER_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IPEND_OUT_OF_ORDER_PEND_MASK) | (((uint32_t)val) << M2M0_IPEND_OUT_OF_ORDER_PEND_SHIFT))

#define  M2M0_IPEND_TERM_CNT_PEND_MASK 0x1
#define  M2M0_IPEND_TERM_CNT_PEND_SHIFT 0
#define  M2M0_IPEND_TERM_CNT_PEND_MASK_SHIFT(reg) (((reg) & M2M0_IPEND_TERM_CNT_PEND_MASK) >> M2M0_IPEND_TERM_CNT_PEND_SHIFT)
#define  M2M0_IPEND_TERM_CNT_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IPEND_TERM_CNT_PEND_MASK) | (((uint32_t)val) << M2M0_IPEND_TERM_CNT_PEND_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge (IACK)
/** \brief Write-Only interrupt acknowledge register.*/
//====================================================================

#define  M2M0_IACK_RESERVED1_MASK 0xfffffff0
#define  M2M0_IACK_RESERVED1_SHIFT 4
#define  M2M0_IACK_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M0_IACK_RESERVED1_MASK) >> M2M0_IACK_RESERVED1_SHIFT)
#define  M2M0_IACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IACK_RESERVED1_MASK) | (((uint32_t)val) << M2M0_IACK_RESERVED1_SHIFT))

#define  M2M0_IACK_CPU_OWNER_ACK_MASK 0x8
#define  M2M0_IACK_CPU_OWNER_ACK_SHIFT 3
#define  M2M0_IACK_CPU_OWNER_ACK_MASK_SHIFT(reg) (((reg) & M2M0_IACK_CPU_OWNER_ACK_MASK) >> M2M0_IACK_CPU_OWNER_ACK_SHIFT)
#define  M2M0_IACK_CPU_OWNER_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IACK_CPU_OWNER_ACK_MASK) | (((uint32_t)val) << M2M0_IACK_CPU_OWNER_ACK_SHIFT))

#define  M2M0_IACK_CHAIN_DONE_ACK_MASK 0x4
#define  M2M0_IACK_CHAIN_DONE_ACK_SHIFT 2
#define  M2M0_IACK_CHAIN_DONE_ACK_MASK_SHIFT(reg) (((reg) & M2M0_IACK_CHAIN_DONE_ACK_MASK) >> M2M0_IACK_CHAIN_DONE_ACK_SHIFT)
#define  M2M0_IACK_CHAIN_DONE_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IACK_CHAIN_DONE_ACK_MASK) | (((uint32_t)val) << M2M0_IACK_CHAIN_DONE_ACK_SHIFT))

#define  M2M0_IACK_OUT_OF_ORDER_ACK_MASK 0x2
#define  M2M0_IACK_OUT_OF_ORDER_ACK_SHIFT 1
#define  M2M0_IACK_OUT_OF_ORDER_ACK_MASK_SHIFT(reg) (((reg) & M2M0_IACK_OUT_OF_ORDER_ACK_MASK) >> M2M0_IACK_OUT_OF_ORDER_ACK_SHIFT)
#define  M2M0_IACK_OUT_OF_ORDER_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IACK_OUT_OF_ORDER_ACK_MASK) | (((uint32_t)val) << M2M0_IACK_OUT_OF_ORDER_ACK_SHIFT))

#define  M2M0_IACK_TERM_CNT_ACK_MASK 0x1
#define  M2M0_IACK_TERM_CNT_ACK_SHIFT 0
#define  M2M0_IACK_TERM_CNT_ACK_MASK_SHIFT(reg) (((reg) & M2M0_IACK_TERM_CNT_ACK_MASK) >> M2M0_IACK_TERM_CNT_ACK_SHIFT)
#define  M2M0_IACK_TERM_CNT_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IACK_TERM_CNT_ACK_MASK) | (((uint32_t)val) << M2M0_IACK_TERM_CNT_ACK_SHIFT))

//====================================================================
//Register: Interrupt Force (IFOR)
/** \brief Forcing interrupt sources register. Write a one to each bit will force
               the corresponding interrupt source to be asserted.*/
//====================================================================

#define  M2M0_IFOR_RESERVED1_MASK 0xfffffff0
#define  M2M0_IFOR_RESERVED1_SHIFT 4
#define  M2M0_IFOR_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M0_IFOR_RESERVED1_MASK) >> M2M0_IFOR_RESERVED1_SHIFT)
#define  M2M0_IFOR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IFOR_RESERVED1_MASK) | (((uint32_t)val) << M2M0_IFOR_RESERVED1_SHIFT))

#define  M2M0_IFOR_CPU_OWNER_FORCE_MASK 0x8
#define  M2M0_IFOR_CPU_OWNER_FORCE_SHIFT 3
#define  M2M0_IFOR_CPU_OWNER_FORCE_MASK_SHIFT(reg) (((reg) & M2M0_IFOR_CPU_OWNER_FORCE_MASK) >> M2M0_IFOR_CPU_OWNER_FORCE_SHIFT)
#define  M2M0_IFOR_CPU_OWNER_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IFOR_CPU_OWNER_FORCE_MASK) | (((uint32_t)val) << M2M0_IFOR_CPU_OWNER_FORCE_SHIFT))

#define  M2M0_IFOR_CHAIN_DONE_FORCE_MASK 0x4
#define  M2M0_IFOR_CHAIN_DONE_FORCE_SHIFT 2
#define  M2M0_IFOR_CHAIN_DONE_FORCE_MASK_SHIFT(reg) (((reg) & M2M0_IFOR_CHAIN_DONE_FORCE_MASK) >> M2M0_IFOR_CHAIN_DONE_FORCE_SHIFT)
#define  M2M0_IFOR_CHAIN_DONE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IFOR_CHAIN_DONE_FORCE_MASK) | (((uint32_t)val) << M2M0_IFOR_CHAIN_DONE_FORCE_SHIFT))

#define  M2M0_IFOR_OUT_OF_ORDER_FORCE_MASK 0x2
#define  M2M0_IFOR_OUT_OF_ORDER_FORCE_SHIFT 1
#define  M2M0_IFOR_OUT_OF_ORDER_FORCE_MASK_SHIFT(reg) (((reg) & M2M0_IFOR_OUT_OF_ORDER_FORCE_MASK) >> M2M0_IFOR_OUT_OF_ORDER_FORCE_SHIFT)
#define  M2M0_IFOR_OUT_OF_ORDER_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IFOR_OUT_OF_ORDER_FORCE_MASK) | (((uint32_t)val) << M2M0_IFOR_OUT_OF_ORDER_FORCE_SHIFT))

#define  M2M0_IFOR_TERM_CNT_FORCE_MASK 0x1
#define  M2M0_IFOR_TERM_CNT_FORCE_SHIFT 0
#define  M2M0_IFOR_TERM_CNT_FORCE_MASK_SHIFT(reg) (((reg) & M2M0_IFOR_TERM_CNT_FORCE_MASK) >> M2M0_IFOR_TERM_CNT_FORCE_SHIFT)
#define  M2M0_IFOR_TERM_CNT_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_IFOR_TERM_CNT_FORCE_MASK) | (((uint32_t)val) << M2M0_IFOR_TERM_CNT_FORCE_SHIFT))

//====================================================================
//Register: Descriptor control register (control)
/** \brief Read the active Descriptor Control Register.*/
//====================================================================

#define  M2M0_CONTROL_OWN_MASK 0x80000000
#define  M2M0_CONTROL_OWN_SHIFT 31
#define  M2M0_CONTROL_OWN_MASK_SHIFT(reg) (((reg) & M2M0_CONTROL_OWN_MASK) >> M2M0_CONTROL_OWN_SHIFT)
#define  M2M0_CONTROL_OWN_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CONTROL_OWN_MASK) | (((uint32_t)val) << M2M0_CONTROL_OWN_SHIFT))

#define  M2M0_CONTROL_UPDATE_XLATE_MASK 0x40000000
#define  M2M0_CONTROL_UPDATE_XLATE_SHIFT 30
#define  M2M0_CONTROL_UPDATE_XLATE_MASK_SHIFT(reg) (((reg) & M2M0_CONTROL_UPDATE_XLATE_MASK) >> M2M0_CONTROL_UPDATE_XLATE_SHIFT)
#define  M2M0_CONTROL_UPDATE_XLATE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CONTROL_UPDATE_XLATE_MASK) | (((uint32_t)val) << M2M0_CONTROL_UPDATE_XLATE_SHIFT))

#define  M2M0_CONTROL_RESERVED1_MASK 0x3fff0000
#define  M2M0_CONTROL_RESERVED1_SHIFT 16
#define  M2M0_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M0_CONTROL_RESERVED1_MASK) >> M2M0_CONTROL_RESERVED1_SHIFT)
#define  M2M0_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << M2M0_CONTROL_RESERVED1_SHIFT))

#define  M2M0_CONTROL_SIZE_MASK 0xffff
#define  M2M0_CONTROL_SIZE_SHIFT 0
#define  M2M0_CONTROL_SIZE_MASK_SHIFT(reg) (((reg) & M2M0_CONTROL_SIZE_MASK) >> M2M0_CONTROL_SIZE_SHIFT)
#define  M2M0_CONTROL_SIZE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CONTROL_SIZE_MASK) | (((uint32_t)val) << M2M0_CONTROL_SIZE_SHIFT))

//====================================================================
//Register: Descriptor Current Source Address (src_addr)
/** \brief Read the active Descriptor Source Address pointer.*/
//====================================================================

#define  M2M0_SRC_ADDR_PTR_MASK 0xffffffff
#define  M2M0_SRC_ADDR_PTR_SHIFT 0
#define  M2M0_SRC_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_SRC_ADDR_PTR_MASK) >> M2M0_SRC_ADDR_PTR_SHIFT)
#define  M2M0_SRC_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_SRC_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_SRC_ADDR_PTR_SHIFT))

//====================================================================
//Register: Descriptor Current Destination Address (dest_addr)
/** \brief Read the active Descriptor Destination Address pointer.*/
//====================================================================

#define  M2M0_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M0_DEST_ADDR_PTR_SHIFT 0
#define  M2M0_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_DEST_ADDR_PTR_MASK) >> M2M0_DEST_ADDR_PTR_SHIFT)
#define  M2M0_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Descriptor PCIe outbound Translate Value (pcie_outbound)
/** \brief Read the active PCIe outbound translate register value.*/
//====================================================================

#define  M2M0_PCIE_OUTBOUND_XLATE_VALUE_MASK 0xffffffff
#define  M2M0_PCIE_OUTBOUND_XLATE_VALUE_SHIFT 0
#define  M2M0_PCIE_OUTBOUND_XLATE_VALUE_MASK_SHIFT(reg) (((reg) & M2M0_PCIE_OUTBOUND_XLATE_VALUE_MASK) >> M2M0_PCIE_OUTBOUND_XLATE_VALUE_SHIFT)
#define  M2M0_PCIE_OUTBOUND_XLATE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~M2M0_PCIE_OUTBOUND_XLATE_VALUE_MASK) | (((uint32_t)val) << M2M0_PCIE_OUTBOUND_XLATE_VALUE_SHIFT))

//====================================================================
//Register: Descriptor Linked List Address (list_addr)
/** \brief Read the active Descriptor Linked List pointer (to the next descriptor).*/
//====================================================================

#define  M2M0_LIST_ADDR_PTR_MASK 0xffffffff
#define  M2M0_LIST_ADDR_PTR_SHIFT 0
#define  M2M0_LIST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_LIST_ADDR_PTR_MASK) >> M2M0_LIST_ADDR_PTR_SHIFT)
#define  M2M0_LIST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_LIST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_LIST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 0 Destination Address (chan0_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M0_CHAN0_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M0_CHAN0_DEST_ADDR_PTR_SHIFT 0
#define  M2M0_CHAN0_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_CHAN0_DEST_ADDR_PTR_MASK) >> M2M0_CHAN0_DEST_ADDR_PTR_SHIFT)
#define  M2M0_CHAN0_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CHAN0_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_CHAN0_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 1 Destination Address (chan1_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M0_CHAN1_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M0_CHAN1_DEST_ADDR_PTR_SHIFT 0
#define  M2M0_CHAN1_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_CHAN1_DEST_ADDR_PTR_MASK) >> M2M0_CHAN1_DEST_ADDR_PTR_SHIFT)
#define  M2M0_CHAN1_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CHAN1_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_CHAN1_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 2 Destination Address (chan2_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M0_CHAN2_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M0_CHAN2_DEST_ADDR_PTR_SHIFT 0
#define  M2M0_CHAN2_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_CHAN2_DEST_ADDR_PTR_MASK) >> M2M0_CHAN2_DEST_ADDR_PTR_SHIFT)
#define  M2M0_CHAN2_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CHAN2_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_CHAN2_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 3 Destination Address (chan3_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M0_CHAN3_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M0_CHAN3_DEST_ADDR_PTR_SHIFT 0
#define  M2M0_CHAN3_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M0_CHAN3_DEST_ADDR_PTR_MASK) >> M2M0_CHAN3_DEST_ADDR_PTR_SHIFT)
#define  M2M0_CHAN3_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M0_CHAN3_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M0_CHAN3_DEST_ADDR_PTR_SHIFT))

//====================================================================
//
//Register File: M2M Registers (m2m1)
//
//====================================================================

//====================================================================
//Register: Link List Index Register (desc_start)
/** \brief Writing this register loads the first descriptor at Start Index.*/
//====================================================================

#define  M2M1_DESC_START_INDEX_MASK 0xfffffffc
#define  M2M1_DESC_START_INDEX_SHIFT 2
#define  M2M1_DESC_START_INDEX_MASK_SHIFT(reg) (((reg) & M2M1_DESC_START_INDEX_MASK) >> M2M1_DESC_START_INDEX_SHIFT)
#define  M2M1_DESC_START_INDEX_REPLACE_VAL(reg,val) (((reg) & ~M2M1_DESC_START_INDEX_MASK) | (((uint32_t)val) << M2M1_DESC_START_INDEX_SHIFT))

#define  M2M1_DESC_START_SOFT_RESET_MASK 0x2
#define  M2M1_DESC_START_SOFT_RESET_SHIFT 1
#define  M2M1_DESC_START_SOFT_RESET_MASK_SHIFT(reg) (((reg) & M2M1_DESC_START_SOFT_RESET_MASK) >> M2M1_DESC_START_SOFT_RESET_SHIFT)
#define  M2M1_DESC_START_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~M2M1_DESC_START_SOFT_RESET_MASK) | (((uint32_t)val) << M2M1_DESC_START_SOFT_RESET_SHIFT))

#define  M2M1_DESC_START_ID_CTL_MASK 0x1
#define  M2M1_DESC_START_ID_CTL_SHIFT 0
#define  M2M1_DESC_START_ID_CTL_MASK_SHIFT(reg) (((reg) & M2M1_DESC_START_ID_CTL_MASK) >> M2M1_DESC_START_ID_CTL_SHIFT)
#define  M2M1_DESC_START_ID_CTL_REPLACE_VAL(reg,val) (((reg) & ~M2M1_DESC_START_ID_CTL_MASK) | (((uint32_t)val) << M2M1_DESC_START_ID_CTL_SHIFT))

//====================================================================
//Register: Current Link List Address Register (current_desc_addr)
/** \brief The current link list address that the DMA block working on.*/
//====================================================================

#define  M2M1_CURRENT_DESC_ADDR_PTR_MASK 0xfffffffc
#define  M2M1_CURRENT_DESC_ADDR_PTR_SHIFT 2
#define  M2M1_CURRENT_DESC_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_CURRENT_DESC_ADDR_PTR_MASK) >> M2M1_CURRENT_DESC_ADDR_PTR_SHIFT)
#define  M2M1_CURRENT_DESC_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CURRENT_DESC_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_CURRENT_DESC_ADDR_PTR_SHIFT))

#define  M2M1_CURRENT_DESC_ADDR_SOFT_RESET_MASK 0x2
#define  M2M1_CURRENT_DESC_ADDR_SOFT_RESET_SHIFT 1
#define  M2M1_CURRENT_DESC_ADDR_SOFT_RESET_MASK_SHIFT(reg) (((reg) & M2M1_CURRENT_DESC_ADDR_SOFT_RESET_MASK) >> M2M1_CURRENT_DESC_ADDR_SOFT_RESET_SHIFT)
#define  M2M1_CURRENT_DESC_ADDR_SOFT_RESET_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CURRENT_DESC_ADDR_SOFT_RESET_MASK) | (((uint32_t)val) << M2M1_CURRENT_DESC_ADDR_SOFT_RESET_SHIFT))

#define  M2M1_CURRENT_DESC_ADDR_ID_CTL_MASK 0x1
#define  M2M1_CURRENT_DESC_ADDR_ID_CTL_SHIFT 0
#define  M2M1_CURRENT_DESC_ADDR_ID_CTL_MASK_SHIFT(reg) (((reg) & M2M1_CURRENT_DESC_ADDR_ID_CTL_MASK) >> M2M1_CURRENT_DESC_ADDR_ID_CTL_SHIFT)
#define  M2M1_CURRENT_DESC_ADDR_ID_CTL_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CURRENT_DESC_ADDR_ID_CTL_MASK) | (((uint32_t)val) << M2M1_CURRENT_DESC_ADDR_ID_CTL_SHIFT))

//====================================================================
//Register: Status register (status)
/** \brief Status register for the m2m unit.*/
//====================================================================

#define  M2M1_STATUS_CHAN_BUSY_MASK 0xf0000000
#define  M2M1_STATUS_CHAN_BUSY_SHIFT 28
#define  M2M1_STATUS_CHAN_BUSY_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_CHAN_BUSY_MASK) >> M2M1_STATUS_CHAN_BUSY_SHIFT)
#define  M2M1_STATUS_CHAN_BUSY_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_CHAN_BUSY_MASK) | (((uint32_t)val) << M2M1_STATUS_CHAN_BUSY_SHIFT))

#define  M2M1_STATUS_CHAN_STATE_MASK 0xff00000
#define  M2M1_STATUS_CHAN_STATE_SHIFT 20
#define  M2M1_STATUS_CHAN_STATE_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_CHAN_STATE_MASK) >> M2M1_STATUS_CHAN_STATE_SHIFT)
#define  M2M1_STATUS_CHAN_STATE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_CHAN_STATE_MASK) | (((uint32_t)val) << M2M1_STATUS_CHAN_STATE_SHIFT))

#define  M2M1_STATUS_FIFO_ERROR_MASK 0x80000
#define  M2M1_STATUS_FIFO_ERROR_SHIFT 19
#define  M2M1_STATUS_FIFO_ERROR_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_FIFO_ERROR_MASK) >> M2M1_STATUS_FIFO_ERROR_SHIFT)
#define  M2M1_STATUS_FIFO_ERROR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_FIFO_ERROR_MASK) | (((uint32_t)val) << M2M1_STATUS_FIFO_ERROR_SHIFT))

#define  M2M1_STATUS_FIFO_ENABLE_MASK 0x40000
#define  M2M1_STATUS_FIFO_ENABLE_SHIFT 18
#define  M2M1_STATUS_FIFO_ENABLE_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_FIFO_ENABLE_MASK) >> M2M1_STATUS_FIFO_ENABLE_SHIFT)
#define  M2M1_STATUS_FIFO_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_FIFO_ENABLE_MASK) | (((uint32_t)val) << M2M1_STATUS_FIFO_ENABLE_SHIFT))

#define  M2M1_STATUS_FIFO_FULL_MASK 0x20000
#define  M2M1_STATUS_FIFO_FULL_SHIFT 17
#define  M2M1_STATUS_FIFO_FULL_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_FIFO_FULL_MASK) >> M2M1_STATUS_FIFO_FULL_SHIFT)
#define  M2M1_STATUS_FIFO_FULL_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_FIFO_FULL_MASK) | (((uint32_t)val) << M2M1_STATUS_FIFO_FULL_SHIFT))

#define  M2M1_STATUS_FIFO_EMPTY_MASK 0x10000
#define  M2M1_STATUS_FIFO_EMPTY_SHIFT 16
#define  M2M1_STATUS_FIFO_EMPTY_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_FIFO_EMPTY_MASK) >> M2M1_STATUS_FIFO_EMPTY_SHIFT)
#define  M2M1_STATUS_FIFO_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_FIFO_EMPTY_MASK) | (((uint32_t)val) << M2M1_STATUS_FIFO_EMPTY_SHIFT))

#define  M2M1_STATUS_IDLE_MASK 0x8000
#define  M2M1_STATUS_IDLE_SHIFT 15
#define  M2M1_STATUS_IDLE_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_IDLE_MASK) >> M2M1_STATUS_IDLE_SHIFT)
#define  M2M1_STATUS_IDLE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_IDLE_MASK) | (((uint32_t)val) << M2M1_STATUS_IDLE_SHIFT))

#define  M2M1_STATUS_RESERVED1_MASK 0x7c00
#define  M2M1_STATUS_RESERVED1_SHIFT 10
#define  M2M1_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_RESERVED1_MASK) >> M2M1_STATUS_RESERVED1_SHIFT)
#define  M2M1_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_RESERVED1_MASK) | (((uint32_t)val) << M2M1_STATUS_RESERVED1_SHIFT))

#define  M2M1_STATUS_BUFFER_ID_MASK 0x300
#define  M2M1_STATUS_BUFFER_ID_SHIFT 8
#define  M2M1_STATUS_BUFFER_ID_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_BUFFER_ID_MASK) >> M2M1_STATUS_BUFFER_ID_SHIFT)
#define  M2M1_STATUS_BUFFER_ID_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_BUFFER_ID_MASK) | (((uint32_t)val) << M2M1_STATUS_BUFFER_ID_SHIFT))

#define  M2M1_STATUS_BUFFER_LENGTH_MASK 0xff
#define  M2M1_STATUS_BUFFER_LENGTH_SHIFT 0
#define  M2M1_STATUS_BUFFER_LENGTH_MASK_SHIFT(reg) (((reg) & M2M1_STATUS_BUFFER_LENGTH_MASK) >> M2M1_STATUS_BUFFER_LENGTH_SHIFT)
#define  M2M1_STATUS_BUFFER_LENGTH_REPLACE_VAL(reg,val) (((reg) & ~M2M1_STATUS_BUFFER_LENGTH_MASK) | (((uint32_t)val) << M2M1_STATUS_BUFFER_LENGTH_SHIFT))

//====================================================================
//Register: Interrupt Enable (IEN)
/** \brief Enable for various interrupt sources.*/
//====================================================================

#define  M2M1_IEN_RESERVED1_MASK 0xfffffff0
#define  M2M1_IEN_RESERVED1_SHIFT 4
#define  M2M1_IEN_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M1_IEN_RESERVED1_MASK) >> M2M1_IEN_RESERVED1_SHIFT)
#define  M2M1_IEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IEN_RESERVED1_MASK) | (((uint32_t)val) << M2M1_IEN_RESERVED1_SHIFT))

#define  M2M1_IEN_CPU_OWNER_MASK 0x8
#define  M2M1_IEN_CPU_OWNER_SHIFT 3
#define  M2M1_IEN_CPU_OWNER_MASK_SHIFT(reg) (((reg) & M2M1_IEN_CPU_OWNER_MASK) >> M2M1_IEN_CPU_OWNER_SHIFT)
#define  M2M1_IEN_CPU_OWNER_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IEN_CPU_OWNER_MASK) | (((uint32_t)val) << M2M1_IEN_CPU_OWNER_SHIFT))

#define  M2M1_IEN_CHAIN_DONE_MASK 0x4
#define  M2M1_IEN_CHAIN_DONE_SHIFT 2
#define  M2M1_IEN_CHAIN_DONE_MASK_SHIFT(reg) (((reg) & M2M1_IEN_CHAIN_DONE_MASK) >> M2M1_IEN_CHAIN_DONE_SHIFT)
#define  M2M1_IEN_CHAIN_DONE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IEN_CHAIN_DONE_MASK) | (((uint32_t)val) << M2M1_IEN_CHAIN_DONE_SHIFT))

#define  M2M1_IEN_OUT_OF_ORDER_MASK 0x2
#define  M2M1_IEN_OUT_OF_ORDER_SHIFT 1
#define  M2M1_IEN_OUT_OF_ORDER_MASK_SHIFT(reg) (((reg) & M2M1_IEN_OUT_OF_ORDER_MASK) >> M2M1_IEN_OUT_OF_ORDER_SHIFT)
#define  M2M1_IEN_OUT_OF_ORDER_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IEN_OUT_OF_ORDER_MASK) | (((uint32_t)val) << M2M1_IEN_OUT_OF_ORDER_SHIFT))

#define  M2M1_IEN_TERM_CNT_MASK 0x1
#define  M2M1_IEN_TERM_CNT_SHIFT 0
#define  M2M1_IEN_TERM_CNT_MASK_SHIFT(reg) (((reg) & M2M1_IEN_TERM_CNT_MASK) >> M2M1_IEN_TERM_CNT_SHIFT)
#define  M2M1_IEN_TERM_CNT_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IEN_TERM_CNT_MASK) | (((uint32_t)val) << M2M1_IEN_TERM_CNT_SHIFT))

//====================================================================
//Register: Interrupt Pending (IPEND)
/** \brief Read-Only interrupt pending register.*/
//====================================================================

#define  M2M1_IPEND_RESERVED1_MASK 0xfffffff0
#define  M2M1_IPEND_RESERVED1_SHIFT 4
#define  M2M1_IPEND_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M1_IPEND_RESERVED1_MASK) >> M2M1_IPEND_RESERVED1_SHIFT)
#define  M2M1_IPEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IPEND_RESERVED1_MASK) | (((uint32_t)val) << M2M1_IPEND_RESERVED1_SHIFT))

#define  M2M1_IPEND_CPU_OWNER_PEND_MASK 0x8
#define  M2M1_IPEND_CPU_OWNER_PEND_SHIFT 3
#define  M2M1_IPEND_CPU_OWNER_PEND_MASK_SHIFT(reg) (((reg) & M2M1_IPEND_CPU_OWNER_PEND_MASK) >> M2M1_IPEND_CPU_OWNER_PEND_SHIFT)
#define  M2M1_IPEND_CPU_OWNER_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IPEND_CPU_OWNER_PEND_MASK) | (((uint32_t)val) << M2M1_IPEND_CPU_OWNER_PEND_SHIFT))

#define  M2M1_IPEND_CHAIN_DONE_PEND_MASK 0x4
#define  M2M1_IPEND_CHAIN_DONE_PEND_SHIFT 2
#define  M2M1_IPEND_CHAIN_DONE_PEND_MASK_SHIFT(reg) (((reg) & M2M1_IPEND_CHAIN_DONE_PEND_MASK) >> M2M1_IPEND_CHAIN_DONE_PEND_SHIFT)
#define  M2M1_IPEND_CHAIN_DONE_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IPEND_CHAIN_DONE_PEND_MASK) | (((uint32_t)val) << M2M1_IPEND_CHAIN_DONE_PEND_SHIFT))

#define  M2M1_IPEND_OUT_OF_ORDER_PEND_MASK 0x2
#define  M2M1_IPEND_OUT_OF_ORDER_PEND_SHIFT 1
#define  M2M1_IPEND_OUT_OF_ORDER_PEND_MASK_SHIFT(reg) (((reg) & M2M1_IPEND_OUT_OF_ORDER_PEND_MASK) >> M2M1_IPEND_OUT_OF_ORDER_PEND_SHIFT)
#define  M2M1_IPEND_OUT_OF_ORDER_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IPEND_OUT_OF_ORDER_PEND_MASK) | (((uint32_t)val) << M2M1_IPEND_OUT_OF_ORDER_PEND_SHIFT))

#define  M2M1_IPEND_TERM_CNT_PEND_MASK 0x1
#define  M2M1_IPEND_TERM_CNT_PEND_SHIFT 0
#define  M2M1_IPEND_TERM_CNT_PEND_MASK_SHIFT(reg) (((reg) & M2M1_IPEND_TERM_CNT_PEND_MASK) >> M2M1_IPEND_TERM_CNT_PEND_SHIFT)
#define  M2M1_IPEND_TERM_CNT_PEND_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IPEND_TERM_CNT_PEND_MASK) | (((uint32_t)val) << M2M1_IPEND_TERM_CNT_PEND_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge (IACK)
/** \brief Write-Only interrupt acknowledge register.*/
//====================================================================

#define  M2M1_IACK_RESERVED1_MASK 0xfffffff0
#define  M2M1_IACK_RESERVED1_SHIFT 4
#define  M2M1_IACK_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M1_IACK_RESERVED1_MASK) >> M2M1_IACK_RESERVED1_SHIFT)
#define  M2M1_IACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IACK_RESERVED1_MASK) | (((uint32_t)val) << M2M1_IACK_RESERVED1_SHIFT))

#define  M2M1_IACK_CPU_OWNER_ACK_MASK 0x8
#define  M2M1_IACK_CPU_OWNER_ACK_SHIFT 3
#define  M2M1_IACK_CPU_OWNER_ACK_MASK_SHIFT(reg) (((reg) & M2M1_IACK_CPU_OWNER_ACK_MASK) >> M2M1_IACK_CPU_OWNER_ACK_SHIFT)
#define  M2M1_IACK_CPU_OWNER_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IACK_CPU_OWNER_ACK_MASK) | (((uint32_t)val) << M2M1_IACK_CPU_OWNER_ACK_SHIFT))

#define  M2M1_IACK_CHAIN_DONE_ACK_MASK 0x4
#define  M2M1_IACK_CHAIN_DONE_ACK_SHIFT 2
#define  M2M1_IACK_CHAIN_DONE_ACK_MASK_SHIFT(reg) (((reg) & M2M1_IACK_CHAIN_DONE_ACK_MASK) >> M2M1_IACK_CHAIN_DONE_ACK_SHIFT)
#define  M2M1_IACK_CHAIN_DONE_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IACK_CHAIN_DONE_ACK_MASK) | (((uint32_t)val) << M2M1_IACK_CHAIN_DONE_ACK_SHIFT))

#define  M2M1_IACK_OUT_OF_ORDER_ACK_MASK 0x2
#define  M2M1_IACK_OUT_OF_ORDER_ACK_SHIFT 1
#define  M2M1_IACK_OUT_OF_ORDER_ACK_MASK_SHIFT(reg) (((reg) & M2M1_IACK_OUT_OF_ORDER_ACK_MASK) >> M2M1_IACK_OUT_OF_ORDER_ACK_SHIFT)
#define  M2M1_IACK_OUT_OF_ORDER_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IACK_OUT_OF_ORDER_ACK_MASK) | (((uint32_t)val) << M2M1_IACK_OUT_OF_ORDER_ACK_SHIFT))

#define  M2M1_IACK_TERM_CNT_ACK_MASK 0x1
#define  M2M1_IACK_TERM_CNT_ACK_SHIFT 0
#define  M2M1_IACK_TERM_CNT_ACK_MASK_SHIFT(reg) (((reg) & M2M1_IACK_TERM_CNT_ACK_MASK) >> M2M1_IACK_TERM_CNT_ACK_SHIFT)
#define  M2M1_IACK_TERM_CNT_ACK_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IACK_TERM_CNT_ACK_MASK) | (((uint32_t)val) << M2M1_IACK_TERM_CNT_ACK_SHIFT))

//====================================================================
//Register: Interrupt Force (IFOR)
/** \brief Forcing interrupt sources register. Write a one to each bit will force
               the corresponding interrupt source to be asserted.*/
//====================================================================

#define  M2M1_IFOR_RESERVED1_MASK 0xfffffff0
#define  M2M1_IFOR_RESERVED1_SHIFT 4
#define  M2M1_IFOR_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M1_IFOR_RESERVED1_MASK) >> M2M1_IFOR_RESERVED1_SHIFT)
#define  M2M1_IFOR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IFOR_RESERVED1_MASK) | (((uint32_t)val) << M2M1_IFOR_RESERVED1_SHIFT))

#define  M2M1_IFOR_CPU_OWNER_FORCE_MASK 0x8
#define  M2M1_IFOR_CPU_OWNER_FORCE_SHIFT 3
#define  M2M1_IFOR_CPU_OWNER_FORCE_MASK_SHIFT(reg) (((reg) & M2M1_IFOR_CPU_OWNER_FORCE_MASK) >> M2M1_IFOR_CPU_OWNER_FORCE_SHIFT)
#define  M2M1_IFOR_CPU_OWNER_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IFOR_CPU_OWNER_FORCE_MASK) | (((uint32_t)val) << M2M1_IFOR_CPU_OWNER_FORCE_SHIFT))

#define  M2M1_IFOR_CHAIN_DONE_FORCE_MASK 0x4
#define  M2M1_IFOR_CHAIN_DONE_FORCE_SHIFT 2
#define  M2M1_IFOR_CHAIN_DONE_FORCE_MASK_SHIFT(reg) (((reg) & M2M1_IFOR_CHAIN_DONE_FORCE_MASK) >> M2M1_IFOR_CHAIN_DONE_FORCE_SHIFT)
#define  M2M1_IFOR_CHAIN_DONE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IFOR_CHAIN_DONE_FORCE_MASK) | (((uint32_t)val) << M2M1_IFOR_CHAIN_DONE_FORCE_SHIFT))

#define  M2M1_IFOR_OUT_OF_ORDER_FORCE_MASK 0x2
#define  M2M1_IFOR_OUT_OF_ORDER_FORCE_SHIFT 1
#define  M2M1_IFOR_OUT_OF_ORDER_FORCE_MASK_SHIFT(reg) (((reg) & M2M1_IFOR_OUT_OF_ORDER_FORCE_MASK) >> M2M1_IFOR_OUT_OF_ORDER_FORCE_SHIFT)
#define  M2M1_IFOR_OUT_OF_ORDER_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IFOR_OUT_OF_ORDER_FORCE_MASK) | (((uint32_t)val) << M2M1_IFOR_OUT_OF_ORDER_FORCE_SHIFT))

#define  M2M1_IFOR_TERM_CNT_FORCE_MASK 0x1
#define  M2M1_IFOR_TERM_CNT_FORCE_SHIFT 0
#define  M2M1_IFOR_TERM_CNT_FORCE_MASK_SHIFT(reg) (((reg) & M2M1_IFOR_TERM_CNT_FORCE_MASK) >> M2M1_IFOR_TERM_CNT_FORCE_SHIFT)
#define  M2M1_IFOR_TERM_CNT_FORCE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_IFOR_TERM_CNT_FORCE_MASK) | (((uint32_t)val) << M2M1_IFOR_TERM_CNT_FORCE_SHIFT))

//====================================================================
//Register: Descriptor control register (control)
/** \brief Read the active Descriptor Control Register.*/
//====================================================================

#define  M2M1_CONTROL_OWN_MASK 0x80000000
#define  M2M1_CONTROL_OWN_SHIFT 31
#define  M2M1_CONTROL_OWN_MASK_SHIFT(reg) (((reg) & M2M1_CONTROL_OWN_MASK) >> M2M1_CONTROL_OWN_SHIFT)
#define  M2M1_CONTROL_OWN_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CONTROL_OWN_MASK) | (((uint32_t)val) << M2M1_CONTROL_OWN_SHIFT))

#define  M2M1_CONTROL_UPDATE_XLATE_MASK 0x40000000
#define  M2M1_CONTROL_UPDATE_XLATE_SHIFT 30
#define  M2M1_CONTROL_UPDATE_XLATE_MASK_SHIFT(reg) (((reg) & M2M1_CONTROL_UPDATE_XLATE_MASK) >> M2M1_CONTROL_UPDATE_XLATE_SHIFT)
#define  M2M1_CONTROL_UPDATE_XLATE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CONTROL_UPDATE_XLATE_MASK) | (((uint32_t)val) << M2M1_CONTROL_UPDATE_XLATE_SHIFT))

#define  M2M1_CONTROL_RESERVED1_MASK 0x3fff0000
#define  M2M1_CONTROL_RESERVED1_SHIFT 16
#define  M2M1_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & M2M1_CONTROL_RESERVED1_MASK) >> M2M1_CONTROL_RESERVED1_SHIFT)
#define  M2M1_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << M2M1_CONTROL_RESERVED1_SHIFT))

#define  M2M1_CONTROL_SIZE_MASK 0xffff
#define  M2M1_CONTROL_SIZE_SHIFT 0
#define  M2M1_CONTROL_SIZE_MASK_SHIFT(reg) (((reg) & M2M1_CONTROL_SIZE_MASK) >> M2M1_CONTROL_SIZE_SHIFT)
#define  M2M1_CONTROL_SIZE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CONTROL_SIZE_MASK) | (((uint32_t)val) << M2M1_CONTROL_SIZE_SHIFT))

//====================================================================
//Register: Descriptor Current Source Address (src_addr)
/** \brief Read the active Descriptor Source Address pointer.*/
//====================================================================

#define  M2M1_SRC_ADDR_PTR_MASK 0xffffffff
#define  M2M1_SRC_ADDR_PTR_SHIFT 0
#define  M2M1_SRC_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_SRC_ADDR_PTR_MASK) >> M2M1_SRC_ADDR_PTR_SHIFT)
#define  M2M1_SRC_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_SRC_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_SRC_ADDR_PTR_SHIFT))

//====================================================================
//Register: Descriptor Current Destination Address (dest_addr)
/** \brief Read the active Descriptor Destination Address pointer.*/
//====================================================================

#define  M2M1_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M1_DEST_ADDR_PTR_SHIFT 0
#define  M2M1_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_DEST_ADDR_PTR_MASK) >> M2M1_DEST_ADDR_PTR_SHIFT)
#define  M2M1_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Descriptor PCIe outbound Translate Value (pcie_outbound)
/** \brief Read the active PCIe outbound translate register value.*/
//====================================================================

#define  M2M1_PCIE_OUTBOUND_XLATE_VALUE_MASK 0xffffffff
#define  M2M1_PCIE_OUTBOUND_XLATE_VALUE_SHIFT 0
#define  M2M1_PCIE_OUTBOUND_XLATE_VALUE_MASK_SHIFT(reg) (((reg) & M2M1_PCIE_OUTBOUND_XLATE_VALUE_MASK) >> M2M1_PCIE_OUTBOUND_XLATE_VALUE_SHIFT)
#define  M2M1_PCIE_OUTBOUND_XLATE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~M2M1_PCIE_OUTBOUND_XLATE_VALUE_MASK) | (((uint32_t)val) << M2M1_PCIE_OUTBOUND_XLATE_VALUE_SHIFT))

//====================================================================
//Register: Descriptor Linked List Address (list_addr)
/** \brief Read the active Descriptor Linked List pointer (to the next descriptor).*/
//====================================================================

#define  M2M1_LIST_ADDR_PTR_MASK 0xffffffff
#define  M2M1_LIST_ADDR_PTR_SHIFT 0
#define  M2M1_LIST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_LIST_ADDR_PTR_MASK) >> M2M1_LIST_ADDR_PTR_SHIFT)
#define  M2M1_LIST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_LIST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_LIST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 0 Destination Address (chan0_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M1_CHAN0_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M1_CHAN0_DEST_ADDR_PTR_SHIFT 0
#define  M2M1_CHAN0_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_CHAN0_DEST_ADDR_PTR_MASK) >> M2M1_CHAN0_DEST_ADDR_PTR_SHIFT)
#define  M2M1_CHAN0_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CHAN0_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_CHAN0_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 1 Destination Address (chan1_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M1_CHAN1_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M1_CHAN1_DEST_ADDR_PTR_SHIFT 0
#define  M2M1_CHAN1_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_CHAN1_DEST_ADDR_PTR_MASK) >> M2M1_CHAN1_DEST_ADDR_PTR_SHIFT)
#define  M2M1_CHAN1_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CHAN1_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_CHAN1_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 2 Destination Address (chan2_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M1_CHAN2_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M1_CHAN2_DEST_ADDR_PTR_SHIFT 0
#define  M2M1_CHAN2_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_CHAN2_DEST_ADDR_PTR_MASK) >> M2M1_CHAN2_DEST_ADDR_PTR_SHIFT)
#define  M2M1_CHAN2_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CHAN2_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_CHAN2_DEST_ADDR_PTR_SHIFT))

//====================================================================
//Register: Channel 3 Destination Address (chan3_dest_addr)
/** \brief Read the active Channel Destination Address pointer.*/
//====================================================================

#define  M2M1_CHAN3_DEST_ADDR_PTR_MASK 0xffffffff
#define  M2M1_CHAN3_DEST_ADDR_PTR_SHIFT 0
#define  M2M1_CHAN3_DEST_ADDR_PTR_MASK_SHIFT(reg) (((reg) & M2M1_CHAN3_DEST_ADDR_PTR_MASK) >> M2M1_CHAN3_DEST_ADDR_PTR_SHIFT)
#define  M2M1_CHAN3_DEST_ADDR_PTR_REPLACE_VAL(reg,val) (((reg) & ~M2M1_CHAN3_DEST_ADDR_PTR_MASK) | (((uint32_t)val) << M2M1_CHAN3_DEST_ADDR_PTR_SHIFT))

#endif // M2M_TOP
