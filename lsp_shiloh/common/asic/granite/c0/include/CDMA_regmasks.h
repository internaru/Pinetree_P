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
 * \file CDMA_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CDMA_REGMASKS_H_
#define _CDMA_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Central DMA registers (CDMA)
/** \brief Registers for Central DMA module*/
//
//====================================================================

//====================================================================
//Register: DMA Configuration Register (CFG)
/** \brief This register is used to configure the data flow for each channel.*/
//====================================================================

#define  CDMA_CFG_WRDELAY_MASK 0xf0000000
#define  CDMA_CFG_WRDELAY_SHIFT 28
#define  CDMA_CFG_WRDELAY_MASK_SHIFT(reg) (((reg) & CDMA_CFG_WRDELAY_MASK) >> CDMA_CFG_WRDELAY_SHIFT)
#define  CDMA_CFG_WRDELAY_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_WRDELAY_MASK) | (((uint32_t)val) << CDMA_CFG_WRDELAY_SHIFT))

#define  CDMA_CFG_RESERVED1_MASK 0xc000000
#define  CDMA_CFG_RESERVED1_SHIFT 26
#define  CDMA_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_CFG_RESERVED1_MASK) >> CDMA_CFG_RESERVED1_SHIFT)
#define  CDMA_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_RESERVED1_MASK) | (((uint32_t)val) << CDMA_CFG_RESERVED1_SHIFT))

#define  CDMA_CFG_DATAUNITSIZE_MASK 0x3000000
#define  CDMA_CFG_DATAUNITSIZE_SHIFT 24
#define  CDMA_CFG_DATAUNITSIZE_MASK_SHIFT(reg) (((reg) & CDMA_CFG_DATAUNITSIZE_MASK) >> CDMA_CFG_DATAUNITSIZE_SHIFT)
#define  CDMA_CFG_DATAUNITSIZE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_DATAUNITSIZE_MASK) | (((uint32_t)val) << CDMA_CFG_DATAUNITSIZE_SHIFT))

#define  CDMA_CFG_HALT_MASK 0x800000
#define  CDMA_CFG_HALT_SHIFT 23
#define  CDMA_CFG_HALT_MASK_SHIFT(reg) (((reg) & CDMA_CFG_HALT_MASK) >> CDMA_CFG_HALT_SHIFT)
#define  CDMA_CFG_HALT_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_HALT_MASK) | (((uint32_t)val) << CDMA_CFG_HALT_SHIFT))

#define  CDMA_CFG_FLOWCTRL_MASK 0x700000
#define  CDMA_CFG_FLOWCTRL_SHIFT 20
#define  CDMA_CFG_FLOWCTRL_MASK_SHIFT(reg) (((reg) & CDMA_CFG_FLOWCTRL_MASK) >> CDMA_CFG_FLOWCTRL_SHIFT)
#define  CDMA_CFG_FLOWCTRL_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_FLOWCTRL_MASK) | (((uint32_t)val) << CDMA_CFG_FLOWCTRL_SHIFT))

#define  CDMA_CFG_RESERVED2_MASK 0xc0000
#define  CDMA_CFG_RESERVED2_SHIFT 18
#define  CDMA_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & CDMA_CFG_RESERVED2_MASK) >> CDMA_CFG_RESERVED2_SHIFT)
#define  CDMA_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_RESERVED2_MASK) | (((uint32_t)val) << CDMA_CFG_RESERVED2_SHIFT))

#define  CDMA_CFG_DESTPID_MASK 0x3f000
#define  CDMA_CFG_DESTPID_SHIFT 12
#define  CDMA_CFG_DESTPID_MASK_SHIFT(reg) (((reg) & CDMA_CFG_DESTPID_MASK) >> CDMA_CFG_DESTPID_SHIFT)
#define  CDMA_CFG_DESTPID_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_DESTPID_MASK) | (((uint32_t)val) << CDMA_CFG_DESTPID_SHIFT))

#define  CDMA_CFG_RESERVED3_MASK 0xc00
#define  CDMA_CFG_RESERVED3_SHIFT 10
#define  CDMA_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & CDMA_CFG_RESERVED3_MASK) >> CDMA_CFG_RESERVED3_SHIFT)
#define  CDMA_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_RESERVED3_MASK) | (((uint32_t)val) << CDMA_CFG_RESERVED3_SHIFT))

#define  CDMA_CFG_SRCPID_MASK 0x3f0
#define  CDMA_CFG_SRCPID_SHIFT 4
#define  CDMA_CFG_SRCPID_MASK_SHIFT(reg) (((reg) & CDMA_CFG_SRCPID_MASK) >> CDMA_CFG_SRCPID_SHIFT)
#define  CDMA_CFG_SRCPID_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_SRCPID_MASK) | (((uint32_t)val) << CDMA_CFG_SRCPID_SHIFT))

#define  CDMA_CFG_DESTENDIAN_MASK 0x8
#define  CDMA_CFG_DESTENDIAN_SHIFT 3
#define  CDMA_CFG_DESTENDIAN_MASK_SHIFT(reg) (((reg) & CDMA_CFG_DESTENDIAN_MASK) >> CDMA_CFG_DESTENDIAN_SHIFT)
#define  CDMA_CFG_DESTENDIAN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_DESTENDIAN_MASK) | (((uint32_t)val) << CDMA_CFG_DESTENDIAN_SHIFT))

