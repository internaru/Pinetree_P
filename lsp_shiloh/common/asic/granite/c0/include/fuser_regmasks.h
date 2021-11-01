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
 * \file fuser_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _FUSER_REGMASKS_H_
#define _FUSER_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: FUSER block registers (fuser)
/** \brief The following registers are used to configure the
    FUSER block. Note that synchronous mode implies that a
    zeroCrossDetect signal is being used (either rising, falling, or both).
    Asynchronous mode implies that no zeroCrossDetect is available.*/
//
//====================================================================

//====================================================================
//Register: Fuser Enable and Polarity Register (FEPR)
/** \brief This register is used to enable and select the polarity for the
      FUSER input and output signals.*/
//====================================================================

#define  FUSER_FEPR_RESERVED1_MASK 0xffc00000
#define  FUSER_FEPR_RESERVED1_SHIFT 22
#define  FUSER_FEPR_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_RESERVED1_MASK) >> FUSER_FEPR_RESERVED1_SHIFT)
#define  FUSER_FEPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FEPR_RESERVED1_SHIFT))

#define  FUSER_FEPR_OUTPUT_MUX_MASK 0x300000
#define  FUSER_FEPR_OUTPUT_MUX_SHIFT 20
#define  FUSER_FEPR_OUTPUT_MUX_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_OUTPUT_MUX_MASK) >> FUSER_FEPR_OUTPUT_MUX_SHIFT)
#define  FUSER_FEPR_OUTPUT_MUX_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_OUTPUT_MUX_MASK) | (((uint32_t)val) << FUSER_FEPR_OUTPUT_MUX_SHIFT))

#define  FUSER_FEPR_RESERVED2_MASK 0x80000
#define  FUSER_FEPR_RESERVED2_SHIFT 19
#define  FUSER_FEPR_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_RESERVED2_MASK) >> FUSER_FEPR_RESERVED2_SHIFT)
#define  FUSER_FEPR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FEPR_RESERVED2_SHIFT))

#define  FUSER_FEPR_LVPOWERON_POLARITY_MASK 0x40000
#define  FUSER_FEPR_LVPOWERON_POLARITY_SHIFT 18
#define  FUSER_FEPR_LVPOWERON_POLARITY_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_LVPOWERON_POLARITY_MASK) >> FUSER_FEPR_LVPOWERON_POLARITY_SHIFT)
#define  FUSER_FEPR_LVPOWERON_POLARITY_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_LVPOWERON_POLARITY_MASK) | (((uint32_t)val) << FUSER_FEPR_LVPOWERON_POLARITY_SHIFT))

#define  FUSER_FEPR_LVPOWERON_OE_MASK 0x20000
#define  FUSER_FEPR_LVPOWERON_OE_SHIFT 17
#define  FUSER_FEPR_LVPOWERON_OE_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_LVPOWERON_OE_MASK) >> FUSER_FEPR_LVPOWERON_OE_SHIFT)
#define  FUSER_FEPR_LVPOWERON_OE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_LVPOWERON_OE_MASK) | (((uint32_t)val) << FUSER_FEPR_LVPOWERON_OE_SHIFT))

#define  FUSER_FEPR_LVPOWERON_MASK 0x10000
#define  FUSER_FEPR_LVPOWERON_SHIFT 16
#define  FUSER_FEPR_LVPOWERON_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_LVPOWERON_MASK) >> FUSER_FEPR_LVPOWERON_SHIFT)
#define  FUSER_FEPR_LVPOWERON_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_LVPOWERON_MASK) | (((uint32_t)val) << FUSER_FEPR_LVPOWERON_SHIFT))

#define  FUSER_FEPR_RESERVED3_MASK 0x8000
#define  FUSER_FEPR_RESERVED3_SHIFT 15
#define  FUSER_FEPR_RESERVED3_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_RESERVED3_MASK) >> FUSER_FEPR_RESERVED3_SHIFT)
#define  FUSER_FEPR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_RESERVED3_MASK) | (((uint32_t)val) << FUSER_FEPR_RESERVED3_SHIFT))

#define  FUSER_FEPR_FUSEROVERTEMP_POLARITY_MASK 0x7000
#define  FUSER_FEPR_FUSEROVERTEMP_POLARITY_SHIFT 12
#define  FUSER_FEPR_FUSEROVERTEMP_POLARITY_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_FUSEROVERTEMP_POLARITY_MASK) >> FUSER_FEPR_FUSEROVERTEMP_POLARITY_SHIFT)
#define  FUSER_FEPR_FUSEROVERTEMP_POLARITY_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_FUSEROVERTEMP_POLARITY_MASK) | (((uint32_t)val) << FUSER_FEPR_FUSEROVERTEMP_POLARITY_SHIFT))

#define  FUSER_FEPR_RESERVED4_MASK 0x800
#define  FUSER_FEPR_RESERVED4_SHIFT 11
#define  FUSER_FEPR_RESERVED4_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_RESERVED4_MASK) >> FUSER_FEPR_RESERVED4_SHIFT)
#define  FUSER_FEPR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_RESERVED4_MASK) | (((uint32_t)val) << FUSER_FEPR_RESERVED4_SHIFT))

#define  FUSER_FEPR_FUSEROVERTEMP_IE_MASK 0x700
#define  FUSER_FEPR_FUSEROVERTEMP_IE_SHIFT 8
#define  FUSER_FEPR_FUSEROVERTEMP_IE_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_FUSEROVERTEMP_IE_MASK) >> FUSER_FEPR_FUSEROVERTEMP_IE_SHIFT)
#define  FUSER_FEPR_FUSEROVERTEMP_IE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_FUSEROVERTEMP_IE_MASK) | (((uint32_t)val) << FUSER_FEPR_FUSEROVERTEMP_IE_SHIFT))

#define  FUSER_FEPR_RESERVED5_MASK 0x80
#define  FUSER_FEPR_RESERVED5_SHIFT 7
#define  FUSER_FEPR_RESERVED5_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_RESERVED5_MASK) >> FUSER_FEPR_RESERVED5_SHIFT)
#define  FUSER_FEPR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_RESERVED5_MASK) | (((uint32_t)val) << FUSER_FEPR_RESERVED5_SHIFT))

#define  FUSER_FEPR_FUSER_POLARITY_MASK 0x70
#define  FUSER_FEPR_FUSER_POLARITY_SHIFT 4
#define  FUSER_FEPR_FUSER_POLARITY_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_FUSER_POLARITY_MASK) >> FUSER_FEPR_FUSER_POLARITY_SHIFT)
#define  FUSER_FEPR_FUSER_POLARITY_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_FUSER_POLARITY_MASK) | (((uint32_t)val) << FUSER_FEPR_FUSER_POLARITY_SHIFT))

#define  FUSER_FEPR_RESERVED6_MASK 0x8
#define  FUSER_FEPR_RESERVED6_SHIFT 3
#define  FUSER_FEPR_RESERVED6_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_RESERVED6_MASK) >> FUSER_FEPR_RESERVED6_SHIFT)
#define  FUSER_FEPR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_RESERVED6_MASK) | (((uint32_t)val) << FUSER_FEPR_RESERVED6_SHIFT))

#define  FUSER_FEPR_FUSER_OE_MASK 0x7
#define  FUSER_FEPR_FUSER_OE_SHIFT 0
#define  FUSER_FEPR_FUSER_OE_MASK_SHIFT(reg) (((reg) & FUSER_FEPR_FUSER_OE_MASK) >> FUSER_FEPR_FUSER_OE_SHIFT)
#define  FUSER_FEPR_FUSER_OE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FEPR_FUSER_OE_MASK) | (((uint32_t)val) << FUSER_FEPR_FUSER_OE_SHIFT))

//====================================================================
//Register: Fuser Configuration Register (Instance 1 of 3) (FCR0)
/** \brief This register specifies timebase and timestep for the Fuser,
      which are only used in asynchronous PWM and DSM mode. It also sets the fuser mode. (Instance 1 of 3)*/
//====================================================================

#define  FUSER_FCR0_FUSER_TIMESTEP_MASK 0xffffff00
#define  FUSER_FCR0_FUSER_TIMESTEP_SHIFT 8
#define  FUSER_FCR0_FUSER_TIMESTEP_MASK_SHIFT(reg) (((reg) & FUSER_FCR0_FUSER_TIMESTEP_MASK) >> FUSER_FCR0_FUSER_TIMESTEP_SHIFT)
#define  FUSER_FCR0_FUSER_TIMESTEP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR0_FUSER_TIMESTEP_MASK) | (((uint32_t)val) << FUSER_FCR0_FUSER_TIMESTEP_SHIFT))

#define  FUSER_FCR0_RESERVED1_MASK 0x80
#define  FUSER_FCR0_RESERVED1_SHIFT 7
#define  FUSER_FCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FCR0_RESERVED1_MASK) >> FUSER_FCR0_RESERVED1_SHIFT)
#define  FUSER_FCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR0_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FCR0_RESERVED1_SHIFT))

#define  FUSER_FCR0_FUSER_TIMEBASE_MASK 0x70
#define  FUSER_FCR0_FUSER_TIMEBASE_SHIFT 4
#define  FUSER_FCR0_FUSER_TIMEBASE_MASK_SHIFT(reg) (((reg) & FUSER_FCR0_FUSER_TIMEBASE_MASK) >> FUSER_FCR0_FUSER_TIMEBASE_SHIFT)
#define  FUSER_FCR0_FUSER_TIMEBASE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR0_FUSER_TIMEBASE_MASK) | (((uint32_t)val) << FUSER_FCR0_FUSER_TIMEBASE_SHIFT))

#define  FUSER_FCR0_RESERVED2_MASK 0xc
#define  FUSER_FCR0_RESERVED2_SHIFT 2
#define  FUSER_FCR0_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FCR0_RESERVED2_MASK) >> FUSER_FCR0_RESERVED2_SHIFT)
#define  FUSER_FCR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR0_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FCR0_RESERVED2_SHIFT))

#define  FUSER_FCR0_FUSER_MODE_MASK 0x3
#define  FUSER_FCR0_FUSER_MODE_SHIFT 0
#define  FUSER_FCR0_FUSER_MODE_MASK_SHIFT(reg) (((reg) & FUSER_FCR0_FUSER_MODE_MASK) >> FUSER_FCR0_FUSER_MODE_SHIFT)
#define  FUSER_FCR0_FUSER_MODE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR0_FUSER_MODE_MASK) | (((uint32_t)val) << FUSER_FCR0_FUSER_MODE_SHIFT))

//====================================================================
//Register: Fuser Configuration Register (Instance 2 of 3) (FCR1)
/** \brief This register specifies timebase and timestep for the Fuser,
      which are only used in asynchronous PWM and DSM mode. It also sets the fuser mode. (Instance 2 of 3)*/
//====================================================================

