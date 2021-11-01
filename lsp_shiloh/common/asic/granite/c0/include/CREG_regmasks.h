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
 * \file CREG_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CREG_REG_REGMASKS_H_
#define _CREG_REG_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CREG Register Set (CREG_reg)
/** \brief The following registers are used to configure the Color Registration (CREG) block.*/
//
//====================================================================

//====================================================================
//Register: Color Registration Configuration Register (CRCR)
/** \brief This register configures the operation of the Color Registration block. The individual sensor inputs may be enabled and disabled. Each bit in the three bit enableSensor field corresponds to a sensor, with the least significant bit referring to CRSensor0. The edge measurement mode can be set to time between only rising edges, only falling edges, or between any edges. The deglitchValue and deglitchMode register values provide control over how to deglitch the sensor inputs. InterruptMode sets when the IRQ for CREG is asserted. Note that the IRQ will be asserted should any of the FIFO's 			assume the selected state. The value chosen for hold-off count determines how 			many samples from the most recent edge before the next edge can be detected.*/
//====================================================================

#define  CREG_REG_CRCR_HOLDOFFCOUNT_MASK 0xffff0000
#define  CREG_REG_CRCR_HOLDOFFCOUNT_SHIFT 16
#define  CREG_REG_CRCR_HOLDOFFCOUNT_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_HOLDOFFCOUNT_MASK) >> CREG_REG_CRCR_HOLDOFFCOUNT_SHIFT)
#define  CREG_REG_CRCR_HOLDOFFCOUNT_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_HOLDOFFCOUNT_MASK) | (((uint32_t)val) << CREG_REG_CRCR_HOLDOFFCOUNT_SHIFT))

#define  CREG_REG_CRCR_RESERVED1_MASK 0xf000
#define  CREG_REG_CRCR_RESERVED1_SHIFT 12
#define  CREG_REG_CRCR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_RESERVED1_MASK) >> CREG_REG_CRCR_RESERVED1_SHIFT)
#define  CREG_REG_CRCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRCR_RESERVED1_SHIFT))

#define  CREG_REG_CRCR_EDGEMODE_MASK 0xc00
#define  CREG_REG_CRCR_EDGEMODE_SHIFT 10
#define  CREG_REG_CRCR_EDGEMODE_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_EDGEMODE_MASK) >> CREG_REG_CRCR_EDGEMODE_SHIFT)
#define  CREG_REG_CRCR_EDGEMODE_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_EDGEMODE_MASK) | (((uint32_t)val) << CREG_REG_CRCR_EDGEMODE_SHIFT))

#define  CREG_REG_CRCR_TOP_SEL_MASK 0x300
#define  CREG_REG_CRCR_TOP_SEL_SHIFT 8
#define  CREG_REG_CRCR_TOP_SEL_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_TOP_SEL_MASK) >> CREG_REG_CRCR_TOP_SEL_SHIFT)
#define  CREG_REG_CRCR_TOP_SEL_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_TOP_SEL_MASK) | (((uint32_t)val) << CREG_REG_CRCR_TOP_SEL_SHIFT))

#define  CREG_REG_CRCR_TOP_CFG_MASK 0xc0
#define  CREG_REG_CRCR_TOP_CFG_SHIFT 6
#define  CREG_REG_CRCR_TOP_CFG_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_TOP_CFG_MASK) >> CREG_REG_CRCR_TOP_CFG_SHIFT)
#define  CREG_REG_CRCR_TOP_CFG_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_TOP_CFG_MASK) | (((uint32_t)val) << CREG_REG_CRCR_TOP_CFG_SHIFT))

#define  CREG_REG_CRCR_INTERRUPTMODE_MASK 0x30
#define  CREG_REG_CRCR_INTERRUPTMODE_SHIFT 4
#define  CREG_REG_CRCR_INTERRUPTMODE_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_INTERRUPTMODE_MASK) >> CREG_REG_CRCR_INTERRUPTMODE_SHIFT)
#define  CREG_REG_CRCR_INTERRUPTMODE_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_INTERRUPTMODE_MASK) | (((uint32_t)val) << CREG_REG_CRCR_INTERRUPTMODE_SHIFT))