#define  CDMA_CFG_SRCENDIAN_MASK 0x4
#define  CDMA_CFG_SRCENDIAN_SHIFT 2
#define  CDMA_CFG_SRCENDIAN_MASK_SHIFT(reg) (((reg) & CDMA_CFG_SRCENDIAN_MASK) >> CDMA_CFG_SRCENDIAN_SHIFT)
#define  CDMA_CFG_SRCENDIAN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_SRCENDIAN_MASK) | (((uint32_t)val) << CDMA_CFG_SRCENDIAN_SHIFT))

#define  CDMA_CFG_FILL_MASK 0x2
#define  CDMA_CFG_FILL_SHIFT 1
#define  CDMA_CFG_FILL_MASK_SHIFT(reg) (((reg) & CDMA_CFG_FILL_MASK) >> CDMA_CFG_FILL_SHIFT)
#define  CDMA_CFG_FILL_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_FILL_MASK) | (((uint32_t)val) << CDMA_CFG_FILL_SHIFT))

#define  CDMA_CFG_ENABLE_MASK 0x1
#define  CDMA_CFG_ENABLE_SHIFT 0
#define  CDMA_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & CDMA_CFG_ENABLE_MASK) >> CDMA_CFG_ENABLE_SHIFT)
#define  CDMA_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CFG_ENABLE_MASK) | (((uint32_t)val) << CDMA_CFG_ENABLE_SHIFT))

//====================================================================
//Register: DMA Channel Control Register (Control)
/** \brief This register programs each channel for control features such as
      auto-increment, initiator select, burst size, transfer size, and
      transfer widths.*/
//====================================================================

#define  CDMA_CONTROL_TRANSSIZE_MASK 0xffff0000
#define  CDMA_CONTROL_TRANSSIZE_SHIFT 16
#define  CDMA_CONTROL_TRANSSIZE_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_TRANSSIZE_MASK) >> CDMA_CONTROL_TRANSSIZE_SHIFT)
#define  CDMA_CONTROL_TRANSSIZE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_TRANSSIZE_MASK) | (((uint32_t)val) << CDMA_CONTROL_TRANSSIZE_SHIFT))

#define  CDMA_CONTROL_RESERVED1_MASK 0xc000
#define  CDMA_CONTROL_RESERVED1_SHIFT 14
#define  CDMA_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_RESERVED1_MASK) >> CDMA_CONTROL_RESERVED1_SHIFT)
#define  CDMA_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << CDMA_CONTROL_RESERVED1_SHIFT))

#define  CDMA_CONTROL_DESTADDRINC_MASK 0x2000
#define  CDMA_CONTROL_DESTADDRINC_SHIFT 13
#define  CDMA_CONTROL_DESTADDRINC_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_DESTADDRINC_MASK) >> CDMA_CONTROL_DESTADDRINC_SHIFT)
#define  CDMA_CONTROL_DESTADDRINC_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_DESTADDRINC_MASK) | (((uint32_t)val) << CDMA_CONTROL_DESTADDRINC_SHIFT))

#define  CDMA_CONTROL_SRCADDRINC_MASK 0x1000
#define  CDMA_CONTROL_SRCADDRINC_SHIFT 12
#define  CDMA_CONTROL_SRCADDRINC_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_SRCADDRINC_MASK) >> CDMA_CONTROL_SRCADDRINC_SHIFT)
#define  CDMA_CONTROL_SRCADDRINC_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_SRCADDRINC_MASK) | (((uint32_t)val) << CDMA_CONTROL_SRCADDRINC_SHIFT))

#define  CDMA_CONTROL_DESTXFERWIDTH_MASK 0xc00
#define  CDMA_CONTROL_DESTXFERWIDTH_SHIFT 10
#define  CDMA_CONTROL_DESTXFERWIDTH_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_DESTXFERWIDTH_MASK) >> CDMA_CONTROL_DESTXFERWIDTH_SHIFT)
#define  CDMA_CONTROL_DESTXFERWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_DESTXFERWIDTH_MASK) | (((uint32_t)val) << CDMA_CONTROL_DESTXFERWIDTH_SHIFT))

#define  CDMA_CONTROL_SRCXFERWIDTH_MASK 0x300
#define  CDMA_CONTROL_SRCXFERWIDTH_SHIFT 8
#define  CDMA_CONTROL_SRCXFERWIDTH_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_SRCXFERWIDTH_MASK) >> CDMA_CONTROL_SRCXFERWIDTH_SHIFT)
#define  CDMA_CONTROL_SRCXFERWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_SRCXFERWIDTH_MASK) | (((uint32_t)val) << CDMA_CONTROL_SRCXFERWIDTH_SHIFT))

#define  CDMA_CONTROL_RESERVED2_MASK 0xc0
#define  CDMA_CONTROL_RESERVED2_SHIFT 6
#define  CDMA_CONTROL_RESERVED2_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_RESERVED2_MASK) >> CDMA_CONTROL_RESERVED2_SHIFT)
#define  CDMA_CONTROL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_RESERVED2_MASK) | (((uint32_t)val) << CDMA_CONTROL_RESERVED2_SHIFT))

