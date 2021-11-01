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
 * \file GPIO_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _GPIO_REGMASKS_H_
#define _GPIO_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: General Purpose I/O (GPIO)
/** \brief The GPIO block registers provide the means to configure the characteristics of each GPIO pin,
    as well as the means for firmware to control/observe these pins.*/
//
//====================================================================

//====================================================================
//Register: Input Data Register (InData)
/** \brief The current state of the GPIO pins in this bank (deglitched, for those pins which have
      deglitching enabled), can be read from this register. The state can be read regardless
      of whether or not the pin is configured in GPIO mode (FuncSel field of the PinCfg register),
      or if it has its output enabled or not.*/
//====================================================================

#define  GPIO_INDATA_DATA_MASK 0xffffffff
#define  GPIO_INDATA_DATA_SHIFT 0
#define  GPIO_INDATA_DATA_MASK_SHIFT(reg) (((reg) & GPIO_INDATA_DATA_MASK) >> GPIO_INDATA_DATA_SHIFT)
#define  GPIO_INDATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~GPIO_INDATA_DATA_MASK) | (((uint32_t)val) << GPIO_INDATA_DATA_SHIFT))

//====================================================================
//Register: Output Data Register (OutData)
/** \brief <p>The values in this register are driven out onto the GPIO pins whenever the
      corresponding bit in the OE register is set. For bits that are not in semaphore mode
      (as set in the SemMode field of the PinCfg register), the value of the bit can be
      changed by writing directly to this register. Bits that are in semaphore mode can
      only be changed by writes to the SemDataSet and SemDataClear registers.</p>
      <p>Regardless of how the bits are set, the current value of the bit can
      always be read from this register. Note that the value of bits in this register will not
      necessarily match the value on the corresponding bits in the InData register if the
      corresponding OE bits are not set.</p>*/
//====================================================================

#define  GPIO_OUTDATA_DATA_MASK 0xffffffff
#define  GPIO_OUTDATA_DATA_SHIFT 0
#define  GPIO_OUTDATA_DATA_MASK_SHIFT(reg) (((reg) & GPIO_OUTDATA_DATA_MASK) >> GPIO_OUTDATA_DATA_SHIFT)
#define  GPIO_OUTDATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~GPIO_OUTDATA_DATA_MASK) | (((uint32_t)val) << GPIO_OUTDATA_DATA_SHIFT))

//====================================================================
//Register: GPIO Output Enable Register (OE)
/** \brief The bits in this register are the output enables for the corresponding GPIO pin when
      the pin is configured in GPIO function mode (FuncSel field of PinCfg register is 3'b000)*/
//====================================================================

#define  GPIO_OE_ENABLE_MASK 0xffffffff
#define  GPIO_OE_ENABLE_SHIFT 0
#define  GPIO_OE_ENABLE_MASK_SHIFT(reg) (((reg) & GPIO_OE_ENABLE_MASK) >> GPIO_OE_ENABLE_SHIFT)
#define  GPIO_OE_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_OE_ENABLE_MASK) | (((uint32_t)val) << GPIO_OE_ENABLE_SHIFT))

//====================================================================
//Register: GPIO Raw DataIn Register (RawData)
/** \brief Raw (but synchronized) input data for the corresponding GPIO pin*/
//====================================================================

#define  GPIO_RAWDATA_DATA_MASK 0xffffffff
#define  GPIO_RAWDATA_DATA_SHIFT 0
#define  GPIO_RAWDATA_DATA_MASK_SHIFT(reg) (((reg) & GPIO_RAWDATA_DATA_MASK) >> GPIO_RAWDATA_DATA_SHIFT)
#define  GPIO_RAWDATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~GPIO_RAWDATA_DATA_MASK) | (((uint32_t)val) << GPIO_RAWDATA_DATA_SHIFT))

//====================================================================
//Register: GPIO Interrupt Enable Register (IntEn)
/** \brief Setting a bit in this register to '1' enables an interrupt from the corresponding pin
      to be propagated to the interrupt port of the GPIO block. Writes to this register for
      bits that are in semaphore mode are ignored. Bits in semaphore mode must be set/cleared
      via the semaphore mode registers. Reading this register will show the current interrupt
      enable state of each bit regardless of whether it is controlled via normal mode or
      semaphore mode.*/
//====================================================================

#define  GPIO_INTEN_ENABLE_MASK 0xffffffff
#define  GPIO_INTEN_ENABLE_SHIFT 0
#define  GPIO_INTEN_ENABLE_MASK_SHIFT(reg) (((reg) & GPIO_INTEN_ENABLE_MASK) >> GPIO_INTEN_ENABLE_SHIFT)
#define  GPIO_INTEN_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_INTEN_ENABLE_MASK) | (((uint32_t)val) << GPIO_INTEN_ENABLE_SHIFT))

//====================================================================
//Register: GPIO Interrupt Acknowledge Register (IntAck)
/** \brief This register is written to acknowledge and clear an interrupt.*/
//====================================================================

#define  GPIO_INTACK_ACK_MASK 0xffffffff
#define  GPIO_INTACK_ACK_SHIFT 0
#define  GPIO_INTACK_ACK_MASK_SHIFT(reg) (((reg) & GPIO_INTACK_ACK_MASK) >> GPIO_INTACK_ACK_SHIFT)
#define  GPIO_INTACK_ACK_REPLACE_VAL(reg,val) (((reg) & ~GPIO_INTACK_ACK_MASK) | (((uint32_t)val) << GPIO_INTACK_ACK_SHIFT))

//====================================================================
//Register: GPIO Interrupt Status Register (IntStat)
/** \brief This register provides the raw interrupt status, prior to masking by the IntEn enable bits.*/
//====================================================================

#define  GPIO_INTSTAT_STATUS_MASK 0xffffffff
#define  GPIO_INTSTAT_STATUS_SHIFT 0
#define  GPIO_INTSTAT_STATUS_MASK_SHIFT(reg) (((reg) & GPIO_INTSTAT_STATUS_MASK) >> GPIO_INTSTAT_STATUS_SHIFT)
#define  GPIO_INTSTAT_STATUS_REPLACE_VAL(reg,val) (((reg) & ~GPIO_INTSTAT_STATUS_MASK) | (((uint32_t)val) << GPIO_INTSTAT_STATUS_SHIFT))

//====================================================================
//Register: GPIO Interrupt Test Register (IntTest)
/** \brief This register allows firmware to simulate an interrupt on any of the GPIO bits.*/
//====================================================================

#define  GPIO_INTTEST_FORCE_MASK 0xffffffff
#define  GPIO_INTTEST_FORCE_SHIFT 0
#define  GPIO_INTTEST_FORCE_MASK_SHIFT(reg) (((reg) & GPIO_INTTEST_FORCE_MASK) >> GPIO_INTTEST_FORCE_SHIFT)
#define  GPIO_INTTEST_FORCE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_INTTEST_FORCE_MASK) | (((uint32_t)val) << GPIO_INTTEST_FORCE_SHIFT))

//====================================================================
//Register: GPIO Semaphore Data Set Register (SemDataSet)
/** \brief Writing any bit in this register to a '1' will set the corresponding Data bit to '1' if
      that bit is in semaphore mode. Data bits corresponding to bits that are written as '0'
      in this register, or data bits that are not in semaphore mode will not be affected by the
      write to this register.*/
//====================================================================

#define  GPIO_SEMDATASET_SET_MASK 0xffffffff
#define  GPIO_SEMDATASET_SET_SHIFT 0
#define  GPIO_SEMDATASET_SET_MASK_SHIFT(reg) (((reg) & GPIO_SEMDATASET_SET_MASK) >> GPIO_SEMDATASET_SET_SHIFT)
#define  GPIO_SEMDATASET_SET_REPLACE_VAL(reg,val) (((reg) & ~GPIO_SEMDATASET_SET_MASK) | (((uint32_t)val) << GPIO_SEMDATASET_SET_SHIFT))

//====================================================================
//Register: GPIO Semaphore Data Clear Register (SemDataClear)
/** \brief Writing any bit in this register to a '1' will clear the corresponding data bit to '0'
      if that bit is in semaphore mode. Data bits corresponding to bits that are written as
      '0' in this register, or data bits that are not in semaphore mode will not be affected
      by the write to this register.*/
//====================================================================

#define  GPIO_SEMDATACLEAR_CLEAR_MASK 0xffffffff
#define  GPIO_SEMDATACLEAR_CLEAR_SHIFT 0
#define  GPIO_SEMDATACLEAR_CLEAR_MASK_SHIFT(reg) (((reg) & GPIO_SEMDATACLEAR_CLEAR_MASK) >> GPIO_SEMDATACLEAR_CLEAR_SHIFT)
#define  GPIO_SEMDATACLEAR_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~GPIO_SEMDATACLEAR_CLEAR_MASK) | (((uint32_t)val) << GPIO_SEMDATACLEAR_CLEAR_SHIFT))

//====================================================================
//Register: GPIO Semaphore Output Enable Set Register (SemOESet)
/** \brief Writing any bit in this register to a '1' will set the corresponding output enable bit
      to '1' if that bit is in semaphore mode. Output enable bits corresponding to bits that
      are written as '0' in this register, or output enable bits that are not in semaphore mode
      will not be affected by the write to this register.*/
//====================================================================

#define  GPIO_SEMOESET_SET_MASK 0xffffffff
#define  GPIO_SEMOESET_SET_SHIFT 0
#define  GPIO_SEMOESET_SET_MASK_SHIFT(reg) (((reg) & GPIO_SEMOESET_SET_MASK) >> GPIO_SEMOESET_SET_SHIFT)
#define  GPIO_SEMOESET_SET_REPLACE_VAL(reg,val) (((reg) & ~GPIO_SEMOESET_SET_MASK) | (((uint32_t)val) << GPIO_SEMOESET_SET_SHIFT))

//====================================================================
//Register: GPIO Semaphore Output Enable Clear Register (SemOEClear)
/** \brief Writing any bit in this register to a '1' will clear the corresponding output enable bit
      to '0' if that bit is in semaphore mode. Output enable bits corresponding to bits that
      are written as '0' in this register, or output enable bits that are not in semaphore
      mode will not be affected by the write to this register.*/
//====================================================================

#define  GPIO_SEMOECLEAR_CLEAR_MASK 0xffffffff
#define  GPIO_SEMOECLEAR_CLEAR_SHIFT 0
#define  GPIO_SEMOECLEAR_CLEAR_MASK_SHIFT(reg) (((reg) & GPIO_SEMOECLEAR_CLEAR_MASK) >> GPIO_SEMOECLEAR_CLEAR_SHIFT)
#define  GPIO_SEMOECLEAR_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~GPIO_SEMOECLEAR_CLEAR_MASK) | (((uint32_t)val) << GPIO_SEMOECLEAR_CLEAR_SHIFT))

//====================================================================
//Register: GPIO Semaphore Interrupt Enable Set Register (SemIESet)
/** \brief Writing any bit in this register to a '1' will set the corresponding interrupt enable bit
      to '1' if that bit is in semaphore mode. Interrupt enable bits corresponding to bits that
      are written as '0' in this register, or interrupt enable bits that are not in semaphore mode
      will not be affected by the write to this register.*/
//====================================================================

#define  GPIO_SEMIESET_SET_MASK 0xffffffff
#define  GPIO_SEMIESET_SET_SHIFT 0
#define  GPIO_SEMIESET_SET_MASK_SHIFT(reg) (((reg) & GPIO_SEMIESET_SET_MASK) >> GPIO_SEMIESET_SET_SHIFT)
#define  GPIO_SEMIESET_SET_REPLACE_VAL(reg,val) (((reg) & ~GPIO_SEMIESET_SET_MASK) | (((uint32_t)val) << GPIO_SEMIESET_SET_SHIFT))

//====================================================================
//Register: GPIO Semaphore Interrupt Enable Clear Register (SemIEClear)
/** \brief Writing any bit in this register to a '1' will clear the corresponding interrupt enable bit
      to '0' if that bit is in semaphore mode. Interrupt enable bits corresponding to bits that
      are written as '0' in this register, or interrupt enable bits that are not in semaphore
      mode will not be affected by the write to this register.*/
//====================================================================

#define  GPIO_SEMIECLEAR_CLEAR_MASK 0xffffffff
#define  GPIO_SEMIECLEAR_CLEAR_SHIFT 0
#define  GPIO_SEMIECLEAR_CLEAR_MASK_SHIFT(reg) (((reg) & GPIO_SEMIECLEAR_CLEAR_MASK) >> GPIO_SEMIECLEAR_CLEAR_SHIFT)
#define  GPIO_SEMIECLEAR_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~GPIO_SEMIECLEAR_CLEAR_MASK) | (((uint32_t)val) << GPIO_SEMIECLEAR_CLEAR_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 1 of 32) (PinCfg0)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 1 of 32)*/
//====================================================================

#define  GPIO_PINCFG0_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG0_RESERVED1_SHIFT 20
#define  GPIO_PINCFG0_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_RESERVED1_MASK) >> GPIO_PINCFG0_RESERVED1_SHIFT)
#define  GPIO_PINCFG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG0_RESERVED1_SHIFT))

#define  GPIO_PINCFG0_SLEW_MASK 0xc0000
#define  GPIO_PINCFG0_SLEW_SHIFT 18
#define  GPIO_PINCFG0_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_SLEW_MASK) >> GPIO_PINCFG0_SLEW_SHIFT)
#define  GPIO_PINCFG0_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG0_SLEW_SHIFT))

#define  GPIO_PINCFG0_PDEN_MASK 0x20000
#define  GPIO_PINCFG0_PDEN_SHIFT 17
#define  GPIO_PINCFG0_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_PDEN_MASK) >> GPIO_PINCFG0_PDEN_SHIFT)
#define  GPIO_PINCFG0_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG0_PDEN_SHIFT))

#define  GPIO_PINCFG0_PUEN_MASK 0x10000
#define  GPIO_PINCFG0_PUEN_SHIFT 16
#define  GPIO_PINCFG0_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_PUEN_MASK) >> GPIO_PINCFG0_PUEN_SHIFT)
#define  GPIO_PINCFG0_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG0_PUEN_SHIFT))

#define  GPIO_PINCFG0_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG0_RESERVED2_SHIFT 13
#define  GPIO_PINCFG0_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_RESERVED2_MASK) >> GPIO_PINCFG0_RESERVED2_SHIFT)
#define  GPIO_PINCFG0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG0_RESERVED2_SHIFT))

#define  GPIO_PINCFG0_RAWALT_MASK 0x1000
#define  GPIO_PINCFG0_RAWALT_SHIFT 12
#define  GPIO_PINCFG0_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_RAWALT_MASK) >> GPIO_PINCFG0_RAWALT_SHIFT)
#define  GPIO_PINCFG0_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG0_RAWALT_SHIFT))

#define  GPIO_PINCFG0_DEGTB_MASK 0xf00
#define  GPIO_PINCFG0_DEGTB_SHIFT 8
#define  GPIO_PINCFG0_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_DEGTB_MASK) >> GPIO_PINCFG0_DEGTB_SHIFT)
#define  GPIO_PINCFG0_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG0_DEGTB_SHIFT))

#define  GPIO_PINCFG0_INTPOL_MASK 0xc0
#define  GPIO_PINCFG0_INTPOL_SHIFT 6
#define  GPIO_PINCFG0_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_INTPOL_MASK) >> GPIO_PINCFG0_INTPOL_SHIFT)
#define  GPIO_PINCFG0_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG0_INTPOL_SHIFT))

#define  GPIO_PINCFG0_INTMODE_MASK 0x20
#define  GPIO_PINCFG0_INTMODE_SHIFT 5
#define  GPIO_PINCFG0_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_INTMODE_MASK) >> GPIO_PINCFG0_INTMODE_SHIFT)
#define  GPIO_PINCFG0_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG0_INTMODE_SHIFT))

#define  GPIO_PINCFG0_SEMMODE_MASK 0x10
#define  GPIO_PINCFG0_SEMMODE_SHIFT 4
#define  GPIO_PINCFG0_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_SEMMODE_MASK) >> GPIO_PINCFG0_SEMMODE_SHIFT)
#define  GPIO_PINCFG0_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG0_SEMMODE_SHIFT))

#define  GPIO_PINCFG0_RESERVED3_MASK 0x8
#define  GPIO_PINCFG0_RESERVED3_SHIFT 3
#define  GPIO_PINCFG0_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_RESERVED3_MASK) >> GPIO_PINCFG0_RESERVED3_SHIFT)
#define  GPIO_PINCFG0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG0_RESERVED3_SHIFT))

#define  GPIO_PINCFG0_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG0_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG0_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG0_FUNCSEL_MASK) >> GPIO_PINCFG0_FUNCSEL_SHIFT)
#define  GPIO_PINCFG0_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG0_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG0_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 2 of 32) (PinCfg1)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 2 of 32)*/
//====================================================================

#define  GPIO_PINCFG1_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG1_RESERVED1_SHIFT 20
#define  GPIO_PINCFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_RESERVED1_MASK) >> GPIO_PINCFG1_RESERVED1_SHIFT)
#define  GPIO_PINCFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG1_RESERVED1_SHIFT))

#define  GPIO_PINCFG1_SLEW_MASK 0xc0000
#define  GPIO_PINCFG1_SLEW_SHIFT 18
#define  GPIO_PINCFG1_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_SLEW_MASK) >> GPIO_PINCFG1_SLEW_SHIFT)
#define  GPIO_PINCFG1_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG1_SLEW_SHIFT))

#define  GPIO_PINCFG1_PDEN_MASK 0x20000
#define  GPIO_PINCFG1_PDEN_SHIFT 17
#define  GPIO_PINCFG1_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_PDEN_MASK) >> GPIO_PINCFG1_PDEN_SHIFT)
#define  GPIO_PINCFG1_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG1_PDEN_SHIFT))

#define  GPIO_PINCFG1_PUEN_MASK 0x10000
#define  GPIO_PINCFG1_PUEN_SHIFT 16
#define  GPIO_PINCFG1_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_PUEN_MASK) >> GPIO_PINCFG1_PUEN_SHIFT)
#define  GPIO_PINCFG1_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG1_PUEN_SHIFT))