#define  CREG_REG_CRCR_RESERVED2_MASK 0x8
#define  CREG_REG_CRCR_RESERVED2_SHIFT 3
#define  CREG_REG_CRCR_RESERVED2_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_RESERVED2_MASK) >> CREG_REG_CRCR_RESERVED2_SHIFT)
#define  CREG_REG_CRCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_RESERVED2_MASK) | (((uint32_t)val) << CREG_REG_CRCR_RESERVED2_SHIFT))

#define  CREG_REG_CRCR_ENABLESENSOR_MASK 0x7
#define  CREG_REG_CRCR_ENABLESENSOR_SHIFT 0
#define  CREG_REG_CRCR_ENABLESENSOR_MASK_SHIFT(reg) (((reg) & CREG_REG_CRCR_ENABLESENSOR_MASK) >> CREG_REG_CRCR_ENABLESENSOR_SHIFT)
#define  CREG_REG_CRCR_ENABLESENSOR_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRCR_ENABLESENSOR_MASK) | (((uint32_t)val) << CREG_REG_CRCR_ENABLESENSOR_SHIFT))

//====================================================================
//Register: CREG Filter Configuration Register (CFCR)
/** \brief The CREG opto-sensor input is passed through a digital filter. The digital filter works by adding a programmable value to a "score" count when a different input level is detected (i.e. when the filter output is low but a high input level is present). The Filter Score Threshold sets the score count level at which the filter output switches to the new value.*/
//====================================================================

#define  CREG_REG_CFCR_RESERVED1_MASK 0xff000000
#define  CREG_REG_CFCR_RESERVED1_SHIFT 24
#define  CREG_REG_CFCR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CFCR_RESERVED1_MASK) >> CREG_REG_CFCR_RESERVED1_SHIFT)
#define  CREG_REG_CFCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CFCR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CFCR_RESERVED1_SHIFT))

#define  CREG_REG_CFCR_FALL_HI_MASK 0xf00000
#define  CREG_REG_CFCR_FALL_HI_SHIFT 20
#define  CREG_REG_CFCR_FALL_HI_MASK_SHIFT(reg) (((reg) & CREG_REG_CFCR_FALL_HI_MASK) >> CREG_REG_CFCR_FALL_HI_SHIFT)
#define  CREG_REG_CFCR_FALL_HI_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CFCR_FALL_HI_MASK) | (((uint32_t)val) << CREG_REG_CFCR_FALL_HI_SHIFT))

#define  CREG_REG_CFCR_FALL_LO_MASK 0xf0000
#define  CREG_REG_CFCR_FALL_LO_SHIFT 16
#define  CREG_REG_CFCR_FALL_LO_MASK_SHIFT(reg) (((reg) & CREG_REG_CFCR_FALL_LO_MASK) >> CREG_REG_CFCR_FALL_LO_SHIFT)
#define  CREG_REG_CFCR_FALL_LO_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CFCR_FALL_LO_MASK) | (((uint32_t)val) << CREG_REG_CFCR_FALL_LO_SHIFT))

#define  CREG_REG_CFCR_RISE_LO_MASK 0xf000
#define  CREG_REG_CFCR_RISE_LO_SHIFT 12
#define  CREG_REG_CFCR_RISE_LO_MASK_SHIFT(reg) (((reg) & CREG_REG_CFCR_RISE_LO_MASK) >> CREG_REG_CFCR_RISE_LO_SHIFT)
#define  CREG_REG_CFCR_RISE_LO_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CFCR_RISE_LO_MASK) | (((uint32_t)val) << CREG_REG_CFCR_RISE_LO_SHIFT))