#define  CDMA_CONTROL_DESTBURSTSIZE_MASK 0x30
#define  CDMA_CONTROL_DESTBURSTSIZE_SHIFT 4
#define  CDMA_CONTROL_DESTBURSTSIZE_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_DESTBURSTSIZE_MASK) >> CDMA_CONTROL_DESTBURSTSIZE_SHIFT)
#define  CDMA_CONTROL_DESTBURSTSIZE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_DESTBURSTSIZE_MASK) | (((uint32_t)val) << CDMA_CONTROL_DESTBURSTSIZE_SHIFT))

#define  CDMA_CONTROL_RESERVED3_MASK 0xc
#define  CDMA_CONTROL_RESERVED3_SHIFT 2
#define  CDMA_CONTROL_RESERVED3_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_RESERVED3_MASK) >> CDMA_CONTROL_RESERVED3_SHIFT)
#define  CDMA_CONTROL_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_RESERVED3_MASK) | (((uint32_t)val) << CDMA_CONTROL_RESERVED3_SHIFT))

#define  CDMA_CONTROL_SRCBURSTSIZE_MASK 0x3
#define  CDMA_CONTROL_SRCBURSTSIZE_SHIFT 0
#define  CDMA_CONTROL_SRCBURSTSIZE_MASK_SHIFT(reg) (((reg) & CDMA_CONTROL_SRCBURSTSIZE_MASK) >> CDMA_CONTROL_SRCBURSTSIZE_SHIFT)
#define  CDMA_CONTROL_SRCBURSTSIZE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_CONTROL_SRCBURSTSIZE_MASK) | (((uint32_t)val) << CDMA_CONTROL_SRCBURSTSIZE_SHIFT))

//====================================================================
//Register: DMA Status Register (Status)
//====================================================================

#define  CDMA_STATUS_RESERVED1_MASK 0xfff00000
#define  CDMA_STATUS_RESERVED1_SHIFT 20
#define  CDMA_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_STATUS_RESERVED1_MASK) >> CDMA_STATUS_RESERVED1_SHIFT)
#define  CDMA_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_STATUS_RESERVED1_MASK) | (((uint32_t)val) << CDMA_STATUS_RESERVED1_SHIFT))

#define  CDMA_STATUS_ODDWIDTHHW_MASK 0x80000
#define  CDMA_STATUS_ODDWIDTHHW_SHIFT 19
#define  CDMA_STATUS_ODDWIDTHHW_MASK_SHIFT(reg) (((reg) & CDMA_STATUS_ODDWIDTHHW_MASK) >> CDMA_STATUS_ODDWIDTHHW_SHIFT)
#define  CDMA_STATUS_ODDWIDTHHW_REPLACE_VAL(reg,val) (((reg) & ~CDMA_STATUS_ODDWIDTHHW_MASK) | (((uint32_t)val) << CDMA_STATUS_ODDWIDTHHW_SHIFT))

#define  CDMA_STATUS_DESCFETCH_MASK 0x40000
#define  CDMA_STATUS_DESCFETCH_SHIFT 18
#define  CDMA_STATUS_DESCFETCH_MASK_SHIFT(reg) (((reg) & CDMA_STATUS_DESCFETCH_MASK) >> CDMA_STATUS_DESCFETCH_SHIFT)
#define  CDMA_STATUS_DESCFETCH_REPLACE_VAL(reg,val) (((reg) & ~CDMA_STATUS_DESCFETCH_MASK) | (((uint32_t)val) << CDMA_STATUS_DESCFETCH_SHIFT))

#define  CDMA_STATUS_DMABUSY_MASK 0x20000
#define  CDMA_STATUS_DMABUSY_SHIFT 17
#define  CDMA_STATUS_DMABUSY_MASK_SHIFT(reg) (((reg) & CDMA_STATUS_DMABUSY_MASK) >> CDMA_STATUS_DMABUSY_SHIFT)
#define  CDMA_STATUS_DMABUSY_REPLACE_VAL(reg,val) (((reg) & ~CDMA_STATUS_DMABUSY_MASK) | (((uint32_t)val) << CDMA_STATUS_DMABUSY_SHIFT))

#define  CDMA_STATUS_FIFOEMPTY_MASK 0x10000
#define  CDMA_STATUS_FIFOEMPTY_SHIFT 16
#define  CDMA_STATUS_FIFOEMPTY_MASK_SHIFT(reg) (((reg) & CDMA_STATUS_FIFOEMPTY_MASK) >> CDMA_STATUS_FIFOEMPTY_SHIFT)
#define  CDMA_STATUS_FIFOEMPTY_REPLACE_VAL(reg,val) (((reg) & ~CDMA_STATUS_FIFOEMPTY_MASK) | (((uint32_t)val) << CDMA_STATUS_FIFOEMPTY_SHIFT))

#define  CDMA_STATUS_BYTESREMAIN_MASK 0xffff
#define  CDMA_STATUS_BYTESREMAIN_SHIFT 0
#define  CDMA_STATUS_BYTESREMAIN_MASK_SHIFT(reg) (((reg) & CDMA_STATUS_BYTESREMAIN_MASK) >> CDMA_STATUS_BYTESREMAIN_SHIFT)
#define  CDMA_STATUS_BYTESREMAIN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_STATUS_BYTESREMAIN_MASK) | (((uint32_t)val) << CDMA_STATUS_BYTESREMAIN_SHIFT))

