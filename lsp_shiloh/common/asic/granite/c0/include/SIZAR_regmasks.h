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
 * \file SIZAR_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SIZAR_REGMASKS_H_
#define _SIZAR_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Sizar Calculations (SIZAR)
/** \brief Top-level register file for the Sizar block*/
//
//====================================================================

//====================================================================
//Register: Status Control Register (SizarCSR)
/** \brief This register holds the control bits for the SIZAR block. Bit 0 enables SIZAR to run for the number of pixels
and lines programmed into the SIZAR Output Size register. When the requested pixels and lines are created
Bit 0 will be cleared and SIZAR will return to the soft reset state. By default the output pixel value will
be calculated using bilinear interpolation. To change the interpolation type set one of bits 1, 2, or 3.
Bit 4 enables a 90 degree clockwise rotation of the image using pixel mapping provided other parameters are
programmed appropriately. Bit 5 enables a 90 degree counter-clockwise rotation of the image using pixel mapping
provided other parameters are programmed appropriately. Bit 6 determines what value will be written for a white pixel.
Bit 7 tells the block to use the SIZAR Alternate Step register values to calculate the position of the beginning of
the next line relative to the beginning of the current line.*/
//====================================================================

#define  SIZAR_SIZARCSR_WHITE_VAL_MASK 0xff000000
#define  SIZAR_SIZARCSR_WHITE_VAL_SHIFT 24
#define  SIZAR_SIZARCSR_WHITE_VAL_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_WHITE_VAL_MASK) >> SIZAR_SIZARCSR_WHITE_VAL_SHIFT)
#define  SIZAR_SIZARCSR_WHITE_VAL_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_WHITE_VAL_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_WHITE_VAL_SHIFT))

#define  SIZAR_SIZARCSR_RESERVED1_MASK 0x800000
#define  SIZAR_SIZARCSR_RESERVED1_SHIFT 23
#define  SIZAR_SIZARCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_RESERVED1_MASK) >> SIZAR_SIZARCSR_RESERVED1_SHIFT)
#define  SIZAR_SIZARCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_RESERVED1_SHIFT))

#define  SIZAR_SIZARCSR_BURST_SIZE_MASK 0x700000
#define  SIZAR_SIZARCSR_BURST_SIZE_SHIFT 20
#define  SIZAR_SIZARCSR_BURST_SIZE_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_BURST_SIZE_MASK) >> SIZAR_SIZARCSR_BURST_SIZE_SHIFT)
#define  SIZAR_SIZARCSR_BURST_SIZE_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_BURST_SIZE_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_BURST_SIZE_SHIFT))

#define  SIZAR_SIZARCSR_AVG_4_LINES_MASK 0x80000
#define  SIZAR_SIZARCSR_AVG_4_LINES_SHIFT 19
#define  SIZAR_SIZARCSR_AVG_4_LINES_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_AVG_4_LINES_MASK) >> SIZAR_SIZARCSR_AVG_4_LINES_SHIFT)
#define  SIZAR_SIZARCSR_AVG_4_LINES_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_AVG_4_LINES_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_AVG_4_LINES_SHIFT))

#define  SIZAR_SIZARCSR_AVG_3_LINES_MASK 0x40000
#define  SIZAR_SIZARCSR_AVG_3_LINES_SHIFT 18
#define  SIZAR_SIZARCSR_AVG_3_LINES_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_AVG_3_LINES_MASK) >> SIZAR_SIZARCSR_AVG_3_LINES_SHIFT)
#define  SIZAR_SIZARCSR_AVG_3_LINES_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_AVG_3_LINES_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_AVG_3_LINES_SHIFT))

#define  SIZAR_SIZARCSR_AVG_4_PIX_MASK 0x20000
#define  SIZAR_SIZARCSR_AVG_4_PIX_SHIFT 17
#define  SIZAR_SIZARCSR_AVG_4_PIX_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_AVG_4_PIX_MASK) >> SIZAR_SIZARCSR_AVG_4_PIX_SHIFT)
#define  SIZAR_SIZARCSR_AVG_4_PIX_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_AVG_4_PIX_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_AVG_4_PIX_SHIFT))

#define  SIZAR_SIZARCSR_AVG_3_PIX_MASK 0x10000
#define  SIZAR_SIZARCSR_AVG_3_PIX_SHIFT 16
#define  SIZAR_SIZARCSR_AVG_3_PIX_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_AVG_3_PIX_MASK) >> SIZAR_SIZARCSR_AVG_3_PIX_SHIFT)
#define  SIZAR_SIZARCSR_AVG_3_PIX_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_AVG_3_PIX_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_AVG_3_PIX_SHIFT))

#define  SIZAR_SIZARCSR_RESERVED2_MASK 0xf000
#define  SIZAR_SIZARCSR_RESERVED2_SHIFT 12
#define  SIZAR_SIZARCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_RESERVED2_MASK) >> SIZAR_SIZARCSR_RESERVED2_SHIFT)
#define  SIZAR_SIZARCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_RESERVED2_SHIFT))

#define  SIZAR_SIZARCSR_MATCH_BICUBIC_MASK 0x800
#define  SIZAR_SIZARCSR_MATCH_BICUBIC_SHIFT 11
#define  SIZAR_SIZARCSR_MATCH_BICUBIC_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_MATCH_BICUBIC_MASK) >> SIZAR_SIZARCSR_MATCH_BICUBIC_SHIFT)
#define  SIZAR_SIZARCSR_MATCH_BICUBIC_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_MATCH_BICUBIC_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_MATCH_BICUBIC_SHIFT))

#define  SIZAR_SIZARCSR_RESERVED3_MASK 0x600
#define  SIZAR_SIZARCSR_RESERVED3_SHIFT 9
#define  SIZAR_SIZARCSR_RESERVED3_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_RESERVED3_MASK) >> SIZAR_SIZARCSR_RESERVED3_SHIFT)
#define  SIZAR_SIZARCSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_RESERVED3_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_RESERVED3_SHIFT))

#define  SIZAR_SIZARCSR_DEBUG_MASK 0x100
#define  SIZAR_SIZARCSR_DEBUG_SHIFT 8
#define  SIZAR_SIZARCSR_DEBUG_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_DEBUG_MASK) >> SIZAR_SIZARCSR_DEBUG_SHIFT)
#define  SIZAR_SIZARCSR_DEBUG_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_DEBUG_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_DEBUG_SHIFT))

#define  SIZAR_SIZARCSR_RESERVED4_MASK 0xe0
#define  SIZAR_SIZARCSR_RESERVED4_SHIFT 5
#define  SIZAR_SIZARCSR_RESERVED4_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_RESERVED4_MASK) >> SIZAR_SIZARCSR_RESERVED4_SHIFT)
#define  SIZAR_SIZARCSR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_RESERVED4_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_RESERVED4_SHIFT))

#define  SIZAR_SIZARCSR_MAP_MODE_MASK 0x10
#define  SIZAR_SIZARCSR_MAP_MODE_SHIFT 4
#define  SIZAR_SIZARCSR_MAP_MODE_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_MAP_MODE_MASK) >> SIZAR_SIZARCSR_MAP_MODE_SHIFT)
#define  SIZAR_SIZARCSR_MAP_MODE_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_MAP_MODE_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_MAP_MODE_SHIFT))

