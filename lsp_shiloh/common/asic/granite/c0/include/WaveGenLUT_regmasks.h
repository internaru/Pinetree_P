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
 * \file WaveGenLUT_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _WAVEGEN_LUT_REGMASKS_H_
#define _WAVEGEN_LUT_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Waveform Generator block registers (WaveGen_LUT)
/** \brief The following registers are used to configure the Waveform Generator (WaveGen) block. The WaveGen output will be low-pass filtered by an external RC circuit to produce a DC voltage. <br/>
    Please note that the intended use of the WaveGen block in Delta-Sigma mode is to generate precise DC voltages that would be unobtainable with a PWM. Delta-Sigma mode (default) should not be used to generate fixed-frequency squarewaves. This is because, due to the nature of Delta-Sigma Modulators, different duty cycles produce different output frequencies.*/
//
//====================================================================

//====================================================================
//Register: WCR : WaveGen Configuration Register (WCR)
/** \brief This register is used to configure the operational WaveGen modes and behaviours.*/
//====================================================================

#define  WAVEGEN_LUT_WCR_RESERVED1_MASK 0xffffffc0
#define  WAVEGEN_LUT_WCR_RESERVED1_SHIFT 6
#define  WAVEGEN_LUT_WCR_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WCR_RESERVED1_MASK) >> WAVEGEN_LUT_WCR_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WCR_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WCR_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WCR_HSYNCSEL_MASK 0x30
#define  WAVEGEN_LUT_WCR_HSYNCSEL_SHIFT 4
#define  WAVEGEN_LUT_WCR_HSYNCSEL_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WCR_HSYNCSEL_MASK) >> WAVEGEN_LUT_WCR_HSYNCSEL_SHIFT)
#define  WAVEGEN_LUT_WCR_HSYNCSEL_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WCR_HSYNCSEL_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WCR_HSYNCSEL_SHIFT))

#define  WAVEGEN_LUT_WCR_AUTOREV_MASK 0x8
#define  WAVEGEN_LUT_WCR_AUTOREV_SHIFT 3
#define  WAVEGEN_LUT_WCR_AUTOREV_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WCR_AUTOREV_MASK) >> WAVEGEN_LUT_WCR_AUTOREV_SHIFT)
#define  WAVEGEN_LUT_WCR_AUTOREV_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WCR_AUTOREV_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WCR_AUTOREV_SHIFT))

#define  WAVEGEN_LUT_WCR_SINGLEVALUE_MASK 0x4
#define  WAVEGEN_LUT_WCR_SINGLEVALUE_SHIFT 2
#define  WAVEGEN_LUT_WCR_SINGLEVALUE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WCR_SINGLEVALUE_MASK) >> WAVEGEN_LUT_WCR_SINGLEVALUE_SHIFT)
#define  WAVEGEN_LUT_WCR_SINGLEVALUE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WCR_SINGLEVALUE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WCR_SINGLEVALUE_SHIFT))

#define  WAVEGEN_LUT_WCR_WAVEMODE_MASK 0x2
#define  WAVEGEN_LUT_WCR_WAVEMODE_SHIFT 1
#define  WAVEGEN_LUT_WCR_WAVEMODE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WCR_WAVEMODE_MASK) >> WAVEGEN_LUT_WCR_WAVEMODE_SHIFT)
#define  WAVEGEN_LUT_WCR_WAVEMODE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WCR_WAVEMODE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WCR_WAVEMODE_SHIFT))

#define  WAVEGEN_LUT_WCR_ENABLE_MASK 0x1
#define  WAVEGEN_LUT_WCR_ENABLE_SHIFT 0
#define  WAVEGEN_LUT_WCR_ENABLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WCR_ENABLE_MASK) >> WAVEGEN_LUT_WCR_ENABLE_SHIFT)
#define  WAVEGEN_LUT_WCR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WCR_ENABLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WCR_ENABLE_SHIFT))

//====================================================================
//Register: WTR : WaveGen Timestep Register (WTR)
/** \brief The value stored in this register specifies the timebase for WaveGen in system clocks.<br/>
The minimum possible pulse width in system clock cycles on the WaveGen output is given by timeStep. <br/>
All output pulses from Wavegen are integer multiples of timeStep system clock cycles.<br/>
Setting this field to 0 or 1 will both result in minimum output pulses of one system clock.*/
//====================================================================

