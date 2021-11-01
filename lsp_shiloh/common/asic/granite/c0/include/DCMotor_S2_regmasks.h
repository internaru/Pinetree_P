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
 * \file DCMotor_S2_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DCMOTOR_S2_REGMASKS_H_
#define _DCMOTOR_S2_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: DCMotor (DCMotor_S2)
/** \brief Top-level register file for DCMotor*/
//
//====================================================================

//====================================================================
//Register: Motor Control Configuration (MCFG)
/** \brief This register controls the overall configuration of the Motor PWM driver. A number of external integrated chips (Allegro A3950, Allegro 4954, TI-drv8810, etc.) can be implemented by configuring the Motor PWM drive pins appropriately. Each of the six DCMotor output pins can be individually controlled by muxing in one of the two PWM signals, '0', or '1'.*/
//====================================================================

#define  DCMOTOR_S2_MCFG_EN_MASK 0x80000000
#define  DCMOTOR_S2_MCFG_EN_SHIFT 31
#define  DCMOTOR_S2_MCFG_EN_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_EN_MASK) >> DCMOTOR_S2_MCFG_EN_SHIFT)
#define  DCMOTOR_S2_MCFG_EN_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_EN_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_EN_SHIFT))

#define  DCMOTOR_S2_MCFG_RESERVED1_MASK 0x40000000
#define  DCMOTOR_S2_MCFG_RESERVED1_SHIFT 30
#define  DCMOTOR_S2_MCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_RESERVED1_MASK) >> DCMOTOR_S2_MCFG_RESERVED1_SHIFT)
#define  DCMOTOR_S2_MCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_RESERVED1_SHIFT))

#define  DCMOTOR_S2_MCFG_INV_MASK 0x3f000000
#define  DCMOTOR_S2_MCFG_INV_SHIFT 24
#define  DCMOTOR_S2_MCFG_INV_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_INV_MASK) >> DCMOTOR_S2_MCFG_INV_SHIFT)
#define  DCMOTOR_S2_MCFG_INV_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_INV_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_INV_SHIFT))

#define  DCMOTOR_S2_MCFG_RESERVED2_MASK 0x800000
#define  DCMOTOR_S2_MCFG_RESERVED2_SHIFT 23
#define  DCMOTOR_S2_MCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_RESERVED2_MASK) >> DCMOTOR_S2_MCFG_RESERVED2_SHIFT)
#define  DCMOTOR_S2_MCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_RESERVED2_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_RESERVED2_SHIFT))

#define  DCMOTOR_S2_MCFG_PINSEL5_MASK 0x700000
#define  DCMOTOR_S2_MCFG_PINSEL5_SHIFT 20
#define  DCMOTOR_S2_MCFG_PINSEL5_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_PINSEL5_MASK) >> DCMOTOR_S2_MCFG_PINSEL5_SHIFT)
#define  DCMOTOR_S2_MCFG_PINSEL5_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_PINSEL5_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_PINSEL5_SHIFT))

#define  DCMOTOR_S2_MCFG_RESERVED3_MASK 0x80000
#define  DCMOTOR_S2_MCFG_RESERVED3_SHIFT 19
#define  DCMOTOR_S2_MCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_RESERVED3_MASK) >> DCMOTOR_S2_MCFG_RESERVED3_SHIFT)
#define  DCMOTOR_S2_MCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_RESERVED3_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_RESERVED3_SHIFT))

#define  DCMOTOR_S2_MCFG_PINSEL4_MASK 0x70000
#define  DCMOTOR_S2_MCFG_PINSEL4_SHIFT 16
#define  DCMOTOR_S2_MCFG_PINSEL4_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_PINSEL4_MASK) >> DCMOTOR_S2_MCFG_PINSEL4_SHIFT)
#define  DCMOTOR_S2_MCFG_PINSEL4_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_PINSEL4_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_PINSEL4_SHIFT))

#define  DCMOTOR_S2_MCFG_RESERVED4_MASK 0x8000
#define  DCMOTOR_S2_MCFG_RESERVED4_SHIFT 15
#define  DCMOTOR_S2_MCFG_RESERVED4_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_RESERVED4_MASK) >> DCMOTOR_S2_MCFG_RESERVED4_SHIFT)
#define  DCMOTOR_S2_MCFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_RESERVED4_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_RESERVED4_SHIFT))

#define  DCMOTOR_S2_MCFG_PINSEL3_MASK 0x7000
#define  DCMOTOR_S2_MCFG_PINSEL3_SHIFT 12
#define  DCMOTOR_S2_MCFG_PINSEL3_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_PINSEL3_MASK) >> DCMOTOR_S2_MCFG_PINSEL3_SHIFT)
#define  DCMOTOR_S2_MCFG_PINSEL3_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_PINSEL3_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_PINSEL3_SHIFT))

#define  DCMOTOR_S2_MCFG_RESERVED5_MASK 0x800
#define  DCMOTOR_S2_MCFG_RESERVED5_SHIFT 11
#define  DCMOTOR_S2_MCFG_RESERVED5_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_RESERVED5_MASK) >> DCMOTOR_S2_MCFG_RESERVED5_SHIFT)
#define  DCMOTOR_S2_MCFG_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_RESERVED5_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_RESERVED5_SHIFT))

#define  DCMOTOR_S2_MCFG_PINSEL2_MASK 0x700
#define  DCMOTOR_S2_MCFG_PINSEL2_SHIFT 8
#define  DCMOTOR_S2_MCFG_PINSEL2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_PINSEL2_MASK) >> DCMOTOR_S2_MCFG_PINSEL2_SHIFT)
#define  DCMOTOR_S2_MCFG_PINSEL2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_PINSEL2_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_PINSEL2_SHIFT))

#define  DCMOTOR_S2_MCFG_RESERVED6_MASK 0x80
#define  DCMOTOR_S2_MCFG_RESERVED6_SHIFT 7
#define  DCMOTOR_S2_MCFG_RESERVED6_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_RESERVED6_MASK) >> DCMOTOR_S2_MCFG_RESERVED6_SHIFT)
#define  DCMOTOR_S2_MCFG_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_RESERVED6_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_RESERVED6_SHIFT))

#define  DCMOTOR_S2_MCFG_PINSEL1_MASK 0x70
#define  DCMOTOR_S2_MCFG_PINSEL1_SHIFT 4
#define  DCMOTOR_S2_MCFG_PINSEL1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_PINSEL1_MASK) >> DCMOTOR_S2_MCFG_PINSEL1_SHIFT)
#define  DCMOTOR_S2_MCFG_PINSEL1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_PINSEL1_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_PINSEL1_SHIFT))

#define  DCMOTOR_S2_MCFG_RESERVED7_MASK 0x8
#define  DCMOTOR_S2_MCFG_RESERVED7_SHIFT 3
#define  DCMOTOR_S2_MCFG_RESERVED7_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_RESERVED7_MASK) >> DCMOTOR_S2_MCFG_RESERVED7_SHIFT)
#define  DCMOTOR_S2_MCFG_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_RESERVED7_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_RESERVED7_SHIFT))

#define  DCMOTOR_S2_MCFG_PINSEL0_MASK 0x7
#define  DCMOTOR_S2_MCFG_PINSEL0_SHIFT 0
#define  DCMOTOR_S2_MCFG_PINSEL0_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_MCFG_PINSEL0_MASK) >> DCMOTOR_S2_MCFG_PINSEL0_SHIFT)
#define  DCMOTOR_S2_MCFG_PINSEL0_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_MCFG_PINSEL0_MASK) | (((uint32_t)val) << DCMOTOR_S2_MCFG_PINSEL0_SHIFT))

//====================================================================
//Register: Input Pin Configuration (IN_PIN_CFG)
/** \brief This register controls which of the six DCMotor pins are used as the inputs. Note that selecting a pin makes that pin an input, so any pin output settings in the MCFG register are ignored. If an input isn't used, then its select field should be set to 0x7.*/
//====================================================================

#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED1_MASK 0xfffff800
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED1_SHIFT 11
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IN_PIN_CFG_RESERVED1_MASK) >> DCMOTOR_S2_IN_PIN_CFG_RESERVED1_SHIFT)
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IN_PIN_CFG_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_IN_PIN_CFG_RESERVED1_SHIFT))

#define  DCMOTOR_S2_IN_PIN_CFG_S2_SEL_MASK 0x700
#define  DCMOTOR_S2_IN_PIN_CFG_S2_SEL_SHIFT 8
#define  DCMOTOR_S2_IN_PIN_CFG_S2_SEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IN_PIN_CFG_S2_SEL_MASK) >> DCMOTOR_S2_IN_PIN_CFG_S2_SEL_SHIFT)
#define  DCMOTOR_S2_IN_PIN_CFG_S2_SEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IN_PIN_CFG_S2_SEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_IN_PIN_CFG_S2_SEL_SHIFT))

#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED2_MASK 0x80
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED2_SHIFT 7
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IN_PIN_CFG_RESERVED2_MASK) >> DCMOTOR_S2_IN_PIN_CFG_RESERVED2_SHIFT)
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IN_PIN_CFG_RESERVED2_MASK) | (((uint32_t)val) << DCMOTOR_S2_IN_PIN_CFG_RESERVED2_SHIFT))

#define  DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_MASK 0x70
#define  DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_SHIFT 4
#define  DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_MASK) >> DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_SHIFT)
#define  DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_IN_PIN_CFG_ENCB_SEL_SHIFT))

#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED3_MASK 0x8
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED3_SHIFT 3
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IN_PIN_CFG_RESERVED3_MASK) >> DCMOTOR_S2_IN_PIN_CFG_RESERVED3_SHIFT)
#define  DCMOTOR_S2_IN_PIN_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IN_PIN_CFG_RESERVED3_MASK) | (((uint32_t)val) << DCMOTOR_S2_IN_PIN_CFG_RESERVED3_SHIFT))

#define  DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_MASK 0x7
#define  DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_SHIFT 0
#define  DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_MASK) >> DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_SHIFT)
#define  DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_IN_PIN_CFG_ENCA_SEL_SHIFT))

//====================================================================
//Register: PWM Frequency Divider 0 (PDIV0)
/** \brief This register sets the divide ratio for the bus clock, which then clocks PWM Generator 0. The PWM frequency is set by this register and the PWM Period Register (PPER0) setting, as follows:
<P />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>PWM Frequency = BusClock/[(PWM_DIV + 1)*(PWM_PER)].</b>*/
//====================================================================

#define  DCMOTOR_S2_PDIV0_RESERVED1_MASK 0xffffffe0
#define  DCMOTOR_S2_PDIV0_RESERVED1_SHIFT 5
#define  DCMOTOR_S2_PDIV0_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDIV0_RESERVED1_MASK) >> DCMOTOR_S2_PDIV0_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PDIV0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDIV0_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDIV0_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PDIV0_PWMDIV_MASK 0x1f
#define  DCMOTOR_S2_PDIV0_PWMDIV_SHIFT 0
#define  DCMOTOR_S2_PDIV0_PWMDIV_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDIV0_PWMDIV_MASK) >> DCMOTOR_S2_PDIV0_PWMDIV_SHIFT)
#define  DCMOTOR_S2_PDIV0_PWMDIV_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDIV0_PWMDIV_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDIV0_PWMDIV_SHIFT))

//====================================================================
//Register: PWM Period 0 (PPER0)
/** \brief This register sets the PWM0 Period (based on the divided system clock as determined by PDIV0)*/
//====================================================================

#define  DCMOTOR_S2_PPER0_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_PPER0_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_PPER0_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PPER0_RESERVED1_MASK) >> DCMOTOR_S2_PPER0_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PPER0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PPER0_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PPER0_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PPER0_PWMPER_MASK 0xffff
#define  DCMOTOR_S2_PPER0_PWMPER_SHIFT 0
#define  DCMOTOR_S2_PPER0_PWMPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PPER0_PWMPER_MASK) >> DCMOTOR_S2_PPER0_PWMPER_SHIFT)
#define  DCMOTOR_S2_PPER0_PWMPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PPER0_PWMPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_PPER0_PWMPER_SHIFT))

//====================================================================
//Register: PWM Duty Cycle 0 (PDTY0)
/** \brief This register sets the PWM0 Duty Cycle. The Duty Cycle % = (PWM_DTY/PWM_PER)*100, with limitations described below:
<OL>
<LI>PWM_DTY must be < PWM_PER or the PWM signal will be "on" all the time.
<LI>PWM_DTY must be > 1 to produce a PWM output, so the Duty Cycle fractional range is limited to 2/PWM_PER <= Duty Cycle <= 1.
</OL>*/
//====================================================================

#define  DCMOTOR_S2_PDTY0_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_PDTY0_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_PDTY0_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDTY0_RESERVED1_MASK) >> DCMOTOR_S2_PDTY0_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PDTY0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDTY0_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDTY0_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PDTY0_PWMDTY_MASK 0xffff
#define  DCMOTOR_S2_PDTY0_PWMDTY_SHIFT 0
#define  DCMOTOR_S2_PDTY0_PWMDTY_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDTY0_PWMDTY_MASK) >> DCMOTOR_S2_PDTY0_PWMDTY_SHIFT)
#define  DCMOTOR_S2_PDTY0_PWMDTY_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDTY0_PWMDTY_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDTY0_PWMDTY_SHIFT))

//====================================================================
//Register: PWM Delay 0 (PDLY0)
/** \brief Delays the start of the PWM0 generator to avoid power supply glitching
<P />
This register sets a delay value that keeps the PWM0 signal inactive whenenver the PWM generator is initially enabled (MOT_CFG.EN = 1). The reason for two enable conditions is to stagger the starting of the dc motors in the system by using different PWM_DLY values for each motor.*/
//====================================================================

#define  DCMOTOR_S2_PDLY0_RESERVED1_MASK 0xfffffe00
#define  DCMOTOR_S2_PDLY0_RESERVED1_SHIFT 9
#define  DCMOTOR_S2_PDLY0_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDLY0_RESERVED1_MASK) >> DCMOTOR_S2_PDLY0_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PDLY0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDLY0_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDLY0_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PDLY0_PWMDLY_MASK 0x1ff
#define  DCMOTOR_S2_PDLY0_PWMDLY_SHIFT 0
#define  DCMOTOR_S2_PDLY0_PWMDLY_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDLY0_PWMDLY_MASK) >> DCMOTOR_S2_PDLY0_PWMDLY_SHIFT)
#define  DCMOTOR_S2_PDLY0_PWMDLY_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDLY0_PWMDLY_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDLY0_PWMDLY_SHIFT))

//====================================================================
//Register: PWM Frequency Divider 1 (PDIV1)
/** \brief This register sets the divide ratio for the bus clock, which then clocks PWM Generator 1. The PWM frequency is set by this register and the PWM Period Register (PPER1), as follows:
<P />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>PWM Frequency = BusClock/[(PWM_DIV + 1)*(PWM_PER)].</b>*/
//====================================================================

#define  DCMOTOR_S2_PDIV1_RESERVED1_MASK 0xffffffe0
#define  DCMOTOR_S2_PDIV1_RESERVED1_SHIFT 5
#define  DCMOTOR_S2_PDIV1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDIV1_RESERVED1_MASK) >> DCMOTOR_S2_PDIV1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PDIV1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDIV1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDIV1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PDIV1_PWMDIV_MASK 0x1f
#define  DCMOTOR_S2_PDIV1_PWMDIV_SHIFT 0
#define  DCMOTOR_S2_PDIV1_PWMDIV_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDIV1_PWMDIV_MASK) >> DCMOTOR_S2_PDIV1_PWMDIV_SHIFT)
#define  DCMOTOR_S2_PDIV1_PWMDIV_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDIV1_PWMDIV_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDIV1_PWMDIV_SHIFT))

//====================================================================
//Register: PWM Period 1 (PPER1)
/** \brief This register sets the PWM1 Period (based on the divided system clock as determined by PDIV1)*/
//====================================================================