#define  GPIO_PINCFG1_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG1_RESERVED2_SHIFT 13
#define  GPIO_PINCFG1_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_RESERVED2_MASK) >> GPIO_PINCFG1_RESERVED2_SHIFT)
#define  GPIO_PINCFG1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG1_RESERVED2_SHIFT))

#define  GPIO_PINCFG1_RAWALT_MASK 0x1000
#define  GPIO_PINCFG1_RAWALT_SHIFT 12
#define  GPIO_PINCFG1_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_RAWALT_MASK) >> GPIO_PINCFG1_RAWALT_SHIFT)
#define  GPIO_PINCFG1_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG1_RAWALT_SHIFT))

#define  GPIO_PINCFG1_DEGTB_MASK 0xf00
#define  GPIO_PINCFG1_DEGTB_SHIFT 8
#define  GPIO_PINCFG1_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_DEGTB_MASK) >> GPIO_PINCFG1_DEGTB_SHIFT)
#define  GPIO_PINCFG1_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG1_DEGTB_SHIFT))

#define  GPIO_PINCFG1_INTPOL_MASK 0xc0
#define  GPIO_PINCFG1_INTPOL_SHIFT 6
#define  GPIO_PINCFG1_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_INTPOL_MASK) >> GPIO_PINCFG1_INTPOL_SHIFT)
#define  GPIO_PINCFG1_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG1_INTPOL_SHIFT))

#define  GPIO_PINCFG1_INTMODE_MASK 0x20
#define  GPIO_PINCFG1_INTMODE_SHIFT 5
#define  GPIO_PINCFG1_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_INTMODE_MASK) >> GPIO_PINCFG1_INTMODE_SHIFT)
#define  GPIO_PINCFG1_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG1_INTMODE_SHIFT))

#define  GPIO_PINCFG1_SEMMODE_MASK 0x10
#define  GPIO_PINCFG1_SEMMODE_SHIFT 4
#define  GPIO_PINCFG1_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_SEMMODE_MASK) >> GPIO_PINCFG1_SEMMODE_SHIFT)
#define  GPIO_PINCFG1_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG1_SEMMODE_SHIFT))

#define  GPIO_PINCFG1_RESERVED3_MASK 0x8
#define  GPIO_PINCFG1_RESERVED3_SHIFT 3
#define  GPIO_PINCFG1_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_RESERVED3_MASK) >> GPIO_PINCFG1_RESERVED3_SHIFT)
#define  GPIO_PINCFG1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG1_RESERVED3_SHIFT))

#define  GPIO_PINCFG1_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG1_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG1_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG1_FUNCSEL_MASK) >> GPIO_PINCFG1_FUNCSEL_SHIFT)
#define  GPIO_PINCFG1_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG1_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG1_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 3 of 32) (PinCfg2)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 3 of 32)*/
//====================================================================

#define  GPIO_PINCFG2_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG2_RESERVED1_SHIFT 20
#define  GPIO_PINCFG2_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_RESERVED1_MASK) >> GPIO_PINCFG2_RESERVED1_SHIFT)
#define  GPIO_PINCFG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG2_RESERVED1_SHIFT))

#define  GPIO_PINCFG2_SLEW_MASK 0xc0000
#define  GPIO_PINCFG2_SLEW_SHIFT 18
#define  GPIO_PINCFG2_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_SLEW_MASK) >> GPIO_PINCFG2_SLEW_SHIFT)
#define  GPIO_PINCFG2_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG2_SLEW_SHIFT))

#define  GPIO_PINCFG2_PDEN_MASK 0x20000
#define  GPIO_PINCFG2_PDEN_SHIFT 17
#define  GPIO_PINCFG2_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_PDEN_MASK) >> GPIO_PINCFG2_PDEN_SHIFT)
#define  GPIO_PINCFG2_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG2_PDEN_SHIFT))

#define  GPIO_PINCFG2_PUEN_MASK 0x10000
#define  GPIO_PINCFG2_PUEN_SHIFT 16
#define  GPIO_PINCFG2_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_PUEN_MASK) >> GPIO_PINCFG2_PUEN_SHIFT)
#define  GPIO_PINCFG2_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG2_PUEN_SHIFT))

#define  GPIO_PINCFG2_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG2_RESERVED2_SHIFT 13
#define  GPIO_PINCFG2_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_RESERVED2_MASK) >> GPIO_PINCFG2_RESERVED2_SHIFT)
#define  GPIO_PINCFG2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG2_RESERVED2_SHIFT))

#define  GPIO_PINCFG2_RAWALT_MASK 0x1000
#define  GPIO_PINCFG2_RAWALT_SHIFT 12
#define  GPIO_PINCFG2_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_RAWALT_MASK) >> GPIO_PINCFG2_RAWALT_SHIFT)
#define  GPIO_PINCFG2_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG2_RAWALT_SHIFT))

#define  GPIO_PINCFG2_DEGTB_MASK 0xf00
#define  GPIO_PINCFG2_DEGTB_SHIFT 8
#define  GPIO_PINCFG2_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_DEGTB_MASK) >> GPIO_PINCFG2_DEGTB_SHIFT)
#define  GPIO_PINCFG2_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG2_DEGTB_SHIFT))

#define  GPIO_PINCFG2_INTPOL_MASK 0xc0
#define  GPIO_PINCFG2_INTPOL_SHIFT 6
#define  GPIO_PINCFG2_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_INTPOL_MASK) >> GPIO_PINCFG2_INTPOL_SHIFT)
#define  GPIO_PINCFG2_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG2_INTPOL_SHIFT))

#define  GPIO_PINCFG2_INTMODE_MASK 0x20
#define  GPIO_PINCFG2_INTMODE_SHIFT 5
#define  GPIO_PINCFG2_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_INTMODE_MASK) >> GPIO_PINCFG2_INTMODE_SHIFT)
#define  GPIO_PINCFG2_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG2_INTMODE_SHIFT))

#define  GPIO_PINCFG2_SEMMODE_MASK 0x10
#define  GPIO_PINCFG2_SEMMODE_SHIFT 4
#define  GPIO_PINCFG2_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_SEMMODE_MASK) >> GPIO_PINCFG2_SEMMODE_SHIFT)
#define  GPIO_PINCFG2_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG2_SEMMODE_SHIFT))

#define  GPIO_PINCFG2_RESERVED3_MASK 0x8
#define  GPIO_PINCFG2_RESERVED3_SHIFT 3
#define  GPIO_PINCFG2_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_RESERVED3_MASK) >> GPIO_PINCFG2_RESERVED3_SHIFT)
#define  GPIO_PINCFG2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG2_RESERVED3_SHIFT))

#define  GPIO_PINCFG2_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG2_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG2_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG2_FUNCSEL_MASK) >> GPIO_PINCFG2_FUNCSEL_SHIFT)
#define  GPIO_PINCFG2_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG2_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG2_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 4 of 32) (PinCfg3)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 4 of 32)*/
//====================================================================

#define  GPIO_PINCFG3_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG3_RESERVED1_SHIFT 20
#define  GPIO_PINCFG3_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_RESERVED1_MASK) >> GPIO_PINCFG3_RESERVED1_SHIFT)
#define  GPIO_PINCFG3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG3_RESERVED1_SHIFT))

#define  GPIO_PINCFG3_SLEW_MASK 0xc0000
#define  GPIO_PINCFG3_SLEW_SHIFT 18
#define  GPIO_PINCFG3_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_SLEW_MASK) >> GPIO_PINCFG3_SLEW_SHIFT)
#define  GPIO_PINCFG3_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG3_SLEW_SHIFT))

#define  GPIO_PINCFG3_PDEN_MASK 0x20000
#define  GPIO_PINCFG3_PDEN_SHIFT 17
#define  GPIO_PINCFG3_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_PDEN_MASK) >> GPIO_PINCFG3_PDEN_SHIFT)
#define  GPIO_PINCFG3_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG3_PDEN_SHIFT))

#define  GPIO_PINCFG3_PUEN_MASK 0x10000
#define  GPIO_PINCFG3_PUEN_SHIFT 16
#define  GPIO_PINCFG3_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_PUEN_MASK) >> GPIO_PINCFG3_PUEN_SHIFT)
#define  GPIO_PINCFG3_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG3_PUEN_SHIFT))

#define  GPIO_PINCFG3_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG3_RESERVED2_SHIFT 13
#define  GPIO_PINCFG3_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_RESERVED2_MASK) >> GPIO_PINCFG3_RESERVED2_SHIFT)
#define  GPIO_PINCFG3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG3_RESERVED2_SHIFT))

#define  GPIO_PINCFG3_RAWALT_MASK 0x1000
#define  GPIO_PINCFG3_RAWALT_SHIFT 12
#define  GPIO_PINCFG3_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_RAWALT_MASK) >> GPIO_PINCFG3_RAWALT_SHIFT)
#define  GPIO_PINCFG3_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG3_RAWALT_SHIFT))

#define  GPIO_PINCFG3_DEGTB_MASK 0xf00
#define  GPIO_PINCFG3_DEGTB_SHIFT 8
#define  GPIO_PINCFG3_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_DEGTB_MASK) >> GPIO_PINCFG3_DEGTB_SHIFT)
#define  GPIO_PINCFG3_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG3_DEGTB_SHIFT))

#define  GPIO_PINCFG3_INTPOL_MASK 0xc0
#define  GPIO_PINCFG3_INTPOL_SHIFT 6
#define  GPIO_PINCFG3_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_INTPOL_MASK) >> GPIO_PINCFG3_INTPOL_SHIFT)
#define  GPIO_PINCFG3_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG3_INTPOL_SHIFT))

#define  GPIO_PINCFG3_INTMODE_MASK 0x20
#define  GPIO_PINCFG3_INTMODE_SHIFT 5
#define  GPIO_PINCFG3_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_INTMODE_MASK) >> GPIO_PINCFG3_INTMODE_SHIFT)
#define  GPIO_PINCFG3_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG3_INTMODE_SHIFT))

#define  GPIO_PINCFG3_SEMMODE_MASK 0x10
#define  GPIO_PINCFG3_SEMMODE_SHIFT 4
#define  GPIO_PINCFG3_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_SEMMODE_MASK) >> GPIO_PINCFG3_SEMMODE_SHIFT)
#define  GPIO_PINCFG3_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG3_SEMMODE_SHIFT))

#define  GPIO_PINCFG3_RESERVED3_MASK 0x8
#define  GPIO_PINCFG3_RESERVED3_SHIFT 3
#define  GPIO_PINCFG3_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_RESERVED3_MASK) >> GPIO_PINCFG3_RESERVED3_SHIFT)
#define  GPIO_PINCFG3_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG3_RESERVED3_SHIFT))

#define  GPIO_PINCFG3_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG3_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG3_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG3_FUNCSEL_MASK) >> GPIO_PINCFG3_FUNCSEL_SHIFT)
#define  GPIO_PINCFG3_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG3_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG3_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 5 of 32) (PinCfg4)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 5 of 32)*/
//====================================================================

#define  GPIO_PINCFG4_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG4_RESERVED1_SHIFT 20
#define  GPIO_PINCFG4_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_RESERVED1_MASK) >> GPIO_PINCFG4_RESERVED1_SHIFT)
#define  GPIO_PINCFG4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG4_RESERVED1_SHIFT))

#define  GPIO_PINCFG4_SLEW_MASK 0xc0000
#define  GPIO_PINCFG4_SLEW_SHIFT 18
#define  GPIO_PINCFG4_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_SLEW_MASK) >> GPIO_PINCFG4_SLEW_SHIFT)
#define  GPIO_PINCFG4_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG4_SLEW_SHIFT))

#define  GPIO_PINCFG4_PDEN_MASK 0x20000
#define  GPIO_PINCFG4_PDEN_SHIFT 17
#define  GPIO_PINCFG4_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_PDEN_MASK) >> GPIO_PINCFG4_PDEN_SHIFT)
#define  GPIO_PINCFG4_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG4_PDEN_SHIFT))

#define  GPIO_PINCFG4_PUEN_MASK 0x10000
#define  GPIO_PINCFG4_PUEN_SHIFT 16
#define  GPIO_PINCFG4_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_PUEN_MASK) >> GPIO_PINCFG4_PUEN_SHIFT)
#define  GPIO_PINCFG4_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG4_PUEN_SHIFT))

#define  GPIO_PINCFG4_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG4_RESERVED2_SHIFT 13
#define  GPIO_PINCFG4_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_RESERVED2_MASK) >> GPIO_PINCFG4_RESERVED2_SHIFT)
#define  GPIO_PINCFG4_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG4_RESERVED2_SHIFT))

#define  GPIO_PINCFG4_RAWALT_MASK 0x1000
#define  GPIO_PINCFG4_RAWALT_SHIFT 12
#define  GPIO_PINCFG4_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_RAWALT_MASK) >> GPIO_PINCFG4_RAWALT_SHIFT)
#define  GPIO_PINCFG4_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG4_RAWALT_SHIFT))

#define  GPIO_PINCFG4_DEGTB_MASK 0xf00
#define  GPIO_PINCFG4_DEGTB_SHIFT 8
#define  GPIO_PINCFG4_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_DEGTB_MASK) >> GPIO_PINCFG4_DEGTB_SHIFT)
#define  GPIO_PINCFG4_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG4_DEGTB_SHIFT))

#define  GPIO_PINCFG4_INTPOL_MASK 0xc0
#define  GPIO_PINCFG4_INTPOL_SHIFT 6
#define  GPIO_PINCFG4_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_INTPOL_MASK) >> GPIO_PINCFG4_INTPOL_SHIFT)
#define  GPIO_PINCFG4_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG4_INTPOL_SHIFT))

#define  GPIO_PINCFG4_INTMODE_MASK 0x20
#define  GPIO_PINCFG4_INTMODE_SHIFT 5
#define  GPIO_PINCFG4_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_INTMODE_MASK) >> GPIO_PINCFG4_INTMODE_SHIFT)
#define  GPIO_PINCFG4_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG4_INTMODE_SHIFT))

#define  GPIO_PINCFG4_SEMMODE_MASK 0x10
#define  GPIO_PINCFG4_SEMMODE_SHIFT 4
#define  GPIO_PINCFG4_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_SEMMODE_MASK) >> GPIO_PINCFG4_SEMMODE_SHIFT)
#define  GPIO_PINCFG4_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG4_SEMMODE_SHIFT))

#define  GPIO_PINCFG4_RESERVED3_MASK 0x8
#define  GPIO_PINCFG4_RESERVED3_SHIFT 3
#define  GPIO_PINCFG4_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_RESERVED3_MASK) >> GPIO_PINCFG4_RESERVED3_SHIFT)
#define  GPIO_PINCFG4_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG4_RESERVED3_SHIFT))

#define  GPIO_PINCFG4_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG4_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG4_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG4_FUNCSEL_MASK) >> GPIO_PINCFG4_FUNCSEL_SHIFT)
#define  GPIO_PINCFG4_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG4_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG4_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 6 of 32) (PinCfg5)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 6 of 32)*/
//====================================================================

#define  GPIO_PINCFG5_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG5_RESERVED1_SHIFT 20
#define  GPIO_PINCFG5_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_RESERVED1_MASK) >> GPIO_PINCFG5_RESERVED1_SHIFT)
#define  GPIO_PINCFG5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG5_RESERVED1_SHIFT))

#define  GPIO_PINCFG5_SLEW_MASK 0xc0000
#define  GPIO_PINCFG5_SLEW_SHIFT 18
#define  GPIO_PINCFG5_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_SLEW_MASK) >> GPIO_PINCFG5_SLEW_SHIFT)
#define  GPIO_PINCFG5_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG5_SLEW_SHIFT))

#define  GPIO_PINCFG5_PDEN_MASK 0x20000
#define  GPIO_PINCFG5_PDEN_SHIFT 17
#define  GPIO_PINCFG5_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_PDEN_MASK) >> GPIO_PINCFG5_PDEN_SHIFT)
#define  GPIO_PINCFG5_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG5_PDEN_SHIFT))

#define  GPIO_PINCFG5_PUEN_MASK 0x10000
#define  GPIO_PINCFG5_PUEN_SHIFT 16
#define  GPIO_PINCFG5_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_PUEN_MASK) >> GPIO_PINCFG5_PUEN_SHIFT)
#define  GPIO_PINCFG5_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG5_PUEN_SHIFT))

#define  GPIO_PINCFG5_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG5_RESERVED2_SHIFT 13
#define  GPIO_PINCFG5_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_RESERVED2_MASK) >> GPIO_PINCFG5_RESERVED2_SHIFT)
#define  GPIO_PINCFG5_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG5_RESERVED2_SHIFT))

#define  GPIO_PINCFG5_RAWALT_MASK 0x1000
#define  GPIO_PINCFG5_RAWALT_SHIFT 12
#define  GPIO_PINCFG5_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_RAWALT_MASK) >> GPIO_PINCFG5_RAWALT_SHIFT)
#define  GPIO_PINCFG5_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG5_RAWALT_SHIFT))

#define  GPIO_PINCFG5_DEGTB_MASK 0xf00
#define  GPIO_PINCFG5_DEGTB_SHIFT 8
#define  GPIO_PINCFG5_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_DEGTB_MASK) >> GPIO_PINCFG5_DEGTB_SHIFT)
#define  GPIO_PINCFG5_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG5_DEGTB_SHIFT))

#define  GPIO_PINCFG5_INTPOL_MASK 0xc0
#define  GPIO_PINCFG5_INTPOL_SHIFT 6
#define  GPIO_PINCFG5_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_INTPOL_MASK) >> GPIO_PINCFG5_INTPOL_SHIFT)
#define  GPIO_PINCFG5_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG5_INTPOL_SHIFT))

#define  GPIO_PINCFG5_INTMODE_MASK 0x20
#define  GPIO_PINCFG5_INTMODE_SHIFT 5
#define  GPIO_PINCFG5_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_INTMODE_MASK) >> GPIO_PINCFG5_INTMODE_SHIFT)
#define  GPIO_PINCFG5_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG5_INTMODE_SHIFT))

