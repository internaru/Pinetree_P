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
 * \file UART_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _UART_REGMASKS_H_
#define _UART_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: UART Registers (UART)
/** \brief Register Description for UART MODULE*/
//
//====================================================================

//====================================================================
//Register: Receive Buffer Register (RBR)
/** \brief The Register gives the data bytes received on the serial input port(sin) in
     UART mode, or the serial infrared input(sir_in)in infrared mode.
     The data in this register is valid only if the Data Ready(DR) bit in the
     Line Status Register (LCR) is set.*/
//====================================================================

#define  UART_RBR_RESERVED1_MASK 0xffffff00
#define  UART_RBR_RESERVED1_SHIFT 8
#define  UART_RBR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_RBR_RESERVED1_MASK) >> UART_RBR_RESERVED1_SHIFT)
#define  UART_RBR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_RBR_RESERVED1_MASK) | (((uint32_t)val) << UART_RBR_RESERVED1_SHIFT))

#define  UART_RBR_RBR_REG_MASK 0xff
#define  UART_RBR_RBR_REG_SHIFT 0
#define  UART_RBR_RBR_REG_MASK_SHIFT(reg) (((reg) & UART_RBR_RBR_REG_MASK) >> UART_RBR_RBR_REG_SHIFT)
#define  UART_RBR_RBR_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_RBR_RBR_REG_MASK) | (((uint32_t)val) << UART_RBR_RBR_REG_SHIFT))

//====================================================================
//Register: Transmit Holding Register (THR)
/** \brief Data to be transmitted on the serial output port(sout) in UART mode or the
     serial infrared output (sir_out_n) in infrared mode.Data should only be
     written to the THR when the THR Empty (THRE) nit (LSR[5]) is set.*/
//====================================================================

#define  UART_THR_RESERVED1_MASK 0xffffff00
#define  UART_THR_RESERVED1_SHIFT 8
#define  UART_THR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_THR_RESERVED1_MASK) >> UART_THR_RESERVED1_SHIFT)
#define  UART_THR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_THR_RESERVED1_MASK) | (((uint32_t)val) << UART_THR_RESERVED1_SHIFT))

#define  UART_THR_THR_REG_MASK 0xff
#define  UART_THR_THR_REG_SHIFT 0
#define  UART_THR_THR_REG_MASK_SHIFT(reg) (((reg) & UART_THR_THR_REG_MASK) >> UART_THR_THR_REG_SHIFT)
#define  UART_THR_THR_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_THR_THR_REG_MASK) | (((uint32_t)val) << UART_THR_THR_REG_SHIFT))

//====================================================================
//Register: Divisor Latch Low (DLL)
/** \brief Upper 8-bit of a 16-bit, read/write, Divisor Latch register that contains
     the baud rate divisor for the UART.If UART_16550_COMPATIBLE==NO,then this
     register may only be accessed when the DLAB bit (LCR[7] is set and the UART
     is not busy(USR[0] is zero);otherwise this register may be accessed only
     when the DLAB bit (LCR[7] is set.*/
//====================================================================

#define  UART_DLL_RESERVED1_MASK 0xffffff00
#define  UART_DLL_RESERVED1_SHIFT 8
#define  UART_DLL_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_DLL_RESERVED1_MASK) >> UART_DLL_RESERVED1_SHIFT)
#define  UART_DLL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_DLL_RESERVED1_MASK) | (((uint32_t)val) << UART_DLL_RESERVED1_SHIFT))

#define  UART_DLL_DLL_REG_MASK 0xff
#define  UART_DLL_DLL_REG_SHIFT 0
#define  UART_DLL_DLL_REG_MASK_SHIFT(reg) (((reg) & UART_DLL_DLL_REG_MASK) >> UART_DLL_DLL_REG_SHIFT)
#define  UART_DLL_DLL_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_DLL_DLL_REG_MASK) | (((uint32_t)val) << UART_DLL_DLL_REG_SHIFT))

//====================================================================
//Register: Divisor Latch High (DLH)
/** \brief Lower 8 bits of a 16-bit, read/write, Divisor Latch register that contains
     the baud rate divisor for the UART. If UART_16550_COMPATIBLE==NO,then this
     register may only be accessed when the the DLAB bit (LCR[7] is set and the
     UART is not busy(USR[0] is zero);otherwise this register may be accessed only
     when the DLAB bit (LCR[7] is set.*/
//====================================================================

#define  UART_DLH_RESERVED1_MASK 0xffffff00
#define  UART_DLH_RESERVED1_SHIFT 8
#define  UART_DLH_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_DLH_RESERVED1_MASK) >> UART_DLH_RESERVED1_SHIFT)
#define  UART_DLH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_DLH_RESERVED1_MASK) | (((uint32_t)val) << UART_DLH_RESERVED1_SHIFT))

#define  UART_DLH_DLH_REG_MASK 0xff
#define  UART_DLH_DLH_REG_SHIFT 0
#define  UART_DLH_DLH_REG_MASK_SHIFT(reg) (((reg) & UART_DLH_DLH_REG_MASK) >> UART_DLH_DLH_REG_SHIFT)
#define  UART_DLH_DLH_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_DLH_DLH_REG_MASK) | (((uint32_t)val) << UART_DLH_DLH_REG_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (IER)
//====================================================================

#define  UART_IER_RESERVED1_MASK 0xffffff00
#define  UART_IER_RESERVED1_SHIFT 8
#define  UART_IER_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_IER_RESERVED1_MASK) >> UART_IER_RESERVED1_SHIFT)
#define  UART_IER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_IER_RESERVED1_MASK) | (((uint32_t)val) << UART_IER_RESERVED1_SHIFT))

#define  UART_IER_PTIME_MASK 0x80
#define  UART_IER_PTIME_SHIFT 7
#define  UART_IER_PTIME_MASK_SHIFT(reg) (((reg) & UART_IER_PTIME_MASK) >> UART_IER_PTIME_SHIFT)
#define  UART_IER_PTIME_REPLACE_VAL(reg,val) (((reg) & ~UART_IER_PTIME_MASK) | (((uint32_t)val) << UART_IER_PTIME_SHIFT))

#define  UART_IER_RESERVED2_MASK 0x70
#define  UART_IER_RESERVED2_SHIFT 4
#define  UART_IER_RESERVED2_MASK_SHIFT(reg) (((reg) & UART_IER_RESERVED2_MASK) >> UART_IER_RESERVED2_SHIFT)
#define  UART_IER_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~UART_IER_RESERVED2_MASK) | (((uint32_t)val) << UART_IER_RESERVED2_SHIFT))

#define  UART_IER_EDSSI_MASK 0x8
#define  UART_IER_EDSSI_SHIFT 3
#define  UART_IER_EDSSI_MASK_SHIFT(reg) (((reg) & UART_IER_EDSSI_MASK) >> UART_IER_EDSSI_SHIFT)
#define  UART_IER_EDSSI_REPLACE_VAL(reg,val) (((reg) & ~UART_IER_EDSSI_MASK) | (((uint32_t)val) << UART_IER_EDSSI_SHIFT))

#define  UART_IER_ELSI_MASK 0x4
#define  UART_IER_ELSI_SHIFT 2
#define  UART_IER_ELSI_MASK_SHIFT(reg) (((reg) & UART_IER_ELSI_MASK) >> UART_IER_ELSI_SHIFT)
#define  UART_IER_ELSI_REPLACE_VAL(reg,val) (((reg) & ~UART_IER_ELSI_MASK) | (((uint32_t)val) << UART_IER_ELSI_SHIFT))

#define  UART_IER_ETBEI_MASK 0x2
#define  UART_IER_ETBEI_SHIFT 1
#define  UART_IER_ETBEI_MASK_SHIFT(reg) (((reg) & UART_IER_ETBEI_MASK) >> UART_IER_ETBEI_SHIFT)
#define  UART_IER_ETBEI_REPLACE_VAL(reg,val) (((reg) & ~UART_IER_ETBEI_MASK) | (((uint32_t)val) << UART_IER_ETBEI_SHIFT))

#define  UART_IER_ERBFI_MASK 0x1
#define  UART_IER_ERBFI_SHIFT 0
#define  UART_IER_ERBFI_MASK_SHIFT(reg) (((reg) & UART_IER_ERBFI_MASK) >> UART_IER_ERBFI_SHIFT)
#define  UART_IER_ERBFI_REPLACE_VAL(reg,val) (((reg) & ~UART_IER_ERBFI_MASK) | (((uint32_t)val) << UART_IER_ERBFI_SHIFT))

