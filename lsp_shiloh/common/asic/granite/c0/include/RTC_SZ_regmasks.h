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
 * \file RTC_SZ_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _RTC_SZ_REGMASKS_H_
#define _RTC_SZ_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Register descriptions for Real-Time Clock module (RTC_SZ)
/** \brief Register Descriptions for the Swiss RTC.
    Since the interface to the RTC hard macro is only 8-bits wide,
    firmware MUST read each register twice. The first read updates a
    32-bit shadow in the RTC wrapper and the second read reads
    the 32-bit shadow.*/
//
//====================================================================

//====================================================================
//Register: RTC Status Register (Status)
/** \brief This register contains the alarm flags from the RTC.*/
//====================================================================

#define  RTC_SZ_STATUS_RESERVED1_MASK 0xfffffffc
#define  RTC_SZ_STATUS_RESERVED1_SHIFT 2
#define  RTC_SZ_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & RTC_SZ_STATUS_RESERVED1_MASK) >> RTC_SZ_STATUS_RESERVED1_SHIFT)
#define  RTC_SZ_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_STATUS_RESERVED1_MASK) | (((uint32_t)val) << RTC_SZ_STATUS_RESERVED1_SHIFT))

#define  RTC_SZ_STATUS_ALARM2_MASK 0x2
#define  RTC_SZ_STATUS_ALARM2_SHIFT 1
#define  RTC_SZ_STATUS_ALARM2_MASK_SHIFT(reg) (((reg) & RTC_SZ_STATUS_ALARM2_MASK) >> RTC_SZ_STATUS_ALARM2_SHIFT)
#define  RTC_SZ_STATUS_ALARM2_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_STATUS_ALARM2_MASK) | (((uint32_t)val) << RTC_SZ_STATUS_ALARM2_SHIFT))

#define  RTC_SZ_STATUS_ALARM1_MASK 0x1
#define  RTC_SZ_STATUS_ALARM1_SHIFT 0
#define  RTC_SZ_STATUS_ALARM1_MASK_SHIFT(reg) (((reg) & RTC_SZ_STATUS_ALARM1_MASK) >> RTC_SZ_STATUS_ALARM1_SHIFT)
#define  RTC_SZ_STATUS_ALARM1_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_STATUS_ALARM1_MASK) | (((uint32_t)val) << RTC_SZ_STATUS_ALARM1_SHIFT))

//====================================================================
//Register: RTC Interrupt 1 Register (Interrupt1)
/** \brief This register controls Interrupt 1*/
//====================================================================

#define  RTC_SZ_INTERRUPT1_RESERVED1_MASK 0xffffffc0
#define  RTC_SZ_INTERRUPT1_RESERVED1_SHIFT 6
#define  RTC_SZ_INTERRUPT1_RESERVED1_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT1_RESERVED1_MASK) >> RTC_SZ_INTERRUPT1_RESERVED1_SHIFT)
#define  RTC_SZ_INTERRUPT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT1_RESERVED1_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT1_RESERVED1_SHIFT))

#define  RTC_SZ_INTERRUPT1_INT1FREQ_MASK 0x3c
#define  RTC_SZ_INTERRUPT1_INT1FREQ_SHIFT 2
#define  RTC_SZ_INTERRUPT1_INT1FREQ_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT1_INT1FREQ_MASK) >> RTC_SZ_INTERRUPT1_INT1FREQ_SHIFT)
#define  RTC_SZ_INTERRUPT1_INT1FREQ_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT1_INT1FREQ_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT1_INT1FREQ_SHIFT))

#define  RTC_SZ_INTERRUPT1_INT1FE_MASK 0x2
#define  RTC_SZ_INTERRUPT1_INT1FE_SHIFT 1
#define  RTC_SZ_INTERRUPT1_INT1FE_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT1_INT1FE_MASK) >> RTC_SZ_INTERRUPT1_INT1FE_SHIFT)
#define  RTC_SZ_INTERRUPT1_INT1FE_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT1_INT1FE_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT1_INT1FE_SHIFT))