#define  SIZAR_SIZARCSR_PIXEL_MAP_MASK 0x8
#define  SIZAR_SIZARCSR_PIXEL_MAP_SHIFT 3
#define  SIZAR_SIZARCSR_PIXEL_MAP_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_PIXEL_MAP_MASK) >> SIZAR_SIZARCSR_PIXEL_MAP_SHIFT)
#define  SIZAR_SIZARCSR_PIXEL_MAP_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_PIXEL_MAP_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_PIXEL_MAP_SHIFT))

#define  SIZAR_SIZARCSR_AVERAGE_MASK 0x4
#define  SIZAR_SIZARCSR_AVERAGE_SHIFT 2
#define  SIZAR_SIZARCSR_AVERAGE_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_AVERAGE_MASK) >> SIZAR_SIZARCSR_AVERAGE_SHIFT)
#define  SIZAR_SIZARCSR_AVERAGE_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_AVERAGE_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_AVERAGE_SHIFT))

#define  SIZAR_SIZARCSR_BICUBIC_MASK 0x2
#define  SIZAR_SIZARCSR_BICUBIC_SHIFT 1
#define  SIZAR_SIZARCSR_BICUBIC_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_BICUBIC_MASK) >> SIZAR_SIZARCSR_BICUBIC_SHIFT)
#define  SIZAR_SIZARCSR_BICUBIC_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_BICUBIC_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_BICUBIC_SHIFT))

#define  SIZAR_SIZARCSR_SIZAR_GO_MASK 0x1
#define  SIZAR_SIZARCSR_SIZAR_GO_SHIFT 0
#define  SIZAR_SIZARCSR_SIZAR_GO_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCSR_SIZAR_GO_MASK) >> SIZAR_SIZARCSR_SIZAR_GO_SHIFT)
#define  SIZAR_SIZARCSR_SIZAR_GO_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCSR_SIZAR_GO_MASK) | (((uint32_t)val) << SIZAR_SIZARCSR_SIZAR_GO_SHIFT))

//====================================================================
//Register: Output Size Register (SizarOutSize)
/** \brief This register is used to indicate the desired size of the output image generated by Sizar.
Note: Firmware must guarantee that there is enough input data available to calculate all requested output pixels.*/
//====================================================================

#define  SIZAR_SIZAROUTSIZE_LINES_OUT_MASK 0xffff0000
#define  SIZAR_SIZAROUTSIZE_LINES_OUT_SHIFT 16
#define  SIZAR_SIZAROUTSIZE_LINES_OUT_MASK_SHIFT(reg) (((reg) & SIZAR_SIZAROUTSIZE_LINES_OUT_MASK) >> SIZAR_SIZAROUTSIZE_LINES_OUT_SHIFT)
#define  SIZAR_SIZAROUTSIZE_LINES_OUT_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZAROUTSIZE_LINES_OUT_MASK) | (((uint32_t)val) << SIZAR_SIZAROUTSIZE_LINES_OUT_SHIFT))

#define  SIZAR_SIZAROUTSIZE_PELS_OUT_MASK 0xffff
#define  SIZAR_SIZAROUTSIZE_PELS_OUT_SHIFT 0
#define  SIZAR_SIZAROUTSIZE_PELS_OUT_MASK_SHIFT(reg) (((reg) & SIZAR_SIZAROUTSIZE_PELS_OUT_MASK) >> SIZAR_SIZAROUTSIZE_PELS_OUT_SHIFT)
#define  SIZAR_SIZAROUTSIZE_PELS_OUT_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZAROUTSIZE_PELS_OUT_MASK) | (((uint32_t)val) << SIZAR_SIZAROUTSIZE_PELS_OUT_SHIFT))

//====================================================================
//Register: X Position Register (SizarXPos)
/** \brief This register holds the current X position within the input image from which the next output pixel will be calculated.
It should be programmed with the starting position at the beginning of each operation. At the end of the operation the
value in this register will be the start position of the next line below the last output line. This value can be stored
by FW to allow block to be used by another process and restored when resuming this operation. The entire register is
actually one position, but is separated here for clarity. The number programmed here is signed and should use 2's 
complement negative numbers.*/
//====================================================================

#define  SIZAR_SIZARXPOS_POS_XPIXEL_MASK 0xffff0000
#define  SIZAR_SIZARXPOS_POS_XPIXEL_SHIFT 16
#define  SIZAR_SIZARXPOS_POS_XPIXEL_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARXPOS_POS_XPIXEL_MASK) >> SIZAR_SIZARXPOS_POS_XPIXEL_SHIFT)
#define  SIZAR_SIZARXPOS_POS_XPIXEL_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARXPOS_POS_XPIXEL_MASK) | (((uint32_t)val) << SIZAR_SIZARXPOS_POS_XPIXEL_SHIFT))

#define  SIZAR_SIZARXPOS_POS_XGRID_MASK 0xffff
#define  SIZAR_SIZARXPOS_POS_XGRID_SHIFT 0
#define  SIZAR_SIZARXPOS_POS_XGRID_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARXPOS_POS_XGRID_MASK) >> SIZAR_SIZARXPOS_POS_XGRID_SHIFT)
#define  SIZAR_SIZARXPOS_POS_XGRID_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARXPOS_POS_XGRID_MASK) | (((uint32_t)val) << SIZAR_SIZARXPOS_POS_XGRID_SHIFT))

//====================================================================
//Register: Y Position Register (SizarYPos)
/** \brief This register holds the current Y position within the input image from which the next output pixel will be calculated.
It should be programmed with the starting position at the beginning of each operation. At the end of the operation the
value in this register will be the start position of the next line below the last output line. This value can be stored
by FW to allow block to be used by another process and restored when resuming this operation. The entire register is
actually one position, but is separated here for clarity. The number programmed here is signed and should use 2's 
complement negative numbers.*/
//====================================================================

#define  SIZAR_SIZARYPOS_POS_YPIXEL_MASK 0xffff0000
#define  SIZAR_SIZARYPOS_POS_YPIXEL_SHIFT 16
#define  SIZAR_SIZARYPOS_POS_YPIXEL_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARYPOS_POS_YPIXEL_MASK) >> SIZAR_SIZARYPOS_POS_YPIXEL_SHIFT)
#define  SIZAR_SIZARYPOS_POS_YPIXEL_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARYPOS_POS_YPIXEL_MASK) | (((uint32_t)val) << SIZAR_SIZARYPOS_POS_YPIXEL_SHIFT))

#define  SIZAR_SIZARYPOS_POS_YGRID_MASK 0xffff
#define  SIZAR_SIZARYPOS_POS_YGRID_SHIFT 0
#define  SIZAR_SIZARYPOS_POS_YGRID_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARYPOS_POS_YGRID_MASK) >> SIZAR_SIZARYPOS_POS_YGRID_SHIFT)
#define  SIZAR_SIZARYPOS_POS_YGRID_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARYPOS_POS_YGRID_MASK) | (((uint32_t)val) << SIZAR_SIZARYPOS_POS_YGRID_SHIFT))

//====================================================================
//Register: X Step Register (SizarXStep)
/** \brief This register directs the block on the size of step that should be taken in the X direction between output pixels. This
register and the next hold the information about the scale size and the rotation angle. After each output pixel is
calculated the XSTEP value is added to the POS_X value to create the next X position. The number programmed here is signed
and should use 2's complement negative numbers.  Calculation of this value, along with the calculation of alternate step values, 
is described in an application note at the end of this document.*/
//====================================================================