#define  DCMOTOR_S2_PPER1_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_PPER1_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_PPER1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PPER1_RESERVED1_MASK) >> DCMOTOR_S2_PPER1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PPER1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PPER1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PPER1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PPER1_PWMPER_MASK 0xffff
#define  DCMOTOR_S2_PPER1_PWMPER_SHIFT 0
#define  DCMOTOR_S2_PPER1_PWMPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PPER1_PWMPER_MASK) >> DCMOTOR_S2_PPER1_PWMPER_SHIFT)
#define  DCMOTOR_S2_PPER1_PWMPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PPER1_PWMPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_PPER1_PWMPER_SHIFT))

//====================================================================
//Register: PWM Duty Cycle 1 (PDTY1)
/** \brief This register sets the PWM1 Duty Cycle. The Duty Cycle % = (PWM_DTY/PWM_PER)*100, with limitations described below:
<OL>
<LI>PWM_DTY must be < PWM_PER or the PWM signal will be "on" all the time.
<LI>PWM_DTY must be > 1 to produce a PWM output, so the Duty Cycle fractional range is limited to 2/PWM_PER <= Duty Cycle <= 1.
</OL>*/
//====================================================================

#define  DCMOTOR_S2_PDTY1_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_PDTY1_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_PDTY1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDTY1_RESERVED1_MASK) >> DCMOTOR_S2_PDTY1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PDTY1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDTY1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDTY1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PDTY1_PWMDTY_MASK 0xffff
#define  DCMOTOR_S2_PDTY1_PWMDTY_SHIFT 0
#define  DCMOTOR_S2_PDTY1_PWMDTY_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDTY1_PWMDTY_MASK) >> DCMOTOR_S2_PDTY1_PWMDTY_SHIFT)
#define  DCMOTOR_S2_PDTY1_PWMDTY_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDTY1_PWMDTY_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDTY1_PWMDTY_SHIFT))

//====================================================================
//Register: PWM Delay 1 (PDLY1)
/** \brief Delays the start of the PWM1 generator to avoid power supply glitching
<P />
This register sets a delay value that keeps the PWM1 signal inactive whenenver the PWM generator is initially enabled (MOT_CFG.EN = '1'). The reason for two enable conditions is to stagger the starting of the dc motors in the system by using different PWM_DLY values for each motor.*/
//====================================================================

#define  DCMOTOR_S2_PDLY1_RESERVED1_MASK 0xfffffe00
#define  DCMOTOR_S2_PDLY1_RESERVED1_SHIFT 9
#define  DCMOTOR_S2_PDLY1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDLY1_RESERVED1_MASK) >> DCMOTOR_S2_PDLY1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PDLY1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDLY1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDLY1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PDLY1_PWMDLY_MASK 0x1ff
#define  DCMOTOR_S2_PDLY1_PWMDLY_SHIFT 0
#define  DCMOTOR_S2_PDLY1_PWMDLY_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PDLY1_PWMDLY_MASK) >> DCMOTOR_S2_PDLY1_PWMDLY_SHIFT)
#define  DCMOTOR_S2_PDLY1_PWMDLY_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PDLY1_PWMDLY_MASK) | (((uint32_t)val) << DCMOTOR_S2_PDLY1_PWMDLY_SHIFT))

//====================================================================
//Register: PWM Watchdog Control (PWATCH_CTRL)
/** \brief Used to control the PWM Watchdog functionality.*/
//====================================================================

#define  DCMOTOR_S2_PWATCH_CTRL_EN_MASK 0x80000000
#define  DCMOTOR_S2_PWATCH_CTRL_EN_SHIFT 31
#define  DCMOTOR_S2_PWATCH_CTRL_EN_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_CTRL_EN_MASK) >> DCMOTOR_S2_PWATCH_CTRL_EN_SHIFT)
#define  DCMOTOR_S2_PWATCH_CTRL_EN_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_CTRL_EN_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_CTRL_EN_SHIFT))

#define  DCMOTOR_S2_PWATCH_CTRL_TBASESEL_MASK 0x70000000
#define  DCMOTOR_S2_PWATCH_CTRL_TBASESEL_SHIFT 28
#define  DCMOTOR_S2_PWATCH_CTRL_TBASESEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_CTRL_TBASESEL_MASK) >> DCMOTOR_S2_PWATCH_CTRL_TBASESEL_SHIFT)
#define  DCMOTOR_S2_PWATCH_CTRL_TBASESEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_CTRL_TBASESEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_CTRL_TBASESEL_SHIFT))

#define  DCMOTOR_S2_PWATCH_CTRL_RESERVED1_MASK 0xfff0000
#define  DCMOTOR_S2_PWATCH_CTRL_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_PWATCH_CTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_CTRL_RESERVED1_MASK) >> DCMOTOR_S2_PWATCH_CTRL_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PWATCH_CTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_CTRL_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_CTRL_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PWATCH_CTRL_TERMCNT_MASK 0xffff
#define  DCMOTOR_S2_PWATCH_CTRL_TERMCNT_SHIFT 0
#define  DCMOTOR_S2_PWATCH_CTRL_TERMCNT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_CTRL_TERMCNT_MASK) >> DCMOTOR_S2_PWATCH_CTRL_TERMCNT_SHIFT)
#define  DCMOTOR_S2_PWATCH_CTRL_TERMCNT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_CTRL_TERMCNT_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_CTRL_TERMCNT_SHIFT))

//====================================================================
//Register: PWM Watchdog Status (PWATCH_STAT)
/** \brief Read-only register that provides status information on the current state of the PWM Watchdog functionality.*/
//====================================================================

#define  DCMOTOR_S2_PWATCH_STAT_EXPIRED_MASK 0x80000000
#define  DCMOTOR_S2_PWATCH_STAT_EXPIRED_SHIFT 31
#define  DCMOTOR_S2_PWATCH_STAT_EXPIRED_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_STAT_EXPIRED_MASK) >> DCMOTOR_S2_PWATCH_STAT_EXPIRED_SHIFT)
#define  DCMOTOR_S2_PWATCH_STAT_EXPIRED_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_STAT_EXPIRED_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_STAT_EXPIRED_SHIFT))

#define  DCMOTOR_S2_PWATCH_STAT_RESERVED1_MASK 0x7ff00000
#define  DCMOTOR_S2_PWATCH_STAT_RESERVED1_SHIFT 20
#define  DCMOTOR_S2_PWATCH_STAT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_STAT_RESERVED1_MASK) >> DCMOTOR_S2_PWATCH_STAT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PWATCH_STAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_STAT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_STAT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PWATCH_STAT_CNT_MASK 0xffff0
#define  DCMOTOR_S2_PWATCH_STAT_CNT_SHIFT 4
#define  DCMOTOR_S2_PWATCH_STAT_CNT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_STAT_CNT_MASK) >> DCMOTOR_S2_PWATCH_STAT_CNT_SHIFT)
#define  DCMOTOR_S2_PWATCH_STAT_CNT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_STAT_CNT_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_STAT_CNT_SHIFT))

#define  DCMOTOR_S2_PWATCH_STAT_LFSR_MASK 0xf
#define  DCMOTOR_S2_PWATCH_STAT_LFSR_SHIFT 0
#define  DCMOTOR_S2_PWATCH_STAT_LFSR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_STAT_LFSR_MASK) >> DCMOTOR_S2_PWATCH_STAT_LFSR_SHIFT)
#define  DCMOTOR_S2_PWATCH_STAT_LFSR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_STAT_LFSR_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_STAT_LFSR_SHIFT))

//====================================================================
//Register: PWM Watchdog Service (PWATCH_SERVICE)
/** \brief Register used to service the PWM Watchdog.*/
//====================================================================

#define  DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_MASK 0xfffffff0
#define  DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_SHIFT 4
#define  DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_MASK) >> DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_SERVICE_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PWATCH_SERVICE_SERVICE_MASK 0xf
#define  DCMOTOR_S2_PWATCH_SERVICE_SERVICE_SHIFT 0
#define  DCMOTOR_S2_PWATCH_SERVICE_SERVICE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_SERVICE_SERVICE_MASK) >> DCMOTOR_S2_PWATCH_SERVICE_SERVICE_SHIFT)
#define  DCMOTOR_S2_PWATCH_SERVICE_SERVICE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_SERVICE_SERVICE_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_SERVICE_SERVICE_SHIFT))

//====================================================================
//Register: PWM Watchdog Count Write (PWATCH_CNT_WR)
/** \brief Write only register that allows the PWM Watchdog Counter to be changed.*/
//====================================================================

#define  DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_MASK) >> DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_SHIFT)
#define  DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_CNT_WR_RESERVED1_SHIFT))

#define  DCMOTOR_S2_PWATCH_CNT_WR_CNT_MASK 0xffff
#define  DCMOTOR_S2_PWATCH_CNT_WR_CNT_SHIFT 0
#define  DCMOTOR_S2_PWATCH_CNT_WR_CNT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_PWATCH_CNT_WR_CNT_MASK) >> DCMOTOR_S2_PWATCH_CNT_WR_CNT_SHIFT)
#define  DCMOTOR_S2_PWATCH_CNT_WR_CNT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_PWATCH_CNT_WR_CNT_MASK) | (((uint32_t)val) << DCMOTOR_S2_PWATCH_CNT_WR_CNT_SHIFT))

//====================================================================
//Register: Encoder Data Path Configuration (ECFG)
/** \brief Register used to configure and enable the Encoder Data Path*/
//====================================================================

#define  DCMOTOR_S2_ECFG_RESERVED1_MASK 0xf8000000
#define  DCMOTOR_S2_ECFG_RESERVED1_SHIFT 27
#define  DCMOTOR_S2_ECFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_RESERVED1_MASK) >> DCMOTOR_S2_ECFG_RESERVED1_SHIFT)
#define  DCMOTOR_S2_ECFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_RESERVED1_SHIFT))

#define  DCMOTOR_S2_ECFG_INVB_MASK 0x4000000
#define  DCMOTOR_S2_ECFG_INVB_SHIFT 26
#define  DCMOTOR_S2_ECFG_INVB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_INVB_MASK) >> DCMOTOR_S2_ECFG_INVB_SHIFT)
#define  DCMOTOR_S2_ECFG_INVB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_INVB_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_INVB_SHIFT))

#define  DCMOTOR_S2_ECFG_INVA_MASK 0x2000000
#define  DCMOTOR_S2_ECFG_INVA_SHIFT 25
#define  DCMOTOR_S2_ECFG_INVA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_INVA_MASK) >> DCMOTOR_S2_ECFG_INVA_SHIFT)
#define  DCMOTOR_S2_ECFG_INVA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_INVA_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_INVA_SHIFT))

#define  DCMOTOR_S2_ECFG_FSOURCE_MASK 0x1000000
#define  DCMOTOR_S2_ECFG_FSOURCE_SHIFT 24
#define  DCMOTOR_S2_ECFG_FSOURCE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_FSOURCE_MASK) >> DCMOTOR_S2_ECFG_FSOURCE_SHIFT)
#define  DCMOTOR_S2_ECFG_FSOURCE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_FSOURCE_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_FSOURCE_SHIFT))

#define  DCMOTOR_S2_ECFG_FSHIFT_MASK 0xe00000
#define  DCMOTOR_S2_ECFG_FSHIFT_SHIFT 21
#define  DCMOTOR_S2_ECFG_FSHIFT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_FSHIFT_MASK) >> DCMOTOR_S2_ECFG_FSHIFT_SHIFT)
#define  DCMOTOR_S2_ECFG_FSHIFT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_FSHIFT_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_FSHIFT_SHIFT))

#define  DCMOTOR_S2_ECFG_TEST_MASK 0x100000
#define  DCMOTOR_S2_ECFG_TEST_SHIFT 20
#define  DCMOTOR_S2_ECFG_TEST_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_TEST_MASK) >> DCMOTOR_S2_ECFG_TEST_SHIFT)
#define  DCMOTOR_S2_ECFG_TEST_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_TEST_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_TEST_SHIFT))

#define  DCMOTOR_S2_ECFG_ENCEN_MASK 0x80000
#define  DCMOTOR_S2_ECFG_ENCEN_SHIFT 19
#define  DCMOTOR_S2_ECFG_ENCEN_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_ENCEN_MASK) >> DCMOTOR_S2_ECFG_ENCEN_SHIFT)
#define  DCMOTOR_S2_ECFG_ENCEN_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_ENCEN_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_ENCEN_SHIFT))

#define  DCMOTOR_S2_ECFG_RSYNCSEL_MASK 0x60000
#define  DCMOTOR_S2_ECFG_RSYNCSEL_SHIFT 17
#define  DCMOTOR_S2_ECFG_RSYNCSEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_RSYNCSEL_MASK) >> DCMOTOR_S2_ECFG_RSYNCSEL_SHIFT)
#define  DCMOTOR_S2_ECFG_RSYNCSEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_RSYNCSEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_RSYNCSEL_SHIFT))

#define  DCMOTOR_S2_ECFG_ISCALE_MASK 0x1e000
#define  DCMOTOR_S2_ECFG_ISCALE_SHIFT 13
#define  DCMOTOR_S2_ECFG_ISCALE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_ISCALE_MASK) >> DCMOTOR_S2_ECFG_ISCALE_SHIFT)
#define  DCMOTOR_S2_ECFG_ISCALE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_ISCALE_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_ISCALE_SHIFT))

#define  DCMOTOR_S2_ECFG_TBASEEN_MASK 0x1000
#define  DCMOTOR_S2_ECFG_TBASEEN_SHIFT 12
#define  DCMOTOR_S2_ECFG_TBASEEN_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_TBASEEN_MASK) >> DCMOTOR_S2_ECFG_TBASEEN_SHIFT)
#define  DCMOTOR_S2_ECFG_TBASEEN_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_TBASEEN_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_TBASEEN_SHIFT))

#define  DCMOTOR_S2_ECFG_TBASESEL_MASK 0xe00
#define  DCMOTOR_S2_ECFG_TBASESEL_SHIFT 9
#define  DCMOTOR_S2_ECFG_TBASESEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_TBASESEL_MASK) >> DCMOTOR_S2_ECFG_TBASESEL_SHIFT)
#define  DCMOTOR_S2_ECFG_TBASESEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_TBASESEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_TBASESEL_SHIFT))

#define  DCMOTOR_S2_ECFG_ZPMODE_MASK 0x180
#define  DCMOTOR_S2_ECFG_ZPMODE_SHIFT 7
#define  DCMOTOR_S2_ECFG_ZPMODE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_ZPMODE_MASK) >> DCMOTOR_S2_ECFG_ZPMODE_SHIFT)
#define  DCMOTOR_S2_ECFG_ZPMODE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_ZPMODE_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_ZPMODE_SHIFT))

#define  DCMOTOR_S2_ECFG_IMODE_MASK 0x60
#define  DCMOTOR_S2_ECFG_IMODE_SHIFT 5
#define  DCMOTOR_S2_ECFG_IMODE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_IMODE_MASK) >> DCMOTOR_S2_ECFG_IMODE_SHIFT)
#define  DCMOTOR_S2_ECFG_IMODE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_IMODE_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_IMODE_SHIFT))

#define  DCMOTOR_S2_ECFG_RESERVED2_MASK 0x10
#define  DCMOTOR_S2_ECFG_RESERVED2_SHIFT 4
#define  DCMOTOR_S2_ECFG_RESERVED2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_RESERVED2_MASK) >> DCMOTOR_S2_ECFG_RESERVED2_SHIFT)
#define  DCMOTOR_S2_ECFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_RESERVED2_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_RESERVED2_SHIFT))

#define  DCMOTOR_S2_ECFG_PMODE_MASK 0xc
#define  DCMOTOR_S2_ECFG_PMODE_SHIFT 2
#define  DCMOTOR_S2_ECFG_PMODE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_PMODE_MASK) >> DCMOTOR_S2_ECFG_PMODE_SHIFT)
#define  DCMOTOR_S2_ECFG_PMODE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_PMODE_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_PMODE_SHIFT))

