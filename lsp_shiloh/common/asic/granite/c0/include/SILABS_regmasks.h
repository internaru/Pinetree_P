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
 * \file SILABS_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SILABS_REGMASKS_H_
#define _SILABS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SILABS (SILABS)
/** \brief Top-level register file for APB SILABS block*/
//
//====================================================================

//====================================================================
//Register: Control Register (CTL)
/** \brief Using this register, firmware can enable interrupts, RX/TX functions, DMA requests and specify single vs. dual channel configuration. <BR>
            The RxFifos and TxFifos are not cleared even when the E bit is set to zero. This allows firmware to fill the transmit FIFO before enabling the Softmodem SPI block, and also allows firmware to drain the receive FIFO before enabling the block.<BR>
            When changing the Channel Configuration bit, firmware must disable the interface by writing a 0 to the Enable bit before writing the new value to the Configuration bit. By disabling the Enable bit, the interface hardware will re-initialize state machines to properly re-synchronize with nFSYNC when changing between single and dual-channel operation. Also, firmware should write a 0 to the EnableRxFifo[2] and EnableTxFifo[2] bits to clear the RxFIFOs and TxFIFOs.<BR>*/
//====================================================================

#define  SILABS_CTL_RESERVED1_MASK 0xffffc000
#define  SILABS_CTL_RESERVED1_SHIFT 14
#define  SILABS_CTL_RESERVED1_MASK_SHIFT(reg) (((reg) & SILABS_CTL_RESERVED1_MASK) >> SILABS_CTL_RESERVED1_SHIFT)
#define  SILABS_CTL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_RESERVED1_MASK) | (((uint32_t)val) << SILABS_CTL_RESERVED1_SHIFT))

#define  SILABS_CTL_ENABLETXFIFO2_MASK 0x2000
#define  SILABS_CTL_ENABLETXFIFO2_SHIFT 13
#define  SILABS_CTL_ENABLETXFIFO2_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLETXFIFO2_MASK) >> SILABS_CTL_ENABLETXFIFO2_SHIFT)
#define  SILABS_CTL_ENABLETXFIFO2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLETXFIFO2_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLETXFIFO2_SHIFT))

#define  SILABS_CTL_ENABLERXFIFO2_MASK 0x1000
#define  SILABS_CTL_ENABLERXFIFO2_SHIFT 12
#define  SILABS_CTL_ENABLERXFIFO2_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLERXFIFO2_MASK) >> SILABS_CTL_ENABLERXFIFO2_SHIFT)
#define  SILABS_CTL_ENABLERXFIFO2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLERXFIFO2_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLERXFIFO2_SHIFT))

#define  SILABS_CTL_ENABLETXUNDERRUNIRQ2_MASK 0x800
#define  SILABS_CTL_ENABLETXUNDERRUNIRQ2_SHIFT 11
#define  SILABS_CTL_ENABLETXUNDERRUNIRQ2_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLETXUNDERRUNIRQ2_MASK) >> SILABS_CTL_ENABLETXUNDERRUNIRQ2_SHIFT)
#define  SILABS_CTL_ENABLETXUNDERRUNIRQ2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLETXUNDERRUNIRQ2_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLETXUNDERRUNIRQ2_SHIFT))

#define  SILABS_CTL_ENABLERXOVERRUNIRQ2_MASK 0x400
#define  SILABS_CTL_ENABLERXOVERRUNIRQ2_SHIFT 10
#define  SILABS_CTL_ENABLERXOVERRUNIRQ2_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLERXOVERRUNIRQ2_MASK) >> SILABS_CTL_ENABLERXOVERRUNIRQ2_SHIFT)
#define  SILABS_CTL_ENABLERXOVERRUNIRQ2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLERXOVERRUNIRQ2_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLERXOVERRUNIRQ2_SHIFT))

#define  SILABS_CTL_ENABLETXIRQ2_MASK 0x200
#define  SILABS_CTL_ENABLETXIRQ2_SHIFT 9
#define  SILABS_CTL_ENABLETXIRQ2_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLETXIRQ2_MASK) >> SILABS_CTL_ENABLETXIRQ2_SHIFT)
#define  SILABS_CTL_ENABLETXIRQ2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLETXIRQ2_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLETXIRQ2_SHIFT))

