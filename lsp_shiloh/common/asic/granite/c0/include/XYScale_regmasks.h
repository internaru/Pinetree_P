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
 * \file XYScale_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _XYSCALE_REGMASKS_H_
#define _XYSCALE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: XYScale (XYScale)
/** \brief Register Descriptions for the PIE XYScale block*/
//
//====================================================================

//====================================================================
//Register: Configure X-Y Scale Register (CSR)
/** \brief Bit [8] Switches Algorithms between Running Average Pixel Replication (reset to zero) and Linear Mode (set to one).
Configurable round up most recently added, allows tweaking of the algorithm to round up instead of truncate for a potentially closer desired output. The round-up bits only work in Running Average Pixel Replication Mode.
Bit [16] adds in 0.5 when starting new accumulation of a column. The other two control bits [18:17] make the accumulators reset to 0.5 instead of zero. To correctly initialize the accumulators with round up values of 0.5, bits 17 & 18 must be set to one prior to the Bypass being turned off.
Restating with explanation: Bypass is ORed into the synchronous reset, and the synchronous reset must be active for one clock in order for any reset value to take hold. Therefore, when using round up bits 17 & 18, two sequential writes must be written to the Configuration Register, one with the bits set and bypass bit [0] set as well, the second with bypass reset to zero to start functional operation.*/
//====================================================================

#define  XYSCALE_CSR_RESERVED1_MASK 0xfff80000
#define  XYSCALE_CSR_RESERVED1_SHIFT 19
#define  XYSCALE_CSR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_CSR_RESERVED1_MASK) >> XYSCALE_CSR_RESERVED1_SHIFT)
#define  XYSCALE_CSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CSR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_CSR_RESERVED1_SHIFT))

#define  XYSCALE_CSR_RU_MASK 0x70000
#define  XYSCALE_CSR_RU_SHIFT 16
#define  XYSCALE_CSR_RU_MASK_SHIFT(reg) (((reg) & XYSCALE_CSR_RU_MASK) >> XYSCALE_CSR_RU_SHIFT)
#define  XYSCALE_CSR_RU_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CSR_RU_MASK) | (((uint32_t)val) << XYSCALE_CSR_RU_SHIFT))

#define  XYSCALE_CSR_RESERVED2_MASK 0xfe00
#define  XYSCALE_CSR_RESERVED2_SHIFT 9
#define  XYSCALE_CSR_RESERVED2_MASK_SHIFT(reg) (((reg) & XYSCALE_CSR_RESERVED2_MASK) >> XYSCALE_CSR_RESERVED2_SHIFT)
#define  XYSCALE_CSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CSR_RESERVED2_MASK) | (((uint32_t)val) << XYSCALE_CSR_RESERVED2_SHIFT))

#define  XYSCALE_CSR_LM_MASK 0x100
#define  XYSCALE_CSR_LM_SHIFT 8
#define  XYSCALE_CSR_LM_MASK_SHIFT(reg) (((reg) & XYSCALE_CSR_LM_MASK) >> XYSCALE_CSR_LM_SHIFT)
#define  XYSCALE_CSR_LM_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CSR_LM_MASK) | (((uint32_t)val) << XYSCALE_CSR_LM_SHIFT))

#define  XYSCALE_CSR_RESERVED3_MASK 0xfe
#define  XYSCALE_CSR_RESERVED3_SHIFT 1
#define  XYSCALE_CSR_RESERVED3_MASK_SHIFT(reg) (((reg) & XYSCALE_CSR_RESERVED3_MASK) >> XYSCALE_CSR_RESERVED3_SHIFT)
#define  XYSCALE_CSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CSR_RESERVED3_MASK) | (((uint32_t)val) << XYSCALE_CSR_RESERVED3_SHIFT))

#define  XYSCALE_CSR_B_MASK 0x1
#define  XYSCALE_CSR_B_SHIFT 0
#define  XYSCALE_CSR_B_MASK_SHIFT(reg) (((reg) & XYSCALE_CSR_B_MASK) >> XYSCALE_CSR_B_SHIFT)
#define  XYSCALE_CSR_B_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CSR_B_MASK) | (((uint32_t)val) << XYSCALE_CSR_B_SHIFT))