#define  RTC_SZ_INTERRUPT1_INT1AE_MASK 0x1
#define  RTC_SZ_INTERRUPT1_INT1AE_SHIFT 0
#define  RTC_SZ_INTERRUPT1_INT1AE_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT1_INT1AE_MASK) >> RTC_SZ_INTERRUPT1_INT1AE_SHIFT)
#define  RTC_SZ_INTERRUPT1_INT1AE_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT1_INT1AE_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT1_INT1AE_SHIFT))

//====================================================================
//Register: RTC Interrupt 2 Register (Interrupt2)
/** \brief This register controls Interrupt 2*/
//====================================================================

#define  RTC_SZ_INTERRUPT2_RESERVED1_MASK 0xffffffc0
#define  RTC_SZ_INTERRUPT2_RESERVED1_SHIFT 6
#define  RTC_SZ_INTERRUPT2_RESERVED1_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT2_RESERVED1_MASK) >> RTC_SZ_INTERRUPT2_RESERVED1_SHIFT)
#define  RTC_SZ_INTERRUPT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT2_RESERVED1_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT2_RESERVED1_SHIFT))

#define  RTC_SZ_INTERRUPT2_INT2FREQ_MASK 0x3c
#define  RTC_SZ_INTERRUPT2_INT2FREQ_SHIFT 2
#define  RTC_SZ_INTERRUPT2_INT2FREQ_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT2_INT2FREQ_MASK) >> RTC_SZ_INTERRUPT2_INT2FREQ_SHIFT)
#define  RTC_SZ_INTERRUPT2_INT2FREQ_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT2_INT2FREQ_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT2_INT2FREQ_SHIFT))

#define  RTC_SZ_INTERRUPT2_INT2FE_MASK 0x2
#define  RTC_SZ_INTERRUPT2_INT2FE_SHIFT 1
#define  RTC_SZ_INTERRUPT2_INT2FE_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT2_INT2FE_MASK) >> RTC_SZ_INTERRUPT2_INT2FE_SHIFT)
#define  RTC_SZ_INTERRUPT2_INT2FE_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT2_INT2FE_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT2_INT2FE_SHIFT))

#define  RTC_SZ_INTERRUPT2_INT2AE_MASK 0x1
#define  RTC_SZ_INTERRUPT2_INT2AE_SHIFT 0
#define  RTC_SZ_INTERRUPT2_INT2AE_MASK_SHIFT(reg) (((reg) & RTC_SZ_INTERRUPT2_INT2AE_MASK) >> RTC_SZ_INTERRUPT2_INT2AE_SHIFT)
#define  RTC_SZ_INTERRUPT2_INT2AE_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_INTERRUPT2_INT2AE_MASK) | (((uint32_t)val) << RTC_SZ_INTERRUPT2_INT2AE_SHIFT))

//====================================================================
//Register: RTC Time Register (Time)
/** \brief This register accesses the RTC Timer. This register MUST be written
      twice in order to initialize a new time value (e.g. set the clock).*/
//====================================================================

#define  RTC_SZ_TIME_TIME_MASK 0xffffffff
#define  RTC_SZ_TIME_TIME_SHIFT 0
#define  RTC_SZ_TIME_TIME_MASK_SHIFT(reg) (((reg) & RTC_SZ_TIME_TIME_MASK) >> RTC_SZ_TIME_TIME_SHIFT)
#define  RTC_SZ_TIME_TIME_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TIME_TIME_MASK) | (((uint32_t)val) << RTC_SZ_TIME_TIME_SHIFT))

//====================================================================
//Register: RTC Alarm 1 Register (Alarm1)
/** \brief This register contains the alarm time. When RTC time matches the
      alarm time and the alarm interrupt is enabled, the alarm port goes
      high for 1 second and the alarm flag is set.*/
//====================================================================

#define  RTC_SZ_ALARM1_TIME_MASK 0xffffffff
#define  RTC_SZ_ALARM1_TIME_SHIFT 0
#define  RTC_SZ_ALARM1_TIME_MASK_SHIFT(reg) (((reg) & RTC_SZ_ALARM1_TIME_MASK) >> RTC_SZ_ALARM1_TIME_SHIFT)
#define  RTC_SZ_ALARM1_TIME_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_ALARM1_TIME_MASK) | (((uint32_t)val) << RTC_SZ_ALARM1_TIME_SHIFT))