#define  DCMOTOR_S2_ECFG_RMODE_MASK 0x3
#define  DCMOTOR_S2_ECFG_RMODE_SHIFT 0
#define  DCMOTOR_S2_ECFG_RMODE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ECFG_RMODE_MASK) >> DCMOTOR_S2_ECFG_RMODE_SHIFT)
#define  DCMOTOR_S2_ECFG_RMODE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ECFG_RMODE_MASK) | (((uint32_t)val) << DCMOTOR_S2_ECFG_RMODE_SHIFT))

//====================================================================
//Register: Soft Reset (SReset)
/** \brief Soft Reset for the block*/
//====================================================================

#define  DCMOTOR_S2_SRESET_RESERVED1_MASK 0xfffffffe
#define  DCMOTOR_S2_SRESET_RESERVED1_SHIFT 1
#define  DCMOTOR_S2_SRESET_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_SRESET_RESERVED1_MASK) >> DCMOTOR_S2_SRESET_RESERVED1_SHIFT)
#define  DCMOTOR_S2_SRESET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_SRESET_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_SRESET_RESERVED1_SHIFT))

#define  DCMOTOR_S2_SRESET_SRESET_MASK 0x1
#define  DCMOTOR_S2_SRESET_SRESET_SHIFT 0
#define  DCMOTOR_S2_SRESET_SRESET_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_SRESET_SRESET_MASK) >> DCMOTOR_S2_SRESET_SRESET_SHIFT)
#define  DCMOTOR_S2_SRESET_SRESET_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_SRESET_SRESET_MASK) | (((uint32_t)val) << DCMOTOR_S2_SRESET_SRESET_SHIFT))

//====================================================================
//Register: Debounce (DB)
/** \brief ENCA/ENCB Debounce value.
<P />
This register is used to debounce the ENCA, ENCB, and ZeroPos inputs. The input signals must be stable for the debounce value to be recognized as a valid input and passed to other Encoder sub-blocks. The upper 16-bits are used for the ZeroPos, and the lower 16-bits for ENCA/ENCB debounce. The debounce counter is clocked by the 1us timebase for ZDB and SysClk for EDB.
<P />
ZDB Debounce range is up to ~65 ms = (2^16) * 1us.
<P />
EDB range is ~300us = (2^16) / SysClk.*/
//====================================================================

#define  DCMOTOR_S2_DB_ZDB_MASK 0xffff0000
#define  DCMOTOR_S2_DB_ZDB_SHIFT 16
#define  DCMOTOR_S2_DB_ZDB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_DB_ZDB_MASK) >> DCMOTOR_S2_DB_ZDB_SHIFT)
#define  DCMOTOR_S2_DB_ZDB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_DB_ZDB_MASK) | (((uint32_t)val) << DCMOTOR_S2_DB_ZDB_SHIFT))

#define  DCMOTOR_S2_DB_EDB_MASK 0xffff
#define  DCMOTOR_S2_DB_EDB_SHIFT 0
#define  DCMOTOR_S2_DB_EDB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_DB_EDB_MASK) >> DCMOTOR_S2_DB_EDB_SHIFT)
#define  DCMOTOR_S2_DB_EDB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_DB_EDB_MASK) | (((uint32_t)val) << DCMOTOR_S2_DB_EDB_SHIFT))

//====================================================================
//Register: Encoder Position Read (EPOS_RD)
/** \brief This register provides the current position and direction based on the ENCA/ENCB input edges from either a quadrature position encoder or single-ended encoder. The register is reset by a Soft Reset. If enabled, the position can also be reset by the ZeroPos input after debouncing and edge detection; The DIR bit is unaffected by the ZeroPos input. When ENCA leads ENCB, DIR = 1, else 0.
<P />
The position counter increments (DIR=1) or decrements (DIR=0) on every ENCA and ENCB edge. If ENC_CFG.IMODE = "00" or "11" then the position count changes on every quadrature edge or at 4*(Encoder frequency). If ENC_CFG.IMODE = "01" or "10" then the position count changes on every ENCA or ENCB edge, or at 2*(Encoder frequency).*/
//====================================================================

#define  DCMOTOR_S2_EPOS_RD_DIR_MASK 0x80000000
#define  DCMOTOR_S2_EPOS_RD_DIR_SHIFT 31
#define  DCMOTOR_S2_EPOS_RD_DIR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_EPOS_RD_DIR_MASK) >> DCMOTOR_S2_EPOS_RD_DIR_SHIFT)
#define  DCMOTOR_S2_EPOS_RD_DIR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_EPOS_RD_DIR_MASK) | (((uint32_t)val) << DCMOTOR_S2_EPOS_RD_DIR_SHIFT))

#define  DCMOTOR_S2_EPOS_RD_POS_MASK 0x7fffffff
#define  DCMOTOR_S2_EPOS_RD_POS_SHIFT 0
#define  DCMOTOR_S2_EPOS_RD_POS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_EPOS_RD_POS_MASK) >> DCMOTOR_S2_EPOS_RD_POS_SHIFT)
#define  DCMOTOR_S2_EPOS_RD_POS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_EPOS_RD_POS_MASK) | (((uint32_t)val) << DCMOTOR_S2_EPOS_RD_POS_SHIFT))

//====================================================================
//Register: Encoder Position Write (EPOS_WR)
//====================================================================

#define  DCMOTOR_S2_EPOS_WR_RESERVED1_MASK 0x80000000
#define  DCMOTOR_S2_EPOS_WR_RESERVED1_SHIFT 31
#define  DCMOTOR_S2_EPOS_WR_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_EPOS_WR_RESERVED1_MASK) >> DCMOTOR_S2_EPOS_WR_RESERVED1_SHIFT)
#define  DCMOTOR_S2_EPOS_WR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_EPOS_WR_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_EPOS_WR_RESERVED1_SHIFT))

#define  DCMOTOR_S2_EPOS_WR_POS_MASK 0x7fffffff
#define  DCMOTOR_S2_EPOS_WR_POS_SHIFT 0
#define  DCMOTOR_S2_EPOS_WR_POS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_EPOS_WR_POS_MASK) >> DCMOTOR_S2_EPOS_WR_POS_SHIFT)
#define  DCMOTOR_S2_EPOS_WR_POS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_EPOS_WR_POS_MASK) | (((uint32_t)val) << DCMOTOR_S2_EPOS_WR_POS_SHIFT))

//====================================================================
//Register: Encoder Latched Position (LPOS)
/** \brief This read-only register is the latched value of the Encoder Position Register. The encoder position is latched on a timebase pulse selected and enabled by ENC_CFG.TBSEL and ENC_CFG.TBEN. The register is reset by a Soft Reset or, if enabled by the ZeroPos input after de-bouncing and edge detection. The DIR bit is unaffected by the ZeroPos input.
<P />
IPEND.TB is set whenever this register is latched.*/
//====================================================================

#define  DCMOTOR_S2_LPOS_DIR_MASK 0x80000000
#define  DCMOTOR_S2_LPOS_DIR_SHIFT 31
#define  DCMOTOR_S2_LPOS_DIR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LPOS_DIR_MASK) >> DCMOTOR_S2_LPOS_DIR_SHIFT)
#define  DCMOTOR_S2_LPOS_DIR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LPOS_DIR_MASK) | (((uint32_t)val) << DCMOTOR_S2_LPOS_DIR_SHIFT))

#define  DCMOTOR_S2_LPOS_LPOS_MASK 0x7fffffff
#define  DCMOTOR_S2_LPOS_LPOS_SHIFT 0
#define  DCMOTOR_S2_LPOS_LPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LPOS_LPOS_MASK) >> DCMOTOR_S2_LPOS_LPOS_SHIFT)
#define  DCMOTOR_S2_LPOS_LPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LPOS_LPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_LPOS_LPOS_SHIFT))

//====================================================================
//Register: ENCA Period (APER)
/** \brief These registers continually measure the period of the ENCA input. If the period is out-of-range then the IPEND.IPER interrupt bit will set. The out-of-range condition = IPEND.IPER is detected by an all 1's condition in the internal 24-bit period measure register before rounding to 23-bits.
<P />
Note that if a motor stops, then the ENC edges will stop transitioning so the illegal period interrupt (IPEND.IPER) will fire.
<P />
This read-only register continuously measures the ENCA or ENCA^ENCB period depending on ENC_CFG.PMODE using the System Clock prescaled by (2^(ENC_CFG.ISCALE)). The register is reset by a Soft Reset.*/
//====================================================================

#define  DCMOTOR_S2_APER_RESERVED1_MASK 0xff800000
#define  DCMOTOR_S2_APER_RESERVED1_SHIFT 23
#define  DCMOTOR_S2_APER_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_APER_RESERVED1_MASK) >> DCMOTOR_S2_APER_RESERVED1_SHIFT)
#define  DCMOTOR_S2_APER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_APER_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_APER_RESERVED1_SHIFT))

#define  DCMOTOR_S2_APER_APER_MASK 0x7fffff
#define  DCMOTOR_S2_APER_APER_SHIFT 0
#define  DCMOTOR_S2_APER_APER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_APER_APER_MASK) >> DCMOTOR_S2_APER_APER_SHIFT)
#define  DCMOTOR_S2_APER_APER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_APER_APER_MASK) | (((uint32_t)val) << DCMOTOR_S2_APER_APER_SHIFT))

//====================================================================
//Register: ENCB Period (BPER)
/** \brief This read-only register continuously measures the ENCB or ENCA^ENCB period depending on ENC_CFG.PMODE, using the System Clock prescaled by (2^ENC_CFG.ISCALE). The register is reset by a Soft Reset.*/
//====================================================================

#define  DCMOTOR_S2_BPER_RESERVED1_MASK 0xff800000
#define  DCMOTOR_S2_BPER_RESERVED1_SHIFT 23
#define  DCMOTOR_S2_BPER_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_BPER_RESERVED1_MASK) >> DCMOTOR_S2_BPER_RESERVED1_SHIFT)
#define  DCMOTOR_S2_BPER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_BPER_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_BPER_RESERVED1_SHIFT))

#define  DCMOTOR_S2_BPER_BPER_MASK 0x7fffff
#define  DCMOTOR_S2_BPER_BPER_SHIFT 0
#define  DCMOTOR_S2_BPER_BPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_BPER_BPER_MASK) >> DCMOTOR_S2_BPER_BPER_SHIFT)
#define  DCMOTOR_S2_BPER_BPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_BPER_BPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_BPER_BPER_SHIFT))

//====================================================================
//Register: Filter Input Period (FILT_IN)
/** \brief This read-only register provdes the current value of the filter input period. The filter input period is a shifted version of APER or BPER depending on the settings of ECFG.Pmode and ECFG.Fshift. If the filter input period is out-of-range then the IPEND.Fiper interrupt bit will set and the FILT_IN register will clamp at its maximum value.*/
//====================================================================

#define  DCMOTOR_S2_FILT_IN_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_FILT_IN_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_FILT_IN_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILT_IN_RESERVED1_MASK) >> DCMOTOR_S2_FILT_IN_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILT_IN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILT_IN_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILT_IN_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILT_IN_FPER_MASK 0xffff
#define  DCMOTOR_S2_FILT_IN_FPER_SHIFT 0
#define  DCMOTOR_S2_FILT_IN_FPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILT_IN_FPER_MASK) >> DCMOTOR_S2_FILT_IN_FPER_SHIFT)
#define  DCMOTOR_S2_FILT_IN_FPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILT_IN_FPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILT_IN_FPER_SHIFT))

//====================================================================
//Register: Test Period (TPER)
/** \brief This register sets the Encoder period in test mode. The value is scaled by ISCALE before generating the ENCA and ENCB waveforms that are input to the position counters and period measurement logic in test mode.*/
//====================================================================

#define  DCMOTOR_S2_TPER_RESERVED1_MASK 0xff800000
#define  DCMOTOR_S2_TPER_RESERVED1_SHIFT 23
#define  DCMOTOR_S2_TPER_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TPER_RESERVED1_MASK) >> DCMOTOR_S2_TPER_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TPER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TPER_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TPER_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TPER_TPER_MASK 0x7fffff
#define  DCMOTOR_S2_TPER_TPER_SHIFT 0
#define  DCMOTOR_S2_TPER_TPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TPER_TPER_MASK) >> DCMOTOR_S2_TPER_TPER_SHIFT)
#define  DCMOTOR_S2_TPER_TPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TPER_TPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_TPER_TPER_SHIFT))

//====================================================================
//Register: Filter Sample Rate (FILT_SAMPLE)
/** \brief Defines filter sample rate.*/
//====================================================================

#define  DCMOTOR_S2_FILT_SAMPLE_RATE_MASK 0xffffffff
#define  DCMOTOR_S2_FILT_SAMPLE_RATE_SHIFT 0
#define  DCMOTOR_S2_FILT_SAMPLE_RATE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILT_SAMPLE_RATE_MASK) >> DCMOTOR_S2_FILT_SAMPLE_RATE_SHIFT)
#define  DCMOTOR_S2_FILT_SAMPLE_RATE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILT_SAMPLE_RATE_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILT_SAMPLE_RATE_SHIFT))

//====================================================================
//Register: Filter Input Gain K1 (FILK1)
/** \brief Filter Input Gain K1*/
//====================================================================

#define  DCMOTOR_S2_FILK1_RESERVED1_MASK 0xffffc000
#define  DCMOTOR_S2_FILK1_RESERVED1_SHIFT 14
#define  DCMOTOR_S2_FILK1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILK1_RESERVED1_MASK) >> DCMOTOR_S2_FILK1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILK1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILK1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILK1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILK1_K1_MASK 0x3fff
#define  DCMOTOR_S2_FILK1_K1_SHIFT 0
#define  DCMOTOR_S2_FILK1_K1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILK1_K1_MASK) >> DCMOTOR_S2_FILK1_K1_SHIFT)
#define  DCMOTOR_S2_FILK1_K1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILK1_K1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILK1_K1_SHIFT))

//====================================================================
//Register: Filter Coefficient A2 (FILA2)
/** \brief Filter Coefficient A2*/
//====================================================================

#define  DCMOTOR_S2_FILA2_RESERVED1_MASK 0xffffc000
#define  DCMOTOR_S2_FILA2_RESERVED1_SHIFT 14
#define  DCMOTOR_S2_FILA2_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILA2_RESERVED1_MASK) >> DCMOTOR_S2_FILA2_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILA2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILA2_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILA2_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILA2_A2_MASK 0x3fff
#define  DCMOTOR_S2_FILA2_A2_SHIFT 0
#define  DCMOTOR_S2_FILA2_A2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILA2_A2_MASK) >> DCMOTOR_S2_FILA2_A2_SHIFT)
#define  DCMOTOR_S2_FILA2_A2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILA2_A2_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILA2_A2_SHIFT))

//====================================================================
//Register: Filter Coefficient A3 (FILA3)
/** \brief Filter Coefficient A3*/
//====================================================================

#define  DCMOTOR_S2_FILA3_RESERVED1_MASK 0xffffc000
#define  DCMOTOR_S2_FILA3_RESERVED1_SHIFT 14
#define  DCMOTOR_S2_FILA3_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILA3_RESERVED1_MASK) >> DCMOTOR_S2_FILA3_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILA3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILA3_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILA3_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILA3_A3_MASK 0x3fff
#define  DCMOTOR_S2_FILA3_A3_SHIFT 0
#define  DCMOTOR_S2_FILA3_A3_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILA3_A3_MASK) >> DCMOTOR_S2_FILA3_A3_SHIFT)
#define  DCMOTOR_S2_FILA3_A3_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILA3_A3_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILA3_A3_SHIFT))

//====================================================================
//Register: Filter Status (FILSTAT)
/** \brief Filter Status*/
//====================================================================

#define  DCMOTOR_S2_FILSTAT_RESERVED1_MASK 0xffffffc0
#define  DCMOTOR_S2_FILSTAT_RESERVED1_SHIFT 6
#define  DCMOTOR_S2_FILSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILSTAT_RESERVED1_MASK) >> DCMOTOR_S2_FILSTAT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILSTAT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILSTAT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILSTAT_STATUS_MASK 0x3f
#define  DCMOTOR_S2_FILSTAT_STATUS_SHIFT 0
#define  DCMOTOR_S2_FILSTAT_STATUS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILSTAT_STATUS_MASK) >> DCMOTOR_S2_FILSTAT_STATUS_SHIFT)
#define  DCMOTOR_S2_FILSTAT_STATUS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILSTAT_STATUS_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILSTAT_STATUS_SHIFT))