//====================================================================
//Register: Interrupt Identity Register (IIR)
//====================================================================

#define  UART_IIR_RESERVED1_MASK 0xffffff00
#define  UART_IIR_RESERVED1_SHIFT 8
#define  UART_IIR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_IIR_RESERVED1_MASK) >> UART_IIR_RESERVED1_SHIFT)
#define  UART_IIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_IIR_RESERVED1_MASK) | (((uint32_t)val) << UART_IIR_RESERVED1_SHIFT))

#define  UART_IIR_FIFOSE_MASK 0xc0
#define  UART_IIR_FIFOSE_SHIFT 6
#define  UART_IIR_FIFOSE_MASK_SHIFT(reg) (((reg) & UART_IIR_FIFOSE_MASK) >> UART_IIR_FIFOSE_SHIFT)
#define  UART_IIR_FIFOSE_REPLACE_VAL(reg,val) (((reg) & ~UART_IIR_FIFOSE_MASK) | (((uint32_t)val) << UART_IIR_FIFOSE_SHIFT))

#define  UART_IIR_RESERVED2_MASK 0x30
#define  UART_IIR_RESERVED2_SHIFT 4
#define  UART_IIR_RESERVED2_MASK_SHIFT(reg) (((reg) & UART_IIR_RESERVED2_MASK) >> UART_IIR_RESERVED2_SHIFT)
#define  UART_IIR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~UART_IIR_RESERVED2_MASK) | (((uint32_t)val) << UART_IIR_RESERVED2_SHIFT))

#define  UART_IIR_IID_MASK 0xf
#define  UART_IIR_IID_SHIFT 0
#define  UART_IIR_IID_MASK_SHIFT(reg) (((reg) & UART_IIR_IID_MASK) >> UART_IIR_IID_SHIFT)
#define  UART_IIR_IID_REPLACE_VAL(reg,val) (((reg) & ~UART_IIR_IID_MASK) | (((uint32_t)val) << UART_IIR_IID_SHIFT))

//====================================================================
//Register: FIFO Control Register (FCR)
/** \brief The Register is only valid when the DW_apb_uart is configured to have FIOFs
    implemented(FIFO_MODE!=NONE).If FIFOs are not implemented, this register
    does not exist and writing to this register address has no effect;reading
    from this register address returns zero.*/
//====================================================================

#define  UART_FCR_RESERVED1_MASK 0xffffff00
#define  UART_FCR_RESERVED1_SHIFT 8
#define  UART_FCR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_FCR_RESERVED1_MASK) >> UART_FCR_RESERVED1_SHIFT)
#define  UART_FCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_FCR_RESERVED1_MASK) | (((uint32_t)val) << UART_FCR_RESERVED1_SHIFT))

#define  UART_FCR_RCVR_TRIG_MASK 0xc0
#define  UART_FCR_RCVR_TRIG_SHIFT 6
#define  UART_FCR_RCVR_TRIG_MASK_SHIFT(reg) (((reg) & UART_FCR_RCVR_TRIG_MASK) >> UART_FCR_RCVR_TRIG_SHIFT)
#define  UART_FCR_RCVR_TRIG_REPLACE_VAL(reg,val) (((reg) & ~UART_FCR_RCVR_TRIG_MASK) | (((uint32_t)val) << UART_FCR_RCVR_TRIG_SHIFT))

#define  UART_FCR_TET_MASK 0x30
#define  UART_FCR_TET_SHIFT 4
#define  UART_FCR_TET_MASK_SHIFT(reg) (((reg) & UART_FCR_TET_MASK) >> UART_FCR_TET_SHIFT)
#define  UART_FCR_TET_REPLACE_VAL(reg,val) (((reg) & ~UART_FCR_TET_MASK) | (((uint32_t)val) << UART_FCR_TET_SHIFT))

#define  UART_FCR_DMAM_MASK 0x8
#define  UART_FCR_DMAM_SHIFT 3
#define  UART_FCR_DMAM_MASK_SHIFT(reg) (((reg) & UART_FCR_DMAM_MASK) >> UART_FCR_DMAM_SHIFT)
#define  UART_FCR_DMAM_REPLACE_VAL(reg,val) (((reg) & ~UART_FCR_DMAM_MASK) | (((uint32_t)val) << UART_FCR_DMAM_SHIFT))

#define  UART_FCR_XFIFOR_MASK 0x4
#define  UART_FCR_XFIFOR_SHIFT 2
#define  UART_FCR_XFIFOR_MASK_SHIFT(reg) (((reg) & UART_FCR_XFIFOR_MASK) >> UART_FCR_XFIFOR_SHIFT)
#define  UART_FCR_XFIFOR_REPLACE_VAL(reg,val) (((reg) & ~UART_FCR_XFIFOR_MASK) | (((uint32_t)val) << UART_FCR_XFIFOR_SHIFT))

#define  UART_FCR_RFIFOR_MASK 0x2
#define  UART_FCR_RFIFOR_SHIFT 1
#define  UART_FCR_RFIFOR_MASK_SHIFT(reg) (((reg) & UART_FCR_RFIFOR_MASK) >> UART_FCR_RFIFOR_SHIFT)
#define  UART_FCR_RFIFOR_REPLACE_VAL(reg,val) (((reg) & ~UART_FCR_RFIFOR_MASK) | (((uint32_t)val) << UART_FCR_RFIFOR_SHIFT))

#define  UART_FCR_FIFOE_MASK 0x1
#define  UART_FCR_FIFOE_SHIFT 0
#define  UART_FCR_FIFOE_MASK_SHIFT(reg) (((reg) & UART_FCR_FIFOE_MASK) >> UART_FCR_FIFOE_SHIFT)
#define  UART_FCR_FIFOE_REPLACE_VAL(reg,val) (((reg) & ~UART_FCR_FIFOE_MASK) | (((uint32_t)val) << UART_FCR_FIFOE_SHIFT))

//====================================================================
//Register: Line Control Register (LCR)
//====================================================================

#define  UART_LCR_RESERVED1_MASK 0xffffff00
#define  UART_LCR_RESERVED1_SHIFT 8
#define  UART_LCR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_LCR_RESERVED1_MASK) >> UART_LCR_RESERVED1_SHIFT)
#define  UART_LCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_RESERVED1_MASK) | (((uint32_t)val) << UART_LCR_RESERVED1_SHIFT))

#define  UART_LCR_DLAB_MASK 0x80
#define  UART_LCR_DLAB_SHIFT 7
#define  UART_LCR_DLAB_MASK_SHIFT(reg) (((reg) & UART_LCR_DLAB_MASK) >> UART_LCR_DLAB_SHIFT)
#define  UART_LCR_DLAB_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_DLAB_MASK) | (((uint32_t)val) << UART_LCR_DLAB_SHIFT))

#define  UART_LCR_BC_MASK 0x40
#define  UART_LCR_BC_SHIFT 6
#define  UART_LCR_BC_MASK_SHIFT(reg) (((reg) & UART_LCR_BC_MASK) >> UART_LCR_BC_SHIFT)
#define  UART_LCR_BC_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_BC_MASK) | (((uint32_t)val) << UART_LCR_BC_SHIFT))

#define  UART_LCR_SPS_MASK 0x20
#define  UART_LCR_SPS_SHIFT 5
#define  UART_LCR_SPS_MASK_SHIFT(reg) (((reg) & UART_LCR_SPS_MASK) >> UART_LCR_SPS_SHIFT)
#define  UART_LCR_SPS_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_SPS_MASK) | (((uint32_t)val) << UART_LCR_SPS_SHIFT))

#define  UART_LCR_EPS_MASK 0x10
#define  UART_LCR_EPS_SHIFT 4
#define  UART_LCR_EPS_MASK_SHIFT(reg) (((reg) & UART_LCR_EPS_MASK) >> UART_LCR_EPS_SHIFT)
#define  UART_LCR_EPS_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_EPS_MASK) | (((uint32_t)val) << UART_LCR_EPS_SHIFT))

#define  UART_LCR_PEN_MASK 0x8
#define  UART_LCR_PEN_SHIFT 3
#define  UART_LCR_PEN_MASK_SHIFT(reg) (((reg) & UART_LCR_PEN_MASK) >> UART_LCR_PEN_SHIFT)
#define  UART_LCR_PEN_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_PEN_MASK) | (((uint32_t)val) << UART_LCR_PEN_SHIFT))