#define  SILABS_CTL_ENABLERXIRQ2_MASK 0x100
#define  SILABS_CTL_ENABLERXIRQ2_SHIFT 8
#define  SILABS_CTL_ENABLERXIRQ2_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLERXIRQ2_MASK) >> SILABS_CTL_ENABLERXIRQ2_SHIFT)
#define  SILABS_CTL_ENABLERXIRQ2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLERXIRQ2_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLERXIRQ2_SHIFT))

#define  SILABS_CTL_ENABLE_MASK 0x80
#define  SILABS_CTL_ENABLE_SHIFT 7
#define  SILABS_CTL_ENABLE_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLE_MASK) >> SILABS_CTL_ENABLE_SHIFT)
#define  SILABS_CTL_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLE_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLE_SHIFT))

#define  SILABS_CTL_CHANNELCONFIG_MASK 0x40
#define  SILABS_CTL_CHANNELCONFIG_SHIFT 6
#define  SILABS_CTL_CHANNELCONFIG_MASK_SHIFT(reg) (((reg) & SILABS_CTL_CHANNELCONFIG_MASK) >> SILABS_CTL_CHANNELCONFIG_SHIFT)
#define  SILABS_CTL_CHANNELCONFIG_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_CHANNELCONFIG_MASK) | (((uint32_t)val) << SILABS_CTL_CHANNELCONFIG_SHIFT))

#define  SILABS_CTL_ENABLETXFIFO_MASK 0x20
#define  SILABS_CTL_ENABLETXFIFO_SHIFT 5
#define  SILABS_CTL_ENABLETXFIFO_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLETXFIFO_MASK) >> SILABS_CTL_ENABLETXFIFO_SHIFT)
#define  SILABS_CTL_ENABLETXFIFO_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLETXFIFO_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLETXFIFO_SHIFT))

#define  SILABS_CTL_ENABLERXFIFO_MASK 0x10
#define  SILABS_CTL_ENABLERXFIFO_SHIFT 4
#define  SILABS_CTL_ENABLERXFIFO_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLERXFIFO_MASK) >> SILABS_CTL_ENABLERXFIFO_SHIFT)
#define  SILABS_CTL_ENABLERXFIFO_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLERXFIFO_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLERXFIFO_SHIFT))

#define  SILABS_CTL_ENABLETXUNDERRUNIRQ_MASK 0x8
#define  SILABS_CTL_ENABLETXUNDERRUNIRQ_SHIFT 3
#define  SILABS_CTL_ENABLETXUNDERRUNIRQ_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLETXUNDERRUNIRQ_MASK) >> SILABS_CTL_ENABLETXUNDERRUNIRQ_SHIFT)
#define  SILABS_CTL_ENABLETXUNDERRUNIRQ_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLETXUNDERRUNIRQ_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLETXUNDERRUNIRQ_SHIFT))

#define  SILABS_CTL_ENABLERXOVERRUNIRQ_MASK 0x4
#define  SILABS_CTL_ENABLERXOVERRUNIRQ_SHIFT 2
#define  SILABS_CTL_ENABLERXOVERRUNIRQ_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLERXOVERRUNIRQ_MASK) >> SILABS_CTL_ENABLERXOVERRUNIRQ_SHIFT)
#define  SILABS_CTL_ENABLERXOVERRUNIRQ_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLERXOVERRUNIRQ_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLERXOVERRUNIRQ_SHIFT))

#define  SILABS_CTL_ENABLETXIRQ_MASK 0x2
#define  SILABS_CTL_ENABLETXIRQ_SHIFT 1
#define  SILABS_CTL_ENABLETXIRQ_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLETXIRQ_MASK) >> SILABS_CTL_ENABLETXIRQ_SHIFT)
#define  SILABS_CTL_ENABLETXIRQ_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLETXIRQ_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLETXIRQ_SHIFT))