//====================================================================
//Register: DMA Source Address Register (SrcAddr)
/** \brief The Source Address register is programmed with the address of the location
      for the DMA to transfer from. If the source is memory, the address can be
      byte aligned.
<br>*/
//====================================================================

#define  CDMA_SRCADDR_SRCADDR_MASK 0xffffffff
#define  CDMA_SRCADDR_SRCADDR_SHIFT 0
#define  CDMA_SRCADDR_SRCADDR_MASK_SHIFT(reg) (((reg) & CDMA_SRCADDR_SRCADDR_MASK) >> CDMA_SRCADDR_SRCADDR_SHIFT)
#define  CDMA_SRCADDR_SRCADDR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_SRCADDR_SRCADDR_MASK) | (((uint32_t)val) << CDMA_SRCADDR_SRCADDR_SHIFT))

//====================================================================
//Register: Current Source Address Register (SrcAddrStat)
/** \brief This register contains the current address for the source device.*/
//====================================================================

#define  CDMA_SRCADDRSTAT_CURRSRCADDR_MASK 0xffffffff
#define  CDMA_SRCADDRSTAT_CURRSRCADDR_SHIFT 0
#define  CDMA_SRCADDRSTAT_CURRSRCADDR_MASK_SHIFT(reg) (((reg) & CDMA_SRCADDRSTAT_CURRSRCADDR_MASK) >> CDMA_SRCADDRSTAT_CURRSRCADDR_SHIFT)
#define  CDMA_SRCADDRSTAT_CURRSRCADDR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_SRCADDRSTAT_CURRSRCADDR_MASK) | (((uint32_t)val) << CDMA_SRCADDRSTAT_CURRSRCADDR_SHIFT))

//====================================================================
//Register: Destination Address Register (DestAddr)
/** \brief The Destination Address register is programmed with the address of the location
      for the DMA to transfer to. If the destination is memory, the address can be
      byte aligned.*/
//====================================================================

#define  CDMA_DESTADDR_DESTADDR_MASK 0xffffffff
#define  CDMA_DESTADDR_DESTADDR_SHIFT 0
#define  CDMA_DESTADDR_DESTADDR_MASK_SHIFT(reg) (((reg) & CDMA_DESTADDR_DESTADDR_MASK) >> CDMA_DESTADDR_DESTADDR_SHIFT)
#define  CDMA_DESTADDR_DESTADDR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_DESTADDR_DESTADDR_MASK) | (((uint32_t)val) << CDMA_DESTADDR_DESTADDR_SHIFT))

//====================================================================
//Register: Current Destination Address Register (DestAddrStat)
/** \brief This register contains the current address for the destination device.*/
//====================================================================

#define  CDMA_DESTADDRSTAT_CURRDESTADDR_MASK 0xffffffff
#define  CDMA_DESTADDRSTAT_CURRDESTADDR_SHIFT 0
#define  CDMA_DESTADDRSTAT_CURRDESTADDR_MASK_SHIFT(reg) (((reg) & CDMA_DESTADDRSTAT_CURRDESTADDR_MASK) >> CDMA_DESTADDRSTAT_CURRDESTADDR_SHIFT)
#define  CDMA_DESTADDRSTAT_CURRDESTADDR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_DESTADDRSTAT_CURRDESTADDR_MASK) | (((uint32_t)val) << CDMA_DESTADDRSTAT_CURRDESTADDR_SHIFT))

//====================================================================
//Register: Fill Value Register (FillValue)
/** \brief This register contains the data value to write to the destination
      address.*/
//====================================================================

#define  CDMA_FILLVALUE_FILLVAL_MASK 0xffffffff
#define  CDMA_FILLVALUE_FILLVAL_SHIFT 0
#define  CDMA_FILLVALUE_FILLVAL_MASK_SHIFT(reg) (((reg) & CDMA_FILLVALUE_FILLVAL_MASK) >> CDMA_FILLVALUE_FILLVAL_SHIFT)
#define  CDMA_FILLVALUE_FILLVAL_REPLACE_VAL(reg,val) (((reg) & ~CDMA_FILLVALUE_FILLVAL_MASK) | (((uint32_t)val) << CDMA_FILLVALUE_FILLVAL_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (intEn)
//====================================================================

#define  CDMA_INTEN_RESERVED1_MASK 0xfffffff0
#define  CDMA_INTEN_RESERVED1_SHIFT 4
#define  CDMA_INTEN_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_INTEN_RESERVED1_MASK) >> CDMA_INTEN_RESERVED1_SHIFT)
#define  CDMA_INTEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTEN_RESERVED1_MASK) | (((uint32_t)val) << CDMA_INTEN_RESERVED1_SHIFT))

#define  CDMA_INTEN_CLEARCOMPLETEEN_MASK 0x8
#define  CDMA_INTEN_CLEARCOMPLETEEN_SHIFT 3
#define  CDMA_INTEN_CLEARCOMPLETEEN_MASK_SHIFT(reg) (((reg) & CDMA_INTEN_CLEARCOMPLETEEN_MASK) >> CDMA_INTEN_CLEARCOMPLETEEN_SHIFT)
#define  CDMA_INTEN_CLEARCOMPLETEEN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTEN_CLEARCOMPLETEEN_MASK) | (((uint32_t)val) << CDMA_INTEN_CLEARCOMPLETEEN_SHIFT))