#define  UART_LCR_STOP_MASK 0x4
#define  UART_LCR_STOP_SHIFT 2
#define  UART_LCR_STOP_MASK_SHIFT(reg) (((reg) & UART_LCR_STOP_MASK) >> UART_LCR_STOP_SHIFT)
#define  UART_LCR_STOP_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_STOP_MASK) | (((uint32_t)val) << UART_LCR_STOP_SHIFT))

#define  UART_LCR_DLS_MASK 0x3
#define  UART_LCR_DLS_SHIFT 0
#define  UART_LCR_DLS_MASK_SHIFT(reg) (((reg) & UART_LCR_DLS_MASK) >> UART_LCR_DLS_SHIFT)
#define  UART_LCR_DLS_REPLACE_VAL(reg,val) (((reg) & ~UART_LCR_DLS_MASK) | (((uint32_t)val) << UART_LCR_DLS_SHIFT))

//====================================================================
//Register: Modem Control Register (MCR)
//====================================================================

#define  UART_MCR_RESERVED1_MASK 0xffffff80
#define  UART_MCR_RESERVED1_SHIFT 7
#define  UART_MCR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_MCR_RESERVED1_MASK) >> UART_MCR_RESERVED1_SHIFT)
#define  UART_MCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_RESERVED1_MASK) | (((uint32_t)val) << UART_MCR_RESERVED1_SHIFT))

#define  UART_MCR_SIRE_MASK 0x40
#define  UART_MCR_SIRE_SHIFT 6
#define  UART_MCR_SIRE_MASK_SHIFT(reg) (((reg) & UART_MCR_SIRE_MASK) >> UART_MCR_SIRE_SHIFT)
#define  UART_MCR_SIRE_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_SIRE_MASK) | (((uint32_t)val) << UART_MCR_SIRE_SHIFT))

#define  UART_MCR_AFCE_MASK 0x20
#define  UART_MCR_AFCE_SHIFT 5
#define  UART_MCR_AFCE_MASK_SHIFT(reg) (((reg) & UART_MCR_AFCE_MASK) >> UART_MCR_AFCE_SHIFT)
#define  UART_MCR_AFCE_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_AFCE_MASK) | (((uint32_t)val) << UART_MCR_AFCE_SHIFT))

#define  UART_MCR_LB_MASK 0x10
#define  UART_MCR_LB_SHIFT 4
#define  UART_MCR_LB_MASK_SHIFT(reg) (((reg) & UART_MCR_LB_MASK) >> UART_MCR_LB_SHIFT)
#define  UART_MCR_LB_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_LB_MASK) | (((uint32_t)val) << UART_MCR_LB_SHIFT))

#define  UART_MCR_OUT2_MASK 0x8
#define  UART_MCR_OUT2_SHIFT 3
#define  UART_MCR_OUT2_MASK_SHIFT(reg) (((reg) & UART_MCR_OUT2_MASK) >> UART_MCR_OUT2_SHIFT)
#define  UART_MCR_OUT2_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_OUT2_MASK) | (((uint32_t)val) << UART_MCR_OUT2_SHIFT))

#define  UART_MCR_OUT1_MASK 0x4
#define  UART_MCR_OUT1_SHIFT 2
#define  UART_MCR_OUT1_MASK_SHIFT(reg) (((reg) & UART_MCR_OUT1_MASK) >> UART_MCR_OUT1_SHIFT)
#define  UART_MCR_OUT1_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_OUT1_MASK) | (((uint32_t)val) << UART_MCR_OUT1_SHIFT))

#define  UART_MCR_RTS_MASK 0x2
#define  UART_MCR_RTS_SHIFT 1
#define  UART_MCR_RTS_MASK_SHIFT(reg) (((reg) & UART_MCR_RTS_MASK) >> UART_MCR_RTS_SHIFT)
#define  UART_MCR_RTS_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_RTS_MASK) | (((uint32_t)val) << UART_MCR_RTS_SHIFT))

#define  UART_MCR_DTR_MASK 0x1
#define  UART_MCR_DTR_SHIFT 0
#define  UART_MCR_DTR_MASK_SHIFT(reg) (((reg) & UART_MCR_DTR_MASK) >> UART_MCR_DTR_SHIFT)
#define  UART_MCR_DTR_REPLACE_VAL(reg,val) (((reg) & ~UART_MCR_DTR_MASK) | (((uint32_t)val) << UART_MCR_DTR_SHIFT))

//====================================================================
//Register: Line Status Register (LSR)
//====================================================================

#define  UART_LSR_RESERVED1_MASK 0xffffff00
#define  UART_LSR_RESERVED1_SHIFT 8
#define  UART_LSR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_LSR_RESERVED1_MASK) >> UART_LSR_RESERVED1_SHIFT)
#define  UART_LSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_RESERVED1_MASK) | (((uint32_t)val) << UART_LSR_RESERVED1_SHIFT))

#define  UART_LSR_RFE_MASK 0x80
#define  UART_LSR_RFE_SHIFT 7
#define  UART_LSR_RFE_MASK_SHIFT(reg) (((reg) & UART_LSR_RFE_MASK) >> UART_LSR_RFE_SHIFT)
#define  UART_LSR_RFE_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_RFE_MASK) | (((uint32_t)val) << UART_LSR_RFE_SHIFT))

#define  UART_LSR_TEMT_MASK 0x40
#define  UART_LSR_TEMT_SHIFT 6
#define  UART_LSR_TEMT_MASK_SHIFT(reg) (((reg) & UART_LSR_TEMT_MASK) >> UART_LSR_TEMT_SHIFT)
#define  UART_LSR_TEMT_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_TEMT_MASK) | (((uint32_t)val) << UART_LSR_TEMT_SHIFT))

#define  UART_LSR_THRE_MASK 0x20
#define  UART_LSR_THRE_SHIFT 5
#define  UART_LSR_THRE_MASK_SHIFT(reg) (((reg) & UART_LSR_THRE_MASK) >> UART_LSR_THRE_SHIFT)
#define  UART_LSR_THRE_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_THRE_MASK) | (((uint32_t)val) << UART_LSR_THRE_SHIFT))

#define  UART_LSR_BI_MASK 0x10
#define  UART_LSR_BI_SHIFT 4
#define  UART_LSR_BI_MASK_SHIFT(reg) (((reg) & UART_LSR_BI_MASK) >> UART_LSR_BI_SHIFT)
#define  UART_LSR_BI_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_BI_MASK) | (((uint32_t)val) << UART_LSR_BI_SHIFT))

#define  UART_LSR_FE_MASK 0x8
#define  UART_LSR_FE_SHIFT 3
#define  UART_LSR_FE_MASK_SHIFT(reg) (((reg) & UART_LSR_FE_MASK) >> UART_LSR_FE_SHIFT)
#define  UART_LSR_FE_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_FE_MASK) | (((uint32_t)val) << UART_LSR_FE_SHIFT))

#define  UART_LSR_PE_MASK 0x4
#define  UART_LSR_PE_SHIFT 2
#define  UART_LSR_PE_MASK_SHIFT(reg) (((reg) & UART_LSR_PE_MASK) >> UART_LSR_PE_SHIFT)
#define  UART_LSR_PE_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_PE_MASK) | (((uint32_t)val) << UART_LSR_PE_SHIFT))

#define  UART_LSR_OE_MASK 0x2
#define  UART_LSR_OE_SHIFT 1
#define  UART_LSR_OE_MASK_SHIFT(reg) (((reg) & UART_LSR_OE_MASK) >> UART_LSR_OE_SHIFT)
#define  UART_LSR_OE_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_OE_MASK) | (((uint32_t)val) << UART_LSR_OE_SHIFT))

#define  UART_LSR_DR_MASK 0x1
#define  UART_LSR_DR_SHIFT 0
#define  UART_LSR_DR_MASK_SHIFT(reg) (((reg) & UART_LSR_DR_MASK) >> UART_LSR_DR_SHIFT)
#define  UART_LSR_DR_REPLACE_VAL(reg,val) (((reg) & ~UART_LSR_DR_MASK) | (((uint32_t)val) << UART_LSR_DR_SHIFT))

//====================================================================
//Register: Modem Status Register (MSR)
//====================================================================