#define  SILABS_CTL_ENABLERXIRQ_MASK 0x1
#define  SILABS_CTL_ENABLERXIRQ_SHIFT 0
#define  SILABS_CTL_ENABLERXIRQ_MASK_SHIFT(reg) (((reg) & SILABS_CTL_ENABLERXIRQ_MASK) >> SILABS_CTL_ENABLERXIRQ_SHIFT)
#define  SILABS_CTL_ENABLERXIRQ_REPLACE_VAL(reg,val) (((reg) & ~SILABS_CTL_ENABLERXIRQ_MASK) | (((uint32_t)val) << SILABS_CTL_ENABLERXIRQ_SHIFT))

//====================================================================
//Register: Secondary Communications Register for DAA Channel (SECTX)
/** \brief Writing to this register initiates communications to the DAA via the secondary frame. The Enable bit of the Control Register must also be set. After this register is written, the specified 16-bit bit value is transferred over the secondary frame. The completion of the command can be determined by observing the SecondaryRQ bit in the Status Register. The secondary response data (if any) can also be observed in the Status Register. <BR>
             In single DAA channel mode (Control Register ChannelConfig bit is 0), this register should only be written when the SecondaryRQ bit in the Status Register is zero (not busy). <BR>
             In dual DAA/Codec channel mode, this register should only be written when the Status.SecondaryRQ register bit is zero AND the Status2.SecondaryRQ register bit zero (both are not busy).*/
//====================================================================

#define  SILABS_SECTX_RESERVED1_MASK 0xffff0000
#define  SILABS_SECTX_RESERVED1_SHIFT 16
#define  SILABS_SECTX_RESERVED1_MASK_SHIFT(reg) (((reg) & SILABS_SECTX_RESERVED1_MASK) >> SILABS_SECTX_RESERVED1_SHIFT)
#define  SILABS_SECTX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SILABS_SECTX_RESERVED1_MASK) | (((uint32_t)val) << SILABS_SECTX_RESERVED1_SHIFT))

#define  SILABS_SECTX_SECTXDATA_MASK 0xffff
#define  SILABS_SECTX_SECTXDATA_SHIFT 0
#define  SILABS_SECTX_SECTXDATA_MASK_SHIFT(reg) (((reg) & SILABS_SECTX_SECTXDATA_MASK) >> SILABS_SECTX_SECTXDATA_SHIFT)
#define  SILABS_SECTX_SECTXDATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_SECTX_SECTXDATA_MASK) | (((uint32_t)val) << SILABS_SECTX_SECTXDATA_SHIFT))

//====================================================================
//Register: Status Register for DAA Channel (STS)
/** \brief This register provides the Rx and Tx FIFO status, the interrupt status, and the secondary communications channel status and response data of the DAA channel. The output IRQ is asserted when at least one of the TxUnderrun flag or RxOverrun flag or TxFifoAlmostEmpty or RxFifoAlmostFull conditions is true (one), as masked by their respective bit in the Control register bits[5:0].*/
//====================================================================

#define  SILABS_STS_RXFIFOALMOSTFULL_MASK 0x80000000
#define  SILABS_STS_RXFIFOALMOSTFULL_SHIFT 31
#define  SILABS_STS_RXFIFOALMOSTFULL_MASK_SHIFT(reg) (((reg) & SILABS_STS_RXFIFOALMOSTFULL_MASK) >> SILABS_STS_RXFIFOALMOSTFULL_SHIFT)
#define  SILABS_STS_RXFIFOALMOSTFULL_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_RXFIFOALMOSTFULL_MASK) | (((uint32_t)val) << SILABS_STS_RXFIFOALMOSTFULL_SHIFT))

#define  SILABS_STS_RXFIFOFULL_MASK 0x40000000
#define  SILABS_STS_RXFIFOFULL_SHIFT 30
#define  SILABS_STS_RXFIFOFULL_MASK_SHIFT(reg) (((reg) & SILABS_STS_RXFIFOFULL_MASK) >> SILABS_STS_RXFIFOFULL_SHIFT)
#define  SILABS_STS_RXFIFOFULL_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_RXFIFOFULL_MASK) | (((uint32_t)val) << SILABS_STS_RXFIFOFULL_SHIFT))

