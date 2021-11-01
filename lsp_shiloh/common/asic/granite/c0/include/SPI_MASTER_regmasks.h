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
 * \file SPI_MASTER_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SPI_MASTER_REGMASKS_H_
#define _SPI_MASTER_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SPI registers (SPI_MASTER)
/** \brief Registers for the SPI Master module*/
//
//====================================================================

//====================================================================
//Register: Control Register 0 (CTRLR0)
/** \brief This register controls the serial data transfer. It is impossible to write to this
register when the DW_apb_ssi is enabled. The DW_apb_ssi is enabled
and disabled by writing to the SSIENR register.*/
//====================================================================

#define  SPI_MASTER_CTRLR0_RESERVED1_MASK 0xffff0000
#define  SPI_MASTER_CTRLR0_RESERVED1_SHIFT 16
#define  SPI_MASTER_CTRLR0_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_RESERVED1_MASK) >> SPI_MASTER_CTRLR0_RESERVED1_SHIFT)
#define  SPI_MASTER_CTRLR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_RESERVED1_SHIFT))

#define  SPI_MASTER_CTRLR0_CFS_MASK 0xf000
#define  SPI_MASTER_CTRLR0_CFS_SHIFT 12
#define  SPI_MASTER_CTRLR0_CFS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_CFS_MASK) >> SPI_MASTER_CTRLR0_CFS_SHIFT)
#define  SPI_MASTER_CTRLR0_CFS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_CFS_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_CFS_SHIFT))

#define  SPI_MASTER_CTRLR0_SRL_MASK 0x800
#define  SPI_MASTER_CTRLR0_SRL_SHIFT 11
#define  SPI_MASTER_CTRLR0_SRL_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_SRL_MASK) >> SPI_MASTER_CTRLR0_SRL_SHIFT)
#define  SPI_MASTER_CTRLR0_SRL_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_SRL_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_SRL_SHIFT))

#define  SPI_MASTER_CTRLR0_SLV_OE_MASK 0x400
#define  SPI_MASTER_CTRLR0_SLV_OE_SHIFT 10
#define  SPI_MASTER_CTRLR0_SLV_OE_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_SLV_OE_MASK) >> SPI_MASTER_CTRLR0_SLV_OE_SHIFT)
#define  SPI_MASTER_CTRLR0_SLV_OE_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_SLV_OE_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_SLV_OE_SHIFT))

#define  SPI_MASTER_CTRLR0_TMOD_MASK 0x300
#define  SPI_MASTER_CTRLR0_TMOD_SHIFT 8
#define  SPI_MASTER_CTRLR0_TMOD_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_TMOD_MASK) >> SPI_MASTER_CTRLR0_TMOD_SHIFT)
#define  SPI_MASTER_CTRLR0_TMOD_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_TMOD_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_TMOD_SHIFT))

#define  SPI_MASTER_CTRLR0_SCPOL_MASK 0x80
#define  SPI_MASTER_CTRLR0_SCPOL_SHIFT 7
#define  SPI_MASTER_CTRLR0_SCPOL_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_SCPOL_MASK) >> SPI_MASTER_CTRLR0_SCPOL_SHIFT)
#define  SPI_MASTER_CTRLR0_SCPOL_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_SCPOL_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_SCPOL_SHIFT))

#define  SPI_MASTER_CTRLR0_SCPH_MASK 0x40
#define  SPI_MASTER_CTRLR0_SCPH_SHIFT 6
#define  SPI_MASTER_CTRLR0_SCPH_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_SCPH_MASK) >> SPI_MASTER_CTRLR0_SCPH_SHIFT)
#define  SPI_MASTER_CTRLR0_SCPH_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_SCPH_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_SCPH_SHIFT))

#define  SPI_MASTER_CTRLR0_FRF_MASK 0x30
#define  SPI_MASTER_CTRLR0_FRF_SHIFT 4
#define  SPI_MASTER_CTRLR0_FRF_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_FRF_MASK) >> SPI_MASTER_CTRLR0_FRF_SHIFT)
#define  SPI_MASTER_CTRLR0_FRF_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_FRF_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_FRF_SHIFT))

#define  SPI_MASTER_CTRLR0_DFS_MASK 0xf
#define  SPI_MASTER_CTRLR0_DFS_SHIFT 0
#define  SPI_MASTER_CTRLR0_DFS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR0_DFS_MASK) >> SPI_MASTER_CTRLR0_DFS_SHIFT)
#define  SPI_MASTER_CTRLR0_DFS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR0_DFS_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR0_DFS_SHIFT))

//====================================================================
//Register: Control Register 1 (CTRLR1)
/** \brief This register exists only when the DW_apb_ssi is configured as a master device. When the
DW_apb_ssi is configured as a serial slave, writing to this location has no effect; reading from this
location returns 0. Control register 1 controls the end of serial transfers when in receive-only mode. It
is impossible to write to this register when the DW_apb_ssi is enabled. The DW_apb_ssi is enabled and
disabled by writing to the SSIENR register.*/
//====================================================================

#define  SPI_MASTER_CTRLR1_RESERVED1_MASK 0xffff0000
#define  SPI_MASTER_CTRLR1_RESERVED1_SHIFT 16
#define  SPI_MASTER_CTRLR1_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR1_RESERVED1_MASK) >> SPI_MASTER_CTRLR1_RESERVED1_SHIFT)
#define  SPI_MASTER_CTRLR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR1_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR1_RESERVED1_SHIFT))

