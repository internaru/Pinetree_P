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
 * \file HFIR_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HFIR_REGMASKS_H_
#define _HFIR_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Horizontal FIR Filter (HFIR)
/** \brief Register file for the HFIR block*/
//
//====================================================================

//====================================================================
//Register: HFIR Configuration Register (Cfg)
/** \brief This register configures the major features of the HFIR module*/
//====================================================================

#define  HFIR_CFG_RESERVED1_MASK 0xffff0000
#define  HFIR_CFG_RESERVED1_SHIFT 16
#define  HFIR_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & HFIR_CFG_RESERVED1_MASK) >> HFIR_CFG_RESERVED1_SHIFT)
#define  HFIR_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CFG_RESERVED1_MASK) | (((uint32_t)val) << HFIR_CFG_RESERVED1_SHIFT))

#define  HFIR_CFG_BCONFIGDATA_MASK 0xfff0
#define  HFIR_CFG_BCONFIGDATA_SHIFT 4
#define  HFIR_CFG_BCONFIGDATA_MASK_SHIFT(reg) (((reg) & HFIR_CFG_BCONFIGDATA_MASK) >> HFIR_CFG_BCONFIGDATA_SHIFT)
#define  HFIR_CFG_BCONFIGDATA_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CFG_BCONFIGDATA_MASK) | (((uint32_t)val) << HFIR_CFG_BCONFIGDATA_SHIFT))

#define  HFIR_CFG_RESERVED2_MASK 0x8
#define  HFIR_CFG_RESERVED2_SHIFT 3
#define  HFIR_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & HFIR_CFG_RESERVED2_MASK) >> HFIR_CFG_RESERVED2_SHIFT)
#define  HFIR_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CFG_RESERVED2_MASK) | (((uint32_t)val) << HFIR_CFG_RESERVED2_SHIFT))

#define  HFIR_CFG_BCONFIG_MASK 0x4
#define  HFIR_CFG_BCONFIG_SHIFT 2
#define  HFIR_CFG_BCONFIG_MASK_SHIFT(reg) (((reg) & HFIR_CFG_BCONFIG_MASK) >> HFIR_CFG_BCONFIG_SHIFT)
#define  HFIR_CFG_BCONFIG_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CFG_BCONFIG_MASK) | (((uint32_t)val) << HFIR_CFG_BCONFIG_SHIFT))

#define  HFIR_CFG_EVEN_MASK 0x2
#define  HFIR_CFG_EVEN_SHIFT 1
#define  HFIR_CFG_EVEN_MASK_SHIFT(reg) (((reg) & HFIR_CFG_EVEN_MASK) >> HFIR_CFG_EVEN_SHIFT)
#define  HFIR_CFG_EVEN_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CFG_EVEN_MASK) | (((uint32_t)val) << HFIR_CFG_EVEN_SHIFT))

#define  HFIR_CFG_BYPASS_MASK 0x1
#define  HFIR_CFG_BYPASS_SHIFT 0
#define  HFIR_CFG_BYPASS_MASK_SHIFT(reg) (((reg) & HFIR_CFG_BYPASS_MASK) >> HFIR_CFG_BYPASS_SHIFT)
#define  HFIR_CFG_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CFG_BYPASS_MASK) | (((uint32_t)val) << HFIR_CFG_BYPASS_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 0 and 1 (coeff0_1)
/** \brief This register sets coefficient value for set 0 and 1*/
//====================================================================

#define  HFIR_COEFF0_1_COEFF1_MASK 0xffff0000
#define  HFIR_COEFF0_1_COEFF1_SHIFT 16
#define  HFIR_COEFF0_1_COEFF1_MASK_SHIFT(reg) (((reg) & HFIR_COEFF0_1_COEFF1_MASK) >> HFIR_COEFF0_1_COEFF1_SHIFT)
#define  HFIR_COEFF0_1_COEFF1_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF0_1_COEFF1_MASK) | (((uint32_t)val) << HFIR_COEFF0_1_COEFF1_SHIFT))

