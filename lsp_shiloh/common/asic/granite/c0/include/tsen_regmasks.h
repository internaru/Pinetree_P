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
 * \file tsen_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TSEN_REGS_REGMASKS_H_
#define _TSEN_REGS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: TSEN Temperature Sensor Registers (tsen_regs)
/** \brief Registers for firmware control/configuration/status to TSEN hardmacro.*/
//
//====================================================================

//====================================================================
//Register: Soft Reset register (SoftReset)
//====================================================================

#define  TSEN_REGS_SOFTRESET_RESERVED1_MASK 0xfffffffe
#define  TSEN_REGS_SOFTRESET_RESERVED1_SHIFT 1
#define  TSEN_REGS_SOFTRESET_RESERVED1_MASK_SHIFT(reg) (((reg) & TSEN_REGS_SOFTRESET_RESERVED1_MASK) >> TSEN_REGS_SOFTRESET_RESERVED1_SHIFT)
#define  TSEN_REGS_SOFTRESET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_SOFTRESET_RESERVED1_MASK) | (((uint32_t)val) << TSEN_REGS_SOFTRESET_RESERVED1_SHIFT))

#define  TSEN_REGS_SOFTRESET_SOFT_RST_MASK 0x1
#define  TSEN_REGS_SOFTRESET_SOFT_RST_SHIFT 0
#define  TSEN_REGS_SOFTRESET_SOFT_RST_MASK_SHIFT(reg) (((reg) & TSEN_REGS_SOFTRESET_SOFT_RST_MASK) >> TSEN_REGS_SOFTRESET_SOFT_RST_SHIFT)
#define  TSEN_REGS_SOFTRESET_SOFT_RST_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_SOFTRESET_SOFT_RST_MASK) | (((uint32_t)val) << TSEN_REGS_SOFTRESET_SOFT_RST_SHIFT))

//====================================================================
//Register: Start Calibration register (StartCal)
//====================================================================

#define  TSEN_REGS_STARTCAL_RESERVED1_MASK 0xfffffffe
#define  TSEN_REGS_STARTCAL_RESERVED1_SHIFT 1
#define  TSEN_REGS_STARTCAL_RESERVED1_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STARTCAL_RESERVED1_MASK) >> TSEN_REGS_STARTCAL_RESERVED1_SHIFT)
#define  TSEN_REGS_STARTCAL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STARTCAL_RESERVED1_MASK) | (((uint32_t)val) << TSEN_REGS_STARTCAL_RESERVED1_SHIFT))

#define  TSEN_REGS_STARTCAL_START_CAL_MASK 0x1
#define  TSEN_REGS_STARTCAL_START_CAL_SHIFT 0
#define  TSEN_REGS_STARTCAL_START_CAL_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STARTCAL_START_CAL_MASK) >> TSEN_REGS_STARTCAL_START_CAL_SHIFT)
#define  TSEN_REGS_STARTCAL_START_CAL_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STARTCAL_START_CAL_MASK) | (((uint32_t)val) << TSEN_REGS_STARTCAL_START_CAL_SHIFT))

//====================================================================
//Register: Power Control register (PWControl)
//====================================================================

#define  TSEN_REGS_PWCONTROL_RESERVED1_MASK 0xfffffffc
#define  TSEN_REGS_PWCONTROL_RESERVED1_SHIFT 2
#define  TSEN_REGS_PWCONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & TSEN_REGS_PWCONTROL_RESERVED1_MASK) >> TSEN_REGS_PWCONTROL_RESERVED1_SHIFT)
#define  TSEN_REGS_PWCONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_PWCONTROL_RESERVED1_MASK) | (((uint32_t)val) << TSEN_REGS_PWCONTROL_RESERVED1_SHIFT))