#define  WAVEGEN_LUT_WTR_RESERVED1_MASK 0xffff0000
#define  WAVEGEN_LUT_WTR_RESERVED1_SHIFT 16
#define  WAVEGEN_LUT_WTR_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WTR_RESERVED1_MASK) >> WAVEGEN_LUT_WTR_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WTR_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WTR_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WTR_TIMESTEP_MASK 0xffff
#define  WAVEGEN_LUT_WTR_TIMESTEP_SHIFT 0
#define  WAVEGEN_LUT_WTR_TIMESTEP_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WTR_TIMESTEP_MASK) >> WAVEGEN_LUT_WTR_TIMESTEP_SHIFT)
#define  WAVEGEN_LUT_WTR_TIMESTEP_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WTR_TIMESTEP_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WTR_TIMESTEP_SHIFT))

//====================================================================
//Register: WSLR : WaveGen Segment Length Register (WSLR)
/** \brief The number of timeStep cycles (specified by the WTR register) that WaveGen will use the value in the current LUT entry before moving on to the next.*/
//====================================================================

#define  WAVEGEN_LUT_WSLR_PWMPERIOD_MASK 0xffff0000
#define  WAVEGEN_LUT_WSLR_PWMPERIOD_SHIFT 16
#define  WAVEGEN_LUT_WSLR_PWMPERIOD_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WSLR_PWMPERIOD_MASK) >> WAVEGEN_LUT_WSLR_PWMPERIOD_SHIFT)
#define  WAVEGEN_LUT_WSLR_PWMPERIOD_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WSLR_PWMPERIOD_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WSLR_PWMPERIOD_SHIFT))

#define  WAVEGEN_LUT_WSLR_SEGLENGTH_MASK 0xffff
#define  WAVEGEN_LUT_WSLR_SEGLENGTH_SHIFT 0
#define  WAVEGEN_LUT_WSLR_SEGLENGTH_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WSLR_SEGLENGTH_MASK) >> WAVEGEN_LUT_WSLR_SEGLENGTH_SHIFT)
#define  WAVEGEN_LUT_WSLR_SEGLENGTH_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WSLR_SEGLENGTH_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WSLR_SEGLENGTH_SHIFT))

//====================================================================
//Register: WOSR : WaveGen Output Status Register (WOSR)
//====================================================================

#define  WAVEGEN_LUT_WOSR_RESERVED1_MASK 0xfffffe00
#define  WAVEGEN_LUT_WOSR_RESERVED1_SHIFT 9
#define  WAVEGEN_LUT_WOSR_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WOSR_RESERVED1_MASK) >> WAVEGEN_LUT_WOSR_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WOSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WOSR_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WOSR_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WOSR_LUTADDR_MASK 0x1f0
#define  WAVEGEN_LUT_WOSR_LUTADDR_SHIFT 4
#define  WAVEGEN_LUT_WOSR_LUTADDR_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WOSR_LUTADDR_MASK) >> WAVEGEN_LUT_WOSR_LUTADDR_SHIFT)
#define  WAVEGEN_LUT_WOSR_LUTADDR_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WOSR_LUTADDR_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WOSR_LUTADDR_SHIFT))

#define  WAVEGEN_LUT_WOSR_RESERVED2_MASK 0xe
#define  WAVEGEN_LUT_WOSR_RESERVED2_SHIFT 1
#define  WAVEGEN_LUT_WOSR_RESERVED2_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WOSR_RESERVED2_MASK) >> WAVEGEN_LUT_WOSR_RESERVED2_SHIFT)
#define  WAVEGEN_LUT_WOSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WOSR_RESERVED2_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WOSR_RESERVED2_SHIFT))