#define  CREG_REG_CFCR_RISE_HI_MASK 0xf00
#define  CREG_REG_CFCR_RISE_HI_SHIFT 8
#define  CREG_REG_CFCR_RISE_HI_MASK_SHIFT(reg) (((reg) & CREG_REG_CFCR_RISE_HI_MASK) >> CREG_REG_CFCR_RISE_HI_SHIFT)
#define  CREG_REG_CFCR_RISE_HI_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CFCR_RISE_HI_MASK) | (((uint32_t)val) << CREG_REG_CFCR_RISE_HI_SHIFT))

#define  CREG_REG_CFCR_THRESHOLD_MASK 0xff
#define  CREG_REG_CFCR_THRESHOLD_SHIFT 0
#define  CREG_REG_CFCR_THRESHOLD_MASK_SHIFT(reg) (((reg) & CREG_REG_CFCR_THRESHOLD_MASK) >> CREG_REG_CFCR_THRESHOLD_SHIFT)
#define  CREG_REG_CFCR_THRESHOLD_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CFCR_THRESHOLD_MASK) | (((uint32_t)val) << CREG_REG_CFCR_THRESHOLD_SHIFT))

//====================================================================
//Register: Color Registration Capture 0 Register (CAP0R)
/** \brief The CAP0R register contains the most recent measurement from the sensor 0 input. The 31-bit number represents the number of non-dithered DEC refclk cycles between the edges as specified by the configuration register.*/
//====================================================================

#define  CREG_REG_CAP0R_LASTWORD0_MASK 0x80000000
#define  CREG_REG_CAP0R_LASTWORD0_SHIFT 31
#define  CREG_REG_CAP0R_LASTWORD0_MASK_SHIFT(reg) (((reg) & CREG_REG_CAP0R_LASTWORD0_MASK) >> CREG_REG_CAP0R_LASTWORD0_SHIFT)
#define  CREG_REG_CAP0R_LASTWORD0_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CAP0R_LASTWORD0_MASK) | (((uint32_t)val) << CREG_REG_CAP0R_LASTWORD0_SHIFT))

#define  CREG_REG_CAP0R_COUNT0_MASK 0x7fffffff
#define  CREG_REG_CAP0R_COUNT0_SHIFT 0
#define  CREG_REG_CAP0R_COUNT0_MASK_SHIFT(reg) (((reg) & CREG_REG_CAP0R_COUNT0_MASK) >> CREG_REG_CAP0R_COUNT0_SHIFT)
#define  CREG_REG_CAP0R_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CAP0R_COUNT0_MASK) | (((uint32_t)val) << CREG_REG_CAP0R_COUNT0_SHIFT))

//====================================================================
//Register: Color Registration Capture 1 Register (CAP1R)
/** \brief The CAP1R register contains the most recent measurement from the sensor 1 input. The 31-bit number represents the number of non-dithered DEC refclk cycles between the edges as specified by the configuration register.*/
//====================================================================

#define  CREG_REG_CAP1R_LASTWORD1_MASK 0x80000000
#define  CREG_REG_CAP1R_LASTWORD1_SHIFT 31
#define  CREG_REG_CAP1R_LASTWORD1_MASK_SHIFT(reg) (((reg) & CREG_REG_CAP1R_LASTWORD1_MASK) >> CREG_REG_CAP1R_LASTWORD1_SHIFT)
#define  CREG_REG_CAP1R_LASTWORD1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CAP1R_LASTWORD1_MASK) | (((uint32_t)val) << CREG_REG_CAP1R_LASTWORD1_SHIFT))

#define  CREG_REG_CAP1R_COUNT1_MASK 0x7fffffff
#define  CREG_REG_CAP1R_COUNT1_SHIFT 0
#define  CREG_REG_CAP1R_COUNT1_MASK_SHIFT(reg) (((reg) & CREG_REG_CAP1R_COUNT1_MASK) >> CREG_REG_CAP1R_COUNT1_SHIFT)
#define  CREG_REG_CAP1R_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CAP1R_COUNT1_MASK) | (((uint32_t)val) << CREG_REG_CAP1R_COUNT1_SHIFT))