//====================================================================
//Register: Input Strip Column and Line Count Register (CLCR)
/** \brief The Input Strip Column Count may also be thought of as the number of x-direction pixels in a given input line. The Input Strip Line Count may also be thought of as the number of y-direction pixels in a given input column. The Line Count is dependent upon the scale whether reducing or enlarging.
When reducing, the maximum count may not exceed the capabilities of the preceding ICE blocks, this is set to 17 lines. A Bi-Linear mode reducing scale range near or equal to one takes 17 input lines and creates 16 output lines. This Y-Scale output is stored in sixteen intermediate registers as the X-Scale traverses column to column.
When enlarging, the maximum count may not exceed the floor of the Maximum Lines per Strip (L) divided by the enlarging Y scale (Sy). For example with L = 16 and if the scale is 7x (700%) the maximum Line Count is the floor [16 / 7] = floor [2.286] = 2. Two lines may seem limiting but, at a scale 7x in both directions, each input pixel will generate 49 output pixels.
Once written, this register will stick through all subsequent strips until changed.*/
//====================================================================

#define  XYSCALE_CLCR_RESERVED1_MASK 0xe0000000
#define  XYSCALE_CLCR_RESERVED1_SHIFT 29
#define  XYSCALE_CLCR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_CLCR_RESERVED1_MASK) >> XYSCALE_CLCR_RESERVED1_SHIFT)
#define  XYSCALE_CLCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CLCR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_CLCR_RESERVED1_SHIFT))

#define  XYSCALE_CLCR_LC_MASK 0x1f000000
#define  XYSCALE_CLCR_LC_SHIFT 24
#define  XYSCALE_CLCR_LC_MASK_SHIFT(reg) (((reg) & XYSCALE_CLCR_LC_MASK) >> XYSCALE_CLCR_LC_SHIFT)
#define  XYSCALE_CLCR_LC_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CLCR_LC_MASK) | (((uint32_t)val) << XYSCALE_CLCR_LC_SHIFT))

#define  XYSCALE_CLCR_RESERVED2_MASK 0xfc0000
#define  XYSCALE_CLCR_RESERVED2_SHIFT 18
#define  XYSCALE_CLCR_RESERVED2_MASK_SHIFT(reg) (((reg) & XYSCALE_CLCR_RESERVED2_MASK) >> XYSCALE_CLCR_RESERVED2_SHIFT)
#define  XYSCALE_CLCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CLCR_RESERVED2_MASK) | (((uint32_t)val) << XYSCALE_CLCR_RESERVED2_SHIFT))

#define  XYSCALE_CLCR_CC_MASK 0x3ffff
#define  XYSCALE_CLCR_CC_SHIFT 0
#define  XYSCALE_CLCR_CC_MASK_SHIFT(reg) (((reg) & XYSCALE_CLCR_CC_MASK) >> XYSCALE_CLCR_CC_SHIFT)
#define  XYSCALE_CLCR_CC_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CLCR_CC_MASK) | (((uint32_t)val) << XYSCALE_CLCR_CC_SHIFT))

//====================================================================
//Register: Output Strip Column Count Register (CCR)
/** \brief The Output Column Count may also be thought of as the number of x-direction pixels in a given output line. This value may exceed the scaled output count of the input pixels in which case data padding will be used to make up the difference. In the case where this value is less than the scaled output count the output is cut short to this new count. Data padding will either use individual fill data as setup in the color X fill data registers below, or if the Replicate Mode bit is set, will use the last column data values to pad with.
This replaces a pad count that extends the data size to some optimal DMA sizing, and adds the feature of being able to cut short output data.
In cases of Mono with 1, 2 or 4-bit data, calculation of the padding to obtain 16-byte boundaries is as follows:
a.) native output count / (8-bits / 1, 2 or 4-bit mono data) = output byte count
b.) 16 - output byte count MOD 16 = pad byte count
c.) pad byte count * (8-bits / 1, 2 or 4-bit mono data) = pad count
d.) new column count = native output count + pad count
Once written, this register will stick through all subsequent strips until changed.*/
//====================================================================

#define  XYSCALE_CCR_RESERVED1_MASK 0xfe000000
#define  XYSCALE_CCR_RESERVED1_SHIFT 25
#define  XYSCALE_CCR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_CCR_RESERVED1_MASK) >> XYSCALE_CCR_RESERVED1_SHIFT)
#define  XYSCALE_CCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CCR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_CCR_RESERVED1_SHIFT))