#define  UART_MSR_RESERVED1_MASK 0xffffff00
#define  UART_MSR_RESERVED1_SHIFT 8
#define  UART_MSR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_MSR_RESERVED1_MASK) >> UART_MSR_RESERVED1_SHIFT)
#define  UART_MSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_RESERVED1_MASK) | (((uint32_t)val) << UART_MSR_RESERVED1_SHIFT))

#define  UART_MSR_DCD_MASK 0x80
#define  UART_MSR_DCD_SHIFT 7
#define  UART_MSR_DCD_MASK_SHIFT(reg) (((reg) & UART_MSR_DCD_MASK) >> UART_MSR_DCD_SHIFT)
#define  UART_MSR_DCD_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_DCD_MASK) | (((uint32_t)val) << UART_MSR_DCD_SHIFT))

#define  UART_MSR_RI_MASK 0x40
#define  UART_MSR_RI_SHIFT 6
#define  UART_MSR_RI_MASK_SHIFT(reg) (((reg) & UART_MSR_RI_MASK) >> UART_MSR_RI_SHIFT)
#define  UART_MSR_RI_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_RI_MASK) | (((uint32_t)val) << UART_MSR_RI_SHIFT))

#define  UART_MSR_DSR_MASK 0x20
#define  UART_MSR_DSR_SHIFT 5
#define  UART_MSR_DSR_MASK_SHIFT(reg) (((reg) & UART_MSR_DSR_MASK) >> UART_MSR_DSR_SHIFT)
#define  UART_MSR_DSR_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_DSR_MASK) | (((uint32_t)val) << UART_MSR_DSR_SHIFT))

#define  UART_MSR_CTS_MASK 0x10
#define  UART_MSR_CTS_SHIFT 4
#define  UART_MSR_CTS_MASK_SHIFT(reg) (((reg) & UART_MSR_CTS_MASK) >> UART_MSR_CTS_SHIFT)
#define  UART_MSR_CTS_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_CTS_MASK) | (((uint32_t)val) << UART_MSR_CTS_SHIFT))

#define  UART_MSR_DDCD_MASK 0x8
#define  UART_MSR_DDCD_SHIFT 3
#define  UART_MSR_DDCD_MASK_SHIFT(reg) (((reg) & UART_MSR_DDCD_MASK) >> UART_MSR_DDCD_SHIFT)
#define  UART_MSR_DDCD_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_DDCD_MASK) | (((uint32_t)val) << UART_MSR_DDCD_SHIFT))

#define  UART_MSR_TERI_MASK 0x4
#define  UART_MSR_TERI_SHIFT 2
#define  UART_MSR_TERI_MASK_SHIFT(reg) (((reg) & UART_MSR_TERI_MASK) >> UART_MSR_TERI_SHIFT)
#define  UART_MSR_TERI_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_TERI_MASK) | (((uint32_t)val) << UART_MSR_TERI_SHIFT))

#define  UART_MSR_DDSR_MASK 0x2
#define  UART_MSR_DDSR_SHIFT 1
#define  UART_MSR_DDSR_MASK_SHIFT(reg) (((reg) & UART_MSR_DDSR_MASK) >> UART_MSR_DDSR_SHIFT)
#define  UART_MSR_DDSR_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_DDSR_MASK) | (((uint32_t)val) << UART_MSR_DDSR_SHIFT))

#define  UART_MSR_DCTS_MASK 0x1
#define  UART_MSR_DCTS_SHIFT 0
#define  UART_MSR_DCTS_MASK_SHIFT(reg) (((reg) & UART_MSR_DCTS_MASK) >> UART_MSR_DCTS_SHIFT)
#define  UART_MSR_DCTS_REPLACE_VAL(reg,val) (((reg) & ~UART_MSR_DCTS_MASK) | (((uint32_t)val) << UART_MSR_DCTS_SHIFT))

//====================================================================
//Register: FIFO Access Register (FAR)
//====================================================================

#define  UART_FAR_RESERVED1_MASK 0xfffffffe
#define  UART_FAR_RESERVED1_SHIFT 1
#define  UART_FAR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_FAR_RESERVED1_MASK) >> UART_FAR_RESERVED1_SHIFT)
#define  UART_FAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_FAR_RESERVED1_MASK) | (((uint32_t)val) << UART_FAR_RESERVED1_SHIFT))

#define  UART_FAR_FIFO_AR_MASK 0x1
#define  UART_FAR_FIFO_AR_SHIFT 0
#define  UART_FAR_FIFO_AR_MASK_SHIFT(reg) (((reg) & UART_FAR_FIFO_AR_MASK) >> UART_FAR_FIFO_AR_SHIFT)
#define  UART_FAR_FIFO_AR_REPLACE_VAL(reg,val) (((reg) & ~UART_FAR_FIFO_AR_MASK) | (((uint32_t)val) << UART_FAR_FIFO_AR_SHIFT))

//====================================================================
//Register: UART Status Register (USR)
//====================================================================

#define  UART_USR_RESERVED1_MASK 0xffffffe0
#define  UART_USR_RESERVED1_SHIFT 5
#define  UART_USR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_USR_RESERVED1_MASK) >> UART_USR_RESERVED1_SHIFT)
#define  UART_USR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_USR_RESERVED1_MASK) | (((uint32_t)val) << UART_USR_RESERVED1_SHIFT))

#define  UART_USR_RFE_MASK 0x10
#define  UART_USR_RFE_SHIFT 4
#define  UART_USR_RFE_MASK_SHIFT(reg) (((reg) & UART_USR_RFE_MASK) >> UART_USR_RFE_SHIFT)
#define  UART_USR_RFE_REPLACE_VAL(reg,val) (((reg) & ~UART_USR_RFE_MASK) | (((uint32_t)val) << UART_USR_RFE_SHIFT))

#define  UART_USR_RFNE_MASK 0x8
#define  UART_USR_RFNE_SHIFT 3
#define  UART_USR_RFNE_MASK_SHIFT(reg) (((reg) & UART_USR_RFNE_MASK) >> UART_USR_RFNE_SHIFT)
#define  UART_USR_RFNE_REPLACE_VAL(reg,val) (((reg) & ~UART_USR_RFNE_MASK) | (((uint32_t)val) << UART_USR_RFNE_SHIFT))

#define  UART_USR_TFE_MASK 0x4
#define  UART_USR_TFE_SHIFT 2
#define  UART_USR_TFE_MASK_SHIFT(reg) (((reg) & UART_USR_TFE_MASK) >> UART_USR_TFE_SHIFT)
#define  UART_USR_TFE_REPLACE_VAL(reg,val) (((reg) & ~UART_USR_TFE_MASK) | (((uint32_t)val) << UART_USR_TFE_SHIFT))

#define  UART_USR_TFNE_MASK 0x2
#define  UART_USR_TFNE_SHIFT 1
#define  UART_USR_TFNE_MASK_SHIFT(reg) (((reg) & UART_USR_TFNE_MASK) >> UART_USR_TFNE_SHIFT)
#define  UART_USR_TFNE_REPLACE_VAL(reg,val) (((reg) & ~UART_USR_TFNE_MASK) | (((uint32_t)val) << UART_USR_TFNE_SHIFT))

#define  UART_USR_BUSY_MASK 0x1
#define  UART_USR_BUSY_SHIFT 0
#define  UART_USR_BUSY_MASK_SHIFT(reg) (((reg) & UART_USR_BUSY_MASK) >> UART_USR_BUSY_SHIFT)
#define  UART_USR_BUSY_REPLACE_VAL(reg,val) (((reg) & ~UART_USR_BUSY_MASK) | (((uint32_t)val) << UART_USR_BUSY_SHIFT))

//====================================================================
//Register: Scratchpad Register (SCR)
//====================================================================

#define  UART_SCR_RESERVED1_MASK 0xffffff00
#define  UART_SCR_RESERVED1_SHIFT 8
#define  UART_SCR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SCR_RESERVED1_MASK) >> UART_SCR_RESERVED1_SHIFT)
#define  UART_SCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SCR_RESERVED1_MASK) | (((uint32_t)val) << UART_SCR_RESERVED1_SHIFT))