//====================================================================
//Register: Color Registration Capture 2 Register (CAP2R)
/** \brief The CAP2R register contains the most recent measurement from the sensor 2 input. The 31-bit number represents the number of non-dithered DEC refclk cycles between the edges as specified by the configuration register.*/
//====================================================================

#define  CREG_REG_CAP2R_LASTWORD2_MASK 0x80000000
#define  CREG_REG_CAP2R_LASTWORD2_SHIFT 31
#define  CREG_REG_CAP2R_LASTWORD2_MASK_SHIFT(reg) (((reg) & CREG_REG_CAP2R_LASTWORD2_MASK) >> CREG_REG_CAP2R_LASTWORD2_SHIFT)
#define  CREG_REG_CAP2R_LASTWORD2_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CAP2R_LASTWORD2_MASK) | (((uint32_t)val) << CREG_REG_CAP2R_LASTWORD2_SHIFT))

#define  CREG_REG_CAP2R_COUNT2_MASK 0x7fffffff
#define  CREG_REG_CAP2R_COUNT2_SHIFT 0
#define  CREG_REG_CAP2R_COUNT2_MASK_SHIFT(reg) (((reg) & CREG_REG_CAP2R_COUNT2_MASK) >> CREG_REG_CAP2R_COUNT2_SHIFT)
#define  CREG_REG_CAP2R_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CAP2R_COUNT2_MASK) | (((uint32_t)val) << CREG_REG_CAP2R_COUNT2_SHIFT))

//====================================================================
//Register: Color Registration Status Register (CRSR)
/** \brief This register contains the current values for each of the three sensor inputs. It also contains the FIFO status values. The values are arranged in 3-bit fields with the least-significant bit of each field corresponding to sensor0 (for current value) or FIFO0 (else).*/
//====================================================================

#define  CREG_REG_CRSR_RESERVED1_MASK 0xff800000
#define  CREG_REG_CRSR_RESERVED1_SHIFT 23
#define  CREG_REG_CRSR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_RESERVED1_MASK) >> CREG_REG_CRSR_RESERVED1_SHIFT)
#define  CREG_REG_CRSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRSR_RESERVED1_SHIFT))

#define  CREG_REG_CRSR_FIFOOVERFLOW_MASK 0x700000
#define  CREG_REG_CRSR_FIFOOVERFLOW_SHIFT 20
#define  CREG_REG_CRSR_FIFOOVERFLOW_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_FIFOOVERFLOW_MASK) >> CREG_REG_CRSR_FIFOOVERFLOW_SHIFT)
#define  CREG_REG_CRSR_FIFOOVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_FIFOOVERFLOW_MASK) | (((uint32_t)val) << CREG_REG_CRSR_FIFOOVERFLOW_SHIFT))

#define  CREG_REG_CRSR_RESERVED2_MASK 0x80000
#define  CREG_REG_CRSR_RESERVED2_SHIFT 19
#define  CREG_REG_CRSR_RESERVED2_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_RESERVED2_MASK) >> CREG_REG_CRSR_RESERVED2_SHIFT)
#define  CREG_REG_CRSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_RESERVED2_MASK) | (((uint32_t)val) << CREG_REG_CRSR_RESERVED2_SHIFT))

#define  CREG_REG_CRSR_FIFOFULL_MASK 0x70000
#define  CREG_REG_CRSR_FIFOFULL_SHIFT 16
#define  CREG_REG_CRSR_FIFOFULL_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_FIFOFULL_MASK) >> CREG_REG_CRSR_FIFOFULL_SHIFT)
#define  CREG_REG_CRSR_FIFOFULL_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_FIFOFULL_MASK) | (((uint32_t)val) << CREG_REG_CRSR_FIFOFULL_SHIFT))

#define  CREG_REG_CRSR_RESERVED3_MASK 0x8000
#define  CREG_REG_CRSR_RESERVED3_SHIFT 15
#define  CREG_REG_CRSR_RESERVED3_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_RESERVED3_MASK) >> CREG_REG_CRSR_RESERVED3_SHIFT)
#define  CREG_REG_CRSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_RESERVED3_MASK) | (((uint32_t)val) << CREG_REG_CRSR_RESERVED3_SHIFT))