#define  SPI_MASTER_CTRLR1_NDF_MASK 0xffff
#define  SPI_MASTER_CTRLR1_NDF_SHIFT 0
#define  SPI_MASTER_CTRLR1_NDF_MASK_SHIFT(reg) (((reg) & SPI_MASTER_CTRLR1_NDF_MASK) >> SPI_MASTER_CTRLR1_NDF_SHIFT)
#define  SPI_MASTER_CTRLR1_NDF_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_CTRLR1_NDF_MASK) | (((uint32_t)val) << SPI_MASTER_CTRLR1_NDF_SHIFT))

//====================================================================
//Register: SSI Enable Register (SSIENR)
/** \brief This register enables and disables the DW_apb_ssi.*/
//====================================================================

#define  SPI_MASTER_SSIENR_RESERVED1_MASK 0xfffffffe
#define  SPI_MASTER_SSIENR_RESERVED1_SHIFT 1
#define  SPI_MASTER_SSIENR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SSIENR_RESERVED1_MASK) >> SPI_MASTER_SSIENR_RESERVED1_SHIFT)
#define  SPI_MASTER_SSIENR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SSIENR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_SSIENR_RESERVED1_SHIFT))

#define  SPI_MASTER_SSIENR_SSI_EN_MASK 0x1
#define  SPI_MASTER_SSIENR_SSI_EN_SHIFT 0
#define  SPI_MASTER_SSIENR_SSI_EN_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SSIENR_SSI_EN_MASK) >> SPI_MASTER_SSIENR_SSI_EN_SHIFT)
#define  SPI_MASTER_SSIENR_SSI_EN_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SSIENR_SSI_EN_MASK) | (((uint32_t)val) << SPI_MASTER_SSIENR_SSI_EN_SHIFT))

//====================================================================
//Register: Slave Enable Register (SER)
/** \brief This register is valid only when the DW_apb_ssi is configured as a master device. When the
DW_apb_ssi is configured as a serial slave, writing to this location has no effect; reading from this
location returns 0. The register enables the individual slave select output lines from the DW_apb_ssi
master. Up to 16 slave-select output signals are available on the DW_apb_ssi master. You cannot write
to this register when DW_apb_ssi is busy.*/
//====================================================================

#define  SPI_MASTER_SER_RESERVED1_MASK 0xfffffffc
#define  SPI_MASTER_SER_RESERVED1_SHIFT 2
#define  SPI_MASTER_SER_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SER_RESERVED1_MASK) >> SPI_MASTER_SER_RESERVED1_SHIFT)
#define  SPI_MASTER_SER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SER_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_SER_RESERVED1_SHIFT))

#define  SPI_MASTER_SER_SER_MASK 0x3
#define  SPI_MASTER_SER_SER_SHIFT 0
#define  SPI_MASTER_SER_SER_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SER_SER_MASK) >> SPI_MASTER_SER_SER_SHIFT)
#define  SPI_MASTER_SER_SER_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SER_SER_MASK) | (((uint32_t)val) << SPI_MASTER_SER_SER_SHIFT))

//====================================================================
//Register: Baud Rate Select (BAUDR)
/** \brief This register is valid only when the DW_apb_ssi is configured as a master device. When the
DW_apb_ssi is configured as a serial slave, writing to this location has no effect; reading from this
location returns 0. The register derives the frequency of the serial clock that regulates the data transfer.
The 16-bit field in this register defines the ssi_clk divider value. It is impossible to write to this register
when the DW_apb_ssi is enabled. The DW_apb_ssi is enabled and disabled by writing to the SSIENR
register.*/
//====================================================================

#define  SPI_MASTER_BAUDR_RESERVED1_MASK 0xffff0000
#define  SPI_MASTER_BAUDR_RESERVED1_SHIFT 16
#define  SPI_MASTER_BAUDR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_BAUDR_RESERVED1_MASK) >> SPI_MASTER_BAUDR_RESERVED1_SHIFT)
#define  SPI_MASTER_BAUDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_BAUDR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_BAUDR_RESERVED1_SHIFT))

#define  SPI_MASTER_BAUDR_SCKDV_MASK 0xffff
#define  SPI_MASTER_BAUDR_SCKDV_SHIFT 0
#define  SPI_MASTER_BAUDR_SCKDV_MASK_SHIFT(reg) (((reg) & SPI_MASTER_BAUDR_SCKDV_MASK) >> SPI_MASTER_BAUDR_SCKDV_SHIFT)
#define  SPI_MASTER_BAUDR_SCKDV_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_BAUDR_SCKDV_MASK) | (((uint32_t)val) << SPI_MASTER_BAUDR_SCKDV_SHIFT))

//====================================================================
//Register: Transmit FIFO Threshold Level (TXFTLR)
/** \brief This register controls the threshold value for the transmit FIFO memory. It is impossible to write to this
register when the DW_apb_ssi is enabled. The DW_apb_ssi is enabled and disabled by writing to the
SSIENR register.*/
//====================================================================

#define  SPI_MASTER_TXFTLR_RESERVED1_MASK 0xfffffff8
#define  SPI_MASTER_TXFTLR_RESERVED1_SHIFT 3
#define  SPI_MASTER_TXFTLR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_TXFTLR_RESERVED1_MASK) >> SPI_MASTER_TXFTLR_RESERVED1_SHIFT)
#define  SPI_MASTER_TXFTLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_TXFTLR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_TXFTLR_RESERVED1_SHIFT))

#define  SPI_MASTER_TXFTLR_TFT_MASK 0x7
#define  SPI_MASTER_TXFTLR_TFT_SHIFT 0
#define  SPI_MASTER_TXFTLR_TFT_MASK_SHIFT(reg) (((reg) & SPI_MASTER_TXFTLR_TFT_MASK) >> SPI_MASTER_TXFTLR_TFT_SHIFT)
#define  SPI_MASTER_TXFTLR_TFT_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_TXFTLR_TFT_MASK) | (((uint32_t)val) << SPI_MASTER_TXFTLR_TFT_SHIFT))