#define  WAVEGEN_LUT_WOSR_WAVEOUT_MASK 0x1
#define  WAVEGEN_LUT_WOSR_WAVEOUT_SHIFT 0
#define  WAVEGEN_LUT_WOSR_WAVEOUT_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WOSR_WAVEOUT_MASK) >> WAVEGEN_LUT_WOSR_WAVEOUT_SHIFT)
#define  WAVEGEN_LUT_WOSR_WAVEOUT_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WOSR_WAVEOUT_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WOSR_WAVEOUT_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 1 of 32) (WGLUT0)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 1 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT0_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT0_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT0_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT0_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT0_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT0_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT0_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT0_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT0_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT0_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT0_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT0_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT0_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT0_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT0_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 2 of 32) (WGLUT1)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 2 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT1_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT1_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT1_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT1_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT1_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT1_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT1_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT1_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT1_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT1_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT1_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT1_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT1_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT1_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT1_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 3 of 32) (WGLUT2)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 3 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT2_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT2_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT2_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT2_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT2_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT2_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT2_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT2_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT2_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT2_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT2_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT2_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT2_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT2_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT2_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 4 of 32) (WGLUT3)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 4 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT3_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT3_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT3_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT3_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT3_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT3_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT3_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT3_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT3_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT3_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT3_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT3_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT3_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT3_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT3_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 5 of 32) (WGLUT4)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 5 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT4_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT4_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT4_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT4_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT4_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT4_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT4_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT4_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT4_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT4_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT4_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT4_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT4_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT4_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT4_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 6 of 32) (WGLUT5)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 6 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT5_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT5_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT5_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT5_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT5_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT5_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT5_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT5_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT5_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT5_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT5_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT5_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT5_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT5_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT5_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 7 of 32) (WGLUT6)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 7 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT6_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT6_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT6_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT6_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT6_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT6_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT6_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT6_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT6_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT6_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT6_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT6_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT6_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT6_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT6_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 8 of 32) (WGLUT7)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 8 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT7_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT7_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT7_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT7_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT7_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT7_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT7_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT7_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT7_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT7_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT7_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT7_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT7_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT7_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT7_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 9 of 32) (WGLUT8)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 9 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT8_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT8_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT8_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT8_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT8_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT8_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT8_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT8_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT8_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT8_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT8_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT8_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT8_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT8_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT8_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 10 of 32) (WGLUT9)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 10 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT9_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT9_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT9_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT9_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT9_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT9_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT9_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT9_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT9_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT9_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT9_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT9_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT9_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT9_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT9_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 11 of 32) (WGLUT10)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 11 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT10_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT10_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT10_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT10_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT10_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT10_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT10_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT10_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT10_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT10_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT10_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT10_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT10_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT10_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT10_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 12 of 32) (WGLUT11)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 12 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT11_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT11_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT11_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT11_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT11_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT11_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT11_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT11_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT11_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT11_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT11_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT11_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT11_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT11_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT11_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 13 of 32) (WGLUT12)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 13 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT12_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT12_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT12_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT12_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT12_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT12_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT12_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT12_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT12_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT12_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT12_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT12_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT12_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT12_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT12_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 14 of 32) (WGLUT13)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 14 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT13_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT13_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT13_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT13_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT13_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT13_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT13_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT13_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT13_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT13_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT13_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT13_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT13_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT13_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT13_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 15 of 32) (WGLUT14)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 15 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT14_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT14_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT14_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT14_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT14_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT14_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT14_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT14_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT14_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT14_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT14_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT14_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT14_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT14_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT14_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 16 of 32) (WGLUT15)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 16 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT15_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT15_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT15_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT15_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT15_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT15_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT15_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT15_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT15_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT15_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT15_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT15_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT15_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT15_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT15_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 17 of 32) (WGLUT16)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 17 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT16_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT16_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT16_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT16_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT16_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT16_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT16_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT16_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT16_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT16_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT16_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT16_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT16_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT16_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT16_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 18 of 32) (WGLUT17)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 18 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT17_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT17_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT17_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT17_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT17_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT17_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT17_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT17_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT17_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT17_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT17_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT17_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT17_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT17_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT17_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 19 of 32) (WGLUT18)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 19 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT18_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT18_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT18_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT18_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT18_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT18_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT18_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT18_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT18_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT18_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT18_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT18_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT18_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT18_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT18_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 20 of 32) (WGLUT19)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 20 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT19_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT19_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT19_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT19_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT19_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT19_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT19_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT19_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT19_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT19_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT19_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT19_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT19_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT19_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT19_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 21 of 32) (WGLUT20)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 21 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT20_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT20_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT20_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT20_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT20_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT20_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT20_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT20_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT20_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT20_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT20_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT20_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT20_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT20_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT20_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 22 of 32) (WGLUT21)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 22 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT21_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT21_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT21_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT21_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT21_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT21_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT21_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT21_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT21_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT21_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT21_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT21_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT21_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT21_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT21_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 23 of 32) (WGLUT22)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 23 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT22_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT22_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT22_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT22_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT22_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT22_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT22_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT22_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT22_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT22_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT22_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT22_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT22_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT22_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT22_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 24 of 32) (WGLUT23)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 24 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT23_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT23_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT23_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT23_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT23_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT23_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT23_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT23_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT23_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT23_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT23_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT23_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT23_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT23_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT23_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 25 of 32) (WGLUT24)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 25 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT24_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT24_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT24_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT24_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT24_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT24_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT24_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT24_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT24_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT24_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT24_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT24_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT24_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT24_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT24_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 26 of 32) (WGLUT25)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 26 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT25_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT25_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT25_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT25_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT25_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT25_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT25_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT25_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT25_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT25_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT25_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT25_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT25_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT25_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT25_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 27 of 32) (WGLUT26)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 27 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT26_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT26_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT26_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT26_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT26_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT26_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT26_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT26_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT26_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT26_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT26_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT26_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT26_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT26_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT26_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 28 of 32) (WGLUT27)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 28 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT27_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT27_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT27_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT27_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT27_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT27_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT27_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT27_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT27_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT27_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT27_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT27_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT27_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT27_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT27_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 29 of 32) (WGLUT28)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 29 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT28_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT28_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT28_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT28_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT28_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT28_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT28_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT28_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT28_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT28_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT28_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT28_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT28_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT28_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT28_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 30 of 32) (WGLUT29)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 30 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT29_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT29_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT29_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT29_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT29_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT29_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT29_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT29_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT29_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT29_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT29_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT29_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT29_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT29_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT29_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 31 of 32) (WGLUT30)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 31 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT30_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT30_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT30_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT30_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT30_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT30_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT30_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT30_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT30_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT30_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT30_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT30_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT30_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT30_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT30_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT30_DUTYCYCLE_SHIFT))