#define  SIZAR_SIZARXSTEP_RESERVED1_MASK 0xfff00000
#define  SIZAR_SIZARXSTEP_RESERVED1_SHIFT 20
#define  SIZAR_SIZARXSTEP_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARXSTEP_RESERVED1_MASK) >> SIZAR_SIZARXSTEP_RESERVED1_SHIFT)
#define  SIZAR_SIZARXSTEP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARXSTEP_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARXSTEP_RESERVED1_SHIFT))

#define  SIZAR_SIZARXSTEP_XSTEP_MASK 0xfffff
#define  SIZAR_SIZARXSTEP_XSTEP_SHIFT 0
#define  SIZAR_SIZARXSTEP_XSTEP_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARXSTEP_XSTEP_MASK) >> SIZAR_SIZARXSTEP_XSTEP_SHIFT)
#define  SIZAR_SIZARXSTEP_XSTEP_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARXSTEP_XSTEP_MASK) | (((uint32_t)val) << SIZAR_SIZARXSTEP_XSTEP_SHIFT))

//====================================================================
//Register: Y Step Register (SizarYStep)
/** \brief This register directs the block on the size of step that should be taken in the Y direction between output pixels. This register and
the previous hold the information about the scale size and the rotation angle. After each output pixel is calculated the YSTEP values
is added to the POS_Y value to create the next Y position. The number programmed here is signed and should use 2's complement negative 
numbers. Calculation of this value, along with the calculation of alternate step values, is described in an application note at the end
of this document.*/
//====================================================================

#define  SIZAR_SIZARYSTEP_RESERVED1_MASK 0xfff00000
#define  SIZAR_SIZARYSTEP_RESERVED1_SHIFT 20
#define  SIZAR_SIZARYSTEP_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARYSTEP_RESERVED1_MASK) >> SIZAR_SIZARYSTEP_RESERVED1_SHIFT)
#define  SIZAR_SIZARYSTEP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARYSTEP_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARYSTEP_RESERVED1_SHIFT))

#define  SIZAR_SIZARYSTEP_YSTEP_MASK 0xfffff
#define  SIZAR_SIZARYSTEP_YSTEP_SHIFT 0
#define  SIZAR_SIZARYSTEP_YSTEP_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARYSTEP_YSTEP_MASK) >> SIZAR_SIZARYSTEP_YSTEP_SHIFT)
#define  SIZAR_SIZARYSTEP_YSTEP_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARYSTEP_YSTEP_MASK) | (((uint32_t)val) << SIZAR_SIZARYSTEP_YSTEP_SHIFT))

//====================================================================
//Register: Input Size Register (SizarInSize)
/** \brief This register should be programmed with the size of the input image. This allows the block to generate white data for output pixels
that fall outside of the input image.*/
//====================================================================

#define  SIZAR_SIZARINSIZE_RESERVED1_MASK 0x80000000
#define  SIZAR_SIZARINSIZE_RESERVED1_SHIFT 31
#define  SIZAR_SIZARINSIZE_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARINSIZE_RESERVED1_MASK) >> SIZAR_SIZARINSIZE_RESERVED1_SHIFT)
#define  SIZAR_SIZARINSIZE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARINSIZE_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARINSIZE_RESERVED1_SHIFT))

#define  SIZAR_SIZARINSIZE_IN_LINES_MASK 0x7fff0000
#define  SIZAR_SIZARINSIZE_IN_LINES_SHIFT 16
#define  SIZAR_SIZARINSIZE_IN_LINES_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARINSIZE_IN_LINES_MASK) >> SIZAR_SIZARINSIZE_IN_LINES_SHIFT)
#define  SIZAR_SIZARINSIZE_IN_LINES_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARINSIZE_IN_LINES_MASK) | (((uint32_t)val) << SIZAR_SIZARINSIZE_IN_LINES_SHIFT))

#define  SIZAR_SIZARINSIZE_RESERVED2_MASK 0x8000
#define  SIZAR_SIZARINSIZE_RESERVED2_SHIFT 15
#define  SIZAR_SIZARINSIZE_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARINSIZE_RESERVED2_MASK) >> SIZAR_SIZARINSIZE_RESERVED2_SHIFT)
#define  SIZAR_SIZARINSIZE_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARINSIZE_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_SIZARINSIZE_RESERVED2_SHIFT))

#define  SIZAR_SIZARINSIZE_IN_PIXELS_MASK 0x7fff
#define  SIZAR_SIZARINSIZE_IN_PIXELS_SHIFT 0
#define  SIZAR_SIZARINSIZE_IN_PIXELS_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARINSIZE_IN_PIXELS_MASK) >> SIZAR_SIZARINSIZE_IN_PIXELS_SHIFT)
#define  SIZAR_SIZARINSIZE_IN_PIXELS_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARINSIZE_IN_PIXELS_MASK) | (((uint32_t)val) << SIZAR_SIZARINSIZE_IN_PIXELS_SHIFT))

//====================================================================
//Register: Band and Line Number Register (SizarBandLineNum)
/** \brief To maintain the pointers for any pixel within the input SIZAR must have a pointer to a line, what line number that is, and what band
that line falls in. The line number chosen should be close (or equal) to the first line needed during interpolation. Program the
band and line numbers here that corresponds to the pointer programmed in the SIZAR Current Ptr register below.*/
//====================================================================

#define  SIZAR_SIZARBANDLINENUM_RESERVED1_MASK 0x80000000
#define  SIZAR_SIZARBANDLINENUM_RESERVED1_SHIFT 31
#define  SIZAR_SIZARBANDLINENUM_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARBANDLINENUM_RESERVED1_MASK) >> SIZAR_SIZARBANDLINENUM_RESERVED1_SHIFT)
#define  SIZAR_SIZARBANDLINENUM_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARBANDLINENUM_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARBANDLINENUM_RESERVED1_SHIFT))

#define  SIZAR_SIZARBANDLINENUM_CUR_LINE_MASK 0x7fff0000
#define  SIZAR_SIZARBANDLINENUM_CUR_LINE_SHIFT 16
#define  SIZAR_SIZARBANDLINENUM_CUR_LINE_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARBANDLINENUM_CUR_LINE_MASK) >> SIZAR_SIZARBANDLINENUM_CUR_LINE_SHIFT)
#define  SIZAR_SIZARBANDLINENUM_CUR_LINE_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARBANDLINENUM_CUR_LINE_MASK) | (((uint32_t)val) << SIZAR_SIZARBANDLINENUM_CUR_LINE_SHIFT))

#define  SIZAR_SIZARBANDLINENUM_RESERVED2_MASK 0xf000
#define  SIZAR_SIZARBANDLINENUM_RESERVED2_SHIFT 12
#define  SIZAR_SIZARBANDLINENUM_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARBANDLINENUM_RESERVED2_MASK) >> SIZAR_SIZARBANDLINENUM_RESERVED2_SHIFT)
#define  SIZAR_SIZARBANDLINENUM_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARBANDLINENUM_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_SIZARBANDLINENUM_RESERVED2_SHIFT))

#define  SIZAR_SIZARBANDLINENUM_BAND_NUM_MASK 0xfff
#define  SIZAR_SIZARBANDLINENUM_BAND_NUM_SHIFT 0
#define  SIZAR_SIZARBANDLINENUM_BAND_NUM_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARBANDLINENUM_BAND_NUM_MASK) >> SIZAR_SIZARBANDLINENUM_BAND_NUM_SHIFT)
#define  SIZAR_SIZARBANDLINENUM_BAND_NUM_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARBANDLINENUM_BAND_NUM_MASK) | (((uint32_t)val) << SIZAR_SIZARBANDLINENUM_BAND_NUM_SHIFT))