#define  HFIR_COEFF0_1_COEFF0_MASK 0xffff
#define  HFIR_COEFF0_1_COEFF0_SHIFT 0
#define  HFIR_COEFF0_1_COEFF0_MASK_SHIFT(reg) (((reg) & HFIR_COEFF0_1_COEFF0_MASK) >> HFIR_COEFF0_1_COEFF0_SHIFT)
#define  HFIR_COEFF0_1_COEFF0_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF0_1_COEFF0_MASK) | (((uint32_t)val) << HFIR_COEFF0_1_COEFF0_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 2 and 3 (coeff2_3)
/** \brief This register sets coefficient value for set 2 and 3*/
//====================================================================

#define  HFIR_COEFF2_3_COEFF3_MASK 0xffff0000
#define  HFIR_COEFF2_3_COEFF3_SHIFT 16
#define  HFIR_COEFF2_3_COEFF3_MASK_SHIFT(reg) (((reg) & HFIR_COEFF2_3_COEFF3_MASK) >> HFIR_COEFF2_3_COEFF3_SHIFT)
#define  HFIR_COEFF2_3_COEFF3_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF2_3_COEFF3_MASK) | (((uint32_t)val) << HFIR_COEFF2_3_COEFF3_SHIFT))

#define  HFIR_COEFF2_3_COEFF2_MASK 0xffff
#define  HFIR_COEFF2_3_COEFF2_SHIFT 0
#define  HFIR_COEFF2_3_COEFF2_MASK_SHIFT(reg) (((reg) & HFIR_COEFF2_3_COEFF2_MASK) >> HFIR_COEFF2_3_COEFF2_SHIFT)
#define  HFIR_COEFF2_3_COEFF2_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF2_3_COEFF2_MASK) | (((uint32_t)val) << HFIR_COEFF2_3_COEFF2_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 4 and 5 (coeff4_5)
/** \brief This register sets coefficient value for set 4 and 5*/
//====================================================================

#define  HFIR_COEFF4_5_COEFF5_MASK 0xffff0000
#define  HFIR_COEFF4_5_COEFF5_SHIFT 16
#define  HFIR_COEFF4_5_COEFF5_MASK_SHIFT(reg) (((reg) & HFIR_COEFF4_5_COEFF5_MASK) >> HFIR_COEFF4_5_COEFF5_SHIFT)
#define  HFIR_COEFF4_5_COEFF5_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF4_5_COEFF5_MASK) | (((uint32_t)val) << HFIR_COEFF4_5_COEFF5_SHIFT))

#define  HFIR_COEFF4_5_COEFF4_MASK 0xffff
#define  HFIR_COEFF4_5_COEFF4_SHIFT 0
#define  HFIR_COEFF4_5_COEFF4_MASK_SHIFT(reg) (((reg) & HFIR_COEFF4_5_COEFF4_MASK) >> HFIR_COEFF4_5_COEFF4_SHIFT)
#define  HFIR_COEFF4_5_COEFF4_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF4_5_COEFF4_MASK) | (((uint32_t)val) << HFIR_COEFF4_5_COEFF4_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 6 and 7 (coeff6_7)
/** \brief This register sets coefficient value for set 6 and 7*/
//====================================================================

#define  HFIR_COEFF6_7_COEFF7_MASK 0xffff0000
#define  HFIR_COEFF6_7_COEFF7_SHIFT 16
#define  HFIR_COEFF6_7_COEFF7_MASK_SHIFT(reg) (((reg) & HFIR_COEFF6_7_COEFF7_MASK) >> HFIR_COEFF6_7_COEFF7_SHIFT)
#define  HFIR_COEFF6_7_COEFF7_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF6_7_COEFF7_MASK) | (((uint32_t)val) << HFIR_COEFF6_7_COEFF7_SHIFT))