//====================================================================
//Register: Filter Delay1 (FILD1)
/** \brief Filter Delay1*/
//====================================================================

#define  DCMOTOR_S2_FILD1_RESERVED1_MASK 0xfffe0000
#define  DCMOTOR_S2_FILD1_RESERVED1_SHIFT 17
#define  DCMOTOR_S2_FILD1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILD1_RESERVED1_MASK) >> DCMOTOR_S2_FILD1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILD1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILD1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILD1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILD1_D1_MASK 0x1ffff
#define  DCMOTOR_S2_FILD1_D1_SHIFT 0
#define  DCMOTOR_S2_FILD1_D1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILD1_D1_MASK) >> DCMOTOR_S2_FILD1_D1_SHIFT)
#define  DCMOTOR_S2_FILD1_D1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILD1_D1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILD1_D1_SHIFT))

//====================================================================
//Register: Filter Delay2 (FILD2)
/** \brief Filter Delay2*/
//====================================================================

#define  DCMOTOR_S2_FILD2_RESERVED1_MASK 0xfffe0000
#define  DCMOTOR_S2_FILD2_RESERVED1_SHIFT 17
#define  DCMOTOR_S2_FILD2_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILD2_RESERVED1_MASK) >> DCMOTOR_S2_FILD2_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILD2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILD2_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILD2_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILD2_D2_MASK 0x1ffff
#define  DCMOTOR_S2_FILD2_D2_SHIFT 0
#define  DCMOTOR_S2_FILD2_D2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILD2_D2_MASK) >> DCMOTOR_S2_FILD2_D2_SHIFT)
#define  DCMOTOR_S2_FILD2_D2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILD2_D2_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILD2_D2_SHIFT))

//====================================================================
//Register: Filter Output (FILTOUT)
/** \brief Filter Output*/
//====================================================================

#define  DCMOTOR_S2_FILTOUT_RESERVED1_MASK 0xfffe0000
#define  DCMOTOR_S2_FILTOUT_RESERVED1_SHIFT 17
#define  DCMOTOR_S2_FILTOUT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILTOUT_RESERVED1_MASK) >> DCMOTOR_S2_FILTOUT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_FILTOUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILTOUT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILTOUT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_FILTOUT_FILTOUT_MASK 0x1ffff
#define  DCMOTOR_S2_FILTOUT_FILTOUT_SHIFT 0
#define  DCMOTOR_S2_FILTOUT_FILTOUT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_FILTOUT_FILTOUT_MASK) >> DCMOTOR_S2_FILTOUT_FILTOUT_SHIFT)
#define  DCMOTOR_S2_FILTOUT_FILTOUT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_FILTOUT_FILTOUT_MASK) | (((uint32_t)val) << DCMOTOR_S2_FILTOUT_FILTOUT_SHIFT))

//====================================================================
//Register: Row Sync Period Multiplier (RS_PM)
/** \brief Defines the scaling used to derive the rowsync output wave from the input encoder period. The input encoder period (pre-Iscaling) is multiplied by scaling factor Mult/(2^Mscale). The final rowsync output must scaled by 1/(2^Oscale) in order to fit in 16bits. Thus the RowSync period is calculated by:
<P />
RowSync Period = Encoder Period * Mult/(2^Mscale) * 1/(2^Oscale)*/
//====================================================================

#define  DCMOTOR_S2_RS_PM_RESERVED1_MASK 0xf0000000
#define  DCMOTOR_S2_RS_PM_RESERVED1_SHIFT 28
#define  DCMOTOR_S2_RS_PM_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_PM_RESERVED1_MASK) >> DCMOTOR_S2_RS_PM_RESERVED1_SHIFT)
#define  DCMOTOR_S2_RS_PM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_PM_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_PM_RESERVED1_SHIFT))

#define  DCMOTOR_S2_RS_PM_OSCALE_MASK 0xf000000
#define  DCMOTOR_S2_RS_PM_OSCALE_SHIFT 24
#define  DCMOTOR_S2_RS_PM_OSCALE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_PM_OSCALE_MASK) >> DCMOTOR_S2_RS_PM_OSCALE_SHIFT)
#define  DCMOTOR_S2_RS_PM_OSCALE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_PM_OSCALE_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_PM_OSCALE_SHIFT))

#define  DCMOTOR_S2_RS_PM_RESERVED2_MASK 0xe00000
#define  DCMOTOR_S2_RS_PM_RESERVED2_SHIFT 21
#define  DCMOTOR_S2_RS_PM_RESERVED2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_PM_RESERVED2_MASK) >> DCMOTOR_S2_RS_PM_RESERVED2_SHIFT)
#define  DCMOTOR_S2_RS_PM_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_PM_RESERVED2_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_PM_RESERVED2_SHIFT))

#define  DCMOTOR_S2_RS_PM_MSCALE_MASK 0x1f0000
#define  DCMOTOR_S2_RS_PM_MSCALE_SHIFT 16
#define  DCMOTOR_S2_RS_PM_MSCALE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_PM_MSCALE_MASK) >> DCMOTOR_S2_RS_PM_MSCALE_SHIFT)
#define  DCMOTOR_S2_RS_PM_MSCALE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_PM_MSCALE_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_PM_MSCALE_SHIFT))

#define  DCMOTOR_S2_RS_PM_RESERVED3_MASK 0xf000
#define  DCMOTOR_S2_RS_PM_RESERVED3_SHIFT 12
#define  DCMOTOR_S2_RS_PM_RESERVED3_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_PM_RESERVED3_MASK) >> DCMOTOR_S2_RS_PM_RESERVED3_SHIFT)
#define  DCMOTOR_S2_RS_PM_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_PM_RESERVED3_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_PM_RESERVED3_SHIFT))

#define  DCMOTOR_S2_RS_PM_MULT_MASK 0xfff
#define  DCMOTOR_S2_RS_PM_MULT_SHIFT 0
#define  DCMOTOR_S2_RS_PM_MULT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_PM_MULT_MASK) >> DCMOTOR_S2_RS_PM_MULT_SHIFT)
#define  DCMOTOR_S2_RS_PM_MULT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_PM_MULT_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_PM_MULT_SHIFT))

//====================================================================
//Register: Row Sync Input Debounce (RS_DB)
/** \brief RowSync Input Debounce*/
//====================================================================

#define  DCMOTOR_S2_RS_DB_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_RS_DB_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_RS_DB_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_DB_RESERVED1_MASK) >> DCMOTOR_S2_RS_DB_RESERVED1_SHIFT)
#define  DCMOTOR_S2_RS_DB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_DB_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_DB_RESERVED1_SHIFT))

#define  DCMOTOR_S2_RS_DB_RSDB_MASK 0xffff
#define  DCMOTOR_S2_RS_DB_RSDB_SHIFT 0
#define  DCMOTOR_S2_RS_DB_RSDB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_DB_RSDB_MASK) >> DCMOTOR_S2_RS_DB_RSDB_SHIFT)
#define  DCMOTOR_S2_RS_DB_RSDB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_DB_RSDB_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_DB_RSDB_SHIFT))

//====================================================================
//Register: Row Sync Delay (RS_DLY)
/** \brief Row Sync Delay. Used to delay the RowSync output wave by the specified number of SysClks when ENCPOS==RSTART.*/
//====================================================================

#define  DCMOTOR_S2_RS_DLY_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_RS_DLY_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_RS_DLY_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_DLY_RESERVED1_MASK) >> DCMOTOR_S2_RS_DLY_RESERVED1_SHIFT)
#define  DCMOTOR_S2_RS_DLY_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_DLY_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_DLY_RESERVED1_SHIFT))

#define  DCMOTOR_S2_RS_DLY_RSDLY_MASK 0xffff
#define  DCMOTOR_S2_RS_DLY_RSDLY_SHIFT 0
#define  DCMOTOR_S2_RS_DLY_RSDLY_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_DLY_RSDLY_MASK) >> DCMOTOR_S2_RS_DLY_RSDLY_SHIFT)
#define  DCMOTOR_S2_RS_DLY_RSDLY_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_DLY_RSDLY_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_DLY_RSDLY_SHIFT))

//====================================================================
//Register: Row Sync Output (RS_OUT)
/** \brief This register is a copy of the internal Row Sync Period register post-scaling. The Row Sync output wave is generated by toggling or pulsing (based on RSEL[0]) at the end of the Row Sync Period count.
<P />
RowSyncFrequency = (ENC_CFG.RSEL[0] == 0) ? RSClk /2*(RS_OUT.PER) : RSClk /(RS_OUT.PER)
where RSClk = SysClk/2^(RS_PM.OSCALE).
<P />
<i>Note: The Row Sync Period out of range condition = IPEND.OPER is detected by an all 1's condition in the internal 17-bit period measure register before rounding to 16-bits; e.g.,
Period_temp[16:0] = {17{1'b1}};</i>*/
//====================================================================

#define  DCMOTOR_S2_RS_OUT_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_RS_OUT_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_RS_OUT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_OUT_RESERVED1_MASK) >> DCMOTOR_S2_RS_OUT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_RS_OUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_OUT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_OUT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_RS_OUT_RSPER_MASK 0xffff
#define  DCMOTOR_S2_RS_OUT_RSPER_SHIFT 0
#define  DCMOTOR_S2_RS_OUT_RSPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RS_OUT_RSPER_MASK) >> DCMOTOR_S2_RS_OUT_RSPER_SHIFT)
#define  DCMOTOR_S2_RS_OUT_RSPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RS_OUT_RSPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_RS_OUT_RSPER_SHIFT))

//====================================================================
//Register: Row Sync Start Position (RSTART)
/** \brief The Row Sync pulse or toggle waveform starts immediately when ENC_POS.P = START. If RS_DLY.DLY > 0, the IntRsyncOut waveform starts after (DLY+1) SysClk's.*/
//====================================================================

#define  DCMOTOR_S2_RSTART_RESERVED1_MASK 0x80000000
#define  DCMOTOR_S2_RSTART_RESERVED1_SHIFT 31
#define  DCMOTOR_S2_RSTART_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RSTART_RESERVED1_MASK) >> DCMOTOR_S2_RSTART_RESERVED1_SHIFT)
#define  DCMOTOR_S2_RSTART_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RSTART_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_RSTART_RESERVED1_SHIFT))

#define  DCMOTOR_S2_RSTART_RSTART_MASK 0x7fffffff
#define  DCMOTOR_S2_RSTART_RSTART_SHIFT 0
#define  DCMOTOR_S2_RSTART_RSTART_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RSTART_RSTART_MASK) >> DCMOTOR_S2_RSTART_RSTART_SHIFT)
#define  DCMOTOR_S2_RSTART_RSTART_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RSTART_RSTART_MASK) | (((uint32_t)val) << DCMOTOR_S2_RSTART_RSTART_SHIFT))

//====================================================================
//Register: Row Sync Stop Position (RSTOP)
/** \brief If RS_STOP.I = 0, the Row Sync pulse or toggle waveform stops immediately when ENC_POS.P = STOP. If the Row Sync outputs are a pulse waveform, the next normally occuring pulse is suppressed. If the Row Sync outputs are a toggle waveform, the waveform returns to 0 on the next SysClk after the STOP condition is reached.
<P />
As noted earlier in the ENC_POS register description, Row Sync outputs can also be disabled by setting ENC_POS.P = 0.*/
//====================================================================

#define  DCMOTOR_S2_RSTOP_IGNORE_MASK 0x80000000
#define  DCMOTOR_S2_RSTOP_IGNORE_SHIFT 31
#define  DCMOTOR_S2_RSTOP_IGNORE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RSTOP_IGNORE_MASK) >> DCMOTOR_S2_RSTOP_IGNORE_SHIFT)
#define  DCMOTOR_S2_RSTOP_IGNORE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RSTOP_IGNORE_MASK) | (((uint32_t)val) << DCMOTOR_S2_RSTOP_IGNORE_SHIFT))

#define  DCMOTOR_S2_RSTOP_RSTOP_MASK 0x7fffffff
#define  DCMOTOR_S2_RSTOP_RSTOP_SHIFT 0
#define  DCMOTOR_S2_RSTOP_RSTOP_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_RSTOP_RSTOP_MASK) >> DCMOTOR_S2_RSTOP_RSTOP_SHIFT)
#define  DCMOTOR_S2_RSTOP_RSTOP_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_RSTOP_RSTOP_MASK) | (((uint32_t)val) << DCMOTOR_S2_RSTOP_RSTOP_SHIFT))

//====================================================================
//Register: Interrupt Enable (IEN)
/** \brief Enables internal interrupt sources*/
//====================================================================

#define  DCMOTOR_S2_IEN_RESERVED1_MASK 0xfffff000
#define  DCMOTOR_S2_IEN_RESERVED1_SHIFT 12
#define  DCMOTOR_S2_IEN_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_RESERVED1_MASK) >> DCMOTOR_S2_IEN_RESERVED1_SHIFT)
#define  DCMOTOR_S2_IEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_RESERVED1_SHIFT))

#define  DCMOTOR_S2_IEN_S2VAL_MASK 0x800
#define  DCMOTOR_S2_IEN_S2VAL_SHIFT 11
#define  DCMOTOR_S2_IEN_S2VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_S2VAL_MASK) >> DCMOTOR_S2_IEN_S2VAL_SHIFT)
#define  DCMOTOR_S2_IEN_S2VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_S2VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_S2VAL_SHIFT))

#define  DCMOTOR_S2_IEN_FIPER_MASK 0x400
#define  DCMOTOR_S2_IEN_FIPER_SHIFT 10
#define  DCMOTOR_S2_IEN_FIPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_FIPER_MASK) >> DCMOTOR_S2_IEN_FIPER_SHIFT)
#define  DCMOTOR_S2_IEN_FIPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_FIPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_FIPER_SHIFT))

#define  DCMOTOR_S2_IEN_TSOVER_MASK 0x200
#define  DCMOTOR_S2_IEN_TSOVER_SHIFT 9
#define  DCMOTOR_S2_IEN_TSOVER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_TSOVER_MASK) >> DCMOTOR_S2_IEN_TSOVER_SHIFT)
#define  DCMOTOR_S2_IEN_TSOVER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_TSOVER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_TSOVER_SHIFT))

#define  DCMOTOR_S2_IEN_ENCPOS_MASK 0x100
#define  DCMOTOR_S2_IEN_ENCPOS_SHIFT 8
#define  DCMOTOR_S2_IEN_ENCPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_ENCPOS_MASK) >> DCMOTOR_S2_IEN_ENCPOS_SHIFT)
#define  DCMOTOR_S2_IEN_ENCPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_ENCPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_ENCPOS_SHIFT))

#define  DCMOTOR_S2_IEN_IENC_MASK 0x80
#define  DCMOTOR_S2_IEN_IENC_SHIFT 7
#define  DCMOTOR_S2_IEN_IENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_IENC_MASK) >> DCMOTOR_S2_IEN_IENC_SHIFT)
#define  DCMOTOR_S2_IEN_IENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_IENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_IENC_SHIFT))

#define  DCMOTOR_S2_IEN_IPER_MASK 0x40
#define  DCMOTOR_S2_IEN_IPER_SHIFT 6
#define  DCMOTOR_S2_IEN_IPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_IPER_MASK) >> DCMOTOR_S2_IEN_IPER_SHIFT)
#define  DCMOTOR_S2_IEN_IPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_IPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_IPER_SHIFT))

#define  DCMOTOR_S2_IEN_FILR_MASK 0x20
#define  DCMOTOR_S2_IEN_FILR_SHIFT 5
#define  DCMOTOR_S2_IEN_FILR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_FILR_MASK) >> DCMOTOR_S2_IEN_FILR_SHIFT)
#define  DCMOTOR_S2_IEN_FILR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_FILR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_FILR_SHIFT))