#define  CREG_REG_CRSR_FIFOALMOSTFULL_MASK 0x7000
#define  CREG_REG_CRSR_FIFOALMOSTFULL_SHIFT 12
#define  CREG_REG_CRSR_FIFOALMOSTFULL_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_FIFOALMOSTFULL_MASK) >> CREG_REG_CRSR_FIFOALMOSTFULL_SHIFT)
#define  CREG_REG_CRSR_FIFOALMOSTFULL_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_FIFOALMOSTFULL_MASK) | (((uint32_t)val) << CREG_REG_CRSR_FIFOALMOSTFULL_SHIFT))

#define  CREG_REG_CRSR_RESERVED4_MASK 0x800
#define  CREG_REG_CRSR_RESERVED4_SHIFT 11
#define  CREG_REG_CRSR_RESERVED4_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_RESERVED4_MASK) >> CREG_REG_CRSR_RESERVED4_SHIFT)
#define  CREG_REG_CRSR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_RESERVED4_MASK) | (((uint32_t)val) << CREG_REG_CRSR_RESERVED4_SHIFT))

#define  CREG_REG_CRSR_FIFOALMOSTEMPTY_MASK 0x700
#define  CREG_REG_CRSR_FIFOALMOSTEMPTY_SHIFT 8
#define  CREG_REG_CRSR_FIFOALMOSTEMPTY_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_FIFOALMOSTEMPTY_MASK) >> CREG_REG_CRSR_FIFOALMOSTEMPTY_SHIFT)
#define  CREG_REG_CRSR_FIFOALMOSTEMPTY_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_FIFOALMOSTEMPTY_MASK) | (((uint32_t)val) << CREG_REG_CRSR_FIFOALMOSTEMPTY_SHIFT))

#define  CREG_REG_CRSR_RESERVED5_MASK 0x80
#define  CREG_REG_CRSR_RESERVED5_SHIFT 7
#define  CREG_REG_CRSR_RESERVED5_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_RESERVED5_MASK) >> CREG_REG_CRSR_RESERVED5_SHIFT)
#define  CREG_REG_CRSR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_RESERVED5_MASK) | (((uint32_t)val) << CREG_REG_CRSR_RESERVED5_SHIFT))

#define  CREG_REG_CRSR_FIFOEMPTY_MASK 0x70
#define  CREG_REG_CRSR_FIFOEMPTY_SHIFT 4
#define  CREG_REG_CRSR_FIFOEMPTY_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_FIFOEMPTY_MASK) >> CREG_REG_CRSR_FIFOEMPTY_SHIFT)
#define  CREG_REG_CRSR_FIFOEMPTY_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_FIFOEMPTY_MASK) | (((uint32_t)val) << CREG_REG_CRSR_FIFOEMPTY_SHIFT))

#define  CREG_REG_CRSR_RESERVED6_MASK 0x8
#define  CREG_REG_CRSR_RESERVED6_SHIFT 3
#define  CREG_REG_CRSR_RESERVED6_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_RESERVED6_MASK) >> CREG_REG_CRSR_RESERVED6_SHIFT)
#define  CREG_REG_CRSR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_RESERVED6_MASK) | (((uint32_t)val) << CREG_REG_CRSR_RESERVED6_SHIFT))

#define  CREG_REG_CRSR_SENSORVALUE_MASK 0x7
#define  CREG_REG_CRSR_SENSORVALUE_SHIFT 0
#define  CREG_REG_CRSR_SENSORVALUE_MASK_SHIFT(reg) (((reg) & CREG_REG_CRSR_SENSORVALUE_MASK) >> CREG_REG_CRSR_SENSORVALUE_SHIFT)
#define  CREG_REG_CRSR_SENSORVALUE_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRSR_SENSORVALUE_MASK) | (((uint32_t)val) << CREG_REG_CRSR_SENSORVALUE_SHIFT))