#define  FUSER_FCR1_FUSER_TIMESTEP_MASK 0xffffff00
#define  FUSER_FCR1_FUSER_TIMESTEP_SHIFT 8
#define  FUSER_FCR1_FUSER_TIMESTEP_MASK_SHIFT(reg) (((reg) & FUSER_FCR1_FUSER_TIMESTEP_MASK) >> FUSER_FCR1_FUSER_TIMESTEP_SHIFT)
#define  FUSER_FCR1_FUSER_TIMESTEP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR1_FUSER_TIMESTEP_MASK) | (((uint32_t)val) << FUSER_FCR1_FUSER_TIMESTEP_SHIFT))

#define  FUSER_FCR1_RESERVED1_MASK 0x80
#define  FUSER_FCR1_RESERVED1_SHIFT 7
#define  FUSER_FCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FCR1_RESERVED1_MASK) >> FUSER_FCR1_RESERVED1_SHIFT)
#define  FUSER_FCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR1_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FCR1_RESERVED1_SHIFT))

#define  FUSER_FCR1_FUSER_TIMEBASE_MASK 0x70
#define  FUSER_FCR1_FUSER_TIMEBASE_SHIFT 4
#define  FUSER_FCR1_FUSER_TIMEBASE_MASK_SHIFT(reg) (((reg) & FUSER_FCR1_FUSER_TIMEBASE_MASK) >> FUSER_FCR1_FUSER_TIMEBASE_SHIFT)
#define  FUSER_FCR1_FUSER_TIMEBASE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR1_FUSER_TIMEBASE_MASK) | (((uint32_t)val) << FUSER_FCR1_FUSER_TIMEBASE_SHIFT))

#define  FUSER_FCR1_RESERVED2_MASK 0xc
#define  FUSER_FCR1_RESERVED2_SHIFT 2
#define  FUSER_FCR1_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FCR1_RESERVED2_MASK) >> FUSER_FCR1_RESERVED2_SHIFT)
#define  FUSER_FCR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR1_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FCR1_RESERVED2_SHIFT))

#define  FUSER_FCR1_FUSER_MODE_MASK 0x3
#define  FUSER_FCR1_FUSER_MODE_SHIFT 0
#define  FUSER_FCR1_FUSER_MODE_MASK_SHIFT(reg) (((reg) & FUSER_FCR1_FUSER_MODE_MASK) >> FUSER_FCR1_FUSER_MODE_SHIFT)
#define  FUSER_FCR1_FUSER_MODE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR1_FUSER_MODE_MASK) | (((uint32_t)val) << FUSER_FCR1_FUSER_MODE_SHIFT))

//====================================================================
//Register: Fuser Configuration Register (Instance 3 of 3) (FCR2)
/** \brief This register specifies timebase and timestep for the Fuser,
      which are only used in asynchronous PWM and DSM mode. It also sets the fuser mode. (Instance 3 of 3)*/
//====================================================================

#define  FUSER_FCR2_FUSER_TIMESTEP_MASK 0xffffff00
#define  FUSER_FCR2_FUSER_TIMESTEP_SHIFT 8
#define  FUSER_FCR2_FUSER_TIMESTEP_MASK_SHIFT(reg) (((reg) & FUSER_FCR2_FUSER_TIMESTEP_MASK) >> FUSER_FCR2_FUSER_TIMESTEP_SHIFT)
#define  FUSER_FCR2_FUSER_TIMESTEP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR2_FUSER_TIMESTEP_MASK) | (((uint32_t)val) << FUSER_FCR2_FUSER_TIMESTEP_SHIFT))

#define  FUSER_FCR2_RESERVED1_MASK 0x80
#define  FUSER_FCR2_RESERVED1_SHIFT 7
#define  FUSER_FCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FCR2_RESERVED1_MASK) >> FUSER_FCR2_RESERVED1_SHIFT)
#define  FUSER_FCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR2_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FCR2_RESERVED1_SHIFT))

#define  FUSER_FCR2_FUSER_TIMEBASE_MASK 0x70
#define  FUSER_FCR2_FUSER_TIMEBASE_SHIFT 4
#define  FUSER_FCR2_FUSER_TIMEBASE_MASK_SHIFT(reg) (((reg) & FUSER_FCR2_FUSER_TIMEBASE_MASK) >> FUSER_FCR2_FUSER_TIMEBASE_SHIFT)
#define  FUSER_FCR2_FUSER_TIMEBASE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR2_FUSER_TIMEBASE_MASK) | (((uint32_t)val) << FUSER_FCR2_FUSER_TIMEBASE_SHIFT))

#define  FUSER_FCR2_RESERVED2_MASK 0xc
#define  FUSER_FCR2_RESERVED2_SHIFT 2
#define  FUSER_FCR2_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FCR2_RESERVED2_MASK) >> FUSER_FCR2_RESERVED2_SHIFT)
#define  FUSER_FCR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR2_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FCR2_RESERVED2_SHIFT))

#define  FUSER_FCR2_FUSER_MODE_MASK 0x3
#define  FUSER_FCR2_FUSER_MODE_SHIFT 0
#define  FUSER_FCR2_FUSER_MODE_MASK_SHIFT(reg) (((reg) & FUSER_FCR2_FUSER_MODE_MASK) >> FUSER_FCR2_FUSER_MODE_SHIFT)
#define  FUSER_FCR2_FUSER_MODE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FCR2_FUSER_MODE_MASK) | (((uint32_t)val) << FUSER_FCR2_FUSER_MODE_SHIFT))

//====================================================================
//Register: Fuser PWM Mode Even Cycle Configuration Register (Instance 1 of 3) (FPECR0)
/** \brief If in synchronous PWM mode (fuser_mode == 00), timeOff and
      timeOn specify the number of zeroCrossDetect pulses that comprise the
      deasserted and asserted portions of the PWM cycle. If in asynchronous PWM
      mode (fuser_mode == 01), timeOff and timeOn specify the number of
      ticks in the selected timebase that comprise the deasserted and asserted
      portions of the PWM cycle.
      <br><br>
      To achieve power line balancing and get finer grained control, two
      cycles of control for PWM mode are provided. For example, if a 25%
      on duty cycle is required it could be achieved by programming timeOff to 3
      and timeOn to 1 but this would result in a power line imbalance. 25% on
      could also be achieved by programming timeOff to 6 and timeOn to 2 but the long
      time off may not be desirable.
      <br><br>
      With two cycle configuration registers 25% on can be
      achieved by programming the even cycle to a timeOff of 2 and a timeOn of 1,
      then setting the odd cycle to a timeOff of 4 and a timeOn of 1.
      <br><br>
      If the desired waveform can be achieved with the same values for both the
      even and odd cycles both registers must be programmed. The hardware
      alternates using the even and odd cycle values.
      <br><br>
      To allow 0% and 100% fuser on control, programming 0 in the timeOff or
      timeOn fields result in a setting of 0 ticks.
      <br><br>
      Example: If in synchronous mode, to achieve a 61.5% fuser duty cycle,
      load 0x5 into timeOff and 0x8 into timeOn (both even and odd). The resulting PWM period
      will be (1/60s)*1/2*(5+8) = 108.3ms.
      <br><br>
      Example: If in asynchronous mode with a selected timebase of 10us and a timestep setting of 0, to
      achieve a 25.9% duty cycle, load 0x14 into timeOff and 0x7 into timeOn.
      The resulting PWM period will be 10us*(0+1)*(20+7) = 270us. (Instance 1 of 3)*/
//====================================================================

#define  FUSER_FPECR0_FUSER_TIMEOFF_EVEN_MASK 0xffff0000
#define  FUSER_FPECR0_FUSER_TIMEOFF_EVEN_SHIFT 16
#define  FUSER_FPECR0_FUSER_TIMEOFF_EVEN_MASK_SHIFT(reg) (((reg) & FUSER_FPECR0_FUSER_TIMEOFF_EVEN_MASK) >> FUSER_FPECR0_FUSER_TIMEOFF_EVEN_SHIFT)
#define  FUSER_FPECR0_FUSER_TIMEOFF_EVEN_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPECR0_FUSER_TIMEOFF_EVEN_MASK) | (((uint32_t)val) << FUSER_FPECR0_FUSER_TIMEOFF_EVEN_SHIFT))

#define  FUSER_FPECR0_FUSER_TIMEON_EVEN_MASK 0xffff
#define  FUSER_FPECR0_FUSER_TIMEON_EVEN_SHIFT 0
#define  FUSER_FPECR0_FUSER_TIMEON_EVEN_MASK_SHIFT(reg) (((reg) & FUSER_FPECR0_FUSER_TIMEON_EVEN_MASK) >> FUSER_FPECR0_FUSER_TIMEON_EVEN_SHIFT)
#define  FUSER_FPECR0_FUSER_TIMEON_EVEN_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPECR0_FUSER_TIMEON_EVEN_MASK) | (((uint32_t)val) << FUSER_FPECR0_FUSER_TIMEON_EVEN_SHIFT))

//====================================================================
//Register: Fuser PWM Mode Even Cycle Configuration Register (Instance 2 of 3) (FPECR1)
/** \brief If in synchronous PWM mode (fuser_mode == 00), timeOff and
      timeOn specify the number of zeroCrossDetect pulses that comprise the
      deasserted and asserted portions of the PWM cycle. If in asynchronous PWM
      mode (fuser_mode == 01), timeOff and timeOn specify the number of
      ticks in the selected timebase that comprise the deasserted and asserted
      portions of the PWM cycle.
      <br><br>
      To achieve power line balancing and get finer grained control, two
      cycles of control for PWM mode are provided. For example, if a 25%
      on duty cycle is required it could be achieved by programming timeOff to 3
      and timeOn to 1 but this would result in a power line imbalance. 25% on
      could also be achieved by programming timeOff to 6 and timeOn to 2 but the long
      time off may not be desirable.
      <br><br>
      With two cycle configuration registers 25% on can be
      achieved by programming the even cycle to a timeOff of 2 and a timeOn of 1,
      then setting the odd cycle to a timeOff of 4 and a timeOn of 1.
      <br><br>
      If the desired waveform can be achieved with the same values for both the
      even and odd cycles both registers must be programmed. The hardware
      alternates using the even and odd cycle values.
      <br><br>
      To allow 0% and 100% fuser on control, programming 0 in the timeOff or
      timeOn fields result in a setting of 0 ticks.
      <br><br>
      Example: If in synchronous mode, to achieve a 61.5% fuser duty cycle,
      load 0x5 into timeOff and 0x8 into timeOn (both even and odd). The resulting PWM period
      will be (1/60s)*1/2*(5+8) = 108.3ms.
      <br><br>
      Example: If in asynchronous mode with a selected timebase of 10us and a timestep setting of 0, to
      achieve a 25.9% duty cycle, load 0x14 into timeOff and 0x7 into timeOn.
      The resulting PWM period will be 10us*(0+1)*(20+7) = 270us. (Instance 2 of 3)*/
