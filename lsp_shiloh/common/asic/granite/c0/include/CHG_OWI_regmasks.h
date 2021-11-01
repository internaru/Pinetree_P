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
 * \file CHG_OWI_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CHG_OWI_REGMASKS_H_
#define _CHG_OWI_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CHG OWI (CHG_OWI)
/** \brief Registers for the 1-Wire Bus Master block*/
//
//====================================================================

//====================================================================
//Register: OneWire Command Register (W1CMDR)
/** \brief This register controls the functionality of the 1-Wire bus master controller.*/
//====================================================================

#define  CHG_OWI_W1CMDR_RESERVED1_MASK 0xfffffff0
#define  CHG_OWI_W1CMDR_RESERVED1_SHIFT 4
#define  CHG_OWI_W1CMDR_RESERVED1_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CMDR_RESERVED1_MASK) >> CHG_OWI_W1CMDR_RESERVED1_SHIFT)
#define  CHG_OWI_W1CMDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CMDR_RESERVED1_MASK) | (((uint32_t)val) << CHG_OWI_W1CMDR_RESERVED1_SHIFT))

#define  CHG_OWI_W1CMDR_DQI_MASK 0x8
#define  CHG_OWI_W1CMDR_DQI_SHIFT 3
#define  CHG_OWI_W1CMDR_DQI_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CMDR_DQI_MASK) >> CHG_OWI_W1CMDR_DQI_SHIFT)
#define  CHG_OWI_W1CMDR_DQI_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CMDR_DQI_MASK) | (((uint32_t)val) << CHG_OWI_W1CMDR_DQI_SHIFT))

#define  CHG_OWI_W1CMDR_DQO_MASK 0x4
#define  CHG_OWI_W1CMDR_DQO_SHIFT 2
#define  CHG_OWI_W1CMDR_DQO_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CMDR_DQO_MASK) >> CHG_OWI_W1CMDR_DQO_SHIFT)
#define  CHG_OWI_W1CMDR_DQO_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CMDR_DQO_MASK) | (((uint32_t)val) << CHG_OWI_W1CMDR_DQO_SHIFT))

#define  CHG_OWI_W1CMDR_SRA_MASK 0x2
#define  CHG_OWI_W1CMDR_SRA_SHIFT 1
#define  CHG_OWI_W1CMDR_SRA_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CMDR_SRA_MASK) >> CHG_OWI_W1CMDR_SRA_SHIFT)
#define  CHG_OWI_W1CMDR_SRA_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CMDR_SRA_MASK) | (((uint32_t)val) << CHG_OWI_W1CMDR_SRA_SHIFT))

#define  CHG_OWI_W1CMDR_BUSRESET_MASK 0x1
#define  CHG_OWI_W1CMDR_BUSRESET_SHIFT 0
#define  CHG_OWI_W1CMDR_BUSRESET_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CMDR_BUSRESET_MASK) >> CHG_OWI_W1CMDR_BUSRESET_SHIFT)
#define  CHG_OWI_W1CMDR_BUSRESET_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CMDR_BUSRESET_MASK) | (((uint32_t)val) << CHG_OWI_W1CMDR_BUSRESET_SHIFT))

//====================================================================
//Register: OneWire Transmit/Receive Buffer (W1TRR)
/** \brief Data sent and received from the 1-Wire bus master interface controller passes through the transmit/receive buffer location. The 1-Wire bus master interface controller is double-buffered with separate transmit and receive buffers. Writing to this location connects the transmit buffer to the data bus, while reading connects the receive buffer to the data bus.*/
//====================================================================

#define  CHG_OWI_W1TRR_RESERVED1_MASK 0xffffff00
#define  CHG_OWI_W1TRR_RESERVED1_SHIFT 8
#define  CHG_OWI_W1TRR_RESERVED1_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1TRR_RESERVED1_MASK) >> CHG_OWI_W1TRR_RESERVED1_SHIFT)
#define  CHG_OWI_W1TRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1TRR_RESERVED1_MASK) | (((uint32_t)val) << CHG_OWI_W1TRR_RESERVED1_SHIFT))

#define  CHG_OWI_W1TRR_DATA_MASK 0xff
#define  CHG_OWI_W1TRR_DATA_SHIFT 0
#define  CHG_OWI_W1TRR_DATA_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1TRR_DATA_MASK) >> CHG_OWI_W1TRR_DATA_SHIFT)
#define  CHG_OWI_W1TRR_DATA_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1TRR_DATA_MASK) | (((uint32_t)val) << CHG_OWI_W1TRR_DATA_SHIFT))

//====================================================================
//Register: OneWire Interrupt Register (W1INTR)
/** \brief This read-only register contains flags from transmit, receive, and 1-Wire reset operations. The presence-detect (PD) flag bit is cleared when the W1INTR is read; the other flag bits are cleared automatically when the transmit and receive buffers are written to or read, respectively.
            These flags can generate an interrupt if the corresponding enable bit is set in the Interrupt Enable register (W1IER).
            IMPORTANT NOTE: Reading this register will trigger clearing of the [physical] interrupt output line (to the system interrupt controller). The block can take up to 2us to actually clear the interrupt output line, even when the W1IER bit is cleared to disable the interrupt!*/