//====================================================================
//Register: Color Registration Time Synchronization Register (CRTSR)
/** \brief This register provides a single frame of reference for all three timers. When written, it will clear the values that are stored in both the capture registers for each input. It will then, in unison, start each of the three timers.*/
//====================================================================

#define  CREG_REG_CRTSR_RESERVED1_MASK 0xfffffffe
#define  CREG_REG_CRTSR_RESERVED1_SHIFT 1
#define  CREG_REG_CRTSR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRTSR_RESERVED1_MASK) >> CREG_REG_CRTSR_RESERVED1_SHIFT)
#define  CREG_REG_CRTSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRTSR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRTSR_RESERVED1_SHIFT))

#define  CREG_REG_CRTSR_STARTMEASUREMENT_MASK 0x1
#define  CREG_REG_CRTSR_STARTMEASUREMENT_SHIFT 0
#define  CREG_REG_CRTSR_STARTMEASUREMENT_MASK_SHIFT(reg) (((reg) & CREG_REG_CRTSR_STARTMEASUREMENT_MASK) >> CREG_REG_CRTSR_STARTMEASUREMENT_SHIFT)
#define  CREG_REG_CRTSR_STARTMEASUREMENT_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRTSR_STARTMEASUREMENT_MASK) | (((uint32_t)val) << CREG_REG_CRTSR_STARTMEASUREMENT_SHIFT))

//====================================================================
//Register: Color Registration FIFO Pop Register (CRFPR)
/** \brief This register is used to pop the FIFO outputs. For east of test, performing a read of a FIFO outputs stored at CAP0R through CAP2R does not also pop the FIFO. FIFO0 captures counts for CRSensor0, FIFO1 for CRSensor1, and FIFO2 for CRSensor2.*/
//====================================================================

#define  CREG_REG_CRFPR_RESERVED1_MASK 0xfffffff8
#define  CREG_REG_CRFPR_RESERVED1_SHIFT 3
#define  CREG_REG_CRFPR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRFPR_RESERVED1_MASK) >> CREG_REG_CRFPR_RESERVED1_SHIFT)
#define  CREG_REG_CRFPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRFPR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRFPR_RESERVED1_SHIFT))

#define  CREG_REG_CRFPR_POPFIFO_MASK 0x7
#define  CREG_REG_CRFPR_POPFIFO_SHIFT 0
#define  CREG_REG_CRFPR_POPFIFO_MASK_SHIFT(reg) (((reg) & CREG_REG_CRFPR_POPFIFO_MASK) >> CREG_REG_CRFPR_POPFIFO_SHIFT)
#define  CREG_REG_CRFPR_POPFIFO_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRFPR_POPFIFO_MASK) | (((uint32_t)val) << CREG_REG_CRFPR_POPFIFO_SHIFT))

//====================================================================
//Register: Color Registration Interrupt Enable Register (CRIER)
/** \brief This register enables interrupts for the sensor inputs. The least significant bit corresponds to CRSensor0 and the most significant bit corresponds to CRSensor2.*/
//====================================================================

#define  CREG_REG_CRIER_RESERVED1_MASK 0xfffffff8
#define  CREG_REG_CRIER_RESERVED1_SHIFT 3
#define  CREG_REG_CRIER_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRIER_RESERVED1_MASK) >> CREG_REG_CRIER_RESERVED1_SHIFT)
#define  CREG_REG_CRIER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRIER_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRIER_RESERVED1_SHIFT))

#define  CREG_REG_CRIER_ENABLEINT_MASK 0x7
#define  CREG_REG_CRIER_ENABLEINT_SHIFT 0
#define  CREG_REG_CRIER_ENABLEINT_MASK_SHIFT(reg) (((reg) & CREG_REG_CRIER_ENABLEINT_MASK) >> CREG_REG_CRIER_ENABLEINT_SHIFT)
#define  CREG_REG_CRIER_ENABLEINT_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRIER_ENABLEINT_MASK) | (((uint32_t)val) << CREG_REG_CRIER_ENABLEINT_SHIFT))