#define  XYSCALE_CCR_RM_MASK 0x1000000
#define  XYSCALE_CCR_RM_SHIFT 24
#define  XYSCALE_CCR_RM_MASK_SHIFT(reg) (((reg) & XYSCALE_CCR_RM_MASK) >> XYSCALE_CCR_RM_SHIFT)
#define  XYSCALE_CCR_RM_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CCR_RM_MASK) | (((uint32_t)val) << XYSCALE_CCR_RM_SHIFT))

#define  XYSCALE_CCR_RESERVED2_MASK 0xfc0000
#define  XYSCALE_CCR_RESERVED2_SHIFT 18
#define  XYSCALE_CCR_RESERVED2_MASK_SHIFT(reg) (((reg) & XYSCALE_CCR_RESERVED2_MASK) >> XYSCALE_CCR_RESERVED2_SHIFT)
#define  XYSCALE_CCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CCR_RESERVED2_MASK) | (((uint32_t)val) << XYSCALE_CCR_RESERVED2_SHIFT))

#define  XYSCALE_CCR_CC_MASK 0x3ffff
#define  XYSCALE_CCR_CC_SHIFT 0
#define  XYSCALE_CCR_CC_MASK_SHIFT(reg) (((reg) & XYSCALE_CCR_CC_MASK) >> XYSCALE_CCR_CC_SHIFT)
#define  XYSCALE_CCR_CC_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CCR_CC_MASK) | (((uint32_t)val) << XYSCALE_CCR_CC_SHIFT))

//====================================================================
//Register: Color 2 and Color 1 Fill Data Register (CFR1)
//====================================================================

#define  XYSCALE_CFR1_RESERVED1_MASK 0xff000000
#define  XYSCALE_CFR1_RESERVED1_SHIFT 24
#define  XYSCALE_CFR1_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_CFR1_RESERVED1_MASK) >> XYSCALE_CFR1_RESERVED1_SHIFT)
#define  XYSCALE_CFR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CFR1_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_CFR1_RESERVED1_SHIFT))

#define  XYSCALE_CFR1_FILL2_MASK 0xff0000
#define  XYSCALE_CFR1_FILL2_SHIFT 16
#define  XYSCALE_CFR1_FILL2_MASK_SHIFT(reg) (((reg) & XYSCALE_CFR1_FILL2_MASK) >> XYSCALE_CFR1_FILL2_SHIFT)
#define  XYSCALE_CFR1_FILL2_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CFR1_FILL2_MASK) | (((uint32_t)val) << XYSCALE_CFR1_FILL2_SHIFT))

#define  XYSCALE_CFR1_RESERVED2_MASK 0xff00
#define  XYSCALE_CFR1_RESERVED2_SHIFT 8
#define  XYSCALE_CFR1_RESERVED2_MASK_SHIFT(reg) (((reg) & XYSCALE_CFR1_RESERVED2_MASK) >> XYSCALE_CFR1_RESERVED2_SHIFT)
#define  XYSCALE_CFR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CFR1_RESERVED2_MASK) | (((uint32_t)val) << XYSCALE_CFR1_RESERVED2_SHIFT))

#define  XYSCALE_CFR1_FILL1_MASK 0xff
#define  XYSCALE_CFR1_FILL1_SHIFT 0
#define  XYSCALE_CFR1_FILL1_MASK_SHIFT(reg) (((reg) & XYSCALE_CFR1_FILL1_MASK) >> XYSCALE_CFR1_FILL1_SHIFT)
#define  XYSCALE_CFR1_FILL1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CFR1_FILL1_MASK) | (((uint32_t)val) << XYSCALE_CFR1_FILL1_SHIFT))

//====================================================================
//Register: Color 0 Fill Data Register (CFR2)
//====================================================================

#define  XYSCALE_CFR2_RESERVED1_MASK 0xffffff00
#define  XYSCALE_CFR2_RESERVED1_SHIFT 8
#define  XYSCALE_CFR2_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_CFR2_RESERVED1_MASK) >> XYSCALE_CFR2_RESERVED1_SHIFT)
#define  XYSCALE_CFR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CFR2_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_CFR2_RESERVED1_SHIFT))