#define  TSEN_REGS_PWCONTROL_EXT_PWD_MASK 0x2
#define  TSEN_REGS_PWCONTROL_EXT_PWD_SHIFT 1
#define  TSEN_REGS_PWCONTROL_EXT_PWD_MASK_SHIFT(reg) (((reg) & TSEN_REGS_PWCONTROL_EXT_PWD_MASK) >> TSEN_REGS_PWCONTROL_EXT_PWD_SHIFT)
#define  TSEN_REGS_PWCONTROL_EXT_PWD_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_PWCONTROL_EXT_PWD_MASK) | (((uint32_t)val) << TSEN_REGS_PWCONTROL_EXT_PWD_SHIFT))

#define  TSEN_REGS_PWCONTROL_SLEEP_MASK 0x1
#define  TSEN_REGS_PWCONTROL_SLEEP_SHIFT 0
#define  TSEN_REGS_PWCONTROL_SLEEP_MASK_SHIFT(reg) (((reg) & TSEN_REGS_PWCONTROL_SLEEP_MASK) >> TSEN_REGS_PWCONTROL_SLEEP_SHIFT)
#define  TSEN_REGS_PWCONTROL_SLEEP_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_PWCONTROL_SLEEP_MASK) | (((uint32_t)val) << TSEN_REGS_PWCONTROL_SLEEP_SHIFT))

//====================================================================
//Register: Sensor Configuration register (ConfigSensor)
//====================================================================

#define  TSEN_REGS_CONFIGSENSOR_RESERVED1_MASK 0xfffffc00
#define  TSEN_REGS_CONFIGSENSOR_RESERVED1_SHIFT 10
#define  TSEN_REGS_CONFIGSENSOR_RESERVED1_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGSENSOR_RESERVED1_MASK) >> TSEN_REGS_CONFIGSENSOR_RESERVED1_SHIFT)
#define  TSEN_REGS_CONFIGSENSOR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGSENSOR_RESERVED1_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGSENSOR_RESERVED1_SHIFT))

#define  TSEN_REGS_CONFIGSENSOR_RESERVEBITS_MASK 0x300
#define  TSEN_REGS_CONFIGSENSOR_RESERVEBITS_SHIFT 8
#define  TSEN_REGS_CONFIGSENSOR_RESERVEBITS_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGSENSOR_RESERVEBITS_MASK) >> TSEN_REGS_CONFIGSENSOR_RESERVEBITS_SHIFT)
#define  TSEN_REGS_CONFIGSENSOR_RESERVEBITS_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGSENSOR_RESERVEBITS_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGSENSOR_RESERVEBITS_SHIFT))

#define  TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_MASK 0x80
#define  TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_SHIFT 7
#define  TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_MASK) >> TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_SHIFT)
#define  TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGSENSOR_VBE_BYPASS_SHIFT))

#define  TSEN_REGS_CONFIGSENSOR_TC_TRIM_MASK 0x70
#define  TSEN_REGS_CONFIGSENSOR_TC_TRIM_SHIFT 4
#define  TSEN_REGS_CONFIGSENSOR_TC_TRIM_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGSENSOR_TC_TRIM_MASK) >> TSEN_REGS_CONFIGSENSOR_TC_TRIM_SHIFT)
#define  TSEN_REGS_CONFIGSENSOR_TC_TRIM_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGSENSOR_TC_TRIM_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGSENSOR_TC_TRIM_SHIFT))

#define  TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_MASK 0x8
#define  TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_SHIFT 3
#define  TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_MASK) >> TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_SHIFT)
#define  TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGSENSOR_SEL_IREF_SOURCE_SHIFT))

#define  TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_MASK 0x4
#define  TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_SHIFT 2
#define  TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_MASK) >> TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_SHIFT)
#define  TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGSENSOR_DOUBLE_SLOPE_SHIFT))