#define  SILABS_STS_RXFIFOEMPTY_MASK 0x20000000
#define  SILABS_STS_RXFIFOEMPTY_SHIFT 29
#define  SILABS_STS_RXFIFOEMPTY_MASK_SHIFT(reg) (((reg) & SILABS_STS_RXFIFOEMPTY_MASK) >> SILABS_STS_RXFIFOEMPTY_SHIFT)
#define  SILABS_STS_RXFIFOEMPTY_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_RXFIFOEMPTY_MASK) | (((uint32_t)val) << SILABS_STS_RXFIFOEMPTY_SHIFT))

#define  SILABS_STS_RESERVED1_MASK 0x10000000
#define  SILABS_STS_RESERVED1_SHIFT 28
#define  SILABS_STS_RESERVED1_MASK_SHIFT(reg) (((reg) & SILABS_STS_RESERVED1_MASK) >> SILABS_STS_RESERVED1_SHIFT)
#define  SILABS_STS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_RESERVED1_MASK) | (((uint32_t)val) << SILABS_STS_RESERVED1_SHIFT))

#define  SILABS_STS_TXFIFOALMOSTEMPTY_MASK 0x8000000
#define  SILABS_STS_TXFIFOALMOSTEMPTY_SHIFT 27
#define  SILABS_STS_TXFIFOALMOSTEMPTY_MASK_SHIFT(reg) (((reg) & SILABS_STS_TXFIFOALMOSTEMPTY_MASK) >> SILABS_STS_TXFIFOALMOSTEMPTY_SHIFT)
#define  SILABS_STS_TXFIFOALMOSTEMPTY_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_TXFIFOALMOSTEMPTY_MASK) | (((uint32_t)val) << SILABS_STS_TXFIFOALMOSTEMPTY_SHIFT))

#define  SILABS_STS_TXFIFOFULL_MASK 0x4000000
#define  SILABS_STS_TXFIFOFULL_SHIFT 26
#define  SILABS_STS_TXFIFOFULL_MASK_SHIFT(reg) (((reg) & SILABS_STS_TXFIFOFULL_MASK) >> SILABS_STS_TXFIFOFULL_SHIFT)
#define  SILABS_STS_TXFIFOFULL_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_TXFIFOFULL_MASK) | (((uint32_t)val) << SILABS_STS_TXFIFOFULL_SHIFT))

#define  SILABS_STS_TXFIFOEMPTY_MASK 0x2000000
#define  SILABS_STS_TXFIFOEMPTY_SHIFT 25
#define  SILABS_STS_TXFIFOEMPTY_MASK_SHIFT(reg) (((reg) & SILABS_STS_TXFIFOEMPTY_MASK) >> SILABS_STS_TXFIFOEMPTY_SHIFT)
#define  SILABS_STS_TXFIFOEMPTY_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_TXFIFOEMPTY_MASK) | (((uint32_t)val) << SILABS_STS_TXFIFOEMPTY_SHIFT))

#define  SILABS_STS_RESERVED2_MASK 0x1c00000
#define  SILABS_STS_RESERVED2_SHIFT 22
#define  SILABS_STS_RESERVED2_MASK_SHIFT(reg) (((reg) & SILABS_STS_RESERVED2_MASK) >> SILABS_STS_RESERVED2_SHIFT)
#define  SILABS_STS_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_RESERVED2_MASK) | (((uint32_t)val) << SILABS_STS_RESERVED2_SHIFT))

#define  SILABS_STS_RXOVERRUN_MASK 0x200000
#define  SILABS_STS_RXOVERRUN_SHIFT 21
#define  SILABS_STS_RXOVERRUN_MASK_SHIFT(reg) (((reg) & SILABS_STS_RXOVERRUN_MASK) >> SILABS_STS_RXOVERRUN_SHIFT)
#define  SILABS_STS_RXOVERRUN_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_RXOVERRUN_MASK) | (((uint32_t)val) << SILABS_STS_RXOVERRUN_SHIFT))

#define  SILABS_STS_TXUNDERRUN_MASK 0x100000
#define  SILABS_STS_TXUNDERRUN_SHIFT 20
#define  SILABS_STS_TXUNDERRUN_MASK_SHIFT(reg) (((reg) & SILABS_STS_TXUNDERRUN_MASK) >> SILABS_STS_TXUNDERRUN_SHIFT)
#define  SILABS_STS_TXUNDERRUN_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_TXUNDERRUN_MASK) | (((uint32_t)val) << SILABS_STS_TXUNDERRUN_SHIFT))