#define  CDMA_INTEN_CHAINDONEEN_MASK 0x4
#define  CDMA_INTEN_CHAINDONEEN_SHIFT 2
#define  CDMA_INTEN_CHAINDONEEN_MASK_SHIFT(reg) (((reg) & CDMA_INTEN_CHAINDONEEN_MASK) >> CDMA_INTEN_CHAINDONEEN_SHIFT)
#define  CDMA_INTEN_CHAINDONEEN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTEN_CHAINDONEEN_MASK) | (((uint32_t)val) << CDMA_INTEN_CHAINDONEEN_SHIFT))

#define  CDMA_INTEN_TIMEOUTEN_MASK 0x2
#define  CDMA_INTEN_TIMEOUTEN_SHIFT 1
#define  CDMA_INTEN_TIMEOUTEN_MASK_SHIFT(reg) (((reg) & CDMA_INTEN_TIMEOUTEN_MASK) >> CDMA_INTEN_TIMEOUTEN_SHIFT)
#define  CDMA_INTEN_TIMEOUTEN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTEN_TIMEOUTEN_MASK) | (((uint32_t)val) << CDMA_INTEN_TIMEOUTEN_SHIFT))

#define  CDMA_INTEN_TERMCNTEN_MASK 0x1
#define  CDMA_INTEN_TERMCNTEN_SHIFT 0
#define  CDMA_INTEN_TERMCNTEN_MASK_SHIFT(reg) (((reg) & CDMA_INTEN_TERMCNTEN_MASK) >> CDMA_INTEN_TERMCNTEN_SHIFT)
#define  CDMA_INTEN_TERMCNTEN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTEN_TERMCNTEN_MASK) | (((uint32_t)val) << CDMA_INTEN_TERMCNTEN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (intPend)
//====================================================================

#define  CDMA_INTPEND_RESERVED1_MASK 0xfffffff0
#define  CDMA_INTPEND_RESERVED1_SHIFT 4
#define  CDMA_INTPEND_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_INTPEND_RESERVED1_MASK) >> CDMA_INTPEND_RESERVED1_SHIFT)
#define  CDMA_INTPEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTPEND_RESERVED1_MASK) | (((uint32_t)val) << CDMA_INTPEND_RESERVED1_SHIFT))

#define  CDMA_INTPEND_CLEARCOMPLETEPEND_MASK 0x8
#define  CDMA_INTPEND_CLEARCOMPLETEPEND_SHIFT 3
#define  CDMA_INTPEND_CLEARCOMPLETEPEND_MASK_SHIFT(reg) (((reg) & CDMA_INTPEND_CLEARCOMPLETEPEND_MASK) >> CDMA_INTPEND_CLEARCOMPLETEPEND_SHIFT)
#define  CDMA_INTPEND_CLEARCOMPLETEPEND_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTPEND_CLEARCOMPLETEPEND_MASK) | (((uint32_t)val) << CDMA_INTPEND_CLEARCOMPLETEPEND_SHIFT))

#define  CDMA_INTPEND_CHAINDONEPEND_MASK 0x4
#define  CDMA_INTPEND_CHAINDONEPEND_SHIFT 2
#define  CDMA_INTPEND_CHAINDONEPEND_MASK_SHIFT(reg) (((reg) & CDMA_INTPEND_CHAINDONEPEND_MASK) >> CDMA_INTPEND_CHAINDONEPEND_SHIFT)
#define  CDMA_INTPEND_CHAINDONEPEND_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTPEND_CHAINDONEPEND_MASK) | (((uint32_t)val) << CDMA_INTPEND_CHAINDONEPEND_SHIFT))

#define  CDMA_INTPEND_TIMEOUTPEND_MASK 0x2
#define  CDMA_INTPEND_TIMEOUTPEND_SHIFT 1
#define  CDMA_INTPEND_TIMEOUTPEND_MASK_SHIFT(reg) (((reg) & CDMA_INTPEND_TIMEOUTPEND_MASK) >> CDMA_INTPEND_TIMEOUTPEND_SHIFT)
#define  CDMA_INTPEND_TIMEOUTPEND_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTPEND_TIMEOUTPEND_MASK) | (((uint32_t)val) << CDMA_INTPEND_TIMEOUTPEND_SHIFT))

#define  CDMA_INTPEND_TERMCNTPEND_MASK 0x1
#define  CDMA_INTPEND_TERMCNTPEND_SHIFT 0
#define  CDMA_INTPEND_TERMCNTPEND_MASK_SHIFT(reg) (((reg) & CDMA_INTPEND_TERMCNTPEND_MASK) >> CDMA_INTPEND_TERMCNTPEND_SHIFT)
#define  CDMA_INTPEND_TERMCNTPEND_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTPEND_TERMCNTPEND_MASK) | (((uint32_t)val) << CDMA_INTPEND_TERMCNTPEND_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (intAck)
//====================================================================

#define  CDMA_INTACK_RESERVED1_MASK 0xfffffff0
#define  CDMA_INTACK_RESERVED1_SHIFT 4
#define  CDMA_INTACK_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_INTACK_RESERVED1_MASK) >> CDMA_INTACK_RESERVED1_SHIFT)
#define  CDMA_INTACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTACK_RESERVED1_MASK) | (((uint32_t)val) << CDMA_INTACK_RESERVED1_SHIFT))