//====================================================================
//Register: Color Registration Interrupt Pending Register (CRIPR)
/** \brief This register indicates pending interrupts for the sensor inputs. The least significant bit corresponds to CRSensor0 and the most significant bit corresponds to CRSensor2.*/
//====================================================================

#define  CREG_REG_CRIPR_RESERVED1_MASK 0xfffffff8
#define  CREG_REG_CRIPR_RESERVED1_SHIFT 3
#define  CREG_REG_CRIPR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRIPR_RESERVED1_MASK) >> CREG_REG_CRIPR_RESERVED1_SHIFT)
#define  CREG_REG_CRIPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRIPR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRIPR_RESERVED1_SHIFT))

#define  CREG_REG_CRIPR_PENDINGINT_MASK 0x7
#define  CREG_REG_CRIPR_PENDINGINT_SHIFT 0
#define  CREG_REG_CRIPR_PENDINGINT_MASK_SHIFT(reg) (((reg) & CREG_REG_CRIPR_PENDINGINT_MASK) >> CREG_REG_CRIPR_PENDINGINT_SHIFT)
#define  CREG_REG_CRIPR_PENDINGINT_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRIPR_PENDINGINT_MASK) | (((uint32_t)val) << CREG_REG_CRIPR_PENDINGINT_SHIFT))

//====================================================================
//Register: Color Registration Interrupt Clear Register (CRICR)
/** \brief Write to this register to clear the corresponding interrupts. The least significant bit corresponds to CRSensor0 and the most significant bit corresponds to CRSensor2.*/
//====================================================================

#define  CREG_REG_CRICR_RESERVED1_MASK 0xfffffff8
#define  CREG_REG_CRICR_RESERVED1_SHIFT 3
#define  CREG_REG_CRICR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRICR_RESERVED1_MASK) >> CREG_REG_CRICR_RESERVED1_SHIFT)
#define  CREG_REG_CRICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRICR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRICR_RESERVED1_SHIFT))

#define  CREG_REG_CRICR_CLEARINT_MASK 0x7
#define  CREG_REG_CRICR_CLEARINT_SHIFT 0
#define  CREG_REG_CRICR_CLEARINT_MASK_SHIFT(reg) (((reg) & CREG_REG_CRICR_CLEARINT_MASK) >> CREG_REG_CRICR_CLEARINT_SHIFT)
#define  CREG_REG_CRICR_CLEARINT_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRICR_CLEARINT_MASK) | (((uint32_t)val) << CREG_REG_CRICR_CLEARINT_SHIFT))

//====================================================================
//Register: Color Registration Interrupt Force Register (CRIFR)
/** \brief Write to this register to force an interrupt. The least significant bit corresponds to CRSensor0 and the most significant bit corresponds to CRSensor2.*/
//====================================================================

#define  CREG_REG_CRIFR_RESERVED1_MASK 0xfffffff8
#define  CREG_REG_CRIFR_RESERVED1_SHIFT 3
#define  CREG_REG_CRIFR_RESERVED1_MASK_SHIFT(reg) (((reg) & CREG_REG_CRIFR_RESERVED1_MASK) >> CREG_REG_CRIFR_RESERVED1_SHIFT)
#define  CREG_REG_CRIFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRIFR_RESERVED1_MASK) | (((uint32_t)val) << CREG_REG_CRIFR_RESERVED1_SHIFT))

#define  CREG_REG_CRIFR_FORCEINT_MASK 0x7
#define  CREG_REG_CRIFR_FORCEINT_SHIFT 0
#define  CREG_REG_CRIFR_FORCEINT_MASK_SHIFT(reg) (((reg) & CREG_REG_CRIFR_FORCEINT_MASK) >> CREG_REG_CRIFR_FORCEINT_SHIFT)
#define  CREG_REG_CRIFR_FORCEINT_REPLACE_VAL(reg,val) (((reg) & ~CREG_REG_CRIFR_FORCEINT_MASK) | (((uint32_t)val) << CREG_REG_CRIFR_FORCEINT_SHIFT))

#endif // CREG_REG
