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
 * \file istone_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ISTONE_REGMASKS_H_
#define _ISTONE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: IsTone Calculations (istone)
/** \brief Top-level register file for the IsTone block*/
//
//====================================================================

//====================================================================
//Register: IsTone Control Register (IsCtrl)
/** \brief This register contains control bits for the tone detection block.*/
//====================================================================

#define  ISTONE_ISCTRL_RESERVED1_MASK 0xffffffe0
#define  ISTONE_ISCTRL_RESERVED1_SHIFT 5
#define  ISTONE_ISCTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & ISTONE_ISCTRL_RESERVED1_MASK) >> ISTONE_ISCTRL_RESERVED1_SHIFT)
#define  ISTONE_ISCTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCTRL_RESERVED1_MASK) | (((uint32_t)val) << ISTONE_ISCTRL_RESERVED1_SHIFT))

#define  ISTONE_ISCTRL_BYPASSALL_MASK 0x10
#define  ISTONE_ISCTRL_BYPASSALL_SHIFT 4
#define  ISTONE_ISCTRL_BYPASSALL_MASK_SHIFT(reg) (((reg) & ISTONE_ISCTRL_BYPASSALL_MASK) >> ISTONE_ISCTRL_BYPASSALL_SHIFT)
#define  ISTONE_ISCTRL_BYPASSALL_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCTRL_BYPASSALL_MASK) | (((uint32_t)val) << ISTONE_ISCTRL_BYPASSALL_SHIFT))

#define  ISTONE_ISCTRL_RESERVED2_MASK 0xe
#define  ISTONE_ISCTRL_RESERVED2_SHIFT 1
#define  ISTONE_ISCTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & ISTONE_ISCTRL_RESERVED2_MASK) >> ISTONE_ISCTRL_RESERVED2_SHIFT)
#define  ISTONE_ISCTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCTRL_RESERVED2_MASK) | (((uint32_t)val) << ISTONE_ISCTRL_RESERVED2_SHIFT))

#define  ISTONE_ISCTRL_ISTONE_EN_MASK 0x1
#define  ISTONE_ISCTRL_ISTONE_EN_SHIFT 0
#define  ISTONE_ISCTRL_ISTONE_EN_MASK_SHIFT(reg) (((reg) & ISTONE_ISCTRL_ISTONE_EN_MASK) >> ISTONE_ISCTRL_ISTONE_EN_SHIFT)
#define  ISTONE_ISCTRL_ISTONE_EN_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCTRL_ISTONE_EN_MASK) | (((uint32_t)val) << ISTONE_ISCTRL_ISTONE_EN_SHIFT))

//====================================================================
//Register: Luminance Thresholds Register (IsLumThresh)
/** \brief This register contains the 2 different sets of luminance channel thresholds that the Y channel must pass for the tone detection logic.*/
//====================================================================

#define  ISTONE_ISLUMTHRESH_YTHRESH_MAX_MASK 0xff000000
#define  ISTONE_ISLUMTHRESH_YTHRESH_MAX_SHIFT 24
#define  ISTONE_ISLUMTHRESH_YTHRESH_MAX_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMTHRESH_YTHRESH_MAX_MASK) >> ISTONE_ISLUMTHRESH_YTHRESH_MAX_SHIFT)
#define  ISTONE_ISLUMTHRESH_YTHRESH_MAX_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMTHRESH_YTHRESH_MAX_MASK) | (((uint32_t)val) << ISTONE_ISLUMTHRESH_YTHRESH_MAX_SHIFT))

#define  ISTONE_ISLUMTHRESH_YTHRESH_MIN_MASK 0xff0000
#define  ISTONE_ISLUMTHRESH_YTHRESH_MIN_SHIFT 16
#define  ISTONE_ISLUMTHRESH_YTHRESH_MIN_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMTHRESH_YTHRESH_MIN_MASK) >> ISTONE_ISLUMTHRESH_YTHRESH_MIN_SHIFT)
#define  ISTONE_ISLUMTHRESH_YTHRESH_MIN_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMTHRESH_YTHRESH_MIN_MASK) | (((uint32_t)val) << ISTONE_ISLUMTHRESH_YTHRESH_MIN_SHIFT))