#define  CDMA_INTACK_CLEARCOMPLETEACK_MASK 0x8
#define  CDMA_INTACK_CLEARCOMPLETEACK_SHIFT 3
#define  CDMA_INTACK_CLEARCOMPLETEACK_MASK_SHIFT(reg) (((reg) & CDMA_INTACK_CLEARCOMPLETEACK_MASK) >> CDMA_INTACK_CLEARCOMPLETEACK_SHIFT)
#define  CDMA_INTACK_CLEARCOMPLETEACK_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTACK_CLEARCOMPLETEACK_MASK) | (((uint32_t)val) << CDMA_INTACK_CLEARCOMPLETEACK_SHIFT))

#define  CDMA_INTACK_CHAINDONEACK_MASK 0x4
#define  CDMA_INTACK_CHAINDONEACK_SHIFT 2
#define  CDMA_INTACK_CHAINDONEACK_MASK_SHIFT(reg) (((reg) & CDMA_INTACK_CHAINDONEACK_MASK) >> CDMA_INTACK_CHAINDONEACK_SHIFT)
#define  CDMA_INTACK_CHAINDONEACK_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTACK_CHAINDONEACK_MASK) | (((uint32_t)val) << CDMA_INTACK_CHAINDONEACK_SHIFT))

#define  CDMA_INTACK_TIMEOUTACK_MASK 0x2
#define  CDMA_INTACK_TIMEOUTACK_SHIFT 1
#define  CDMA_INTACK_TIMEOUTACK_MASK_SHIFT(reg) (((reg) & CDMA_INTACK_TIMEOUTACK_MASK) >> CDMA_INTACK_TIMEOUTACK_SHIFT)
#define  CDMA_INTACK_TIMEOUTACK_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTACK_TIMEOUTACK_MASK) | (((uint32_t)val) << CDMA_INTACK_TIMEOUTACK_SHIFT))

#define  CDMA_INTACK_TERMCNTACK_MASK 0x1
#define  CDMA_INTACK_TERMCNTACK_SHIFT 0
#define  CDMA_INTACK_TERMCNTACK_MASK_SHIFT(reg) (((reg) & CDMA_INTACK_TERMCNTACK_MASK) >> CDMA_INTACK_TERMCNTACK_SHIFT)
#define  CDMA_INTACK_TERMCNTACK_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTACK_TERMCNTACK_MASK) | (((uint32_t)val) << CDMA_INTACK_TERMCNTACK_SHIFT))

//====================================================================
//Register: Interrupt Force Register (intForce)
//====================================================================

#define  CDMA_INTFORCE_RESERVED1_MASK 0xfffffff0
#define  CDMA_INTFORCE_RESERVED1_SHIFT 4
#define  CDMA_INTFORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_INTFORCE_RESERVED1_MASK) >> CDMA_INTFORCE_RESERVED1_SHIFT)
#define  CDMA_INTFORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTFORCE_RESERVED1_MASK) | (((uint32_t)val) << CDMA_INTFORCE_RESERVED1_SHIFT))

#define  CDMA_INTFORCE_CLEARCOMPLETEFOR_MASK 0x8
#define  CDMA_INTFORCE_CLEARCOMPLETEFOR_SHIFT 3
#define  CDMA_INTFORCE_CLEARCOMPLETEFOR_MASK_SHIFT(reg) (((reg) & CDMA_INTFORCE_CLEARCOMPLETEFOR_MASK) >> CDMA_INTFORCE_CLEARCOMPLETEFOR_SHIFT)
#define  CDMA_INTFORCE_CLEARCOMPLETEFOR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTFORCE_CLEARCOMPLETEFOR_MASK) | (((uint32_t)val) << CDMA_INTFORCE_CLEARCOMPLETEFOR_SHIFT))

#define  CDMA_INTFORCE_CHAINDONEFOR_MASK 0x4
#define  CDMA_INTFORCE_CHAINDONEFOR_SHIFT 2
#define  CDMA_INTFORCE_CHAINDONEFOR_MASK_SHIFT(reg) (((reg) & CDMA_INTFORCE_CHAINDONEFOR_MASK) >> CDMA_INTFORCE_CHAINDONEFOR_SHIFT)
#define  CDMA_INTFORCE_CHAINDONEFOR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTFORCE_CHAINDONEFOR_MASK) | (((uint32_t)val) << CDMA_INTFORCE_CHAINDONEFOR_SHIFT))

#define  CDMA_INTFORCE_TIMEOUTFOR_MASK 0x2
#define  CDMA_INTFORCE_TIMEOUTFOR_SHIFT 1
#define  CDMA_INTFORCE_TIMEOUTFOR_MASK_SHIFT(reg) (((reg) & CDMA_INTFORCE_TIMEOUTFOR_MASK) >> CDMA_INTFORCE_TIMEOUTFOR_SHIFT)
#define  CDMA_INTFORCE_TIMEOUTFOR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTFORCE_TIMEOUTFOR_MASK) | (((uint32_t)val) << CDMA_INTFORCE_TIMEOUTFOR_SHIFT))