#define  GPIO_PINCFG5_SEMMODE_MASK 0x10
#define  GPIO_PINCFG5_SEMMODE_SHIFT 4
#define  GPIO_PINCFG5_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_SEMMODE_MASK) >> GPIO_PINCFG5_SEMMODE_SHIFT)
#define  GPIO_PINCFG5_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG5_SEMMODE_SHIFT))

#define  GPIO_PINCFG5_RESERVED3_MASK 0x8
#define  GPIO_PINCFG5_RESERVED3_SHIFT 3
#define  GPIO_PINCFG5_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_RESERVED3_MASK) >> GPIO_PINCFG5_RESERVED3_SHIFT)
#define  GPIO_PINCFG5_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG5_RESERVED3_SHIFT))

#define  GPIO_PINCFG5_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG5_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG5_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG5_FUNCSEL_MASK) >> GPIO_PINCFG5_FUNCSEL_SHIFT)
#define  GPIO_PINCFG5_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG5_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG5_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 7 of 32) (PinCfg6)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 7 of 32)*/
//====================================================================

#define  GPIO_PINCFG6_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG6_RESERVED1_SHIFT 20
#define  GPIO_PINCFG6_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_RESERVED1_MASK) >> GPIO_PINCFG6_RESERVED1_SHIFT)
#define  GPIO_PINCFG6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG6_RESERVED1_SHIFT))

#define  GPIO_PINCFG6_SLEW_MASK 0xc0000
#define  GPIO_PINCFG6_SLEW_SHIFT 18
#define  GPIO_PINCFG6_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_SLEW_MASK) >> GPIO_PINCFG6_SLEW_SHIFT)
#define  GPIO_PINCFG6_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG6_SLEW_SHIFT))

#define  GPIO_PINCFG6_PDEN_MASK 0x20000
#define  GPIO_PINCFG6_PDEN_SHIFT 17
#define  GPIO_PINCFG6_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_PDEN_MASK) >> GPIO_PINCFG6_PDEN_SHIFT)
#define  GPIO_PINCFG6_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG6_PDEN_SHIFT))

#define  GPIO_PINCFG6_PUEN_MASK 0x10000
#define  GPIO_PINCFG6_PUEN_SHIFT 16
#define  GPIO_PINCFG6_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_PUEN_MASK) >> GPIO_PINCFG6_PUEN_SHIFT)
#define  GPIO_PINCFG6_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG6_PUEN_SHIFT))

#define  GPIO_PINCFG6_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG6_RESERVED2_SHIFT 13
#define  GPIO_PINCFG6_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_RESERVED2_MASK) >> GPIO_PINCFG6_RESERVED2_SHIFT)
#define  GPIO_PINCFG6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG6_RESERVED2_SHIFT))

#define  GPIO_PINCFG6_RAWALT_MASK 0x1000
#define  GPIO_PINCFG6_RAWALT_SHIFT 12
#define  GPIO_PINCFG6_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_RAWALT_MASK) >> GPIO_PINCFG6_RAWALT_SHIFT)
#define  GPIO_PINCFG6_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG6_RAWALT_SHIFT))

#define  GPIO_PINCFG6_DEGTB_MASK 0xf00
#define  GPIO_PINCFG6_DEGTB_SHIFT 8
#define  GPIO_PINCFG6_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_DEGTB_MASK) >> GPIO_PINCFG6_DEGTB_SHIFT)
#define  GPIO_PINCFG6_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG6_DEGTB_SHIFT))

#define  GPIO_PINCFG6_INTPOL_MASK 0xc0
#define  GPIO_PINCFG6_INTPOL_SHIFT 6
#define  GPIO_PINCFG6_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_INTPOL_MASK) >> GPIO_PINCFG6_INTPOL_SHIFT)
#define  GPIO_PINCFG6_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG6_INTPOL_SHIFT))

#define  GPIO_PINCFG6_INTMODE_MASK 0x20
#define  GPIO_PINCFG6_INTMODE_SHIFT 5
#define  GPIO_PINCFG6_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_INTMODE_MASK) >> GPIO_PINCFG6_INTMODE_SHIFT)
#define  GPIO_PINCFG6_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG6_INTMODE_SHIFT))

#define  GPIO_PINCFG6_SEMMODE_MASK 0x10
#define  GPIO_PINCFG6_SEMMODE_SHIFT 4
#define  GPIO_PINCFG6_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_SEMMODE_MASK) >> GPIO_PINCFG6_SEMMODE_SHIFT)
#define  GPIO_PINCFG6_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG6_SEMMODE_SHIFT))

#define  GPIO_PINCFG6_RESERVED3_MASK 0x8
#define  GPIO_PINCFG6_RESERVED3_SHIFT 3
#define  GPIO_PINCFG6_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_RESERVED3_MASK) >> GPIO_PINCFG6_RESERVED3_SHIFT)
#define  GPIO_PINCFG6_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG6_RESERVED3_SHIFT))

#define  GPIO_PINCFG6_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG6_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG6_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG6_FUNCSEL_MASK) >> GPIO_PINCFG6_FUNCSEL_SHIFT)
#define  GPIO_PINCFG6_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG6_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG6_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 8 of 32) (PinCfg7)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 8 of 32)*/
//====================================================================

#define  GPIO_PINCFG7_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG7_RESERVED1_SHIFT 20
#define  GPIO_PINCFG7_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_RESERVED1_MASK) >> GPIO_PINCFG7_RESERVED1_SHIFT)
#define  GPIO_PINCFG7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG7_RESERVED1_SHIFT))

#define  GPIO_PINCFG7_SLEW_MASK 0xc0000
#define  GPIO_PINCFG7_SLEW_SHIFT 18
#define  GPIO_PINCFG7_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_SLEW_MASK) >> GPIO_PINCFG7_SLEW_SHIFT)
#define  GPIO_PINCFG7_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG7_SLEW_SHIFT))

#define  GPIO_PINCFG7_PDEN_MASK 0x20000
#define  GPIO_PINCFG7_PDEN_SHIFT 17
#define  GPIO_PINCFG7_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_PDEN_MASK) >> GPIO_PINCFG7_PDEN_SHIFT)
#define  GPIO_PINCFG7_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG7_PDEN_SHIFT))

#define  GPIO_PINCFG7_PUEN_MASK 0x10000
#define  GPIO_PINCFG7_PUEN_SHIFT 16
#define  GPIO_PINCFG7_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_PUEN_MASK) >> GPIO_PINCFG7_PUEN_SHIFT)
#define  GPIO_PINCFG7_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG7_PUEN_SHIFT))

#define  GPIO_PINCFG7_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG7_RESERVED2_SHIFT 13
#define  GPIO_PINCFG7_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_RESERVED2_MASK) >> GPIO_PINCFG7_RESERVED2_SHIFT)
#define  GPIO_PINCFG7_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG7_RESERVED2_SHIFT))

#define  GPIO_PINCFG7_RAWALT_MASK 0x1000
#define  GPIO_PINCFG7_RAWALT_SHIFT 12
#define  GPIO_PINCFG7_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_RAWALT_MASK) >> GPIO_PINCFG7_RAWALT_SHIFT)
#define  GPIO_PINCFG7_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG7_RAWALT_SHIFT))

#define  GPIO_PINCFG7_DEGTB_MASK 0xf00
#define  GPIO_PINCFG7_DEGTB_SHIFT 8
#define  GPIO_PINCFG7_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_DEGTB_MASK) >> GPIO_PINCFG7_DEGTB_SHIFT)
#define  GPIO_PINCFG7_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG7_DEGTB_SHIFT))

#define  GPIO_PINCFG7_INTPOL_MASK 0xc0
#define  GPIO_PINCFG7_INTPOL_SHIFT 6
#define  GPIO_PINCFG7_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_INTPOL_MASK) >> GPIO_PINCFG7_INTPOL_SHIFT)
#define  GPIO_PINCFG7_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG7_INTPOL_SHIFT))

#define  GPIO_PINCFG7_INTMODE_MASK 0x20
#define  GPIO_PINCFG7_INTMODE_SHIFT 5
#define  GPIO_PINCFG7_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_INTMODE_MASK) >> GPIO_PINCFG7_INTMODE_SHIFT)
#define  GPIO_PINCFG7_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG7_INTMODE_SHIFT))

#define  GPIO_PINCFG7_SEMMODE_MASK 0x10
#define  GPIO_PINCFG7_SEMMODE_SHIFT 4
#define  GPIO_PINCFG7_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_SEMMODE_MASK) >> GPIO_PINCFG7_SEMMODE_SHIFT)
#define  GPIO_PINCFG7_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG7_SEMMODE_SHIFT))

#define  GPIO_PINCFG7_RESERVED3_MASK 0x8
#define  GPIO_PINCFG7_RESERVED3_SHIFT 3
#define  GPIO_PINCFG7_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_RESERVED3_MASK) >> GPIO_PINCFG7_RESERVED3_SHIFT)
#define  GPIO_PINCFG7_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG7_RESERVED3_SHIFT))

#define  GPIO_PINCFG7_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG7_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG7_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG7_FUNCSEL_MASK) >> GPIO_PINCFG7_FUNCSEL_SHIFT)
#define  GPIO_PINCFG7_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG7_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG7_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 9 of 32) (PinCfg8)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 9 of 32)*/
//====================================================================

#define  GPIO_PINCFG8_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG8_RESERVED1_SHIFT 20
#define  GPIO_PINCFG8_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_RESERVED1_MASK) >> GPIO_PINCFG8_RESERVED1_SHIFT)
#define  GPIO_PINCFG8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG8_RESERVED1_SHIFT))

#define  GPIO_PINCFG8_SLEW_MASK 0xc0000
#define  GPIO_PINCFG8_SLEW_SHIFT 18
#define  GPIO_PINCFG8_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_SLEW_MASK) >> GPIO_PINCFG8_SLEW_SHIFT)
#define  GPIO_PINCFG8_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG8_SLEW_SHIFT))

#define  GPIO_PINCFG8_PDEN_MASK 0x20000
#define  GPIO_PINCFG8_PDEN_SHIFT 17
#define  GPIO_PINCFG8_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_PDEN_MASK) >> GPIO_PINCFG8_PDEN_SHIFT)
#define  GPIO_PINCFG8_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG8_PDEN_SHIFT))

#define  GPIO_PINCFG8_PUEN_MASK 0x10000
#define  GPIO_PINCFG8_PUEN_SHIFT 16
#define  GPIO_PINCFG8_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_PUEN_MASK) >> GPIO_PINCFG8_PUEN_SHIFT)
#define  GPIO_PINCFG8_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG8_PUEN_SHIFT))

#define  GPIO_PINCFG8_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG8_RESERVED2_SHIFT 13
#define  GPIO_PINCFG8_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_RESERVED2_MASK) >> GPIO_PINCFG8_RESERVED2_SHIFT)
#define  GPIO_PINCFG8_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG8_RESERVED2_SHIFT))

#define  GPIO_PINCFG8_RAWALT_MASK 0x1000
#define  GPIO_PINCFG8_RAWALT_SHIFT 12
#define  GPIO_PINCFG8_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_RAWALT_MASK) >> GPIO_PINCFG8_RAWALT_SHIFT)
#define  GPIO_PINCFG8_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG8_RAWALT_SHIFT))

#define  GPIO_PINCFG8_DEGTB_MASK 0xf00
#define  GPIO_PINCFG8_DEGTB_SHIFT 8
#define  GPIO_PINCFG8_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_DEGTB_MASK) >> GPIO_PINCFG8_DEGTB_SHIFT)
#define  GPIO_PINCFG8_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG8_DEGTB_SHIFT))

#define  GPIO_PINCFG8_INTPOL_MASK 0xc0
#define  GPIO_PINCFG8_INTPOL_SHIFT 6
#define  GPIO_PINCFG8_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_INTPOL_MASK) >> GPIO_PINCFG8_INTPOL_SHIFT)
#define  GPIO_PINCFG8_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG8_INTPOL_SHIFT))

#define  GPIO_PINCFG8_INTMODE_MASK 0x20
#define  GPIO_PINCFG8_INTMODE_SHIFT 5
#define  GPIO_PINCFG8_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_INTMODE_MASK) >> GPIO_PINCFG8_INTMODE_SHIFT)
#define  GPIO_PINCFG8_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG8_INTMODE_SHIFT))

#define  GPIO_PINCFG8_SEMMODE_MASK 0x10
#define  GPIO_PINCFG8_SEMMODE_SHIFT 4
#define  GPIO_PINCFG8_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_SEMMODE_MASK) >> GPIO_PINCFG8_SEMMODE_SHIFT)
#define  GPIO_PINCFG8_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG8_SEMMODE_SHIFT))

#define  GPIO_PINCFG8_RESERVED3_MASK 0x8
#define  GPIO_PINCFG8_RESERVED3_SHIFT 3
#define  GPIO_PINCFG8_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_RESERVED3_MASK) >> GPIO_PINCFG8_RESERVED3_SHIFT)
#define  GPIO_PINCFG8_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG8_RESERVED3_SHIFT))

#define  GPIO_PINCFG8_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG8_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG8_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG8_FUNCSEL_MASK) >> GPIO_PINCFG8_FUNCSEL_SHIFT)
#define  GPIO_PINCFG8_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG8_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG8_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 10 of 32) (PinCfg9)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 10 of 32)*/
//====================================================================

#define  GPIO_PINCFG9_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG9_RESERVED1_SHIFT 20
#define  GPIO_PINCFG9_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_RESERVED1_MASK) >> GPIO_PINCFG9_RESERVED1_SHIFT)
#define  GPIO_PINCFG9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG9_RESERVED1_SHIFT))

#define  GPIO_PINCFG9_SLEW_MASK 0xc0000
#define  GPIO_PINCFG9_SLEW_SHIFT 18
#define  GPIO_PINCFG9_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_SLEW_MASK) >> GPIO_PINCFG9_SLEW_SHIFT)
#define  GPIO_PINCFG9_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG9_SLEW_SHIFT))

#define  GPIO_PINCFG9_PDEN_MASK 0x20000
#define  GPIO_PINCFG9_PDEN_SHIFT 17
#define  GPIO_PINCFG9_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_PDEN_MASK) >> GPIO_PINCFG9_PDEN_SHIFT)
#define  GPIO_PINCFG9_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG9_PDEN_SHIFT))

#define  GPIO_PINCFG9_PUEN_MASK 0x10000
#define  GPIO_PINCFG9_PUEN_SHIFT 16
#define  GPIO_PINCFG9_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_PUEN_MASK) >> GPIO_PINCFG9_PUEN_SHIFT)
#define  GPIO_PINCFG9_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG9_PUEN_SHIFT))

#define  GPIO_PINCFG9_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG9_RESERVED2_SHIFT 13
#define  GPIO_PINCFG9_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_RESERVED2_MASK) >> GPIO_PINCFG9_RESERVED2_SHIFT)
#define  GPIO_PINCFG9_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG9_RESERVED2_SHIFT))

#define  GPIO_PINCFG9_RAWALT_MASK 0x1000
#define  GPIO_PINCFG9_RAWALT_SHIFT 12
#define  GPIO_PINCFG9_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_RAWALT_MASK) >> GPIO_PINCFG9_RAWALT_SHIFT)
#define  GPIO_PINCFG9_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG9_RAWALT_SHIFT))

#define  GPIO_PINCFG9_DEGTB_MASK 0xf00
#define  GPIO_PINCFG9_DEGTB_SHIFT 8
#define  GPIO_PINCFG9_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_DEGTB_MASK) >> GPIO_PINCFG9_DEGTB_SHIFT)
#define  GPIO_PINCFG9_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG9_DEGTB_SHIFT))

#define  GPIO_PINCFG9_INTPOL_MASK 0xc0
#define  GPIO_PINCFG9_INTPOL_SHIFT 6
#define  GPIO_PINCFG9_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_INTPOL_MASK) >> GPIO_PINCFG9_INTPOL_SHIFT)
#define  GPIO_PINCFG9_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG9_INTPOL_SHIFT))

#define  GPIO_PINCFG9_INTMODE_MASK 0x20
#define  GPIO_PINCFG9_INTMODE_SHIFT 5
#define  GPIO_PINCFG9_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_INTMODE_MASK) >> GPIO_PINCFG9_INTMODE_SHIFT)
#define  GPIO_PINCFG9_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG9_INTMODE_SHIFT))

#define  GPIO_PINCFG9_SEMMODE_MASK 0x10
#define  GPIO_PINCFG9_SEMMODE_SHIFT 4
#define  GPIO_PINCFG9_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_SEMMODE_MASK) >> GPIO_PINCFG9_SEMMODE_SHIFT)
#define  GPIO_PINCFG9_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG9_SEMMODE_SHIFT))

#define  GPIO_PINCFG9_RESERVED3_MASK 0x8
#define  GPIO_PINCFG9_RESERVED3_SHIFT 3
#define  GPIO_PINCFG9_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_RESERVED3_MASK) >> GPIO_PINCFG9_RESERVED3_SHIFT)
#define  GPIO_PINCFG9_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG9_RESERVED3_SHIFT))

#define  GPIO_PINCFG9_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG9_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG9_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG9_FUNCSEL_MASK) >> GPIO_PINCFG9_FUNCSEL_SHIFT)
#define  GPIO_PINCFG9_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG9_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG9_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 11 of 32) (PinCfg10)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 11 of 32)*/
//====================================================================

#define  GPIO_PINCFG10_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG10_RESERVED1_SHIFT 20
#define  GPIO_PINCFG10_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_RESERVED1_MASK) >> GPIO_PINCFG10_RESERVED1_SHIFT)
#define  GPIO_PINCFG10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG10_RESERVED1_SHIFT))

#define  GPIO_PINCFG10_SLEW_MASK 0xc0000
#define  GPIO_PINCFG10_SLEW_SHIFT 18
#define  GPIO_PINCFG10_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_SLEW_MASK) >> GPIO_PINCFG10_SLEW_SHIFT)
#define  GPIO_PINCFG10_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG10_SLEW_SHIFT))

#define  GPIO_PINCFG10_PDEN_MASK 0x20000
#define  GPIO_PINCFG10_PDEN_SHIFT 17
#define  GPIO_PINCFG10_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_PDEN_MASK) >> GPIO_PINCFG10_PDEN_SHIFT)
#define  GPIO_PINCFG10_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG10_PDEN_SHIFT))