#define  DCMOTOR_S2_IEN_OUTR_MASK 0x10
#define  DCMOTOR_S2_IEN_OUTR_SHIFT 4
#define  DCMOTOR_S2_IEN_OUTR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_OUTR_MASK) >> DCMOTOR_S2_IEN_OUTR_SHIFT)
#define  DCMOTOR_S2_IEN_OUTR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_OUTR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_OUTR_SHIFT))

#define  DCMOTOR_S2_IEN_WATCH_MASK 0x8
#define  DCMOTOR_S2_IEN_WATCH_SHIFT 3
#define  DCMOTOR_S2_IEN_WATCH_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_WATCH_MASK) >> DCMOTOR_S2_IEN_WATCH_SHIFT)
#define  DCMOTOR_S2_IEN_WATCH_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_WATCH_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_WATCH_SHIFT))

#define  DCMOTOR_S2_IEN_ZPOS_MASK 0x4
#define  DCMOTOR_S2_IEN_ZPOS_SHIFT 2
#define  DCMOTOR_S2_IEN_ZPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_ZPOS_MASK) >> DCMOTOR_S2_IEN_ZPOS_SHIFT)
#define  DCMOTOR_S2_IEN_ZPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_ZPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_ZPOS_SHIFT))

#define  DCMOTOR_S2_IEN_ENC_MASK 0x2
#define  DCMOTOR_S2_IEN_ENC_SHIFT 1
#define  DCMOTOR_S2_IEN_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_ENC_MASK) >> DCMOTOR_S2_IEN_ENC_SHIFT)
#define  DCMOTOR_S2_IEN_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_ENC_SHIFT))

#define  DCMOTOR_S2_IEN_TBASE_MASK 0x1
#define  DCMOTOR_S2_IEN_TBASE_SHIFT 0
#define  DCMOTOR_S2_IEN_TBASE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IEN_TBASE_MASK) >> DCMOTOR_S2_IEN_TBASE_SHIFT)
#define  DCMOTOR_S2_IEN_TBASE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IEN_TBASE_MASK) | (((uint32_t)val) << DCMOTOR_S2_IEN_TBASE_SHIFT))

//====================================================================
//Register: Timestamp/EPOS Interrupt Enable (TS_IEN)
/** \brief Enables internal interrupt sources for timestamp and encoder position capture (see INT_ENC_TS/INT_TS/INT_EPOS registers)*/
//====================================================================

#define  DCMOTOR_S2_TS_IEN_RESERVED1_MASK 0xfffff000
#define  DCMOTOR_S2_TS_IEN_RESERVED1_SHIFT 12
#define  DCMOTOR_S2_TS_IEN_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_RESERVED1_MASK) >> DCMOTOR_S2_TS_IEN_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_IEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_IEN_S2VAL_MASK 0x800
#define  DCMOTOR_S2_TS_IEN_S2VAL_SHIFT 11
#define  DCMOTOR_S2_TS_IEN_S2VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_S2VAL_MASK) >> DCMOTOR_S2_TS_IEN_S2VAL_SHIFT)
#define  DCMOTOR_S2_TS_IEN_S2VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_S2VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_S2VAL_SHIFT))

#define  DCMOTOR_S2_TS_IEN_FIPER_MASK 0x400
#define  DCMOTOR_S2_TS_IEN_FIPER_SHIFT 10
#define  DCMOTOR_S2_TS_IEN_FIPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_FIPER_MASK) >> DCMOTOR_S2_TS_IEN_FIPER_SHIFT)
#define  DCMOTOR_S2_TS_IEN_FIPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_FIPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_FIPER_SHIFT))

#define  DCMOTOR_S2_TS_IEN_TSOVER_MASK 0x200
#define  DCMOTOR_S2_TS_IEN_TSOVER_SHIFT 9
#define  DCMOTOR_S2_TS_IEN_TSOVER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_TSOVER_MASK) >> DCMOTOR_S2_TS_IEN_TSOVER_SHIFT)
#define  DCMOTOR_S2_TS_IEN_TSOVER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_TSOVER_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_TSOVER_SHIFT))

#define  DCMOTOR_S2_TS_IEN_ENCPOS_MASK 0x100
#define  DCMOTOR_S2_TS_IEN_ENCPOS_SHIFT 8
#define  DCMOTOR_S2_TS_IEN_ENCPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_ENCPOS_MASK) >> DCMOTOR_S2_TS_IEN_ENCPOS_SHIFT)
#define  DCMOTOR_S2_TS_IEN_ENCPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_ENCPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_ENCPOS_SHIFT))

#define  DCMOTOR_S2_TS_IEN_IENC_MASK 0x80
#define  DCMOTOR_S2_TS_IEN_IENC_SHIFT 7
#define  DCMOTOR_S2_TS_IEN_IENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_IENC_MASK) >> DCMOTOR_S2_TS_IEN_IENC_SHIFT)
#define  DCMOTOR_S2_TS_IEN_IENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_IENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_IENC_SHIFT))

#define  DCMOTOR_S2_TS_IEN_IPER_MASK 0x40
#define  DCMOTOR_S2_TS_IEN_IPER_SHIFT 6
#define  DCMOTOR_S2_TS_IEN_IPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_IPER_MASK) >> DCMOTOR_S2_TS_IEN_IPER_SHIFT)
#define  DCMOTOR_S2_TS_IEN_IPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_IPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_IPER_SHIFT))

#define  DCMOTOR_S2_TS_IEN_FILR_MASK 0x20
#define  DCMOTOR_S2_TS_IEN_FILR_SHIFT 5
#define  DCMOTOR_S2_TS_IEN_FILR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_FILR_MASK) >> DCMOTOR_S2_TS_IEN_FILR_SHIFT)
#define  DCMOTOR_S2_TS_IEN_FILR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_FILR_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_FILR_SHIFT))

#define  DCMOTOR_S2_TS_IEN_OUTR_MASK 0x10
#define  DCMOTOR_S2_TS_IEN_OUTR_SHIFT 4
#define  DCMOTOR_S2_TS_IEN_OUTR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_OUTR_MASK) >> DCMOTOR_S2_TS_IEN_OUTR_SHIFT)
#define  DCMOTOR_S2_TS_IEN_OUTR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_OUTR_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_OUTR_SHIFT))

#define  DCMOTOR_S2_TS_IEN_WATCH_MASK 0x8
#define  DCMOTOR_S2_TS_IEN_WATCH_SHIFT 3
#define  DCMOTOR_S2_TS_IEN_WATCH_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_WATCH_MASK) >> DCMOTOR_S2_TS_IEN_WATCH_SHIFT)
#define  DCMOTOR_S2_TS_IEN_WATCH_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_WATCH_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_WATCH_SHIFT))

#define  DCMOTOR_S2_TS_IEN_ZPOS_MASK 0x4
#define  DCMOTOR_S2_TS_IEN_ZPOS_SHIFT 2
#define  DCMOTOR_S2_TS_IEN_ZPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_ZPOS_MASK) >> DCMOTOR_S2_TS_IEN_ZPOS_SHIFT)
#define  DCMOTOR_S2_TS_IEN_ZPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_ZPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_ZPOS_SHIFT))

#define  DCMOTOR_S2_TS_IEN_ENC_MASK 0x2
#define  DCMOTOR_S2_TS_IEN_ENC_SHIFT 1
#define  DCMOTOR_S2_TS_IEN_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_ENC_MASK) >> DCMOTOR_S2_TS_IEN_ENC_SHIFT)
#define  DCMOTOR_S2_TS_IEN_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_ENC_SHIFT))

#define  DCMOTOR_S2_TS_IEN_TBASE_MASK 0x1
#define  DCMOTOR_S2_TS_IEN_TBASE_SHIFT 0
#define  DCMOTOR_S2_TS_IEN_TBASE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_IEN_TBASE_MASK) >> DCMOTOR_S2_TS_IEN_TBASE_SHIFT)
#define  DCMOTOR_S2_TS_IEN_TBASE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_IEN_TBASE_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_IEN_TBASE_SHIFT))

//====================================================================
//Register: Interrupt Pending (IPEND)
/** \brief Read Only register that indicates the current status of the internal interrupt sources.*/
//====================================================================

#define  DCMOTOR_S2_IPEND_RESERVED1_MASK 0xfffff000
#define  DCMOTOR_S2_IPEND_RESERVED1_SHIFT 12
#define  DCMOTOR_S2_IPEND_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_RESERVED1_MASK) >> DCMOTOR_S2_IPEND_RESERVED1_SHIFT)
#define  DCMOTOR_S2_IPEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_RESERVED1_SHIFT))

#define  DCMOTOR_S2_IPEND_S2VAL_MASK 0x800
#define  DCMOTOR_S2_IPEND_S2VAL_SHIFT 11
#define  DCMOTOR_S2_IPEND_S2VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_S2VAL_MASK) >> DCMOTOR_S2_IPEND_S2VAL_SHIFT)
#define  DCMOTOR_S2_IPEND_S2VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_S2VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_S2VAL_SHIFT))

#define  DCMOTOR_S2_IPEND_FIPER_MASK 0x400
#define  DCMOTOR_S2_IPEND_FIPER_SHIFT 10
#define  DCMOTOR_S2_IPEND_FIPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_FIPER_MASK) >> DCMOTOR_S2_IPEND_FIPER_SHIFT)
#define  DCMOTOR_S2_IPEND_FIPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_FIPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_FIPER_SHIFT))

#define  DCMOTOR_S2_IPEND_TSOVER_MASK 0x200
#define  DCMOTOR_S2_IPEND_TSOVER_SHIFT 9
#define  DCMOTOR_S2_IPEND_TSOVER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_TSOVER_MASK) >> DCMOTOR_S2_IPEND_TSOVER_SHIFT)
#define  DCMOTOR_S2_IPEND_TSOVER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_TSOVER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_TSOVER_SHIFT))

#define  DCMOTOR_S2_IPEND_ENCPOS_MASK 0x100
#define  DCMOTOR_S2_IPEND_ENCPOS_SHIFT 8
#define  DCMOTOR_S2_IPEND_ENCPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_ENCPOS_MASK) >> DCMOTOR_S2_IPEND_ENCPOS_SHIFT)
#define  DCMOTOR_S2_IPEND_ENCPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_ENCPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_ENCPOS_SHIFT))

#define  DCMOTOR_S2_IPEND_IENC_MASK 0x80
#define  DCMOTOR_S2_IPEND_IENC_SHIFT 7
#define  DCMOTOR_S2_IPEND_IENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_IENC_MASK) >> DCMOTOR_S2_IPEND_IENC_SHIFT)
#define  DCMOTOR_S2_IPEND_IENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_IENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_IENC_SHIFT))

#define  DCMOTOR_S2_IPEND_IPER_MASK 0x40
#define  DCMOTOR_S2_IPEND_IPER_SHIFT 6
#define  DCMOTOR_S2_IPEND_IPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_IPER_MASK) >> DCMOTOR_S2_IPEND_IPER_SHIFT)
#define  DCMOTOR_S2_IPEND_IPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_IPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_IPER_SHIFT))

#define  DCMOTOR_S2_IPEND_FILR_MASK 0x20
#define  DCMOTOR_S2_IPEND_FILR_SHIFT 5
#define  DCMOTOR_S2_IPEND_FILR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_FILR_MASK) >> DCMOTOR_S2_IPEND_FILR_SHIFT)
#define  DCMOTOR_S2_IPEND_FILR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_FILR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_FILR_SHIFT))

#define  DCMOTOR_S2_IPEND_OUTR_MASK 0x10
#define  DCMOTOR_S2_IPEND_OUTR_SHIFT 4
#define  DCMOTOR_S2_IPEND_OUTR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_OUTR_MASK) >> DCMOTOR_S2_IPEND_OUTR_SHIFT)
#define  DCMOTOR_S2_IPEND_OUTR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_OUTR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_OUTR_SHIFT))

#define  DCMOTOR_S2_IPEND_WATCH_MASK 0x8
#define  DCMOTOR_S2_IPEND_WATCH_SHIFT 3
#define  DCMOTOR_S2_IPEND_WATCH_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_WATCH_MASK) >> DCMOTOR_S2_IPEND_WATCH_SHIFT)
#define  DCMOTOR_S2_IPEND_WATCH_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_WATCH_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_WATCH_SHIFT))

#define  DCMOTOR_S2_IPEND_ZPOS_MASK 0x4
#define  DCMOTOR_S2_IPEND_ZPOS_SHIFT 2
#define  DCMOTOR_S2_IPEND_ZPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_ZPOS_MASK) >> DCMOTOR_S2_IPEND_ZPOS_SHIFT)
#define  DCMOTOR_S2_IPEND_ZPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_ZPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_ZPOS_SHIFT))

#define  DCMOTOR_S2_IPEND_ENC_MASK 0x2
#define  DCMOTOR_S2_IPEND_ENC_SHIFT 1
#define  DCMOTOR_S2_IPEND_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_ENC_MASK) >> DCMOTOR_S2_IPEND_ENC_SHIFT)
#define  DCMOTOR_S2_IPEND_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_ENC_SHIFT))

#define  DCMOTOR_S2_IPEND_TBASE_MASK 0x1
#define  DCMOTOR_S2_IPEND_TBASE_SHIFT 0
#define  DCMOTOR_S2_IPEND_TBASE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IPEND_TBASE_MASK) >> DCMOTOR_S2_IPEND_TBASE_SHIFT)
#define  DCMOTOR_S2_IPEND_TBASE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IPEND_TBASE_MASK) | (((uint32_t)val) << DCMOTOR_S2_IPEND_TBASE_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge (IACK)
/** \brief Write-only register used to clear pending interrupts. Write a '1' to a bit to clear the corresponding interrupt.*/
//====================================================================

#define  DCMOTOR_S2_IACK_RESERVED1_MASK 0xfffff000
#define  DCMOTOR_S2_IACK_RESERVED1_SHIFT 12
#define  DCMOTOR_S2_IACK_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_RESERVED1_MASK) >> DCMOTOR_S2_IACK_RESERVED1_SHIFT)
#define  DCMOTOR_S2_IACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_RESERVED1_SHIFT))

#define  DCMOTOR_S2_IACK_S2VAL_MASK 0x800
#define  DCMOTOR_S2_IACK_S2VAL_SHIFT 11
#define  DCMOTOR_S2_IACK_S2VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_S2VAL_MASK) >> DCMOTOR_S2_IACK_S2VAL_SHIFT)
#define  DCMOTOR_S2_IACK_S2VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_S2VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_S2VAL_SHIFT))

#define  DCMOTOR_S2_IACK_FIPER_MASK 0x400
#define  DCMOTOR_S2_IACK_FIPER_SHIFT 10
#define  DCMOTOR_S2_IACK_FIPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_FIPER_MASK) >> DCMOTOR_S2_IACK_FIPER_SHIFT)
#define  DCMOTOR_S2_IACK_FIPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_FIPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_FIPER_SHIFT))

#define  DCMOTOR_S2_IACK_TSOVER_MASK 0x200
#define  DCMOTOR_S2_IACK_TSOVER_SHIFT 9
#define  DCMOTOR_S2_IACK_TSOVER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_TSOVER_MASK) >> DCMOTOR_S2_IACK_TSOVER_SHIFT)
#define  DCMOTOR_S2_IACK_TSOVER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_TSOVER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_TSOVER_SHIFT))

#define  DCMOTOR_S2_IACK_ENCPOS_MASK 0x100
#define  DCMOTOR_S2_IACK_ENCPOS_SHIFT 8
#define  DCMOTOR_S2_IACK_ENCPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_ENCPOS_MASK) >> DCMOTOR_S2_IACK_ENCPOS_SHIFT)
#define  DCMOTOR_S2_IACK_ENCPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_ENCPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_ENCPOS_SHIFT))

#define  DCMOTOR_S2_IACK_IENC_MASK 0x80
#define  DCMOTOR_S2_IACK_IENC_SHIFT 7
#define  DCMOTOR_S2_IACK_IENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_IENC_MASK) >> DCMOTOR_S2_IACK_IENC_SHIFT)
#define  DCMOTOR_S2_IACK_IENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_IENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_IENC_SHIFT))