//====================================================================
//Register: Receive FIFO Threshold Level (RXFTLR)
/** \brief This register controls the threshold value for the receive FIFO memory. It is impossible to write to this
register when the DW_apb_ssi is enabled. The DW_apb_ssi is enabled and disabled by writing to the
SSIENR register.*/
//====================================================================

#define  SPI_MASTER_RXFTLR_RESERVED1_MASK 0xfffffff8
#define  SPI_MASTER_RXFTLR_RESERVED1_SHIFT 3
#define  SPI_MASTER_RXFTLR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXFTLR_RESERVED1_MASK) >> SPI_MASTER_RXFTLR_RESERVED1_SHIFT)
#define  SPI_MASTER_RXFTLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXFTLR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_RXFTLR_RESERVED1_SHIFT))

#define  SPI_MASTER_RXFTLR_RFT_MASK 0x7
#define  SPI_MASTER_RXFTLR_RFT_SHIFT 0
#define  SPI_MASTER_RXFTLR_RFT_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXFTLR_RFT_MASK) >> SPI_MASTER_RXFTLR_RFT_SHIFT)
#define  SPI_MASTER_RXFTLR_RFT_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXFTLR_RFT_MASK) | (((uint32_t)val) << SPI_MASTER_RXFTLR_RFT_SHIFT))

//====================================================================
//Register: Transmit FIFO Level Register (TXFLR)
/** \brief This register contains the number of valid data entries in the transmit FIFO memory.*/
//====================================================================

#define  SPI_MASTER_TXFLR_RESERVED1_MASK 0xffffff00
#define  SPI_MASTER_TXFLR_RESERVED1_SHIFT 8
#define  SPI_MASTER_TXFLR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_TXFLR_RESERVED1_MASK) >> SPI_MASTER_TXFLR_RESERVED1_SHIFT)
#define  SPI_MASTER_TXFLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_TXFLR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_TXFLR_RESERVED1_SHIFT))

#define  SPI_MASTER_TXFLR_TXTFL_MASK 0xff
#define  SPI_MASTER_TXFLR_TXTFL_SHIFT 0
#define  SPI_MASTER_TXFLR_TXTFL_MASK_SHIFT(reg) (((reg) & SPI_MASTER_TXFLR_TXTFL_MASK) >> SPI_MASTER_TXFLR_TXTFL_SHIFT)
#define  SPI_MASTER_TXFLR_TXTFL_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_TXFLR_TXTFL_MASK) | (((uint32_t)val) << SPI_MASTER_TXFLR_TXTFL_SHIFT))

//====================================================================
//Register: Receive FIFO Level Register (RXFLR)
/** \brief This register contains the number of valid data entries in the transmit FIFO memory. This register can
be read at any time*/
//====================================================================

#define  SPI_MASTER_RXFLR_RESERVED1_MASK 0xffffff00
#define  SPI_MASTER_RXFLR_RESERVED1_SHIFT 8
#define  SPI_MASTER_RXFLR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXFLR_RESERVED1_MASK) >> SPI_MASTER_RXFLR_RESERVED1_SHIFT)
#define  SPI_MASTER_RXFLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXFLR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_RXFLR_RESERVED1_SHIFT))

#define  SPI_MASTER_RXFLR_RXTFL_MASK 0xff
#define  SPI_MASTER_RXFLR_RXTFL_SHIFT 0
#define  SPI_MASTER_RXFLR_RXTFL_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXFLR_RXTFL_MASK) >> SPI_MASTER_RXFLR_RXTFL_SHIFT)
#define  SPI_MASTER_RXFLR_RXTFL_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXFLR_RXTFL_MASK) | (((uint32_t)val) << SPI_MASTER_RXFLR_RXTFL_SHIFT))

//====================================================================
//Register: Status Register (SR)
/** \brief This is a read-only register used to indicate the current transfer status, FIFO status, and any
transmission/reception errors that may have occurred. The status register may be read at any time.
None of the bits in this register request an interrupt.*/
//====================================================================

#define  SPI_MASTER_SR_RESERVED1_MASK 0xffffff80
#define  SPI_MASTER_SR_RESERVED1_SHIFT 7
#define  SPI_MASTER_SR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_RESERVED1_MASK) >> SPI_MASTER_SR_RESERVED1_SHIFT)
#define  SPI_MASTER_SR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_SR_RESERVED1_SHIFT))

#define  SPI_MASTER_SR_DCOL_MASK 0x40
#define  SPI_MASTER_SR_DCOL_SHIFT 6
#define  SPI_MASTER_SR_DCOL_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_DCOL_MASK) >> SPI_MASTER_SR_DCOL_SHIFT)
#define  SPI_MASTER_SR_DCOL_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_DCOL_MASK) | (((uint32_t)val) << SPI_MASTER_SR_DCOL_SHIFT))

#define  SPI_MASTER_SR_TXE_MASK 0x20
#define  SPI_MASTER_SR_TXE_SHIFT 5
#define  SPI_MASTER_SR_TXE_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_TXE_MASK) >> SPI_MASTER_SR_TXE_SHIFT)
#define  SPI_MASTER_SR_TXE_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_TXE_MASK) | (((uint32_t)val) << SPI_MASTER_SR_TXE_SHIFT))

