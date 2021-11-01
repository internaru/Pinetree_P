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
 * \file ADCNorm_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ADCNORM_REGMASKS_H_
#define _ADCNORM_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: ADCNorm (ADCNorm)
/** \brief Register Descriptions for the PIC ADCNorm block*/
//
//====================================================================

//====================================================================
//Register: ADC Normalization Register (ANR)
/** \brief The ANR Register controls the normalization functions as defined below. Note that the normalization logic is bypassed in Bypass Mode and when the ExposureIn CBI signal is valid.*/
//====================================================================

#define  ADCNORM_ANR_RESERVED1_MASK 0xfffffc00
#define  ADCNORM_ANR_RESERVED1_SHIFT 10
#define  ADCNORM_ANR_RESERVED1_MASK_SHIFT(reg) (((reg) & ADCNORM_ANR_RESERVED1_MASK) >> ADCNORM_ANR_RESERVED1_SHIFT)
#define  ADCNORM_ANR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANR_RESERVED1_MASK) | (((uint32_t)val) << ADCNORM_ANR_RESERVED1_SHIFT))

#define  ADCNORM_ANR_TESTMODE_MASK 0x200
#define  ADCNORM_ANR_TESTMODE_SHIFT 9
#define  ADCNORM_ANR_TESTMODE_MASK_SHIFT(reg) (((reg) & ADCNORM_ANR_TESTMODE_MASK) >> ADCNORM_ANR_TESTMODE_SHIFT)
#define  ADCNORM_ANR_TESTMODE_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANR_TESTMODE_MASK) | (((uint32_t)val) << ADCNORM_ANR_TESTMODE_SHIFT))

#define  ADCNORM_ANR_BYPASS_MASK 0x100
#define  ADCNORM_ANR_BYPASS_SHIFT 8
#define  ADCNORM_ANR_BYPASS_MASK_SHIFT(reg) (((reg) & ADCNORM_ANR_BYPASS_MASK) >> ADCNORM_ANR_BYPASS_SHIFT)
#define  ADCNORM_ANR_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANR_BYPASS_MASK) | (((uint32_t)val) << ADCNORM_ANR_BYPASS_SHIFT))

#define  ADCNORM_ANR_INVERT_MASK 0x80
#define  ADCNORM_ANR_INVERT_SHIFT 7
#define  ADCNORM_ANR_INVERT_MASK_SHIFT(reg) (((reg) & ADCNORM_ANR_INVERT_MASK) >> ADCNORM_ANR_INVERT_SHIFT)
#define  ADCNORM_ANR_INVERT_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANR_INVERT_MASK) | (((uint32_t)val) << ADCNORM_ANR_INVERT_SHIFT))

#define  ADCNORM_ANR_JUSTIFY_MASK 0x40
#define  ADCNORM_ANR_JUSTIFY_SHIFT 6
#define  ADCNORM_ANR_JUSTIFY_MASK_SHIFT(reg) (((reg) & ADCNORM_ANR_JUSTIFY_MASK) >> ADCNORM_ANR_JUSTIFY_SHIFT)
#define  ADCNORM_ANR_JUSTIFY_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANR_JUSTIFY_MASK) | (((uint32_t)val) << ADCNORM_ANR_JUSTIFY_SHIFT))

#define  ADCNORM_ANR_PADFILL_MASK 0x30
#define  ADCNORM_ANR_PADFILL_SHIFT 4
#define  ADCNORM_ANR_PADFILL_MASK_SHIFT(reg) (((reg) & ADCNORM_ANR_PADFILL_MASK) >> ADCNORM_ANR_PADFILL_SHIFT)
#define  ADCNORM_ANR_PADFILL_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANR_PADFILL_MASK) | (((uint32_t)val) << ADCNORM_ANR_PADFILL_SHIFT))

