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
 * \file XYCScale_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _XYCSCALE_REGMASKS_H_
#define _XYCSCALE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: XYCScale (XYCScale)
//
//====================================================================

//====================================================================
//Register: Configure X-Y Scale Register (CSR)
//====================================================================

#define  XYCSCALE_CSR_RESERVED1_MASK 0xfffffff0
#define  XYCSCALE_CSR_RESERVED1_SHIFT 4
#define  XYCSCALE_CSR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CSR_RESERVED1_MASK) >> XYCSCALE_CSR_RESERVED1_SHIFT)
#define  XYCSCALE_CSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CSR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_CSR_RESERVED1_SHIFT))

#define  XYCSCALE_CSR_S_MASK 0x8
#define  XYCSCALE_CSR_S_SHIFT 3
#define  XYCSCALE_CSR_S_MASK_SHIFT(reg) (((reg) & XYCSCALE_CSR_S_MASK) >> XYCSCALE_CSR_S_SHIFT)
#define  XYCSCALE_CSR_S_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CSR_S_MASK) | (((uint32_t)val) << XYCSCALE_CSR_S_SHIFT))

#define  XYCSCALE_CSR_M_MASK 0x4
#define  XYCSCALE_CSR_M_SHIFT 2
#define  XYCSCALE_CSR_M_MASK_SHIFT(reg) (((reg) & XYCSCALE_CSR_M_MASK) >> XYCSCALE_CSR_M_SHIFT)
#define  XYCSCALE_CSR_M_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CSR_M_MASK) | (((uint32_t)val) << XYCSCALE_CSR_M_SHIFT))

#define  XYCSCALE_CSR_O_MASK 0x2
#define  XYCSCALE_CSR_O_SHIFT 1
#define  XYCSCALE_CSR_O_MASK_SHIFT(reg) (((reg) & XYCSCALE_CSR_O_MASK) >> XYCSCALE_CSR_O_SHIFT)
#define  XYCSCALE_CSR_O_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CSR_O_MASK) | (((uint32_t)val) << XYCSCALE_CSR_O_SHIFT))

#define  XYCSCALE_CSR_B_MASK 0x1
#define  XYCSCALE_CSR_B_SHIFT 0
#define  XYCSCALE_CSR_B_MASK_SHIFT(reg) (((reg) & XYCSCALE_CSR_B_MASK) >> XYCSCALE_CSR_B_SHIFT)
#define  XYCSCALE_CSR_B_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CSR_B_MASK) | (((uint32_t)val) << XYCSCALE_CSR_B_SHIFT))

//====================================================================
//Register: Input Strip Column and Line Count Register  (CLCR)
/** \brief The Input Strip Column Count may also be thought of as the number of x-direction pixels in a given input line. The Input Strip Line Count may also be thought of as the number of y-direction pixels in a given input column. The Line Count is not dependent upon the scale whether reducing or enlarging.
When reducing, the maximum count may not exceed the capabilities of the preceding ICE blocks, this is set to 17 lines. A Bi-Cubic reducing scale range near or equal to one takes 17 input lines and creates 14 output lines. This Y-Scale output is stored in fourteen intermediate registers as the X-Scale traverses column to column.
When enlarging, the maximum count may not exceed the floor of the Maximum Lines per Strip (L) divided by the enlarging Y scale (Sy). For example with L = 16 and if the scale is 7x (700%) the maximum Line Count is the floor [16 / 7] = floor [2.286] = 2. Two lines may seem limiting but, at a scale 7x in both directions, each input pixel will generate 49 output pixels.
               Once written, this register will stick through all subsequent strips until changed.*/
//====================================================================

#define  XYCSCALE_CLCR_RESERVED1_MASK 0xe0000000
#define  XYCSCALE_CLCR_RESERVED1_SHIFT 29
#define  XYCSCALE_CLCR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CLCR_RESERVED1_MASK) >> XYCSCALE_CLCR_RESERVED1_SHIFT)
#define  XYCSCALE_CLCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CLCR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_CLCR_RESERVED1_SHIFT))