#define  SPI_MASTER_SR_RFF_MASK 0x10
#define  SPI_MASTER_SR_RFF_SHIFT 4
#define  SPI_MASTER_SR_RFF_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_RFF_MASK) >> SPI_MASTER_SR_RFF_SHIFT)
#define  SPI_MASTER_SR_RFF_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_RFF_MASK) | (((uint32_t)val) << SPI_MASTER_SR_RFF_SHIFT))

#define  SPI_MASTER_SR_RFNE_MASK 0x8
#define  SPI_MASTER_SR_RFNE_SHIFT 3
#define  SPI_MASTER_SR_RFNE_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_RFNE_MASK) >> SPI_MASTER_SR_RFNE_SHIFT)
#define  SPI_MASTER_SR_RFNE_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_RFNE_MASK) | (((uint32_t)val) << SPI_MASTER_SR_RFNE_SHIFT))

#define  SPI_MASTER_SR_TFE_MASK 0x4
#define  SPI_MASTER_SR_TFE_SHIFT 2
#define  SPI_MASTER_SR_TFE_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_TFE_MASK) >> SPI_MASTER_SR_TFE_SHIFT)
#define  SPI_MASTER_SR_TFE_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_TFE_MASK) | (((uint32_t)val) << SPI_MASTER_SR_TFE_SHIFT))

#define  SPI_MASTER_SR_TFNF_MASK 0x2
#define  SPI_MASTER_SR_TFNF_SHIFT 1
#define  SPI_MASTER_SR_TFNF_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_TFNF_MASK) >> SPI_MASTER_SR_TFNF_SHIFT)
#define  SPI_MASTER_SR_TFNF_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_TFNF_MASK) | (((uint32_t)val) << SPI_MASTER_SR_TFNF_SHIFT))

#define  SPI_MASTER_SR_BUSY_MASK 0x1
#define  SPI_MASTER_SR_BUSY_SHIFT 0
#define  SPI_MASTER_SR_BUSY_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SR_BUSY_MASK) >> SPI_MASTER_SR_BUSY_SHIFT)
#define  SPI_MASTER_SR_BUSY_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SR_BUSY_MASK) | (((uint32_t)val) << SPI_MASTER_SR_BUSY_SHIFT))

//====================================================================
//Register: Interrupt Mask Register (IMR)
/** \brief This read/write register masks or enables all interrupts generated by the DW_apb_ssi. When the
DW_apb_ssi is configured as a slave device, the MSTIM bit field is not present. This changes the reset
value from 0x3F for serial-master configurations to 0x1F for serial-slave configurations*/
//====================================================================

#define  SPI_MASTER_IMR_RESERVED1_MASK 0xffffffc0
#define  SPI_MASTER_IMR_RESERVED1_SHIFT 6
#define  SPI_MASTER_IMR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IMR_RESERVED1_MASK) >> SPI_MASTER_IMR_RESERVED1_SHIFT)
#define  SPI_MASTER_IMR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IMR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_IMR_RESERVED1_SHIFT))

#define  SPI_MASTER_IMR_MSTIM_MASK 0x20
#define  SPI_MASTER_IMR_MSTIM_SHIFT 5
#define  SPI_MASTER_IMR_MSTIM_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IMR_MSTIM_MASK) >> SPI_MASTER_IMR_MSTIM_SHIFT)
#define  SPI_MASTER_IMR_MSTIM_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IMR_MSTIM_MASK) | (((uint32_t)val) << SPI_MASTER_IMR_MSTIM_SHIFT))

#define  SPI_MASTER_IMR_RXFIM_MASK 0x10
#define  SPI_MASTER_IMR_RXFIM_SHIFT 4
#define  SPI_MASTER_IMR_RXFIM_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IMR_RXFIM_MASK) >> SPI_MASTER_IMR_RXFIM_SHIFT)
#define  SPI_MASTER_IMR_RXFIM_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IMR_RXFIM_MASK) | (((uint32_t)val) << SPI_MASTER_IMR_RXFIM_SHIFT))

#define  SPI_MASTER_IMR_RXOIM_MASK 0x8
#define  SPI_MASTER_IMR_RXOIM_SHIFT 3
#define  SPI_MASTER_IMR_RXOIM_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IMR_RXOIM_MASK) >> SPI_MASTER_IMR_RXOIM_SHIFT)
#define  SPI_MASTER_IMR_RXOIM_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IMR_RXOIM_MASK) | (((uint32_t)val) << SPI_MASTER_IMR_RXOIM_SHIFT))

#define  SPI_MASTER_IMR_RXUIM_MASK 0x4
#define  SPI_MASTER_IMR_RXUIM_SHIFT 2
#define  SPI_MASTER_IMR_RXUIM_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IMR_RXUIM_MASK) >> SPI_MASTER_IMR_RXUIM_SHIFT)
#define  SPI_MASTER_IMR_RXUIM_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IMR_RXUIM_MASK) | (((uint32_t)val) << SPI_MASTER_IMR_RXUIM_SHIFT))

#define  SPI_MASTER_IMR_TXOIM_MASK 0x2
#define  SPI_MASTER_IMR_TXOIM_SHIFT 1
#define  SPI_MASTER_IMR_TXOIM_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IMR_TXOIM_MASK) >> SPI_MASTER_IMR_TXOIM_SHIFT)
#define  SPI_MASTER_IMR_TXOIM_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IMR_TXOIM_MASK) | (((uint32_t)val) << SPI_MASTER_IMR_TXOIM_SHIFT))

#define  SPI_MASTER_IMR_TXEIM_MASK 0x1
#define  SPI_MASTER_IMR_TXEIM_SHIFT 0
#define  SPI_MASTER_IMR_TXEIM_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IMR_TXEIM_MASK) >> SPI_MASTER_IMR_TXEIM_SHIFT)
#define  SPI_MASTER_IMR_TXEIM_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IMR_TXEIM_MASK) | (((uint32_t)val) << SPI_MASTER_IMR_TXEIM_SHIFT))