#define  ADCNORM_ANR_ADCBITS_MASK 0xf
#define  ADCNORM_ANR_ADCBITS_SHIFT 0
#define  ADCNORM_ANR_ADCBITS_MASK_SHIFT(reg) (((reg) & ADCNORM_ANR_ADCBITS_MASK) >> ADCNORM_ANR_ADCBITS_SHIFT)
#define  ADCNORM_ANR_ADCBITS_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANR_ADCBITS_MASK) | (((uint32_t)val) << ADCNORM_ANR_ADCBITS_SHIFT))

//====================================================================
//Register: ADC Normalizer Interrupt Enable Register (ANIE)
//====================================================================

#define  ADCNORM_ANIE_RESERVED1_MASK 0xfffffffe
#define  ADCNORM_ANIE_RESERVED1_SHIFT 1
#define  ADCNORM_ANIE_RESERVED1_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIE_RESERVED1_MASK) >> ADCNORM_ANIE_RESERVED1_SHIFT)
#define  ADCNORM_ANIE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIE_RESERVED1_MASK) | (((uint32_t)val) << ADCNORM_ANIE_RESERVED1_SHIFT))

#define  ADCNORM_ANIE_OVERRUN_MASK 0x1
#define  ADCNORM_ANIE_OVERRUN_SHIFT 0
#define  ADCNORM_ANIE_OVERRUN_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIE_OVERRUN_MASK) >> ADCNORM_ANIE_OVERRUN_SHIFT)
#define  ADCNORM_ANIE_OVERRUN_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIE_OVERRUN_MASK) | (((uint32_t)val) << ADCNORM_ANIE_OVERRUN_SHIFT))

//====================================================================
//Register: ADC Normalizer Interrupt Pending Register (ANIP)
//====================================================================

#define  ADCNORM_ANIP_RESERVED1_MASK 0xfffffffe
#define  ADCNORM_ANIP_RESERVED1_SHIFT 1
#define  ADCNORM_ANIP_RESERVED1_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIP_RESERVED1_MASK) >> ADCNORM_ANIP_RESERVED1_SHIFT)
#define  ADCNORM_ANIP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIP_RESERVED1_MASK) | (((uint32_t)val) << ADCNORM_ANIP_RESERVED1_SHIFT))

#define  ADCNORM_ANIP_OVERRUN_MASK 0x1
#define  ADCNORM_ANIP_OVERRUN_SHIFT 0
#define  ADCNORM_ANIP_OVERRUN_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIP_OVERRUN_MASK) >> ADCNORM_ANIP_OVERRUN_SHIFT)
#define  ADCNORM_ANIP_OVERRUN_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIP_OVERRUN_MASK) | (((uint32_t)val) << ADCNORM_ANIP_OVERRUN_SHIFT))

//====================================================================
//Register: ADC Normalizer Interrupt Acknowledge Register (ANIA)
//====================================================================

#define  ADCNORM_ANIA_RESERVED1_MASK 0xfffffffe
#define  ADCNORM_ANIA_RESERVED1_SHIFT 1
#define  ADCNORM_ANIA_RESERVED1_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIA_RESERVED1_MASK) >> ADCNORM_ANIA_RESERVED1_SHIFT)
#define  ADCNORM_ANIA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIA_RESERVED1_MASK) | (((uint32_t)val) << ADCNORM_ANIA_RESERVED1_SHIFT))

#define  ADCNORM_ANIA_OVERRUN_MASK 0x1
#define  ADCNORM_ANIA_OVERRUN_SHIFT 0
#define  ADCNORM_ANIA_OVERRUN_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIA_OVERRUN_MASK) >> ADCNORM_ANIA_OVERRUN_SHIFT)
#define  ADCNORM_ANIA_OVERRUN_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIA_OVERRUN_MASK) | (((uint32_t)val) << ADCNORM_ANIA_OVERRUN_SHIFT))

//====================================================================
//Register: ADC Normalizer Interrupt Force Register (ANIF)
//====================================================================