#define  HFIR_COEFF6_7_COEFF6_MASK 0xffff
#define  HFIR_COEFF6_7_COEFF6_SHIFT 0
#define  HFIR_COEFF6_7_COEFF6_MASK_SHIFT(reg) (((reg) & HFIR_COEFF6_7_COEFF6_MASK) >> HFIR_COEFF6_7_COEFF6_SHIFT)
#define  HFIR_COEFF6_7_COEFF6_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF6_7_COEFF6_MASK) | (((uint32_t)val) << HFIR_COEFF6_7_COEFF6_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 8 and 9 (coeff8_9)
/** \brief This register sets coefficient value for set 8 and 9*/
//====================================================================

#define  HFIR_COEFF8_9_COEFF9_MASK 0xffff0000
#define  HFIR_COEFF8_9_COEFF9_SHIFT 16
#define  HFIR_COEFF8_9_COEFF9_MASK_SHIFT(reg) (((reg) & HFIR_COEFF8_9_COEFF9_MASK) >> HFIR_COEFF8_9_COEFF9_SHIFT)
#define  HFIR_COEFF8_9_COEFF9_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF8_9_COEFF9_MASK) | (((uint32_t)val) << HFIR_COEFF8_9_COEFF9_SHIFT))

#define  HFIR_COEFF8_9_COEFF8_MASK 0xffff
#define  HFIR_COEFF8_9_COEFF8_SHIFT 0
#define  HFIR_COEFF8_9_COEFF8_MASK_SHIFT(reg) (((reg) & HFIR_COEFF8_9_COEFF8_MASK) >> HFIR_COEFF8_9_COEFF8_SHIFT)
#define  HFIR_COEFF8_9_COEFF8_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF8_9_COEFF8_MASK) | (((uint32_t)val) << HFIR_COEFF8_9_COEFF8_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 10 and 11 (coeff10_11)
/** \brief This register sets coefficient value for set 10 and 11*/
//====================================================================

#define  HFIR_COEFF10_11_COEFF11_MASK 0xffff0000
#define  HFIR_COEFF10_11_COEFF11_SHIFT 16
#define  HFIR_COEFF10_11_COEFF11_MASK_SHIFT(reg) (((reg) & HFIR_COEFF10_11_COEFF11_MASK) >> HFIR_COEFF10_11_COEFF11_SHIFT)
#define  HFIR_COEFF10_11_COEFF11_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF10_11_COEFF11_MASK) | (((uint32_t)val) << HFIR_COEFF10_11_COEFF11_SHIFT))

#define  HFIR_COEFF10_11_COEFF10_MASK 0xffff
#define  HFIR_COEFF10_11_COEFF10_SHIFT 0
#define  HFIR_COEFF10_11_COEFF10_MASK_SHIFT(reg) (((reg) & HFIR_COEFF10_11_COEFF10_MASK) >> HFIR_COEFF10_11_COEFF10_SHIFT)
#define  HFIR_COEFF10_11_COEFF10_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF10_11_COEFF10_MASK) | (((uint32_t)val) << HFIR_COEFF10_11_COEFF10_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 12 and 13 (coeff12_13)
/** \brief This register sets coefficient value for set 12 and 13*/
//====================================================================

#define  HFIR_COEFF12_13_COEFF13_MASK 0xffff0000
#define  HFIR_COEFF12_13_COEFF13_SHIFT 16
#define  HFIR_COEFF12_13_COEFF13_MASK_SHIFT(reg) (((reg) & HFIR_COEFF12_13_COEFF13_MASK) >> HFIR_COEFF12_13_COEFF13_SHIFT)
#define  HFIR_COEFF12_13_COEFF13_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF12_13_COEFF13_MASK) | (((uint32_t)val) << HFIR_COEFF12_13_COEFF13_SHIFT))

