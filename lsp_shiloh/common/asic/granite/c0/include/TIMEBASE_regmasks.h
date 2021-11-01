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
 * \file TIMEBASE_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _APB_TIMEBASE_REG_REGMASKS_H_
#define _APB_TIMEBASE_REG_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Timebase (apb_timebase_reg)
/** \brief The timebase is used to generate timing periods that are useful to other blocks in the system, including the APB Timers/watchdog, Switcher, DRO monitor, and Temperature Sensor monitor. The timebase is programmable to generate correct period timing over a range of input system clock frequencies. By using a common configurable timebase, each block is not burdened with programmable dividers.<BR>
   The timebase provides the following output frequencies (single clock pulse):<BR>
 1Î¼s, 10Î¼s, 100Î¼s, 1ms, 10ms, 100ms*/
//
//====================================================================

//====================================================================
//Register: Timebase Configuration Register (TCR)
/** \brief This register is set to indicate the operating frequency of the ASIC. This permits the timebase to generate correct timebase periods over a wide range of system operating frequencies.<BR>
            This register should be programmed immediately following boot, and should be adjusted whenever the frequency changes (if using a programmable system PLL). Note that the default frequency is set to 48 MHz. When this register is written, the timebase counter restarts.<BR>
            This register may also be used to disable the timebase outputs (for testing or low-power purposes).*/
//====================================================================

#define  APB_TIMEBASE_REG_TCR_RESERVED1_MASK 0xc0000000
#define  APB_TIMEBASE_REG_TCR_RESERVED1_SHIFT 30
#define  APB_TIMEBASE_REG_TCR_RESERVED1_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_RESERVED1_MASK) >> APB_TIMEBASE_REG_TCR_RESERVED1_SHIFT)
#define  APB_TIMEBASE_REG_TCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_RESERVED1_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_RESERVED1_SHIFT))

#define  APB_TIMEBASE_REG_TCR_DISABLE100MS_MASK 0x20000000
#define  APB_TIMEBASE_REG_TCR_DISABLE100MS_SHIFT 29
#define  APB_TIMEBASE_REG_TCR_DISABLE100MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_DISABLE100MS_MASK) >> APB_TIMEBASE_REG_TCR_DISABLE100MS_SHIFT)
#define  APB_TIMEBASE_REG_TCR_DISABLE100MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_DISABLE100MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_DISABLE100MS_SHIFT))

#define  APB_TIMEBASE_REG_TCR_DISABLE10MS_MASK 0x10000000
#define  APB_TIMEBASE_REG_TCR_DISABLE10MS_SHIFT 28
#define  APB_TIMEBASE_REG_TCR_DISABLE10MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_DISABLE10MS_MASK) >> APB_TIMEBASE_REG_TCR_DISABLE10MS_SHIFT)
#define  APB_TIMEBASE_REG_TCR_DISABLE10MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_DISABLE10MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_DISABLE10MS_SHIFT))

#define  APB_TIMEBASE_REG_TCR_DISABLE1MS_MASK 0x8000000
#define  APB_TIMEBASE_REG_TCR_DISABLE1MS_SHIFT 27
#define  APB_TIMEBASE_REG_TCR_DISABLE1MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_DISABLE1MS_MASK) >> APB_TIMEBASE_REG_TCR_DISABLE1MS_SHIFT)
#define  APB_TIMEBASE_REG_TCR_DISABLE1MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_DISABLE1MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_DISABLE1MS_SHIFT))

#define  APB_TIMEBASE_REG_TCR_DISABLE100US_MASK 0x4000000
#define  APB_TIMEBASE_REG_TCR_DISABLE100US_SHIFT 26
#define  APB_TIMEBASE_REG_TCR_DISABLE100US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_DISABLE100US_MASK) >> APB_TIMEBASE_REG_TCR_DISABLE100US_SHIFT)
#define  APB_TIMEBASE_REG_TCR_DISABLE100US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_DISABLE100US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_DISABLE100US_SHIFT))

#define  APB_TIMEBASE_REG_TCR_DISABLE10US_MASK 0x2000000
#define  APB_TIMEBASE_REG_TCR_DISABLE10US_SHIFT 25
#define  APB_TIMEBASE_REG_TCR_DISABLE10US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_DISABLE10US_MASK) >> APB_TIMEBASE_REG_TCR_DISABLE10US_SHIFT)
#define  APB_TIMEBASE_REG_TCR_DISABLE10US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_DISABLE10US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_DISABLE10US_SHIFT))

#define  APB_TIMEBASE_REG_TCR_DISABLE1US_MASK 0x1000000
#define  APB_TIMEBASE_REG_TCR_DISABLE1US_SHIFT 24
#define  APB_TIMEBASE_REG_TCR_DISABLE1US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_DISABLE1US_MASK) >> APB_TIMEBASE_REG_TCR_DISABLE1US_SHIFT)
#define  APB_TIMEBASE_REG_TCR_DISABLE1US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_DISABLE1US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_DISABLE1US_SHIFT))