#define  XYCSCALE_CLCR_LC_MASK 0x1f000000
#define  XYCSCALE_CLCR_LC_SHIFT 24
#define  XYCSCALE_CLCR_LC_MASK_SHIFT(reg) (((reg) & XYCSCALE_CLCR_LC_MASK) >> XYCSCALE_CLCR_LC_SHIFT)
#define  XYCSCALE_CLCR_LC_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CLCR_LC_MASK) | (((uint32_t)val) << XYCSCALE_CLCR_LC_SHIFT))

#define  XYCSCALE_CLCR_RESERVED2_MASK 0xfc0000
#define  XYCSCALE_CLCR_RESERVED2_SHIFT 18
#define  XYCSCALE_CLCR_RESERVED2_MASK_SHIFT(reg) (((reg) & XYCSCALE_CLCR_RESERVED2_MASK) >> XYCSCALE_CLCR_RESERVED2_SHIFT)
#define  XYCSCALE_CLCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CLCR_RESERVED2_MASK) | (((uint32_t)val) << XYCSCALE_CLCR_RESERVED2_SHIFT))

#define  XYCSCALE_CLCR_CC_MASK 0x3ffff
#define  XYCSCALE_CLCR_CC_SHIFT 0
#define  XYCSCALE_CLCR_CC_MASK_SHIFT(reg) (((reg) & XYCSCALE_CLCR_CC_MASK) >> XYCSCALE_CLCR_CC_SHIFT)
#define  XYCSCALE_CLCR_CC_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CLCR_CC_MASK) | (((uint32_t)val) << XYCSCALE_CLCR_CC_SHIFT))

//====================================================================
//Register: Output Strip Column Count Register (CCR)
/** \brief The Output Column Count may also be thought of as the number of x-direction pixels in a given output line. This value may exceed the scaled output count of the input pixels in which case data padding will be used to make up the difference. In the case where this value is less than the scaled output count the output is cut short to this new count. Data padding will either use individual fill data as setup in the color X fill data registers below, or if the Replicate Mode bit is set, will use the last column data values to pad with.
This replaces a pad count that extends the data size to some optimal DMA sizing, and adds the feature of being able to cut short output data.
               Once written, this register will stick through all subsequent strips until changed.*/
//====================================================================

#define  XYCSCALE_CCR_RESERVED1_MASK 0xfe000000
#define  XYCSCALE_CCR_RESERVED1_SHIFT 25
#define  XYCSCALE_CCR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CCR_RESERVED1_MASK) >> XYCSCALE_CCR_RESERVED1_SHIFT)
#define  XYCSCALE_CCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CCR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_CCR_RESERVED1_SHIFT))

#define  XYCSCALE_CCR_RM_MASK 0x1000000
#define  XYCSCALE_CCR_RM_SHIFT 24
#define  XYCSCALE_CCR_RM_MASK_SHIFT(reg) (((reg) & XYCSCALE_CCR_RM_MASK) >> XYCSCALE_CCR_RM_SHIFT)
#define  XYCSCALE_CCR_RM_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CCR_RM_MASK) | (((uint32_t)val) << XYCSCALE_CCR_RM_SHIFT))

#define  XYCSCALE_CCR_RESERVED2_MASK 0xfc0000
#define  XYCSCALE_CCR_RESERVED2_SHIFT 18
#define  XYCSCALE_CCR_RESERVED2_MASK_SHIFT(reg) (((reg) & XYCSCALE_CCR_RESERVED2_MASK) >> XYCSCALE_CCR_RESERVED2_SHIFT)
#define  XYCSCALE_CCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CCR_RESERVED2_MASK) | (((uint32_t)val) << XYCSCALE_CCR_RESERVED2_SHIFT))

#define  XYCSCALE_CCR_CC_MASK 0x3ffff
#define  XYCSCALE_CCR_CC_SHIFT 0
#define  XYCSCALE_CCR_CC_MASK_SHIFT(reg) (((reg) & XYCSCALE_CCR_CC_MASK) >> XYCSCALE_CCR_CC_SHIFT)
#define  XYCSCALE_CCR_CC_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CCR_CC_MASK) | (((uint32_t)val) << XYCSCALE_CCR_CC_SHIFT))

//====================================================================
//Register: Color 2 and Color 1 Fill Data Register (CFR1)
//====================================================================