#define  TSEN_REGS_CONFIGSENSOR_ATEST_MASK 0x3
#define  TSEN_REGS_CONFIGSENSOR_ATEST_SHIFT 0
#define  TSEN_REGS_CONFIGSENSOR_ATEST_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGSENSOR_ATEST_MASK) >> TSEN_REGS_CONFIGSENSOR_ATEST_SHIFT)
#define  TSEN_REGS_CONFIGSENSOR_ATEST_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGSENSOR_ATEST_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGSENSOR_ATEST_SHIFT))

//====================================================================
//Register: Calibration Configuration register (ConfigCalib)
//====================================================================

#define  TSEN_REGS_CONFIGCALIB_RESERVED1_MASK 0xfc000000
#define  TSEN_REGS_CONFIGCALIB_RESERVED1_SHIFT 26
#define  TSEN_REGS_CONFIGCALIB_RESERVED1_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGCALIB_RESERVED1_MASK) >> TSEN_REGS_CONFIGCALIB_RESERVED1_SHIFT)
#define  TSEN_REGS_CONFIGCALIB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGCALIB_RESERVED1_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGCALIB_RESERVED1_SHIFT))

#define  TSEN_REGS_CONFIGCALIB_SEL_VCAL_MASK 0x3000000
#define  TSEN_REGS_CONFIGCALIB_SEL_VCAL_SHIFT 24
#define  TSEN_REGS_CONFIGCALIB_SEL_VCAL_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGCALIB_SEL_VCAL_MASK) >> TSEN_REGS_CONFIGCALIB_SEL_VCAL_SHIFT)
#define  TSEN_REGS_CONFIGCALIB_SEL_VCAL_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGCALIB_SEL_VCAL_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGCALIB_SEL_VCAL_SHIFT))

#define  TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_MASK 0xe00000
#define  TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_SHIFT 21
#define  TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_MASK) >> TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_SHIFT)
#define  TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGCALIB_SEL_CAL_CAP_SRC_SHIFT))

#define  TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_MASK 0x1ff000
#define  TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_SHIFT 12
#define  TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_MASK) >> TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_SHIFT)
#define  TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGCALIB_REF_CAL_COUNT_SHIFT))

#define  TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_MASK 0x800
#define  TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_SHIFT 11
#define  TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_MASK) >> TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_SHIFT)
#define  TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGCALIB_OTF_CALIBRATION_SHIFT))

#define  TSEN_REGS_CONFIGCALIB_AVG_NUM_MASK 0x700
#define  TSEN_REGS_CONFIGCALIB_AVG_NUM_SHIFT 8
#define  TSEN_REGS_CONFIGCALIB_AVG_NUM_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGCALIB_AVG_NUM_MASK) >> TSEN_REGS_CONFIGCALIB_AVG_NUM_SHIFT)
#define  TSEN_REGS_CONFIGCALIB_AVG_NUM_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGCALIB_AVG_NUM_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGCALIB_AVG_NUM_SHIFT))

#define  TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_MASK 0xff
#define  TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_SHIFT 0
#define  TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_MASK_SHIFT(reg) (((reg) & TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_MASK) >> TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_SHIFT)
#define  TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_MASK) | (((uint32_t)val) << TSEN_REGS_CONFIGCALIB_CAL_START_VALUE_SHIFT))

//====================================================================
//Register: Internal Status register (Status1)
/** \brief The internal state of the temperature sensor (temp_out_int[8:0], cal, cal_value[7:0]) is continuously updated into the Internal Status register (Status1) register.*/
//====================================================================

#define  TSEN_REGS_STATUS1_RESERVED1_MASK 0xfe000000
#define  TSEN_REGS_STATUS1_RESERVED1_SHIFT 25
#define  TSEN_REGS_STATUS1_RESERVED1_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS1_RESERVED1_MASK) >> TSEN_REGS_STATUS1_RESERVED1_SHIFT)
#define  TSEN_REGS_STATUS1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS1_RESERVED1_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS1_RESERVED1_SHIFT))