#define  ISTONE_ISLUMTHRESH_YMAX_MASK 0xff00
#define  ISTONE_ISLUMTHRESH_YMAX_SHIFT 8
#define  ISTONE_ISLUMTHRESH_YMAX_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMTHRESH_YMAX_MASK) >> ISTONE_ISLUMTHRESH_YMAX_SHIFT)
#define  ISTONE_ISLUMTHRESH_YMAX_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMTHRESH_YMAX_MASK) | (((uint32_t)val) << ISTONE_ISLUMTHRESH_YMAX_SHIFT))

#define  ISTONE_ISLUMTHRESH_YMIN_MASK 0xff
#define  ISTONE_ISLUMTHRESH_YMIN_SHIFT 0
#define  ISTONE_ISLUMTHRESH_YMIN_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMTHRESH_YMIN_MASK) >> ISTONE_ISLUMTHRESH_YMIN_SHIFT)
#define  ISTONE_ISLUMTHRESH_YMIN_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMTHRESH_YMIN_MASK) | (((uint32_t)val) << ISTONE_ISLUMTHRESH_YMIN_SHIFT))

//====================================================================
//Register: Chrominance Thresholds Register (IsChromThresh)
/** \brief This register contains the chrominance channel thresholds that the Cr and Cb channels must pass for the tone detection logic.*/
//====================================================================

#define  ISTONE_ISCHROMTHRESH_CRMAX_MASK 0xff000000
#define  ISTONE_ISCHROMTHRESH_CRMAX_SHIFT 24
#define  ISTONE_ISCHROMTHRESH_CRMAX_MASK_SHIFT(reg) (((reg) & ISTONE_ISCHROMTHRESH_CRMAX_MASK) >> ISTONE_ISCHROMTHRESH_CRMAX_SHIFT)
#define  ISTONE_ISCHROMTHRESH_CRMAX_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCHROMTHRESH_CRMAX_MASK) | (((uint32_t)val) << ISTONE_ISCHROMTHRESH_CRMAX_SHIFT))

#define  ISTONE_ISCHROMTHRESH_CRMIN_MASK 0xff0000
#define  ISTONE_ISCHROMTHRESH_CRMIN_SHIFT 16
#define  ISTONE_ISCHROMTHRESH_CRMIN_MASK_SHIFT(reg) (((reg) & ISTONE_ISCHROMTHRESH_CRMIN_MASK) >> ISTONE_ISCHROMTHRESH_CRMIN_SHIFT)
#define  ISTONE_ISCHROMTHRESH_CRMIN_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCHROMTHRESH_CRMIN_MASK) | (((uint32_t)val) << ISTONE_ISCHROMTHRESH_CRMIN_SHIFT))

#define  ISTONE_ISCHROMTHRESH_CBMAX_MASK 0xff00
#define  ISTONE_ISCHROMTHRESH_CBMAX_SHIFT 8
#define  ISTONE_ISCHROMTHRESH_CBMAX_MASK_SHIFT(reg) (((reg) & ISTONE_ISCHROMTHRESH_CBMAX_MASK) >> ISTONE_ISCHROMTHRESH_CBMAX_SHIFT)
#define  ISTONE_ISCHROMTHRESH_CBMAX_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCHROMTHRESH_CBMAX_MASK) | (((uint32_t)val) << ISTONE_ISCHROMTHRESH_CBMAX_SHIFT))

#define  ISTONE_ISCHROMTHRESH_CBMIN_MASK 0xff
#define  ISTONE_ISCHROMTHRESH_CBMIN_SHIFT 0
#define  ISTONE_ISCHROMTHRESH_CBMIN_MASK_SHIFT(reg) (((reg) & ISTONE_ISCHROMTHRESH_CBMIN_MASK) >> ISTONE_ISCHROMTHRESH_CBMIN_SHIFT)
#define  ISTONE_ISCHROMTHRESH_CBMIN_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISCHROMTHRESH_CBMIN_MASK) | (((uint32_t)val) << ISTONE_ISCHROMTHRESH_CBMIN_SHIFT))

//====================================================================
//Register: Rotation Matrix Coefficient Register (IsRotCoeff)
/** \brief This register contains the 4 chrominance rotation matrix coefficients to be used in the tone detection calculations.
These values must be written prior to enabling IsTone function for proper operation.*/
//====================================================================