#define  UART_SCR_SCR_REG_MASK 0xff
#define  UART_SCR_SCR_REG_SHIFT 0
#define  UART_SCR_SCR_REG_MASK_SHIFT(reg) (((reg) & UART_SCR_SCR_REG_MASK) >> UART_SCR_SCR_REG_SHIFT)
#define  UART_SCR_SCR_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_SCR_SCR_REG_MASK) | (((uint32_t)val) << UART_SCR_SCR_REG_SHIFT))

//====================================================================
//Register: Low Power Divisor Latch Low Register (LPDLL)
//====================================================================

#define  UART_LPDLL_RESERVED1_MASK 0xffffff00
#define  UART_LPDLL_RESERVED1_SHIFT 8
#define  UART_LPDLL_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_LPDLL_RESERVED1_MASK) >> UART_LPDLL_RESERVED1_SHIFT)
#define  UART_LPDLL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_LPDLL_RESERVED1_MASK) | (((uint32_t)val) << UART_LPDLL_RESERVED1_SHIFT))

#define  UART_LPDLL_LPDLL_REG_MASK 0xff
#define  UART_LPDLL_LPDLL_REG_SHIFT 0
#define  UART_LPDLL_LPDLL_REG_MASK_SHIFT(reg) (((reg) & UART_LPDLL_LPDLL_REG_MASK) >> UART_LPDLL_LPDLL_REG_SHIFT)
#define  UART_LPDLL_LPDLL_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_LPDLL_LPDLL_REG_MASK) | (((uint32_t)val) << UART_LPDLL_LPDLL_REG_SHIFT))

//====================================================================
//Register: Low Power Divisor Latch High Register (LPDLH)
//====================================================================

#define  UART_LPDLH_RESERVED1_MASK 0xffffff00
#define  UART_LPDLH_RESERVED1_SHIFT 8
#define  UART_LPDLH_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_LPDLH_RESERVED1_MASK) >> UART_LPDLH_RESERVED1_SHIFT)
#define  UART_LPDLH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_LPDLH_RESERVED1_MASK) | (((uint32_t)val) << UART_LPDLH_RESERVED1_SHIFT))

#define  UART_LPDLH_LPDLH_REG_MASK 0xff
#define  UART_LPDLH_LPDLH_REG_SHIFT 0
#define  UART_LPDLH_LPDLH_REG_MASK_SHIFT(reg) (((reg) & UART_LPDLH_LPDLH_REG_MASK) >> UART_LPDLH_LPDLH_REG_SHIFT)
#define  UART_LPDLH_LPDLH_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_LPDLH_LPDLH_REG_MASK) | (((uint32_t)val) << UART_LPDLH_LPDLH_REG_SHIFT))

//====================================================================
//Register: Shadow Receive Buffer Low Register (SRBRL)
//====================================================================

#define  UART_SRBRL_RESERVED1_MASK 0xffffff00
#define  UART_SRBRL_RESERVED1_SHIFT 8
#define  UART_SRBRL_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SRBRL_RESERVED1_MASK) >> UART_SRBRL_RESERVED1_SHIFT)
#define  UART_SRBRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SRBRL_RESERVED1_MASK) | (((uint32_t)val) << UART_SRBRL_RESERVED1_SHIFT))

#define  UART_SRBRL_SHADOWRBRL_REG_MASK 0xff
#define  UART_SRBRL_SHADOWRBRL_REG_SHIFT 0
#define  UART_SRBRL_SHADOWRBRL_REG_MASK_SHIFT(reg) (((reg) & UART_SRBRL_SHADOWRBRL_REG_MASK) >> UART_SRBRL_SHADOWRBRL_REG_SHIFT)
#define  UART_SRBRL_SHADOWRBRL_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_SRBRL_SHADOWRBRL_REG_MASK) | (((uint32_t)val) << UART_SRBRL_SHADOWRBRL_REG_SHIFT))

//====================================================================
//Register: Shadow Transmit Holding Low Register (STHRL)
//====================================================================

#define  UART_STHRL_RESERVED1_MASK 0xffffff00
#define  UART_STHRL_RESERVED1_SHIFT 8
#define  UART_STHRL_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_STHRL_RESERVED1_MASK) >> UART_STHRL_RESERVED1_SHIFT)
#define  UART_STHRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_STHRL_RESERVED1_MASK) | (((uint32_t)val) << UART_STHRL_RESERVED1_SHIFT))

#define  UART_STHRL_SHADOWTHRL_REG_MASK 0xff
#define  UART_STHRL_SHADOWTHRL_REG_SHIFT 0
#define  UART_STHRL_SHADOWTHRL_REG_MASK_SHIFT(reg) (((reg) & UART_STHRL_SHADOWTHRL_REG_MASK) >> UART_STHRL_SHADOWTHRL_REG_SHIFT)
#define  UART_STHRL_SHADOWTHRL_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_STHRL_SHADOWTHRL_REG_MASK) | (((uint32_t)val) << UART_STHRL_SHADOWTHRL_REG_SHIFT))

//====================================================================
//Register: Transmit FIFO Read Register (TFR)
//====================================================================

#define  UART_TFR_RESERVED1_MASK 0xffffff00
#define  UART_TFR_RESERVED1_SHIFT 8
#define  UART_TFR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_TFR_RESERVED1_MASK) >> UART_TFR_RESERVED1_SHIFT)
#define  UART_TFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_TFR_RESERVED1_MASK) | (((uint32_t)val) << UART_TFR_RESERVED1_SHIFT))

#define  UART_TFR_TFR_REG_MASK 0xff
#define  UART_TFR_TFR_REG_SHIFT 0
#define  UART_TFR_TFR_REG_MASK_SHIFT(reg) (((reg) & UART_TFR_TFR_REG_MASK) >> UART_TFR_TFR_REG_SHIFT)
#define  UART_TFR_TFR_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_TFR_TFR_REG_MASK) | (((uint32_t)val) << UART_TFR_TFR_REG_SHIFT))

//====================================================================
//Register: Receive FIFO Write Register (RFW)
//====================================================================

#define  UART_RFW_RESERVED1_MASK 0xfffffc00
#define  UART_RFW_RESERVED1_SHIFT 10
#define  UART_RFW_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_RFW_RESERVED1_MASK) >> UART_RFW_RESERVED1_SHIFT)
#define  UART_RFW_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_RFW_RESERVED1_MASK) | (((uint32_t)val) << UART_RFW_RESERVED1_SHIFT))

#define  UART_RFW_RFFE_REG_MASK 0x200
#define  UART_RFW_RFFE_REG_SHIFT 9
#define  UART_RFW_RFFE_REG_MASK_SHIFT(reg) (((reg) & UART_RFW_RFFE_REG_MASK) >> UART_RFW_RFFE_REG_SHIFT)
#define  UART_RFW_RFFE_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_RFW_RFFE_REG_MASK) | (((uint32_t)val) << UART_RFW_RFFE_REG_SHIFT))

#define  UART_RFW_RFPE_REG_MASK 0x100
#define  UART_RFW_RFPE_REG_SHIFT 8
#define  UART_RFW_RFPE_REG_MASK_SHIFT(reg) (((reg) & UART_RFW_RFPE_REG_MASK) >> UART_RFW_RFPE_REG_SHIFT)
#define  UART_RFW_RFPE_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_RFW_RFPE_REG_MASK) | (((uint32_t)val) << UART_RFW_RFPE_REG_SHIFT))

#define  UART_RFW_RFWD_REG_MASK 0xff
#define  UART_RFW_RFWD_REG_SHIFT 0
#define  UART_RFW_RFWD_REG_MASK_SHIFT(reg) (((reg) & UART_RFW_RFWD_REG_MASK) >> UART_RFW_RFWD_REG_SHIFT)
#define  UART_RFW_RFWD_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_RFW_RFWD_REG_MASK) | (((uint32_t)val) << UART_RFW_RFWD_REG_SHIFT))

//====================================================================
//Register: Transmit FIFO Level (TFL)
//====================================================================

#define  UART_TFL_RESERVED1_MASK 0xffffffe0
#define  UART_TFL_RESERVED1_SHIFT 5
#define  UART_TFL_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_TFL_RESERVED1_MASK) >> UART_TFL_RESERVED1_SHIFT)
#define  UART_TFL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_TFL_RESERVED1_MASK) | (((uint32_t)val) << UART_TFL_RESERVED1_SHIFT))