//====================================================================

#define  CHG_OWI_W1INTR_RESERVED1_MASK 0xffffffc0
#define  CHG_OWI_W1INTR_RESERVED1_SHIFT 6
#define  CHG_OWI_W1INTR_RESERVED1_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1INTR_RESERVED1_MASK) >> CHG_OWI_W1INTR_RESERVED1_SHIFT)
#define  CHG_OWI_W1INTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1INTR_RESERVED1_MASK) | (((uint32_t)val) << CHG_OWI_W1INTR_RESERVED1_SHIFT))

#define  CHG_OWI_W1INTR_RSF_MASK 0x20
#define  CHG_OWI_W1INTR_RSF_SHIFT 5
#define  CHG_OWI_W1INTR_RSF_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1INTR_RSF_MASK) >> CHG_OWI_W1INTR_RSF_SHIFT)
#define  CHG_OWI_W1INTR_RSF_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1INTR_RSF_MASK) | (((uint32_t)val) << CHG_OWI_W1INTR_RSF_SHIFT))

#define  CHG_OWI_W1INTR_RBF_MASK 0x10
#define  CHG_OWI_W1INTR_RBF_SHIFT 4
#define  CHG_OWI_W1INTR_RBF_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1INTR_RBF_MASK) >> CHG_OWI_W1INTR_RBF_SHIFT)
#define  CHG_OWI_W1INTR_RBF_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1INTR_RBF_MASK) | (((uint32_t)val) << CHG_OWI_W1INTR_RBF_SHIFT))

#define  CHG_OWI_W1INTR_TEMT_MASK 0x8
#define  CHG_OWI_W1INTR_TEMT_SHIFT 3
#define  CHG_OWI_W1INTR_TEMT_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1INTR_TEMT_MASK) >> CHG_OWI_W1INTR_TEMT_SHIFT)
#define  CHG_OWI_W1INTR_TEMT_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1INTR_TEMT_MASK) | (((uint32_t)val) << CHG_OWI_W1INTR_TEMT_SHIFT))

#define  CHG_OWI_W1INTR_TBE_MASK 0x4
#define  CHG_OWI_W1INTR_TBE_SHIFT 2
#define  CHG_OWI_W1INTR_TBE_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1INTR_TBE_MASK) >> CHG_OWI_W1INTR_TBE_SHIFT)
#define  CHG_OWI_W1INTR_TBE_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1INTR_TBE_MASK) | (((uint32_t)val) << CHG_OWI_W1INTR_TBE_SHIFT))

#define  CHG_OWI_W1INTR_PDR_MASK 0x2
#define  CHG_OWI_W1INTR_PDR_SHIFT 1
#define  CHG_OWI_W1INTR_PDR_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1INTR_PDR_MASK) >> CHG_OWI_W1INTR_PDR_SHIFT)
#define  CHG_OWI_W1INTR_PDR_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1INTR_PDR_MASK) | (((uint32_t)val) << CHG_OWI_W1INTR_PDR_SHIFT))

#define  CHG_OWI_W1INTR_PD_MASK 0x1
#define  CHG_OWI_W1INTR_PD_SHIFT 0
#define  CHG_OWI_W1INTR_PD_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1INTR_PD_MASK) >> CHG_OWI_W1INTR_PD_SHIFT)
#define  CHG_OWI_W1INTR_PD_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1INTR_PD_MASK) | (((uint32_t)val) << CHG_OWI_W1INTR_PD_SHIFT))

//====================================================================
//Register: OneWire Interrupt Enable Register (W1IER)
/** \brief This register allows system programmers to specify which of the interrupt sources causes an interrupt. When a reset is received, all non-reserved bits in this register except for the IAS bit are cleared to 0, disabling all interrupt sources. The IAS bit is reset to 1 by the 1-Wire controller.
            IMPORTANT NOTE: When an interrupt condition is in an enabled state (W1IER register bit is set), writing a 0 to a W1IER register bit will disable the interrupt condition and can clear the interrupt output line, but it can the block up to 2us to actually clear the interrupt line!*/
//====================================================================

#define  CHG_OWI_W1IER_RESERVED1_MASK 0xffffff00
#define  CHG_OWI_W1IER_RESERVED1_SHIFT 8
#define  CHG_OWI_W1IER_RESERVED1_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_RESERVED1_MASK) >> CHG_OWI_W1IER_RESERVED1_SHIFT)
#define  CHG_OWI_W1IER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_RESERVED1_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_RESERVED1_SHIFT))

#define  CHG_OWI_W1IER_DQOE_MASK 0x80
#define  CHG_OWI_W1IER_DQOE_SHIFT 7
#define  CHG_OWI_W1IER_DQOE_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_DQOE_MASK) >> CHG_OWI_W1IER_DQOE_SHIFT)
#define  CHG_OWI_W1IER_DQOE_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_DQOE_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_DQOE_SHIFT))