//====================================================================
//Register: Interrupt Status Register (ISR)
/** \brief This register reports the status of the DW_apb_ssi interrupts after they have been masked.*/
//====================================================================

#define  SPI_MASTER_ISR_RESERVED1_MASK 0xffffffc0
#define  SPI_MASTER_ISR_RESERVED1_SHIFT 6
#define  SPI_MASTER_ISR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ISR_RESERVED1_MASK) >> SPI_MASTER_ISR_RESERVED1_SHIFT)
#define  SPI_MASTER_ISR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ISR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_ISR_RESERVED1_SHIFT))

#define  SPI_MASTER_ISR_MSTIS_MASK 0x20
#define  SPI_MASTER_ISR_MSTIS_SHIFT 5
#define  SPI_MASTER_ISR_MSTIS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ISR_MSTIS_MASK) >> SPI_MASTER_ISR_MSTIS_SHIFT)
#define  SPI_MASTER_ISR_MSTIS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ISR_MSTIS_MASK) | (((uint32_t)val) << SPI_MASTER_ISR_MSTIS_SHIFT))

#define  SPI_MASTER_ISR_RXFIS_MASK 0x10
#define  SPI_MASTER_ISR_RXFIS_SHIFT 4
#define  SPI_MASTER_ISR_RXFIS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ISR_RXFIS_MASK) >> SPI_MASTER_ISR_RXFIS_SHIFT)
#define  SPI_MASTER_ISR_RXFIS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ISR_RXFIS_MASK) | (((uint32_t)val) << SPI_MASTER_ISR_RXFIS_SHIFT))

#define  SPI_MASTER_ISR_RXOIS_MASK 0x8
#define  SPI_MASTER_ISR_RXOIS_SHIFT 3
#define  SPI_MASTER_ISR_RXOIS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ISR_RXOIS_MASK) >> SPI_MASTER_ISR_RXOIS_SHIFT)
#define  SPI_MASTER_ISR_RXOIS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ISR_RXOIS_MASK) | (((uint32_t)val) << SPI_MASTER_ISR_RXOIS_SHIFT))

#define  SPI_MASTER_ISR_RXUIS_MASK 0x4
#define  SPI_MASTER_ISR_RXUIS_SHIFT 2
#define  SPI_MASTER_ISR_RXUIS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ISR_RXUIS_MASK) >> SPI_MASTER_ISR_RXUIS_SHIFT)
#define  SPI_MASTER_ISR_RXUIS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ISR_RXUIS_MASK) | (((uint32_t)val) << SPI_MASTER_ISR_RXUIS_SHIFT))

#define  SPI_MASTER_ISR_TXOIS_MASK 0x2
#define  SPI_MASTER_ISR_TXOIS_SHIFT 1
#define  SPI_MASTER_ISR_TXOIS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ISR_TXOIS_MASK) >> SPI_MASTER_ISR_TXOIS_SHIFT)
#define  SPI_MASTER_ISR_TXOIS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ISR_TXOIS_MASK) | (((uint32_t)val) << SPI_MASTER_ISR_TXOIS_SHIFT))

#define  SPI_MASTER_ISR_TXEIS_MASK 0x1
#define  SPI_MASTER_ISR_TXEIS_SHIFT 0
#define  SPI_MASTER_ISR_TXEIS_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ISR_TXEIS_MASK) >> SPI_MASTER_ISR_TXEIS_SHIFT)
#define  SPI_MASTER_ISR_TXEIS_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ISR_TXEIS_MASK) | (((uint32_t)val) << SPI_MASTER_ISR_TXEIS_SHIFT))

//====================================================================
//Register: Raw Interrupt Status Register (RISR)
/** \brief This read-only register reports the status of the DW_apb_ssi interrupts prior to masking*/
//====================================================================

#define  SPI_MASTER_RISR_RESERVED1_MASK 0xffffffc0
#define  SPI_MASTER_RISR_RESERVED1_SHIFT 6
#define  SPI_MASTER_RISR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RISR_RESERVED1_MASK) >> SPI_MASTER_RISR_RESERVED1_SHIFT)
#define  SPI_MASTER_RISR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RISR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_RISR_RESERVED1_SHIFT))

#define  SPI_MASTER_RISR_MSTIR_MASK 0x20
#define  SPI_MASTER_RISR_MSTIR_SHIFT 5
#define  SPI_MASTER_RISR_MSTIR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RISR_MSTIR_MASK) >> SPI_MASTER_RISR_MSTIR_SHIFT)
#define  SPI_MASTER_RISR_MSTIR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RISR_MSTIR_MASK) | (((uint32_t)val) << SPI_MASTER_RISR_MSTIR_SHIFT))

#define  SPI_MASTER_RISR_RXFIR_MASK 0x10
#define  SPI_MASTER_RISR_RXFIR_SHIFT 4
#define  SPI_MASTER_RISR_RXFIR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RISR_RXFIR_MASK) >> SPI_MASTER_RISR_RXFIR_SHIFT)
#define  SPI_MASTER_RISR_RXFIR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RISR_RXFIR_MASK) | (((uint32_t)val) << SPI_MASTER_RISR_RXFIR_SHIFT))

#define  SPI_MASTER_RISR_RXOIR_MASK 0x8
#define  SPI_MASTER_RISR_RXOIR_SHIFT 3
#define  SPI_MASTER_RISR_RXOIR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RISR_RXOIR_MASK) >> SPI_MASTER_RISR_RXOIR_SHIFT)
#define  SPI_MASTER_RISR_RXOIR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RISR_RXOIR_MASK) | (((uint32_t)val) << SPI_MASTER_RISR_RXOIR_SHIFT))