#define  UART_TFL_TX_FIFO_LEVEL_MASK 0x1f
#define  UART_TFL_TX_FIFO_LEVEL_SHIFT 0
#define  UART_TFL_TX_FIFO_LEVEL_MASK_SHIFT(reg) (((reg) & UART_TFL_TX_FIFO_LEVEL_MASK) >> UART_TFL_TX_FIFO_LEVEL_SHIFT)
#define  UART_TFL_TX_FIFO_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~UART_TFL_TX_FIFO_LEVEL_MASK) | (((uint32_t)val) << UART_TFL_TX_FIFO_LEVEL_SHIFT))

//====================================================================
//Register: Receive FIFO Level (RFL)
//====================================================================

#define  UART_RFL_RESERVED1_MASK 0xffffffe0
#define  UART_RFL_RESERVED1_SHIFT 5
#define  UART_RFL_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_RFL_RESERVED1_MASK) >> UART_RFL_RESERVED1_SHIFT)
#define  UART_RFL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_RFL_RESERVED1_MASK) | (((uint32_t)val) << UART_RFL_RESERVED1_SHIFT))

#define  UART_RFL_RX_FIFO_LEVEL_MASK 0x1f
#define  UART_RFL_RX_FIFO_LEVEL_SHIFT 0
#define  UART_RFL_RX_FIFO_LEVEL_MASK_SHIFT(reg) (((reg) & UART_RFL_RX_FIFO_LEVEL_MASK) >> UART_RFL_RX_FIFO_LEVEL_SHIFT)
#define  UART_RFL_RX_FIFO_LEVEL_REPLACE_VAL(reg,val) (((reg) & ~UART_RFL_RX_FIFO_LEVEL_MASK) | (((uint32_t)val) << UART_RFL_RX_FIFO_LEVEL_SHIFT))

//====================================================================
//Register: Software Reset Register (SRR)
//====================================================================

#define  UART_SRR_RESERVED1_MASK 0xfffffff8
#define  UART_SRR_RESERVED1_SHIFT 3
#define  UART_SRR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SRR_RESERVED1_MASK) >> UART_SRR_RESERVED1_SHIFT)
#define  UART_SRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SRR_RESERVED1_MASK) | (((uint32_t)val) << UART_SRR_RESERVED1_SHIFT))

#define  UART_SRR_XFR_MASK 0x4
#define  UART_SRR_XFR_SHIFT 2
#define  UART_SRR_XFR_MASK_SHIFT(reg) (((reg) & UART_SRR_XFR_MASK) >> UART_SRR_XFR_SHIFT)
#define  UART_SRR_XFR_REPLACE_VAL(reg,val) (((reg) & ~UART_SRR_XFR_MASK) | (((uint32_t)val) << UART_SRR_XFR_SHIFT))

#define  UART_SRR_RFR_MASK 0x2
#define  UART_SRR_RFR_SHIFT 1
#define  UART_SRR_RFR_MASK_SHIFT(reg) (((reg) & UART_SRR_RFR_MASK) >> UART_SRR_RFR_SHIFT)
#define  UART_SRR_RFR_REPLACE_VAL(reg,val) (((reg) & ~UART_SRR_RFR_MASK) | (((uint32_t)val) << UART_SRR_RFR_SHIFT))

#define  UART_SRR_UR_MASK 0x1
#define  UART_SRR_UR_SHIFT 0
#define  UART_SRR_UR_MASK_SHIFT(reg) (((reg) & UART_SRR_UR_MASK) >> UART_SRR_UR_SHIFT)
#define  UART_SRR_UR_REPLACE_VAL(reg,val) (((reg) & ~UART_SRR_UR_MASK) | (((uint32_t)val) << UART_SRR_UR_SHIFT))

//====================================================================
//Register: Shadow Request to Send (SRTS)
//====================================================================

#define  UART_SRTS_RESERVED1_MASK 0xfffffffe
#define  UART_SRTS_RESERVED1_SHIFT 1
#define  UART_SRTS_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SRTS_RESERVED1_MASK) >> UART_SRTS_RESERVED1_SHIFT)
#define  UART_SRTS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SRTS_RESERVED1_MASK) | (((uint32_t)val) << UART_SRTS_RESERVED1_SHIFT))

#define  UART_SRTS_SHADOW_REQ_TO_SEND_MASK 0x1
#define  UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT 0
#define  UART_SRTS_SHADOW_REQ_TO_SEND_MASK_SHIFT(reg) (((reg) & UART_SRTS_SHADOW_REQ_TO_SEND_MASK) >> UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT)
#define  UART_SRTS_SHADOW_REQ_TO_SEND_REPLACE_VAL(reg,val) (((reg) & ~UART_SRTS_SHADOW_REQ_TO_SEND_MASK) | (((uint32_t)val) << UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT))

//====================================================================
//Register: Shadow Break Control Register (SBCR)
//====================================================================

#define  UART_SBCR_RESERVED1_MASK 0xfffffffe
#define  UART_SBCR_RESERVED1_SHIFT 1
#define  UART_SBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SBCR_RESERVED1_MASK) >> UART_SBCR_RESERVED1_SHIFT)
#define  UART_SBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SBCR_RESERVED1_MASK) | (((uint32_t)val) << UART_SBCR_RESERVED1_SHIFT))

#define  UART_SBCR_SHADOW_BREAK_CTRL_MASK 0x1
#define  UART_SBCR_SHADOW_BREAK_CTRL_SHIFT 0
#define  UART_SBCR_SHADOW_BREAK_CTRL_MASK_SHIFT(reg) (((reg) & UART_SBCR_SHADOW_BREAK_CTRL_MASK) >> UART_SBCR_SHADOW_BREAK_CTRL_SHIFT)
#define  UART_SBCR_SHADOW_BREAK_CTRL_REPLACE_VAL(reg,val) (((reg) & ~UART_SBCR_SHADOW_BREAK_CTRL_MASK) | (((uint32_t)val) << UART_SBCR_SHADOW_BREAK_CTRL_SHIFT))

//====================================================================
//Register: Shadow DMA Mode Register (SDMAM)
//====================================================================

#define  UART_SDMAM_RESERVED1_MASK 0xfffffffe
#define  UART_SDMAM_RESERVED1_SHIFT 1
#define  UART_SDMAM_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SDMAM_RESERVED1_MASK) >> UART_SDMAM_RESERVED1_SHIFT)
#define  UART_SDMAM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SDMAM_RESERVED1_MASK) | (((uint32_t)val) << UART_SDMAM_RESERVED1_SHIFT))

#define  UART_SDMAM_SHADOW_DMA_MASK 0x1
#define  UART_SDMAM_SHADOW_DMA_SHIFT 0
#define  UART_SDMAM_SHADOW_DMA_MASK_SHIFT(reg) (((reg) & UART_SDMAM_SHADOW_DMA_MASK) >> UART_SDMAM_SHADOW_DMA_SHIFT)
#define  UART_SDMAM_SHADOW_DMA_REPLACE_VAL(reg,val) (((reg) & ~UART_SDMAM_SHADOW_DMA_MASK) | (((uint32_t)val) << UART_SDMAM_SHADOW_DMA_SHIFT))

//====================================================================
//Register: Shadow FIFO Enable Register (SFE)
//====================================================================

#define  UART_SFE_RESERVED1_MASK 0xfffffffe
#define  UART_SFE_RESERVED1_SHIFT 1
#define  UART_SFE_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SFE_RESERVED1_MASK) >> UART_SFE_RESERVED1_SHIFT)
#define  UART_SFE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SFE_RESERVED1_MASK) | (((uint32_t)val) << UART_SFE_RESERVED1_SHIFT))

#define  UART_SFE_SHADOW_FIFO_EN_MASK 0x1
#define  UART_SFE_SHADOW_FIFO_EN_SHIFT 0
#define  UART_SFE_SHADOW_FIFO_EN_MASK_SHIFT(reg) (((reg) & UART_SFE_SHADOW_FIFO_EN_MASK) >> UART_SFE_SHADOW_FIFO_EN_SHIFT)
#define  UART_SFE_SHADOW_FIFO_EN_REPLACE_VAL(reg,val) (((reg) & ~UART_SFE_SHADOW_FIFO_EN_MASK) | (((uint32_t)val) << UART_SFE_SHADOW_FIFO_EN_SHIFT))

//====================================================================
//Register: Shadow RCVR Trigger Register (SRT)
//====================================================================