#define  CDMA_INTFORCE_TERMCNTFOR_MASK 0x1
#define  CDMA_INTFORCE_TERMCNTFOR_SHIFT 0
#define  CDMA_INTFORCE_TERMCNTFOR_MASK_SHIFT(reg) (((reg) & CDMA_INTFORCE_TERMCNTFOR_MASK) >> CDMA_INTFORCE_TERMCNTFOR_SHIFT)
#define  CDMA_INTFORCE_TERMCNTFOR_REPLACE_VAL(reg,val) (((reg) & ~CDMA_INTFORCE_TERMCNTFOR_MASK) | (((uint32_t)val) << CDMA_INTFORCE_TERMCNTFOR_SHIFT))

//====================================================================
//Register: DMA Timer Control (TimerControl)
/** \brief The Timer Control register specifies the number of timebase cycles between word
      transfers before generating a timeout interrupt. The timer is reset each time
      a transfer occurs.*/
//====================================================================

#define  CDMA_TIMERCONTROL_RESERVED1_MASK 0xffff0000
#define  CDMA_TIMERCONTROL_RESERVED1_SHIFT 16
#define  CDMA_TIMERCONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_TIMERCONTROL_RESERVED1_MASK) >> CDMA_TIMERCONTROL_RESERVED1_SHIFT)
#define  CDMA_TIMERCONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TIMERCONTROL_RESERVED1_MASK) | (((uint32_t)val) << CDMA_TIMERCONTROL_RESERVED1_SHIFT))

#define  CDMA_TIMERCONTROL_TIMERENABLE_MASK 0x8000
#define  CDMA_TIMERCONTROL_TIMERENABLE_SHIFT 15
#define  CDMA_TIMERCONTROL_TIMERENABLE_MASK_SHIFT(reg) (((reg) & CDMA_TIMERCONTROL_TIMERENABLE_MASK) >> CDMA_TIMERCONTROL_TIMERENABLE_SHIFT)
#define  CDMA_TIMERCONTROL_TIMERENABLE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TIMERCONTROL_TIMERENABLE_MASK) | (((uint32_t)val) << CDMA_TIMERCONTROL_TIMERENABLE_SHIFT))

#define  CDMA_TIMERCONTROL_TIMEBASE_MASK 0x7000
#define  CDMA_TIMERCONTROL_TIMEBASE_SHIFT 12
#define  CDMA_TIMERCONTROL_TIMEBASE_MASK_SHIFT(reg) (((reg) & CDMA_TIMERCONTROL_TIMEBASE_MASK) >> CDMA_TIMERCONTROL_TIMEBASE_SHIFT)
#define  CDMA_TIMERCONTROL_TIMEBASE_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TIMERCONTROL_TIMEBASE_MASK) | (((uint32_t)val) << CDMA_TIMERCONTROL_TIMEBASE_SHIFT))

#define  CDMA_TIMERCONTROL_RESERVED2_MASK 0x800
#define  CDMA_TIMERCONTROL_RESERVED2_SHIFT 11
#define  CDMA_TIMERCONTROL_RESERVED2_MASK_SHIFT(reg) (((reg) & CDMA_TIMERCONTROL_RESERVED2_MASK) >> CDMA_TIMERCONTROL_RESERVED2_SHIFT)
#define  CDMA_TIMERCONTROL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TIMERCONTROL_RESERVED2_MASK) | (((uint32_t)val) << CDMA_TIMERCONTROL_RESERVED2_SHIFT))

#define  CDMA_TIMERCONTROL_COUNT_MASK 0x7ff
#define  CDMA_TIMERCONTROL_COUNT_SHIFT 0
#define  CDMA_TIMERCONTROL_COUNT_MASK_SHIFT(reg) (((reg) & CDMA_TIMERCONTROL_COUNT_MASK) >> CDMA_TIMERCONTROL_COUNT_SHIFT)
#define  CDMA_TIMERCONTROL_COUNT_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TIMERCONTROL_COUNT_MASK) | (((uint32_t)val) << CDMA_TIMERCONTROL_COUNT_SHIFT))

//====================================================================
//Register: Timeout Count Status Register (TimeOutStat)
/** \brief This register contains the current timeout count.*/
//====================================================================

#define  CDMA_TIMEOUTSTAT_RESERVED1_MASK 0xfffff800
#define  CDMA_TIMEOUTSTAT_RESERVED1_SHIFT 11
#define  CDMA_TIMEOUTSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_TIMEOUTSTAT_RESERVED1_MASK) >> CDMA_TIMEOUTSTAT_RESERVED1_SHIFT)
#define  CDMA_TIMEOUTSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TIMEOUTSTAT_RESERVED1_MASK) | (((uint32_t)val) << CDMA_TIMEOUTSTAT_RESERVED1_SHIFT))

#define  CDMA_TIMEOUTSTAT_CURRTOCNT_MASK 0x7ff
#define  CDMA_TIMEOUTSTAT_CURRTOCNT_SHIFT 0
#define  CDMA_TIMEOUTSTAT_CURRTOCNT_MASK_SHIFT(reg) (((reg) & CDMA_TIMEOUTSTAT_CURRTOCNT_MASK) >> CDMA_TIMEOUTSTAT_CURRTOCNT_SHIFT)
#define  CDMA_TIMEOUTSTAT_CURRTOCNT_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TIMEOUTSTAT_CURRTOCNT_MASK) | (((uint32_t)val) << CDMA_TIMEOUTSTAT_CURRTOCNT_SHIFT))