#define  ISTONE_ISROTCOEFF_ROT22_MASK 0xff000000
#define  ISTONE_ISROTCOEFF_ROT22_SHIFT 24
#define  ISTONE_ISROTCOEFF_ROT22_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTCOEFF_ROT22_MASK) >> ISTONE_ISROTCOEFF_ROT22_SHIFT)
#define  ISTONE_ISROTCOEFF_ROT22_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTCOEFF_ROT22_MASK) | (((uint32_t)val) << ISTONE_ISROTCOEFF_ROT22_SHIFT))

#define  ISTONE_ISROTCOEFF_ROT21_MASK 0xff0000
#define  ISTONE_ISROTCOEFF_ROT21_SHIFT 16
#define  ISTONE_ISROTCOEFF_ROT21_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTCOEFF_ROT21_MASK) >> ISTONE_ISROTCOEFF_ROT21_SHIFT)
#define  ISTONE_ISROTCOEFF_ROT21_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTCOEFF_ROT21_MASK) | (((uint32_t)val) << ISTONE_ISROTCOEFF_ROT21_SHIFT))

#define  ISTONE_ISROTCOEFF_ROT12_MASK 0xff00
#define  ISTONE_ISROTCOEFF_ROT12_SHIFT 8
#define  ISTONE_ISROTCOEFF_ROT12_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTCOEFF_ROT12_MASK) >> ISTONE_ISROTCOEFF_ROT12_SHIFT)
#define  ISTONE_ISROTCOEFF_ROT12_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTCOEFF_ROT12_MASK) | (((uint32_t)val) << ISTONE_ISROTCOEFF_ROT12_SHIFT))

#define  ISTONE_ISROTCOEFF_ROT11_MASK 0xff
#define  ISTONE_ISROTCOEFF_ROT11_SHIFT 0
#define  ISTONE_ISROTCOEFF_ROT11_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTCOEFF_ROT11_MASK) >> ISTONE_ISROTCOEFF_ROT11_SHIFT)
#define  ISTONE_ISROTCOEFF_ROT11_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTCOEFF_ROT11_MASK) | (((uint32_t)val) << ISTONE_ISROTCOEFF_ROT11_SHIFT))

//====================================================================
//Register: Rotation Offset Register (IsRotOffst)
/** \brief This register contains the 2 rotation matix offset values. These values must be written prior to enabling IsTone for proper operation.*/
//====================================================================

#define  ISTONE_ISROTOFFST_RESERVED1_MASK 0xfe000000
#define  ISTONE_ISROTOFFST_RESERVED1_SHIFT 25
#define  ISTONE_ISROTOFFST_RESERVED1_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTOFFST_RESERVED1_MASK) >> ISTONE_ISROTOFFST_RESERVED1_SHIFT)
#define  ISTONE_ISROTOFFST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTOFFST_RESERVED1_MASK) | (((uint32_t)val) << ISTONE_ISROTOFFST_RESERVED1_SHIFT))

#define  ISTONE_ISROTOFFST_DIFFY_MASK 0x1ff0000
#define  ISTONE_ISROTOFFST_DIFFY_SHIFT 16
#define  ISTONE_ISROTOFFST_DIFFY_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTOFFST_DIFFY_MASK) >> ISTONE_ISROTOFFST_DIFFY_SHIFT)
#define  ISTONE_ISROTOFFST_DIFFY_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTOFFST_DIFFY_MASK) | (((uint32_t)val) << ISTONE_ISROTOFFST_DIFFY_SHIFT))

#define  ISTONE_ISROTOFFST_RESERVED2_MASK 0xfe00
#define  ISTONE_ISROTOFFST_RESERVED2_SHIFT 9
#define  ISTONE_ISROTOFFST_RESERVED2_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTOFFST_RESERVED2_MASK) >> ISTONE_ISROTOFFST_RESERVED2_SHIFT)
#define  ISTONE_ISROTOFFST_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTOFFST_RESERVED2_MASK) | (((uint32_t)val) << ISTONE_ISROTOFFST_RESERVED2_SHIFT))