#define  SPI_MASTER_RISR_RXUIR_MASK 0x4
#define  SPI_MASTER_RISR_RXUIR_SHIFT 2
#define  SPI_MASTER_RISR_RXUIR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RISR_RXUIR_MASK) >> SPI_MASTER_RISR_RXUIR_SHIFT)
#define  SPI_MASTER_RISR_RXUIR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RISR_RXUIR_MASK) | (((uint32_t)val) << SPI_MASTER_RISR_RXUIR_SHIFT))

#define  SPI_MASTER_RISR_TXOIR_MASK 0x2
#define  SPI_MASTER_RISR_TXOIR_SHIFT 1
#define  SPI_MASTER_RISR_TXOIR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RISR_TXOIR_MASK) >> SPI_MASTER_RISR_TXOIR_SHIFT)
#define  SPI_MASTER_RISR_TXOIR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RISR_TXOIR_MASK) | (((uint32_t)val) << SPI_MASTER_RISR_TXOIR_SHIFT))

#define  SPI_MASTER_RISR_TXEIR_MASK 0x1
#define  SPI_MASTER_RISR_TXEIR_SHIFT 0
#define  SPI_MASTER_RISR_TXEIR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RISR_TXEIR_MASK) >> SPI_MASTER_RISR_TXEIR_SHIFT)
#define  SPI_MASTER_RISR_TXEIR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RISR_TXEIR_MASK) | (((uint32_t)val) << SPI_MASTER_RISR_TXEIR_SHIFT))

//====================================================================
//Register: Transmit FIFO Overflow Interrupt Clear Register (TXOICR)
//====================================================================

#define  SPI_MASTER_TXOICR_RESERVED1_MASK 0xfffffffe
#define  SPI_MASTER_TXOICR_RESERVED1_SHIFT 1
#define  SPI_MASTER_TXOICR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_TXOICR_RESERVED1_MASK) >> SPI_MASTER_TXOICR_RESERVED1_SHIFT)
#define  SPI_MASTER_TXOICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_TXOICR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_TXOICR_RESERVED1_SHIFT))

#define  SPI_MASTER_TXOICR_TXOICR_MASK 0x1
#define  SPI_MASTER_TXOICR_TXOICR_SHIFT 0
#define  SPI_MASTER_TXOICR_TXOICR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_TXOICR_TXOICR_MASK) >> SPI_MASTER_TXOICR_TXOICR_SHIFT)
#define  SPI_MASTER_TXOICR_TXOICR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_TXOICR_TXOICR_MASK) | (((uint32_t)val) << SPI_MASTER_TXOICR_TXOICR_SHIFT))

//====================================================================
//Register: Receive FIFO Overflow Interrupt Clear Register (RXOICR)
//====================================================================

#define  SPI_MASTER_RXOICR_RESERVED1_MASK 0xfffffffe
#define  SPI_MASTER_RXOICR_RESERVED1_SHIFT 1
#define  SPI_MASTER_RXOICR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXOICR_RESERVED1_MASK) >> SPI_MASTER_RXOICR_RESERVED1_SHIFT)
#define  SPI_MASTER_RXOICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXOICR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_RXOICR_RESERVED1_SHIFT))

#define  SPI_MASTER_RXOICR_RXOICR_MASK 0x1
#define  SPI_MASTER_RXOICR_RXOICR_SHIFT 0
#define  SPI_MASTER_RXOICR_RXOICR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXOICR_RXOICR_MASK) >> SPI_MASTER_RXOICR_RXOICR_SHIFT)
#define  SPI_MASTER_RXOICR_RXOICR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXOICR_RXOICR_MASK) | (((uint32_t)val) << SPI_MASTER_RXOICR_RXOICR_SHIFT))

//====================================================================
//Register: Receive FIFO Underflow Interrupt Clear Register (RXUICR)
//====================================================================

#define  SPI_MASTER_RXUICR_RESERVED1_MASK 0xfffffffe
#define  SPI_MASTER_RXUICR_RESERVED1_SHIFT 1
#define  SPI_MASTER_RXUICR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXUICR_RESERVED1_MASK) >> SPI_MASTER_RXUICR_RESERVED1_SHIFT)
#define  SPI_MASTER_RXUICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXUICR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_RXUICR_RESERVED1_SHIFT))

#define  SPI_MASTER_RXUICR_RXUICR_MASK 0x1
#define  SPI_MASTER_RXUICR_RXUICR_SHIFT 0
#define  SPI_MASTER_RXUICR_RXUICR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_RXUICR_RXUICR_MASK) >> SPI_MASTER_RXUICR_RXUICR_SHIFT)
#define  SPI_MASTER_RXUICR_RXUICR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_RXUICR_RXUICR_MASK) | (((uint32_t)val) << SPI_MASTER_RXUICR_RXUICR_SHIFT))

//====================================================================
//Register: Multi-Master Interrupt Clear Register (MSTICR)
//====================================================================

#define  SPI_MASTER_MSTICR_RESERVED1_MASK 0xfffffffe
#define  SPI_MASTER_MSTICR_RESERVED1_SHIFT 1
#define  SPI_MASTER_MSTICR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_MSTICR_RESERVED1_MASK) >> SPI_MASTER_MSTICR_RESERVED1_SHIFT)
#define  SPI_MASTER_MSTICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_MSTICR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_MSTICR_RESERVED1_SHIFT))