//====================================================================

#define  FUSER_FPECR1_FUSER_TIMEOFF_EVEN_MASK 0xffff0000
#define  FUSER_FPECR1_FUSER_TIMEOFF_EVEN_SHIFT 16
#define  FUSER_FPECR1_FUSER_TIMEOFF_EVEN_MASK_SHIFT(reg) (((reg) & FUSER_FPECR1_FUSER_TIMEOFF_EVEN_MASK) >> FUSER_FPECR1_FUSER_TIMEOFF_EVEN_SHIFT)
#define  FUSER_FPECR1_FUSER_TIMEOFF_EVEN_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPECR1_FUSER_TIMEOFF_EVEN_MASK) | (((uint32_t)val) << FUSER_FPECR1_FUSER_TIMEOFF_EVEN_SHIFT))

#define  FUSER_FPECR1_FUSER_TIMEON_EVEN_MASK 0xffff
#define  FUSER_FPECR1_FUSER_TIMEON_EVEN_SHIFT 0
#define  FUSER_FPECR1_FUSER_TIMEON_EVEN_MASK_SHIFT(reg) (((reg) & FUSER_FPECR1_FUSER_TIMEON_EVEN_MASK) >> FUSER_FPECR1_FUSER_TIMEON_EVEN_SHIFT)
#define  FUSER_FPECR1_FUSER_TIMEON_EVEN_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPECR1_FUSER_TIMEON_EVEN_MASK) | (((uint32_t)val) << FUSER_FPECR1_FUSER_TIMEON_EVEN_SHIFT))

//====================================================================
//Register: Fuser PWM Mode Even Cycle Configuration Register (Instance 3 of 3) (FPECR2)
/** \brief If in synchronous PWM mode (fuser_mode == 00), timeOff and
      timeOn specify the number of zeroCrossDetect pulses that comprise the
      deasserted and asserted portions of the PWM cycle. If in asynchronous PWM
      mode (fuser_mode == 01), timeOff and timeOn specify the number of
      ticks in the selected timebase that comprise the deasserted and asserted
      portions of the PWM cycle.
      <br><br>
      To achieve power line balancing and get finer grained control, two
      cycles of control for PWM mode are provided. For example, if a 25%
      on duty cycle is required it could be achieved by programming timeOff to 3
      and timeOn to 1 but this would result in a power line imbalance. 25% on
      could also be achieved by programming timeOff to 6 and timeOn to 2 but the long
      time off may not be desirable.
      <br><br>
      With two cycle configuration registers 25% on can be
      achieved by programming the even cycle to a timeOff of 2 and a timeOn of 1,
      then setting the odd cycle to a timeOff of 4 and a timeOn of 1.
      <br><br>
      If the desired waveform can be achieved with the same values for both the
      even and odd cycles both registers must be programmed. The hardware
      alternates using the even and odd cycle values.
      <br><br>
      To allow 0% and 100% fuser on control, programming 0 in the timeOff or
      timeOn fields result in a setting of 0 ticks.
      <br><br>
      Example: If in synchronous mode, to achieve a 61.5% fuser duty cycle,
      load 0x5 into timeOff and 0x8 into timeOn (both even and odd). The resulting PWM period
      will be (1/60s)*1/2*(5+8) = 108.3ms.
      <br><br>
      Example: If in asynchronous mode with a selected timebase of 10us and a timestep setting of 0, to
      achieve a 25.9% duty cycle, load 0x14 into timeOff and 0x7 into timeOn.
      The resulting PWM period will be 10us*(0+1)*(20+7) = 270us. (Instance 3 of 3)*/
//====================================================================

#define  FUSER_FPECR2_FUSER_TIMEOFF_EVEN_MASK 0xffff0000
#define  FUSER_FPECR2_FUSER_TIMEOFF_EVEN_SHIFT 16
#define  FUSER_FPECR2_FUSER_TIMEOFF_EVEN_MASK_SHIFT(reg) (((reg) & FUSER_FPECR2_FUSER_TIMEOFF_EVEN_MASK) >> FUSER_FPECR2_FUSER_TIMEOFF_EVEN_SHIFT)
#define  FUSER_FPECR2_FUSER_TIMEOFF_EVEN_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPECR2_FUSER_TIMEOFF_EVEN_MASK) | (((uint32_t)val) << FUSER_FPECR2_FUSER_TIMEOFF_EVEN_SHIFT))

#define  FUSER_FPECR2_FUSER_TIMEON_EVEN_MASK 0xffff
#define  FUSER_FPECR2_FUSER_TIMEON_EVEN_SHIFT 0
#define  FUSER_FPECR2_FUSER_TIMEON_EVEN_MASK_SHIFT(reg) (((reg) & FUSER_FPECR2_FUSER_TIMEON_EVEN_MASK) >> FUSER_FPECR2_FUSER_TIMEON_EVEN_SHIFT)
#define  FUSER_FPECR2_FUSER_TIMEON_EVEN_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPECR2_FUSER_TIMEON_EVEN_MASK) | (((uint32_t)val) << FUSER_FPECR2_FUSER_TIMEON_EVEN_SHIFT))

//====================================================================
//Register: Fuser PWM Mode Odd Cycle Configuration Register (Instance 1 of 3) (FPOCR0)
/** \brief If in synchronous PWM mode (fuser_mode == 00), timeOff and
      timeOn specify the number of zeroCrossDetect pulses that comprise the
      deasserted and asserted portions of the PWM cycle. If in asynchronous PWM
      mode (fuser_mode == 01), timeOff and timeOn specify the number of
      ticks in the selected timebase that comprise the deasserted and asserted
      portions of the PWM cycle.
      <br><br>
      To achieve power line balancing and get finer grained control, two
      cycles of control for PWM mode are provided. For example, if a 25%
      on duty cycle is required it could be achieved by programming timeOff to 3
      and timeOn to 1 but this would result in a power line imbalance. 25% on
      could also be achieved by programming timeOff to 6 and timeOn to 2 but the long
      time off may not be desirable.
      <br><br>
      With two cycle configuration registers 25% on can be
      achieved by programming the even cycle to a timeOff of 2 and a timeOn of 1,
      then setting the odd cycle to a timeOff of 4 and a timeOn of 1.
      <br><br>
      If the desired waveform can be achieved with the same values for both the
      even and odd cycles both registers must be programmed. The hardware
      alternates using the even and odd cycle values.
      <br><br>
      To allow 0% and 100% fuser on control, programming 0 in the timeOff or
      timeOn fields result in a setting of 0 ticks.
      <br><br>
      Example: If in synchronous mode, to achieve a 61.5% fuser duty cycle,
      load 0x5 into timeOff and 0x8 into timeOn (both even and odd). The resulting PWM period
      will be (1/60s)*1/2*(5+8) = 108.3ms.
      <br><br>
      Example: If in asynchronous mode with a selected timebase of 10us and a timestep setting of 0, to
      achieve a 25.9% duty cycle, load 0x14 into timeOff and 0x7 into timeOn.
      The resulting PWM period will be 10us*(0+1)*(20+7) = 270us. (Instance 1 of 3)*/
//====================================================================

#define  FUSER_FPOCR0_FUSER_TIMEOFF_ODD_MASK 0xffff0000
#define  FUSER_FPOCR0_FUSER_TIMEOFF_ODD_SHIFT 16
#define  FUSER_FPOCR0_FUSER_TIMEOFF_ODD_MASK_SHIFT(reg) (((reg) & FUSER_FPOCR0_FUSER_TIMEOFF_ODD_MASK) >> FUSER_FPOCR0_FUSER_TIMEOFF_ODD_SHIFT)
#define  FUSER_FPOCR0_FUSER_TIMEOFF_ODD_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPOCR0_FUSER_TIMEOFF_ODD_MASK) | (((uint32_t)val) << FUSER_FPOCR0_FUSER_TIMEOFF_ODD_SHIFT))

#define  FUSER_FPOCR0_FUSER_TIMEON_ODD_MASK 0xffff
#define  FUSER_FPOCR0_FUSER_TIMEON_ODD_SHIFT 0
#define  FUSER_FPOCR0_FUSER_TIMEON_ODD_MASK_SHIFT(reg) (((reg) & FUSER_FPOCR0_FUSER_TIMEON_ODD_MASK) >> FUSER_FPOCR0_FUSER_TIMEON_ODD_SHIFT)
#define  FUSER_FPOCR0_FUSER_TIMEON_ODD_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPOCR0_FUSER_TIMEON_ODD_MASK) | (((uint32_t)val) << FUSER_FPOCR0_FUSER_TIMEON_ODD_SHIFT))

//====================================================================
//Register: Fuser PWM Mode Odd Cycle Configuration Register (Instance 2 of 3) (FPOCR1)
/** \brief If in synchronous PWM mode (fuser_mode == 00), timeOff and
      timeOn specify the number of zeroCrossDetect pulses that comprise the
      deasserted and asserted portions of the PWM cycle. If in asynchronous PWM
      mode (fuser_mode == 01), timeOff and timeOn specify the number of
      ticks in the selected timebase that comprise the deasserted and asserted
      portions of the PWM cycle.
      <br><br>
      To achieve power line balancing and get finer grained control, two
      cycles of control for PWM mode are provided. For example, if a 25%
      on duty cycle is required it could be achieved by programming timeOff to 3
      and timeOn to 1 but this would result in a power line imbalance. 25% on
      could also be achieved by programming timeOff to 6 and timeOn to 2 but the long
      time off may not be desirable.
      <br><br>
      With two cycle configuration registers 25% on can be
      achieved by programming the even cycle to a timeOff of 2 and a timeOn of 1,
      then setting the odd cycle to a timeOff of 4 and a timeOn of 1.
      <br><br>
      If the desired waveform can be achieved with the same values for both the
      even and odd cycles both registers must be programmed. The hardware
      alternates using the even and odd cycle values.
      <br><br>
      To allow 0% and 100% fuser on control, programming 0 in the timeOff or
      timeOn fields result in a setting of 0 ticks.
      <br><br>
      Example: If in synchronous mode, to achieve a 61.5% fuser duty cycle,
      load 0x5 into timeOff and 0x8 into timeOn (both even and odd). The resulting PWM period
      will be (1/60s)*1/2*(5+8) = 108.3ms.
      <br><br>
      Example: If in asynchronous mode with a selected timebase of 10us and a timestep setting of 0, to
      achieve a 25.9% duty cycle, load 0x14 into timeOff and 0x7 into timeOn.
      The resulting PWM period will be 10us*(0+1)*(20+7) = 270us. (Instance 2 of 3)*/
//====================================================================