#define  APB_TIMEBASE_REG_TCR_RESERVED2_MASK 0xffff00
#define  APB_TIMEBASE_REG_TCR_RESERVED2_SHIFT 8
#define  APB_TIMEBASE_REG_TCR_RESERVED2_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_RESERVED2_MASK) >> APB_TIMEBASE_REG_TCR_RESERVED2_SHIFT)
#define  APB_TIMEBASE_REG_TCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_RESERVED2_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_RESERVED2_SHIFT))

#define  APB_TIMEBASE_REG_TCR_SYSFREQ_MASK 0xff
#define  APB_TIMEBASE_REG_TCR_SYSFREQ_SHIFT 0
#define  APB_TIMEBASE_REG_TCR_SYSFREQ_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TCR_SYSFREQ_MASK) >> APB_TIMEBASE_REG_TCR_SYSFREQ_SHIFT)
#define  APB_TIMEBASE_REG_TCR_SYSFREQ_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TCR_SYSFREQ_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TCR_SYSFREQ_SHIFT))

//====================================================================
//Register: Timebase Testing Register (TTR)
/** \brief Writing to this register can force the respective timing outputs to be asserted for a single cycle, or will cause the respective timing outputs to be asserted continuously every other cycle. This is handy for writing ASIC test procedures to "speed up" the testing of slow real-time blocks, and to keep the test synchronized without having to explicitly count clock cycles.<BR>
     During testing, it is generally advisable to disable the normal tick outputs via the Timebase Configuration Register, and then force the desired timing sequence using this register. If the timebase is not disabled using the timebase configuration register, requesting a single tick will cause the count to increment by two.<BR>
     Requesting a single tick affects the count for higher timebases. Care should be taken when disabling the 1us timebase, because it can be required by other blocks. For example, DRAM or LCD refresh.*/
//====================================================================

#define  APB_TIMEBASE_REG_TTR_RESERVED1_MASK 0xc0000000
#define  APB_TIMEBASE_REG_TTR_RESERVED1_SHIFT 30
#define  APB_TIMEBASE_REG_TTR_RESERVED1_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_RESERVED1_MASK) >> APB_TIMEBASE_REG_TTR_RESERVED1_SHIFT)
#define  APB_TIMEBASE_REG_TTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_RESERVED1_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_RESERVED1_SHIFT))

#define  APB_TIMEBASE_REG_TTR_FORCE100MS_MASK 0x20000000
#define  APB_TIMEBASE_REG_TTR_FORCE100MS_SHIFT 29
#define  APB_TIMEBASE_REG_TTR_FORCE100MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_FORCE100MS_MASK) >> APB_TIMEBASE_REG_TTR_FORCE100MS_SHIFT)
#define  APB_TIMEBASE_REG_TTR_FORCE100MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_FORCE100MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_FORCE100MS_SHIFT))

#define  APB_TIMEBASE_REG_TTR_FORCE10MS_MASK 0x10000000
#define  APB_TIMEBASE_REG_TTR_FORCE10MS_SHIFT 28
#define  APB_TIMEBASE_REG_TTR_FORCE10MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_FORCE10MS_MASK) >> APB_TIMEBASE_REG_TTR_FORCE10MS_SHIFT)
#define  APB_TIMEBASE_REG_TTR_FORCE10MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_FORCE10MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_FORCE10MS_SHIFT))

#define  APB_TIMEBASE_REG_TTR_FORCE1MS_MASK 0x8000000
#define  APB_TIMEBASE_REG_TTR_FORCE1MS_SHIFT 27
#define  APB_TIMEBASE_REG_TTR_FORCE1MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_FORCE1MS_MASK) >> APB_TIMEBASE_REG_TTR_FORCE1MS_SHIFT)
#define  APB_TIMEBASE_REG_TTR_FORCE1MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_FORCE1MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_FORCE1MS_SHIFT))

#define  APB_TIMEBASE_REG_TTR_FORCE100US_MASK 0x4000000
#define  APB_TIMEBASE_REG_TTR_FORCE100US_SHIFT 26
#define  APB_TIMEBASE_REG_TTR_FORCE100US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_FORCE100US_MASK) >> APB_TIMEBASE_REG_TTR_FORCE100US_SHIFT)
#define  APB_TIMEBASE_REG_TTR_FORCE100US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_FORCE100US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_FORCE100US_SHIFT))

#define  APB_TIMEBASE_REG_TTR_FORCE10US_MASK 0x2000000
#define  APB_TIMEBASE_REG_TTR_FORCE10US_SHIFT 25
#define  APB_TIMEBASE_REG_TTR_FORCE10US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_FORCE10US_MASK) >> APB_TIMEBASE_REG_TTR_FORCE10US_SHIFT)
#define  APB_TIMEBASE_REG_TTR_FORCE10US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_FORCE10US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_FORCE10US_SHIFT))