//====================================================================
//Register: Current Pointer Register (SizarCurPtr)
/** \brief This register contains the pointer to the line specified in the SIZAR Current Position register above. The input burst size field should be
programmed with the number of DMA words per burst minus 1. This value will be smaller for more extreme rotation angles.*/
//====================================================================

#define  SIZAR_SIZARCURPTR_CUR_PTR_MASK 0xfffffffc
#define  SIZAR_SIZARCURPTR_CUR_PTR_SHIFT 2
#define  SIZAR_SIZARCURPTR_CUR_PTR_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCURPTR_CUR_PTR_MASK) >> SIZAR_SIZARCURPTR_CUR_PTR_SHIFT)
#define  SIZAR_SIZARCURPTR_CUR_PTR_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCURPTR_CUR_PTR_MASK) | (((uint32_t)val) << SIZAR_SIZARCURPTR_CUR_PTR_SHIFT))

#define  SIZAR_SIZARCURPTR_RESERVED1_MASK 0x3
#define  SIZAR_SIZARCURPTR_RESERVED1_SHIFT 0
#define  SIZAR_SIZARCURPTR_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARCURPTR_RESERVED1_MASK) >> SIZAR_SIZARCURPTR_RESERVED1_SHIFT)
#define  SIZAR_SIZARCURPTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARCURPTR_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARCURPTR_RESERVED1_SHIFT))

//====================================================================
//Register: Band Structure Pointer Register (SizarBandPtr)
/** \brief This register should be programmed with the pointer to the first structure in the array of structures which gives information
about the location and parameters of the input bands. The format of each structure is given at the end of this document.*/
//====================================================================

#define  SIZAR_SIZARBANDPTR_STRUCT_PTR_MASK 0xfffffffc
#define  SIZAR_SIZARBANDPTR_STRUCT_PTR_SHIFT 2
#define  SIZAR_SIZARBANDPTR_STRUCT_PTR_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARBANDPTR_STRUCT_PTR_MASK) >> SIZAR_SIZARBANDPTR_STRUCT_PTR_SHIFT)
#define  SIZAR_SIZARBANDPTR_STRUCT_PTR_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARBANDPTR_STRUCT_PTR_MASK) | (((uint32_t)val) << SIZAR_SIZARBANDPTR_STRUCT_PTR_SHIFT))

#define  SIZAR_SIZARBANDPTR_RESERVED1_MASK 0x3
#define  SIZAR_SIZARBANDPTR_RESERVED1_SHIFT 0
#define  SIZAR_SIZARBANDPTR_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARBANDPTR_RESERVED1_MASK) >> SIZAR_SIZARBANDPTR_RESERVED1_SHIFT)
#define  SIZAR_SIZARBANDPTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARBANDPTR_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARBANDPTR_RESERVED1_SHIFT))

//====================================================================
//Register: Output Pointer Register (SizarOutPtr)
/** \brief This register should be programmed with the output memory pointer.*/
//====================================================================

#define  SIZAR_SIZAROUTPTR_OUT_PTR_MASK 0xfffffffc
#define  SIZAR_SIZAROUTPTR_OUT_PTR_SHIFT 2
#define  SIZAR_SIZAROUTPTR_OUT_PTR_MASK_SHIFT(reg) (((reg) & SIZAR_SIZAROUTPTR_OUT_PTR_MASK) >> SIZAR_SIZAROUTPTR_OUT_PTR_SHIFT)
#define  SIZAR_SIZAROUTPTR_OUT_PTR_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZAROUTPTR_OUT_PTR_MASK) | (((uint32_t)val) << SIZAR_SIZAROUTPTR_OUT_PTR_SHIFT))

#define  SIZAR_SIZAROUTPTR_RESERVED1_MASK 0x3
#define  SIZAR_SIZAROUTPTR_RESERVED1_SHIFT 0
#define  SIZAR_SIZAROUTPTR_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZAROUTPTR_RESERVED1_MASK) >> SIZAR_SIZAROUTPTR_RESERVED1_SHIFT)
#define  SIZAR_SIZAROUTPTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZAROUTPTR_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZAROUTPTR_RESERVED1_SHIFT))

//====================================================================
//Register: Output Step Register (SizarOutStep)
/** \brief This register should be programmed with the allocated size of each output line. This should be at least as large as the Pixels Out
field in the SIZAR Output Size register.*/
//====================================================================

#define  SIZAR_SIZAROUTSTEP_RESERVED1_MASK 0xffff0000
#define  SIZAR_SIZAROUTSTEP_RESERVED1_SHIFT 16
#define  SIZAR_SIZAROUTSTEP_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZAROUTSTEP_RESERVED1_MASK) >> SIZAR_SIZAROUTSTEP_RESERVED1_SHIFT)
#define  SIZAR_SIZAROUTSTEP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZAROUTSTEP_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZAROUTSTEP_RESERVED1_SHIFT))

#define  SIZAR_SIZAROUTSTEP_OUT_STEP_MASK 0xfffc
#define  SIZAR_SIZAROUTSTEP_OUT_STEP_SHIFT 2
#define  SIZAR_SIZAROUTSTEP_OUT_STEP_MASK_SHIFT(reg) (((reg) & SIZAR_SIZAROUTSTEP_OUT_STEP_MASK) >> SIZAR_SIZAROUTSTEP_OUT_STEP_SHIFT)
#define  SIZAR_SIZAROUTSTEP_OUT_STEP_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZAROUTSTEP_OUT_STEP_MASK) | (((uint32_t)val) << SIZAR_SIZAROUTSTEP_OUT_STEP_SHIFT))

#define  SIZAR_SIZAROUTSTEP_RESERVED2_MASK 0x3
#define  SIZAR_SIZAROUTSTEP_RESERVED2_SHIFT 0
#define  SIZAR_SIZAROUTSTEP_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZAROUTSTEP_RESERVED2_MASK) >> SIZAR_SIZAROUTSTEP_RESERVED2_SHIFT)
#define  SIZAR_SIZAROUTSTEP_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZAROUTSTEP_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_SIZAROUTSTEP_RESERVED2_SHIFT))

//====================================================================
//Register: Valid Band Register (SizarValBand)
/** \brief Valid in memory indicates that the memory is allocated the data is valid and the band information structure for this band is also valid.*/
//====================================================================

#define  SIZAR_SIZARVALBAND_RESERVED1_MASK 0xf0000000
#define  SIZAR_SIZARVALBAND_RESERVED1_SHIFT 28
#define  SIZAR_SIZARVALBAND_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARVALBAND_RESERVED1_MASK) >> SIZAR_SIZARVALBAND_RESERVED1_SHIFT)
#define  SIZAR_SIZARVALBAND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARVALBAND_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARVALBAND_RESERVED1_SHIFT))

#define  SIZAR_SIZARVALBAND_HIGH_VALID_MASK 0xfff0000
#define  SIZAR_SIZARVALBAND_HIGH_VALID_SHIFT 16
#define  SIZAR_SIZARVALBAND_HIGH_VALID_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARVALBAND_HIGH_VALID_MASK) >> SIZAR_SIZARVALBAND_HIGH_VALID_SHIFT)
#define  SIZAR_SIZARVALBAND_HIGH_VALID_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARVALBAND_HIGH_VALID_MASK) | (((uint32_t)val) << SIZAR_SIZARVALBAND_HIGH_VALID_SHIFT))