#define  UART_SRT_RESERVED1_MASK 0xfffffffc
#define  UART_SRT_RESERVED1_SHIFT 2
#define  UART_SRT_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_SRT_RESERVED1_MASK) >> UART_SRT_RESERVED1_SHIFT)
#define  UART_SRT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_SRT_RESERVED1_MASK) | (((uint32_t)val) << UART_SRT_RESERVED1_SHIFT))

#define  UART_SRT_SHADOW_RCVR_TRIG_MASK 0x3
#define  UART_SRT_SHADOW_RCVR_TRIG_SHIFT 0
#define  UART_SRT_SHADOW_RCVR_TRIG_MASK_SHIFT(reg) (((reg) & UART_SRT_SHADOW_RCVR_TRIG_MASK) >> UART_SRT_SHADOW_RCVR_TRIG_SHIFT)
#define  UART_SRT_SHADOW_RCVR_TRIG_REPLACE_VAL(reg,val) (((reg) & ~UART_SRT_SHADOW_RCVR_TRIG_MASK) | (((uint32_t)val) << UART_SRT_SHADOW_RCVR_TRIG_SHIFT))

//====================================================================
//Register: Shadow TX Empty Trigger Register (STET)
//====================================================================

#define  UART_STET_RESERVED1_MASK 0xfffffffc
#define  UART_STET_RESERVED1_SHIFT 2
#define  UART_STET_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_STET_RESERVED1_MASK) >> UART_STET_RESERVED1_SHIFT)
#define  UART_STET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_STET_RESERVED1_MASK) | (((uint32_t)val) << UART_STET_RESERVED1_SHIFT))

#define  UART_STET_SHADOW_TX_EMPTY_TRIG_MASK 0x3
#define  UART_STET_SHADOW_TX_EMPTY_TRIG_SHIFT 0
#define  UART_STET_SHADOW_TX_EMPTY_TRIG_MASK_SHIFT(reg) (((reg) & UART_STET_SHADOW_TX_EMPTY_TRIG_MASK) >> UART_STET_SHADOW_TX_EMPTY_TRIG_SHIFT)
#define  UART_STET_SHADOW_TX_EMPTY_TRIG_REPLACE_VAL(reg,val) (((reg) & ~UART_STET_SHADOW_TX_EMPTY_TRIG_MASK) | (((uint32_t)val) << UART_STET_SHADOW_TX_EMPTY_TRIG_SHIFT))

//====================================================================
//Register: Halt TX Register (HTX)
//====================================================================

#define  UART_HTX_RESERVED1_MASK 0xfffffffe
#define  UART_HTX_RESERVED1_SHIFT 1
#define  UART_HTX_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_HTX_RESERVED1_MASK) >> UART_HTX_RESERVED1_SHIFT)
#define  UART_HTX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_HTX_RESERVED1_MASK) | (((uint32_t)val) << UART_HTX_RESERVED1_SHIFT))

#define  UART_HTX_HTX_MASK 0x1
#define  UART_HTX_HTX_SHIFT 0
#define  UART_HTX_HTX_MASK_SHIFT(reg) (((reg) & UART_HTX_HTX_MASK) >> UART_HTX_HTX_SHIFT)
#define  UART_HTX_HTX_REPLACE_VAL(reg,val) (((reg) & ~UART_HTX_HTX_MASK) | (((uint32_t)val) << UART_HTX_HTX_SHIFT))

//====================================================================
//Register: DMA Software Acknowledge Register (DMASA)
//====================================================================

#define  UART_DMASA_RESERVED1_MASK 0xfffffffe
#define  UART_DMASA_RESERVED1_SHIFT 1
#define  UART_DMASA_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_DMASA_RESERVED1_MASK) >> UART_DMASA_RESERVED1_SHIFT)
#define  UART_DMASA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_DMASA_RESERVED1_MASK) | (((uint32_t)val) << UART_DMASA_RESERVED1_SHIFT))

#define  UART_DMASA_DMA_SW_ACK_MASK 0x1
#define  UART_DMASA_DMA_SW_ACK_SHIFT 0
#define  UART_DMASA_DMA_SW_ACK_MASK_SHIFT(reg) (((reg) & UART_DMASA_DMA_SW_ACK_MASK) >> UART_DMASA_DMA_SW_ACK_SHIFT)
#define  UART_DMASA_DMA_SW_ACK_REPLACE_VAL(reg,val) (((reg) & ~UART_DMASA_DMA_SW_ACK_MASK) | (((uint32_t)val) << UART_DMASA_DMA_SW_ACK_SHIFT))

//====================================================================
//Register: Nine-Bit Receive Buffer Register (NBRBR)
/** \brief A read from this register does the same thing as a read from the
      RBR register (as far as popping the RX FIFO is concerned), however
      a read from this register returns two extra bits of data. Bit[7:0]
      is received UART serial data. Bit[8] is the parity error status from
      that transfer. Bit[9] is the framing error status from that transfer.*/
//====================================================================

#define  UART_NBRBR_DV_MASK 0x80000000
#define  UART_NBRBR_DV_SHIFT 31
#define  UART_NBRBR_DV_MASK_SHIFT(reg) (((reg) & UART_NBRBR_DV_MASK) >> UART_NBRBR_DV_SHIFT)
#define  UART_NBRBR_DV_REPLACE_VAL(reg,val) (((reg) & ~UART_NBRBR_DV_MASK) | (((uint32_t)val) << UART_NBRBR_DV_SHIFT))

#define  UART_NBRBR_RESERVED1_MASK 0x7ffffc00
#define  UART_NBRBR_RESERVED1_SHIFT 10
#define  UART_NBRBR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_NBRBR_RESERVED1_MASK) >> UART_NBRBR_RESERVED1_SHIFT)
#define  UART_NBRBR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_NBRBR_RESERVED1_MASK) | (((uint32_t)val) << UART_NBRBR_RESERVED1_SHIFT))

#define  UART_NBRBR_FE_MASK 0x200
#define  UART_NBRBR_FE_SHIFT 9
#define  UART_NBRBR_FE_MASK_SHIFT(reg) (((reg) & UART_NBRBR_FE_MASK) >> UART_NBRBR_FE_SHIFT)
#define  UART_NBRBR_FE_REPLACE_VAL(reg,val) (((reg) & ~UART_NBRBR_FE_MASK) | (((uint32_t)val) << UART_NBRBR_FE_SHIFT))

#define  UART_NBRBR_PE_MASK 0x100
#define  UART_NBRBR_PE_SHIFT 8
#define  UART_NBRBR_PE_MASK_SHIFT(reg) (((reg) & UART_NBRBR_PE_MASK) >> UART_NBRBR_PE_SHIFT)
#define  UART_NBRBR_PE_REPLACE_VAL(reg,val) (((reg) & ~UART_NBRBR_PE_MASK) | (((uint32_t)val) << UART_NBRBR_PE_SHIFT))

#define  UART_NBRBR_NBRBR_REG_MASK 0xff
#define  UART_NBRBR_NBRBR_REG_SHIFT 0
#define  UART_NBRBR_NBRBR_REG_MASK_SHIFT(reg) (((reg) & UART_NBRBR_NBRBR_REG_MASK) >> UART_NBRBR_NBRBR_REG_SHIFT)
#define  UART_NBRBR_NBRBR_REG_REPLACE_VAL(reg,val) (((reg) & ~UART_NBRBR_NBRBR_REG_MASK) | (((uint32_t)val) << UART_NBRBR_NBRBR_REG_SHIFT))

//====================================================================
//Register: Component Parameter Register (CPR)
//====================================================================

#define  UART_CPR_MOD_MASK 0x80000000
#define  UART_CPR_MOD_SHIFT 31
#define  UART_CPR_MOD_MASK_SHIFT(reg) (((reg) & UART_CPR_MOD_MASK) >> UART_CPR_MOD_SHIFT)
#define  UART_CPR_MOD_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_MOD_MASK) | (((uint32_t)val) << UART_CPR_MOD_SHIFT))

#define  UART_CPR_RESERVED1_MASK 0x7f000000
#define  UART_CPR_RESERVED1_SHIFT 24
#define  UART_CPR_RESERVED1_MASK_SHIFT(reg) (((reg) & UART_CPR_RESERVED1_MASK) >> UART_CPR_RESERVED1_SHIFT)
#define  UART_CPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_RESERVED1_MASK) | (((uint32_t)val) << UART_CPR_RESERVED1_SHIFT))