#define  GPIO_PINCFG10_PUEN_MASK 0x10000
#define  GPIO_PINCFG10_PUEN_SHIFT 16
#define  GPIO_PINCFG10_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_PUEN_MASK) >> GPIO_PINCFG10_PUEN_SHIFT)
#define  GPIO_PINCFG10_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG10_PUEN_SHIFT))

#define  GPIO_PINCFG10_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG10_RESERVED2_SHIFT 13
#define  GPIO_PINCFG10_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_RESERVED2_MASK) >> GPIO_PINCFG10_RESERVED2_SHIFT)
#define  GPIO_PINCFG10_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG10_RESERVED2_SHIFT))

#define  GPIO_PINCFG10_RAWALT_MASK 0x1000
#define  GPIO_PINCFG10_RAWALT_SHIFT 12
#define  GPIO_PINCFG10_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_RAWALT_MASK) >> GPIO_PINCFG10_RAWALT_SHIFT)
#define  GPIO_PINCFG10_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG10_RAWALT_SHIFT))

#define  GPIO_PINCFG10_DEGTB_MASK 0xf00
#define  GPIO_PINCFG10_DEGTB_SHIFT 8
#define  GPIO_PINCFG10_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_DEGTB_MASK) >> GPIO_PINCFG10_DEGTB_SHIFT)
#define  GPIO_PINCFG10_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG10_DEGTB_SHIFT))

#define  GPIO_PINCFG10_INTPOL_MASK 0xc0
#define  GPIO_PINCFG10_INTPOL_SHIFT 6
#define  GPIO_PINCFG10_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_INTPOL_MASK) >> GPIO_PINCFG10_INTPOL_SHIFT)
#define  GPIO_PINCFG10_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG10_INTPOL_SHIFT))

#define  GPIO_PINCFG10_INTMODE_MASK 0x20
#define  GPIO_PINCFG10_INTMODE_SHIFT 5
#define  GPIO_PINCFG10_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_INTMODE_MASK) >> GPIO_PINCFG10_INTMODE_SHIFT)
#define  GPIO_PINCFG10_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG10_INTMODE_SHIFT))

#define  GPIO_PINCFG10_SEMMODE_MASK 0x10
#define  GPIO_PINCFG10_SEMMODE_SHIFT 4
#define  GPIO_PINCFG10_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_SEMMODE_MASK) >> GPIO_PINCFG10_SEMMODE_SHIFT)
#define  GPIO_PINCFG10_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG10_SEMMODE_SHIFT))

#define  GPIO_PINCFG10_RESERVED3_MASK 0x8
#define  GPIO_PINCFG10_RESERVED3_SHIFT 3
#define  GPIO_PINCFG10_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_RESERVED3_MASK) >> GPIO_PINCFG10_RESERVED3_SHIFT)
#define  GPIO_PINCFG10_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG10_RESERVED3_SHIFT))

#define  GPIO_PINCFG10_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG10_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG10_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG10_FUNCSEL_MASK) >> GPIO_PINCFG10_FUNCSEL_SHIFT)
#define  GPIO_PINCFG10_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG10_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG10_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 12 of 32) (PinCfg11)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 12 of 32)*/
//====================================================================

#define  GPIO_PINCFG11_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG11_RESERVED1_SHIFT 20
#define  GPIO_PINCFG11_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_RESERVED1_MASK) >> GPIO_PINCFG11_RESERVED1_SHIFT)
#define  GPIO_PINCFG11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG11_RESERVED1_SHIFT))

#define  GPIO_PINCFG11_SLEW_MASK 0xc0000
#define  GPIO_PINCFG11_SLEW_SHIFT 18
#define  GPIO_PINCFG11_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_SLEW_MASK) >> GPIO_PINCFG11_SLEW_SHIFT)
#define  GPIO_PINCFG11_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG11_SLEW_SHIFT))

#define  GPIO_PINCFG11_PDEN_MASK 0x20000
#define  GPIO_PINCFG11_PDEN_SHIFT 17
#define  GPIO_PINCFG11_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_PDEN_MASK) >> GPIO_PINCFG11_PDEN_SHIFT)
#define  GPIO_PINCFG11_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG11_PDEN_SHIFT))

#define  GPIO_PINCFG11_PUEN_MASK 0x10000
#define  GPIO_PINCFG11_PUEN_SHIFT 16
#define  GPIO_PINCFG11_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_PUEN_MASK) >> GPIO_PINCFG11_PUEN_SHIFT)
#define  GPIO_PINCFG11_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG11_PUEN_SHIFT))

#define  GPIO_PINCFG11_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG11_RESERVED2_SHIFT 13
#define  GPIO_PINCFG11_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_RESERVED2_MASK) >> GPIO_PINCFG11_RESERVED2_SHIFT)
#define  GPIO_PINCFG11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG11_RESERVED2_SHIFT))

#define  GPIO_PINCFG11_RAWALT_MASK 0x1000
#define  GPIO_PINCFG11_RAWALT_SHIFT 12
#define  GPIO_PINCFG11_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_RAWALT_MASK) >> GPIO_PINCFG11_RAWALT_SHIFT)
#define  GPIO_PINCFG11_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG11_RAWALT_SHIFT))

#define  GPIO_PINCFG11_DEGTB_MASK 0xf00
#define  GPIO_PINCFG11_DEGTB_SHIFT 8
#define  GPIO_PINCFG11_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_DEGTB_MASK) >> GPIO_PINCFG11_DEGTB_SHIFT)
#define  GPIO_PINCFG11_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG11_DEGTB_SHIFT))

#define  GPIO_PINCFG11_INTPOL_MASK 0xc0
#define  GPIO_PINCFG11_INTPOL_SHIFT 6
#define  GPIO_PINCFG11_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_INTPOL_MASK) >> GPIO_PINCFG11_INTPOL_SHIFT)
#define  GPIO_PINCFG11_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG11_INTPOL_SHIFT))

#define  GPIO_PINCFG11_INTMODE_MASK 0x20
#define  GPIO_PINCFG11_INTMODE_SHIFT 5
#define  GPIO_PINCFG11_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_INTMODE_MASK) >> GPIO_PINCFG11_INTMODE_SHIFT)
#define  GPIO_PINCFG11_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG11_INTMODE_SHIFT))

#define  GPIO_PINCFG11_SEMMODE_MASK 0x10
#define  GPIO_PINCFG11_SEMMODE_SHIFT 4
#define  GPIO_PINCFG11_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_SEMMODE_MASK) >> GPIO_PINCFG11_SEMMODE_SHIFT)
#define  GPIO_PINCFG11_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG11_SEMMODE_SHIFT))

#define  GPIO_PINCFG11_RESERVED3_MASK 0x8
#define  GPIO_PINCFG11_RESERVED3_SHIFT 3
#define  GPIO_PINCFG11_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_RESERVED3_MASK) >> GPIO_PINCFG11_RESERVED3_SHIFT)
#define  GPIO_PINCFG11_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG11_RESERVED3_SHIFT))

#define  GPIO_PINCFG11_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG11_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG11_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG11_FUNCSEL_MASK) >> GPIO_PINCFG11_FUNCSEL_SHIFT)
#define  GPIO_PINCFG11_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG11_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG11_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 13 of 32) (PinCfg12)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 13 of 32)*/
//====================================================================

#define  GPIO_PINCFG12_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG12_RESERVED1_SHIFT 20
#define  GPIO_PINCFG12_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_RESERVED1_MASK) >> GPIO_PINCFG12_RESERVED1_SHIFT)
#define  GPIO_PINCFG12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG12_RESERVED1_SHIFT))

#define  GPIO_PINCFG12_SLEW_MASK 0xc0000
#define  GPIO_PINCFG12_SLEW_SHIFT 18
#define  GPIO_PINCFG12_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_SLEW_MASK) >> GPIO_PINCFG12_SLEW_SHIFT)
#define  GPIO_PINCFG12_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG12_SLEW_SHIFT))

#define  GPIO_PINCFG12_PDEN_MASK 0x20000
#define  GPIO_PINCFG12_PDEN_SHIFT 17
#define  GPIO_PINCFG12_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_PDEN_MASK) >> GPIO_PINCFG12_PDEN_SHIFT)
#define  GPIO_PINCFG12_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG12_PDEN_SHIFT))

#define  GPIO_PINCFG12_PUEN_MASK 0x10000
#define  GPIO_PINCFG12_PUEN_SHIFT 16
#define  GPIO_PINCFG12_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_PUEN_MASK) >> GPIO_PINCFG12_PUEN_SHIFT)
#define  GPIO_PINCFG12_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG12_PUEN_SHIFT))

#define  GPIO_PINCFG12_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG12_RESERVED2_SHIFT 13
#define  GPIO_PINCFG12_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_RESERVED2_MASK) >> GPIO_PINCFG12_RESERVED2_SHIFT)
#define  GPIO_PINCFG12_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG12_RESERVED2_SHIFT))

#define  GPIO_PINCFG12_RAWALT_MASK 0x1000
#define  GPIO_PINCFG12_RAWALT_SHIFT 12
#define  GPIO_PINCFG12_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_RAWALT_MASK) >> GPIO_PINCFG12_RAWALT_SHIFT)
#define  GPIO_PINCFG12_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG12_RAWALT_SHIFT))

#define  GPIO_PINCFG12_DEGTB_MASK 0xf00
#define  GPIO_PINCFG12_DEGTB_SHIFT 8
#define  GPIO_PINCFG12_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_DEGTB_MASK) >> GPIO_PINCFG12_DEGTB_SHIFT)
#define  GPIO_PINCFG12_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG12_DEGTB_SHIFT))

#define  GPIO_PINCFG12_INTPOL_MASK 0xc0
#define  GPIO_PINCFG12_INTPOL_SHIFT 6
#define  GPIO_PINCFG12_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_INTPOL_MASK) >> GPIO_PINCFG12_INTPOL_SHIFT)
#define  GPIO_PINCFG12_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG12_INTPOL_SHIFT))

#define  GPIO_PINCFG12_INTMODE_MASK 0x20
#define  GPIO_PINCFG12_INTMODE_SHIFT 5
#define  GPIO_PINCFG12_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_INTMODE_MASK) >> GPIO_PINCFG12_INTMODE_SHIFT)
#define  GPIO_PINCFG12_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG12_INTMODE_SHIFT))

#define  GPIO_PINCFG12_SEMMODE_MASK 0x10
#define  GPIO_PINCFG12_SEMMODE_SHIFT 4
#define  GPIO_PINCFG12_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_SEMMODE_MASK) >> GPIO_PINCFG12_SEMMODE_SHIFT)
#define  GPIO_PINCFG12_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG12_SEMMODE_SHIFT))

#define  GPIO_PINCFG12_RESERVED3_MASK 0x8
#define  GPIO_PINCFG12_RESERVED3_SHIFT 3
#define  GPIO_PINCFG12_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_RESERVED3_MASK) >> GPIO_PINCFG12_RESERVED3_SHIFT)
#define  GPIO_PINCFG12_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG12_RESERVED3_SHIFT))

#define  GPIO_PINCFG12_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG12_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG12_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG12_FUNCSEL_MASK) >> GPIO_PINCFG12_FUNCSEL_SHIFT)
#define  GPIO_PINCFG12_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG12_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG12_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 14 of 32) (PinCfg13)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 14 of 32)*/
//====================================================================

#define  GPIO_PINCFG13_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG13_RESERVED1_SHIFT 20
#define  GPIO_PINCFG13_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_RESERVED1_MASK) >> GPIO_PINCFG13_RESERVED1_SHIFT)
#define  GPIO_PINCFG13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG13_RESERVED1_SHIFT))

#define  GPIO_PINCFG13_SLEW_MASK 0xc0000
#define  GPIO_PINCFG13_SLEW_SHIFT 18
#define  GPIO_PINCFG13_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_SLEW_MASK) >> GPIO_PINCFG13_SLEW_SHIFT)
#define  GPIO_PINCFG13_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG13_SLEW_SHIFT))

#define  GPIO_PINCFG13_PDEN_MASK 0x20000
#define  GPIO_PINCFG13_PDEN_SHIFT 17
#define  GPIO_PINCFG13_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_PDEN_MASK) >> GPIO_PINCFG13_PDEN_SHIFT)
#define  GPIO_PINCFG13_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG13_PDEN_SHIFT))

#define  GPIO_PINCFG13_PUEN_MASK 0x10000
#define  GPIO_PINCFG13_PUEN_SHIFT 16
#define  GPIO_PINCFG13_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_PUEN_MASK) >> GPIO_PINCFG13_PUEN_SHIFT)
#define  GPIO_PINCFG13_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG13_PUEN_SHIFT))

#define  GPIO_PINCFG13_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG13_RESERVED2_SHIFT 13
#define  GPIO_PINCFG13_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_RESERVED2_MASK) >> GPIO_PINCFG13_RESERVED2_SHIFT)
#define  GPIO_PINCFG13_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG13_RESERVED2_SHIFT))

#define  GPIO_PINCFG13_RAWALT_MASK 0x1000
#define  GPIO_PINCFG13_RAWALT_SHIFT 12
#define  GPIO_PINCFG13_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_RAWALT_MASK) >> GPIO_PINCFG13_RAWALT_SHIFT)
#define  GPIO_PINCFG13_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG13_RAWALT_SHIFT))

#define  GPIO_PINCFG13_DEGTB_MASK 0xf00
#define  GPIO_PINCFG13_DEGTB_SHIFT 8
#define  GPIO_PINCFG13_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_DEGTB_MASK) >> GPIO_PINCFG13_DEGTB_SHIFT)
#define  GPIO_PINCFG13_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG13_DEGTB_SHIFT))

#define  GPIO_PINCFG13_INTPOL_MASK 0xc0
#define  GPIO_PINCFG13_INTPOL_SHIFT 6
#define  GPIO_PINCFG13_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_INTPOL_MASK) >> GPIO_PINCFG13_INTPOL_SHIFT)
#define  GPIO_PINCFG13_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG13_INTPOL_SHIFT))

#define  GPIO_PINCFG13_INTMODE_MASK 0x20
#define  GPIO_PINCFG13_INTMODE_SHIFT 5
#define  GPIO_PINCFG13_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_INTMODE_MASK) >> GPIO_PINCFG13_INTMODE_SHIFT)
#define  GPIO_PINCFG13_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG13_INTMODE_SHIFT))

#define  GPIO_PINCFG13_SEMMODE_MASK 0x10
#define  GPIO_PINCFG13_SEMMODE_SHIFT 4
#define  GPIO_PINCFG13_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_SEMMODE_MASK) >> GPIO_PINCFG13_SEMMODE_SHIFT)
#define  GPIO_PINCFG13_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG13_SEMMODE_SHIFT))

#define  GPIO_PINCFG13_RESERVED3_MASK 0x8
#define  GPIO_PINCFG13_RESERVED3_SHIFT 3
#define  GPIO_PINCFG13_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_RESERVED3_MASK) >> GPIO_PINCFG13_RESERVED3_SHIFT)
#define  GPIO_PINCFG13_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG13_RESERVED3_SHIFT))

#define  GPIO_PINCFG13_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG13_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG13_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG13_FUNCSEL_MASK) >> GPIO_PINCFG13_FUNCSEL_SHIFT)
#define  GPIO_PINCFG13_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG13_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG13_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 15 of 32) (PinCfg14)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 15 of 32)*/
//====================================================================

#define  GPIO_PINCFG14_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG14_RESERVED1_SHIFT 20
#define  GPIO_PINCFG14_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_RESERVED1_MASK) >> GPIO_PINCFG14_RESERVED1_SHIFT)
#define  GPIO_PINCFG14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG14_RESERVED1_SHIFT))

#define  GPIO_PINCFG14_SLEW_MASK 0xc0000
#define  GPIO_PINCFG14_SLEW_SHIFT 18
#define  GPIO_PINCFG14_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_SLEW_MASK) >> GPIO_PINCFG14_SLEW_SHIFT)
#define  GPIO_PINCFG14_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG14_SLEW_SHIFT))

#define  GPIO_PINCFG14_PDEN_MASK 0x20000
#define  GPIO_PINCFG14_PDEN_SHIFT 17
#define  GPIO_PINCFG14_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_PDEN_MASK) >> GPIO_PINCFG14_PDEN_SHIFT)
#define  GPIO_PINCFG14_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG14_PDEN_SHIFT))

#define  GPIO_PINCFG14_PUEN_MASK 0x10000
#define  GPIO_PINCFG14_PUEN_SHIFT 16
#define  GPIO_PINCFG14_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_PUEN_MASK) >> GPIO_PINCFG14_PUEN_SHIFT)
#define  GPIO_PINCFG14_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG14_PUEN_SHIFT))

#define  GPIO_PINCFG14_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG14_RESERVED2_SHIFT 13
#define  GPIO_PINCFG14_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_RESERVED2_MASK) >> GPIO_PINCFG14_RESERVED2_SHIFT)
#define  GPIO_PINCFG14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG14_RESERVED2_SHIFT))

#define  GPIO_PINCFG14_RAWALT_MASK 0x1000
#define  GPIO_PINCFG14_RAWALT_SHIFT 12
#define  GPIO_PINCFG14_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_RAWALT_MASK) >> GPIO_PINCFG14_RAWALT_SHIFT)
#define  GPIO_PINCFG14_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG14_RAWALT_SHIFT))

#define  GPIO_PINCFG14_DEGTB_MASK 0xf00
#define  GPIO_PINCFG14_DEGTB_SHIFT 8
#define  GPIO_PINCFG14_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_DEGTB_MASK) >> GPIO_PINCFG14_DEGTB_SHIFT)
#define  GPIO_PINCFG14_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG14_DEGTB_SHIFT))

#define  GPIO_PINCFG14_INTPOL_MASK 0xc0
#define  GPIO_PINCFG14_INTPOL_SHIFT 6
#define  GPIO_PINCFG14_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_INTPOL_MASK) >> GPIO_PINCFG14_INTPOL_SHIFT)
#define  GPIO_PINCFG14_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG14_INTPOL_SHIFT))