#define  APB_TIMEBASE_REG_TTR_FORCE1US_MASK 0x1000000
#define  APB_TIMEBASE_REG_TTR_FORCE1US_SHIFT 24
#define  APB_TIMEBASE_REG_TTR_FORCE1US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_FORCE1US_MASK) >> APB_TIMEBASE_REG_TTR_FORCE1US_SHIFT)
#define  APB_TIMEBASE_REG_TTR_FORCE1US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_FORCE1US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_FORCE1US_SHIFT))

#define  APB_TIMEBASE_REG_TTR_RESERVED2_MASK 0xc00000
#define  APB_TIMEBASE_REG_TTR_RESERVED2_SHIFT 22
#define  APB_TIMEBASE_REG_TTR_RESERVED2_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_RESERVED2_MASK) >> APB_TIMEBASE_REG_TTR_RESERVED2_SHIFT)
#define  APB_TIMEBASE_REG_TTR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_RESERVED2_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_RESERVED2_SHIFT))

#define  APB_TIMEBASE_REG_TTR_CONT100MS_MASK 0x200000
#define  APB_TIMEBASE_REG_TTR_CONT100MS_SHIFT 21
#define  APB_TIMEBASE_REG_TTR_CONT100MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_CONT100MS_MASK) >> APB_TIMEBASE_REG_TTR_CONT100MS_SHIFT)
#define  APB_TIMEBASE_REG_TTR_CONT100MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_CONT100MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_CONT100MS_SHIFT))

#define  APB_TIMEBASE_REG_TTR_CONT10MS_MASK 0x100000
#define  APB_TIMEBASE_REG_TTR_CONT10MS_SHIFT 20
#define  APB_TIMEBASE_REG_TTR_CONT10MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_CONT10MS_MASK) >> APB_TIMEBASE_REG_TTR_CONT10MS_SHIFT)
#define  APB_TIMEBASE_REG_TTR_CONT10MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_CONT10MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_CONT10MS_SHIFT))

#define  APB_TIMEBASE_REG_TTR_CONT1MS_MASK 0x80000
#define  APB_TIMEBASE_REG_TTR_CONT1MS_SHIFT 19
#define  APB_TIMEBASE_REG_TTR_CONT1MS_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_CONT1MS_MASK) >> APB_TIMEBASE_REG_TTR_CONT1MS_SHIFT)
#define  APB_TIMEBASE_REG_TTR_CONT1MS_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_CONT1MS_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_CONT1MS_SHIFT))

#define  APB_TIMEBASE_REG_TTR_CONT100US_MASK 0x40000
#define  APB_TIMEBASE_REG_TTR_CONT100US_SHIFT 18
#define  APB_TIMEBASE_REG_TTR_CONT100US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_CONT100US_MASK) >> APB_TIMEBASE_REG_TTR_CONT100US_SHIFT)
#define  APB_TIMEBASE_REG_TTR_CONT100US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_CONT100US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_CONT100US_SHIFT))

#define  APB_TIMEBASE_REG_TTR_CONT10US_MASK 0x20000
#define  APB_TIMEBASE_REG_TTR_CONT10US_SHIFT 17
#define  APB_TIMEBASE_REG_TTR_CONT10US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_CONT10US_MASK) >> APB_TIMEBASE_REG_TTR_CONT10US_SHIFT)
#define  APB_TIMEBASE_REG_TTR_CONT10US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_CONT10US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_CONT10US_SHIFT))

#define  APB_TIMEBASE_REG_TTR_CONT1US_MASK 0x10000
#define  APB_TIMEBASE_REG_TTR_CONT1US_SHIFT 16
#define  APB_TIMEBASE_REG_TTR_CONT1US_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_CONT1US_MASK) >> APB_TIMEBASE_REG_TTR_CONT1US_SHIFT)
#define  APB_TIMEBASE_REG_TTR_CONT1US_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_CONT1US_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_CONT1US_SHIFT))

#define  APB_TIMEBASE_REG_TTR_RESERVED3_MASK 0xffff
#define  APB_TIMEBASE_REG_TTR_RESERVED3_SHIFT 0
#define  APB_TIMEBASE_REG_TTR_RESERVED3_MASK_SHIFT(reg) (((reg) & APB_TIMEBASE_REG_TTR_RESERVED3_MASK) >> APB_TIMEBASE_REG_TTR_RESERVED3_SHIFT)
#define  APB_TIMEBASE_REG_TTR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~APB_TIMEBASE_REG_TTR_RESERVED3_MASK) | (((uint32_t)val) << APB_TIMEBASE_REG_TTR_RESERVED3_SHIFT))

#endif // APB_TIMEBASE_REG