#define  XYSCALE_CFR2_FILL0_MASK 0xff
#define  XYSCALE_CFR2_FILL0_SHIFT 0
#define  XYSCALE_CFR2_FILL0_MASK_SHIFT(reg) (((reg) & XYSCALE_CFR2_FILL0_MASK) >> XYSCALE_CFR2_FILL0_SHIFT)
#define  XYSCALE_CFR2_FILL0_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CFR2_FILL0_MASK) | (((uint32_t)val) << XYSCALE_CFR2_FILL0_SHIFT))

//====================================================================
//Register: Horizontal Scale Register (HSR)
/** \brief Scale factor x-direction
The following table is for reference, loading the scale, transpose, edge count, and scale - normal.
<table>
<th>Hex Program Values</th><th>Mode:</th><th>Running Average Pixel Replication Mode</th><th colspan=4>Bi-Linear Mode</th></tr>
<tr><td> </td><td>Register:</td><td>(scaleX/Y)</td><td>(scaleX/Y)</td><td>(scaleX/Y)</td><td>(scaleX/YsubN)</td><td>(transposeX/Y)</td></tr>
<tr><td> </td><td>Bits:</td><td>S: Scale Factor</td><td>S: Scale Factor</td><td>E: Edge pixel count</td><td>S: Scale Factor minus Normal</td><td>Dx/Dy: x/y-direction Delta Initial Value</td></tr>
<tr><td>Scale</td><td>Percentage</td><td>Hex value</td><td>Hex value</td><td>Hex value</td><td>Hex value</td><td>Hex value</td></tr>
<tr><td>1x</td><td>100%</td><td>1_0000h</td><td>1_0000h</td><td>0h</td><td>0_0000h</td><td>0_0000h</td></tr>
<tr><td> </td><td>100.00153%</td><td>1_0001h</td><td>0_FFFFh</td><td>1h</td><td>F_FFFFh</td><td>0_FFFEh</td></tr>
<tr><td>1.01x</td><td>101%</td><td>1_028Fh</td><td>0_FD77h</td><td>1h</td><td>F_FD77h</td><td>0_FC32h</td></tr>
<tr><td>1.5x</td><td>150%</td><td>1_8000h</td><td>0_AAAAh</td><td>1h</td><td>F_AAAAh</td><td>0_7FFFh</td></tr>
<tr><td>2x</td><td>200%</td><td>2_0000h</td><td>0_8000h</td><td>1h</td><td>F_8000h</td><td>0_4000h</td></tr>
<tr><td>3x</td><td>300%</td><td>3_0000h</td><td>0_5555h</td><td>1h</td><td>F_5555h</td><td>0_0000h</td></tr>
<tr><td>4x</td><td>400%</td><td>4_0000h</td><td>0_4000h</td><td>2h</td><td>F_4000h</td><td>2_0000h</td></tr>
<tr><td></td><td>99%</td><td>0_FD70h</td><td>1_0295h</td><td>0h</td><td>0_0295</td><td>0_0014h</td></tr>
<tr><td></td><td>99.99847%</td><td>0_FFFFh</td><td>1_0001h</td><td>0h</td><td>0_0001h</td><td>0_0000h</td></tr>
<tr><td>.75x</td><td></td><td>0_C000h</td><td>1_5555h</td><td>0h</td><td>0_5555h</td><td>0_2AAAh</td></tr>
<tr><td>.5x</td><td>50%</td><td>0_8000h</td><td>2_0000h</td><td>0h</td><td>1_0000h</td><td>0_8000h</td></tr>
<tr><td>.25x</td><td>25%</td><td>0_4000h</td><td>N/A</td><td>N/A</td><td>N/A</td><td>N/A</td></tr>
</table>
<p>Once written, this register will stick through all subsequent strips until changed. The edge pixel count is ignored if in Average Pixel Replication mode. The edge pixel count is reloaded internally for the first column of each new row and the last column if the end of output column count has not been reached. An internal countdown commences to zero before the scale gets underway. The pixels are padded with the nearest neighbor value.</p>*/
//====================================================================