#define  GPIO_PINCFG14_INTMODE_MASK 0x20
#define  GPIO_PINCFG14_INTMODE_SHIFT 5
#define  GPIO_PINCFG14_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_INTMODE_MASK) >> GPIO_PINCFG14_INTMODE_SHIFT)
#define  GPIO_PINCFG14_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG14_INTMODE_SHIFT))

#define  GPIO_PINCFG14_SEMMODE_MASK 0x10
#define  GPIO_PINCFG14_SEMMODE_SHIFT 4
#define  GPIO_PINCFG14_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_SEMMODE_MASK) >> GPIO_PINCFG14_SEMMODE_SHIFT)
#define  GPIO_PINCFG14_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG14_SEMMODE_SHIFT))

#define  GPIO_PINCFG14_RESERVED3_MASK 0x8
#define  GPIO_PINCFG14_RESERVED3_SHIFT 3
#define  GPIO_PINCFG14_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_RESERVED3_MASK) >> GPIO_PINCFG14_RESERVED3_SHIFT)
#define  GPIO_PINCFG14_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG14_RESERVED3_SHIFT))

#define  GPIO_PINCFG14_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG14_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG14_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG14_FUNCSEL_MASK) >> GPIO_PINCFG14_FUNCSEL_SHIFT)
#define  GPIO_PINCFG14_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG14_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG14_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 16 of 32) (PinCfg15)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 16 of 32)*/
//====================================================================

#define  GPIO_PINCFG15_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG15_RESERVED1_SHIFT 20
#define  GPIO_PINCFG15_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_RESERVED1_MASK) >> GPIO_PINCFG15_RESERVED1_SHIFT)
#define  GPIO_PINCFG15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG15_RESERVED1_SHIFT))

#define  GPIO_PINCFG15_SLEW_MASK 0xc0000
#define  GPIO_PINCFG15_SLEW_SHIFT 18
#define  GPIO_PINCFG15_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_SLEW_MASK) >> GPIO_PINCFG15_SLEW_SHIFT)
#define  GPIO_PINCFG15_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG15_SLEW_SHIFT))

#define  GPIO_PINCFG15_PDEN_MASK 0x20000
#define  GPIO_PINCFG15_PDEN_SHIFT 17
#define  GPIO_PINCFG15_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_PDEN_MASK) >> GPIO_PINCFG15_PDEN_SHIFT)
#define  GPIO_PINCFG15_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG15_PDEN_SHIFT))

#define  GPIO_PINCFG15_PUEN_MASK 0x10000
#define  GPIO_PINCFG15_PUEN_SHIFT 16
#define  GPIO_PINCFG15_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_PUEN_MASK) >> GPIO_PINCFG15_PUEN_SHIFT)
#define  GPIO_PINCFG15_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG15_PUEN_SHIFT))

#define  GPIO_PINCFG15_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG15_RESERVED2_SHIFT 13
#define  GPIO_PINCFG15_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_RESERVED2_MASK) >> GPIO_PINCFG15_RESERVED2_SHIFT)
#define  GPIO_PINCFG15_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG15_RESERVED2_SHIFT))

#define  GPIO_PINCFG15_RAWALT_MASK 0x1000
#define  GPIO_PINCFG15_RAWALT_SHIFT 12
#define  GPIO_PINCFG15_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_RAWALT_MASK) >> GPIO_PINCFG15_RAWALT_SHIFT)
#define  GPIO_PINCFG15_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG15_RAWALT_SHIFT))

#define  GPIO_PINCFG15_DEGTB_MASK 0xf00
#define  GPIO_PINCFG15_DEGTB_SHIFT 8
#define  GPIO_PINCFG15_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_DEGTB_MASK) >> GPIO_PINCFG15_DEGTB_SHIFT)
#define  GPIO_PINCFG15_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG15_DEGTB_SHIFT))

#define  GPIO_PINCFG15_INTPOL_MASK 0xc0
#define  GPIO_PINCFG15_INTPOL_SHIFT 6
#define  GPIO_PINCFG15_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_INTPOL_MASK) >> GPIO_PINCFG15_INTPOL_SHIFT)
#define  GPIO_PINCFG15_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG15_INTPOL_SHIFT))

#define  GPIO_PINCFG15_INTMODE_MASK 0x20
#define  GPIO_PINCFG15_INTMODE_SHIFT 5
#define  GPIO_PINCFG15_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_INTMODE_MASK) >> GPIO_PINCFG15_INTMODE_SHIFT)
#define  GPIO_PINCFG15_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG15_INTMODE_SHIFT))

#define  GPIO_PINCFG15_SEMMODE_MASK 0x10
#define  GPIO_PINCFG15_SEMMODE_SHIFT 4
#define  GPIO_PINCFG15_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_SEMMODE_MASK) >> GPIO_PINCFG15_SEMMODE_SHIFT)
#define  GPIO_PINCFG15_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG15_SEMMODE_SHIFT))

#define  GPIO_PINCFG15_RESERVED3_MASK 0x8
#define  GPIO_PINCFG15_RESERVED3_SHIFT 3
#define  GPIO_PINCFG15_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_RESERVED3_MASK) >> GPIO_PINCFG15_RESERVED3_SHIFT)
#define  GPIO_PINCFG15_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG15_RESERVED3_SHIFT))

#define  GPIO_PINCFG15_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG15_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG15_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG15_FUNCSEL_MASK) >> GPIO_PINCFG15_FUNCSEL_SHIFT)
#define  GPIO_PINCFG15_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG15_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG15_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 17 of 32) (PinCfg16)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 17 of 32)*/
//====================================================================

#define  GPIO_PINCFG16_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG16_RESERVED1_SHIFT 20
#define  GPIO_PINCFG16_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_RESERVED1_MASK) >> GPIO_PINCFG16_RESERVED1_SHIFT)
#define  GPIO_PINCFG16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG16_RESERVED1_SHIFT))

#define  GPIO_PINCFG16_SLEW_MASK 0xc0000
#define  GPIO_PINCFG16_SLEW_SHIFT 18
#define  GPIO_PINCFG16_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_SLEW_MASK) >> GPIO_PINCFG16_SLEW_SHIFT)
#define  GPIO_PINCFG16_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG16_SLEW_SHIFT))

#define  GPIO_PINCFG16_PDEN_MASK 0x20000
#define  GPIO_PINCFG16_PDEN_SHIFT 17
#define  GPIO_PINCFG16_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_PDEN_MASK) >> GPIO_PINCFG16_PDEN_SHIFT)
#define  GPIO_PINCFG16_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG16_PDEN_SHIFT))

#define  GPIO_PINCFG16_PUEN_MASK 0x10000
#define  GPIO_PINCFG16_PUEN_SHIFT 16
#define  GPIO_PINCFG16_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_PUEN_MASK) >> GPIO_PINCFG16_PUEN_SHIFT)
#define  GPIO_PINCFG16_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG16_PUEN_SHIFT))

#define  GPIO_PINCFG16_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG16_RESERVED2_SHIFT 13
#define  GPIO_PINCFG16_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_RESERVED2_MASK) >> GPIO_PINCFG16_RESERVED2_SHIFT)
#define  GPIO_PINCFG16_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG16_RESERVED2_SHIFT))

#define  GPIO_PINCFG16_RAWALT_MASK 0x1000
#define  GPIO_PINCFG16_RAWALT_SHIFT 12
#define  GPIO_PINCFG16_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_RAWALT_MASK) >> GPIO_PINCFG16_RAWALT_SHIFT)
#define  GPIO_PINCFG16_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG16_RAWALT_SHIFT))

#define  GPIO_PINCFG16_DEGTB_MASK 0xf00
#define  GPIO_PINCFG16_DEGTB_SHIFT 8
#define  GPIO_PINCFG16_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_DEGTB_MASK) >> GPIO_PINCFG16_DEGTB_SHIFT)
#define  GPIO_PINCFG16_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG16_DEGTB_SHIFT))

#define  GPIO_PINCFG16_INTPOL_MASK 0xc0
#define  GPIO_PINCFG16_INTPOL_SHIFT 6
#define  GPIO_PINCFG16_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_INTPOL_MASK) >> GPIO_PINCFG16_INTPOL_SHIFT)
#define  GPIO_PINCFG16_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG16_INTPOL_SHIFT))

#define  GPIO_PINCFG16_INTMODE_MASK 0x20
#define  GPIO_PINCFG16_INTMODE_SHIFT 5
#define  GPIO_PINCFG16_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_INTMODE_MASK) >> GPIO_PINCFG16_INTMODE_SHIFT)
#define  GPIO_PINCFG16_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG16_INTMODE_SHIFT))

#define  GPIO_PINCFG16_SEMMODE_MASK 0x10
#define  GPIO_PINCFG16_SEMMODE_SHIFT 4
#define  GPIO_PINCFG16_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_SEMMODE_MASK) >> GPIO_PINCFG16_SEMMODE_SHIFT)
#define  GPIO_PINCFG16_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG16_SEMMODE_SHIFT))

#define  GPIO_PINCFG16_RESERVED3_MASK 0x8
#define  GPIO_PINCFG16_RESERVED3_SHIFT 3
#define  GPIO_PINCFG16_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_RESERVED3_MASK) >> GPIO_PINCFG16_RESERVED3_SHIFT)
#define  GPIO_PINCFG16_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG16_RESERVED3_SHIFT))

#define  GPIO_PINCFG16_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG16_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG16_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG16_FUNCSEL_MASK) >> GPIO_PINCFG16_FUNCSEL_SHIFT)
#define  GPIO_PINCFG16_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG16_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG16_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 18 of 32) (PinCfg17)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 18 of 32)*/
//====================================================================

#define  GPIO_PINCFG17_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG17_RESERVED1_SHIFT 20
#define  GPIO_PINCFG17_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_RESERVED1_MASK) >> GPIO_PINCFG17_RESERVED1_SHIFT)
#define  GPIO_PINCFG17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG17_RESERVED1_SHIFT))

#define  GPIO_PINCFG17_SLEW_MASK 0xc0000
#define  GPIO_PINCFG17_SLEW_SHIFT 18
#define  GPIO_PINCFG17_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_SLEW_MASK) >> GPIO_PINCFG17_SLEW_SHIFT)
#define  GPIO_PINCFG17_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG17_SLEW_SHIFT))

#define  GPIO_PINCFG17_PDEN_MASK 0x20000
#define  GPIO_PINCFG17_PDEN_SHIFT 17
#define  GPIO_PINCFG17_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_PDEN_MASK) >> GPIO_PINCFG17_PDEN_SHIFT)
#define  GPIO_PINCFG17_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG17_PDEN_SHIFT))

#define  GPIO_PINCFG17_PUEN_MASK 0x10000
#define  GPIO_PINCFG17_PUEN_SHIFT 16
#define  GPIO_PINCFG17_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_PUEN_MASK) >> GPIO_PINCFG17_PUEN_SHIFT)
#define  GPIO_PINCFG17_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG17_PUEN_SHIFT))

#define  GPIO_PINCFG17_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG17_RESERVED2_SHIFT 13
#define  GPIO_PINCFG17_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_RESERVED2_MASK) >> GPIO_PINCFG17_RESERVED2_SHIFT)
#define  GPIO_PINCFG17_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG17_RESERVED2_SHIFT))

#define  GPIO_PINCFG17_RAWALT_MASK 0x1000
#define  GPIO_PINCFG17_RAWALT_SHIFT 12
#define  GPIO_PINCFG17_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_RAWALT_MASK) >> GPIO_PINCFG17_RAWALT_SHIFT)
#define  GPIO_PINCFG17_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG17_RAWALT_SHIFT))

#define  GPIO_PINCFG17_DEGTB_MASK 0xf00
#define  GPIO_PINCFG17_DEGTB_SHIFT 8
#define  GPIO_PINCFG17_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_DEGTB_MASK) >> GPIO_PINCFG17_DEGTB_SHIFT)
#define  GPIO_PINCFG17_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG17_DEGTB_SHIFT))

#define  GPIO_PINCFG17_INTPOL_MASK 0xc0
#define  GPIO_PINCFG17_INTPOL_SHIFT 6
#define  GPIO_PINCFG17_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_INTPOL_MASK) >> GPIO_PINCFG17_INTPOL_SHIFT)
#define  GPIO_PINCFG17_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG17_INTPOL_SHIFT))

#define  GPIO_PINCFG17_INTMODE_MASK 0x20
#define  GPIO_PINCFG17_INTMODE_SHIFT 5
#define  GPIO_PINCFG17_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_INTMODE_MASK) >> GPIO_PINCFG17_INTMODE_SHIFT)
#define  GPIO_PINCFG17_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG17_INTMODE_SHIFT))

#define  GPIO_PINCFG17_SEMMODE_MASK 0x10
#define  GPIO_PINCFG17_SEMMODE_SHIFT 4
#define  GPIO_PINCFG17_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_SEMMODE_MASK) >> GPIO_PINCFG17_SEMMODE_SHIFT)
#define  GPIO_PINCFG17_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG17_SEMMODE_SHIFT))

#define  GPIO_PINCFG17_RESERVED3_MASK 0x8
#define  GPIO_PINCFG17_RESERVED3_SHIFT 3
#define  GPIO_PINCFG17_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_RESERVED3_MASK) >> GPIO_PINCFG17_RESERVED3_SHIFT)
#define  GPIO_PINCFG17_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG17_RESERVED3_SHIFT))

#define  GPIO_PINCFG17_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG17_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG17_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG17_FUNCSEL_MASK) >> GPIO_PINCFG17_FUNCSEL_SHIFT)
#define  GPIO_PINCFG17_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG17_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG17_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 19 of 32) (PinCfg18)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 19 of 32)*/
//====================================================================

#define  GPIO_PINCFG18_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG18_RESERVED1_SHIFT 20
#define  GPIO_PINCFG18_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_RESERVED1_MASK) >> GPIO_PINCFG18_RESERVED1_SHIFT)
#define  GPIO_PINCFG18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG18_RESERVED1_SHIFT))

#define  GPIO_PINCFG18_SLEW_MASK 0xc0000
#define  GPIO_PINCFG18_SLEW_SHIFT 18
#define  GPIO_PINCFG18_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_SLEW_MASK) >> GPIO_PINCFG18_SLEW_SHIFT)
#define  GPIO_PINCFG18_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG18_SLEW_SHIFT))

#define  GPIO_PINCFG18_PDEN_MASK 0x20000
#define  GPIO_PINCFG18_PDEN_SHIFT 17
#define  GPIO_PINCFG18_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_PDEN_MASK) >> GPIO_PINCFG18_PDEN_SHIFT)
#define  GPIO_PINCFG18_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG18_PDEN_SHIFT))

#define  GPIO_PINCFG18_PUEN_MASK 0x10000
#define  GPIO_PINCFG18_PUEN_SHIFT 16
#define  GPIO_PINCFG18_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_PUEN_MASK) >> GPIO_PINCFG18_PUEN_SHIFT)
#define  GPIO_PINCFG18_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG18_PUEN_SHIFT))

#define  GPIO_PINCFG18_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG18_RESERVED2_SHIFT 13
#define  GPIO_PINCFG18_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_RESERVED2_MASK) >> GPIO_PINCFG18_RESERVED2_SHIFT)
#define  GPIO_PINCFG18_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG18_RESERVED2_SHIFT))

#define  GPIO_PINCFG18_RAWALT_MASK 0x1000
#define  GPIO_PINCFG18_RAWALT_SHIFT 12
#define  GPIO_PINCFG18_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_RAWALT_MASK) >> GPIO_PINCFG18_RAWALT_SHIFT)
#define  GPIO_PINCFG18_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG18_RAWALT_SHIFT))

#define  GPIO_PINCFG18_DEGTB_MASK 0xf00
#define  GPIO_PINCFG18_DEGTB_SHIFT 8
#define  GPIO_PINCFG18_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_DEGTB_MASK) >> GPIO_PINCFG18_DEGTB_SHIFT)
#define  GPIO_PINCFG18_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG18_DEGTB_SHIFT))

#define  GPIO_PINCFG18_INTPOL_MASK 0xc0
#define  GPIO_PINCFG18_INTPOL_SHIFT 6
#define  GPIO_PINCFG18_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_INTPOL_MASK) >> GPIO_PINCFG18_INTPOL_SHIFT)
#define  GPIO_PINCFG18_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG18_INTPOL_SHIFT))

#define  GPIO_PINCFG18_INTMODE_MASK 0x20
#define  GPIO_PINCFG18_INTMODE_SHIFT 5
#define  GPIO_PINCFG18_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_INTMODE_MASK) >> GPIO_PINCFG18_INTMODE_SHIFT)
#define  GPIO_PINCFG18_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG18_INTMODE_SHIFT))

#define  GPIO_PINCFG18_SEMMODE_MASK 0x10
#define  GPIO_PINCFG18_SEMMODE_SHIFT 4
#define  GPIO_PINCFG18_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_SEMMODE_MASK) >> GPIO_PINCFG18_SEMMODE_SHIFT)
#define  GPIO_PINCFG18_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG18_SEMMODE_SHIFT))

#define  GPIO_PINCFG18_RESERVED3_MASK 0x8
#define  GPIO_PINCFG18_RESERVED3_SHIFT 3
#define  GPIO_PINCFG18_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_RESERVED3_MASK) >> GPIO_PINCFG18_RESERVED3_SHIFT)
#define  GPIO_PINCFG18_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG18_RESERVED3_SHIFT))

#define  GPIO_PINCFG18_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG18_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG18_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG18_FUNCSEL_MASK) >> GPIO_PINCFG18_FUNCSEL_SHIFT)
#define  GPIO_PINCFG18_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG18_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG18_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 20 of 32) (PinCfg19)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 20 of 32)*/
//====================================================================

#define  GPIO_PINCFG19_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG19_RESERVED1_SHIFT 20
#define  GPIO_PINCFG19_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_RESERVED1_MASK) >> GPIO_PINCFG19_RESERVED1_SHIFT)
#define  GPIO_PINCFG19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG19_RESERVED1_SHIFT))