#define  SIZAR_SIZARVALBAND_RESERVED2_MASK 0xf000
#define  SIZAR_SIZARVALBAND_RESERVED2_SHIFT 12
#define  SIZAR_SIZARVALBAND_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARVALBAND_RESERVED2_MASK) >> SIZAR_SIZARVALBAND_RESERVED2_SHIFT)
#define  SIZAR_SIZARVALBAND_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARVALBAND_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_SIZARVALBAND_RESERVED2_SHIFT))

#define  SIZAR_SIZARVALBAND_LOW_VALID_MASK 0xfff
#define  SIZAR_SIZARVALBAND_LOW_VALID_SHIFT 0
#define  SIZAR_SIZARVALBAND_LOW_VALID_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARVALBAND_LOW_VALID_MASK) >> SIZAR_SIZARVALBAND_LOW_VALID_SHIFT)
#define  SIZAR_SIZARVALBAND_LOW_VALID_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARVALBAND_LOW_VALID_MASK) | (((uint32_t)val) << SIZAR_SIZARVALBAND_LOW_VALID_SHIFT))

//====================================================================
//Register: Sizar Debug 7 Register (SizarDBG7)
//====================================================================

#define  SIZAR_SIZARDBG7_PX_CNT_MASK 0xffff0000
#define  SIZAR_SIZARDBG7_PX_CNT_SHIFT 16
#define  SIZAR_SIZARDBG7_PX_CNT_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG7_PX_CNT_MASK) >> SIZAR_SIZARDBG7_PX_CNT_SHIFT)
#define  SIZAR_SIZARDBG7_PX_CNT_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG7_PX_CNT_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG7_PX_CNT_SHIFT))

#define  SIZAR_SIZARDBG7_MMU_SM_MASK 0xff00
#define  SIZAR_SIZARDBG7_MMU_SM_SHIFT 8
#define  SIZAR_SIZARDBG7_MMU_SM_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG7_MMU_SM_MASK) >> SIZAR_SIZARDBG7_MMU_SM_SHIFT)
#define  SIZAR_SIZARDBG7_MMU_SM_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG7_MMU_SM_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG7_MMU_SM_SHIFT))

#define  SIZAR_SIZARDBG7_RESERVED1_MASK 0xc0
#define  SIZAR_SIZARDBG7_RESERVED1_SHIFT 6
#define  SIZAR_SIZARDBG7_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG7_RESERVED1_MASK) >> SIZAR_SIZARDBG7_RESERVED1_SHIFT)
#define  SIZAR_SIZARDBG7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG7_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG7_RESERVED1_SHIFT))

#define  SIZAR_SIZARDBG7_CNTRL_SM_MASK 0x3f
#define  SIZAR_SIZARDBG7_CNTRL_SM_SHIFT 0
#define  SIZAR_SIZARDBG7_CNTRL_SM_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG7_CNTRL_SM_MASK) >> SIZAR_SIZARDBG7_CNTRL_SM_SHIFT)
#define  SIZAR_SIZARDBG7_CNTRL_SM_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG7_CNTRL_SM_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG7_CNTRL_SM_SHIFT))

//====================================================================
//Register: Interrupt Register (SizarIRQ)
/** \brief This register is used to control and observe the status of the SIZAR interrupts.*/
//====================================================================

#define  SIZAR_SIZARIRQ_FORCE_IRQ_MASK 0x80000000
#define  SIZAR_SIZARIRQ_FORCE_IRQ_SHIFT 31
#define  SIZAR_SIZARIRQ_FORCE_IRQ_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_FORCE_IRQ_MASK) >> SIZAR_SIZARIRQ_FORCE_IRQ_SHIFT)
#define  SIZAR_SIZARIRQ_FORCE_IRQ_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_FORCE_IRQ_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_FORCE_IRQ_SHIFT))

#define  SIZAR_SIZARIRQ_RESERVED1_MASK 0x7fe00000
#define  SIZAR_SIZARIRQ_RESERVED1_SHIFT 21
#define  SIZAR_SIZARIRQ_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_RESERVED1_MASK) >> SIZAR_SIZARIRQ_RESERVED1_SHIFT)
#define  SIZAR_SIZARIRQ_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_RESERVED1_SHIFT))

#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_MASK 0x100000
#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_SHIFT 20
#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_MASK) >> SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_SHIFT)
#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_WR_FAULT_IRQ_EN_SHIFT))

#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_MASK 0x80000
#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_SHIFT 19
#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_MASK) >> SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_SHIFT)
#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_RD_FAULT_IRQ_EN_SHIFT))

#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_MASK 0x40000
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_SHIFT 18
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_MASK) >> SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_SHIFT)
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_EN_SHIFT))

#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_MASK 0x20000
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_SHIFT 17
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_MASK) >> SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_SHIFT)
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_EN_SHIFT))

#define  SIZAR_SIZARIRQ_DONE_IRQ_EN_MASK 0x10000
#define  SIZAR_SIZARIRQ_DONE_IRQ_EN_SHIFT 16
#define  SIZAR_SIZARIRQ_DONE_IRQ_EN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_DONE_IRQ_EN_MASK) >> SIZAR_SIZARIRQ_DONE_IRQ_EN_SHIFT)
#define  SIZAR_SIZARIRQ_DONE_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_DONE_IRQ_EN_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_DONE_IRQ_EN_SHIFT))

#define  SIZAR_SIZARIRQ_RESERVED2_MASK 0xffe0
#define  SIZAR_SIZARIRQ_RESERVED2_SHIFT 5
#define  SIZAR_SIZARIRQ_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_RESERVED2_MASK) >> SIZAR_SIZARIRQ_RESERVED2_SHIFT)
#define  SIZAR_SIZARIRQ_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_RESERVED2_SHIFT))

#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_MASK 0x10
#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_SHIFT 4
#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_MASK) >> SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_SHIFT)
#define  SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_WR_FAULT_IRQ_PND_SHIFT))

#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_MASK 0x8
#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_SHIFT 3
#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_MASK) >> SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_SHIFT)
#define  SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_RD_FAULT_IRQ_PND_SHIFT))

#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_MASK 0x4
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_SHIFT 2
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_MASK) >> SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_SHIFT)
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_OUT_OF_BAND_STRUCT_IRQ_PND_SHIFT))

#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_MASK 0x2
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_SHIFT 1
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_MASK) >> SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_SHIFT)
#define  SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_OUT_OF_BAND_IRQ_PND_SHIFT))

#define  SIZAR_SIZARIRQ_DONE_IRQ_PND_MASK 0x1
#define  SIZAR_SIZARIRQ_DONE_IRQ_PND_SHIFT 0
#define  SIZAR_SIZARIRQ_DONE_IRQ_PND_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARIRQ_DONE_IRQ_PND_MASK) >> SIZAR_SIZARIRQ_DONE_IRQ_PND_SHIFT)
#define  SIZAR_SIZARIRQ_DONE_IRQ_PND_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARIRQ_DONE_IRQ_PND_MASK) | (((uint32_t)val) << SIZAR_SIZARIRQ_DONE_IRQ_PND_SHIFT))

//====================================================================
//Register: Sizar Debug 1 Register (SizarDBG1)
/** \brief This register contains the dot values of the line above the anchor pixel line.*/
//====================================================================

