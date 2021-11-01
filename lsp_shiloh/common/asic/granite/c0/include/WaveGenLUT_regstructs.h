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
 * \file WaveGenLUT_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _WAVEGEN_LUT_REGSTRUCTS_H_
#define _WAVEGEN_LUT_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Waveform Generator block registers (WaveGen_LUT)
/** \brief The following registers are used to configure the Waveform Generator (WaveGen) block. The WaveGen output will be low-pass filtered by an external RC circuit to produce a DC voltage. <br/>
    Please note that the intended use of the WaveGen block in Delta-Sigma mode is to generate precise DC voltages that would be unobtainable with a PWM. Delta-Sigma mode (default) should not be used to generate fixed-frequency squarewaves. This is because, due to the nature of Delta-Sigma Modulators, different duty cycles produce different output frequencies.*/
//
//====================================================================

typedef struct WAVEGEN_LUT_REGS_s
{
  volatile uint32_t WCR;  ///< 0x0 [R/W]: WCR : WaveGen Configuration Register
  volatile uint32_t WTR;  ///< 0x4 [R/W]: WTR : WaveGen Timestep Register
  volatile uint32_t WSLR;  ///< 0x8 [R/W]: WSLR : WaveGen Segment Length Register
  volatile uint32_t WOSR;  ///< 0xc [R]: WOSR : WaveGen Output Status Register
  volatile uint32_t WGLUT0;  ///< 0x10 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 1 of 32)
  volatile uint32_t WGLUT1;  ///< 0x14 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 2 of 32)
  volatile uint32_t WGLUT2;  ///< 0x18 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 3 of 32)
  volatile uint32_t WGLUT3;  ///< 0x1c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 4 of 32)
  volatile uint32_t WGLUT4;  ///< 0x20 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 5 of 32)
  volatile uint32_t WGLUT5;  ///< 0x24 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 6 of 32)
  volatile uint32_t WGLUT6;  ///< 0x28 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 7 of 32)
  volatile uint32_t WGLUT7;  ///< 0x2c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 8 of 32)
  volatile uint32_t WGLUT8;  ///< 0x30 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 9 of 32)
  volatile uint32_t WGLUT9;  ///< 0x34 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 10 of 32)
  volatile uint32_t WGLUT10;  ///< 0x38 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 11 of 32)
  volatile uint32_t WGLUT11;  ///< 0x3c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 12 of 32)
  volatile uint32_t WGLUT12;  ///< 0x40 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 13 of 32)
  volatile uint32_t WGLUT13;  ///< 0x44 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 14 of 32)
  volatile uint32_t WGLUT14;  ///< 0x48 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 15 of 32)
  volatile uint32_t WGLUT15;  ///< 0x4c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 16 of 32)
  volatile uint32_t WGLUT16;  ///< 0x50 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 17 of 32)
  volatile uint32_t WGLUT17;  ///< 0x54 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 18 of 32)
  volatile uint32_t WGLUT18;  ///< 0x58 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 19 of 32)
  volatile uint32_t WGLUT19;  ///< 0x5c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 20 of 32)
  volatile uint32_t WGLUT20;  ///< 0x60 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 21 of 32)
  volatile uint32_t WGLUT21;  ///< 0x64 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 22 of 32)
  volatile uint32_t WGLUT22;  ///< 0x68 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 23 of 32)
  volatile uint32_t WGLUT23;  ///< 0x6c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 24 of 32)
  volatile uint32_t WGLUT24;  ///< 0x70 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 25 of 32)
  volatile uint32_t WGLUT25;  ///< 0x74 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 26 of 32)
  volatile uint32_t WGLUT26;  ///< 0x78 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 27 of 32)
  volatile uint32_t WGLUT27;  ///< 0x7c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 28 of 32)
  volatile uint32_t WGLUT28;  ///< 0x80 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 29 of 32)
  volatile uint32_t WGLUT29;  ///< 0x84 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 30 of 32)
  volatile uint32_t WGLUT30;  ///< 0x88 [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 31 of 32)
  volatile uint32_t WGLUT31;  ///< 0x8c [R/W]: WGLUTx : WaveGen DAC Input LUT (Instance 32 of 32)
} WAVEGEN_LUT_REGS_t;

#endif // WAVEGEN_LUT