#define  GPIO_PINCFG19_SLEW_MASK 0xc0000
#define  GPIO_PINCFG19_SLEW_SHIFT 18
#define  GPIO_PINCFG19_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_SLEW_MASK) >> GPIO_PINCFG19_SLEW_SHIFT)
#define  GPIO_PINCFG19_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG19_SLEW_SHIFT))

#define  GPIO_PINCFG19_PDEN_MASK 0x20000
#define  GPIO_PINCFG19_PDEN_SHIFT 17
#define  GPIO_PINCFG19_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_PDEN_MASK) >> GPIO_PINCFG19_PDEN_SHIFT)
#define  GPIO_PINCFG19_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG19_PDEN_SHIFT))

#define  GPIO_PINCFG19_PUEN_MASK 0x10000
#define  GPIO_PINCFG19_PUEN_SHIFT 16
#define  GPIO_PINCFG19_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_PUEN_MASK) >> GPIO_PINCFG19_PUEN_SHIFT)
#define  GPIO_PINCFG19_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG19_PUEN_SHIFT))

#define  GPIO_PINCFG19_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG19_RESERVED2_SHIFT 13
#define  GPIO_PINCFG19_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_RESERVED2_MASK) >> GPIO_PINCFG19_RESERVED2_SHIFT)
#define  GPIO_PINCFG19_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG19_RESERVED2_SHIFT))

#define  GPIO_PINCFG19_RAWALT_MASK 0x1000
#define  GPIO_PINCFG19_RAWALT_SHIFT 12
#define  GPIO_PINCFG19_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_RAWALT_MASK) >> GPIO_PINCFG19_RAWALT_SHIFT)
#define  GPIO_PINCFG19_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG19_RAWALT_SHIFT))

#define  GPIO_PINCFG19_DEGTB_MASK 0xf00
#define  GPIO_PINCFG19_DEGTB_SHIFT 8
#define  GPIO_PINCFG19_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_DEGTB_MASK) >> GPIO_PINCFG19_DEGTB_SHIFT)
#define  GPIO_PINCFG19_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG19_DEGTB_SHIFT))

#define  GPIO_PINCFG19_INTPOL_MASK 0xc0
#define  GPIO_PINCFG19_INTPOL_SHIFT 6
#define  GPIO_PINCFG19_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_INTPOL_MASK) >> GPIO_PINCFG19_INTPOL_SHIFT)
#define  GPIO_PINCFG19_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG19_INTPOL_SHIFT))

#define  GPIO_PINCFG19_INTMODE_MASK 0x20
#define  GPIO_PINCFG19_INTMODE_SHIFT 5
#define  GPIO_PINCFG19_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_INTMODE_MASK) >> GPIO_PINCFG19_INTMODE_SHIFT)
#define  GPIO_PINCFG19_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG19_INTMODE_SHIFT))

#define  GPIO_PINCFG19_SEMMODE_MASK 0x10
#define  GPIO_PINCFG19_SEMMODE_SHIFT 4
#define  GPIO_PINCFG19_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_SEMMODE_MASK) >> GPIO_PINCFG19_SEMMODE_SHIFT)
#define  GPIO_PINCFG19_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG19_SEMMODE_SHIFT))

#define  GPIO_PINCFG19_RESERVED3_MASK 0x8
#define  GPIO_PINCFG19_RESERVED3_SHIFT 3
#define  GPIO_PINCFG19_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_RESERVED3_MASK) >> GPIO_PINCFG19_RESERVED3_SHIFT)
#define  GPIO_PINCFG19_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG19_RESERVED3_SHIFT))

#define  GPIO_PINCFG19_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG19_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG19_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG19_FUNCSEL_MASK) >> GPIO_PINCFG19_FUNCSEL_SHIFT)
#define  GPIO_PINCFG19_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG19_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG19_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 21 of 32) (PinCfg20)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 21 of 32)*/
//====================================================================

#define  GPIO_PINCFG20_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG20_RESERVED1_SHIFT 20
#define  GPIO_PINCFG20_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_RESERVED1_MASK) >> GPIO_PINCFG20_RESERVED1_SHIFT)
#define  GPIO_PINCFG20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG20_RESERVED1_SHIFT))

#define  GPIO_PINCFG20_SLEW_MASK 0xc0000
#define  GPIO_PINCFG20_SLEW_SHIFT 18
#define  GPIO_PINCFG20_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_SLEW_MASK) >> GPIO_PINCFG20_SLEW_SHIFT)
#define  GPIO_PINCFG20_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG20_SLEW_SHIFT))

#define  GPIO_PINCFG20_PDEN_MASK 0x20000
#define  GPIO_PINCFG20_PDEN_SHIFT 17
#define  GPIO_PINCFG20_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_PDEN_MASK) >> GPIO_PINCFG20_PDEN_SHIFT)
#define  GPIO_PINCFG20_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG20_PDEN_SHIFT))

#define  GPIO_PINCFG20_PUEN_MASK 0x10000
#define  GPIO_PINCFG20_PUEN_SHIFT 16
#define  GPIO_PINCFG20_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_PUEN_MASK) >> GPIO_PINCFG20_PUEN_SHIFT)
#define  GPIO_PINCFG20_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG20_PUEN_SHIFT))

#define  GPIO_PINCFG20_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG20_RESERVED2_SHIFT 13
#define  GPIO_PINCFG20_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_RESERVED2_MASK) >> GPIO_PINCFG20_RESERVED2_SHIFT)
#define  GPIO_PINCFG20_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG20_RESERVED2_SHIFT))

#define  GPIO_PINCFG20_RAWALT_MASK 0x1000
#define  GPIO_PINCFG20_RAWALT_SHIFT 12
#define  GPIO_PINCFG20_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_RAWALT_MASK) >> GPIO_PINCFG20_RAWALT_SHIFT)
#define  GPIO_PINCFG20_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG20_RAWALT_SHIFT))

#define  GPIO_PINCFG20_DEGTB_MASK 0xf00
#define  GPIO_PINCFG20_DEGTB_SHIFT 8
#define  GPIO_PINCFG20_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_DEGTB_MASK) >> GPIO_PINCFG20_DEGTB_SHIFT)
#define  GPIO_PINCFG20_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG20_DEGTB_SHIFT))

#define  GPIO_PINCFG20_INTPOL_MASK 0xc0
#define  GPIO_PINCFG20_INTPOL_SHIFT 6
#define  GPIO_PINCFG20_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_INTPOL_MASK) >> GPIO_PINCFG20_INTPOL_SHIFT)
#define  GPIO_PINCFG20_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG20_INTPOL_SHIFT))

#define  GPIO_PINCFG20_INTMODE_MASK 0x20
#define  GPIO_PINCFG20_INTMODE_SHIFT 5
#define  GPIO_PINCFG20_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_INTMODE_MASK) >> GPIO_PINCFG20_INTMODE_SHIFT)
#define  GPIO_PINCFG20_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG20_INTMODE_SHIFT))

#define  GPIO_PINCFG20_SEMMODE_MASK 0x10
#define  GPIO_PINCFG20_SEMMODE_SHIFT 4
#define  GPIO_PINCFG20_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_SEMMODE_MASK) >> GPIO_PINCFG20_SEMMODE_SHIFT)
#define  GPIO_PINCFG20_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG20_SEMMODE_SHIFT))

#define  GPIO_PINCFG20_RESERVED3_MASK 0x8
#define  GPIO_PINCFG20_RESERVED3_SHIFT 3
#define  GPIO_PINCFG20_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_RESERVED3_MASK) >> GPIO_PINCFG20_RESERVED3_SHIFT)
#define  GPIO_PINCFG20_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG20_RESERVED3_SHIFT))

#define  GPIO_PINCFG20_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG20_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG20_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG20_FUNCSEL_MASK) >> GPIO_PINCFG20_FUNCSEL_SHIFT)
#define  GPIO_PINCFG20_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG20_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG20_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 22 of 32) (PinCfg21)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 22 of 32)*/
//====================================================================

#define  GPIO_PINCFG21_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG21_RESERVED1_SHIFT 20
#define  GPIO_PINCFG21_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_RESERVED1_MASK) >> GPIO_PINCFG21_RESERVED1_SHIFT)
#define  GPIO_PINCFG21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG21_RESERVED1_SHIFT))

#define  GPIO_PINCFG21_SLEW_MASK 0xc0000
#define  GPIO_PINCFG21_SLEW_SHIFT 18
#define  GPIO_PINCFG21_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_SLEW_MASK) >> GPIO_PINCFG21_SLEW_SHIFT)
#define  GPIO_PINCFG21_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG21_SLEW_SHIFT))

#define  GPIO_PINCFG21_PDEN_MASK 0x20000
#define  GPIO_PINCFG21_PDEN_SHIFT 17
#define  GPIO_PINCFG21_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_PDEN_MASK) >> GPIO_PINCFG21_PDEN_SHIFT)
#define  GPIO_PINCFG21_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG21_PDEN_SHIFT))

#define  GPIO_PINCFG21_PUEN_MASK 0x10000
#define  GPIO_PINCFG21_PUEN_SHIFT 16
#define  GPIO_PINCFG21_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_PUEN_MASK) >> GPIO_PINCFG21_PUEN_SHIFT)
#define  GPIO_PINCFG21_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG21_PUEN_SHIFT))

#define  GPIO_PINCFG21_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG21_RESERVED2_SHIFT 13
#define  GPIO_PINCFG21_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_RESERVED2_MASK) >> GPIO_PINCFG21_RESERVED2_SHIFT)
#define  GPIO_PINCFG21_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG21_RESERVED2_SHIFT))

#define  GPIO_PINCFG21_RAWALT_MASK 0x1000
#define  GPIO_PINCFG21_RAWALT_SHIFT 12
#define  GPIO_PINCFG21_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_RAWALT_MASK) >> GPIO_PINCFG21_RAWALT_SHIFT)
#define  GPIO_PINCFG21_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG21_RAWALT_SHIFT))

#define  GPIO_PINCFG21_DEGTB_MASK 0xf00
#define  GPIO_PINCFG21_DEGTB_SHIFT 8
#define  GPIO_PINCFG21_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_DEGTB_MASK) >> GPIO_PINCFG21_DEGTB_SHIFT)
#define  GPIO_PINCFG21_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG21_DEGTB_SHIFT))

#define  GPIO_PINCFG21_INTPOL_MASK 0xc0
#define  GPIO_PINCFG21_INTPOL_SHIFT 6
#define  GPIO_PINCFG21_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_INTPOL_MASK) >> GPIO_PINCFG21_INTPOL_SHIFT)
#define  GPIO_PINCFG21_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG21_INTPOL_SHIFT))

#define  GPIO_PINCFG21_INTMODE_MASK 0x20
#define  GPIO_PINCFG21_INTMODE_SHIFT 5
#define  GPIO_PINCFG21_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_INTMODE_MASK) >> GPIO_PINCFG21_INTMODE_SHIFT)
#define  GPIO_PINCFG21_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG21_INTMODE_SHIFT))

#define  GPIO_PINCFG21_SEMMODE_MASK 0x10
#define  GPIO_PINCFG21_SEMMODE_SHIFT 4
#define  GPIO_PINCFG21_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_SEMMODE_MASK) >> GPIO_PINCFG21_SEMMODE_SHIFT)
#define  GPIO_PINCFG21_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG21_SEMMODE_SHIFT))

#define  GPIO_PINCFG21_RESERVED3_MASK 0x8
#define  GPIO_PINCFG21_RESERVED3_SHIFT 3
#define  GPIO_PINCFG21_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_RESERVED3_MASK) >> GPIO_PINCFG21_RESERVED3_SHIFT)
#define  GPIO_PINCFG21_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG21_RESERVED3_SHIFT))

#define  GPIO_PINCFG21_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG21_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG21_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG21_FUNCSEL_MASK) >> GPIO_PINCFG21_FUNCSEL_SHIFT)
#define  GPIO_PINCFG21_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG21_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG21_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 23 of 32) (PinCfg22)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 23 of 32)*/
//====================================================================

#define  GPIO_PINCFG22_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG22_RESERVED1_SHIFT 20
#define  GPIO_PINCFG22_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_RESERVED1_MASK) >> GPIO_PINCFG22_RESERVED1_SHIFT)
#define  GPIO_PINCFG22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG22_RESERVED1_SHIFT))

#define  GPIO_PINCFG22_SLEW_MASK 0xc0000
#define  GPIO_PINCFG22_SLEW_SHIFT 18
#define  GPIO_PINCFG22_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_SLEW_MASK) >> GPIO_PINCFG22_SLEW_SHIFT)
#define  GPIO_PINCFG22_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG22_SLEW_SHIFT))

#define  GPIO_PINCFG22_PDEN_MASK 0x20000
#define  GPIO_PINCFG22_PDEN_SHIFT 17
#define  GPIO_PINCFG22_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_PDEN_MASK) >> GPIO_PINCFG22_PDEN_SHIFT)
#define  GPIO_PINCFG22_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG22_PDEN_SHIFT))

#define  GPIO_PINCFG22_PUEN_MASK 0x10000
#define  GPIO_PINCFG22_PUEN_SHIFT 16
#define  GPIO_PINCFG22_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_PUEN_MASK) >> GPIO_PINCFG22_PUEN_SHIFT)
#define  GPIO_PINCFG22_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG22_PUEN_SHIFT))

#define  GPIO_PINCFG22_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG22_RESERVED2_SHIFT 13
#define  GPIO_PINCFG22_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_RESERVED2_MASK) >> GPIO_PINCFG22_RESERVED2_SHIFT)
#define  GPIO_PINCFG22_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG22_RESERVED2_SHIFT))

#define  GPIO_PINCFG22_RAWALT_MASK 0x1000
#define  GPIO_PINCFG22_RAWALT_SHIFT 12
#define  GPIO_PINCFG22_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_RAWALT_MASK) >> GPIO_PINCFG22_RAWALT_SHIFT)
#define  GPIO_PINCFG22_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG22_RAWALT_SHIFT))

#define  GPIO_PINCFG22_DEGTB_MASK 0xf00
#define  GPIO_PINCFG22_DEGTB_SHIFT 8
#define  GPIO_PINCFG22_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_DEGTB_MASK) >> GPIO_PINCFG22_DEGTB_SHIFT)
#define  GPIO_PINCFG22_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG22_DEGTB_SHIFT))

#define  GPIO_PINCFG22_INTPOL_MASK 0xc0
#define  GPIO_PINCFG22_INTPOL_SHIFT 6
#define  GPIO_PINCFG22_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_INTPOL_MASK) >> GPIO_PINCFG22_INTPOL_SHIFT)
#define  GPIO_PINCFG22_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG22_INTPOL_SHIFT))

#define  GPIO_PINCFG22_INTMODE_MASK 0x20
#define  GPIO_PINCFG22_INTMODE_SHIFT 5
#define  GPIO_PINCFG22_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_INTMODE_MASK) >> GPIO_PINCFG22_INTMODE_SHIFT)
#define  GPIO_PINCFG22_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG22_INTMODE_SHIFT))

#define  GPIO_PINCFG22_SEMMODE_MASK 0x10
#define  GPIO_PINCFG22_SEMMODE_SHIFT 4
#define  GPIO_PINCFG22_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_SEMMODE_MASK) >> GPIO_PINCFG22_SEMMODE_SHIFT)
#define  GPIO_PINCFG22_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG22_SEMMODE_SHIFT))

#define  GPIO_PINCFG22_RESERVED3_MASK 0x8
#define  GPIO_PINCFG22_RESERVED3_SHIFT 3
#define  GPIO_PINCFG22_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_RESERVED3_MASK) >> GPIO_PINCFG22_RESERVED3_SHIFT)
#define  GPIO_PINCFG22_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG22_RESERVED3_SHIFT))

#define  GPIO_PINCFG22_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG22_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG22_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG22_FUNCSEL_MASK) >> GPIO_PINCFG22_FUNCSEL_SHIFT)
#define  GPIO_PINCFG22_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG22_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG22_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 24 of 32) (PinCfg23)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 24 of 32)*/
//====================================================================

#define  GPIO_PINCFG23_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG23_RESERVED1_SHIFT 20
#define  GPIO_PINCFG23_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_RESERVED1_MASK) >> GPIO_PINCFG23_RESERVED1_SHIFT)
#define  GPIO_PINCFG23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG23_RESERVED1_SHIFT))

#define  GPIO_PINCFG23_SLEW_MASK 0xc0000
#define  GPIO_PINCFG23_SLEW_SHIFT 18
#define  GPIO_PINCFG23_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_SLEW_MASK) >> GPIO_PINCFG23_SLEW_SHIFT)
#define  GPIO_PINCFG23_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG23_SLEW_SHIFT))

#define  GPIO_PINCFG23_PDEN_MASK 0x20000
#define  GPIO_PINCFG23_PDEN_SHIFT 17
#define  GPIO_PINCFG23_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_PDEN_MASK) >> GPIO_PINCFG23_PDEN_SHIFT)
#define  GPIO_PINCFG23_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG23_PDEN_SHIFT))

#define  GPIO_PINCFG23_PUEN_MASK 0x10000
#define  GPIO_PINCFG23_PUEN_SHIFT 16
#define  GPIO_PINCFG23_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_PUEN_MASK) >> GPIO_PINCFG23_PUEN_SHIFT)
#define  GPIO_PINCFG23_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG23_PUEN_SHIFT))

#define  GPIO_PINCFG23_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG23_RESERVED2_SHIFT 13
#define  GPIO_PINCFG23_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_RESERVED2_MASK) >> GPIO_PINCFG23_RESERVED2_SHIFT)
#define  GPIO_PINCFG23_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG23_RESERVED2_SHIFT))

#define  GPIO_PINCFG23_RAWALT_MASK 0x1000
#define  GPIO_PINCFG23_RAWALT_SHIFT 12
#define  GPIO_PINCFG23_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_RAWALT_MASK) >> GPIO_PINCFG23_RAWALT_SHIFT)
#define  GPIO_PINCFG23_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG23_RAWALT_SHIFT))

#define  GPIO_PINCFG23_DEGTB_MASK 0xf00
#define  GPIO_PINCFG23_DEGTB_SHIFT 8
#define  GPIO_PINCFG23_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_DEGTB_MASK) >> GPIO_PINCFG23_DEGTB_SHIFT)
#define  GPIO_PINCFG23_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG23_DEGTB_SHIFT))