#define  ISTONE_ISROTOFFST_DIFFX_MASK 0x1ff
#define  ISTONE_ISROTOFFST_DIFFX_SHIFT 0
#define  ISTONE_ISROTOFFST_DIFFX_MASK_SHIFT(reg) (((reg) & ISTONE_ISROTOFFST_DIFFX_MASK) >> ISTONE_ISROTOFFST_DIFFX_SHIFT)
#define  ISTONE_ISROTOFFST_DIFFX_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISROTOFFST_DIFFX_MASK) | (((uint32_t)val) << ISTONE_ISROTOFFST_DIFFX_SHIFT))

//====================================================================
//Register: Chrominance Pixel Offset Register (IsPxlOffst)
/** \brief This register contains the chrominance offset values to be used in the initial stage of the tone detection calculations.
These values must be written prior to enabling IsTone for proper operation.*/
//====================================================================

#define  ISTONE_ISPXLOFFST_RESERVED1_MASK 0xffff0000
#define  ISTONE_ISPXLOFFST_RESERVED1_SHIFT 16
#define  ISTONE_ISPXLOFFST_RESERVED1_MASK_SHIFT(reg) (((reg) & ISTONE_ISPXLOFFST_RESERVED1_MASK) >> ISTONE_ISPXLOFFST_RESERVED1_SHIFT)
#define  ISTONE_ISPXLOFFST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISPXLOFFST_RESERVED1_MASK) | (((uint32_t)val) << ISTONE_ISPXLOFFST_RESERVED1_SHIFT))

#define  ISTONE_ISPXLOFFST_CRSHIFT_MASK 0xff00
#define  ISTONE_ISPXLOFFST_CRSHIFT_SHIFT 8
#define  ISTONE_ISPXLOFFST_CRSHIFT_MASK_SHIFT(reg) (((reg) & ISTONE_ISPXLOFFST_CRSHIFT_MASK) >> ISTONE_ISPXLOFFST_CRSHIFT_SHIFT)
#define  ISTONE_ISPXLOFFST_CRSHIFT_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISPXLOFFST_CRSHIFT_MASK) | (((uint32_t)val) << ISTONE_ISPXLOFFST_CRSHIFT_SHIFT))

#define  ISTONE_ISPXLOFFST_CBSHIFT_MASK 0xff
#define  ISTONE_ISPXLOFFST_CBSHIFT_SHIFT 0
#define  ISTONE_ISPXLOFFST_CBSHIFT_MASK_SHIFT(reg) (((reg) & ISTONE_ISPXLOFFST_CBSHIFT_MASK) >> ISTONE_ISPXLOFFST_CBSHIFT_SHIFT)
#define  ISTONE_ISPXLOFFST_CBSHIFT_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISPXLOFFST_CBSHIFT_MASK) | (((uint32_t)val) << ISTONE_ISPXLOFFST_CBSHIFT_SHIFT))

//====================================================================
//Register: Luminance Shift Register (IsLumShift)
/** \brief This register contains offset values needed for the luminance compensation portion of the tone detection calculations.
This value must be written prior to enabling IsTone for proper operation.*/
//====================================================================

#define  ISTONE_ISLUMSHIFT_RESERVED1_MASK 0xffffff00
#define  ISTONE_ISLUMSHIFT_RESERVED1_SHIFT 8
#define  ISTONE_ISLUMSHIFT_RESERVED1_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMSHIFT_RESERVED1_MASK) >> ISTONE_ISLUMSHIFT_RESERVED1_SHIFT)
#define  ISTONE_ISLUMSHIFT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMSHIFT_RESERVED1_MASK) | (((uint32_t)val) << ISTONE_ISLUMSHIFT_RESERVED1_SHIFT))

#define  ISTONE_ISLUMSHIFT_YSHIFT_MASK 0xff
#define  ISTONE_ISLUMSHIFT_YSHIFT_SHIFT 0
#define  ISTONE_ISLUMSHIFT_YSHIFT_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMSHIFT_YSHIFT_MASK) >> ISTONE_ISLUMSHIFT_YSHIFT_SHIFT)
#define  ISTONE_ISLUMSHIFT_YSHIFT_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMSHIFT_YSHIFT_MASK) | (((uint32_t)val) << ISTONE_ISLUMSHIFT_YSHIFT_SHIFT))

//====================================================================
//Register: Luminance Compensation Register (IsLumComp)
/** \brief This register contains multiplier values needed for the luminance compensation portion of the tone detection calculations.
These values must be written prior to enabling IsTone for proper operation.*/
//====================================================================

