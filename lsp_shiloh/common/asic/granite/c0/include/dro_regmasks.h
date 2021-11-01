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
 * \file dro_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DRO_REGMASKS_H_
#define _DRO_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: DRO Monitor (DRO)
/** \brief The ASIC provides multiple instances of the digital ring oscillator (DRO) block for production verification of the silicon, and for active on-chip thermal management. The DRO can be accessed via ASIC pins in analog test mode, or it can be accessed via registers in normal operation. The APB block apb_dro provides the firmware accessible DRO Monitor control and status registers. <BR>
The DRO source is a ring oscillator that outputs a clock up to 1GHz that is proportional to process-voltage-temperature. Measurement logic is provided by the DRO count-down timer and counter which are external to APB. The DRO can take ~700ns (worst case) to get a valid DRO count. That count may be read from the DRO Monitor Status register. The count is continuously updated into the Status register every 2ms when measurement is enabled (see Control register EnableTimer and EnableRO bits).*/
//
//====================================================================

//====================================================================
//Register: DRO Control Register (CR)
/** \brief Firmware can enable DRO functions using this register.
            A valid DRO measurement is enabled when both the EnableTimer and EnableRO bits are set.
            Clearing either one of these bits will invalidate the measurement value provided in the Status Register.*/
//====================================================================

#define  DRO_CR_RESERVED1_MASK 0xfffffffc
#define  DRO_CR_RESERVED1_SHIFT 2
#define  DRO_CR_RESERVED1_MASK_SHIFT(reg) (((reg) & DRO_CR_RESERVED1_MASK) >> DRO_CR_RESERVED1_SHIFT)
#define  DRO_CR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DRO_CR_RESERVED1_MASK) | (((uint32_t)val) << DRO_CR_RESERVED1_SHIFT))

#define  DRO_CR_ENABLETIMER_MASK 0x2
#define  DRO_CR_ENABLETIMER_SHIFT 1
#define  DRO_CR_ENABLETIMER_MASK_SHIFT(reg) (((reg) & DRO_CR_ENABLETIMER_MASK) >> DRO_CR_ENABLETIMER_SHIFT)
#define  DRO_CR_ENABLETIMER_REPLACE_VAL(reg,val) (((reg) & ~DRO_CR_ENABLETIMER_MASK) | (((uint32_t)val) << DRO_CR_ENABLETIMER_SHIFT))

#define  DRO_CR_ENABLERO_MASK 0x1
#define  DRO_CR_ENABLERO_SHIFT 0
#define  DRO_CR_ENABLERO_MASK_SHIFT(reg) (((reg) & DRO_CR_ENABLERO_MASK) >> DRO_CR_ENABLERO_SHIFT)
#define  DRO_CR_ENABLERO_REPLACE_VAL(reg,val) (((reg) & ~DRO_CR_ENABLERO_MASK) | (((uint32_t)val) << DRO_CR_ENABLERO_SHIFT))

//====================================================================
//Register: DRO Status Register (SR)
/** \brief Firmware can get the measured DRO count from this register.*/
//====================================================================

#define  DRO_SR_VALID_MASK 0x80000000
#define  DRO_SR_VALID_SHIFT 31
#define  DRO_SR_VALID_MASK_SHIFT(reg) (((reg) & DRO_SR_VALID_MASK) >> DRO_SR_VALID_SHIFT)
#define  DRO_SR_VALID_REPLACE_VAL(reg,val) (((reg) & ~DRO_SR_VALID_MASK) | (((uint32_t)val) << DRO_SR_VALID_SHIFT))

#define  DRO_SR_ENABLE_MASK 0x40000000
#define  DRO_SR_ENABLE_SHIFT 30
#define  DRO_SR_ENABLE_MASK_SHIFT(reg) (((reg) & DRO_SR_ENABLE_MASK) >> DRO_SR_ENABLE_SHIFT)
#define  DRO_SR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DRO_SR_ENABLE_MASK) | (((uint32_t)val) << DRO_SR_ENABLE_SHIFT))

#define  DRO_SR_RESERVED1_MASK 0x3ffff800
#define  DRO_SR_RESERVED1_SHIFT 11
#define  DRO_SR_RESERVED1_MASK_SHIFT(reg) (((reg) & DRO_SR_RESERVED1_MASK) >> DRO_SR_RESERVED1_SHIFT)
#define  DRO_SR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DRO_SR_RESERVED1_MASK) | (((uint32_t)val) << DRO_SR_RESERVED1_SHIFT))

#define  DRO_SR_OVERFLOW_MASK 0x400
#define  DRO_SR_OVERFLOW_SHIFT 10
#define  DRO_SR_OVERFLOW_MASK_SHIFT(reg) (((reg) & DRO_SR_OVERFLOW_MASK) >> DRO_SR_OVERFLOW_SHIFT)
#define  DRO_SR_OVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~DRO_SR_OVERFLOW_MASK) | (((uint32_t)val) << DRO_SR_OVERFLOW_SHIFT))

#define  DRO_SR_COUNT_MASK 0x3ff
#define  DRO_SR_COUNT_SHIFT 0
#define  DRO_SR_COUNT_MASK_SHIFT(reg) (((reg) & DRO_SR_COUNT_MASK) >> DRO_SR_COUNT_SHIFT)
#define  DRO_SR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~DRO_SR_COUNT_MASK) | (((uint32_t)val) << DRO_SR_COUNT_SHIFT))

#endif // DRO