#define  GPIO_PINCFG23_INTPOL_MASK 0xc0
#define  GPIO_PINCFG23_INTPOL_SHIFT 6
#define  GPIO_PINCFG23_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_INTPOL_MASK) >> GPIO_PINCFG23_INTPOL_SHIFT)
#define  GPIO_PINCFG23_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG23_INTPOL_SHIFT))

#define  GPIO_PINCFG23_INTMODE_MASK 0x20
#define  GPIO_PINCFG23_INTMODE_SHIFT 5
#define  GPIO_PINCFG23_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_INTMODE_MASK) >> GPIO_PINCFG23_INTMODE_SHIFT)
#define  GPIO_PINCFG23_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG23_INTMODE_SHIFT))

#define  GPIO_PINCFG23_SEMMODE_MASK 0x10
#define  GPIO_PINCFG23_SEMMODE_SHIFT 4
#define  GPIO_PINCFG23_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_SEMMODE_MASK) >> GPIO_PINCFG23_SEMMODE_SHIFT)
#define  GPIO_PINCFG23_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG23_SEMMODE_SHIFT))

#define  GPIO_PINCFG23_RESERVED3_MASK 0x8
#define  GPIO_PINCFG23_RESERVED3_SHIFT 3
#define  GPIO_PINCFG23_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_RESERVED3_MASK) >> GPIO_PINCFG23_RESERVED3_SHIFT)
#define  GPIO_PINCFG23_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG23_RESERVED3_SHIFT))

#define  GPIO_PINCFG23_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG23_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG23_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG23_FUNCSEL_MASK) >> GPIO_PINCFG23_FUNCSEL_SHIFT)
#define  GPIO_PINCFG23_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG23_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG23_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 25 of 32) (PinCfg24)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 25 of 32)*/
//====================================================================

#define  GPIO_PINCFG24_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG24_RESERVED1_SHIFT 20
#define  GPIO_PINCFG24_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_RESERVED1_MASK) >> GPIO_PINCFG24_RESERVED1_SHIFT)
#define  GPIO_PINCFG24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG24_RESERVED1_SHIFT))

#define  GPIO_PINCFG24_SLEW_MASK 0xc0000
#define  GPIO_PINCFG24_SLEW_SHIFT 18
#define  GPIO_PINCFG24_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_SLEW_MASK) >> GPIO_PINCFG24_SLEW_SHIFT)
#define  GPIO_PINCFG24_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG24_SLEW_SHIFT))

#define  GPIO_PINCFG24_PDEN_MASK 0x20000
#define  GPIO_PINCFG24_PDEN_SHIFT 17
#define  GPIO_PINCFG24_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_PDEN_MASK) >> GPIO_PINCFG24_PDEN_SHIFT)
#define  GPIO_PINCFG24_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG24_PDEN_SHIFT))

#define  GPIO_PINCFG24_PUEN_MASK 0x10000
#define  GPIO_PINCFG24_PUEN_SHIFT 16
#define  GPIO_PINCFG24_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_PUEN_MASK) >> GPIO_PINCFG24_PUEN_SHIFT)
#define  GPIO_PINCFG24_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG24_PUEN_SHIFT))

#define  GPIO_PINCFG24_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG24_RESERVED2_SHIFT 13
#define  GPIO_PINCFG24_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_RESERVED2_MASK) >> GPIO_PINCFG24_RESERVED2_SHIFT)
#define  GPIO_PINCFG24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG24_RESERVED2_SHIFT))

#define  GPIO_PINCFG24_RAWALT_MASK 0x1000
#define  GPIO_PINCFG24_RAWALT_SHIFT 12
#define  GPIO_PINCFG24_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_RAWALT_MASK) >> GPIO_PINCFG24_RAWALT_SHIFT)
#define  GPIO_PINCFG24_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG24_RAWALT_SHIFT))

#define  GPIO_PINCFG24_DEGTB_MASK 0xf00
#define  GPIO_PINCFG24_DEGTB_SHIFT 8
#define  GPIO_PINCFG24_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_DEGTB_MASK) >> GPIO_PINCFG24_DEGTB_SHIFT)
#define  GPIO_PINCFG24_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG24_DEGTB_SHIFT))

#define  GPIO_PINCFG24_INTPOL_MASK 0xc0
#define  GPIO_PINCFG24_INTPOL_SHIFT 6
#define  GPIO_PINCFG24_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_INTPOL_MASK) >> GPIO_PINCFG24_INTPOL_SHIFT)
#define  GPIO_PINCFG24_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG24_INTPOL_SHIFT))

#define  GPIO_PINCFG24_INTMODE_MASK 0x20
#define  GPIO_PINCFG24_INTMODE_SHIFT 5
#define  GPIO_PINCFG24_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_INTMODE_MASK) >> GPIO_PINCFG24_INTMODE_SHIFT)
#define  GPIO_PINCFG24_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG24_INTMODE_SHIFT))

#define  GPIO_PINCFG24_SEMMODE_MASK 0x10
#define  GPIO_PINCFG24_SEMMODE_SHIFT 4
#define  GPIO_PINCFG24_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_SEMMODE_MASK) >> GPIO_PINCFG24_SEMMODE_SHIFT)
#define  GPIO_PINCFG24_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG24_SEMMODE_SHIFT))

#define  GPIO_PINCFG24_RESERVED3_MASK 0x8
#define  GPIO_PINCFG24_RESERVED3_SHIFT 3
#define  GPIO_PINCFG24_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_RESERVED3_MASK) >> GPIO_PINCFG24_RESERVED3_SHIFT)
#define  GPIO_PINCFG24_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG24_RESERVED3_SHIFT))

#define  GPIO_PINCFG24_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG24_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG24_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG24_FUNCSEL_MASK) >> GPIO_PINCFG24_FUNCSEL_SHIFT)
#define  GPIO_PINCFG24_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG24_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG24_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 26 of 32) (PinCfg25)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 26 of 32)*/
//====================================================================

#define  GPIO_PINCFG25_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG25_RESERVED1_SHIFT 20
#define  GPIO_PINCFG25_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_RESERVED1_MASK) >> GPIO_PINCFG25_RESERVED1_SHIFT)
#define  GPIO_PINCFG25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG25_RESERVED1_SHIFT))

#define  GPIO_PINCFG25_SLEW_MASK 0xc0000
#define  GPIO_PINCFG25_SLEW_SHIFT 18
#define  GPIO_PINCFG25_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_SLEW_MASK) >> GPIO_PINCFG25_SLEW_SHIFT)
#define  GPIO_PINCFG25_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG25_SLEW_SHIFT))

#define  GPIO_PINCFG25_PDEN_MASK 0x20000
#define  GPIO_PINCFG25_PDEN_SHIFT 17
#define  GPIO_PINCFG25_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_PDEN_MASK) >> GPIO_PINCFG25_PDEN_SHIFT)
#define  GPIO_PINCFG25_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG25_PDEN_SHIFT))

#define  GPIO_PINCFG25_PUEN_MASK 0x10000
#define  GPIO_PINCFG25_PUEN_SHIFT 16
#define  GPIO_PINCFG25_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_PUEN_MASK) >> GPIO_PINCFG25_PUEN_SHIFT)
#define  GPIO_PINCFG25_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG25_PUEN_SHIFT))

#define  GPIO_PINCFG25_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG25_RESERVED2_SHIFT 13
#define  GPIO_PINCFG25_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_RESERVED2_MASK) >> GPIO_PINCFG25_RESERVED2_SHIFT)
#define  GPIO_PINCFG25_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG25_RESERVED2_SHIFT))

#define  GPIO_PINCFG25_RAWALT_MASK 0x1000
#define  GPIO_PINCFG25_RAWALT_SHIFT 12
#define  GPIO_PINCFG25_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_RAWALT_MASK) >> GPIO_PINCFG25_RAWALT_SHIFT)
#define  GPIO_PINCFG25_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG25_RAWALT_SHIFT))

#define  GPIO_PINCFG25_DEGTB_MASK 0xf00
#define  GPIO_PINCFG25_DEGTB_SHIFT 8
#define  GPIO_PINCFG25_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_DEGTB_MASK) >> GPIO_PINCFG25_DEGTB_SHIFT)
#define  GPIO_PINCFG25_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG25_DEGTB_SHIFT))

#define  GPIO_PINCFG25_INTPOL_MASK 0xc0
#define  GPIO_PINCFG25_INTPOL_SHIFT 6
#define  GPIO_PINCFG25_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_INTPOL_MASK) >> GPIO_PINCFG25_INTPOL_SHIFT)
#define  GPIO_PINCFG25_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG25_INTPOL_SHIFT))

#define  GPIO_PINCFG25_INTMODE_MASK 0x20
#define  GPIO_PINCFG25_INTMODE_SHIFT 5
#define  GPIO_PINCFG25_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_INTMODE_MASK) >> GPIO_PINCFG25_INTMODE_SHIFT)
#define  GPIO_PINCFG25_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG25_INTMODE_SHIFT))

#define  GPIO_PINCFG25_SEMMODE_MASK 0x10
#define  GPIO_PINCFG25_SEMMODE_SHIFT 4
#define  GPIO_PINCFG25_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_SEMMODE_MASK) >> GPIO_PINCFG25_SEMMODE_SHIFT)
#define  GPIO_PINCFG25_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG25_SEMMODE_SHIFT))

#define  GPIO_PINCFG25_RESERVED3_MASK 0x8
#define  GPIO_PINCFG25_RESERVED3_SHIFT 3
#define  GPIO_PINCFG25_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_RESERVED3_MASK) >> GPIO_PINCFG25_RESERVED3_SHIFT)
#define  GPIO_PINCFG25_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG25_RESERVED3_SHIFT))

#define  GPIO_PINCFG25_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG25_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG25_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG25_FUNCSEL_MASK) >> GPIO_PINCFG25_FUNCSEL_SHIFT)
#define  GPIO_PINCFG25_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG25_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG25_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 27 of 32) (PinCfg26)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 27 of 32)*/
//====================================================================

#define  GPIO_PINCFG26_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG26_RESERVED1_SHIFT 20
#define  GPIO_PINCFG26_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_RESERVED1_MASK) >> GPIO_PINCFG26_RESERVED1_SHIFT)
#define  GPIO_PINCFG26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG26_RESERVED1_SHIFT))

#define  GPIO_PINCFG26_SLEW_MASK 0xc0000
#define  GPIO_PINCFG26_SLEW_SHIFT 18
#define  GPIO_PINCFG26_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_SLEW_MASK) >> GPIO_PINCFG26_SLEW_SHIFT)
#define  GPIO_PINCFG26_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG26_SLEW_SHIFT))

#define  GPIO_PINCFG26_PDEN_MASK 0x20000
#define  GPIO_PINCFG26_PDEN_SHIFT 17
#define  GPIO_PINCFG26_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_PDEN_MASK) >> GPIO_PINCFG26_PDEN_SHIFT)
#define  GPIO_PINCFG26_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG26_PDEN_SHIFT))

#define  GPIO_PINCFG26_PUEN_MASK 0x10000
#define  GPIO_PINCFG26_PUEN_SHIFT 16
#define  GPIO_PINCFG26_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_PUEN_MASK) >> GPIO_PINCFG26_PUEN_SHIFT)
#define  GPIO_PINCFG26_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG26_PUEN_SHIFT))

#define  GPIO_PINCFG26_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG26_RESERVED2_SHIFT 13
#define  GPIO_PINCFG26_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_RESERVED2_MASK) >> GPIO_PINCFG26_RESERVED2_SHIFT)
#define  GPIO_PINCFG26_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG26_RESERVED2_SHIFT))

#define  GPIO_PINCFG26_RAWALT_MASK 0x1000
#define  GPIO_PINCFG26_RAWALT_SHIFT 12
#define  GPIO_PINCFG26_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_RAWALT_MASK) >> GPIO_PINCFG26_RAWALT_SHIFT)
#define  GPIO_PINCFG26_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG26_RAWALT_SHIFT))

#define  GPIO_PINCFG26_DEGTB_MASK 0xf00
#define  GPIO_PINCFG26_DEGTB_SHIFT 8
#define  GPIO_PINCFG26_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_DEGTB_MASK) >> GPIO_PINCFG26_DEGTB_SHIFT)
#define  GPIO_PINCFG26_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG26_DEGTB_SHIFT))

#define  GPIO_PINCFG26_INTPOL_MASK 0xc0
#define  GPIO_PINCFG26_INTPOL_SHIFT 6
#define  GPIO_PINCFG26_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_INTPOL_MASK) >> GPIO_PINCFG26_INTPOL_SHIFT)
#define  GPIO_PINCFG26_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG26_INTPOL_SHIFT))

#define  GPIO_PINCFG26_INTMODE_MASK 0x20
#define  GPIO_PINCFG26_INTMODE_SHIFT 5
#define  GPIO_PINCFG26_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_INTMODE_MASK) >> GPIO_PINCFG26_INTMODE_SHIFT)
#define  GPIO_PINCFG26_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG26_INTMODE_SHIFT))

#define  GPIO_PINCFG26_SEMMODE_MASK 0x10
#define  GPIO_PINCFG26_SEMMODE_SHIFT 4
#define  GPIO_PINCFG26_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_SEMMODE_MASK) >> GPIO_PINCFG26_SEMMODE_SHIFT)
#define  GPIO_PINCFG26_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG26_SEMMODE_SHIFT))

#define  GPIO_PINCFG26_RESERVED3_MASK 0x8
#define  GPIO_PINCFG26_RESERVED3_SHIFT 3
#define  GPIO_PINCFG26_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_RESERVED3_MASK) >> GPIO_PINCFG26_RESERVED3_SHIFT)
#define  GPIO_PINCFG26_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG26_RESERVED3_SHIFT))

#define  GPIO_PINCFG26_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG26_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG26_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG26_FUNCSEL_MASK) >> GPIO_PINCFG26_FUNCSEL_SHIFT)
#define  GPIO_PINCFG26_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG26_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG26_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 28 of 32) (PinCfg27)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 28 of 32)*/
//====================================================================

#define  GPIO_PINCFG27_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG27_RESERVED1_SHIFT 20
#define  GPIO_PINCFG27_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_RESERVED1_MASK) >> GPIO_PINCFG27_RESERVED1_SHIFT)
#define  GPIO_PINCFG27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG27_RESERVED1_SHIFT))

#define  GPIO_PINCFG27_SLEW_MASK 0xc0000
#define  GPIO_PINCFG27_SLEW_SHIFT 18
#define  GPIO_PINCFG27_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_SLEW_MASK) >> GPIO_PINCFG27_SLEW_SHIFT)
#define  GPIO_PINCFG27_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG27_SLEW_SHIFT))

#define  GPIO_PINCFG27_PDEN_MASK 0x20000
#define  GPIO_PINCFG27_PDEN_SHIFT 17
#define  GPIO_PINCFG27_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_PDEN_MASK) >> GPIO_PINCFG27_PDEN_SHIFT)
#define  GPIO_PINCFG27_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG27_PDEN_SHIFT))

#define  GPIO_PINCFG27_PUEN_MASK 0x10000
#define  GPIO_PINCFG27_PUEN_SHIFT 16
#define  GPIO_PINCFG27_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_PUEN_MASK) >> GPIO_PINCFG27_PUEN_SHIFT)
#define  GPIO_PINCFG27_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG27_PUEN_SHIFT))

#define  GPIO_PINCFG27_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG27_RESERVED2_SHIFT 13
#define  GPIO_PINCFG27_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_RESERVED2_MASK) >> GPIO_PINCFG27_RESERVED2_SHIFT)
#define  GPIO_PINCFG27_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG27_RESERVED2_SHIFT))

#define  GPIO_PINCFG27_RAWALT_MASK 0x1000
#define  GPIO_PINCFG27_RAWALT_SHIFT 12
#define  GPIO_PINCFG27_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_RAWALT_MASK) >> GPIO_PINCFG27_RAWALT_SHIFT)
#define  GPIO_PINCFG27_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG27_RAWALT_SHIFT))

#define  GPIO_PINCFG27_DEGTB_MASK 0xf00
#define  GPIO_PINCFG27_DEGTB_SHIFT 8
#define  GPIO_PINCFG27_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_DEGTB_MASK) >> GPIO_PINCFG27_DEGTB_SHIFT)
#define  GPIO_PINCFG27_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG27_DEGTB_SHIFT))

#define  GPIO_PINCFG27_INTPOL_MASK 0xc0
#define  GPIO_PINCFG27_INTPOL_SHIFT 6
#define  GPIO_PINCFG27_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_INTPOL_MASK) >> GPIO_PINCFG27_INTPOL_SHIFT)
#define  GPIO_PINCFG27_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG27_INTPOL_SHIFT))

#define  GPIO_PINCFG27_INTMODE_MASK 0x20
#define  GPIO_PINCFG27_INTMODE_SHIFT 5
#define  GPIO_PINCFG27_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_INTMODE_MASK) >> GPIO_PINCFG27_INTMODE_SHIFT)
#define  GPIO_PINCFG27_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG27_INTMODE_SHIFT))

#define  GPIO_PINCFG27_SEMMODE_MASK 0x10
#define  GPIO_PINCFG27_SEMMODE_SHIFT 4
#define  GPIO_PINCFG27_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_SEMMODE_MASK) >> GPIO_PINCFG27_SEMMODE_SHIFT)
#define  GPIO_PINCFG27_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG27_SEMMODE_SHIFT))

#define  GPIO_PINCFG27_RESERVED3_MASK 0x8
#define  GPIO_PINCFG27_RESERVED3_SHIFT 3
#define  GPIO_PINCFG27_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_RESERVED3_MASK) >> GPIO_PINCFG27_RESERVED3_SHIFT)
#define  GPIO_PINCFG27_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG27_RESERVED3_SHIFT))

#define  GPIO_PINCFG27_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG27_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG27_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG27_FUNCSEL_MASK) >> GPIO_PINCFG27_FUNCSEL_SHIFT)
#define  GPIO_PINCFG27_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG27_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG27_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 29 of 32) (PinCfg28)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 29 of 32)*/
//====================================================================