#define  XYSCALE_HSR_RESERVED1_MASK 0xf0000000
#define  XYSCALE_HSR_RESERVED1_SHIFT 28
#define  XYSCALE_HSR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_HSR_RESERVED1_MASK) >> XYSCALE_HSR_RESERVED1_SHIFT)
#define  XYSCALE_HSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HSR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_HSR_RESERVED1_SHIFT))

#define  XYSCALE_HSR_E_MASK 0xf000000
#define  XYSCALE_HSR_E_SHIFT 24
#define  XYSCALE_HSR_E_MASK_SHIFT(reg) (((reg) & XYSCALE_HSR_E_MASK) >> XYSCALE_HSR_E_SHIFT)
#define  XYSCALE_HSR_E_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HSR_E_MASK) | (((uint32_t)val) << XYSCALE_HSR_E_SHIFT))

#define  XYSCALE_HSR_RESERVED2_MASK 0xf00000
#define  XYSCALE_HSR_RESERVED2_SHIFT 20
#define  XYSCALE_HSR_RESERVED2_MASK_SHIFT(reg) (((reg) & XYSCALE_HSR_RESERVED2_MASK) >> XYSCALE_HSR_RESERVED2_SHIFT)
#define  XYSCALE_HSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HSR_RESERVED2_MASK) | (((uint32_t)val) << XYSCALE_HSR_RESERVED2_SHIFT))

#define  XYSCALE_HSR_S_MASK 0xfffff
#define  XYSCALE_HSR_S_SHIFT 0
#define  XYSCALE_HSR_S_MASK_SHIFT(reg) (((reg) & XYSCALE_HSR_S_MASK) >> XYSCALE_HSR_S_SHIFT)
#define  XYSCALE_HSR_S_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HSR_S_MASK) | (((uint32_t)val) << XYSCALE_HSR_S_SHIFT))

//====================================================================
//Register: Vertical Scale Register (VSR)
/** \brief Scale factor y-direction
At this time the Vertical Scale does not implement the Edge Pixel Pad Option. Instead firmware is required to pad the top and bottom of page with extra bits when enlarging an image. The reason for any padding is that the bi-linear scale when enlarging will have its output transpose grid overlapping the input grid, such that the overlapping output pixels can only reference one input pixel and a slope cannot be determined.*/
//====================================================================

#define  XYSCALE_VSR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_VSR_RESERVED1_SHIFT 20
#define  XYSCALE_VSR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_VSR_RESERVED1_MASK) >> XYSCALE_VSR_RESERVED1_SHIFT)
#define  XYSCALE_VSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VSR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_VSR_RESERVED1_SHIFT))

#define  XYSCALE_VSR_S_MASK 0xfffff
#define  XYSCALE_VSR_S_SHIFT 0
#define  XYSCALE_VSR_S_MASK_SHIFT(reg) (((reg) & XYSCALE_VSR_S_MASK) >> XYSCALE_VSR_S_SHIFT)
#define  XYSCALE_VSR_S_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VSR_S_MASK) | (((uint32_t)val) << XYSCALE_VSR_S_SHIFT))

//====================================================================
//Register: Horizontal Scale Minus Normal Register (HSMR)
/** \brief Scale factor x-direction minus the Normal. Only required for linear mode operation.*/
//====================================================================

#define  XYSCALE_HSMR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_HSMR_RESERVED1_SHIFT 20
#define  XYSCALE_HSMR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_HSMR_RESERVED1_MASK) >> XYSCALE_HSMR_RESERVED1_SHIFT)
#define  XYSCALE_HSMR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HSMR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_HSMR_RESERVED1_SHIFT))

#define  XYSCALE_HSMR_S_MASK 0xfffff
#define  XYSCALE_HSMR_S_SHIFT 0
#define  XYSCALE_HSMR_S_MASK_SHIFT(reg) (((reg) & XYSCALE_HSMR_S_MASK) >> XYSCALE_HSMR_S_SHIFT)
#define  XYSCALE_HSMR_S_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HSMR_S_MASK) | (((uint32_t)val) << XYSCALE_HSMR_S_SHIFT))

//====================================================================
//Register: Vertical Scale Minus Normal Register (VSMR)
/** \brief Scale factor y-direction minus the Normal. Only required for linear mode operation.*/
//====================================================================