#define  SILABS_STS_RESERVED3_MASK 0xe0000
#define  SILABS_STS_RESERVED3_SHIFT 17
#define  SILABS_STS_RESERVED3_MASK_SHIFT(reg) (((reg) & SILABS_STS_RESERVED3_MASK) >> SILABS_STS_RESERVED3_SHIFT)
#define  SILABS_STS_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_RESERVED3_MASK) | (((uint32_t)val) << SILABS_STS_RESERVED3_SHIFT))

#define  SILABS_STS_SECONDARYRQ_MASK 0x10000
#define  SILABS_STS_SECONDARYRQ_SHIFT 16
#define  SILABS_STS_SECONDARYRQ_MASK_SHIFT(reg) (((reg) & SILABS_STS_SECONDARYRQ_MASK) >> SILABS_STS_SECONDARYRQ_SHIFT)
#define  SILABS_STS_SECONDARYRQ_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_SECONDARYRQ_MASK) | (((uint32_t)val) << SILABS_STS_SECONDARYRQ_SHIFT))

#define  SILABS_STS_SECONDARYRXDATA_MASK 0xffff
#define  SILABS_STS_SECONDARYRXDATA_SHIFT 0
#define  SILABS_STS_SECONDARYRXDATA_MASK_SHIFT(reg) (((reg) & SILABS_STS_SECONDARYRXDATA_MASK) >> SILABS_STS_SECONDARYRXDATA_SHIFT)
#define  SILABS_STS_SECONDARYRXDATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS_SECONDARYRXDATA_MASK) | (((uint32_t)val) << SILABS_STS_SECONDARYRXDATA_SHIFT))

//====================================================================
//Register: Receive Register for DAA Channel (RXD)
/** \brief This register should only be read when the RxEmpty bit is not set in the Status Register (or by DMA when DMARX*REQ is asserted). The RxFifo will pack 16bit words from the incoming serial line into a 32bit word. The first 16bit word received is placed in D[31:16], and the second 16bit word received is placed in D[15:0].<BR>
             When using DMA, this register address is programmed as the source. The DMARXSREQ output is asserted when there are one or more RxFifo words available to read. The DMARXBREQ output is asserted when there are four or more RxFifo words available. The RxFifo is flushed when the Control.EnableRxFifo bit is cleared (=0).*/
//====================================================================

#define  SILABS_RXD_DATA_MASK 0xffffffff
#define  SILABS_RXD_DATA_SHIFT 0
#define  SILABS_RXD_DATA_MASK_SHIFT(reg) (((reg) & SILABS_RXD_DATA_MASK) >> SILABS_RXD_DATA_SHIFT)
#define  SILABS_RXD_DATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_RXD_DATA_MASK) | (((uint32_t)val) << SILABS_RXD_DATA_SHIFT))

//====================================================================
//Register: Transmit Register for DAA Channel (TXD)
/** \brief This register should only be written by firmware when the TxFifoFull bit is not set in the Status Register (or by DMA when DMATX*REQ is asserted). When using DMA, this register address is programmed as the destination. The TxFifo is flushed when the Control.EnableTxFifo bit is cleared (=0). <BR>
             The TxFifo will unpack a 32bit word into 16b words for the outgoing serial line. The halfword in D[31:16] is transmitted first, followed by the halfword in D[15:0]. The LSB of the transmitted 16b word is the software FC bit (recognized by the Si3000 or Si3056 devices) to indicated if a secondary frame is requested. The SiLabs block will set the LSB to 1 if there Status.SecondaryRQ bit is set.*/
//====================================================================

#define  SILABS_TXD_DATA_MASK 0xffffffff
#define  SILABS_TXD_DATA_SHIFT 0
#define  SILABS_TXD_DATA_MASK_SHIFT(reg) (((reg) & SILABS_TXD_DATA_MASK) >> SILABS_TXD_DATA_SHIFT)
#define  SILABS_TXD_DATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_TXD_DATA_MASK) | (((uint32_t)val) << SILABS_TXD_DATA_SHIFT))