#define  SPI_MASTER_MSTICR_MSTICR_MASK 0x1
#define  SPI_MASTER_MSTICR_MSTICR_SHIFT 0
#define  SPI_MASTER_MSTICR_MSTICR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_MSTICR_MSTICR_MASK) >> SPI_MASTER_MSTICR_MSTICR_SHIFT)
#define  SPI_MASTER_MSTICR_MSTICR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_MSTICR_MSTICR_MASK) | (((uint32_t)val) << SPI_MASTER_MSTICR_MSTICR_SHIFT))

//====================================================================
//Register: Interrupt Clear Register (ICR)
//====================================================================

#define  SPI_MASTER_ICR_RESERVED1_MASK 0xfffffffe
#define  SPI_MASTER_ICR_RESERVED1_SHIFT 1
#define  SPI_MASTER_ICR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ICR_RESERVED1_MASK) >> SPI_MASTER_ICR_RESERVED1_SHIFT)
#define  SPI_MASTER_ICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ICR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_ICR_RESERVED1_SHIFT))

#define  SPI_MASTER_ICR_ICR_MASK 0x1
#define  SPI_MASTER_ICR_ICR_SHIFT 0
#define  SPI_MASTER_ICR_ICR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_ICR_ICR_MASK) >> SPI_MASTER_ICR_ICR_SHIFT)
#define  SPI_MASTER_ICR_ICR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_ICR_ICR_MASK) | (((uint32_t)val) << SPI_MASTER_ICR_ICR_SHIFT))

//====================================================================
//Register: DMA Control Register (DMACR)
/** \brief This register is only valid when DW_apb_ssi is configured with a set of DMA Controller interface
signals (SSI_HAS_DMA = 1). When DW_apb_ssi is not configured for DMA operation, this register
will not exist and writing to the register's address will have no effect; reading from this register address
will return zero. The register is used to enable the DMA Controller interface operation.*/
//====================================================================

#define  SPI_MASTER_DMACR_RESERVED1_MASK 0xfffffffc
#define  SPI_MASTER_DMACR_RESERVED1_SHIFT 2
#define  SPI_MASTER_DMACR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DMACR_RESERVED1_MASK) >> SPI_MASTER_DMACR_RESERVED1_SHIFT)
#define  SPI_MASTER_DMACR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DMACR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_DMACR_RESERVED1_SHIFT))

#define  SPI_MASTER_DMACR_TDMAE_MASK 0x2
#define  SPI_MASTER_DMACR_TDMAE_SHIFT 1
#define  SPI_MASTER_DMACR_TDMAE_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DMACR_TDMAE_MASK) >> SPI_MASTER_DMACR_TDMAE_SHIFT)
#define  SPI_MASTER_DMACR_TDMAE_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DMACR_TDMAE_MASK) | (((uint32_t)val) << SPI_MASTER_DMACR_TDMAE_SHIFT))

#define  SPI_MASTER_DMACR_RDMAE_MASK 0x1
#define  SPI_MASTER_DMACR_RDMAE_SHIFT 0
#define  SPI_MASTER_DMACR_RDMAE_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DMACR_RDMAE_MASK) >> SPI_MASTER_DMACR_RDMAE_SHIFT)
#define  SPI_MASTER_DMACR_RDMAE_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DMACR_RDMAE_MASK) | (((uint32_t)val) << SPI_MASTER_DMACR_RDMAE_SHIFT))

//====================================================================
//Register: DMA Transmit Data Level (DMATDLR)
/** \brief This register is only valid when the DW_apb_ssi is configured with a set of DMA interface signals
(SSI_HAS_DMA = 1). When DW_apb_ssi is not configured for DMA operation, this register will not
exist and writing to its address will have no effect; reading from its address will return zero*/
//====================================================================

#define  SPI_MASTER_DMATDLR_RESERVED1_MASK 0xfffffff8
#define  SPI_MASTER_DMATDLR_RESERVED1_SHIFT 3
#define  SPI_MASTER_DMATDLR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DMATDLR_RESERVED1_MASK) >> SPI_MASTER_DMATDLR_RESERVED1_SHIFT)
#define  SPI_MASTER_DMATDLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DMATDLR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_DMATDLR_RESERVED1_SHIFT))

#define  SPI_MASTER_DMATDLR_DMATDL_MASK 0x7
#define  SPI_MASTER_DMATDLR_DMATDL_SHIFT 0
#define  SPI_MASTER_DMATDLR_DMATDL_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DMATDLR_DMATDL_MASK) >> SPI_MASTER_DMATDLR_DMATDL_SHIFT)
#define  SPI_MASTER_DMATDLR_DMATDL_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DMATDLR_DMATDL_MASK) | (((uint32_t)val) << SPI_MASTER_DMATDLR_DMATDL_SHIFT))

//====================================================================
//Register: DMA Receive Data Level (DMARDLR)
/** \brief This register is only valid when DW_apb_ssi is configured with a set of DMA interface signals
(SSI_HAS_DMA = 1). When DW_apb_ssi is not configured for DMA operation, this register will not
exist and writing to its address will have no effect; reading from its address will return zero.*/
//====================================================================

#define  SPI_MASTER_DMARDLR_RESERVED1_MASK 0xfffffff8
#define  SPI_MASTER_DMARDLR_RESERVED1_SHIFT 3
#define  SPI_MASTER_DMARDLR_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DMARDLR_RESERVED1_MASK) >> SPI_MASTER_DMARDLR_RESERVED1_SHIFT)
#define  SPI_MASTER_DMARDLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DMARDLR_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_DMARDLR_RESERVED1_SHIFT))