#define  XYCSCALE_CFR1_RESERVED1_MASK 0xff000000
#define  XYCSCALE_CFR1_RESERVED1_SHIFT 24
#define  XYCSCALE_CFR1_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CFR1_RESERVED1_MASK) >> XYCSCALE_CFR1_RESERVED1_SHIFT)
#define  XYCSCALE_CFR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CFR1_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_CFR1_RESERVED1_SHIFT))

#define  XYCSCALE_CFR1_FILL2_MASK 0xff0000
#define  XYCSCALE_CFR1_FILL2_SHIFT 16
#define  XYCSCALE_CFR1_FILL2_MASK_SHIFT(reg) (((reg) & XYCSCALE_CFR1_FILL2_MASK) >> XYCSCALE_CFR1_FILL2_SHIFT)
#define  XYCSCALE_CFR1_FILL2_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CFR1_FILL2_MASK) | (((uint32_t)val) << XYCSCALE_CFR1_FILL2_SHIFT))

#define  XYCSCALE_CFR1_RESERVED2_MASK 0xff00
#define  XYCSCALE_CFR1_RESERVED2_SHIFT 8
#define  XYCSCALE_CFR1_RESERVED2_MASK_SHIFT(reg) (((reg) & XYCSCALE_CFR1_RESERVED2_MASK) >> XYCSCALE_CFR1_RESERVED2_SHIFT)
#define  XYCSCALE_CFR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CFR1_RESERVED2_MASK) | (((uint32_t)val) << XYCSCALE_CFR1_RESERVED2_SHIFT))

#define  XYCSCALE_CFR1_FILL1_MASK 0xff
#define  XYCSCALE_CFR1_FILL1_SHIFT 0
#define  XYCSCALE_CFR1_FILL1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CFR1_FILL1_MASK) >> XYCSCALE_CFR1_FILL1_SHIFT)
#define  XYCSCALE_CFR1_FILL1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CFR1_FILL1_MASK) | (((uint32_t)val) << XYCSCALE_CFR1_FILL1_SHIFT))

//====================================================================
//Register: Color 0 Fill Data Register (CFR2)
//====================================================================

#define  XYCSCALE_CFR2_RESERVED1_MASK 0xffffff00
#define  XYCSCALE_CFR2_RESERVED1_SHIFT 8
#define  XYCSCALE_CFR2_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CFR2_RESERVED1_MASK) >> XYCSCALE_CFR2_RESERVED1_SHIFT)
#define  XYCSCALE_CFR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CFR2_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_CFR2_RESERVED1_SHIFT))

#define  XYCSCALE_CFR2_FILL0_MASK 0xff
#define  XYCSCALE_CFR2_FILL0_SHIFT 0
#define  XYCSCALE_CFR2_FILL0_MASK_SHIFT(reg) (((reg) & XYCSCALE_CFR2_FILL0_MASK) >> XYCSCALE_CFR2_FILL0_SHIFT)
#define  XYCSCALE_CFR2_FILL0_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CFR2_FILL0_MASK) | (((uint32_t)val) << XYCSCALE_CFR2_FILL0_SHIFT))

//====================================================================
//Register: Horizontal Scale Register (HSR)
/** \brief Scale factor x-direction
The following table is for reference, loading the scale, transpose, edge count, and scale - normal.
<table>
<th>Hex Program Values</th><th>Mode:</th></th><th colspan=4>Bi-Cubic Mode</th></tr>
<tr><td> </td><td>Register:</td></td><td>(scaleX/Y)</td></td></tr>
<tr><td> </td><td>Bits:</td></td><td>S: Scale Factor</td></td></td></tr>
<tr><td>Scale</td><td>Percentage</td></td><td>Hex value</td></td></tr>
<tr><td>1x</td><td>100%</td></td><td>1_0000h</td></td></tr>
<tr><td> </td><td>100.00153%</td></td><td>0_FFFFh</td></td></tr>
<tr><td>1.01x</td><td>101%</td></td><td>0_FD77h</td></td></td></tr>
<tr><td>1.5x</td><td>150%</td></td><td>0_AAAAh</td></td></td></tr>
<tr><td>2x</td><td>200%</td></td><td>0_8000h</td></td></td></tr>
<tr><td>3x</td><td>300%</td></td><td>0_5555h</td></td></td></tr>
<tr><td>4x</td><td>400%</td></td><td>0_4000h</td></td></td></tr>
<tr><td></td><td>99%</td></td><td>1_0295h</td></td></td></tr>
<tr><td></td><td>99.99847%</td></td><td>1_1001h</td></td></td></tr>
<tr><td>.75x</td><td></td></td><td>1_5555h</td></td></td></tr>
<tr><td>.5x</td><td>50%</td></td><td>2_0000h</td></td></td></tr>
<tr><td>.25x</td><td>25%</td></td><td>N/A</td></td></td></tr>
</table>
               <p>Once written, this register will stick through all subsequent strips until changed.</p>*/