#define  FUSER_FPOCR1_FUSER_TIMEOFF_ODD_MASK 0xffff0000
#define  FUSER_FPOCR1_FUSER_TIMEOFF_ODD_SHIFT 16
#define  FUSER_FPOCR1_FUSER_TIMEOFF_ODD_MASK_SHIFT(reg) (((reg) & FUSER_FPOCR1_FUSER_TIMEOFF_ODD_MASK) >> FUSER_FPOCR1_FUSER_TIMEOFF_ODD_SHIFT)
#define  FUSER_FPOCR1_FUSER_TIMEOFF_ODD_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPOCR1_FUSER_TIMEOFF_ODD_MASK) | (((uint32_t)val) << FUSER_FPOCR1_FUSER_TIMEOFF_ODD_SHIFT))

#define  FUSER_FPOCR1_FUSER_TIMEON_ODD_MASK 0xffff
#define  FUSER_FPOCR1_FUSER_TIMEON_ODD_SHIFT 0
#define  FUSER_FPOCR1_FUSER_TIMEON_ODD_MASK_SHIFT(reg) (((reg) & FUSER_FPOCR1_FUSER_TIMEON_ODD_MASK) >> FUSER_FPOCR1_FUSER_TIMEON_ODD_SHIFT)
#define  FUSER_FPOCR1_FUSER_TIMEON_ODD_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPOCR1_FUSER_TIMEON_ODD_MASK) | (((uint32_t)val) << FUSER_FPOCR1_FUSER_TIMEON_ODD_SHIFT))

//====================================================================
//Register: Fuser PWM Mode Odd Cycle Configuration Register (Instance 3 of 3) (FPOCR2)
/** \brief If in synchronous PWM mode (fuser_mode == 00), timeOff and
      timeOn specify the number of zeroCrossDetect pulses that comprise the
      deasserted and asserted portions of the PWM cycle. If in asynchronous PWM
      mode (fuser_mode == 01), timeOff and timeOn specify the number of
      ticks in the selected timebase that comprise the deasserted and asserted
      portions of the PWM cycle.
      <br><br>
      To achieve power line balancing and get finer grained control, two
      cycles of control for PWM mode are provided. For example, if a 25%
      on duty cycle is required it could be achieved by programming timeOff to 3
      and timeOn to 1 but this would result in a power line imbalance. 25% on
      could also be achieved by programming timeOff to 6 and timeOn to 2 but the long
      time off may not be desirable.
      <br><br>
      With two cycle configuration registers 25% on can be
      achieved by programming the even cycle to a timeOff of 2 and a timeOn of 1,
      then setting the odd cycle to a timeOff of 4 and a timeOn of 1.
      <br><br>
      If the desired waveform can be achieved with the same values for both the
      even and odd cycles both registers must be programmed. The hardware
      alternates using the even and odd cycle values.
      <br><br>
      To allow 0% and 100% fuser on control, programming 0 in the timeOff or
      timeOn fields result in a setting of 0 ticks.
      <br><br>
      Example: If in synchronous mode, to achieve a 61.5% fuser duty cycle,
      load 0x5 into timeOff and 0x8 into timeOn (both even and odd). The resulting PWM period
      will be (1/60s)*1/2*(5+8) = 108.3ms.
      <br><br>
      Example: If in asynchronous mode with a selected timebase of 10us and a timestep setting of 0, to
      achieve a 25.9% duty cycle, load 0x14 into timeOff and 0x7 into timeOn.
      The resulting PWM period will be 10us*(0+1)*(20+7) = 270us. (Instance 3 of 3)*/
//====================================================================

#define  FUSER_FPOCR2_FUSER_TIMEOFF_ODD_MASK 0xffff0000
#define  FUSER_FPOCR2_FUSER_TIMEOFF_ODD_SHIFT 16
#define  FUSER_FPOCR2_FUSER_TIMEOFF_ODD_MASK_SHIFT(reg) (((reg) & FUSER_FPOCR2_FUSER_TIMEOFF_ODD_MASK) >> FUSER_FPOCR2_FUSER_TIMEOFF_ODD_SHIFT)
#define  FUSER_FPOCR2_FUSER_TIMEOFF_ODD_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPOCR2_FUSER_TIMEOFF_ODD_MASK) | (((uint32_t)val) << FUSER_FPOCR2_FUSER_TIMEOFF_ODD_SHIFT))

#define  FUSER_FPOCR2_FUSER_TIMEON_ODD_MASK 0xffff
#define  FUSER_FPOCR2_FUSER_TIMEON_ODD_SHIFT 0
#define  FUSER_FPOCR2_FUSER_TIMEON_ODD_MASK_SHIFT(reg) (((reg) & FUSER_FPOCR2_FUSER_TIMEON_ODD_MASK) >> FUSER_FPOCR2_FUSER_TIMEON_ODD_SHIFT)
#define  FUSER_FPOCR2_FUSER_TIMEON_ODD_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPOCR2_FUSER_TIMEON_ODD_MASK) | (((uint32_t)val) << FUSER_FPOCR2_FUSER_TIMEON_ODD_SHIFT))

//====================================================================
//Register: Fuser DSM Mode Configuration Register (Instance 1 of 3) (FDCR0)
/** \brief In synchronous mode, the Delta Sigma Modulator (DSM) operates by
      enabling and disabling half or full cycles of the ac line voltage.
      In asynchronous mode, the DSM is clocked at a frequency specified
      by selecting a timebase and a timestep. For example, to clock the
      DSM at 1/(70us) select a timebase of 10us and a timestep of 0x6.
      <br><br>
      The fuser_dutyCycle is 8 bits. In this application it is used to modulate
      the 50-60Hz AC to the fuser.
      However, a 8-bit DSM requires up to 256 60Hz periods per sample. This
      is 256*8.33ms=2.1seconds for half-cycle chopping. A desired update rate would
      be something around 100ms-200ms. Therefore using all 8-bits is not desirable
      for a 50-60Hz signal.
      <br><br>
      To get the desired update rate use only the high-order bits of the DSM. For example to
      emulate a 4-bit DSM, fuser_dutyCycle &= 0xf0. The output of the 8-bit DSM
      with its lower four bits zeroed is the same as a 4-bit DSM.
<table align="center"> <tr><td>Bits Used</td><td>Update Rate</td></tr> <tr><td>4</td><td>16*8.33ms = 133ms</td></tr> <tr><td>3</td><td>8*8.33ms = 65ms</td></tr> <tr><td>2</td><td>4*8.33ms = 33ms</td></tr></table>
      <br>
      Note: The frequency of the DSM output is dependent on the duty cycle
      and is not directly specifiable. (Instance 1 of 3)*/
//====================================================================

#define  FUSER_FDCR0_RESERVED1_MASK 0xfffe0000
#define  FUSER_FDCR0_RESERVED1_SHIFT 17
#define  FUSER_FDCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FDCR0_RESERVED1_MASK) >> FUSER_FDCR0_RESERVED1_SHIFT)
#define  FUSER_FDCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR0_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FDCR0_RESERVED1_SHIFT))

#define  FUSER_FDCR0_FUSER_FULLCYCLE_MASK 0x10000
#define  FUSER_FDCR0_FUSER_FULLCYCLE_SHIFT 16
#define  FUSER_FDCR0_FUSER_FULLCYCLE_MASK_SHIFT(reg) (((reg) & FUSER_FDCR0_FUSER_FULLCYCLE_MASK) >> FUSER_FDCR0_FUSER_FULLCYCLE_SHIFT)
#define  FUSER_FDCR0_FUSER_FULLCYCLE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR0_FUSER_FULLCYCLE_MASK) | (((uint32_t)val) << FUSER_FDCR0_FUSER_FULLCYCLE_SHIFT))

#define  FUSER_FDCR0_RESERVED2_MASK 0xff00
#define  FUSER_FDCR0_RESERVED2_SHIFT 8
#define  FUSER_FDCR0_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FDCR0_RESERVED2_MASK) >> FUSER_FDCR0_RESERVED2_SHIFT)
#define  FUSER_FDCR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR0_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FDCR0_RESERVED2_SHIFT))

#define  FUSER_FDCR0_FUSER_DUTYCYCLE_MASK 0xff
#define  FUSER_FDCR0_FUSER_DUTYCYCLE_SHIFT 0
#define  FUSER_FDCR0_FUSER_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & FUSER_FDCR0_FUSER_DUTYCYCLE_MASK) >> FUSER_FDCR0_FUSER_DUTYCYCLE_SHIFT)
#define  FUSER_FDCR0_FUSER_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR0_FUSER_DUTYCYCLE_MASK) | (((uint32_t)val) << FUSER_FDCR0_FUSER_DUTYCYCLE_SHIFT))

//====================================================================
//Register: Fuser DSM Mode Configuration Register (Instance 2 of 3) (FDCR1)
/** \brief In synchronous mode, the Delta Sigma Modulator (DSM) operates by
      enabling and disabling half or full cycles of the ac line voltage.
      In asynchronous mode, the DSM is clocked at a frequency specified
      by selecting a timebase and a timestep. For example, to clock the
      DSM at 1/(70us) select a timebase of 10us and a timestep of 0x6.
      <br><br>
      The fuser_dutyCycle is 8 bits. In this application it is used to modulate
      the 50-60Hz AC to the fuser.
      However, a 8-bit DSM requires up to 256 60Hz periods per sample. This
      is 256*8.33ms=2.1seconds for half-cycle chopping. A desired update rate would
      be something around 100ms-200ms. Therefore using all 8-bits is not desirable
      for a 50-60Hz signal.
      <br><br>
      To get the desired update rate use only the high-order bits of the DSM. For example to
      emulate a 4-bit DSM, fuser_dutyCycle &= 0xf0. The output of the 8-bit DSM
      with its lower four bits zeroed is the same as a 4-bit DSM.
<table align="center"> <tr><td>Bits Used</td><td>Update Rate</td></tr> <tr><td>4</td><td>16*8.33ms = 133ms</td></tr> <tr><td>3</td><td>8*8.33ms = 65ms</td></tr> <tr><td>2</td><td>4*8.33ms = 33ms</td></tr></table>
      <br>
      Note: The frequency of the DSM output is dependent on the duty cycle
      and is not directly specifiable. (Instance 2 of 3)*/
//====================================================================

#define  FUSER_FDCR1_RESERVED1_MASK 0xfffe0000
#define  FUSER_FDCR1_RESERVED1_SHIFT 17
#define  FUSER_FDCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FDCR1_RESERVED1_MASK) >> FUSER_FDCR1_RESERVED1_SHIFT)
#define  FUSER_FDCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR1_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FDCR1_RESERVED1_SHIFT))

#define  FUSER_FDCR1_FUSER_FULLCYCLE_MASK 0x10000
#define  FUSER_FDCR1_FUSER_FULLCYCLE_SHIFT 16
#define  FUSER_FDCR1_FUSER_FULLCYCLE_MASK_SHIFT(reg) (((reg) & FUSER_FDCR1_FUSER_FULLCYCLE_MASK) >> FUSER_FDCR1_FUSER_FULLCYCLE_SHIFT)
#define  FUSER_FDCR1_FUSER_FULLCYCLE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR1_FUSER_FULLCYCLE_MASK) | (((uint32_t)val) << FUSER_FDCR1_FUSER_FULLCYCLE_SHIFT))