#define  ADCNORM_ANIF_RESERVED1_MASK 0xfffffffe
#define  ADCNORM_ANIF_RESERVED1_SHIFT 1
#define  ADCNORM_ANIF_RESERVED1_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIF_RESERVED1_MASK) >> ADCNORM_ANIF_RESERVED1_SHIFT)
#define  ADCNORM_ANIF_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIF_RESERVED1_MASK) | (((uint32_t)val) << ADCNORM_ANIF_RESERVED1_SHIFT))

#define  ADCNORM_ANIF_OVERRUN_MASK 0x1
#define  ADCNORM_ANIF_OVERRUN_SHIFT 0
#define  ADCNORM_ANIF_OVERRUN_MASK_SHIFT(reg) (((reg) & ADCNORM_ANIF_OVERRUN_MASK) >> ADCNORM_ANIF_OVERRUN_SHIFT)
#define  ADCNORM_ANIF_OVERRUN_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANIF_OVERRUN_MASK) | (((uint32_t)val) << ADCNORM_ANIF_OVERRUN_SHIFT))

//====================================================================
//Register: ADC Normalization Test Register (ANT)
//====================================================================

#define  ADCNORM_ANT_RESERVED1_MASK 0xffc00000
#define  ADCNORM_ANT_RESERVED1_SHIFT 22
#define  ADCNORM_ANT_RESERVED1_MASK_SHIFT(reg) (((reg) & ADCNORM_ANT_RESERVED1_MASK) >> ADCNORM_ANT_RESERVED1_SHIFT)
#define  ADCNORM_ANT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANT_RESERVED1_MASK) | (((uint32_t)val) << ADCNORM_ANT_RESERVED1_SHIFT))

#define  ADCNORM_ANT_EXPOSURE_MASK 0x200000
#define  ADCNORM_ANT_EXPOSURE_SHIFT 21
#define  ADCNORM_ANT_EXPOSURE_MASK_SHIFT(reg) (((reg) & ADCNORM_ANT_EXPOSURE_MASK) >> ADCNORM_ANT_EXPOSURE_SHIFT)
#define  ADCNORM_ANT_EXPOSURE_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANT_EXPOSURE_MASK) | (((uint32_t)val) << ADCNORM_ANT_EXPOSURE_SHIFT))

#define  ADCNORM_ANT_COLOR_MASK 0x1c0000
#define  ADCNORM_ANT_COLOR_SHIFT 18
#define  ADCNORM_ANT_COLOR_MASK_SHIFT(reg) (((reg) & ADCNORM_ANT_COLOR_MASK) >> ADCNORM_ANT_COLOR_SHIFT)
#define  ADCNORM_ANT_COLOR_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANT_COLOR_MASK) | (((uint32_t)val) << ADCNORM_ANT_COLOR_SHIFT))

#define  ADCNORM_ANT_DATATYPE_MASK 0x30000
#define  ADCNORM_ANT_DATATYPE_SHIFT 16
#define  ADCNORM_ANT_DATATYPE_MASK_SHIFT(reg) (((reg) & ADCNORM_ANT_DATATYPE_MASK) >> ADCNORM_ANT_DATATYPE_SHIFT)
#define  ADCNORM_ANT_DATATYPE_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANT_DATATYPE_MASK) | (((uint32_t)val) << ADCNORM_ANT_DATATYPE_SHIFT))

#define  ADCNORM_ANT_DATA_MASK 0xffff
#define  ADCNORM_ANT_DATA_SHIFT 0
#define  ADCNORM_ANT_DATA_MASK_SHIFT(reg) (((reg) & ADCNORM_ANT_DATA_MASK) >> ADCNORM_ANT_DATA_SHIFT)
#define  ADCNORM_ANT_DATA_REPLACE_VAL(reg,val) (((reg) & ~ADCNORM_ANT_DATA_MASK) | (((uint32_t)val) << ADCNORM_ANT_DATA_SHIFT))

#endif // ADCNORM