//====================================================================
//Register: RTC Alarm 2 Register (Alarm2)
/** \brief This register contains the alarm time. When RTC time matches the
      alarm time and the alarm interrupt is enabled, the alarm port goes
      high for 1 second and the alarm flag is set.*/
//====================================================================

#define  RTC_SZ_ALARM2_TIME_MASK 0xffffffff
#define  RTC_SZ_ALARM2_TIME_SHIFT 0
#define  RTC_SZ_ALARM2_TIME_MASK_SHIFT(reg) (((reg) & RTC_SZ_ALARM2_TIME_MASK) >> RTC_SZ_ALARM2_TIME_SHIFT)
#define  RTC_SZ_ALARM2_TIME_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_ALARM2_TIME_MASK) | (((uint32_t)val) << RTC_SZ_ALARM2_TIME_SHIFT))

//====================================================================
//Register: Clock Correction Register (ClockCorrect)
/** \brief This register sets the clock correction values.*/
//====================================================================

#define  RTC_SZ_CLOCKCORRECT_RESERVED1_MASK 0xffff0000
#define  RTC_SZ_CLOCKCORRECT_RESERVED1_SHIFT 16
#define  RTC_SZ_CLOCKCORRECT_RESERVED1_MASK_SHIFT(reg) (((reg) & RTC_SZ_CLOCKCORRECT_RESERVED1_MASK) >> RTC_SZ_CLOCKCORRECT_RESERVED1_SHIFT)
#define  RTC_SZ_CLOCKCORRECT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_CLOCKCORRECT_RESERVED1_MASK) | (((uint32_t)val) << RTC_SZ_CLOCKCORRECT_RESERVED1_SHIFT))

#define  RTC_SZ_CLOCKCORRECT_CMODE_MASK 0x8000
#define  RTC_SZ_CLOCKCORRECT_CMODE_SHIFT 15
#define  RTC_SZ_CLOCKCORRECT_CMODE_MASK_SHIFT(reg) (((reg) & RTC_SZ_CLOCKCORRECT_CMODE_MASK) >> RTC_SZ_CLOCKCORRECT_CMODE_SHIFT)
#define  RTC_SZ_CLOCKCORRECT_CMODE_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_CLOCKCORRECT_CMODE_MASK) | (((uint32_t)val) << RTC_SZ_CLOCKCORRECT_CMODE_SHIFT))

#define  RTC_SZ_CLOCKCORRECT_CVAL_EXT_MASK 0x7f00
#define  RTC_SZ_CLOCKCORRECT_CVAL_EXT_SHIFT 8
#define  RTC_SZ_CLOCKCORRECT_CVAL_EXT_MASK_SHIFT(reg) (((reg) & RTC_SZ_CLOCKCORRECT_CVAL_EXT_MASK) >> RTC_SZ_CLOCKCORRECT_CVAL_EXT_SHIFT)
#define  RTC_SZ_CLOCKCORRECT_CVAL_EXT_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_CLOCKCORRECT_CVAL_EXT_MASK) | (((uint32_t)val) << RTC_SZ_CLOCKCORRECT_CVAL_EXT_SHIFT))

#define  RTC_SZ_CLOCKCORRECT_CVALUE_MASK 0xff
#define  RTC_SZ_CLOCKCORRECT_CVALUE_SHIFT 0
#define  RTC_SZ_CLOCKCORRECT_CVALUE_MASK_SHIFT(reg) (((reg) & RTC_SZ_CLOCKCORRECT_CVALUE_MASK) >> RTC_SZ_CLOCKCORRECT_CVALUE_SHIFT)
#define  RTC_SZ_CLOCKCORRECT_CVALUE_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_CLOCKCORRECT_CVALUE_MASK) | (((uint32_t)val) << RTC_SZ_CLOCKCORRECT_CVALUE_SHIFT))

//====================================================================
//Register: RTC Test Configuration Register (TestConfig)
/** \brief This register sets test control bits.*/
//====================================================================