#define  FUSER_FDCR1_RESERVED2_MASK 0xff00
#define  FUSER_FDCR1_RESERVED2_SHIFT 8
#define  FUSER_FDCR1_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FDCR1_RESERVED2_MASK) >> FUSER_FDCR1_RESERVED2_SHIFT)
#define  FUSER_FDCR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR1_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FDCR1_RESERVED2_SHIFT))

#define  FUSER_FDCR1_FUSER_DUTYCYCLE_MASK 0xff
#define  FUSER_FDCR1_FUSER_DUTYCYCLE_SHIFT 0
#define  FUSER_FDCR1_FUSER_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & FUSER_FDCR1_FUSER_DUTYCYCLE_MASK) >> FUSER_FDCR1_FUSER_DUTYCYCLE_SHIFT)
#define  FUSER_FDCR1_FUSER_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR1_FUSER_DUTYCYCLE_MASK) | (((uint32_t)val) << FUSER_FDCR1_FUSER_DUTYCYCLE_SHIFT))

//====================================================================
//Register: Fuser DSM Mode Configuration Register (Instance 3 of 3) (FDCR2)
/** \brief In synchronous mode, the Delta Sigma Modulator (DSM) operates by
      enabling and disabling half or full cycles of the ac line voltage.
      In asynchronous mode, the DSM is clocked at a frequency specified
      by selecting a timebase and a timestep. For example, to clock the
      DSM at 1/(70us) select a timebase of 10us and a timestep of 0x6.
      <br><br>
      The fuser_dutyCycle is 8 bits. In this application it is used to modulate
      the 50-60Hz AC to the fuser.
      However, a 8-bit DSM requires up to 256 60Hz periods per sample. This
      is 256*8.33ms=2.1seconds for half-cycle chopping. A desired update rate would
      be something around 100ms-200ms. Therefore using all 8-bits is not desirable
      for a 50-60Hz signal.
      <br><br>
      To get the desired update rate use only the high-order bits of the DSM. For example to
      emulate a 4-bit DSM, fuser_dutyCycle &= 0xf0. The output of the 8-bit DSM
      with its lower four bits zeroed is the same as a 4-bit DSM.
<table align="center"> <tr><td>Bits Used</td><td>Update Rate</td></tr> <tr><td>4</td><td>16*8.33ms = 133ms</td></tr> <tr><td>3</td><td>8*8.33ms = 65ms</td></tr> <tr><td>2</td><td>4*8.33ms = 33ms</td></tr></table>
      <br>
      Note: The frequency of the DSM output is dependent on the duty cycle
      and is not directly specifiable. (Instance 3 of 3)*/
//====================================================================

#define  FUSER_FDCR2_RESERVED1_MASK 0xfffe0000
#define  FUSER_FDCR2_RESERVED1_SHIFT 17
#define  FUSER_FDCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FDCR2_RESERVED1_MASK) >> FUSER_FDCR2_RESERVED1_SHIFT)
#define  FUSER_FDCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR2_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FDCR2_RESERVED1_SHIFT))

#define  FUSER_FDCR2_FUSER_FULLCYCLE_MASK 0x10000
#define  FUSER_FDCR2_FUSER_FULLCYCLE_SHIFT 16
#define  FUSER_FDCR2_FUSER_FULLCYCLE_MASK_SHIFT(reg) (((reg) & FUSER_FDCR2_FUSER_FULLCYCLE_MASK) >> FUSER_FDCR2_FUSER_FULLCYCLE_SHIFT)
#define  FUSER_FDCR2_FUSER_FULLCYCLE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR2_FUSER_FULLCYCLE_MASK) | (((uint32_t)val) << FUSER_FDCR2_FUSER_FULLCYCLE_SHIFT))

#define  FUSER_FDCR2_RESERVED2_MASK 0xff00
#define  FUSER_FDCR2_RESERVED2_SHIFT 8
#define  FUSER_FDCR2_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FDCR2_RESERVED2_MASK) >> FUSER_FDCR2_RESERVED2_SHIFT)
#define  FUSER_FDCR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR2_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FDCR2_RESERVED2_SHIFT))

#define  FUSER_FDCR2_FUSER_DUTYCYCLE_MASK 0xff
#define  FUSER_FDCR2_FUSER_DUTYCYCLE_SHIFT 0
#define  FUSER_FDCR2_FUSER_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & FUSER_FDCR2_FUSER_DUTYCYCLE_MASK) >> FUSER_FDCR2_FUSER_DUTYCYCLE_SHIFT)
#define  FUSER_FDCR2_FUSER_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FDCR2_FUSER_DUTYCYCLE_MASK) | (((uint32_t)val) << FUSER_FDCR2_FUSER_DUTYCYCLE_SHIFT))

//====================================================================
//Register: Fuser Phase Angle Configuration Register (Instance 1 of 3) (FPACR0)
/** \brief This register allows for further customization of how the triac is
      controlled. These values are in affect for synchronous PWM and DSM modes
      only.
      <br><br>
      Pulse offset determines how long to wait from a zeroCrossDetect
      assertion before enabling the fuser output.
      <br><br>
      Pulse width determines how long the fuser output remains high. Due to
      the nature of the triac, it is not necessary to hold fuser output high
      throughout an entire ac half-cycle.
      <br><br>
      Note the power on default value is a 100 us pulse width with a 100 us pulse offset. (Instance 1 of 3)*/
//====================================================================

#define  FUSER_FPACR0_RESERVED1_MASK 0xfc000000
#define  FUSER_FPACR0_RESERVED1_SHIFT 26
#define  FUSER_FPACR0_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FPACR0_RESERVED1_MASK) >> FUSER_FPACR0_RESERVED1_SHIFT)
#define  FUSER_FPACR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR0_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FPACR0_RESERVED1_SHIFT))

#define  FUSER_FPACR0_FUSER_PULSEWIDTH_MASK 0x3ff0000
#define  FUSER_FPACR0_FUSER_PULSEWIDTH_SHIFT 16
#define  FUSER_FPACR0_FUSER_PULSEWIDTH_MASK_SHIFT(reg) (((reg) & FUSER_FPACR0_FUSER_PULSEWIDTH_MASK) >> FUSER_FPACR0_FUSER_PULSEWIDTH_SHIFT)
#define  FUSER_FPACR0_FUSER_PULSEWIDTH_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR0_FUSER_PULSEWIDTH_MASK) | (((uint32_t)val) << FUSER_FPACR0_FUSER_PULSEWIDTH_SHIFT))

#define  FUSER_FPACR0_FUSER_PULSEOFFSET_MASK 0xffff
#define  FUSER_FPACR0_FUSER_PULSEOFFSET_SHIFT 0
#define  FUSER_FPACR0_FUSER_PULSEOFFSET_MASK_SHIFT(reg) (((reg) & FUSER_FPACR0_FUSER_PULSEOFFSET_MASK) >> FUSER_FPACR0_FUSER_PULSEOFFSET_SHIFT)
#define  FUSER_FPACR0_FUSER_PULSEOFFSET_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR0_FUSER_PULSEOFFSET_MASK) | (((uint32_t)val) << FUSER_FPACR0_FUSER_PULSEOFFSET_SHIFT))

//====================================================================
//Register: Fuser Phase Angle Configuration Register (Instance 2 of 3) (FPACR1)
/** \brief This register allows for further customization of how the triac is
      controlled. These values are in affect for synchronous PWM and DSM modes
      only.
      <br><br>
      Pulse offset determines how long to wait from a zeroCrossDetect
      assertion before enabling the fuser output.
      <br><br>
      Pulse width determines how long the fuser output remains high. Due to
      the nature of the triac, it is not necessary to hold fuser output high
      throughout an entire ac half-cycle.
      <br><br>
      Note the power on default value is a 100 us pulse width with a 100 us pulse offset. (Instance 2 of 3)*/
//====================================================================

#define  FUSER_FPACR1_RESERVED1_MASK 0xfc000000
#define  FUSER_FPACR1_RESERVED1_SHIFT 26
#define  FUSER_FPACR1_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FPACR1_RESERVED1_MASK) >> FUSER_FPACR1_RESERVED1_SHIFT)
#define  FUSER_FPACR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR1_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FPACR1_RESERVED1_SHIFT))

#define  FUSER_FPACR1_FUSER_PULSEWIDTH_MASK 0x3ff0000
#define  FUSER_FPACR1_FUSER_PULSEWIDTH_SHIFT 16
#define  FUSER_FPACR1_FUSER_PULSEWIDTH_MASK_SHIFT(reg) (((reg) & FUSER_FPACR1_FUSER_PULSEWIDTH_MASK) >> FUSER_FPACR1_FUSER_PULSEWIDTH_SHIFT)
#define  FUSER_FPACR1_FUSER_PULSEWIDTH_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR1_FUSER_PULSEWIDTH_MASK) | (((uint32_t)val) << FUSER_FPACR1_FUSER_PULSEWIDTH_SHIFT))

#define  FUSER_FPACR1_FUSER_PULSEOFFSET_MASK 0xffff
#define  FUSER_FPACR1_FUSER_PULSEOFFSET_SHIFT 0
#define  FUSER_FPACR1_FUSER_PULSEOFFSET_MASK_SHIFT(reg) (((reg) & FUSER_FPACR1_FUSER_PULSEOFFSET_MASK) >> FUSER_FPACR1_FUSER_PULSEOFFSET_SHIFT)
#define  FUSER_FPACR1_FUSER_PULSEOFFSET_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR1_FUSER_PULSEOFFSET_MASK) | (((uint32_t)val) << FUSER_FPACR1_FUSER_PULSEOFFSET_SHIFT))

//====================================================================
//Register: Fuser Phase Angle Configuration Register (Instance 3 of 3) (FPACR2)
/** \brief This register allows for further customization of how the triac is
      controlled. These values are in affect for synchronous PWM and DSM modes
      only.
      <br><br>
      Pulse offset determines how long to wait from a zeroCrossDetect
      assertion before enabling the fuser output.
      <br><br>
      Pulse width determines how long the fuser output remains high. Due to
      the nature of the triac, it is not necessary to hold fuser output high
      throughout an entire ac half-cycle.
      <br><br>
      Note the power on default value is a 100 us pulse width with a 100 us pulse offset. (Instance 3 of 3)*/
//====================================================================

#define  FUSER_FPACR2_RESERVED1_MASK 0xfc000000
#define  FUSER_FPACR2_RESERVED1_SHIFT 26
#define  FUSER_FPACR2_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FPACR2_RESERVED1_MASK) >> FUSER_FPACR2_RESERVED1_SHIFT)
#define  FUSER_FPACR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR2_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FPACR2_RESERVED1_SHIFT))