#define  SIZAR_SIZARDBG1_P2_MASK 0xff000000
#define  SIZAR_SIZARDBG1_P2_SHIFT 24
#define  SIZAR_SIZARDBG1_P2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG1_P2_MASK) >> SIZAR_SIZARDBG1_P2_SHIFT)
#define  SIZAR_SIZARDBG1_P2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG1_P2_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG1_P2_SHIFT))

#define  SIZAR_SIZARDBG1_P1_MASK 0xff0000
#define  SIZAR_SIZARDBG1_P1_SHIFT 16
#define  SIZAR_SIZARDBG1_P1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG1_P1_MASK) >> SIZAR_SIZARDBG1_P1_SHIFT)
#define  SIZAR_SIZARDBG1_P1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG1_P1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG1_P1_SHIFT))

#define  SIZAR_SIZARDBG1_AN_MASK 0xff00
#define  SIZAR_SIZARDBG1_AN_SHIFT 8
#define  SIZAR_SIZARDBG1_AN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG1_AN_MASK) >> SIZAR_SIZARDBG1_AN_SHIFT)
#define  SIZAR_SIZARDBG1_AN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG1_AN_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG1_AN_SHIFT))

#define  SIZAR_SIZARDBG1_M1_MASK 0xff
#define  SIZAR_SIZARDBG1_M1_SHIFT 0
#define  SIZAR_SIZARDBG1_M1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG1_M1_MASK) >> SIZAR_SIZARDBG1_M1_SHIFT)
#define  SIZAR_SIZARDBG1_M1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG1_M1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG1_M1_SHIFT))

//====================================================================
//Register: Sizar Debug 2 Register (SizarDBG2)
/** \brief This register contains the dot values of theanchor pixel line.*/
//====================================================================

#define  SIZAR_SIZARDBG2_P2_MASK 0xff000000
#define  SIZAR_SIZARDBG2_P2_SHIFT 24
#define  SIZAR_SIZARDBG2_P2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG2_P2_MASK) >> SIZAR_SIZARDBG2_P2_SHIFT)
#define  SIZAR_SIZARDBG2_P2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG2_P2_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG2_P2_SHIFT))

#define  SIZAR_SIZARDBG2_P1_MASK 0xff0000
#define  SIZAR_SIZARDBG2_P1_SHIFT 16
#define  SIZAR_SIZARDBG2_P1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG2_P1_MASK) >> SIZAR_SIZARDBG2_P1_SHIFT)
#define  SIZAR_SIZARDBG2_P1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG2_P1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG2_P1_SHIFT))

#define  SIZAR_SIZARDBG2_AN_MASK 0xff00
#define  SIZAR_SIZARDBG2_AN_SHIFT 8
#define  SIZAR_SIZARDBG2_AN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG2_AN_MASK) >> SIZAR_SIZARDBG2_AN_SHIFT)
#define  SIZAR_SIZARDBG2_AN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG2_AN_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG2_AN_SHIFT))

#define  SIZAR_SIZARDBG2_M1_MASK 0xff
#define  SIZAR_SIZARDBG2_M1_SHIFT 0
#define  SIZAR_SIZARDBG2_M1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG2_M1_MASK) >> SIZAR_SIZARDBG2_M1_SHIFT)
#define  SIZAR_SIZARDBG2_M1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG2_M1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG2_M1_SHIFT))

//====================================================================
//Register: Sizar Debug 3 Register (SizarDBG3)
/** \brief This register contains the dot values of the line below the anchor pixel line.*/
//====================================================================

#define  SIZAR_SIZARDBG3_P2_MASK 0xff000000
#define  SIZAR_SIZARDBG3_P2_SHIFT 24
#define  SIZAR_SIZARDBG3_P2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG3_P2_MASK) >> SIZAR_SIZARDBG3_P2_SHIFT)
#define  SIZAR_SIZARDBG3_P2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG3_P2_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG3_P2_SHIFT))

#define  SIZAR_SIZARDBG3_P1_MASK 0xff0000
#define  SIZAR_SIZARDBG3_P1_SHIFT 16
#define  SIZAR_SIZARDBG3_P1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG3_P1_MASK) >> SIZAR_SIZARDBG3_P1_SHIFT)
#define  SIZAR_SIZARDBG3_P1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG3_P1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG3_P1_SHIFT))

#define  SIZAR_SIZARDBG3_AN_MASK 0xff00
#define  SIZAR_SIZARDBG3_AN_SHIFT 8
#define  SIZAR_SIZARDBG3_AN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG3_AN_MASK) >> SIZAR_SIZARDBG3_AN_SHIFT)
#define  SIZAR_SIZARDBG3_AN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG3_AN_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG3_AN_SHIFT))

#define  SIZAR_SIZARDBG3_M1_MASK 0xff
#define  SIZAR_SIZARDBG3_M1_SHIFT 0
#define  SIZAR_SIZARDBG3_M1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG3_M1_MASK) >> SIZAR_SIZARDBG3_M1_SHIFT)
#define  SIZAR_SIZARDBG3_M1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG3_M1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG3_M1_SHIFT))

//====================================================================
//Register: Sizar Debug 4 Register (SizarDBG4)
/** \brief This register contains the dot values of two lines below the anchor pixel line.*/
//====================================================================

#define  SIZAR_SIZARDBG4_P2_MASK 0xff000000
#define  SIZAR_SIZARDBG4_P2_SHIFT 24
#define  SIZAR_SIZARDBG4_P2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG4_P2_MASK) >> SIZAR_SIZARDBG4_P2_SHIFT)
#define  SIZAR_SIZARDBG4_P2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG4_P2_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG4_P2_SHIFT))

#define  SIZAR_SIZARDBG4_P1_MASK 0xff0000
#define  SIZAR_SIZARDBG4_P1_SHIFT 16
#define  SIZAR_SIZARDBG4_P1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG4_P1_MASK) >> SIZAR_SIZARDBG4_P1_SHIFT)
#define  SIZAR_SIZARDBG4_P1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG4_P1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG4_P1_SHIFT))

#define  SIZAR_SIZARDBG4_AN_MASK 0xff00
#define  SIZAR_SIZARDBG4_AN_SHIFT 8
#define  SIZAR_SIZARDBG4_AN_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG4_AN_MASK) >> SIZAR_SIZARDBG4_AN_SHIFT)
#define  SIZAR_SIZARDBG4_AN_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG4_AN_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG4_AN_SHIFT))

#define  SIZAR_SIZARDBG4_M1_MASK 0xff
#define  SIZAR_SIZARDBG4_M1_SHIFT 0
#define  SIZAR_SIZARDBG4_M1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG4_M1_MASK) >> SIZAR_SIZARDBG4_M1_SHIFT)
#define  SIZAR_SIZARDBG4_M1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG4_M1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG4_M1_SHIFT))

//====================================================================
//Register: Sizar Debug 5 Register (SizarDBG5)
/** \brief This register contains the results of the 4 column calculations which are the result of the first stage of the interpolation pipeline.*/
//====================================================================

#define  SIZAR_SIZARDBG5_COL3_MASK 0xff000000
#define  SIZAR_SIZARDBG5_COL3_SHIFT 24
#define  SIZAR_SIZARDBG5_COL3_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG5_COL3_MASK) >> SIZAR_SIZARDBG5_COL3_SHIFT)
#define  SIZAR_SIZARDBG5_COL3_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG5_COL3_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG5_COL3_SHIFT))