#define  CHG_OWI_W1IER_RESERVED2_MASK 0x40
#define  CHG_OWI_W1IER_RESERVED2_SHIFT 6
#define  CHG_OWI_W1IER_RESERVED2_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_RESERVED2_MASK) >> CHG_OWI_W1IER_RESERVED2_SHIFT)
#define  CHG_OWI_W1IER_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_RESERVED2_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_RESERVED2_SHIFT))

#define  CHG_OWI_W1IER_ERSF_MASK 0x20
#define  CHG_OWI_W1IER_ERSF_SHIFT 5
#define  CHG_OWI_W1IER_ERSF_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_ERSF_MASK) >> CHG_OWI_W1IER_ERSF_SHIFT)
#define  CHG_OWI_W1IER_ERSF_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_ERSF_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_ERSF_SHIFT))

#define  CHG_OWI_W1IER_ERBF_MASK 0x10
#define  CHG_OWI_W1IER_ERBF_SHIFT 4
#define  CHG_OWI_W1IER_ERBF_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_ERBF_MASK) >> CHG_OWI_W1IER_ERBF_SHIFT)
#define  CHG_OWI_W1IER_ERBF_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_ERBF_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_ERBF_SHIFT))

#define  CHG_OWI_W1IER_ETMT_MASK 0x8
#define  CHG_OWI_W1IER_ETMT_SHIFT 3
#define  CHG_OWI_W1IER_ETMT_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_ETMT_MASK) >> CHG_OWI_W1IER_ETMT_SHIFT)
#define  CHG_OWI_W1IER_ETMT_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_ETMT_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_ETMT_SHIFT))

#define  CHG_OWI_W1IER_ETBE_MASK 0x4
#define  CHG_OWI_W1IER_ETBE_SHIFT 2
#define  CHG_OWI_W1IER_ETBE_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_ETBE_MASK) >> CHG_OWI_W1IER_ETBE_SHIFT)
#define  CHG_OWI_W1IER_ETBE_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_ETBE_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_ETBE_SHIFT))

#define  CHG_OWI_W1IER_IAS_MASK 0x2
#define  CHG_OWI_W1IER_IAS_SHIFT 1
#define  CHG_OWI_W1IER_IAS_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_IAS_MASK) >> CHG_OWI_W1IER_IAS_SHIFT)
#define  CHG_OWI_W1IER_IAS_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_IAS_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_IAS_SHIFT))

#define  CHG_OWI_W1IER_EPD_MASK 0x1
#define  CHG_OWI_W1IER_EPD_SHIFT 0
#define  CHG_OWI_W1IER_EPD_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1IER_EPD_MASK) >> CHG_OWI_W1IER_EPD_SHIFT)
#define  CHG_OWI_W1IER_EPD_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1IER_EPD_MASK) | (((uint32_t)val) << CHG_OWI_W1IER_EPD_SHIFT))

//====================================================================
//Register: OneWire Clock Divisor Register (W1CDR)
/** \brief The block requires a 1us timebase for timing the 1-Wire bus pulses (a.k.a. write-one and write-zero time slots of the iButton Standard). The timebase can be internally generated or input from external source, and this is hard configured when the block is instantiated in the asic.
            This register prescales and divides the reference clock to generate an internal 1us timebase. This register must be programmed before using the 1-Wire bus master interface. This register value is only used for the internal timebase configuration.
            If the block has been configured to input an external timebase, the register value is ignored.*/
//====================================================================

#define  CHG_OWI_W1CDR_RESERVED1_MASK 0xffffffe0
#define  CHG_OWI_W1CDR_RESERVED1_SHIFT 5
#define  CHG_OWI_W1CDR_RESERVED1_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CDR_RESERVED1_MASK) >> CHG_OWI_W1CDR_RESERVED1_SHIFT)
#define  CHG_OWI_W1CDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CDR_RESERVED1_MASK) | (((uint32_t)val) << CHG_OWI_W1CDR_RESERVED1_SHIFT))

#define  CHG_OWI_W1CDR_DIV_MASK 0x1c
#define  CHG_OWI_W1CDR_DIV_SHIFT 2
#define  CHG_OWI_W1CDR_DIV_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CDR_DIV_MASK) >> CHG_OWI_W1CDR_DIV_SHIFT)
#define  CHG_OWI_W1CDR_DIV_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CDR_DIV_MASK) | (((uint32_t)val) << CHG_OWI_W1CDR_DIV_SHIFT))

#define  CHG_OWI_W1CDR_PRE_MASK 0x3
#define  CHG_OWI_W1CDR_PRE_SHIFT 0
#define  CHG_OWI_W1CDR_PRE_MASK_SHIFT(reg) (((reg) & CHG_OWI_W1CDR_PRE_MASK) >> CHG_OWI_W1CDR_PRE_SHIFT)
#define  CHG_OWI_W1CDR_PRE_REPLACE_VAL(reg,val) (((reg) & ~CHG_OWI_W1CDR_PRE_MASK) | (((uint32_t)val) << CHG_OWI_W1CDR_PRE_SHIFT))

#endif // CHG_OWI