#define  SPI_MASTER_DMARDLR_DMARDL_MASK 0x7
#define  SPI_MASTER_DMARDLR_DMARDL_SHIFT 0
#define  SPI_MASTER_DMARDLR_DMARDL_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DMARDLR_DMARDL_MASK) >> SPI_MASTER_DMARDLR_DMARDL_SHIFT)
#define  SPI_MASTER_DMARDLR_DMARDL_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DMARDLR_DMARDL_MASK) | (((uint32_t)val) << SPI_MASTER_DMARDLR_DMARDL_SHIFT))

//====================================================================
//Register: Identification Register (IDR)
/** \brief This read-only register is available for use to store a peripheral identification code.*/
//====================================================================

#define  SPI_MASTER_IDR_IDR_MASK 0xffffffff
#define  SPI_MASTER_IDR_IDR_SHIFT 0
#define  SPI_MASTER_IDR_IDR_MASK_SHIFT(reg) (((reg) & SPI_MASTER_IDR_IDR_MASK) >> SPI_MASTER_IDR_IDR_SHIFT)
#define  SPI_MASTER_IDR_IDR_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_IDR_IDR_MASK) | (((uint32_t)val) << SPI_MASTER_IDR_IDR_SHIFT))

//====================================================================
//Register: coreKit version ID register (SSI_COMP_VERSION)
/** \brief This read-only register stores the specific DW_apb_ssi component version*/
//====================================================================

#define  SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_MASK 0xffffffff
#define  SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_SHIFT 0
#define  SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_MASK_SHIFT(reg) (((reg) & SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_MASK) >> SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_SHIFT)
#define  SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_MASK) | (((uint32_t)val) << SPI_MASTER_SSI_COMP_VERSION_SSI_COMP_VERSION_SHIFT))

//====================================================================
//Register: Data Register (Low) (DR_LOW)
/** \brief The DW_apb_ssi data register is a 16-bit read/write buffer for the transmit/receive FIFOs. When the
register is read, data in the receive FIFO buffer is accessed. When it is written to, data are moved into
the transmit FIFO buffer; a write can occur only when SSI_EN = 1. FIFOs are reset when SSI_EN = 0.
The DR register in the DW_apb_ssi occupies sixteen 32-bit locations of the memory map
to facilitate AHB burst transfers. Writing to any of these address locations has the same
effect as pushing the data from the pwdata bus into the transmit FIFO. Reading from any
of these locations has the same effect as popping data from the receive FIFO onto the
prdata bus. The FIFO buffers on the DW_apb_ssi are not addressable.*/
//====================================================================

#define  SPI_MASTER_DR_LOW_RESERVED1_MASK 0xffff0000
#define  SPI_MASTER_DR_LOW_RESERVED1_SHIFT 16
#define  SPI_MASTER_DR_LOW_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DR_LOW_RESERVED1_MASK) >> SPI_MASTER_DR_LOW_RESERVED1_SHIFT)
#define  SPI_MASTER_DR_LOW_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DR_LOW_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_DR_LOW_RESERVED1_SHIFT))

#define  SPI_MASTER_DR_LOW_DR_LOW_MASK 0xffff
#define  SPI_MASTER_DR_LOW_DR_LOW_SHIFT 0
#define  SPI_MASTER_DR_LOW_DR_LOW_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DR_LOW_DR_LOW_MASK) >> SPI_MASTER_DR_LOW_DR_LOW_SHIFT)
#define  SPI_MASTER_DR_LOW_DR_LOW_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DR_LOW_DR_LOW_MASK) | (((uint32_t)val) << SPI_MASTER_DR_LOW_DR_LOW_SHIFT))

//====================================================================
//Register: Data Register (High) (DR_HIGH)
/** \brief The DW_apb_ssi data register is a 16-bit read/write buffer for the transmit/receive FIFOs. When the
register is read, data in the receive FIFO buffer is accessed. When it is written to, data are moved into
the transmit FIFO buffer; a write can occur only when SSI_EN = 1. FIFOs are reset when SSI_EN = 0.
The DR register in the DW_apb_ssi occupies sixteen 32-bit locations of the memory map
to facilitate AHB burst transfers. Writing to any of these address locations has the same
effect as pushing the data from the pwdata bus into the transmit FIFO. Reading from any
of these locations has the same effect as popping data from the receive FIFO onto the
prdata bus. The FIFO buffers on the DW_apb_ssi are not addressable.*/
//====================================================================

#define  SPI_MASTER_DR_HIGH_RESERVED1_MASK 0xffff0000
#define  SPI_MASTER_DR_HIGH_RESERVED1_SHIFT 16
#define  SPI_MASTER_DR_HIGH_RESERVED1_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DR_HIGH_RESERVED1_MASK) >> SPI_MASTER_DR_HIGH_RESERVED1_SHIFT)
#define  SPI_MASTER_DR_HIGH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DR_HIGH_RESERVED1_MASK) | (((uint32_t)val) << SPI_MASTER_DR_HIGH_RESERVED1_SHIFT))

#define  SPI_MASTER_DR_HIGH_DR_HIGH_MASK 0xffff
#define  SPI_MASTER_DR_HIGH_DR_HIGH_SHIFT 0
#define  SPI_MASTER_DR_HIGH_DR_HIGH_MASK_SHIFT(reg) (((reg) & SPI_MASTER_DR_HIGH_DR_HIGH_MASK) >> SPI_MASTER_DR_HIGH_DR_HIGH_SHIFT)
#define  SPI_MASTER_DR_HIGH_DR_HIGH_REPLACE_VAL(reg,val) (((reg) & ~SPI_MASTER_DR_HIGH_DR_HIGH_MASK) | (((uint32_t)val) << SPI_MASTER_DR_HIGH_DR_HIGH_SHIFT))

#endif // SPI_MASTER