//====================================================================

#define  XYCSCALE_HSR_RESERVED1_MASK 0xfff00000
#define  XYCSCALE_HSR_RESERVED1_SHIFT 20
#define  XYCSCALE_HSR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_HSR_RESERVED1_MASK) >> XYCSCALE_HSR_RESERVED1_SHIFT)
#define  XYCSCALE_HSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_HSR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_HSR_RESERVED1_SHIFT))

#define  XYCSCALE_HSR_S_MASK 0xfffff
#define  XYCSCALE_HSR_S_SHIFT 0
#define  XYCSCALE_HSR_S_MASK_SHIFT(reg) (((reg) & XYCSCALE_HSR_S_MASK) >> XYCSCALE_HSR_S_SHIFT)
#define  XYCSCALE_HSR_S_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_HSR_S_MASK) | (((uint32_t)val) << XYCSCALE_HSR_S_SHIFT))

//====================================================================
//Register: Vertical Scale Register (VSR)
/** \brief Scale factor y-direction
              Firmware is required to pad the top and bottom of page with extra bits when enlarging an image.*/
//====================================================================

#define  XYCSCALE_VSR_RESERVED1_MASK 0xfff00000
#define  XYCSCALE_VSR_RESERVED1_SHIFT 20
#define  XYCSCALE_VSR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_VSR_RESERVED1_MASK) >> XYCSCALE_VSR_RESERVED1_SHIFT)
#define  XYCSCALE_VSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_VSR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_VSR_RESERVED1_SHIFT))

#define  XYCSCALE_VSR_S_MASK 0xfffff
#define  XYCSCALE_VSR_S_SHIFT 0
#define  XYCSCALE_VSR_S_MASK_SHIFT(reg) (((reg) & XYCSCALE_VSR_S_MASK) >> XYCSCALE_VSR_S_SHIFT)
#define  XYCSCALE_VSR_S_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_VSR_S_MASK) | (((uint32_t)val) << XYCSCALE_VSR_S_SHIFT))

//====================================================================
//Register: X-Transpose Configuration Register (XTR)
/** \brief This register in conjunction with the Horizontal Scale register stores configuration information beyond scale factors that the Scale block uses.
               Within the block, the Dx is reloaded after the column count is met.*/
//====================================================================

#define  XYCSCALE_XTR_RESERVED1_MASK 0xfff00000
#define  XYCSCALE_XTR_RESERVED1_SHIFT 20
#define  XYCSCALE_XTR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_XTR_RESERVED1_MASK) >> XYCSCALE_XTR_RESERVED1_SHIFT)
#define  XYCSCALE_XTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_XTR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_XTR_RESERVED1_SHIFT))

#define  XYCSCALE_XTR_DX_MASK 0xfffff
#define  XYCSCALE_XTR_DX_SHIFT 0
#define  XYCSCALE_XTR_DX_MASK_SHIFT(reg) (((reg) & XYCSCALE_XTR_DX_MASK) >> XYCSCALE_XTR_DX_SHIFT)
#define  XYCSCALE_XTR_DX_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_XTR_DX_MASK) | (((uint32_t)val) << XYCSCALE_XTR_DX_SHIFT))

//====================================================================
//Register: Y-Transpose Configuration Register (YTR)
/** \brief This register in conjunction with the Vertical Scale register stores configuration information beyond scale factors that the Scale block uses.
               Typically, Dy is loaded only once for top of page. The transpose in the vertical direction is maintained by the Current Y-Start register. The Current Y-Start register can be saved and re-written into this register to stop current image, write this register start & finish a different image strip, and then resume.*/