#define  SIZAR_SIZARDBG5_COL2_MASK 0xff0000
#define  SIZAR_SIZARDBG5_COL2_SHIFT 16
#define  SIZAR_SIZARDBG5_COL2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG5_COL2_MASK) >> SIZAR_SIZARDBG5_COL2_SHIFT)
#define  SIZAR_SIZARDBG5_COL2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG5_COL2_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG5_COL2_SHIFT))

#define  SIZAR_SIZARDBG5_COL1_MASK 0xff00
#define  SIZAR_SIZARDBG5_COL1_SHIFT 8
#define  SIZAR_SIZARDBG5_COL1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG5_COL1_MASK) >> SIZAR_SIZARDBG5_COL1_SHIFT)
#define  SIZAR_SIZARDBG5_COL1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG5_COL1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG5_COL1_SHIFT))

#define  SIZAR_SIZARDBG5_COL0_MASK 0xff
#define  SIZAR_SIZARDBG5_COL0_SHIFT 0
#define  SIZAR_SIZARDBG5_COL0_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG5_COL0_MASK) >> SIZAR_SIZARDBG5_COL0_SHIFT)
#define  SIZAR_SIZARDBG5_COL0_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG5_COL0_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG5_COL0_SHIFT))

//====================================================================
//Register: Sizar Debug 6 Register (SizarDBG6)
//====================================================================

#define  SIZAR_SIZARDBG6_DEBUG_STEP_MASK 0x80000000
#define  SIZAR_SIZARDBG6_DEBUG_STEP_SHIFT 31
#define  SIZAR_SIZARDBG6_DEBUG_STEP_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG6_DEBUG_STEP_MASK) >> SIZAR_SIZARDBG6_DEBUG_STEP_SHIFT)
#define  SIZAR_SIZARDBG6_DEBUG_STEP_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG6_DEBUG_STEP_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG6_DEBUG_STEP_SHIFT))

#define  SIZAR_SIZARDBG6_Y_FACTOR_MASK 0x7f000000
#define  SIZAR_SIZARDBG6_Y_FACTOR_SHIFT 24
#define  SIZAR_SIZARDBG6_Y_FACTOR_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG6_Y_FACTOR_MASK) >> SIZAR_SIZARDBG6_Y_FACTOR_SHIFT)
#define  SIZAR_SIZARDBG6_Y_FACTOR_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG6_Y_FACTOR_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG6_Y_FACTOR_SHIFT))

#define  SIZAR_SIZARDBG6_RESERVED1_MASK 0x800000
#define  SIZAR_SIZARDBG6_RESERVED1_SHIFT 23
#define  SIZAR_SIZARDBG6_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG6_RESERVED1_MASK) >> SIZAR_SIZARDBG6_RESERVED1_SHIFT)
#define  SIZAR_SIZARDBG6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG6_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG6_RESERVED1_SHIFT))

#define  SIZAR_SIZARDBG6_X_FACTOR_MASK 0x7f0000
#define  SIZAR_SIZARDBG6_X_FACTOR_SHIFT 16
#define  SIZAR_SIZARDBG6_X_FACTOR_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG6_X_FACTOR_MASK) >> SIZAR_SIZARDBG6_X_FACTOR_SHIFT)
#define  SIZAR_SIZARDBG6_X_FACTOR_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG6_X_FACTOR_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG6_X_FACTOR_SHIFT))

#define  SIZAR_SIZARDBG6_RESERVED2_MASK 0xffff
#define  SIZAR_SIZARDBG6_RESERVED2_SHIFT 0
#define  SIZAR_SIZARDBG6_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_SIZARDBG6_RESERVED2_MASK) >> SIZAR_SIZARDBG6_RESERVED2_SHIFT)
#define  SIZAR_SIZARDBG6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_SIZARDBG6_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_SIZARDBG6_RESERVED2_SHIFT))

//====================================================================
//Register: Alternate X Step Register (AltXStep)
/** \brief This register holds the alternate step information that is used when changing lines. At the end of each line the XSTEP_ALT
value will be added to the starting POS_X value to create the starting X position of the next output line. The number programmed
here is signed and should use 2's compliment negative numbers.  See the application note at the end of this document for the 
calculation of this value.*/
//====================================================================

#define  SIZAR_ALTXSTEP_RESERVED1_MASK 0xfff00000
#define  SIZAR_ALTXSTEP_RESERVED1_SHIFT 20
#define  SIZAR_ALTXSTEP_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_ALTXSTEP_RESERVED1_MASK) >> SIZAR_ALTXSTEP_RESERVED1_SHIFT)
#define  SIZAR_ALTXSTEP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_ALTXSTEP_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_ALTXSTEP_RESERVED1_SHIFT))

#define  SIZAR_ALTXSTEP_XSTEP_ALT_MASK 0xfffff
#define  SIZAR_ALTXSTEP_XSTEP_ALT_SHIFT 0
#define  SIZAR_ALTXSTEP_XSTEP_ALT_MASK_SHIFT(reg) (((reg) & SIZAR_ALTXSTEP_XSTEP_ALT_MASK) >> SIZAR_ALTXSTEP_XSTEP_ALT_SHIFT)
#define  SIZAR_ALTXSTEP_XSTEP_ALT_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_ALTXSTEP_XSTEP_ALT_MASK) | (((uint32_t)val) << SIZAR_ALTXSTEP_XSTEP_ALT_SHIFT))

//====================================================================
//Register: Alternate Y Step Register (AltYStep)
/** \brief This register holds the alternate step information that is used when changing lines. At the end of each line the YSTEP_ALT value
will be added to the starting POS_Y value to create the starting Y position of the next output line. The number programmed here is
signed and should use 2's compliment negative numbers.  See the application note at the end of this document for the calculation of 
this value.*/
//====================================================================

#define  SIZAR_ALTYSTEP_RESERVED1_MASK 0xfff00000
#define  SIZAR_ALTYSTEP_RESERVED1_SHIFT 20
#define  SIZAR_ALTYSTEP_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_ALTYSTEP_RESERVED1_MASK) >> SIZAR_ALTYSTEP_RESERVED1_SHIFT)
#define  SIZAR_ALTYSTEP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_ALTYSTEP_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_ALTYSTEP_RESERVED1_SHIFT))

#define  SIZAR_ALTYSTEP_YSTEP_ALT_MASK 0xfffff
#define  SIZAR_ALTYSTEP_YSTEP_ALT_SHIFT 0
#define  SIZAR_ALTYSTEP_YSTEP_ALT_MASK_SHIFT(reg) (((reg) & SIZAR_ALTYSTEP_YSTEP_ALT_MASK) >> SIZAR_ALTYSTEP_YSTEP_ALT_SHIFT)
#define  SIZAR_ALTYSTEP_YSTEP_ALT_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_ALTYSTEP_YSTEP_ALT_MASK) | (((uint32_t)val) << SIZAR_ALTYSTEP_YSTEP_ALT_SHIFT))

//====================================================================
//Register: Map Control Register (MapCtrl)
/** \brief This register holds the control setting for the sparsely populated Map Mode.*/
//====================================================================

#define  SIZAR_MAPCTRL_MAP_PIXEL_CNT_MASK 0xffff0000
#define  SIZAR_MAPCTRL_MAP_PIXEL_CNT_SHIFT 16
#define  SIZAR_MAPCTRL_MAP_PIXEL_CNT_MASK_SHIFT(reg) (((reg) & SIZAR_MAPCTRL_MAP_PIXEL_CNT_MASK) >> SIZAR_MAPCTRL_MAP_PIXEL_CNT_SHIFT)
#define  SIZAR_MAPCTRL_MAP_PIXEL_CNT_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPCTRL_MAP_PIXEL_CNT_MASK) | (((uint32_t)val) << SIZAR_MAPCTRL_MAP_PIXEL_CNT_SHIFT))