//====================================================================
//Register: Secondary Communications Register for CODEC Channel (SECTX2)
/** \brief Writing to this register initiates communications to the CODEC via the secondary frame. The Enable bit of the Control Register must also be set. After this register is written, the specified 16-bit bit value is transferred over the secondary frame. The completion of the command can be determined by observing the SecondaryRQ bit in the Status2 Register. The secondary response data (if any) can also be observed in the Status2 Register. <BR>
             In single DAA channel mode (Control Register ChannelConfig bit is 0), this register should only be written when the SecondaryRQ bit in the Status Register is zero (not busy). <BR>
             In dual DAA/Codec channel mode, this register should only be written when the Status.SecondaryRQ register bit is zero AND the Status2.SecondaryRQ register bit zero (both are not busy).*/
//====================================================================

#define  SILABS_SECTX2_RESERVED1_MASK 0xffff0000
#define  SILABS_SECTX2_RESERVED1_SHIFT 16
#define  SILABS_SECTX2_RESERVED1_MASK_SHIFT(reg) (((reg) & SILABS_SECTX2_RESERVED1_MASK) >> SILABS_SECTX2_RESERVED1_SHIFT)
#define  SILABS_SECTX2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SILABS_SECTX2_RESERVED1_MASK) | (((uint32_t)val) << SILABS_SECTX2_RESERVED1_SHIFT))

#define  SILABS_SECTX2_SECTXDATA_MASK 0xffff
#define  SILABS_SECTX2_SECTXDATA_SHIFT 0
#define  SILABS_SECTX2_SECTXDATA_MASK_SHIFT(reg) (((reg) & SILABS_SECTX2_SECTXDATA_MASK) >> SILABS_SECTX2_SECTXDATA_SHIFT)
#define  SILABS_SECTX2_SECTXDATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_SECTX2_SECTXDATA_MASK) | (((uint32_t)val) << SILABS_SECTX2_SECTXDATA_SHIFT))

//====================================================================
//Register: Status Register for CODEC Channel (STS2)
/** \brief This register provides the Rx and Tx FIFO status, the interrupt status, and the secondary communications channel status and response data of the CODEC channel. The output IRQ is asserted when at least one of the TxUnderrun flag or RxOverrun flag or TxFifoAlmostEmpty or RxFifoAlmostFull conditions is true (one), as masked by their respective bit in the Control register bits[13:8].*/
//====================================================================

#define  SILABS_STS2_RXFIFOALMOSTFULL_MASK 0x80000000
#define  SILABS_STS2_RXFIFOALMOSTFULL_SHIFT 31
#define  SILABS_STS2_RXFIFOALMOSTFULL_MASK_SHIFT(reg) (((reg) & SILABS_STS2_RXFIFOALMOSTFULL_MASK) >> SILABS_STS2_RXFIFOALMOSTFULL_SHIFT)
#define  SILABS_STS2_RXFIFOALMOSTFULL_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_RXFIFOALMOSTFULL_MASK) | (((uint32_t)val) << SILABS_STS2_RXFIFOALMOSTFULL_SHIFT))

#define  SILABS_STS2_RXFIFOFULL_MASK 0x40000000
#define  SILABS_STS2_RXFIFOFULL_SHIFT 30
#define  SILABS_STS2_RXFIFOFULL_MASK_SHIFT(reg) (((reg) & SILABS_STS2_RXFIFOFULL_MASK) >> SILABS_STS2_RXFIFOFULL_SHIFT)
#define  SILABS_STS2_RXFIFOFULL_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_RXFIFOFULL_MASK) | (((uint32_t)val) << SILABS_STS2_RXFIFOFULL_SHIFT))

#define  SILABS_STS2_RXFIFOEMPTY_MASK 0x20000000
#define  SILABS_STS2_RXFIFOEMPTY_SHIFT 29
#define  SILABS_STS2_RXFIFOEMPTY_MASK_SHIFT(reg) (((reg) & SILABS_STS2_RXFIFOEMPTY_MASK) >> SILABS_STS2_RXFIFOEMPTY_SHIFT)
#define  SILABS_STS2_RXFIFOEMPTY_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_RXFIFOEMPTY_MASK) | (((uint32_t)val) << SILABS_STS2_RXFIFOEMPTY_SHIFT))