#define  DCMOTOR_S2_IACK_IPER_MASK 0x40
#define  DCMOTOR_S2_IACK_IPER_SHIFT 6
#define  DCMOTOR_S2_IACK_IPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_IPER_MASK) >> DCMOTOR_S2_IACK_IPER_SHIFT)
#define  DCMOTOR_S2_IACK_IPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_IPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_IPER_SHIFT))

#define  DCMOTOR_S2_IACK_FILR_MASK 0x20
#define  DCMOTOR_S2_IACK_FILR_SHIFT 5
#define  DCMOTOR_S2_IACK_FILR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_FILR_MASK) >> DCMOTOR_S2_IACK_FILR_SHIFT)
#define  DCMOTOR_S2_IACK_FILR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_FILR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_FILR_SHIFT))

#define  DCMOTOR_S2_IACK_OUTR_MASK 0x10
#define  DCMOTOR_S2_IACK_OUTR_SHIFT 4
#define  DCMOTOR_S2_IACK_OUTR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_OUTR_MASK) >> DCMOTOR_S2_IACK_OUTR_SHIFT)
#define  DCMOTOR_S2_IACK_OUTR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_OUTR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_OUTR_SHIFT))

#define  DCMOTOR_S2_IACK_WATCH_MASK 0x8
#define  DCMOTOR_S2_IACK_WATCH_SHIFT 3
#define  DCMOTOR_S2_IACK_WATCH_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_WATCH_MASK) >> DCMOTOR_S2_IACK_WATCH_SHIFT)
#define  DCMOTOR_S2_IACK_WATCH_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_WATCH_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_WATCH_SHIFT))

#define  DCMOTOR_S2_IACK_ZPOS_MASK 0x4
#define  DCMOTOR_S2_IACK_ZPOS_SHIFT 2
#define  DCMOTOR_S2_IACK_ZPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_ZPOS_MASK) >> DCMOTOR_S2_IACK_ZPOS_SHIFT)
#define  DCMOTOR_S2_IACK_ZPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_ZPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_ZPOS_SHIFT))

#define  DCMOTOR_S2_IACK_ENC_MASK 0x2
#define  DCMOTOR_S2_IACK_ENC_SHIFT 1
#define  DCMOTOR_S2_IACK_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_ENC_MASK) >> DCMOTOR_S2_IACK_ENC_SHIFT)
#define  DCMOTOR_S2_IACK_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_ENC_SHIFT))

#define  DCMOTOR_S2_IACK_TBASE_MASK 0x1
#define  DCMOTOR_S2_IACK_TBASE_SHIFT 0
#define  DCMOTOR_S2_IACK_TBASE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IACK_TBASE_MASK) >> DCMOTOR_S2_IACK_TBASE_SHIFT)
#define  DCMOTOR_S2_IACK_TBASE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IACK_TBASE_MASK) | (((uint32_t)val) << DCMOTOR_S2_IACK_TBASE_SHIFT))

//====================================================================
//Register: Interrupt Force (IFORCE)
/** \brief Write-only register used to set interrupts. Write a '1' to a bit to set the corresponding interrupt.*/
//====================================================================

#define  DCMOTOR_S2_IFORCE_RESERVED1_MASK 0xfffff000
#define  DCMOTOR_S2_IFORCE_RESERVED1_SHIFT 12
#define  DCMOTOR_S2_IFORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_RESERVED1_MASK) >> DCMOTOR_S2_IFORCE_RESERVED1_SHIFT)
#define  DCMOTOR_S2_IFORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_RESERVED1_SHIFT))

#define  DCMOTOR_S2_IFORCE_S2VAL_MASK 0x800
#define  DCMOTOR_S2_IFORCE_S2VAL_SHIFT 11
#define  DCMOTOR_S2_IFORCE_S2VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_S2VAL_MASK) >> DCMOTOR_S2_IFORCE_S2VAL_SHIFT)
#define  DCMOTOR_S2_IFORCE_S2VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_S2VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_S2VAL_SHIFT))

#define  DCMOTOR_S2_IFORCE_FIPER_MASK 0x400
#define  DCMOTOR_S2_IFORCE_FIPER_SHIFT 10
#define  DCMOTOR_S2_IFORCE_FIPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_FIPER_MASK) >> DCMOTOR_S2_IFORCE_FIPER_SHIFT)
#define  DCMOTOR_S2_IFORCE_FIPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_FIPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_FIPER_SHIFT))

#define  DCMOTOR_S2_IFORCE_TSOVER_MASK 0x200
#define  DCMOTOR_S2_IFORCE_TSOVER_SHIFT 9
#define  DCMOTOR_S2_IFORCE_TSOVER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_TSOVER_MASK) >> DCMOTOR_S2_IFORCE_TSOVER_SHIFT)
#define  DCMOTOR_S2_IFORCE_TSOVER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_TSOVER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_TSOVER_SHIFT))

#define  DCMOTOR_S2_IFORCE_ENCPOS_MASK 0x100
#define  DCMOTOR_S2_IFORCE_ENCPOS_SHIFT 8
#define  DCMOTOR_S2_IFORCE_ENCPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_ENCPOS_MASK) >> DCMOTOR_S2_IFORCE_ENCPOS_SHIFT)
#define  DCMOTOR_S2_IFORCE_ENCPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_ENCPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_ENCPOS_SHIFT))

#define  DCMOTOR_S2_IFORCE_IENC_MASK 0x80
#define  DCMOTOR_S2_IFORCE_IENC_SHIFT 7
#define  DCMOTOR_S2_IFORCE_IENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_IENC_MASK) >> DCMOTOR_S2_IFORCE_IENC_SHIFT)
#define  DCMOTOR_S2_IFORCE_IENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_IENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_IENC_SHIFT))

#define  DCMOTOR_S2_IFORCE_IPER_MASK 0x40
#define  DCMOTOR_S2_IFORCE_IPER_SHIFT 6
#define  DCMOTOR_S2_IFORCE_IPER_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_IPER_MASK) >> DCMOTOR_S2_IFORCE_IPER_SHIFT)
#define  DCMOTOR_S2_IFORCE_IPER_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_IPER_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_IPER_SHIFT))

#define  DCMOTOR_S2_IFORCE_FILR_MASK 0x20
#define  DCMOTOR_S2_IFORCE_FILR_SHIFT 5
#define  DCMOTOR_S2_IFORCE_FILR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_FILR_MASK) >> DCMOTOR_S2_IFORCE_FILR_SHIFT)
#define  DCMOTOR_S2_IFORCE_FILR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_FILR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_FILR_SHIFT))

#define  DCMOTOR_S2_IFORCE_OUTR_MASK 0x10
#define  DCMOTOR_S2_IFORCE_OUTR_SHIFT 4
#define  DCMOTOR_S2_IFORCE_OUTR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_OUTR_MASK) >> DCMOTOR_S2_IFORCE_OUTR_SHIFT)
#define  DCMOTOR_S2_IFORCE_OUTR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_OUTR_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_OUTR_SHIFT))

#define  DCMOTOR_S2_IFORCE_WATCH_MASK 0x8
#define  DCMOTOR_S2_IFORCE_WATCH_SHIFT 3
#define  DCMOTOR_S2_IFORCE_WATCH_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_WATCH_MASK) >> DCMOTOR_S2_IFORCE_WATCH_SHIFT)
#define  DCMOTOR_S2_IFORCE_WATCH_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_WATCH_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_WATCH_SHIFT))

#define  DCMOTOR_S2_IFORCE_ZPOS_MASK 0x4
#define  DCMOTOR_S2_IFORCE_ZPOS_SHIFT 2
#define  DCMOTOR_S2_IFORCE_ZPOS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_ZPOS_MASK) >> DCMOTOR_S2_IFORCE_ZPOS_SHIFT)
#define  DCMOTOR_S2_IFORCE_ZPOS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_ZPOS_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_ZPOS_SHIFT))

#define  DCMOTOR_S2_IFORCE_ENC_MASK 0x2
#define  DCMOTOR_S2_IFORCE_ENC_SHIFT 1
#define  DCMOTOR_S2_IFORCE_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_ENC_MASK) >> DCMOTOR_S2_IFORCE_ENC_SHIFT)
#define  DCMOTOR_S2_IFORCE_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_ENC_SHIFT))

#define  DCMOTOR_S2_IFORCE_TBASE_MASK 0x1
#define  DCMOTOR_S2_IFORCE_TBASE_SHIFT 0
#define  DCMOTOR_S2_IFORCE_TBASE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_IFORCE_TBASE_MASK) >> DCMOTOR_S2_IFORCE_TBASE_SHIFT)
#define  DCMOTOR_S2_IFORCE_TBASE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_IFORCE_TBASE_MASK) | (((uint32_t)val) << DCMOTOR_S2_IFORCE_TBASE_SHIFT))

//====================================================================
//Register: Timestamp Control (TS_CTRL)
/** \brief Register which controls the timestamp counter. Has a selectable timebase for incrementing the count and an enable bit start/stop counting.*/
//====================================================================

#define  DCMOTOR_S2_TS_CTRL_TS_ENABLE_MASK 0x80000000
#define  DCMOTOR_S2_TS_CTRL_TS_ENABLE_SHIFT 31
#define  DCMOTOR_S2_TS_CTRL_TS_ENABLE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CTRL_TS_ENABLE_MASK) >> DCMOTOR_S2_TS_CTRL_TS_ENABLE_SHIFT)
#define  DCMOTOR_S2_TS_CTRL_TS_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CTRL_TS_ENABLE_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CTRL_TS_ENABLE_SHIFT))

#define  DCMOTOR_S2_TS_CTRL_RESERVED1_MASK 0x7fffffc0
#define  DCMOTOR_S2_TS_CTRL_RESERVED1_SHIFT 6
#define  DCMOTOR_S2_TS_CTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CTRL_RESERVED1_MASK) >> DCMOTOR_S2_TS_CTRL_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_CTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CTRL_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CTRL_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_MASK 0x20
#define  DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_SHIFT 5
#define  DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_MASK) >> DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_SHIFT)
#define  DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CTRL_TS_ABSOLUTE_SHIFT))

#define  DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_MASK 0x10
#define  DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_SHIFT 4
#define  DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_MASK) >> DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_SHIFT)
#define  DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CTRL_TS_EXTERNAL_SHIFT))

#define  DCMOTOR_S2_TS_CTRL_RESERVED2_MASK 0x8
#define  DCMOTOR_S2_TS_CTRL_RESERVED2_SHIFT 3
#define  DCMOTOR_S2_TS_CTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CTRL_RESERVED2_MASK) >> DCMOTOR_S2_TS_CTRL_RESERVED2_SHIFT)
#define  DCMOTOR_S2_TS_CTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CTRL_RESERVED2_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CTRL_RESERVED2_SHIFT))

#define  DCMOTOR_S2_TS_CTRL_TS_TBSEL_MASK 0x7
#define  DCMOTOR_S2_TS_CTRL_TS_TBSEL_SHIFT 0
#define  DCMOTOR_S2_TS_CTRL_TS_TBSEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CTRL_TS_TBSEL_MASK) >> DCMOTOR_S2_TS_CTRL_TS_TBSEL_SHIFT)
#define  DCMOTOR_S2_TS_CTRL_TS_TBSEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CTRL_TS_TBSEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CTRL_TS_TBSEL_SHIFT))

//====================================================================
//Register: Timestamp Cnt Read (TS_CNT)
/** \brief Reading this register provides the current value of the timestamp counter and the current values of the debounced ENCA and ENCB.*/
//====================================================================

#define  DCMOTOR_S2_TS_CNT_RESERVED1_MASK 0x80000000
#define  DCMOTOR_S2_TS_CNT_RESERVED1_SHIFT 31
#define  DCMOTOR_S2_TS_CNT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CNT_RESERVED1_MASK) >> DCMOTOR_S2_TS_CNT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_CNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CNT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CNT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_CNT_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_TS_CNT_ENCA_SHIFT 30
#define  DCMOTOR_S2_TS_CNT_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CNT_ENCA_MASK) >> DCMOTOR_S2_TS_CNT_ENCA_SHIFT)
#define  DCMOTOR_S2_TS_CNT_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CNT_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CNT_ENCA_SHIFT))

#define  DCMOTOR_S2_TS_CNT_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_TS_CNT_ENCB_SHIFT 29
#define  DCMOTOR_S2_TS_CNT_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CNT_ENCB_MASK) >> DCMOTOR_S2_TS_CNT_ENCB_SHIFT)
#define  DCMOTOR_S2_TS_CNT_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CNT_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CNT_ENCB_SHIFT))

#define  DCMOTOR_S2_TS_CNT_RESERVED2_MASK 0x1fe00000
#define  DCMOTOR_S2_TS_CNT_RESERVED2_SHIFT 21
#define  DCMOTOR_S2_TS_CNT_RESERVED2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CNT_RESERVED2_MASK) >> DCMOTOR_S2_TS_CNT_RESERVED2_SHIFT)
#define  DCMOTOR_S2_TS_CNT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CNT_RESERVED2_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CNT_RESERVED2_SHIFT))

#define  DCMOTOR_S2_TS_CNT_TS_CNT_MASK 0x1fffff
#define  DCMOTOR_S2_TS_CNT_TS_CNT_SHIFT 0
#define  DCMOTOR_S2_TS_CNT_TS_CNT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_CNT_TS_CNT_MASK) >> DCMOTOR_S2_TS_CNT_TS_CNT_SHIFT)
#define  DCMOTOR_S2_TS_CNT_TS_CNT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_CNT_TS_CNT_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_CNT_TS_CNT_SHIFT))

//====================================================================
//Register: Enc Timestamp 0 (TS_ENC_0)
/** \brief Read-only register that provides a timestamp for most recent edge of Enc (newest).*/
//====================================================================

#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_MASK) >> DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_0_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_TS_ENC_0_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_TS_ENC_0_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_TS_ENC_0_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_0_TS_ENCA_MASK) >> DCMOTOR_S2_TS_ENC_0_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_TS_ENC_0_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_0_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_0_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_TS_ENC_0_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_TS_ENC_0_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_TS_ENC_0_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_0_TS_ENCB_MASK) >> DCMOTOR_S2_TS_ENC_0_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_TS_ENC_0_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_0_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_0_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_TS_ENC_0_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_TS_ENC_0_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_TS_ENC_0_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_0_TS_WHO_MASK) >> DCMOTOR_S2_TS_ENC_0_TS_WHO_SHIFT)
#define  DCMOTOR_S2_TS_ENC_0_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_0_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_0_TS_WHO_SHIFT))

#define  DCMOTOR_S2_TS_ENC_0_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_TS_ENC_0_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_TS_ENC_0_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_0_RESERVED1_MASK) >> DCMOTOR_S2_TS_ENC_0_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_ENC_0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_0_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_0_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_0_TS_ENC_MASK) >> DCMOTOR_S2_TS_ENC_0_TS_ENC_SHIFT)
#define  DCMOTOR_S2_TS_ENC_0_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_0_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_0_TS_ENC_SHIFT))

//====================================================================
//Register: Enc Timestamp 1 (TS_ENC_1)
/** \brief Read-only register that provides a timestamp for the second most recent edge of Enc.*/
//====================================================================

#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_MASK) >> DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_1_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_TS_ENC_1_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_TS_ENC_1_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_TS_ENC_1_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_1_TS_ENCA_MASK) >> DCMOTOR_S2_TS_ENC_1_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_TS_ENC_1_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_1_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_1_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_TS_ENC_1_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_TS_ENC_1_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_TS_ENC_1_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_1_TS_ENCB_MASK) >> DCMOTOR_S2_TS_ENC_1_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_TS_ENC_1_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_1_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_1_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_TS_ENC_1_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_TS_ENC_1_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_TS_ENC_1_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_1_TS_WHO_MASK) >> DCMOTOR_S2_TS_ENC_1_TS_WHO_SHIFT)
#define  DCMOTOR_S2_TS_ENC_1_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_1_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_1_TS_WHO_SHIFT))