#define  SIZAR_MAPCTRL_RESERVED1_MASK 0xc000
#define  SIZAR_MAPCTRL_RESERVED1_SHIFT 14
#define  SIZAR_MAPCTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_MAPCTRL_RESERVED1_MASK) >> SIZAR_MAPCTRL_RESERVED1_SHIFT)
#define  SIZAR_MAPCTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPCTRL_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_MAPCTRL_RESERVED1_SHIFT))

#define  SIZAR_MAPCTRL_MAP_LINE_CNT_MASK 0x3f00
#define  SIZAR_MAPCTRL_MAP_LINE_CNT_SHIFT 8
#define  SIZAR_MAPCTRL_MAP_LINE_CNT_MASK_SHIFT(reg) (((reg) & SIZAR_MAPCTRL_MAP_LINE_CNT_MASK) >> SIZAR_MAPCTRL_MAP_LINE_CNT_SHIFT)
#define  SIZAR_MAPCTRL_MAP_LINE_CNT_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPCTRL_MAP_LINE_CNT_MASK) | (((uint32_t)val) << SIZAR_MAPCTRL_MAP_LINE_CNT_SHIFT))

#define  SIZAR_MAPCTRL_RESERVED2_MASK 0xf8
#define  SIZAR_MAPCTRL_RESERVED2_SHIFT 3
#define  SIZAR_MAPCTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_MAPCTRL_RESERVED2_MASK) >> SIZAR_MAPCTRL_RESERVED2_SHIFT)
#define  SIZAR_MAPCTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPCTRL_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_MAPCTRL_RESERVED2_SHIFT))

#define  SIZAR_MAPCTRL_MAP_STEPS_MASK 0x7
#define  SIZAR_MAPCTRL_MAP_STEPS_SHIFT 0
#define  SIZAR_MAPCTRL_MAP_STEPS_MASK_SHIFT(reg) (((reg) & SIZAR_MAPCTRL_MAP_STEPS_MASK) >> SIZAR_MAPCTRL_MAP_STEPS_SHIFT)
#define  SIZAR_MAPCTRL_MAP_STEPS_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPCTRL_MAP_STEPS_MASK) | (((uint32_t)val) << SIZAR_MAPCTRL_MAP_STEPS_SHIFT))

//====================================================================
//Register: Map Base Address Register (MapBaseAddr)
/** \brief This register contains the pointer to the beginning of the map data.*/
//====================================================================

#define  SIZAR_MAPBASEADDR_MAP_BASE_ADDR_MASK 0xfffffffc
#define  SIZAR_MAPBASEADDR_MAP_BASE_ADDR_SHIFT 2
#define  SIZAR_MAPBASEADDR_MAP_BASE_ADDR_MASK_SHIFT(reg) (((reg) & SIZAR_MAPBASEADDR_MAP_BASE_ADDR_MASK) >> SIZAR_MAPBASEADDR_MAP_BASE_ADDR_SHIFT)
#define  SIZAR_MAPBASEADDR_MAP_BASE_ADDR_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPBASEADDR_MAP_BASE_ADDR_MASK) | (((uint32_t)val) << SIZAR_MAPBASEADDR_MAP_BASE_ADDR_SHIFT))

#define  SIZAR_MAPBASEADDR_RESERVED1_MASK 0x3
#define  SIZAR_MAPBASEADDR_RESERVED1_SHIFT 0
#define  SIZAR_MAPBASEADDR_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_MAPBASEADDR_RESERVED1_MASK) >> SIZAR_MAPBASEADDR_RESERVED1_SHIFT)
#define  SIZAR_MAPBASEADDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPBASEADDR_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_MAPBASEADDR_RESERVED1_SHIFT))

//====================================================================
//Register: Map Line Size Register (MapLineSize)
/** \brief This register contains the size of one line of the map data.*/
//====================================================================

#define  SIZAR_MAPLINESIZE_RESERVED1_MASK 0xffff0000
#define  SIZAR_MAPLINESIZE_RESERVED1_SHIFT 16
#define  SIZAR_MAPLINESIZE_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_MAPLINESIZE_RESERVED1_MASK) >> SIZAR_MAPLINESIZE_RESERVED1_SHIFT)
#define  SIZAR_MAPLINESIZE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPLINESIZE_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_MAPLINESIZE_RESERVED1_SHIFT))

#define  SIZAR_MAPLINESIZE_MAP_LINE_SIZE_MASK 0xfffc
#define  SIZAR_MAPLINESIZE_MAP_LINE_SIZE_SHIFT 2
#define  SIZAR_MAPLINESIZE_MAP_LINE_SIZE_MASK_SHIFT(reg) (((reg) & SIZAR_MAPLINESIZE_MAP_LINE_SIZE_MASK) >> SIZAR_MAPLINESIZE_MAP_LINE_SIZE_SHIFT)
#define  SIZAR_MAPLINESIZE_MAP_LINE_SIZE_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPLINESIZE_MAP_LINE_SIZE_MASK) | (((uint32_t)val) << SIZAR_MAPLINESIZE_MAP_LINE_SIZE_SHIFT))

#define  SIZAR_MAPLINESIZE_RESERVED2_MASK 0x3
#define  SIZAR_MAPLINESIZE_RESERVED2_SHIFT 0
#define  SIZAR_MAPLINESIZE_RESERVED2_MASK_SHIFT(reg) (((reg) & SIZAR_MAPLINESIZE_RESERVED2_MASK) >> SIZAR_MAPLINESIZE_RESERVED2_SHIFT)
#define  SIZAR_MAPLINESIZE_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_MAPLINESIZE_RESERVED2_MASK) | (((uint32_t)val) << SIZAR_MAPLINESIZE_RESERVED2_SHIFT))

//====================================================================
//Register: Reverse Offset Register (ReverseOfst)
/** \brief This register contains the reverse offset.*/
//====================================================================

#define  SIZAR_REVERSEOFST_RESERVED1_MASK 0xffffffc0
#define  SIZAR_REVERSEOFST_RESERVED1_SHIFT 6
#define  SIZAR_REVERSEOFST_RESERVED1_MASK_SHIFT(reg) (((reg) & SIZAR_REVERSEOFST_RESERVED1_MASK) >> SIZAR_REVERSEOFST_RESERVED1_SHIFT)
#define  SIZAR_REVERSEOFST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_REVERSEOFST_RESERVED1_MASK) | (((uint32_t)val) << SIZAR_REVERSEOFST_RESERVED1_SHIFT))

#define  SIZAR_REVERSEOFST_REVERSE_OFFSET_MASK 0x3f
#define  SIZAR_REVERSEOFST_REVERSE_OFFSET_SHIFT 0
#define  SIZAR_REVERSEOFST_REVERSE_OFFSET_MASK_SHIFT(reg) (((reg) & SIZAR_REVERSEOFST_REVERSE_OFFSET_MASK) >> SIZAR_REVERSEOFST_REVERSE_OFFSET_SHIFT)
#define  SIZAR_REVERSEOFST_REVERSE_OFFSET_REPLACE_VAL(reg,val) (((reg) & ~SIZAR_REVERSEOFST_REVERSE_OFFSET_MASK) | (((uint32_t)val) << SIZAR_REVERSEOFST_REVERSE_OFFSET_SHIFT))

#endif // SIZAR