#define  SILABS_STS2_RESERVED1_MASK 0x10000000
#define  SILABS_STS2_RESERVED1_SHIFT 28
#define  SILABS_STS2_RESERVED1_MASK_SHIFT(reg) (((reg) & SILABS_STS2_RESERVED1_MASK) >> SILABS_STS2_RESERVED1_SHIFT)
#define  SILABS_STS2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_RESERVED1_MASK) | (((uint32_t)val) << SILABS_STS2_RESERVED1_SHIFT))

#define  SILABS_STS2_TXFIFOALMOSTEMPTY_MASK 0x8000000
#define  SILABS_STS2_TXFIFOALMOSTEMPTY_SHIFT 27
#define  SILABS_STS2_TXFIFOALMOSTEMPTY_MASK_SHIFT(reg) (((reg) & SILABS_STS2_TXFIFOALMOSTEMPTY_MASK) >> SILABS_STS2_TXFIFOALMOSTEMPTY_SHIFT)
#define  SILABS_STS2_TXFIFOALMOSTEMPTY_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_TXFIFOALMOSTEMPTY_MASK) | (((uint32_t)val) << SILABS_STS2_TXFIFOALMOSTEMPTY_SHIFT))

#define  SILABS_STS2_TXFIFOFULL_MASK 0x4000000
#define  SILABS_STS2_TXFIFOFULL_SHIFT 26
#define  SILABS_STS2_TXFIFOFULL_MASK_SHIFT(reg) (((reg) & SILABS_STS2_TXFIFOFULL_MASK) >> SILABS_STS2_TXFIFOFULL_SHIFT)
#define  SILABS_STS2_TXFIFOFULL_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_TXFIFOFULL_MASK) | (((uint32_t)val) << SILABS_STS2_TXFIFOFULL_SHIFT))

#define  SILABS_STS2_TXFIFOEMPTY_MASK 0x2000000
#define  SILABS_STS2_TXFIFOEMPTY_SHIFT 25
#define  SILABS_STS2_TXFIFOEMPTY_MASK_SHIFT(reg) (((reg) & SILABS_STS2_TXFIFOEMPTY_MASK) >> SILABS_STS2_TXFIFOEMPTY_SHIFT)
#define  SILABS_STS2_TXFIFOEMPTY_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_TXFIFOEMPTY_MASK) | (((uint32_t)val) << SILABS_STS2_TXFIFOEMPTY_SHIFT))

#define  SILABS_STS2_RESERVED2_MASK 0x1c00000
#define  SILABS_STS2_RESERVED2_SHIFT 22
#define  SILABS_STS2_RESERVED2_MASK_SHIFT(reg) (((reg) & SILABS_STS2_RESERVED2_MASK) >> SILABS_STS2_RESERVED2_SHIFT)
#define  SILABS_STS2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_RESERVED2_MASK) | (((uint32_t)val) << SILABS_STS2_RESERVED2_SHIFT))

#define  SILABS_STS2_RXOVERRUN_MASK 0x200000
#define  SILABS_STS2_RXOVERRUN_SHIFT 21
#define  SILABS_STS2_RXOVERRUN_MASK_SHIFT(reg) (((reg) & SILABS_STS2_RXOVERRUN_MASK) >> SILABS_STS2_RXOVERRUN_SHIFT)
#define  SILABS_STS2_RXOVERRUN_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_RXOVERRUN_MASK) | (((uint32_t)val) << SILABS_STS2_RXOVERRUN_SHIFT))

#define  SILABS_STS2_TXUNDERRUN_MASK 0x100000
#define  SILABS_STS2_TXUNDERRUN_SHIFT 20
#define  SILABS_STS2_TXUNDERRUN_MASK_SHIFT(reg) (((reg) & SILABS_STS2_TXUNDERRUN_MASK) >> SILABS_STS2_TXUNDERRUN_SHIFT)
#define  SILABS_STS2_TXUNDERRUN_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_TXUNDERRUN_MASK) | (((uint32_t)val) << SILABS_STS2_TXUNDERRUN_SHIFT))