#define  XYSCALE_VSMR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_VSMR_RESERVED1_SHIFT 20
#define  XYSCALE_VSMR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_VSMR_RESERVED1_MASK) >> XYSCALE_VSMR_RESERVED1_SHIFT)
#define  XYSCALE_VSMR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VSMR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_VSMR_RESERVED1_SHIFT))

#define  XYSCALE_VSMR_S_MASK 0xfffff
#define  XYSCALE_VSMR_S_SHIFT 0
#define  XYSCALE_VSMR_S_MASK_SHIFT(reg) (((reg) & XYSCALE_VSMR_S_MASK) >> XYSCALE_VSMR_S_SHIFT)
#define  XYSCALE_VSMR_S_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VSMR_S_MASK) | (((uint32_t)val) << XYSCALE_VSMR_S_SHIFT))

//====================================================================
//Register: X-Transpose Configuration Register (XTR)
/** \brief This register in conjunction with the Horizontal Scale register stores configuration information beyond scale factors that the X-Y Scale block uses.
The Dx calculation uses the results of the Horizontal Scale register, so this results should be determined first.
Within the block, the Dx is reloaded after the column count is met.*/
//====================================================================

#define  XYSCALE_XTR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_XTR_RESERVED1_SHIFT 20
#define  XYSCALE_XTR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_XTR_RESERVED1_MASK) >> XYSCALE_XTR_RESERVED1_SHIFT)
#define  XYSCALE_XTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XTR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_XTR_RESERVED1_SHIFT))

#define  XYSCALE_XTR_DX_MASK 0xfffff
#define  XYSCALE_XTR_DX_SHIFT 0
#define  XYSCALE_XTR_DX_MASK_SHIFT(reg) (((reg) & XYSCALE_XTR_DX_MASK) >> XYSCALE_XTR_DX_SHIFT)
#define  XYSCALE_XTR_DX_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XTR_DX_MASK) | (((uint32_t)val) << XYSCALE_XTR_DX_SHIFT))

//====================================================================
//Register: Y-Transpose Configuration Register (YTR)
/** \brief In linear mode, this register in conjunction with the Vertical Scale register stores configuration information beyond scale factors that the X-Y Scale block uses.
Calculation for this register bits Dy requires results of the Vertical Scale register which should be determined beforehand. The Vertical Edge Pixel Count is determined as if it were being used. In actuality, FW is padding the top & bottom of strips.
Typically, Dy is loaded only once for top of page. The transpose in the vertical direction is maintained by the Current Y-Start register. The Current Y-Start register can be saved and re-written into this register to stop current image, write this register start & finish a different image strip, and then resume.*/
//====================================================================

#define  XYSCALE_YTR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_YTR_RESERVED1_SHIFT 20
#define  XYSCALE_YTR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_YTR_RESERVED1_MASK) >> XYSCALE_YTR_RESERVED1_SHIFT)
#define  XYSCALE_YTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_YTR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_YTR_RESERVED1_SHIFT))

#define  XYSCALE_YTR_DY_MASK 0xfffff
#define  XYSCALE_YTR_DY_SHIFT 0
#define  XYSCALE_YTR_DY_MASK_SHIFT(reg) (((reg) & XYSCALE_YTR_DY_MASK) >> XYSCALE_YTR_DY_SHIFT)
#define  XYSCALE_YTR_DY_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_YTR_DY_MASK) | (((uint32_t)val) << XYSCALE_YTR_DY_SHIFT))

//====================================================================
//Register: Current X-Start Register (CXR)
/** \brief This register allows read access of the current internal start Dx register. The initial value comes from the X-Transpose Register and can be read back as such during page initialization.*/
//====================================================================

#define  XYSCALE_CXR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_CXR_RESERVED1_SHIFT 20
#define  XYSCALE_CXR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_CXR_RESERVED1_MASK) >> XYSCALE_CXR_RESERVED1_SHIFT)
#define  XYSCALE_CXR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CXR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_CXR_RESERVED1_SHIFT))

#define  XYSCALE_CXR_DX_MASK 0xfffff
#define  XYSCALE_CXR_DX_SHIFT 0
#define  XYSCALE_CXR_DX_MASK_SHIFT(reg) (((reg) & XYSCALE_CXR_DX_MASK) >> XYSCALE_CXR_DX_SHIFT)
#define  XYSCALE_CXR_DX_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CXR_DX_MASK) | (((uint32_t)val) << XYSCALE_CXR_DX_SHIFT))