//====================================================================
//Register: WGLUTx : WaveGen DAC Input LUT (Instance 32 of 32) (WGLUT31)
/** \brief The meaning of the value in this register is dependent on the value of WaveMode.<br/>
WaveMode = 0 (Delt-Sigma Mode) <br/>
This LUT specifies the duty cycle of the WaveGen output. <br/>
This duty cycle multiplies the reference voltage to produce a desired DC output voltage (after external low-pass filtering).<br/>
The delta sigma algorithm produces a pulse stream with a long term average equal to the desired value. The pulse stream does not have a fixed frequency, but generates a stream of pulses that are integer multiples of timeStep*system clock in width. To achieve very fine control, the external filter will need to account for frequencies that will be significantly lower than (timeStep*system clock frequency).<br/>
<br/>
WaveMode = 1 (PWM Mode) <br/>
This LUT value specifies the number of timeStep cycles that the WaveGen output will be held high before transitioning low for the remaining timeStep cycles set in PWMPeriod.<br/> (Instance 32 of 32)*/
//====================================================================

#define  WAVEGEN_LUT_WGLUT31_RESERVED1_MASK 0xfffffc00
#define  WAVEGEN_LUT_WGLUT31_RESERVED1_SHIFT 10
#define  WAVEGEN_LUT_WGLUT31_RESERVED1_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT31_RESERVED1_MASK) >> WAVEGEN_LUT_WGLUT31_RESERVED1_SHIFT)
#define  WAVEGEN_LUT_WGLUT31_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT31_RESERVED1_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT31_RESERVED1_SHIFT))

#define  WAVEGEN_LUT_WGLUT31_DUTYCYCLE_MASK 0x3ff
#define  WAVEGEN_LUT_WGLUT31_DUTYCYCLE_SHIFT 0
#define  WAVEGEN_LUT_WGLUT31_DUTYCYCLE_MASK_SHIFT(reg) (((reg) & WAVEGEN_LUT_WGLUT31_DUTYCYCLE_MASK) >> WAVEGEN_LUT_WGLUT31_DUTYCYCLE_SHIFT)
#define  WAVEGEN_LUT_WGLUT31_DUTYCYCLE_REPLACE_VAL(reg,val) (((reg) & ~WAVEGEN_LUT_WGLUT31_DUTYCYCLE_MASK) | (((uint32_t)val) << WAVEGEN_LUT_WGLUT31_DUTYCYCLE_SHIFT))

#endif // WAVEGEN_LUT