#define  SILABS_STS2_RESERVED3_MASK 0xe0000
#define  SILABS_STS2_RESERVED3_SHIFT 17
#define  SILABS_STS2_RESERVED3_MASK_SHIFT(reg) (((reg) & SILABS_STS2_RESERVED3_MASK) >> SILABS_STS2_RESERVED3_SHIFT)
#define  SILABS_STS2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_RESERVED3_MASK) | (((uint32_t)val) << SILABS_STS2_RESERVED3_SHIFT))

#define  SILABS_STS2_SECONDARYRQ_MASK 0x10000
#define  SILABS_STS2_SECONDARYRQ_SHIFT 16
#define  SILABS_STS2_SECONDARYRQ_MASK_SHIFT(reg) (((reg) & SILABS_STS2_SECONDARYRQ_MASK) >> SILABS_STS2_SECONDARYRQ_SHIFT)
#define  SILABS_STS2_SECONDARYRQ_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_SECONDARYRQ_MASK) | (((uint32_t)val) << SILABS_STS2_SECONDARYRQ_SHIFT))

#define  SILABS_STS2_SECONDARYRXDATA_MASK 0xffff
#define  SILABS_STS2_SECONDARYRXDATA_SHIFT 0
#define  SILABS_STS2_SECONDARYRXDATA_MASK_SHIFT(reg) (((reg) & SILABS_STS2_SECONDARYRXDATA_MASK) >> SILABS_STS2_SECONDARYRXDATA_SHIFT)
#define  SILABS_STS2_SECONDARYRXDATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_STS2_SECONDARYRXDATA_MASK) | (((uint32_t)val) << SILABS_STS2_SECONDARYRXDATA_SHIFT))

//====================================================================
//Register: Receive Register for CODEC Channel (RXD2)
/** \brief This register should only be read when the RxEmpty bit is not set in the Status2 Register (or by DMA when DMARX*REQ2 is asserted). The RxFifo2 will pack 16bit words from the incoming serial line into a 32bit word. The first 16bit word received is placed in D[31:16], and the second 16bit word received is placed in D[15:0].<BR>
             When using DMA, this register address is programmed as the source. The DMARXSREQ2 output is asserted when there are one or more RxFifo2 words available to read. The DMARXBREQ2 output is asserted when there are four or more RxFifo2 words available. The RxFifo2 is flushed when the Control.EnableRxFifo2 bit is cleared (=0).*/
//====================================================================

#define  SILABS_RXD2_DATA_MASK 0xffffffff
#define  SILABS_RXD2_DATA_SHIFT 0
#define  SILABS_RXD2_DATA_MASK_SHIFT(reg) (((reg) & SILABS_RXD2_DATA_MASK) >> SILABS_RXD2_DATA_SHIFT)
#define  SILABS_RXD2_DATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_RXD2_DATA_MASK) | (((uint32_t)val) << SILABS_RXD2_DATA_SHIFT))

//====================================================================
//Register: Transmit Register for CODEC Channel (TXD2)
/** \brief This register should only be written by firmware when the TxFifoFull bit is not set in the Status2 Register (or by DMA when DMATX*REQ2 is asserted). When using DMA, this register address is programmed as the destination. The TxFifo is flushed when the Control.EnableTxFifo2 bit is cleared (=0). <BR>
             The TxFifo2 will unpack a 32bit word into 16b words for the outgoing serial line. The halfword in D[31:16] is transmitted first, followed by the halfword in D[15:0]. The LSB of the transmitted 16b word is the software FC bit (recognized by the Si3000 or Si3056 devices) to indicated if a secondary frame is requested. The SiLabs block will set the LSB to 1 if there Status.SecondaryRQ2 bit is set.*/
//====================================================================

#define  SILABS_TXD2_DATA_MASK 0xffffffff
#define  SILABS_TXD2_DATA_SHIFT 0
#define  SILABS_TXD2_DATA_MASK_SHIFT(reg) (((reg) & SILABS_TXD2_DATA_MASK) >> SILABS_TXD2_DATA_SHIFT)
#define  SILABS_TXD2_DATA_REPLACE_VAL(reg,val) (((reg) & ~SILABS_TXD2_DATA_MASK) | (((uint32_t)val) << SILABS_TXD2_DATA_SHIFT))

#endif // SILABS