#define  UART_CPR_FIFO_MODE_MASK 0xff0000
#define  UART_CPR_FIFO_MODE_SHIFT 16
#define  UART_CPR_FIFO_MODE_MASK_SHIFT(reg) (((reg) & UART_CPR_FIFO_MODE_MASK) >> UART_CPR_FIFO_MODE_SHIFT)
#define  UART_CPR_FIFO_MODE_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_FIFO_MODE_MASK) | (((uint32_t)val) << UART_CPR_FIFO_MODE_SHIFT))

#define  UART_CPR_RESERVED2_MASK 0xc000
#define  UART_CPR_RESERVED2_SHIFT 14
#define  UART_CPR_RESERVED2_MASK_SHIFT(reg) (((reg) & UART_CPR_RESERVED2_MASK) >> UART_CPR_RESERVED2_SHIFT)
#define  UART_CPR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_RESERVED2_MASK) | (((uint32_t)val) << UART_CPR_RESERVED2_SHIFT))

#define  UART_CPR_DMA_EXTRA_MASK 0x2000
#define  UART_CPR_DMA_EXTRA_SHIFT 13
#define  UART_CPR_DMA_EXTRA_MASK_SHIFT(reg) (((reg) & UART_CPR_DMA_EXTRA_MASK) >> UART_CPR_DMA_EXTRA_SHIFT)
#define  UART_CPR_DMA_EXTRA_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_DMA_EXTRA_MASK) | (((uint32_t)val) << UART_CPR_DMA_EXTRA_SHIFT))

#define  UART_CPR_UART_ADD_ENCODED_PARAMS_MASK 0x1000
#define  UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT 12
#define  UART_CPR_UART_ADD_ENCODED_PARAMS_MASK_SHIFT(reg) (((reg) & UART_CPR_UART_ADD_ENCODED_PARAMS_MASK) >> UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT)
#define  UART_CPR_UART_ADD_ENCODED_PARAMS_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_UART_ADD_ENCODED_PARAMS_MASK) | (((uint32_t)val) << UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT))

#define  UART_CPR_SHADOW_MASK 0x800
#define  UART_CPR_SHADOW_SHIFT 11
#define  UART_CPR_SHADOW_MASK_SHIFT(reg) (((reg) & UART_CPR_SHADOW_MASK) >> UART_CPR_SHADOW_SHIFT)
#define  UART_CPR_SHADOW_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_SHADOW_MASK) | (((uint32_t)val) << UART_CPR_SHADOW_SHIFT))

#define  UART_CPR_FIFO_STAT_MASK 0x400
#define  UART_CPR_FIFO_STAT_SHIFT 10
#define  UART_CPR_FIFO_STAT_MASK_SHIFT(reg) (((reg) & UART_CPR_FIFO_STAT_MASK) >> UART_CPR_FIFO_STAT_SHIFT)
#define  UART_CPR_FIFO_STAT_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_FIFO_STAT_MASK) | (((uint32_t)val) << UART_CPR_FIFO_STAT_SHIFT))

#define  UART_CPR_FIFO_ACCESS_MASK 0x200
#define  UART_CPR_FIFO_ACCESS_SHIFT 9
#define  UART_CPR_FIFO_ACCESS_MASK_SHIFT(reg) (((reg) & UART_CPR_FIFO_ACCESS_MASK) >> UART_CPR_FIFO_ACCESS_SHIFT)
#define  UART_CPR_FIFO_ACCESS_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_FIFO_ACCESS_MASK) | (((uint32_t)val) << UART_CPR_FIFO_ACCESS_SHIFT))

#define  UART_CPR_ADD_FEAT_MASK 0x100
#define  UART_CPR_ADD_FEAT_SHIFT 8
#define  UART_CPR_ADD_FEAT_MASK_SHIFT(reg) (((reg) & UART_CPR_ADD_FEAT_MASK) >> UART_CPR_ADD_FEAT_SHIFT)
#define  UART_CPR_ADD_FEAT_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_ADD_FEAT_MASK) | (((uint32_t)val) << UART_CPR_ADD_FEAT_SHIFT))

#define  UART_CPR_SIR_LP_MODE_MASK 0x80
#define  UART_CPR_SIR_LP_MODE_SHIFT 7
#define  UART_CPR_SIR_LP_MODE_MASK_SHIFT(reg) (((reg) & UART_CPR_SIR_LP_MODE_MASK) >> UART_CPR_SIR_LP_MODE_SHIFT)
#define  UART_CPR_SIR_LP_MODE_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_SIR_LP_MODE_MASK) | (((uint32_t)val) << UART_CPR_SIR_LP_MODE_SHIFT))

#define  UART_CPR_SIR_MODE_MASK 0x40
#define  UART_CPR_SIR_MODE_SHIFT 6
#define  UART_CPR_SIR_MODE_MASK_SHIFT(reg) (((reg) & UART_CPR_SIR_MODE_MASK) >> UART_CPR_SIR_MODE_SHIFT)
#define  UART_CPR_SIR_MODE_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_SIR_MODE_MASK) | (((uint32_t)val) << UART_CPR_SIR_MODE_SHIFT))

#define  UART_CPR_THRE_MODE_MASK 0x20
#define  UART_CPR_THRE_MODE_SHIFT 5
#define  UART_CPR_THRE_MODE_MASK_SHIFT(reg) (((reg) & UART_CPR_THRE_MODE_MASK) >> UART_CPR_THRE_MODE_SHIFT)
#define  UART_CPR_THRE_MODE_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_THRE_MODE_MASK) | (((uint32_t)val) << UART_CPR_THRE_MODE_SHIFT))

#define  UART_CPR_AFCE_MODE_MASK 0x10
#define  UART_CPR_AFCE_MODE_SHIFT 4
#define  UART_CPR_AFCE_MODE_MASK_SHIFT(reg) (((reg) & UART_CPR_AFCE_MODE_MASK) >> UART_CPR_AFCE_MODE_SHIFT)
#define  UART_CPR_AFCE_MODE_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_AFCE_MODE_MASK) | (((uint32_t)val) << UART_CPR_AFCE_MODE_SHIFT))

#define  UART_CPR_RESERVED3_MASK 0xc
#define  UART_CPR_RESERVED3_SHIFT 2
#define  UART_CPR_RESERVED3_MASK_SHIFT(reg) (((reg) & UART_CPR_RESERVED3_MASK) >> UART_CPR_RESERVED3_SHIFT)
#define  UART_CPR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_RESERVED3_MASK) | (((uint32_t)val) << UART_CPR_RESERVED3_SHIFT))

#define  UART_CPR_APB_DATA_WIDTH_MASK 0x3
#define  UART_CPR_APB_DATA_WIDTH_SHIFT 0
#define  UART_CPR_APB_DATA_WIDTH_MASK_SHIFT(reg) (((reg) & UART_CPR_APB_DATA_WIDTH_MASK) >> UART_CPR_APB_DATA_WIDTH_SHIFT)
#define  UART_CPR_APB_DATA_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~UART_CPR_APB_DATA_WIDTH_MASK) | (((uint32_t)val) << UART_CPR_APB_DATA_WIDTH_SHIFT))

//====================================================================
//Register: UART Component Version Register (UCV)
//====================================================================

#define  UART_UCV_UART_COMPNT_VER_MASK 0xffffffff
#define  UART_UCV_UART_COMPNT_VER_SHIFT 0
#define  UART_UCV_UART_COMPNT_VER_MASK_SHIFT(reg) (((reg) & UART_UCV_UART_COMPNT_VER_MASK) >> UART_UCV_UART_COMPNT_VER_SHIFT)
#define  UART_UCV_UART_COMPNT_VER_REPLACE_VAL(reg,val) (((reg) & ~UART_UCV_UART_COMPNT_VER_MASK) | (((uint32_t)val) << UART_UCV_UART_COMPNT_VER_SHIFT))

//====================================================================
//Register: Component Type Register (CTR)
//====================================================================

#define  UART_CTR_PID_MASK 0xffffffff
#define  UART_CTR_PID_SHIFT 0
#define  UART_CTR_PID_MASK_SHIFT(reg) (((reg) & UART_CTR_PID_MASK) >> UART_CTR_PID_SHIFT)
#define  UART_CTR_PID_REPLACE_VAL(reg,val) (((reg) & ~UART_CTR_PID_MASK) | (((uint32_t)val) << UART_CTR_PID_SHIFT))

#endif // UART