#define  ISTONE_ISLUMCOMP_RESERVED1_MASK 0xfffffe00
#define  ISTONE_ISLUMCOMP_RESERVED1_SHIFT 9
#define  ISTONE_ISLUMCOMP_RESERVED1_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMCOMP_RESERVED1_MASK) >> ISTONE_ISLUMCOMP_RESERVED1_SHIFT)
#define  ISTONE_ISLUMCOMP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMCOMP_RESERVED1_MASK) | (((uint32_t)val) << ISTONE_ISLUMCOMP_RESERVED1_SHIFT))

#define  ISTONE_ISLUMCOMP_YSIGN_MASK 0x100
#define  ISTONE_ISLUMCOMP_YSIGN_SHIFT 8
#define  ISTONE_ISLUMCOMP_YSIGN_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMCOMP_YSIGN_MASK) >> ISTONE_ISLUMCOMP_YSIGN_SHIFT)
#define  ISTONE_ISLUMCOMP_YSIGN_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMCOMP_YSIGN_MASK) | (((uint32_t)val) << ISTONE_ISLUMCOMP_YSIGN_SHIFT))

#define  ISTONE_ISLUMCOMP_RESERVED2_MASK 0xe0
#define  ISTONE_ISLUMCOMP_RESERVED2_SHIFT 5
#define  ISTONE_ISLUMCOMP_RESERVED2_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMCOMP_RESERVED2_MASK) >> ISTONE_ISLUMCOMP_RESERVED2_SHIFT)
#define  ISTONE_ISLUMCOMP_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMCOMP_RESERVED2_MASK) | (((uint32_t)val) << ISTONE_ISLUMCOMP_RESERVED2_SHIFT))

#define  ISTONE_ISLUMCOMP_YDIV_MASK 0x10
#define  ISTONE_ISLUMCOMP_YDIV_SHIFT 4
#define  ISTONE_ISLUMCOMP_YDIV_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMCOMP_YDIV_MASK) >> ISTONE_ISLUMCOMP_YDIV_SHIFT)
#define  ISTONE_ISLUMCOMP_YDIV_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMCOMP_YDIV_MASK) | (((uint32_t)val) << ISTONE_ISLUMCOMP_YDIV_SHIFT))

#define  ISTONE_ISLUMCOMP_RESERVED3_MASK 0xc
#define  ISTONE_ISLUMCOMP_RESERVED3_SHIFT 2
#define  ISTONE_ISLUMCOMP_RESERVED3_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMCOMP_RESERVED3_MASK) >> ISTONE_ISLUMCOMP_RESERVED3_SHIFT)
#define  ISTONE_ISLUMCOMP_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMCOMP_RESERVED3_MASK) | (((uint32_t)val) << ISTONE_ISLUMCOMP_RESERVED3_SHIFT))

#define  ISTONE_ISLUMCOMP_YMULTDIV_MASK 0x3
#define  ISTONE_ISLUMCOMP_YMULTDIV_SHIFT 0
#define  ISTONE_ISLUMCOMP_YMULTDIV_MASK_SHIFT(reg) (((reg) & ISTONE_ISLUMCOMP_YMULTDIV_MASK) >> ISTONE_ISLUMCOMP_YMULTDIV_SHIFT)
#define  ISTONE_ISLUMCOMP_YMULTDIV_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISLUMCOMP_YMULTDIV_MASK) | (((uint32_t)val) << ISTONE_ISLUMCOMP_YMULTDIV_SHIFT))

//====================================================================
//Register: Chrominance Multiply / Divide Register (IsMultDiv)
/** \brief This register contains the chrominance multiply and divide values to be used in the tone detection calculations.
These values must be written prior to enabling IsTone for proper operation.*/
//====================================================================

#define  ISTONE_ISMULTDIV_DIVCR_MASK 0xf0000000
#define  ISTONE_ISMULTDIV_DIVCR_SHIFT 28
#define  ISTONE_ISMULTDIV_DIVCR_MASK_SHIFT(reg) (((reg) & ISTONE_ISMULTDIV_DIVCR_MASK) >> ISTONE_ISMULTDIV_DIVCR_SHIFT)
#define  ISTONE_ISMULTDIV_DIVCR_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISMULTDIV_DIVCR_MASK) | (((uint32_t)val) << ISTONE_ISMULTDIV_DIVCR_SHIFT))