//====================================================================
//Register: Current Y Transpose Register (CYR)
/** \brief In linear mode, this register allows read access of the current internal Dy register. The initial value comes from the Y-Transpose Register and can be read back as such during page initialization. When switching between images, the value of this register should be read and saved before starting the other image, and then the saved value should be written to the Y-Transpose Register before resuming the original image.*/
//====================================================================

#define  XYSCALE_CYR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_CYR_RESERVED1_SHIFT 20
#define  XYSCALE_CYR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_CYR_RESERVED1_MASK) >> XYSCALE_CYR_RESERVED1_SHIFT)
#define  XYSCALE_CYR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CYR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_CYR_RESERVED1_SHIFT))

#define  XYSCALE_CYR_DY_MASK 0xfffff
#define  XYSCALE_CYR_DY_SHIFT 0
#define  XYSCALE_CYR_DY_MASK_SHIFT(reg) (((reg) & XYSCALE_CYR_DY_MASK) >> XYSCALE_CYR_DY_SHIFT)
#define  XYSCALE_CYR_DY_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_CYR_DY_MASK) | (((uint32_t)val) << XYSCALE_CYR_DY_SHIFT))

//====================================================================
//Register: Vertical Start Percentage (VSPR)
/** \brief For RAPR mode, at the beginning of an image this register should be initialized to the normal or 2^16 (10000h). When switching between images, the value from the VCPR should be saved off before starting the new image. When retuning to the original image the saved value should be written back into this register.*/
//====================================================================

#define  XYSCALE_VSPR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_VSPR_RESERVED1_SHIFT 20
#define  XYSCALE_VSPR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_VSPR_RESERVED1_MASK) >> XYSCALE_VSPR_RESERVED1_SHIFT)
#define  XYSCALE_VSPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VSPR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_VSPR_RESERVED1_SHIFT))

#define  XYSCALE_VSPR_P_MASK 0xfffff
#define  XYSCALE_VSPR_P_SHIFT 0
#define  XYSCALE_VSPR_P_MASK_SHIFT(reg) (((reg) & XYSCALE_VSPR_P_MASK) >> XYSCALE_VSPR_P_SHIFT)
#define  XYSCALE_VSPR_P_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VSPR_P_MASK) | (((uint32_t)val) << XYSCALE_VSPR_P_SHIFT))

//====================================================================
//Register: Horizontal Current/End Percentage Register (HCPR)
/** \brief In RAPR mode, the first column of each strip starts with a percentage of the normal or 2^16 (1000h), and has a running percentage as the X-Y Scale traverses horizontally. This register allows looking at the current (or ending) percentage. The normal is re-loaded once the column count has been met. This is for debug purposes only.*/
//====================================================================

#define  XYSCALE_HCPR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_HCPR_RESERVED1_SHIFT 20
#define  XYSCALE_HCPR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_HCPR_RESERVED1_MASK) >> XYSCALE_HCPR_RESERVED1_SHIFT)
#define  XYSCALE_HCPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HCPR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_HCPR_RESERVED1_SHIFT))

#define  XYSCALE_HCPR_P_MASK 0xfffff
#define  XYSCALE_HCPR_P_SHIFT 0
#define  XYSCALE_HCPR_P_MASK_SHIFT(reg) (((reg) & XYSCALE_HCPR_P_MASK) >> XYSCALE_HCPR_P_SHIFT)
#define  XYSCALE_HCPR_P_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_HCPR_P_MASK) | (((uint32_t)val) << XYSCALE_HCPR_P_SHIFT))

//====================================================================
//Register: Vertical Current/End Percentage Register (VCPR)
/** \brief In RAPR mode, this register tracks the vertical percentage while traversing pixels in a column. This register is read only, and may be used to save off the current percentage at the end of a strip before switching to a different image. This saved-off value can then be written into the VSPR register when re-starting the original image.*/
//====================================================================

#define  XYSCALE_VCPR_RESERVED1_MASK 0xfff00000
#define  XYSCALE_VCPR_RESERVED1_SHIFT 20
#define  XYSCALE_VCPR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_VCPR_RESERVED1_MASK) >> XYSCALE_VCPR_RESERVED1_SHIFT)
#define  XYSCALE_VCPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VCPR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_VCPR_RESERVED1_SHIFT))