#define  HFIR_COEFF12_13_COEFF12_MASK 0xffff
#define  HFIR_COEFF12_13_COEFF12_SHIFT 0
#define  HFIR_COEFF12_13_COEFF12_MASK_SHIFT(reg) (((reg) & HFIR_COEFF12_13_COEFF12_MASK) >> HFIR_COEFF12_13_COEFF12_SHIFT)
#define  HFIR_COEFF12_13_COEFF12_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF12_13_COEFF12_MASK) | (((uint32_t)val) << HFIR_COEFF12_13_COEFF12_SHIFT))

//====================================================================
//Register: HFIR Coefficient set 14 and 15 (coeff14_15)
/** \brief This register sets coefficient value for set 14 and 15*/
//====================================================================

#define  HFIR_COEFF14_15_COEFF15_MASK 0xffff0000
#define  HFIR_COEFF14_15_COEFF15_SHIFT 16
#define  HFIR_COEFF14_15_COEFF15_MASK_SHIFT(reg) (((reg) & HFIR_COEFF14_15_COEFF15_MASK) >> HFIR_COEFF14_15_COEFF15_SHIFT)
#define  HFIR_COEFF14_15_COEFF15_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF14_15_COEFF15_MASK) | (((uint32_t)val) << HFIR_COEFF14_15_COEFF15_SHIFT))

#define  HFIR_COEFF14_15_COEFF14_MASK 0xffff
#define  HFIR_COEFF14_15_COEFF14_SHIFT 0
#define  HFIR_COEFF14_15_COEFF14_MASK_SHIFT(reg) (((reg) & HFIR_COEFF14_15_COEFF14_MASK) >> HFIR_COEFF14_15_COEFF14_SHIFT)
#define  HFIR_COEFF14_15_COEFF14_REPLACE_VAL(reg,val) (((reg) & ~HFIR_COEFF14_15_COEFF14_MASK) | (((uint32_t)val) << HFIR_COEFF14_15_COEFF14_SHIFT))

//====================================================================
//Register: Check Sum (CSum)
/** \brief This register is used for debug and test only*/
//====================================================================

#define  HFIR_CSUM_CSUM_MASK 0xffffffff
#define  HFIR_CSUM_CSUM_SHIFT 0
#define  HFIR_CSUM_CSUM_MASK_SHIFT(reg) (((reg) & HFIR_CSUM_CSUM_MASK) >> HFIR_CSUM_CSUM_SHIFT)
#define  HFIR_CSUM_CSUM_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CSUM_CSUM_MASK) | (((uint32_t)val) << HFIR_CSUM_CSUM_SHIFT))

//====================================================================
//Register: Channel Color Decode (CHColorDecode)
/** \brief This register maps the ColorIn to each of the 6 channels.*/
//====================================================================

#define  HFIR_CHCOLORDECODE_RESERVED1_MASK 0xfffc0000
#define  HFIR_CHCOLORDECODE_RESERVED1_SHIFT 18
#define  HFIR_CHCOLORDECODE_RESERVED1_MASK_SHIFT(reg) (((reg) & HFIR_CHCOLORDECODE_RESERVED1_MASK) >> HFIR_CHCOLORDECODE_RESERVED1_SHIFT)
#define  HFIR_CHCOLORDECODE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CHCOLORDECODE_RESERVED1_MASK) | (((uint32_t)val) << HFIR_CHCOLORDECODE_RESERVED1_SHIFT))

#define  HFIR_CHCOLORDECODE_CH5COLORDECODE_MASK 0x38000
#define  HFIR_CHCOLORDECODE_CH5COLORDECODE_SHIFT 15
#define  HFIR_CHCOLORDECODE_CH5COLORDECODE_MASK_SHIFT(reg) (((reg) & HFIR_CHCOLORDECODE_CH5COLORDECODE_MASK) >> HFIR_CHCOLORDECODE_CH5COLORDECODE_SHIFT)
#define  HFIR_CHCOLORDECODE_CH5COLORDECODE_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CHCOLORDECODE_CH5COLORDECODE_MASK) | (((uint32_t)val) << HFIR_CHCOLORDECODE_CH5COLORDECODE_SHIFT))