//====================================================================

#define  XYCSCALE_YTR_RESERVED1_MASK 0xfff00000
#define  XYCSCALE_YTR_RESERVED1_SHIFT 20
#define  XYCSCALE_YTR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_YTR_RESERVED1_MASK) >> XYCSCALE_YTR_RESERVED1_SHIFT)
#define  XYCSCALE_YTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_YTR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_YTR_RESERVED1_SHIFT))

#define  XYCSCALE_YTR_DY_MASK 0xfffff
#define  XYCSCALE_YTR_DY_SHIFT 0
#define  XYCSCALE_YTR_DY_MASK_SHIFT(reg) (((reg) & XYCSCALE_YTR_DY_MASK) >> XYCSCALE_YTR_DY_SHIFT)
#define  XYCSCALE_YTR_DY_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_YTR_DY_MASK) | (((uint32_t)val) << XYCSCALE_YTR_DY_SHIFT))

//====================================================================
//Register: Current X-Start Register (CXR)
/** \brief This register allows read access of the current internal start Dx register. The initial value comes from the X-Transpose Register and should be read back as such during page initialization.*/
//====================================================================

#define  XYCSCALE_CXR_RESERVED1_MASK 0xfff00000
#define  XYCSCALE_CXR_RESERVED1_SHIFT 20
#define  XYCSCALE_CXR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CXR_RESERVED1_MASK) >> XYCSCALE_CXR_RESERVED1_SHIFT)
#define  XYCSCALE_CXR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CXR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_CXR_RESERVED1_SHIFT))

#define  XYCSCALE_CXR_DX_MASK 0xfffff
#define  XYCSCALE_CXR_DX_SHIFT 0
#define  XYCSCALE_CXR_DX_MASK_SHIFT(reg) (((reg) & XYCSCALE_CXR_DX_MASK) >> XYCSCALE_CXR_DX_SHIFT)
#define  XYCSCALE_CXR_DX_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CXR_DX_MASK) | (((uint32_t)val) << XYCSCALE_CXR_DX_SHIFT))

//====================================================================
//Register: Current Y-Start Register (CYR)
/** \brief This register in allows read access of the current internal start Dy register. The initial value comes from the Y-Transpose Register and should be read back as such during page initialization.*/
//====================================================================

#define  XYCSCALE_CYR_RESERVED1_MASK 0xfff00000
#define  XYCSCALE_CYR_RESERVED1_SHIFT 20
#define  XYCSCALE_CYR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYCSCALE_CYR_RESERVED1_MASK) >> XYCSCALE_CYR_RESERVED1_SHIFT)
#define  XYCSCALE_CYR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CYR_RESERVED1_MASK) | (((uint32_t)val) << XYCSCALE_CYR_RESERVED1_SHIFT))

#define  XYCSCALE_CYR_DY_MASK 0xfffff
#define  XYCSCALE_CYR_DY_SHIFT 0
#define  XYCSCALE_CYR_DY_MASK_SHIFT(reg) (((reg) & XYCSCALE_CYR_DY_MASK) >> XYCSCALE_CYR_DY_SHIFT)
#define  XYCSCALE_CYR_DY_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_CYR_DY_MASK) | (((uint32_t)val) << XYCSCALE_CYR_DY_SHIFT))

//====================================================================
//Register: Observation Register (XOR)
/** \brief This read only register is for observation of certain XYCScale internals.*/
//====================================================================

#define  XYCSCALE_XOR_DEBUG_MASK 0xffffffff
#define  XYCSCALE_XOR_DEBUG_SHIFT 0
#define  XYCSCALE_XOR_DEBUG_MASK_SHIFT(reg) (((reg) & XYCSCALE_XOR_DEBUG_MASK) >> XYCSCALE_XOR_DEBUG_SHIFT)
#define  XYCSCALE_XOR_DEBUG_REPLACE_VAL(reg,val) (((reg) & ~XYCSCALE_XOR_DEBUG_MASK) | (((uint32_t)val) << XYCSCALE_XOR_DEBUG_SHIFT))

#endif // XYCSCALE