#define  XYSCALE_VCPR_P_MASK 0xfffff
#define  XYSCALE_VCPR_P_SHIFT 0
#define  XYSCALE_VCPR_P_MASK_SHIFT(reg) (((reg) & XYSCALE_VCPR_P_MASK) >> XYSCALE_VCPR_P_SHIFT)
#define  XYSCALE_VCPR_P_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_VCPR_P_MASK) | (((uint32_t)val) << XYSCALE_VCPR_P_SHIFT))

//====================================================================
//Register: Observation Register (XOR)
/** \brief This read only register is for observation of certain XYScale internals.*/
//====================================================================

#define  XYSCALE_XOR_RESERVED1_MASK 0xf0000000
#define  XYSCALE_XOR_RESERVED1_SHIFT 28
#define  XYSCALE_XOR_RESERVED1_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_RESERVED1_MASK) >> XYSCALE_XOR_RESERVED1_SHIFT)
#define  XYSCALE_XOR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_RESERVED1_MASK) | (((uint32_t)val) << XYSCALE_XOR_RESERVED1_SHIFT))

#define  XYSCALE_XOR_AY_MASK 0xf000000
#define  XYSCALE_XOR_AY_SHIFT 24
#define  XYSCALE_XOR_AY_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_AY_MASK) >> XYSCALE_XOR_AY_SHIFT)
#define  XYSCALE_XOR_AY_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_AY_MASK) | (((uint32_t)val) << XYSCALE_XOR_AY_SHIFT))

#define  XYSCALE_XOR_RESERVED2_MASK 0xe00000
#define  XYSCALE_XOR_RESERVED2_SHIFT 21
#define  XYSCALE_XOR_RESERVED2_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_RESERVED2_MASK) >> XYSCALE_XOR_RESERVED2_SHIFT)
#define  XYSCALE_XOR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_RESERVED2_MASK) | (((uint32_t)val) << XYSCALE_XOR_RESERVED2_SHIFT))

#define  XYSCALE_XOR_LC_MASK 0x1f0000
#define  XYSCALE_XOR_LC_SHIFT 16
#define  XYSCALE_XOR_LC_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_LC_MASK) >> XYSCALE_XOR_LC_SHIFT)
#define  XYSCALE_XOR_LC_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_LC_MASK) | (((uint32_t)val) << XYSCALE_XOR_LC_SHIFT))

#define  XYSCALE_XOR_SM_MASK 0xff00
#define  XYSCALE_XOR_SM_SHIFT 8
#define  XYSCALE_XOR_SM_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_SM_MASK) >> XYSCALE_XOR_SM_SHIFT)
#define  XYSCALE_XOR_SM_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_SM_MASK) | (((uint32_t)val) << XYSCALE_XOR_SM_SHIFT))

#define  XYSCALE_XOR_RESERVED3_MASK 0xfc
#define  XYSCALE_XOR_RESERVED3_SHIFT 2
#define  XYSCALE_XOR_RESERVED3_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_RESERVED3_MASK) >> XYSCALE_XOR_RESERVED3_SHIFT)
#define  XYSCALE_XOR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_RESERVED3_MASK) | (((uint32_t)val) << XYSCALE_XOR_RESERVED3_SHIFT))

#define  XYSCALE_XOR_RY_MASK 0x2
#define  XYSCALE_XOR_RY_SHIFT 1
#define  XYSCALE_XOR_RY_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_RY_MASK) >> XYSCALE_XOR_RY_SHIFT)
#define  XYSCALE_XOR_RY_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_RY_MASK) | (((uint32_t)val) << XYSCALE_XOR_RY_SHIFT))

#define  XYSCALE_XOR_RX_MASK 0x1
#define  XYSCALE_XOR_RX_SHIFT 0
#define  XYSCALE_XOR_RX_MASK_SHIFT(reg) (((reg) & XYSCALE_XOR_RX_MASK) >> XYSCALE_XOR_RX_SHIFT)
#define  XYSCALE_XOR_RX_REPLACE_VAL(reg,val) (((reg) & ~XYSCALE_XOR_RX_MASK) | (((uint32_t)val) << XYSCALE_XOR_RX_SHIFT))

#endif // XYSCALE