#define  TSEN_REGS_STATUS1_TEMP_OUT_INT_MASK 0x1ff0000
#define  TSEN_REGS_STATUS1_TEMP_OUT_INT_SHIFT 16
#define  TSEN_REGS_STATUS1_TEMP_OUT_INT_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS1_TEMP_OUT_INT_MASK) >> TSEN_REGS_STATUS1_TEMP_OUT_INT_SHIFT)
#define  TSEN_REGS_STATUS1_TEMP_OUT_INT_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS1_TEMP_OUT_INT_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS1_TEMP_OUT_INT_SHIFT))

#define  TSEN_REGS_STATUS1_RESERVED2_MASK 0xfe00
#define  TSEN_REGS_STATUS1_RESERVED2_SHIFT 9
#define  TSEN_REGS_STATUS1_RESERVED2_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS1_RESERVED2_MASK) >> TSEN_REGS_STATUS1_RESERVED2_SHIFT)
#define  TSEN_REGS_STATUS1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS1_RESERVED2_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS1_RESERVED2_SHIFT))

#define  TSEN_REGS_STATUS1_CAL_MASK 0x100
#define  TSEN_REGS_STATUS1_CAL_SHIFT 8
#define  TSEN_REGS_STATUS1_CAL_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS1_CAL_MASK) >> TSEN_REGS_STATUS1_CAL_SHIFT)
#define  TSEN_REGS_STATUS1_CAL_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS1_CAL_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS1_CAL_SHIFT))

#define  TSEN_REGS_STATUS1_CAL_VALUE_MASK 0xff
#define  TSEN_REGS_STATUS1_CAL_VALUE_SHIFT 0
#define  TSEN_REGS_STATUS1_CAL_VALUE_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS1_CAL_VALUE_MASK) >> TSEN_REGS_STATUS1_CAL_VALUE_SHIFT)
#define  TSEN_REGS_STATUS1_CAL_VALUE_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS1_CAL_VALUE_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS1_CAL_VALUE_SHIFT))

//====================================================================
//Register: Temperature Status register (Status2)
/** \brief The temperature measurement (t_valid and temp_out[8:0]) is continuously updated into the Temperature Status register (a.k.a Status2 register).*/
//====================================================================

#define  TSEN_REGS_STATUS2_RESERVED1_MASK 0xfffffc00
#define  TSEN_REGS_STATUS2_RESERVED1_SHIFT 10
#define  TSEN_REGS_STATUS2_RESERVED1_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS2_RESERVED1_MASK) >> TSEN_REGS_STATUS2_RESERVED1_SHIFT)
#define  TSEN_REGS_STATUS2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS2_RESERVED1_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS2_RESERVED1_SHIFT))

#define  TSEN_REGS_STATUS2_T_VALID_MASK 0x200
#define  TSEN_REGS_STATUS2_T_VALID_SHIFT 9
#define  TSEN_REGS_STATUS2_T_VALID_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS2_T_VALID_MASK) >> TSEN_REGS_STATUS2_T_VALID_SHIFT)
#define  TSEN_REGS_STATUS2_T_VALID_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS2_T_VALID_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS2_T_VALID_SHIFT))

#define  TSEN_REGS_STATUS2_TEMP_OUT_MASK 0x1ff
#define  TSEN_REGS_STATUS2_TEMP_OUT_SHIFT 0
#define  TSEN_REGS_STATUS2_TEMP_OUT_MASK_SHIFT(reg) (((reg) & TSEN_REGS_STATUS2_TEMP_OUT_MASK) >> TSEN_REGS_STATUS2_TEMP_OUT_SHIFT)
#define  TSEN_REGS_STATUS2_TEMP_OUT_REPLACE_VAL(reg,val) (((reg) & ~TSEN_REGS_STATUS2_TEMP_OUT_MASK) | (((uint32_t)val) << TSEN_REGS_STATUS2_TEMP_OUT_SHIFT))

#endif // TSEN_REGS
