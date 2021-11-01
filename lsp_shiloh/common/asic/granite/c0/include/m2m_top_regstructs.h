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
 * \file m2m_top_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _M2M_TOP_REGSTRUCTS_H_
#define _M2M_TOP_REGSTRUCTS_H_

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

typedef struct M2M0_REGS_s
{
  volatile uint32_t desc_start;  ///< 0x0 [W]: Link List Index Register
  volatile uint32_t current_desc_addr;  ///< 0x4 [R]: Current Link List Address Register
  volatile uint32_t status;  ///< 0x8 [R]: Status register
  volatile uint32_t IEN;  ///< 0xc [R/W]: Interrupt Enable
  volatile uint32_t IPEND;  ///< 0x10 [R]: Interrupt Pending
  volatile uint32_t IACK;  ///< 0x14 [W]: Interrupt Acknowledge
  volatile uint32_t IFOR;  ///< 0x18 [R/W]: Interrupt Force
  volatile uint32_t control;  ///< 0x1c [R]: Descriptor control register
  volatile uint32_t src_addr;  ///< 0x20 [R]: Descriptor Current Source Address
  volatile uint32_t dest_addr;  ///< 0x24 [R]: Descriptor Current Destination Address
  volatile uint32_t pcie_outbound;  ///< 0x28 [R]: Descriptor PCIe outbound Translate Value
  volatile uint32_t list_addr;  ///< 0x2c [R]: Descriptor Linked List Address
  volatile uint32_t chan0_dest_addr;  ///< 0x30 [R]: Channel 0 Destination Address
  volatile uint32_t chan1_dest_addr;  ///< 0x34 [R]: Channel 1 Destination Address
  volatile uint32_t chan2_dest_addr;  ///< 0x38 [R]: Channel 2 Destination Address
  volatile uint32_t chan3_dest_addr;  ///< 0x3c [R]: Channel 3 Destination Address
} M2M0_REGS_t;

//====================================================================
//
//Register File: M2M Registers (m2m1)
//
//====================================================================

typedef struct M2M1_REGS_s
{
  volatile uint32_t desc_start;  ///< 0x0 [W]: Link List Index Register
  volatile uint32_t current_desc_addr;  ///< 0x4 [R]: Current Link List Address Register
  volatile uint32_t status;  ///< 0x8 [R]: Status register
  volatile uint32_t IEN;  ///< 0xc [R/W]: Interrupt Enable
  volatile uint32_t IPEND;  ///< 0x10 [R]: Interrupt Pending
  volatile uint32_t IACK;  ///< 0x14 [W]: Interrupt Acknowledge
  volatile uint32_t IFOR;  ///< 0x18 [R/W]: Interrupt Force
  volatile uint32_t control;  ///< 0x1c [R]: Descriptor control register
  volatile uint32_t src_addr;  ///< 0x20 [R]: Descriptor Current Source Address
  volatile uint32_t dest_addr;  ///< 0x24 [R]: Descriptor Current Destination Address
  volatile uint32_t pcie_outbound;  ///< 0x28 [R]: Descriptor PCIe outbound Translate Value
  volatile uint32_t list_addr;  ///< 0x2c [R]: Descriptor Linked List Address
  volatile uint32_t chan0_dest_addr;  ///< 0x30 [R]: Channel 0 Destination Address
  volatile uint32_t chan1_dest_addr;  ///< 0x34 [R]: Channel 1 Destination Address
  volatile uint32_t chan2_dest_addr;  ///< 0x38 [R]: Channel 2 Destination Address
  volatile uint32_t chan3_dest_addr;  ///< 0x3c [R]: Channel 3 Destination Address
} M2M1_REGS_t;

#endif // M2M_TOP