#define  DCMOTOR_S2_TS_ENC_1_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_TS_ENC_1_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_TS_ENC_1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_1_RESERVED1_MASK) >> DCMOTOR_S2_TS_ENC_1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_ENC_1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_1_TS_ENC_MASK) >> DCMOTOR_S2_TS_ENC_1_TS_ENC_SHIFT)
#define  DCMOTOR_S2_TS_ENC_1_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_1_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_1_TS_ENC_SHIFT))

//====================================================================
//Register: Enc Timestamp 2 (TS_ENC_2)
/** \brief Read-only register that provides a timestamp for the third most recent edge of Enc.*/
//====================================================================

#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_MASK) >> DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_2_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_TS_ENC_2_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_TS_ENC_2_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_TS_ENC_2_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_2_TS_ENCA_MASK) >> DCMOTOR_S2_TS_ENC_2_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_TS_ENC_2_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_2_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_2_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_TS_ENC_2_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_TS_ENC_2_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_TS_ENC_2_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_2_TS_ENCB_MASK) >> DCMOTOR_S2_TS_ENC_2_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_TS_ENC_2_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_2_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_2_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_TS_ENC_2_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_TS_ENC_2_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_TS_ENC_2_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_2_TS_WHO_MASK) >> DCMOTOR_S2_TS_ENC_2_TS_WHO_SHIFT)
#define  DCMOTOR_S2_TS_ENC_2_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_2_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_2_TS_WHO_SHIFT))

#define  DCMOTOR_S2_TS_ENC_2_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_TS_ENC_2_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_TS_ENC_2_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_2_RESERVED1_MASK) >> DCMOTOR_S2_TS_ENC_2_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_ENC_2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_2_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_2_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_2_TS_ENC_MASK) >> DCMOTOR_S2_TS_ENC_2_TS_ENC_SHIFT)
#define  DCMOTOR_S2_TS_ENC_2_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_2_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_2_TS_ENC_SHIFT))

//====================================================================
//Register: Enc Timestamp 3 (TS_ENC_3)
/** \brief Read-only register that provides a timestamp for the fourth most recent edge of Enc.*/
//====================================================================

#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_MASK) >> DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_3_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_TS_ENC_3_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_TS_ENC_3_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_TS_ENC_3_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_3_TS_ENCA_MASK) >> DCMOTOR_S2_TS_ENC_3_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_TS_ENC_3_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_3_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_3_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_TS_ENC_3_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_TS_ENC_3_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_TS_ENC_3_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_3_TS_ENCB_MASK) >> DCMOTOR_S2_TS_ENC_3_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_TS_ENC_3_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_3_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_3_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_TS_ENC_3_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_TS_ENC_3_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_TS_ENC_3_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_3_TS_WHO_MASK) >> DCMOTOR_S2_TS_ENC_3_TS_WHO_SHIFT)
#define  DCMOTOR_S2_TS_ENC_3_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_3_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_3_TS_WHO_SHIFT))

#define  DCMOTOR_S2_TS_ENC_3_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_TS_ENC_3_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_TS_ENC_3_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_3_RESERVED1_MASK) >> DCMOTOR_S2_TS_ENC_3_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_ENC_3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_3_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_3_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_3_TS_ENC_MASK) >> DCMOTOR_S2_TS_ENC_3_TS_ENC_SHIFT)
#define  DCMOTOR_S2_TS_ENC_3_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_3_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_3_TS_ENC_SHIFT))

//====================================================================
//Register: Enc Timestamp 4 (TS_ENC_4)
/** \brief Read-only register that provides a timestamp for the fifth most recent edge of Enc (oldest).*/
//====================================================================

#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_MASK) >> DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_4_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_TS_ENC_4_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_TS_ENC_4_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_TS_ENC_4_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_4_TS_ENCA_MASK) >> DCMOTOR_S2_TS_ENC_4_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_TS_ENC_4_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_4_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_4_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_TS_ENC_4_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_TS_ENC_4_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_TS_ENC_4_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_4_TS_ENCB_MASK) >> DCMOTOR_S2_TS_ENC_4_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_TS_ENC_4_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_4_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_4_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_TS_ENC_4_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_TS_ENC_4_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_TS_ENC_4_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_4_TS_WHO_MASK) >> DCMOTOR_S2_TS_ENC_4_TS_WHO_SHIFT)
#define  DCMOTOR_S2_TS_ENC_4_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_4_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_4_TS_WHO_SHIFT))

#define  DCMOTOR_S2_TS_ENC_4_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_TS_ENC_4_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_TS_ENC_4_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_4_RESERVED1_MASK) >> DCMOTOR_S2_TS_ENC_4_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TS_ENC_4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_4_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_4_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TS_ENC_4_TS_ENC_MASK) >> DCMOTOR_S2_TS_ENC_4_TS_ENC_SHIFT)
#define  DCMOTOR_S2_TS_ENC_4_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TS_ENC_4_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_TS_ENC_4_TS_ENC_SHIFT))

//====================================================================
//Register: Latch Enable (LATCH_EN)
/** \brief This write-only register enables latching of the timestamp registers
   (TS_ENC_0 to TS_ENC_4). When a '1' is written, the current values of the
   timestamp registers are loaded into the LATCH_TS_ENC_* registers (LATCH_TS_ENC_0
   to LATCH_TS_ENC_4). The LATCH_TS_ENC_* registers provide stable values that are
   guaranteed not to change while FW is reading.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_EN_RESERVED1_MASK 0xfffffffe
#define  DCMOTOR_S2_LATCH_EN_RESERVED1_SHIFT 1
#define  DCMOTOR_S2_LATCH_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_EN_RESERVED1_MASK) >> DCMOTOR_S2_LATCH_EN_RESERVED1_SHIFT)
#define  DCMOTOR_S2_LATCH_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_EN_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_EN_RESERVED1_SHIFT))

#define  DCMOTOR_S2_LATCH_EN_ENABLE_MASK 0x1
#define  DCMOTOR_S2_LATCH_EN_ENABLE_SHIFT 0
#define  DCMOTOR_S2_LATCH_EN_ENABLE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_EN_ENABLE_MASK) >> DCMOTOR_S2_LATCH_EN_ENABLE_SHIFT)
#define  DCMOTOR_S2_LATCH_EN_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_EN_ENABLE_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_EN_ENABLE_SHIFT))

//====================================================================
//Register: Latched Encoder Position (LATCH_EPOS)
/** \brief This read-only register provides a latched encoder position and direction (from EPOS_RD). The value is latched by writing the LATCH_EN register.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_EPOS_DIR_MASK 0x80000000
#define  DCMOTOR_S2_LATCH_EPOS_DIR_SHIFT 31
#define  DCMOTOR_S2_LATCH_EPOS_DIR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_EPOS_DIR_MASK) >> DCMOTOR_S2_LATCH_EPOS_DIR_SHIFT)
#define  DCMOTOR_S2_LATCH_EPOS_DIR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_EPOS_DIR_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_EPOS_DIR_SHIFT))

#define  DCMOTOR_S2_LATCH_EPOS_POS_MASK 0x7fffffff
#define  DCMOTOR_S2_LATCH_EPOS_POS_SHIFT 0
#define  DCMOTOR_S2_LATCH_EPOS_POS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_EPOS_POS_MASK) >> DCMOTOR_S2_LATCH_EPOS_POS_SHIFT)
#define  DCMOTOR_S2_LATCH_EPOS_POS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_EPOS_POS_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_EPOS_POS_SHIFT))

//====================================================================
//Register: Latched Enc Timestamp Cnt (LATCH_TS_CNT)
/** \brief Read-only register that provides the value of the latched timestamp counter and the latched values of the debounced ENCA and ENCB. The value is latched by writing the LATCH_EN register.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_MASK 0x80000000
#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_SHIFT 31
#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_MASK) >> DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_CNT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_CNT_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_LATCH_TS_CNT_ENCA_SHIFT 30
#define  DCMOTOR_S2_LATCH_TS_CNT_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_CNT_ENCA_MASK) >> DCMOTOR_S2_LATCH_TS_CNT_ENCA_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_CNT_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_CNT_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_CNT_ENCA_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_CNT_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_LATCH_TS_CNT_ENCB_SHIFT 29
#define  DCMOTOR_S2_LATCH_TS_CNT_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_CNT_ENCB_MASK) >> DCMOTOR_S2_LATCH_TS_CNT_ENCB_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_CNT_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_CNT_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_CNT_ENCB_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_MASK 0x1fe00000
#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_SHIFT 21
#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_MASK) >> DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_CNT_RESERVED2_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_MASK 0x1fffff
#define  DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_SHIFT 0
#define  DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_MASK) >> DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_CNT_TS_CNT_SHIFT))

//====================================================================
//Register: Latched Enc Timestamp 0 (LATCH_TS_ENC_0)
/** \brief Read-only register that provides a latched timestamp for most recent edge of Enc (newest). The value is latched by writing the LATCH_EN register.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_0_TS_WHO_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_0_RESERVED1_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_0_TS_ENC_SHIFT))

//====================================================================
//Register: Latched Enc Timestamp 1 (LATCH_TS_ENC_1)
/** \brief Read-only register that provides a latched timestamp for the second most recent edge of Enc (newest). The value is latched by writing the LATCH_EN register.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_1_TS_WHO_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_1_RESERVED1_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_1_TS_ENC_SHIFT))

//====================================================================
//Register: Latched Enc Timestamp 2 (LATCH_TS_ENC_2)
/** \brief Read-only register that provides a latched timestamp for the third most recent edge of Enc (newest). The value is latched by writing the LATCH_EN register.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_2_TS_WHO_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_2_RESERVED1_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_2_TS_ENC_SHIFT))

//====================================================================
//Register: Latched Enc Timestamp 3 (LATCH_TS_ENC_3)
/** \brief Read-only register that provides a latched timestamp for the forth most recent edge of Enc (newest). The value is latched by writing the LATCH_EN register.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_3_TS_WHO_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_3_RESERVED1_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_3_TS_ENC_SHIFT))

//====================================================================
//Register: Latched Enc Timestamp 4 (LATCH_TS_ENC_4)
/** \brief Read-only register that provides a latched timestamp for the forth most recent edge of Enc (newest). The value is latched by writing the LATCH_EN register.*/
//====================================================================

#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_4_TS_WHO_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_4_RESERVED1_SHIFT))

#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_MASK) >> DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_SHIFT)
#define  DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_LATCH_TS_ENC_4_TS_ENC_SHIFT))

//====================================================================
//Register: Interrupt Encoder Timestamp (INT_ENC_TS)
/** \brief Read-only register that provides a timestamp for the most recent Encoder edge (based on PMode) occuring prior to the interrupt. Valid is cleared if timetamps are disabled using TS_CTRL.*/
//====================================================================

#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_MASK) >> DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_ENC_TS_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_ENC_TS_TS_ENCA_MASK) >> DCMOTOR_S2_INT_ENC_TS_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_ENC_TS_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_ENC_TS_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_ENC_TS_TS_ENCB_MASK) >> DCMOTOR_S2_INT_ENC_TS_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_ENC_TS_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_ENC_TS_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_INT_ENC_TS_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_INT_ENC_TS_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_INT_ENC_TS_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_ENC_TS_TS_WHO_MASK) >> DCMOTOR_S2_INT_ENC_TS_TS_WHO_SHIFT)
#define  DCMOTOR_S2_INT_ENC_TS_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_ENC_TS_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_ENC_TS_TS_WHO_SHIFT))

#define  DCMOTOR_S2_INT_ENC_TS_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_INT_ENC_TS_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_INT_ENC_TS_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_ENC_TS_RESERVED1_MASK) >> DCMOTOR_S2_INT_ENC_TS_RESERVED1_SHIFT)
#define  DCMOTOR_S2_INT_ENC_TS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_ENC_TS_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_ENC_TS_RESERVED1_SHIFT))

#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_ENC_TS_TS_ENC_MASK) >> DCMOTOR_S2_INT_ENC_TS_TS_ENC_SHIFT)
#define  DCMOTOR_S2_INT_ENC_TS_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_ENC_TS_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_ENC_TS_TS_ENC_SHIFT))

//====================================================================
//Register: Interrupt Timestamp (INT_TS)
/** \brief Read-only register that provides a timestamp for the occurrence of the interrupt. Also has a bit indicating which interrupt was captured. Valid is cleared if timetamps are disabled using TS_CTRL.*/
//====================================================================

#define  DCMOTOR_S2_INT_TS_TSVAL_MASK 0x80000000
#define  DCMOTOR_S2_INT_TS_TSVAL_SHIFT 31
#define  DCMOTOR_S2_INT_TS_TSVAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_TS_TSVAL_MASK) >> DCMOTOR_S2_INT_TS_TSVAL_SHIFT)
#define  DCMOTOR_S2_INT_TS_TSVAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_TS_TSVAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_TS_TSVAL_SHIFT))

#define  DCMOTOR_S2_INT_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_INT_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_INT_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_TS_ENCA_MASK) >> DCMOTOR_S2_INT_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_INT_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_INT_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_INT_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_INT_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_TS_ENCB_MASK) >> DCMOTOR_S2_INT_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_INT_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_INT_TS_RESERVED1_MASK 0x1fe00000
#define  DCMOTOR_S2_INT_TS_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_INT_TS_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_TS_RESERVED1_MASK) >> DCMOTOR_S2_INT_TS_RESERVED1_SHIFT)
#define  DCMOTOR_S2_INT_TS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_TS_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_TS_RESERVED1_SHIFT))

#define  DCMOTOR_S2_INT_TS_TS_MASK 0x1fffff
#define  DCMOTOR_S2_INT_TS_TS_SHIFT 0
#define  DCMOTOR_S2_INT_TS_TS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_TS_TS_MASK) >> DCMOTOR_S2_INT_TS_TS_SHIFT)
#define  DCMOTOR_S2_INT_TS_TS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_TS_TS_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_TS_TS_SHIFT))

//====================================================================
//Register: Interrupt Encoder Position (INT_EPOS)
/** \brief Read-only register that latches the current encoder position on the occurence of an interrupt. See TS_IEN register for configuration details*/
//====================================================================

#define  DCMOTOR_S2_INT_EPOS_DIR_MASK 0x80000000
#define  DCMOTOR_S2_INT_EPOS_DIR_SHIFT 31
#define  DCMOTOR_S2_INT_EPOS_DIR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_EPOS_DIR_MASK) >> DCMOTOR_S2_INT_EPOS_DIR_SHIFT)
#define  DCMOTOR_S2_INT_EPOS_DIR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_EPOS_DIR_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_EPOS_DIR_SHIFT))

#define  DCMOTOR_S2_INT_EPOS_POS_MASK 0x7fffffff
#define  DCMOTOR_S2_INT_EPOS_POS_SHIFT 0
#define  DCMOTOR_S2_INT_EPOS_POS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_INT_EPOS_POS_MASK) >> DCMOTOR_S2_INT_EPOS_POS_SHIFT)
#define  DCMOTOR_S2_INT_EPOS_POS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_INT_EPOS_POS_MASK) | (((uint32_t)val) << DCMOTOR_S2_INT_EPOS_POS_SHIFT))

//====================================================================
//Register: Encoder Watch Control (WATCH_CTRL)
/** \brief Register which controls the Encoder Watch Counter. The Encoder Watch logic allows a timer to be set that will fire an interrupt if a relevant encoder edge (as selected by PMode) does not occur before the timer finishes.*/
//====================================================================

#define  DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_MASK 0x80000000
#define  DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_SHIFT 31
#define  DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_MASK) >> DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_SHIFT)
#define  DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_MASK) | (((uint32_t)val) << DCMOTOR_S2_WATCH_CTRL_WATCHENABLE_SHIFT))

#define  DCMOTOR_S2_WATCH_CTRL_RESERVED1_MASK 0x7ffffff8
#define  DCMOTOR_S2_WATCH_CTRL_RESERVED1_SHIFT 3
#define  DCMOTOR_S2_WATCH_CTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_WATCH_CTRL_RESERVED1_MASK) >> DCMOTOR_S2_WATCH_CTRL_RESERVED1_SHIFT)
#define  DCMOTOR_S2_WATCH_CTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_WATCH_CTRL_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_WATCH_CTRL_RESERVED1_SHIFT))