#define  HFIR_CHCOLORDECODE_CH4COLORDECODE_MASK 0x7000
#define  HFIR_CHCOLORDECODE_CH4COLORDECODE_SHIFT 12
#define  HFIR_CHCOLORDECODE_CH4COLORDECODE_MASK_SHIFT(reg) (((reg) & HFIR_CHCOLORDECODE_CH4COLORDECODE_MASK) >> HFIR_CHCOLORDECODE_CH4COLORDECODE_SHIFT)
#define  HFIR_CHCOLORDECODE_CH4COLORDECODE_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CHCOLORDECODE_CH4COLORDECODE_MASK) | (((uint32_t)val) << HFIR_CHCOLORDECODE_CH4COLORDECODE_SHIFT))

#define  HFIR_CHCOLORDECODE_CH3COLORDECODE_MASK 0xe00
#define  HFIR_CHCOLORDECODE_CH3COLORDECODE_SHIFT 9
#define  HFIR_CHCOLORDECODE_CH3COLORDECODE_MASK_SHIFT(reg) (((reg) & HFIR_CHCOLORDECODE_CH3COLORDECODE_MASK) >> HFIR_CHCOLORDECODE_CH3COLORDECODE_SHIFT)
#define  HFIR_CHCOLORDECODE_CH3COLORDECODE_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CHCOLORDECODE_CH3COLORDECODE_MASK) | (((uint32_t)val) << HFIR_CHCOLORDECODE_CH3COLORDECODE_SHIFT))

#define  HFIR_CHCOLORDECODE_CH2COLORDECODE_MASK 0x1c0
#define  HFIR_CHCOLORDECODE_CH2COLORDECODE_SHIFT 6
#define  HFIR_CHCOLORDECODE_CH2COLORDECODE_MASK_SHIFT(reg) (((reg) & HFIR_CHCOLORDECODE_CH2COLORDECODE_MASK) >> HFIR_CHCOLORDECODE_CH2COLORDECODE_SHIFT)
#define  HFIR_CHCOLORDECODE_CH2COLORDECODE_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CHCOLORDECODE_CH2COLORDECODE_MASK) | (((uint32_t)val) << HFIR_CHCOLORDECODE_CH2COLORDECODE_SHIFT))

#define  HFIR_CHCOLORDECODE_CH1COLORDECODE_MASK 0x38
#define  HFIR_CHCOLORDECODE_CH1COLORDECODE_SHIFT 3
#define  HFIR_CHCOLORDECODE_CH1COLORDECODE_MASK_SHIFT(reg) (((reg) & HFIR_CHCOLORDECODE_CH1COLORDECODE_MASK) >> HFIR_CHCOLORDECODE_CH1COLORDECODE_SHIFT)
#define  HFIR_CHCOLORDECODE_CH1COLORDECODE_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CHCOLORDECODE_CH1COLORDECODE_MASK) | (((uint32_t)val) << HFIR_CHCOLORDECODE_CH1COLORDECODE_SHIFT))

#define  HFIR_CHCOLORDECODE_CH0COLORDECODE_MASK 0x7
#define  HFIR_CHCOLORDECODE_CH0COLORDECODE_SHIFT 0
#define  HFIR_CHCOLORDECODE_CH0COLORDECODE_MASK_SHIFT(reg) (((reg) & HFIR_CHCOLORDECODE_CH0COLORDECODE_MASK) >> HFIR_CHCOLORDECODE_CH0COLORDECODE_SHIFT)
#define  HFIR_CHCOLORDECODE_CH0COLORDECODE_REPLACE_VAL(reg,val) (((reg) & ~HFIR_CHCOLORDECODE_CH0COLORDECODE_MASK) | (((uint32_t)val) << HFIR_CHCOLORDECODE_CH0COLORDECODE_SHIFT))

#endif // HFIR