#define  ISTONE_ISMULTDIV_RESERVED1_MASK 0xc000000
#define  ISTONE_ISMULTDIV_RESERVED1_SHIFT 26
#define  ISTONE_ISMULTDIV_RESERVED1_MASK_SHIFT(reg) (((reg) & ISTONE_ISMULTDIV_RESERVED1_MASK) >> ISTONE_ISMULTDIV_RESERVED1_SHIFT)
#define  ISTONE_ISMULTDIV_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISMULTDIV_RESERVED1_MASK) | (((uint32_t)val) << ISTONE_ISMULTDIV_RESERVED1_SHIFT))

#define  ISTONE_ISMULTDIV_MULTCR_MASK 0x3ff0000
#define  ISTONE_ISMULTDIV_MULTCR_SHIFT 16
#define  ISTONE_ISMULTDIV_MULTCR_MASK_SHIFT(reg) (((reg) & ISTONE_ISMULTDIV_MULTCR_MASK) >> ISTONE_ISMULTDIV_MULTCR_SHIFT)
#define  ISTONE_ISMULTDIV_MULTCR_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISMULTDIV_MULTCR_MASK) | (((uint32_t)val) << ISTONE_ISMULTDIV_MULTCR_SHIFT))

#define  ISTONE_ISMULTDIV_DIVCB_MASK 0xf000
#define  ISTONE_ISMULTDIV_DIVCB_SHIFT 12
#define  ISTONE_ISMULTDIV_DIVCB_MASK_SHIFT(reg) (((reg) & ISTONE_ISMULTDIV_DIVCB_MASK) >> ISTONE_ISMULTDIV_DIVCB_SHIFT)
#define  ISTONE_ISMULTDIV_DIVCB_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISMULTDIV_DIVCB_MASK) | (((uint32_t)val) << ISTONE_ISMULTDIV_DIVCB_SHIFT))

#define  ISTONE_ISMULTDIV_RESERVED2_MASK 0xc00
#define  ISTONE_ISMULTDIV_RESERVED2_SHIFT 10
#define  ISTONE_ISMULTDIV_RESERVED2_MASK_SHIFT(reg) (((reg) & ISTONE_ISMULTDIV_RESERVED2_MASK) >> ISTONE_ISMULTDIV_RESERVED2_SHIFT)
#define  ISTONE_ISMULTDIV_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISMULTDIV_RESERVED2_MASK) | (((uint32_t)val) << ISTONE_ISMULTDIV_RESERVED2_SHIFT))

#define  ISTONE_ISMULTDIV_MULTCB_MASK 0x3ff
#define  ISTONE_ISMULTDIV_MULTCB_SHIFT 0
#define  ISTONE_ISMULTDIV_MULTCB_MASK_SHIFT(reg) (((reg) & ISTONE_ISMULTDIV_MULTCB_MASK) >> ISTONE_ISMULTDIV_MULTCB_SHIFT)
#define  ISTONE_ISMULTDIV_MULTCB_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISMULTDIV_MULTCB_MASK) | (((uint32_t)val) << ISTONE_ISMULTDIV_MULTCB_SHIFT))

//====================================================================
//Register: Test Value Threshold Register (IsThresh)
/** \brief This register contains the final comparison value used in the tone detection calculations to determine if the pixel matches the desired tone.
This value must be written prior to enabling IsTone for proper operation.*/
//====================================================================

#define  ISTONE_ISTHRESH_TESTVALUE_MASK 0xffffffff
#define  ISTONE_ISTHRESH_TESTVALUE_SHIFT 0
#define  ISTONE_ISTHRESH_TESTVALUE_MASK_SHIFT(reg) (((reg) & ISTONE_ISTHRESH_TESTVALUE_MASK) >> ISTONE_ISTHRESH_TESTVALUE_SHIFT)
#define  ISTONE_ISTHRESH_TESTVALUE_REPLACE_VAL(reg,val) (((reg) & ~ISTONE_ISTHRESH_TESTVALUE_MASK) | (((uint32_t)val) << ISTONE_ISTHRESH_TESTVALUE_SHIFT))

#endif // ISTONE