#define  DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_MASK 0x7
#define  DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_SHIFT 0
#define  DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_MASK) >> DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_SHIFT)
#define  DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_WATCH_CTRL_WATCHTBSEL_SHIFT))

//====================================================================
//Register: Encoder Watch Value (WATCH_VALUE)
/** \brief Specifies the watch value for which the encoder watch interrupt will fire.*/
//====================================================================

#define  DCMOTOR_S2_WATCH_VALUE_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_WATCH_VALUE_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_WATCH_VALUE_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_WATCH_VALUE_RESERVED1_MASK) >> DCMOTOR_S2_WATCH_VALUE_RESERVED1_SHIFT)
#define  DCMOTOR_S2_WATCH_VALUE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_WATCH_VALUE_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_WATCH_VALUE_RESERVED1_SHIFT))

#define  DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_MASK 0xffff
#define  DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_SHIFT 0
#define  DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_MASK) >> DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_SHIFT)
#define  DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_MASK) | (((uint32_t)val) << DCMOTOR_S2_WATCH_VALUE_WATCHVALUE_SHIFT))

//====================================================================
//Register: Encoder Watch Counter Read (WATCH_CNT)
/** \brief Reading this register provides the current value of the encoder watch counter.*/
//====================================================================

#define  DCMOTOR_S2_WATCH_CNT_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_WATCH_CNT_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_WATCH_CNT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_WATCH_CNT_RESERVED1_MASK) >> DCMOTOR_S2_WATCH_CNT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_WATCH_CNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_WATCH_CNT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_WATCH_CNT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_WATCH_CNT_WATCHCNT_MASK 0xffff
#define  DCMOTOR_S2_WATCH_CNT_WATCHCNT_SHIFT 0
#define  DCMOTOR_S2_WATCH_CNT_WATCHCNT_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_WATCH_CNT_WATCHCNT_MASK) >> DCMOTOR_S2_WATCH_CNT_WATCHCNT_SHIFT)
#define  DCMOTOR_S2_WATCH_CNT_WATCHCNT_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_WATCH_CNT_WATCHCNT_MASK) | (((uint32_t)val) << DCMOTOR_S2_WATCH_CNT_WATCHCNT_SHIFT))

//====================================================================
//Register: Encoder Edge Interrupt Configuration (ENC_INT_CFG)
/** \brief Configures the encoder edge interrupt, allowing the edge type to be selected. The encoder source used depends on the PMode setting.*/
//====================================================================

#define  DCMOTOR_S2_ENC_INT_CFG_RESERVED1_MASK 0xfffffffc
#define  DCMOTOR_S2_ENC_INT_CFG_RESERVED1_SHIFT 2
#define  DCMOTOR_S2_ENC_INT_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ENC_INT_CFG_RESERVED1_MASK) >> DCMOTOR_S2_ENC_INT_CFG_RESERVED1_SHIFT)
#define  DCMOTOR_S2_ENC_INT_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ENC_INT_CFG_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_ENC_INT_CFG_RESERVED1_SHIFT))

#define  DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_MASK 0x3
#define  DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_SHIFT 0
#define  DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_MASK) >> DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_SHIFT)
#define  DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_ENC_INT_CFG_ENCEDGESEL_SHIFT))

//====================================================================
//Register: Encoder Position Trigger (TRIG_EPOS)
/** \brief Set the encoder position at which the encoder position trigger will fire. The trigger can be routed out to ADC or can be used to generate an interrupt.*/
//====================================================================

#define  DCMOTOR_S2_TRIG_EPOS_RESERVED1_MASK 0x80000000
#define  DCMOTOR_S2_TRIG_EPOS_RESERVED1_SHIFT 31
#define  DCMOTOR_S2_TRIG_EPOS_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TRIG_EPOS_RESERVED1_MASK) >> DCMOTOR_S2_TRIG_EPOS_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TRIG_EPOS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TRIG_EPOS_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TRIG_EPOS_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_MASK 0x7fffffff
#define  DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_SHIFT 0
#define  DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_MASK) >> DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_SHIFT)
#define  DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_MASK) | (((uint32_t)val) << DCMOTOR_S2_TRIG_EPOS_ENCPOSTRIG_SHIFT))

//====================================================================
//Register: Trigger Control Register (TRIG_CTRL)
/** \brief Configures the external trigger (which connects to ADC and SCCP).*/
//====================================================================

#define  DCMOTOR_S2_TRIG_CTRL_TRIGEN_MASK 0x80000000
#define  DCMOTOR_S2_TRIG_CTRL_TRIGEN_SHIFT 31
#define  DCMOTOR_S2_TRIG_CTRL_TRIGEN_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TRIG_CTRL_TRIGEN_MASK) >> DCMOTOR_S2_TRIG_CTRL_TRIGEN_SHIFT)
#define  DCMOTOR_S2_TRIG_CTRL_TRIGEN_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TRIG_CTRL_TRIGEN_MASK) | (((uint32_t)val) << DCMOTOR_S2_TRIG_CTRL_TRIGEN_SHIFT))

#define  DCMOTOR_S2_TRIG_CTRL_RESERVED1_MASK 0x7ffffff8
#define  DCMOTOR_S2_TRIG_CTRL_RESERVED1_SHIFT 3
#define  DCMOTOR_S2_TRIG_CTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TRIG_CTRL_RESERVED1_MASK) >> DCMOTOR_S2_TRIG_CTRL_RESERVED1_SHIFT)
#define  DCMOTOR_S2_TRIG_CTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TRIG_CTRL_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_TRIG_CTRL_RESERVED1_SHIFT))

#define  DCMOTOR_S2_TRIG_CTRL_TRIGSEL_MASK 0x7
#define  DCMOTOR_S2_TRIG_CTRL_TRIGSEL_SHIFT 0
#define  DCMOTOR_S2_TRIG_CTRL_TRIGSEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_TRIG_CTRL_TRIGSEL_MASK) >> DCMOTOR_S2_TRIG_CTRL_TRIGSEL_SHIFT)
#define  DCMOTOR_S2_TRIG_CTRL_TRIGSEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_TRIG_CTRL_TRIGSEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_TRIG_CTRL_TRIGSEL_SHIFT))

//====================================================================
//Register: S2 Cfg Register (S2_CFG)
/** \brief This register is used to configure the S2 capture logic. The register
   contains fields for selecting which signal edge is used to initiate a capture and an
      enable bit for the function. ONLY AVAILABLE IF INC_S2 PARAMETER SET.*/
//====================================================================

#define  DCMOTOR_S2_S2_CFG_EN_MASK 0x80000000
#define  DCMOTOR_S2_S2_CFG_EN_SHIFT 31
#define  DCMOTOR_S2_S2_CFG_EN_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_CFG_EN_MASK) >> DCMOTOR_S2_S2_CFG_EN_SHIFT)
#define  DCMOTOR_S2_S2_CFG_EN_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_CFG_EN_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_CFG_EN_SHIFT))

#define  DCMOTOR_S2_S2_CFG_RESERVED1_MASK 0x7ffffffc
#define  DCMOTOR_S2_S2_CFG_RESERVED1_SHIFT 2
#define  DCMOTOR_S2_S2_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_CFG_RESERVED1_MASK) >> DCMOTOR_S2_S2_CFG_RESERVED1_SHIFT)
#define  DCMOTOR_S2_S2_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_CFG_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_CFG_RESERVED1_SHIFT))

#define  DCMOTOR_S2_S2_CFG_EDGE_SEL_MASK 0x3
#define  DCMOTOR_S2_S2_CFG_EDGE_SEL_SHIFT 0
#define  DCMOTOR_S2_S2_CFG_EDGE_SEL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_CFG_EDGE_SEL_MASK) >> DCMOTOR_S2_S2_CFG_EDGE_SEL_SHIFT)
#define  DCMOTOR_S2_S2_CFG_EDGE_SEL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_CFG_EDGE_SEL_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_CFG_EDGE_SEL_SHIFT))

//====================================================================
//Register: S2 Debounce (S2_DB)
/** \brief This register is used to debounce the S2 signal. The
   input signal must be stable for the debounce value to be recognized as a
   valid input and passed to the S2 capture logic. The debounce counter is
   clocked by the SysClk. ONLY AVAILABLE IF INC_S2 PARAMETER SET.
<P />
   Debounce range is ~300us = (2^16) / SysClk*/
//====================================================================

#define  DCMOTOR_S2_S2_DB_RESERVED1_MASK 0xffff0000
#define  DCMOTOR_S2_S2_DB_RESERVED1_SHIFT 16
#define  DCMOTOR_S2_S2_DB_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_DB_RESERVED1_MASK) >> DCMOTOR_S2_S2_DB_RESERVED1_SHIFT)
#define  DCMOTOR_S2_S2_DB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_DB_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_DB_RESERVED1_SHIFT))

#define  DCMOTOR_S2_S2_DB_DB_MASK 0xffff
#define  DCMOTOR_S2_S2_DB_DB_SHIFT 0
#define  DCMOTOR_S2_S2_DB_DB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_DB_DB_MASK) >> DCMOTOR_S2_S2_DB_DB_SHIFT)
#define  DCMOTOR_S2_S2_DB_DB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_DB_DB_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_DB_DB_SHIFT))

//====================================================================
//Register: S2 Timestamp Read (S2_TSTAMP)
/** \brief Read-only register that provides the timestamp of the most recent edge of the
   S2 signal (as selected by the Edge Select field of S2_CFG register).
      ONLY AVAILABLE IF INC_S2 PARAMETER SET.*/
//====================================================================

#define  DCMOTOR_S2_S2_TSTAMP_VALID_MASK 0x80000000
#define  DCMOTOR_S2_S2_TSTAMP_VALID_SHIFT 31
#define  DCMOTOR_S2_S2_TSTAMP_VALID_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_TSTAMP_VALID_MASK) >> DCMOTOR_S2_S2_TSTAMP_VALID_SHIFT)
#define  DCMOTOR_S2_S2_TSTAMP_VALID_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_TSTAMP_VALID_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_TSTAMP_VALID_SHIFT))

#define  DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_MASK 0x40000000
#define  DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_SHIFT 30
#define  DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_MASK) >> DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_SHIFT)
#define  DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_TSTAMP_EDGE_TYPE_SHIFT))

#define  DCMOTOR_S2_S2_TSTAMP_RESERVED1_MASK 0x3fe00000
#define  DCMOTOR_S2_S2_TSTAMP_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_S2_TSTAMP_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_TSTAMP_RESERVED1_MASK) >> DCMOTOR_S2_S2_TSTAMP_RESERVED1_SHIFT)
#define  DCMOTOR_S2_S2_TSTAMP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_TSTAMP_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_TSTAMP_RESERVED1_SHIFT))

#define  DCMOTOR_S2_S2_TSTAMP_TSTAMP_MASK 0x1fffff
#define  DCMOTOR_S2_S2_TSTAMP_TSTAMP_SHIFT 0
#define  DCMOTOR_S2_S2_TSTAMP_TSTAMP_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_TSTAMP_TSTAMP_MASK) >> DCMOTOR_S2_S2_TSTAMP_TSTAMP_SHIFT)
#define  DCMOTOR_S2_S2_TSTAMP_TSTAMP_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_TSTAMP_TSTAMP_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_TSTAMP_TSTAMP_SHIFT))

//====================================================================
//Register: S2 Internal Encoder Position (S2_ENCPOS_INT)
/** \brief Read-only register that provides the internal encoders position value at the occurrence of
   the relevant S2 edge (as selected by the Edge Select field of S2_CFG register).
      ONLY AVAILABLE IF INC_S2 PARAMETER SET.*/
//====================================================================

#define  DCMOTOR_S2_S2_ENCPOS_INT_DIR_MASK 0x80000000
#define  DCMOTOR_S2_S2_ENCPOS_INT_DIR_SHIFT 31
#define  DCMOTOR_S2_S2_ENCPOS_INT_DIR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENCPOS_INT_DIR_MASK) >> DCMOTOR_S2_S2_ENCPOS_INT_DIR_SHIFT)
#define  DCMOTOR_S2_S2_ENCPOS_INT_DIR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENCPOS_INT_DIR_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENCPOS_INT_DIR_SHIFT))

#define  DCMOTOR_S2_S2_ENCPOS_INT_POS_MASK 0x7fffffff
#define  DCMOTOR_S2_S2_ENCPOS_INT_POS_SHIFT 0
#define  DCMOTOR_S2_S2_ENCPOS_INT_POS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENCPOS_INT_POS_MASK) >> DCMOTOR_S2_S2_ENCPOS_INT_POS_SHIFT)
#define  DCMOTOR_S2_S2_ENCPOS_INT_POS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENCPOS_INT_POS_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENCPOS_INT_POS_SHIFT))

//====================================================================
//Register: S2 Internal Enc Timestamp (S2_ENC_TSTAMP_INT)
/** \brief Read-only register that provides the timestamp for most recent edge of the internal encoder (newest).
   The timestamp value is latched on the relevant edge of the S2 signal (as selected by the Edge
   Select field of S2_CFG register). ONLY AVAILABLE IF INC_S2 PARAMETER SET.*/
//====================================================================

#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_WHO_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_INT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_INT_TS_ENC_SHIFT))

//====================================================================
//Register: S2 External Encoder Position (S2_ENCPOS_EXT)
/** \brief Read-only register that provides the external encoders position value at the occurrence of
   the relevant S2 edge (as selected by the Edge Select field of S2_CFG register).
      ONLY AVAILABLE IF INC_S2 PARAMETER SET.*/
//====================================================================

#define  DCMOTOR_S2_S2_ENCPOS_EXT_DIR_MASK 0x80000000
#define  DCMOTOR_S2_S2_ENCPOS_EXT_DIR_SHIFT 31
#define  DCMOTOR_S2_S2_ENCPOS_EXT_DIR_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENCPOS_EXT_DIR_MASK) >> DCMOTOR_S2_S2_ENCPOS_EXT_DIR_SHIFT)
#define  DCMOTOR_S2_S2_ENCPOS_EXT_DIR_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENCPOS_EXT_DIR_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENCPOS_EXT_DIR_SHIFT))

#define  DCMOTOR_S2_S2_ENCPOS_EXT_POS_MASK 0x7fffffff
#define  DCMOTOR_S2_S2_ENCPOS_EXT_POS_SHIFT 0
#define  DCMOTOR_S2_S2_ENCPOS_EXT_POS_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENCPOS_EXT_POS_MASK) >> DCMOTOR_S2_S2_ENCPOS_EXT_POS_SHIFT)
#define  DCMOTOR_S2_S2_ENCPOS_EXT_POS_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENCPOS_EXT_POS_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENCPOS_EXT_POS_SHIFT))

//====================================================================
//Register: S2 External Enc Timestamp (S2_ENC_TSTAMP_EXT)
/** \brief Read-only register that provides the timestamp for most recent edge of the external encoder (newest).
   The timestamp value is latched on the relevant edge of the S2 signal (as selected by the Edge
   Select field of S2_CFG register). ONLY AVAILABLE IF INC_S2 PARAMETER SET.*/
//====================================================================

#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_MASK 0x80000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_SHIFT 31
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_VAL_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_MASK 0x40000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_SHIFT 30
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCA_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_MASK 0x20000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_SHIFT 29
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENCB_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_MASK 0x10000000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_SHIFT 28
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_WHO_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_MASK 0xfe00000
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_SHIFT 21
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_EXT_RESERVED1_SHIFT))

#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_MASK 0x1fffff
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_SHIFT 0
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_MASK_SHIFT(reg) (((reg) & DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_MASK) >> DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_SHIFT)
#define  DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_REPLACE_VAL(reg,val) (((reg) & ~DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_MASK) | (((uint32_t)val) << DCMOTOR_S2_S2_ENC_TSTAMP_EXT_TS_ENC_SHIFT))

#endif // DCMOTOR_S2