#define  FUSER_FPACR2_FUSER_PULSEWIDTH_MASK 0x3ff0000
#define  FUSER_FPACR2_FUSER_PULSEWIDTH_SHIFT 16
#define  FUSER_FPACR2_FUSER_PULSEWIDTH_MASK_SHIFT(reg) (((reg) & FUSER_FPACR2_FUSER_PULSEWIDTH_MASK) >> FUSER_FPACR2_FUSER_PULSEWIDTH_SHIFT)
#define  FUSER_FPACR2_FUSER_PULSEWIDTH_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR2_FUSER_PULSEWIDTH_MASK) | (((uint32_t)val) << FUSER_FPACR2_FUSER_PULSEWIDTH_SHIFT))

#define  FUSER_FPACR2_FUSER_PULSEOFFSET_MASK 0xffff
#define  FUSER_FPACR2_FUSER_PULSEOFFSET_SHIFT 0
#define  FUSER_FPACR2_FUSER_PULSEOFFSET_MASK_SHIFT(reg) (((reg) & FUSER_FPACR2_FUSER_PULSEOFFSET_MASK) >> FUSER_FPACR2_FUSER_PULSEOFFSET_SHIFT)
#define  FUSER_FPACR2_FUSER_PULSEOFFSET_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FPACR2_FUSER_PULSEOFFSET_MASK) | (((uint32_t)val) << FUSER_FPACR2_FUSER_PULSEOFFSET_SHIFT))

//====================================================================
//Register: Fuser Status Register (FSR)
/** \brief This register specifies indicates the current state of the fuser
      outputs, as well as the fuserOverTemp inputs and the zero-crossing
      detect signal.*/
//====================================================================

#define  FUSER_FSR_ZCTIMER_MASK 0xfff00000
#define  FUSER_FSR_ZCTIMER_SHIFT 20
#define  FUSER_FSR_ZCTIMER_MASK_SHIFT(reg) (((reg) & FUSER_FSR_ZCTIMER_MASK) >> FUSER_FSR_ZCTIMER_SHIFT)
#define  FUSER_FSR_ZCTIMER_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_ZCTIMER_MASK) | (((uint32_t)val) << FUSER_FSR_ZCTIMER_SHIFT))

#define  FUSER_FSR_RESERVED1_MASK 0xe0000
#define  FUSER_FSR_RESERVED1_SHIFT 17
#define  FUSER_FSR_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FSR_RESERVED1_MASK) >> FUSER_FSR_RESERVED1_SHIFT)
#define  FUSER_FSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FSR_RESERVED1_SHIFT))

#define  FUSER_FSR_ZEROCROSSDETECT_MASK 0x10000
#define  FUSER_FSR_ZEROCROSSDETECT_SHIFT 16
#define  FUSER_FSR_ZEROCROSSDETECT_MASK_SHIFT(reg) (((reg) & FUSER_FSR_ZEROCROSSDETECT_MASK) >> FUSER_FSR_ZEROCROSSDETECT_SHIFT)
#define  FUSER_FSR_ZEROCROSSDETECT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_ZEROCROSSDETECT_MASK) | (((uint32_t)val) << FUSER_FSR_ZEROCROSSDETECT_SHIFT))

#define  FUSER_FSR_RESERVED2_MASK 0x8000
#define  FUSER_FSR_RESERVED2_SHIFT 15
#define  FUSER_FSR_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_FSR_RESERVED2_MASK) >> FUSER_FSR_RESERVED2_SHIFT)
#define  FUSER_FSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_RESERVED2_MASK) | (((uint32_t)val) << FUSER_FSR_RESERVED2_SHIFT))

#define  FUSER_FSR_PWM_CYCLE_MASK 0x7000
#define  FUSER_FSR_PWM_CYCLE_SHIFT 12
#define  FUSER_FSR_PWM_CYCLE_MASK_SHIFT(reg) (((reg) & FUSER_FSR_PWM_CYCLE_MASK) >> FUSER_FSR_PWM_CYCLE_SHIFT)
#define  FUSER_FSR_PWM_CYCLE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_PWM_CYCLE_MASK) | (((uint32_t)val) << FUSER_FSR_PWM_CYCLE_SHIFT))

#define  FUSER_FSR_RESERVED3_MASK 0x800
#define  FUSER_FSR_RESERVED3_SHIFT 11
#define  FUSER_FSR_RESERVED3_MASK_SHIFT(reg) (((reg) & FUSER_FSR_RESERVED3_MASK) >> FUSER_FSR_RESERVED3_SHIFT)
#define  FUSER_FSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_RESERVED3_MASK) | (((uint32_t)val) << FUSER_FSR_RESERVED3_SHIFT))

#define  FUSER_FSR_FUSEROVERTEMP_MASK 0x700
#define  FUSER_FSR_FUSEROVERTEMP_SHIFT 8
#define  FUSER_FSR_FUSEROVERTEMP_MASK_SHIFT(reg) (((reg) & FUSER_FSR_FUSEROVERTEMP_MASK) >> FUSER_FSR_FUSEROVERTEMP_SHIFT)
#define  FUSER_FSR_FUSEROVERTEMP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_FUSEROVERTEMP_MASK) | (((uint32_t)val) << FUSER_FSR_FUSEROVERTEMP_SHIFT))

#define  FUSER_FSR_FUSEROE_MASK 0xf0
#define  FUSER_FSR_FUSEROE_SHIFT 4
#define  FUSER_FSR_FUSEROE_MASK_SHIFT(reg) (((reg) & FUSER_FSR_FUSEROE_MASK) >> FUSER_FSR_FUSEROE_SHIFT)
#define  FUSER_FSR_FUSEROE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_FUSEROE_MASK) | (((uint32_t)val) << FUSER_FSR_FUSEROE_SHIFT))

#define  FUSER_FSR_LVPOWERON_MASK 0x8
#define  FUSER_FSR_LVPOWERON_SHIFT 3
#define  FUSER_FSR_LVPOWERON_MASK_SHIFT(reg) (((reg) & FUSER_FSR_LVPOWERON_MASK) >> FUSER_FSR_LVPOWERON_SHIFT)
#define  FUSER_FSR_LVPOWERON_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_LVPOWERON_MASK) | (((uint32_t)val) << FUSER_FSR_LVPOWERON_SHIFT))

#define  FUSER_FSR_FUSER_MASK 0x7
#define  FUSER_FSR_FUSER_SHIFT 0
#define  FUSER_FSR_FUSER_MASK_SHIFT(reg) (((reg) & FUSER_FSR_FUSER_MASK) >> FUSER_FSR_FUSER_SHIFT)
#define  FUSER_FSR_FUSER_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FSR_FUSER_MASK) | (((uint32_t)val) << FUSER_FSR_FUSER_SHIFT))

//====================================================================
//Register: Fuser Interrupt Enable Register (FIER)
/** \brief This register allows for the Fuser interrupts to be individually
      enabled or disabled.
<br>
      A duty-cycle interrupt indicates that the block is ready for an updated
      register value. The interrupt is asserted at the beginning of a Fuser
      duty-cycle to allow firmware one complete cycle time to update any
      register values. The new register values will be used when the next
      duty-cycle begins and will not affect the current cycle. If no duty-cycle
      changes are made the Fuser will continue to use the current settings.
      Firmware may choose to use this interrupt of may choose to
      service the fuser at a fixed interval.
<br>
      If a Fuser register hasn't been updated for the amount of time set by the timeout 
      configuration register, the timeout interrupt is asserted.
<br>
      The over-temperature interrupt is generated when the FuserOverTemp[2:0]
      input is asserted (when enabled).
<br>
      A zeroCross interrupt is generated each time the filtered zeroCrossDetect signal asserts.
<br>
      The timeout and overtemperature interrupts will disable all Fuser outputs.*/
//====================================================================

#define  FUSER_FIER_RESERVED1_MASK 0xffffff00
#define  FUSER_FIER_RESERVED1_SHIFT 8
#define  FUSER_FIER_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FIER_RESERVED1_MASK) >> FUSER_FIER_RESERVED1_SHIFT)
#define  FUSER_FIER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIER_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FIER_RESERVED1_SHIFT))

#define  FUSER_FIER_ZEROCROSS_MASK 0x80
#define  FUSER_FIER_ZEROCROSS_SHIFT 7
#define  FUSER_FIER_ZEROCROSS_MASK_SHIFT(reg) (((reg) & FUSER_FIER_ZEROCROSS_MASK) >> FUSER_FIER_ZEROCROSS_SHIFT)
#define  FUSER_FIER_ZEROCROSS_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIER_ZEROCROSS_MASK) | (((uint32_t)val) << FUSER_FIER_ZEROCROSS_SHIFT))

#define  FUSER_FIER_TIMEOUT_MASK 0x40
#define  FUSER_FIER_TIMEOUT_SHIFT 6
#define  FUSER_FIER_TIMEOUT_MASK_SHIFT(reg) (((reg) & FUSER_FIER_TIMEOUT_MASK) >> FUSER_FIER_TIMEOUT_SHIFT)
#define  FUSER_FIER_TIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIER_TIMEOUT_MASK) | (((uint32_t)val) << FUSER_FIER_TIMEOUT_SHIFT))

#define  FUSER_FIER_FUSEROVERTEMP_MASK 0x38
#define  FUSER_FIER_FUSEROVERTEMP_SHIFT 3
#define  FUSER_FIER_FUSEROVERTEMP_MASK_SHIFT(reg) (((reg) & FUSER_FIER_FUSEROVERTEMP_MASK) >> FUSER_FIER_FUSEROVERTEMP_SHIFT)
#define  FUSER_FIER_FUSEROVERTEMP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIER_FUSEROVERTEMP_MASK) | (((uint32_t)val) << FUSER_FIER_FUSEROVERTEMP_SHIFT))

#define  FUSER_FIER_DUTYCYCLECOMPLETE_MASK 0x7
#define  FUSER_FIER_DUTYCYCLECOMPLETE_SHIFT 0
#define  FUSER_FIER_DUTYCYCLECOMPLETE_MASK_SHIFT(reg) (((reg) & FUSER_FIER_DUTYCYCLECOMPLETE_MASK) >> FUSER_FIER_DUTYCYCLECOMPLETE_SHIFT)
#define  FUSER_FIER_DUTYCYCLECOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIER_DUTYCYCLECOMPLETE_MASK) | (((uint32_t)val) << FUSER_FIER_DUTYCYCLECOMPLETE_SHIFT))

//====================================================================
//Register: Fuser Interrupt Pending Register (FIPR)
/** \brief This register indicates pending interrupts.*/
//====================================================================

#define  FUSER_FIPR_RESERVED1_MASK 0xffffff00
#define  FUSER_FIPR_RESERVED1_SHIFT 8
#define  FUSER_FIPR_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FIPR_RESERVED1_MASK) >> FUSER_FIPR_RESERVED1_SHIFT)
#define  FUSER_FIPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIPR_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FIPR_RESERVED1_SHIFT))