//====================================================================
//Register: Linked List Index Register (LLIR)
/** \brief This register contains a word-aligned address of the first word of the next descriptor. LLI means "linked list index" or pointer to the descriptor in memory. If the LLI register is 0, then the current descriptor (that is loaded in the registers) is the last in the chain, and the DMA channel will stop once the current DMA transfer is completed.*/
//====================================================================

#define  CDMA_LLIR_LLINDEX_MASK 0xfffffffc
#define  CDMA_LLIR_LLINDEX_SHIFT 2
#define  CDMA_LLIR_LLINDEX_MASK_SHIFT(reg) (((reg) & CDMA_LLIR_LLINDEX_MASK) >> CDMA_LLIR_LLINDEX_SHIFT)
#define  CDMA_LLIR_LLINDEX_REPLACE_VAL(reg,val) (((reg) & ~CDMA_LLIR_LLINDEX_MASK) | (((uint32_t)val) << CDMA_LLIR_LLINDEX_SHIFT))

#define  CDMA_LLIR_RESERVED1_MASK 0x3
#define  CDMA_LLIR_RESERVED1_SHIFT 0
#define  CDMA_LLIR_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_LLIR_RESERVED1_MASK) >> CDMA_LLIR_RESERVED1_SHIFT)
#define  CDMA_LLIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_LLIR_RESERVED1_MASK) | (((uint32_t)val) << CDMA_LLIR_RESERVED1_SHIFT))

//====================================================================
//Register: Odd Width Config Register (OWCFG)
//====================================================================

#define  CDMA_OWCFG_RESERVED1_MASK 0xfffe0000
#define  CDMA_OWCFG_RESERVED1_SHIFT 17
#define  CDMA_OWCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_OWCFG_RESERVED1_MASK) >> CDMA_OWCFG_RESERVED1_SHIFT)
#define  CDMA_OWCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_OWCFG_RESERVED1_MASK) | (((uint32_t)val) << CDMA_OWCFG_RESERVED1_SHIFT))

#define  CDMA_OWCFG_EXTRABITS_MASK 0x1f000
#define  CDMA_OWCFG_EXTRABITS_SHIFT 12
#define  CDMA_OWCFG_EXTRABITS_MASK_SHIFT(reg) (((reg) & CDMA_OWCFG_EXTRABITS_MASK) >> CDMA_OWCFG_EXTRABITS_SHIFT)
#define  CDMA_OWCFG_EXTRABITS_REPLACE_VAL(reg,val) (((reg) & ~CDMA_OWCFG_EXTRABITS_MASK) | (((uint32_t)val) << CDMA_OWCFG_EXTRABITS_SHIFT))

#define  CDMA_OWCFG_RESERVED2_MASK 0xe00
#define  CDMA_OWCFG_RESERVED2_SHIFT 9
#define  CDMA_OWCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & CDMA_OWCFG_RESERVED2_MASK) >> CDMA_OWCFG_RESERVED2_SHIFT)
#define  CDMA_OWCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CDMA_OWCFG_RESERVED2_MASK) | (((uint32_t)val) << CDMA_OWCFG_RESERVED2_SHIFT))

#define  CDMA_OWCFG_ODDWIDTH_MASK 0x1f0
#define  CDMA_OWCFG_ODDWIDTH_SHIFT 4
#define  CDMA_OWCFG_ODDWIDTH_MASK_SHIFT(reg) (((reg) & CDMA_OWCFG_ODDWIDTH_MASK) >> CDMA_OWCFG_ODDWIDTH_SHIFT)
#define  CDMA_OWCFG_ODDWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CDMA_OWCFG_ODDWIDTH_MASK) | (((uint32_t)val) << CDMA_OWCFG_ODDWIDTH_SHIFT))

#define  CDMA_OWCFG_RESERVED3_MASK 0xc
#define  CDMA_OWCFG_RESERVED3_SHIFT 2
#define  CDMA_OWCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & CDMA_OWCFG_RESERVED3_MASK) >> CDMA_OWCFG_RESERVED3_SHIFT)
#define  CDMA_OWCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CDMA_OWCFG_RESERVED3_MASK) | (((uint32_t)val) << CDMA_OWCFG_RESERVED3_SHIFT))

#define  CDMA_OWCFG_PACKNUNPACK_MASK 0x2
#define  CDMA_OWCFG_PACKNUNPACK_SHIFT 1
#define  CDMA_OWCFG_PACKNUNPACK_MASK_SHIFT(reg) (((reg) & CDMA_OWCFG_PACKNUNPACK_MASK) >> CDMA_OWCFG_PACKNUNPACK_SHIFT)
#define  CDMA_OWCFG_PACKNUNPACK_REPLACE_VAL(reg,val) (((reg) & ~CDMA_OWCFG_PACKNUNPACK_MASK) | (((uint32_t)val) << CDMA_OWCFG_PACKNUNPACK_SHIFT))

#define  CDMA_OWCFG_OWEN_MASK 0x1
#define  CDMA_OWCFG_OWEN_SHIFT 0
#define  CDMA_OWCFG_OWEN_MASK_SHIFT(reg) (((reg) & CDMA_OWCFG_OWEN_MASK) >> CDMA_OWCFG_OWEN_SHIFT)
#define  CDMA_OWCFG_OWEN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_OWCFG_OWEN_MASK) | (((uint32_t)val) << CDMA_OWCFG_OWEN_SHIFT))

#endif // CDMA