#define  GPIO_PINCFG28_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG28_RESERVED1_SHIFT 20
#define  GPIO_PINCFG28_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_RESERVED1_MASK) >> GPIO_PINCFG28_RESERVED1_SHIFT)
#define  GPIO_PINCFG28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG28_RESERVED1_SHIFT))

#define  GPIO_PINCFG28_SLEW_MASK 0xc0000
#define  GPIO_PINCFG28_SLEW_SHIFT 18
#define  GPIO_PINCFG28_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_SLEW_MASK) >> GPIO_PINCFG28_SLEW_SHIFT)
#define  GPIO_PINCFG28_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG28_SLEW_SHIFT))

#define  GPIO_PINCFG28_PDEN_MASK 0x20000
#define  GPIO_PINCFG28_PDEN_SHIFT 17
#define  GPIO_PINCFG28_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_PDEN_MASK) >> GPIO_PINCFG28_PDEN_SHIFT)
#define  GPIO_PINCFG28_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG28_PDEN_SHIFT))

#define  GPIO_PINCFG28_PUEN_MASK 0x10000
#define  GPIO_PINCFG28_PUEN_SHIFT 16
#define  GPIO_PINCFG28_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_PUEN_MASK) >> GPIO_PINCFG28_PUEN_SHIFT)
#define  GPIO_PINCFG28_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG28_PUEN_SHIFT))

#define  GPIO_PINCFG28_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG28_RESERVED2_SHIFT 13
#define  GPIO_PINCFG28_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_RESERVED2_MASK) >> GPIO_PINCFG28_RESERVED2_SHIFT)
#define  GPIO_PINCFG28_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG28_RESERVED2_SHIFT))

#define  GPIO_PINCFG28_RAWALT_MASK 0x1000
#define  GPIO_PINCFG28_RAWALT_SHIFT 12
#define  GPIO_PINCFG28_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_RAWALT_MASK) >> GPIO_PINCFG28_RAWALT_SHIFT)
#define  GPIO_PINCFG28_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG28_RAWALT_SHIFT))

#define  GPIO_PINCFG28_DEGTB_MASK 0xf00
#define  GPIO_PINCFG28_DEGTB_SHIFT 8
#define  GPIO_PINCFG28_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_DEGTB_MASK) >> GPIO_PINCFG28_DEGTB_SHIFT)
#define  GPIO_PINCFG28_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG28_DEGTB_SHIFT))

#define  GPIO_PINCFG28_INTPOL_MASK 0xc0
#define  GPIO_PINCFG28_INTPOL_SHIFT 6
#define  GPIO_PINCFG28_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_INTPOL_MASK) >> GPIO_PINCFG28_INTPOL_SHIFT)
#define  GPIO_PINCFG28_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG28_INTPOL_SHIFT))

#define  GPIO_PINCFG28_INTMODE_MASK 0x20
#define  GPIO_PINCFG28_INTMODE_SHIFT 5
#define  GPIO_PINCFG28_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_INTMODE_MASK) >> GPIO_PINCFG28_INTMODE_SHIFT)
#define  GPIO_PINCFG28_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG28_INTMODE_SHIFT))

#define  GPIO_PINCFG28_SEMMODE_MASK 0x10
#define  GPIO_PINCFG28_SEMMODE_SHIFT 4
#define  GPIO_PINCFG28_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_SEMMODE_MASK) >> GPIO_PINCFG28_SEMMODE_SHIFT)
#define  GPIO_PINCFG28_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG28_SEMMODE_SHIFT))

#define  GPIO_PINCFG28_RESERVED3_MASK 0x8
#define  GPIO_PINCFG28_RESERVED3_SHIFT 3
#define  GPIO_PINCFG28_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_RESERVED3_MASK) >> GPIO_PINCFG28_RESERVED3_SHIFT)
#define  GPIO_PINCFG28_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG28_RESERVED3_SHIFT))

#define  GPIO_PINCFG28_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG28_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG28_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG28_FUNCSEL_MASK) >> GPIO_PINCFG28_FUNCSEL_SHIFT)
#define  GPIO_PINCFG28_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG28_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG28_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 30 of 32) (PinCfg29)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 30 of 32)*/
//====================================================================

#define  GPIO_PINCFG29_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG29_RESERVED1_SHIFT 20
#define  GPIO_PINCFG29_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_RESERVED1_MASK) >> GPIO_PINCFG29_RESERVED1_SHIFT)
#define  GPIO_PINCFG29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG29_RESERVED1_SHIFT))

#define  GPIO_PINCFG29_SLEW_MASK 0xc0000
#define  GPIO_PINCFG29_SLEW_SHIFT 18
#define  GPIO_PINCFG29_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_SLEW_MASK) >> GPIO_PINCFG29_SLEW_SHIFT)
#define  GPIO_PINCFG29_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG29_SLEW_SHIFT))

#define  GPIO_PINCFG29_PDEN_MASK 0x20000
#define  GPIO_PINCFG29_PDEN_SHIFT 17
#define  GPIO_PINCFG29_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_PDEN_MASK) >> GPIO_PINCFG29_PDEN_SHIFT)
#define  GPIO_PINCFG29_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG29_PDEN_SHIFT))

#define  GPIO_PINCFG29_PUEN_MASK 0x10000
#define  GPIO_PINCFG29_PUEN_SHIFT 16
#define  GPIO_PINCFG29_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_PUEN_MASK) >> GPIO_PINCFG29_PUEN_SHIFT)
#define  GPIO_PINCFG29_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG29_PUEN_SHIFT))

#define  GPIO_PINCFG29_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG29_RESERVED2_SHIFT 13
#define  GPIO_PINCFG29_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_RESERVED2_MASK) >> GPIO_PINCFG29_RESERVED2_SHIFT)
#define  GPIO_PINCFG29_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG29_RESERVED2_SHIFT))

#define  GPIO_PINCFG29_RAWALT_MASK 0x1000
#define  GPIO_PINCFG29_RAWALT_SHIFT 12
#define  GPIO_PINCFG29_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_RAWALT_MASK) >> GPIO_PINCFG29_RAWALT_SHIFT)
#define  GPIO_PINCFG29_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG29_RAWALT_SHIFT))

#define  GPIO_PINCFG29_DEGTB_MASK 0xf00
#define  GPIO_PINCFG29_DEGTB_SHIFT 8
#define  GPIO_PINCFG29_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_DEGTB_MASK) >> GPIO_PINCFG29_DEGTB_SHIFT)
#define  GPIO_PINCFG29_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG29_DEGTB_SHIFT))

#define  GPIO_PINCFG29_INTPOL_MASK 0xc0
#define  GPIO_PINCFG29_INTPOL_SHIFT 6
#define  GPIO_PINCFG29_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_INTPOL_MASK) >> GPIO_PINCFG29_INTPOL_SHIFT)
#define  GPIO_PINCFG29_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG29_INTPOL_SHIFT))

#define  GPIO_PINCFG29_INTMODE_MASK 0x20
#define  GPIO_PINCFG29_INTMODE_SHIFT 5
#define  GPIO_PINCFG29_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_INTMODE_MASK) >> GPIO_PINCFG29_INTMODE_SHIFT)
#define  GPIO_PINCFG29_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG29_INTMODE_SHIFT))

#define  GPIO_PINCFG29_SEMMODE_MASK 0x10
#define  GPIO_PINCFG29_SEMMODE_SHIFT 4
#define  GPIO_PINCFG29_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_SEMMODE_MASK) >> GPIO_PINCFG29_SEMMODE_SHIFT)
#define  GPIO_PINCFG29_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG29_SEMMODE_SHIFT))

#define  GPIO_PINCFG29_RESERVED3_MASK 0x8
#define  GPIO_PINCFG29_RESERVED3_SHIFT 3
#define  GPIO_PINCFG29_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_RESERVED3_MASK) >> GPIO_PINCFG29_RESERVED3_SHIFT)
#define  GPIO_PINCFG29_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG29_RESERVED3_SHIFT))

#define  GPIO_PINCFG29_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG29_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG29_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG29_FUNCSEL_MASK) >> GPIO_PINCFG29_FUNCSEL_SHIFT)
#define  GPIO_PINCFG29_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG29_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG29_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 31 of 32) (PinCfg30)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 31 of 32)*/
//====================================================================

#define  GPIO_PINCFG30_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG30_RESERVED1_SHIFT 20
#define  GPIO_PINCFG30_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_RESERVED1_MASK) >> GPIO_PINCFG30_RESERVED1_SHIFT)
#define  GPIO_PINCFG30_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG30_RESERVED1_SHIFT))

#define  GPIO_PINCFG30_SLEW_MASK 0xc0000
#define  GPIO_PINCFG30_SLEW_SHIFT 18
#define  GPIO_PINCFG30_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_SLEW_MASK) >> GPIO_PINCFG30_SLEW_SHIFT)
#define  GPIO_PINCFG30_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG30_SLEW_SHIFT))

#define  GPIO_PINCFG30_PDEN_MASK 0x20000
#define  GPIO_PINCFG30_PDEN_SHIFT 17
#define  GPIO_PINCFG30_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_PDEN_MASK) >> GPIO_PINCFG30_PDEN_SHIFT)
#define  GPIO_PINCFG30_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG30_PDEN_SHIFT))

#define  GPIO_PINCFG30_PUEN_MASK 0x10000
#define  GPIO_PINCFG30_PUEN_SHIFT 16
#define  GPIO_PINCFG30_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_PUEN_MASK) >> GPIO_PINCFG30_PUEN_SHIFT)
#define  GPIO_PINCFG30_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG30_PUEN_SHIFT))

#define  GPIO_PINCFG30_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG30_RESERVED2_SHIFT 13
#define  GPIO_PINCFG30_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_RESERVED2_MASK) >> GPIO_PINCFG30_RESERVED2_SHIFT)
#define  GPIO_PINCFG30_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG30_RESERVED2_SHIFT))

#define  GPIO_PINCFG30_RAWALT_MASK 0x1000
#define  GPIO_PINCFG30_RAWALT_SHIFT 12
#define  GPIO_PINCFG30_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_RAWALT_MASK) >> GPIO_PINCFG30_RAWALT_SHIFT)
#define  GPIO_PINCFG30_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG30_RAWALT_SHIFT))

#define  GPIO_PINCFG30_DEGTB_MASK 0xf00
#define  GPIO_PINCFG30_DEGTB_SHIFT 8
#define  GPIO_PINCFG30_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_DEGTB_MASK) >> GPIO_PINCFG30_DEGTB_SHIFT)
#define  GPIO_PINCFG30_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG30_DEGTB_SHIFT))

#define  GPIO_PINCFG30_INTPOL_MASK 0xc0
#define  GPIO_PINCFG30_INTPOL_SHIFT 6
#define  GPIO_PINCFG30_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_INTPOL_MASK) >> GPIO_PINCFG30_INTPOL_SHIFT)
#define  GPIO_PINCFG30_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG30_INTPOL_SHIFT))

#define  GPIO_PINCFG30_INTMODE_MASK 0x20
#define  GPIO_PINCFG30_INTMODE_SHIFT 5
#define  GPIO_PINCFG30_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_INTMODE_MASK) >> GPIO_PINCFG30_INTMODE_SHIFT)
#define  GPIO_PINCFG30_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG30_INTMODE_SHIFT))

#define  GPIO_PINCFG30_SEMMODE_MASK 0x10
#define  GPIO_PINCFG30_SEMMODE_SHIFT 4
#define  GPIO_PINCFG30_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_SEMMODE_MASK) >> GPIO_PINCFG30_SEMMODE_SHIFT)
#define  GPIO_PINCFG30_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG30_SEMMODE_SHIFT))

#define  GPIO_PINCFG30_RESERVED3_MASK 0x8
#define  GPIO_PINCFG30_RESERVED3_SHIFT 3
#define  GPIO_PINCFG30_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_RESERVED3_MASK) >> GPIO_PINCFG30_RESERVED3_SHIFT)
#define  GPIO_PINCFG30_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG30_RESERVED3_SHIFT))

#define  GPIO_PINCFG30_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG30_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG30_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG30_FUNCSEL_MASK) >> GPIO_PINCFG30_FUNCSEL_SHIFT)
#define  GPIO_PINCFG30_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG30_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG30_FUNCSEL_SHIFT))

//====================================================================
//Register: GPIO Pin Configuration Register (Instance 32 of 32) (PinCfg31)
/** \brief This register is used to program the desired characteristics of the corresponding pin. (Instance 32 of 32)*/
//====================================================================

#define  GPIO_PINCFG31_RESERVED1_MASK 0xfff00000
#define  GPIO_PINCFG31_RESERVED1_SHIFT 20
#define  GPIO_PINCFG31_RESERVED1_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_RESERVED1_MASK) >> GPIO_PINCFG31_RESERVED1_SHIFT)
#define  GPIO_PINCFG31_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_RESERVED1_MASK) | (((uint32_t)val) << GPIO_PINCFG31_RESERVED1_SHIFT))

#define  GPIO_PINCFG31_SLEW_MASK 0xc0000
#define  GPIO_PINCFG31_SLEW_SHIFT 18
#define  GPIO_PINCFG31_SLEW_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_SLEW_MASK) >> GPIO_PINCFG31_SLEW_SHIFT)
#define  GPIO_PINCFG31_SLEW_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_SLEW_MASK) | (((uint32_t)val) << GPIO_PINCFG31_SLEW_SHIFT))

#define  GPIO_PINCFG31_PDEN_MASK 0x20000
#define  GPIO_PINCFG31_PDEN_SHIFT 17
#define  GPIO_PINCFG31_PDEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_PDEN_MASK) >> GPIO_PINCFG31_PDEN_SHIFT)
#define  GPIO_PINCFG31_PDEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_PDEN_MASK) | (((uint32_t)val) << GPIO_PINCFG31_PDEN_SHIFT))

#define  GPIO_PINCFG31_PUEN_MASK 0x10000
#define  GPIO_PINCFG31_PUEN_SHIFT 16
#define  GPIO_PINCFG31_PUEN_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_PUEN_MASK) >> GPIO_PINCFG31_PUEN_SHIFT)
#define  GPIO_PINCFG31_PUEN_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_PUEN_MASK) | (((uint32_t)val) << GPIO_PINCFG31_PUEN_SHIFT))

#define  GPIO_PINCFG31_RESERVED2_MASK 0xe000
#define  GPIO_PINCFG31_RESERVED2_SHIFT 13
#define  GPIO_PINCFG31_RESERVED2_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_RESERVED2_MASK) >> GPIO_PINCFG31_RESERVED2_SHIFT)
#define  GPIO_PINCFG31_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_RESERVED2_MASK) | (((uint32_t)val) << GPIO_PINCFG31_RESERVED2_SHIFT))

#define  GPIO_PINCFG31_RAWALT_MASK 0x1000
#define  GPIO_PINCFG31_RAWALT_SHIFT 12
#define  GPIO_PINCFG31_RAWALT_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_RAWALT_MASK) >> GPIO_PINCFG31_RAWALT_SHIFT)
#define  GPIO_PINCFG31_RAWALT_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_RAWALT_MASK) | (((uint32_t)val) << GPIO_PINCFG31_RAWALT_SHIFT))

#define  GPIO_PINCFG31_DEGTB_MASK 0xf00
#define  GPIO_PINCFG31_DEGTB_SHIFT 8
#define  GPIO_PINCFG31_DEGTB_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_DEGTB_MASK) >> GPIO_PINCFG31_DEGTB_SHIFT)
#define  GPIO_PINCFG31_DEGTB_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_DEGTB_MASK) | (((uint32_t)val) << GPIO_PINCFG31_DEGTB_SHIFT))

#define  GPIO_PINCFG31_INTPOL_MASK 0xc0
#define  GPIO_PINCFG31_INTPOL_SHIFT 6
#define  GPIO_PINCFG31_INTPOL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_INTPOL_MASK) >> GPIO_PINCFG31_INTPOL_SHIFT)
#define  GPIO_PINCFG31_INTPOL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_INTPOL_MASK) | (((uint32_t)val) << GPIO_PINCFG31_INTPOL_SHIFT))

#define  GPIO_PINCFG31_INTMODE_MASK 0x20
#define  GPIO_PINCFG31_INTMODE_SHIFT 5
#define  GPIO_PINCFG31_INTMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_INTMODE_MASK) >> GPIO_PINCFG31_INTMODE_SHIFT)
#define  GPIO_PINCFG31_INTMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_INTMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG31_INTMODE_SHIFT))

#define  GPIO_PINCFG31_SEMMODE_MASK 0x10
#define  GPIO_PINCFG31_SEMMODE_SHIFT 4
#define  GPIO_PINCFG31_SEMMODE_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_SEMMODE_MASK) >> GPIO_PINCFG31_SEMMODE_SHIFT)
#define  GPIO_PINCFG31_SEMMODE_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_SEMMODE_MASK) | (((uint32_t)val) << GPIO_PINCFG31_SEMMODE_SHIFT))

#define  GPIO_PINCFG31_RESERVED3_MASK 0x8
#define  GPIO_PINCFG31_RESERVED3_SHIFT 3
#define  GPIO_PINCFG31_RESERVED3_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_RESERVED3_MASK) >> GPIO_PINCFG31_RESERVED3_SHIFT)
#define  GPIO_PINCFG31_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_RESERVED3_MASK) | (((uint32_t)val) << GPIO_PINCFG31_RESERVED3_SHIFT))

#define  GPIO_PINCFG31_FUNCSEL_MASK 0x7
#define  GPIO_PINCFG31_FUNCSEL_SHIFT 0
#define  GPIO_PINCFG31_FUNCSEL_MASK_SHIFT(reg) (((reg) & GPIO_PINCFG31_FUNCSEL_MASK) >> GPIO_PINCFG31_FUNCSEL_SHIFT)
#define  GPIO_PINCFG31_FUNCSEL_REPLACE_VAL(reg,val) (((reg) & ~GPIO_PINCFG31_FUNCSEL_MASK) | (((uint32_t)val) << GPIO_PINCFG31_FUNCSEL_SHIFT))

#endif // GPIO