#define  FUSER_FIPR_ZEROCROSS_MASK 0x80
#define  FUSER_FIPR_ZEROCROSS_SHIFT 7
#define  FUSER_FIPR_ZEROCROSS_MASK_SHIFT(reg) (((reg) & FUSER_FIPR_ZEROCROSS_MASK) >> FUSER_FIPR_ZEROCROSS_SHIFT)
#define  FUSER_FIPR_ZEROCROSS_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIPR_ZEROCROSS_MASK) | (((uint32_t)val) << FUSER_FIPR_ZEROCROSS_SHIFT))

#define  FUSER_FIPR_TIMEOUT_MASK 0x40
#define  FUSER_FIPR_TIMEOUT_SHIFT 6
#define  FUSER_FIPR_TIMEOUT_MASK_SHIFT(reg) (((reg) & FUSER_FIPR_TIMEOUT_MASK) >> FUSER_FIPR_TIMEOUT_SHIFT)
#define  FUSER_FIPR_TIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIPR_TIMEOUT_MASK) | (((uint32_t)val) << FUSER_FIPR_TIMEOUT_SHIFT))

#define  FUSER_FIPR_FUSEROVERTEMP_MASK 0x38
#define  FUSER_FIPR_FUSEROVERTEMP_SHIFT 3
#define  FUSER_FIPR_FUSEROVERTEMP_MASK_SHIFT(reg) (((reg) & FUSER_FIPR_FUSEROVERTEMP_MASK) >> FUSER_FIPR_FUSEROVERTEMP_SHIFT)
#define  FUSER_FIPR_FUSEROVERTEMP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIPR_FUSEROVERTEMP_MASK) | (((uint32_t)val) << FUSER_FIPR_FUSEROVERTEMP_SHIFT))

#define  FUSER_FIPR_DUTYCYCLECOMPLETE_MASK 0x7
#define  FUSER_FIPR_DUTYCYCLECOMPLETE_SHIFT 0
#define  FUSER_FIPR_DUTYCYCLECOMPLETE_MASK_SHIFT(reg) (((reg) & FUSER_FIPR_DUTYCYCLECOMPLETE_MASK) >> FUSER_FIPR_DUTYCYCLECOMPLETE_SHIFT)
#define  FUSER_FIPR_DUTYCYCLECOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIPR_DUTYCYCLECOMPLETE_MASK) | (((uint32_t)val) << FUSER_FIPR_DUTYCYCLECOMPLETE_SHIFT))

//====================================================================
//Register: Fuser Interrupt Acknowledge Register (FIAR)
/** \brief Write to this register to clear the corresponding interrupts.*/
//====================================================================

#define  FUSER_FIAR_RESERVED1_MASK 0xffffff00
#define  FUSER_FIAR_RESERVED1_SHIFT 8
#define  FUSER_FIAR_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FIAR_RESERVED1_MASK) >> FUSER_FIAR_RESERVED1_SHIFT)
#define  FUSER_FIAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIAR_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FIAR_RESERVED1_SHIFT))

#define  FUSER_FIAR_ZEROCROSS_MASK 0x80
#define  FUSER_FIAR_ZEROCROSS_SHIFT 7
#define  FUSER_FIAR_ZEROCROSS_MASK_SHIFT(reg) (((reg) & FUSER_FIAR_ZEROCROSS_MASK) >> FUSER_FIAR_ZEROCROSS_SHIFT)
#define  FUSER_FIAR_ZEROCROSS_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIAR_ZEROCROSS_MASK) | (((uint32_t)val) << FUSER_FIAR_ZEROCROSS_SHIFT))

#define  FUSER_FIAR_TIMEOUT_MASK 0x40
#define  FUSER_FIAR_TIMEOUT_SHIFT 6
#define  FUSER_FIAR_TIMEOUT_MASK_SHIFT(reg) (((reg) & FUSER_FIAR_TIMEOUT_MASK) >> FUSER_FIAR_TIMEOUT_SHIFT)
#define  FUSER_FIAR_TIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIAR_TIMEOUT_MASK) | (((uint32_t)val) << FUSER_FIAR_TIMEOUT_SHIFT))

#define  FUSER_FIAR_FUSEROVERTEMP_MASK 0x38
#define  FUSER_FIAR_FUSEROVERTEMP_SHIFT 3
#define  FUSER_FIAR_FUSEROVERTEMP_MASK_SHIFT(reg) (((reg) & FUSER_FIAR_FUSEROVERTEMP_MASK) >> FUSER_FIAR_FUSEROVERTEMP_SHIFT)
#define  FUSER_FIAR_FUSEROVERTEMP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIAR_FUSEROVERTEMP_MASK) | (((uint32_t)val) << FUSER_FIAR_FUSEROVERTEMP_SHIFT))

#define  FUSER_FIAR_DUTYCYCLECOMPLETE_MASK 0x7
#define  FUSER_FIAR_DUTYCYCLECOMPLETE_SHIFT 0
#define  FUSER_FIAR_DUTYCYCLECOMPLETE_MASK_SHIFT(reg) (((reg) & FUSER_FIAR_DUTYCYCLECOMPLETE_MASK) >> FUSER_FIAR_DUTYCYCLECOMPLETE_SHIFT)
#define  FUSER_FIAR_DUTYCYCLECOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIAR_DUTYCYCLECOMPLETE_MASK) | (((uint32_t)val) << FUSER_FIAR_DUTYCYCLECOMPLETE_SHIFT))

//====================================================================
//Register: Force Interrupt Force Register (FIFR)
/** \brief Write to this register to force the corresponding interrupts.*/
//====================================================================

#define  FUSER_FIFR_RESERVED1_MASK 0xffffff00
#define  FUSER_FIFR_RESERVED1_SHIFT 8
#define  FUSER_FIFR_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_FIFR_RESERVED1_MASK) >> FUSER_FIFR_RESERVED1_SHIFT)
#define  FUSER_FIFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIFR_RESERVED1_MASK) | (((uint32_t)val) << FUSER_FIFR_RESERVED1_SHIFT))

#define  FUSER_FIFR_ZEROCROSS_MASK 0x80
#define  FUSER_FIFR_ZEROCROSS_SHIFT 7
#define  FUSER_FIFR_ZEROCROSS_MASK_SHIFT(reg) (((reg) & FUSER_FIFR_ZEROCROSS_MASK) >> FUSER_FIFR_ZEROCROSS_SHIFT)
#define  FUSER_FIFR_ZEROCROSS_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIFR_ZEROCROSS_MASK) | (((uint32_t)val) << FUSER_FIFR_ZEROCROSS_SHIFT))

#define  FUSER_FIFR_TIMEOUT_MASK 0x40
#define  FUSER_FIFR_TIMEOUT_SHIFT 6
#define  FUSER_FIFR_TIMEOUT_MASK_SHIFT(reg) (((reg) & FUSER_FIFR_TIMEOUT_MASK) >> FUSER_FIFR_TIMEOUT_SHIFT)
#define  FUSER_FIFR_TIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIFR_TIMEOUT_MASK) | (((uint32_t)val) << FUSER_FIFR_TIMEOUT_SHIFT))

#define  FUSER_FIFR_FUSEROVERTEMP_MASK 0x38
#define  FUSER_FIFR_FUSEROVERTEMP_SHIFT 3
#define  FUSER_FIFR_FUSEROVERTEMP_MASK_SHIFT(reg) (((reg) & FUSER_FIFR_FUSEROVERTEMP_MASK) >> FUSER_FIFR_FUSEROVERTEMP_SHIFT)
#define  FUSER_FIFR_FUSEROVERTEMP_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIFR_FUSEROVERTEMP_MASK) | (((uint32_t)val) << FUSER_FIFR_FUSEROVERTEMP_SHIFT))

#define  FUSER_FIFR_DUTYCYCLECOMPLETE_MASK 0x7
#define  FUSER_FIFR_DUTYCYCLECOMPLETE_SHIFT 0
#define  FUSER_FIFR_DUTYCYCLECOMPLETE_MASK_SHIFT(reg) (((reg) & FUSER_FIFR_DUTYCYCLECOMPLETE_MASK) >> FUSER_FIFR_DUTYCYCLECOMPLETE_SHIFT)
#define  FUSER_FIFR_DUTYCYCLECOMPLETE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_FIFR_DUTYCYCLECOMPLETE_MASK) | (((uint32_t)val) << FUSER_FIFR_DUTYCYCLECOMPLETE_SHIFT))

//====================================================================
//Register: Zerocrossing Detect Filter register (ZCFR)
/** \brief This register controls a configurable voting filter used on the asynchronous
      Zero Crossing Detect input. The Fuser hardware requires the filtered Zero
      Crossing detect signal to be a one clock wide active high pulse.
      The filter settings below must be followed depending on the input polarity
      of the Zero Crossing Detect signal:
<table align="center"> <tr><td>Zero Crossing Detect Polarity</td><td>outsense</td><td>outtype</td></tr> <tr><td>Active high</td><td>0</td><td>01</td></tr> <tr><td>Active low</td><td>0</td><td>10</td></tr> <tr><td>Toggling</td><td>0</td><td>11</td></tr></table>*/
//====================================================================

#define  FUSER_ZCFR_FALL_LO_WEIGHT_MASK 0xf0000000
#define  FUSER_ZCFR_FALL_LO_WEIGHT_SHIFT 28
#define  FUSER_ZCFR_FALL_LO_WEIGHT_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_FALL_LO_WEIGHT_MASK) >> FUSER_ZCFR_FALL_LO_WEIGHT_SHIFT)
#define  FUSER_ZCFR_FALL_LO_WEIGHT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_FALL_LO_WEIGHT_MASK) | (((uint32_t)val) << FUSER_ZCFR_FALL_LO_WEIGHT_SHIFT))

#define  FUSER_ZCFR_FALL_HI_WEIGHT_MASK 0xf000000
#define  FUSER_ZCFR_FALL_HI_WEIGHT_SHIFT 24
#define  FUSER_ZCFR_FALL_HI_WEIGHT_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_FALL_HI_WEIGHT_MASK) >> FUSER_ZCFR_FALL_HI_WEIGHT_SHIFT)
#define  FUSER_ZCFR_FALL_HI_WEIGHT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_FALL_HI_WEIGHT_MASK) | (((uint32_t)val) << FUSER_ZCFR_FALL_HI_WEIGHT_SHIFT))

#define  FUSER_ZCFR_RISE_LO_WEIGHT_MASK 0xf00000
#define  FUSER_ZCFR_RISE_LO_WEIGHT_SHIFT 20
#define  FUSER_ZCFR_RISE_LO_WEIGHT_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_RISE_LO_WEIGHT_MASK) >> FUSER_ZCFR_RISE_LO_WEIGHT_SHIFT)
#define  FUSER_ZCFR_RISE_LO_WEIGHT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_RISE_LO_WEIGHT_MASK) | (((uint32_t)val) << FUSER_ZCFR_RISE_LO_WEIGHT_SHIFT))