#define  RTC_SZ_TESTCONFIG_RESERVED1_MASK 0xffffffe0
#define  RTC_SZ_TESTCONFIG_RESERVED1_SHIFT 5
#define  RTC_SZ_TESTCONFIG_RESERVED1_MASK_SHIFT(reg) (((reg) & RTC_SZ_TESTCONFIG_RESERVED1_MASK) >> RTC_SZ_TESTCONFIG_RESERVED1_SHIFT)
#define  RTC_SZ_TESTCONFIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TESTCONFIG_RESERVED1_MASK) | (((uint32_t)val) << RTC_SZ_TESTCONFIG_RESERVED1_SHIFT))

#define  RTC_SZ_TESTCONFIG_FUNCTEST_MASK 0x10
#define  RTC_SZ_TESTCONFIG_FUNCTEST_SHIFT 4
#define  RTC_SZ_TESTCONFIG_FUNCTEST_MASK_SHIFT(reg) (((reg) & RTC_SZ_TESTCONFIG_FUNCTEST_MASK) >> RTC_SZ_TESTCONFIG_FUNCTEST_SHIFT)
#define  RTC_SZ_TESTCONFIG_FUNCTEST_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TESTCONFIG_FUNCTEST_MASK) | (((uint32_t)val) << RTC_SZ_TESTCONFIG_FUNCTEST_SHIFT))

#define  RTC_SZ_TESTCONFIG_TMODE_MASK 0xf
#define  RTC_SZ_TESTCONFIG_TMODE_SHIFT 0
#define  RTC_SZ_TESTCONFIG_TMODE_MASK_SHIFT(reg) (((reg) & RTC_SZ_TESTCONFIG_TMODE_MASK) >> RTC_SZ_TESTCONFIG_TMODE_SHIFT)
#define  RTC_SZ_TESTCONFIG_TMODE_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TESTCONFIG_TMODE_MASK) | (((uint32_t)val) << RTC_SZ_TESTCONFIG_TMODE_SHIFT))

//====================================================================
//Register: RTC Timing Register (Timing)
/** \brief This register controls timing of accesses to the RTC core.*/
//====================================================================

#define  RTC_SZ_TIMING_RESERVED1_MASK 0xff000000
#define  RTC_SZ_TIMING_RESERVED1_SHIFT 24
#define  RTC_SZ_TIMING_RESERVED1_MASK_SHIFT(reg) (((reg) & RTC_SZ_TIMING_RESERVED1_MASK) >> RTC_SZ_TIMING_RESERVED1_SHIFT)
#define  RTC_SZ_TIMING_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TIMING_RESERVED1_MASK) | (((uint32_t)val) << RTC_SZ_TIMING_RESERVED1_SHIFT))

#define  RTC_SZ_TIMING_TDELAY_MASK 0xff0000
#define  RTC_SZ_TIMING_TDELAY_SHIFT 16
#define  RTC_SZ_TIMING_TDELAY_MASK_SHIFT(reg) (((reg) & RTC_SZ_TIMING_TDELAY_MASK) >> RTC_SZ_TIMING_TDELAY_SHIFT)
#define  RTC_SZ_TIMING_TDELAY_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TIMING_TDELAY_MASK) | (((uint32_t)val) << RTC_SZ_TIMING_TDELAY_SHIFT))

#define  RTC_SZ_TIMING_TCLK_MASK 0xff00
#define  RTC_SZ_TIMING_TCLK_SHIFT 8
#define  RTC_SZ_TIMING_TCLK_MASK_SHIFT(reg) (((reg) & RTC_SZ_TIMING_TCLK_MASK) >> RTC_SZ_TIMING_TCLK_SHIFT)
#define  RTC_SZ_TIMING_TCLK_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TIMING_TCLK_MASK) | (((uint32_t)val) << RTC_SZ_TIMING_TCLK_SHIFT))

#define  RTC_SZ_TIMING_TSETUP_MASK 0xff
#define  RTC_SZ_TIMING_TSETUP_SHIFT 0
#define  RTC_SZ_TIMING_TSETUP_MASK_SHIFT(reg) (((reg) & RTC_SZ_TIMING_TSETUP_MASK) >> RTC_SZ_TIMING_TSETUP_SHIFT)
#define  RTC_SZ_TIMING_TSETUP_REPLACE_VAL(reg,val) (((reg) & ~RTC_SZ_TIMING_TSETUP_MASK) | (((uint32_t)val) << RTC_SZ_TIMING_TSETUP_SHIFT))

#endif // RTC_SZ