#define  FUSER_ZCFR_RISE_HI_WEIGHT_MASK 0xf0000
#define  FUSER_ZCFR_RISE_HI_WEIGHT_SHIFT 16
#define  FUSER_ZCFR_RISE_HI_WEIGHT_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_RISE_HI_WEIGHT_MASK) >> FUSER_ZCFR_RISE_HI_WEIGHT_SHIFT)
#define  FUSER_ZCFR_RISE_HI_WEIGHT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_RISE_HI_WEIGHT_MASK) | (((uint32_t)val) << FUSER_ZCFR_RISE_HI_WEIGHT_SHIFT))

#define  FUSER_ZCFR_THRESHOLD_MASK 0xff00
#define  FUSER_ZCFR_THRESHOLD_SHIFT 8
#define  FUSER_ZCFR_THRESHOLD_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_THRESHOLD_MASK) >> FUSER_ZCFR_THRESHOLD_SHIFT)
#define  FUSER_ZCFR_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_THRESHOLD_MASK) | (((uint32_t)val) << FUSER_ZCFR_THRESHOLD_SHIFT))

#define  FUSER_ZCFR_RESERVED1_MASK 0x80
#define  FUSER_ZCFR_RESERVED1_SHIFT 7
#define  FUSER_ZCFR_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_RESERVED1_MASK) >> FUSER_ZCFR_RESERVED1_SHIFT)
#define  FUSER_ZCFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_RESERVED1_MASK) | (((uint32_t)val) << FUSER_ZCFR_RESERVED1_SHIFT))

#define  FUSER_ZCFR_TIMEBASE_SEL_MASK 0x70
#define  FUSER_ZCFR_TIMEBASE_SEL_SHIFT 4
#define  FUSER_ZCFR_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_TIMEBASE_SEL_MASK) >> FUSER_ZCFR_TIMEBASE_SEL_SHIFT)
#define  FUSER_ZCFR_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_TIMEBASE_SEL_MASK) | (((uint32_t)val) << FUSER_ZCFR_TIMEBASE_SEL_SHIFT))

#define  FUSER_ZCFR_RESERVED2_MASK 0x8
#define  FUSER_ZCFR_RESERVED2_SHIFT 3
#define  FUSER_ZCFR_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_RESERVED2_MASK) >> FUSER_ZCFR_RESERVED2_SHIFT)
#define  FUSER_ZCFR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_RESERVED2_MASK) | (((uint32_t)val) << FUSER_ZCFR_RESERVED2_SHIFT))

#define  FUSER_ZCFR_OUTSENSE_MASK 0x4
#define  FUSER_ZCFR_OUTSENSE_SHIFT 2
#define  FUSER_ZCFR_OUTSENSE_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_OUTSENSE_MASK) >> FUSER_ZCFR_OUTSENSE_SHIFT)
#define  FUSER_ZCFR_OUTSENSE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_OUTSENSE_MASK) | (((uint32_t)val) << FUSER_ZCFR_OUTSENSE_SHIFT))

#define  FUSER_ZCFR_OUTTYPE_MASK 0x3
#define  FUSER_ZCFR_OUTTYPE_SHIFT 0
#define  FUSER_ZCFR_OUTTYPE_MASK_SHIFT(reg) (((reg) & FUSER_ZCFR_OUTTYPE_MASK) >> FUSER_ZCFR_OUTTYPE_SHIFT)
#define  FUSER_ZCFR_OUTTYPE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCFR_OUTTYPE_MASK) | (((uint32_t)val) << FUSER_ZCFR_OUTTYPE_SHIFT))

//====================================================================
//Register: Zerocrossing Detect Lockout register (ZCLR)
/** \brief This register allows ignoring any transitions on the zero crossing detect input
      for a programmable amount of time. It also selects type of interrupt generated by
      the receipt of a zeroCrossing detect. The interrupt is generated from the filtered
      ZeroCrossing detect signal and will not fire during a lockout period.*/
//====================================================================

#define  FUSER_ZCLR_RESERVED1_MASK 0xffffc000
#define  FUSER_ZCLR_RESERVED1_SHIFT 14
#define  FUSER_ZCLR_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_ZCLR_RESERVED1_MASK) >> FUSER_ZCLR_RESERVED1_SHIFT)
#define  FUSER_ZCLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCLR_RESERVED1_MASK) | (((uint32_t)val) << FUSER_ZCLR_RESERVED1_SHIFT))

#define  FUSER_ZCLR_ZC_INT_TYPE_MASK 0x3000
#define  FUSER_ZCLR_ZC_INT_TYPE_SHIFT 12
#define  FUSER_ZCLR_ZC_INT_TYPE_MASK_SHIFT(reg) (((reg) & FUSER_ZCLR_ZC_INT_TYPE_MASK) >> FUSER_ZCLR_ZC_INT_TYPE_SHIFT)
#define  FUSER_ZCLR_ZC_INT_TYPE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCLR_ZC_INT_TYPE_MASK) | (((uint32_t)val) << FUSER_ZCLR_ZC_INT_TYPE_SHIFT))

#define  FUSER_ZCLR_RESERVED2_MASK 0x800
#define  FUSER_ZCLR_RESERVED2_SHIFT 11
#define  FUSER_ZCLR_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_ZCLR_RESERVED2_MASK) >> FUSER_ZCLR_RESERVED2_SHIFT)
#define  FUSER_ZCLR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCLR_RESERVED2_MASK) | (((uint32_t)val) << FUSER_ZCLR_RESERVED2_SHIFT))

#define  FUSER_ZCLR_LOCKOUT_TIMEBASE_MASK 0x700
#define  FUSER_ZCLR_LOCKOUT_TIMEBASE_SHIFT 8
#define  FUSER_ZCLR_LOCKOUT_TIMEBASE_MASK_SHIFT(reg) (((reg) & FUSER_ZCLR_LOCKOUT_TIMEBASE_MASK) >> FUSER_ZCLR_LOCKOUT_TIMEBASE_SHIFT)
#define  FUSER_ZCLR_LOCKOUT_TIMEBASE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCLR_LOCKOUT_TIMEBASE_MASK) | (((uint32_t)val) << FUSER_ZCLR_LOCKOUT_TIMEBASE_SHIFT))

#define  FUSER_ZCLR_RESERVED3_MASK 0x80
#define  FUSER_ZCLR_RESERVED3_SHIFT 7
#define  FUSER_ZCLR_RESERVED3_MASK_SHIFT(reg) (((reg) & FUSER_ZCLR_RESERVED3_MASK) >> FUSER_ZCLR_RESERVED3_SHIFT)
#define  FUSER_ZCLR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCLR_RESERVED3_MASK) | (((uint32_t)val) << FUSER_ZCLR_RESERVED3_SHIFT))

#define  FUSER_ZCLR_CLR_LOCKOUT_MASK 0x40
#define  FUSER_ZCLR_CLR_LOCKOUT_SHIFT 6
#define  FUSER_ZCLR_CLR_LOCKOUT_MASK_SHIFT(reg) (((reg) & FUSER_ZCLR_CLR_LOCKOUT_MASK) >> FUSER_ZCLR_CLR_LOCKOUT_SHIFT)
#define  FUSER_ZCLR_CLR_LOCKOUT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCLR_CLR_LOCKOUT_MASK) | (((uint32_t)val) << FUSER_ZCLR_CLR_LOCKOUT_SHIFT))

#define  FUSER_ZCLR_LOCKOUT_COUNT_MASK 0x3f
#define  FUSER_ZCLR_LOCKOUT_COUNT_SHIFT 0
#define  FUSER_ZCLR_LOCKOUT_COUNT_MASK_SHIFT(reg) (((reg) & FUSER_ZCLR_LOCKOUT_COUNT_MASK) >> FUSER_ZCLR_LOCKOUT_COUNT_SHIFT)
#define  FUSER_ZCLR_LOCKOUT_COUNT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_ZCLR_LOCKOUT_COUNT_MASK) | (((uint32_t)val) << FUSER_ZCLR_LOCKOUT_COUNT_SHIFT))

//====================================================================
//Register: Timeout Configuration Register (TOCFG)
/** \brief Configures the timeout watchdog.*/
//====================================================================

#define  FUSER_TOCFG_RESERVED1_MASK 0xfffffe00
#define  FUSER_TOCFG_RESERVED1_SHIFT 9
#define  FUSER_TOCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & FUSER_TOCFG_RESERVED1_MASK) >> FUSER_TOCFG_RESERVED1_SHIFT)
#define  FUSER_TOCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FUSER_TOCFG_RESERVED1_MASK) | (((uint32_t)val) << FUSER_TOCFG_RESERVED1_SHIFT))

#define  FUSER_TOCFG_TIMEOUT_TIMEBASE_MASK 0x100
#define  FUSER_TOCFG_TIMEOUT_TIMEBASE_SHIFT 8
#define  FUSER_TOCFG_TIMEOUT_TIMEBASE_MASK_SHIFT(reg) (((reg) & FUSER_TOCFG_TIMEOUT_TIMEBASE_MASK) >> FUSER_TOCFG_TIMEOUT_TIMEBASE_SHIFT)
#define  FUSER_TOCFG_TIMEOUT_TIMEBASE_REPLACE_VAL(reg,val) (((reg) & ~FUSER_TOCFG_TIMEOUT_TIMEBASE_MASK) | (((uint32_t)val) << FUSER_TOCFG_TIMEOUT_TIMEBASE_SHIFT))

#define  FUSER_TOCFG_RESERVED2_MASK 0xc0
#define  FUSER_TOCFG_RESERVED2_SHIFT 6
#define  FUSER_TOCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & FUSER_TOCFG_RESERVED2_MASK) >> FUSER_TOCFG_RESERVED2_SHIFT)
#define  FUSER_TOCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FUSER_TOCFG_RESERVED2_MASK) | (((uint32_t)val) << FUSER_TOCFG_RESERVED2_SHIFT))

#define  FUSER_TOCFG_TIMEOUT_COUNT_MASK 0x3f
#define  FUSER_TOCFG_TIMEOUT_COUNT_SHIFT 0
#define  FUSER_TOCFG_TIMEOUT_COUNT_MASK_SHIFT(reg) (((reg) & FUSER_TOCFG_TIMEOUT_COUNT_MASK) >> FUSER_TOCFG_TIMEOUT_COUNT_SHIFT)
#define  FUSER_TOCFG_TIMEOUT_COUNT_REPLACE_VAL(reg,val) (((reg) & ~FUSER_TOCFG_TIMEOUT_COUNT_MASK) | (((uint32_t)val) << FUSER_TOCFG_TIMEOUT_COUNT_SHIFT))

#endif // FUSER
