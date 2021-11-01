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
 * \file VCF_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_REGMASKS_H_
#define _VCF_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF Registers (VCF)
//
//====================================================================

//====================================================================
//
//Register File: CORE (CORE)
//
//====================================================================

//====================================================================
//Register: VCF_STATUS (VCF_STATUS)
//====================================================================

#define  CORE_VCF_STATUS_VALUE_MASK 0xffffffff
#define  CORE_VCF_STATUS_VALUE_SHIFT 0
#define  CORE_VCF_STATUS_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_STATUS_VALUE_MASK) >> CORE_VCF_STATUS_VALUE_SHIFT)
#define  CORE_VCF_STATUS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_STATUS_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_STATUS_VALUE_SHIFT))

//====================================================================
//Register: VCF_ENFORCE (VCF_ENFORCE)
//====================================================================

#define  CORE_VCF_ENFORCE_VALUE_MASK 0xffffffff
#define  CORE_VCF_ENFORCE_VALUE_SHIFT 0
#define  CORE_VCF_ENFORCE_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_ENFORCE_VALUE_MASK) >> CORE_VCF_ENFORCE_VALUE_SHIFT)
#define  CORE_VCF_ENFORCE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_ENFORCE_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_ENFORCE_VALUE_SHIFT))

//====================================================================
//Register: VCF_PRE_ENFORCE (VCF_PRE_ENFORCE)
//====================================================================

#define  CORE_VCF_PRE_ENFORCE_VALUE_MASK 0xffffffff
#define  CORE_VCF_PRE_ENFORCE_VALUE_SHIFT 0
#define  CORE_VCF_PRE_ENFORCE_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_PRE_ENFORCE_VALUE_MASK) >> CORE_VCF_PRE_ENFORCE_VALUE_SHIFT)
#define  CORE_VCF_PRE_ENFORCE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_PRE_ENFORCE_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_PRE_ENFORCE_VALUE_SHIFT))

//====================================================================
//Register: VCF_TRIGGERS (VCF_TRIGGERS)
//====================================================================

#define  CORE_VCF_TRIGGERS_VALUE_MASK 0xffffffff
#define  CORE_VCF_TRIGGERS_VALUE_SHIFT 0
#define  CORE_VCF_TRIGGERS_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_TRIGGERS_VALUE_MASK) >> CORE_VCF_TRIGGERS_VALUE_SHIFT)
#define  CORE_VCF_TRIGGERS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_TRIGGERS_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_TRIGGERS_VALUE_SHIFT))

//====================================================================
//Register: VCF_ERROR (VCF_ERROR)
//====================================================================

#define  CORE_VCF_ERROR_VALUE_MASK 0xffffffff
#define  CORE_VCF_ERROR_VALUE_SHIFT 0
#define  CORE_VCF_ERROR_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_ERROR_VALUE_MASK) >> CORE_VCF_ERROR_VALUE_SHIFT)
#define  CORE_VCF_ERROR_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_ERROR_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_ERROR_VALUE_SHIFT))

//====================================================================
//Register: VCF_IRQ_SOURCE (VCF_IRQ_SOURCE)
//====================================================================

#define  CORE_VCF_IRQ_SOURCE_VALUE_MASK 0xffffffff
#define  CORE_VCF_IRQ_SOURCE_VALUE_SHIFT 0
#define  CORE_VCF_IRQ_SOURCE_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_IRQ_SOURCE_VALUE_MASK) >> CORE_VCF_IRQ_SOURCE_VALUE_SHIFT)
#define  CORE_VCF_IRQ_SOURCE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_IRQ_SOURCE_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_IRQ_SOURCE_VALUE_SHIFT))

//====================================================================
//Register: VCF_PIN_MIRROR (VCF_PIN_MIRROR)
//====================================================================

#define  CORE_VCF_PIN_MIRROR_VALUE_MASK 0xffffffff
#define  CORE_VCF_PIN_MIRROR_VALUE_SHIFT 0
#define  CORE_VCF_PIN_MIRROR_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_PIN_MIRROR_VALUE_MASK) >> CORE_VCF_PIN_MIRROR_VALUE_SHIFT)
#define  CORE_VCF_PIN_MIRROR_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_PIN_MIRROR_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_PIN_MIRROR_VALUE_SHIFT))

//====================================================================
//Register: VCF_PRNG (VCF_PRNG)
//====================================================================

#define  CORE_VCF_PRNG_VALUE_MASK 0xffffffff
#define  CORE_VCF_PRNG_VALUE_SHIFT 0
#define  CORE_VCF_PRNG_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_PRNG_VALUE_MASK) >> CORE_VCF_PRNG_VALUE_SHIFT)
#define  CORE_VCF_PRNG_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_PRNG_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_PRNG_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 1 of 4) (VCF_FUSE0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  CORE_VCF_FUSE0_VALUE_MASK 0xffffffff
#define  CORE_VCF_FUSE0_VALUE_SHIFT 0
#define  CORE_VCF_FUSE0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FUSE0_VALUE_MASK) >> CORE_VCF_FUSE0_VALUE_SHIFT)
#define  CORE_VCF_FUSE0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FUSE0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FUSE0_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 2 of 4) (VCF_FUSE1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  CORE_VCF_FUSE1_VALUE_MASK 0xffffffff
#define  CORE_VCF_FUSE1_VALUE_SHIFT 0
#define  CORE_VCF_FUSE1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FUSE1_VALUE_MASK) >> CORE_VCF_FUSE1_VALUE_SHIFT)
#define  CORE_VCF_FUSE1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FUSE1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FUSE1_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 3 of 4) (VCF_FUSE2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  CORE_VCF_FUSE2_VALUE_MASK 0xffffffff
#define  CORE_VCF_FUSE2_VALUE_SHIFT 0
#define  CORE_VCF_FUSE2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FUSE2_VALUE_MASK) >> CORE_VCF_FUSE2_VALUE_SHIFT)
#define  CORE_VCF_FUSE2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FUSE2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FUSE2_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 4 of 4) (VCF_FUSE3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  CORE_VCF_FUSE3_VALUE_MASK 0xffffffff
#define  CORE_VCF_FUSE3_VALUE_SHIFT 0
#define  CORE_VCF_FUSE3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FUSE3_VALUE_MASK) >> CORE_VCF_FUSE3_VALUE_SHIFT)
#define  CORE_VCF_FUSE3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FUSE3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FUSE3_VALUE_SHIFT))

//====================================================================
//Register: VCF_EVENT (VCF_EVENT)
//====================================================================

#define  CORE_VCF_EVENT_VALUE_MASK 0xffffffff
#define  CORE_VCF_EVENT_VALUE_SHIFT 0
#define  CORE_VCF_EVENT_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_EVENT_VALUE_MASK) >> CORE_VCF_EVENT_VALUE_SHIFT)
#define  CORE_VCF_EVENT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_EVENT_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_EVENT_VALUE_SHIFT))

//====================================================================
//Register: VCF_TIMERS (VCF_TIMERS)
//====================================================================

#define  CORE_VCF_TIMERS_VALUE_MASK 0xffffffff
#define  CORE_VCF_TIMERS_VALUE_SHIFT 0
#define  CORE_VCF_TIMERS_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_TIMERS_VALUE_MASK) >> CORE_VCF_TIMERS_VALUE_SHIFT)
#define  CORE_VCF_TIMERS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_TIMERS_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_TIMERS_VALUE_SHIFT))

//====================================================================
//Register: VCF_VERSION (VCF_VERSION)
//====================================================================

#define  CORE_VCF_VERSION_VALUE_MASK 0xffffffff
#define  CORE_VCF_VERSION_VALUE_SHIFT 0
#define  CORE_VCF_VERSION_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VERSION_VALUE_MASK) >> CORE_VCF_VERSION_VALUE_SHIFT)
#define  CORE_VCF_VERSION_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VERSION_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VERSION_VALUE_SHIFT))

//====================================================================
//Register: VCF_RECON_DATA (Instance 1 of 3) (VCF_RECON_DATA0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  CORE_VCF_RECON_DATA0_VALUE_MASK 0xffffffff
#define  CORE_VCF_RECON_DATA0_VALUE_SHIFT 0
#define  CORE_VCF_RECON_DATA0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_RECON_DATA0_VALUE_MASK) >> CORE_VCF_RECON_DATA0_VALUE_SHIFT)
#define  CORE_VCF_RECON_DATA0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_RECON_DATA0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_RECON_DATA0_VALUE_SHIFT))

//====================================================================
//Register: VCF_RECON_DATA (Instance 2 of 3) (VCF_RECON_DATA1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  CORE_VCF_RECON_DATA1_VALUE_MASK 0xffffffff
#define  CORE_VCF_RECON_DATA1_VALUE_SHIFT 0
#define  CORE_VCF_RECON_DATA1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_RECON_DATA1_VALUE_MASK) >> CORE_VCF_RECON_DATA1_VALUE_SHIFT)
#define  CORE_VCF_RECON_DATA1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_RECON_DATA1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_RECON_DATA1_VALUE_SHIFT))

//====================================================================
//Register: VCF_RECON_DATA (Instance 3 of 3) (VCF_RECON_DATA2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  CORE_VCF_RECON_DATA2_VALUE_MASK 0xffffffff
#define  CORE_VCF_RECON_DATA2_VALUE_SHIFT 0
#define  CORE_VCF_RECON_DATA2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_RECON_DATA2_VALUE_MASK) >> CORE_VCF_RECON_DATA2_VALUE_SHIFT)
#define  CORE_VCF_RECON_DATA2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_RECON_DATA2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_RECON_DATA2_VALUE_SHIFT))

//====================================================================
//Register: VCF_INPUT (VCF_INPUT)
//====================================================================

#define  CORE_VCF_INPUT_VALUE_MASK 0xffffffff
#define  CORE_VCF_INPUT_VALUE_SHIFT 0
#define  CORE_VCF_INPUT_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_INPUT_VALUE_MASK) >> CORE_VCF_INPUT_VALUE_SHIFT)
#define  CORE_VCF_INPUT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_INPUT_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_INPUT_VALUE_SHIFT))

//====================================================================
//Register: VCF_OUTPUT (VCF_OUTPUT)
//====================================================================

#define  CORE_VCF_OUTPUT_VALUE_MASK 0xffffffff
#define  CORE_VCF_OUTPUT_VALUE_SHIFT 0
#define  CORE_VCF_OUTPUT_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_OUTPUT_VALUE_MASK) >> CORE_VCF_OUTPUT_VALUE_SHIFT)
#define  CORE_VCF_OUTPUT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_OUTPUT_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_OUTPUT_VALUE_SHIFT))

//====================================================================
//Register: VCF_IRQ_MASK (VCF_IRQ_MASK)
//====================================================================

#define  CORE_VCF_IRQ_MASK_VALUE_MASK 0xffffffff
#define  CORE_VCF_IRQ_MASK_VALUE_SHIFT 0
#define  CORE_VCF_IRQ_MASK_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_IRQ_MASK_VALUE_MASK) >> CORE_VCF_IRQ_MASK_VALUE_SHIFT)
#define  CORE_VCF_IRQ_MASK_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_IRQ_MASK_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_IRQ_MASK_VALUE_SHIFT))

//====================================================================
//Register: VCF_IRQ_STATE (VCF_IRQ_STATE)
//====================================================================

#define  CORE_VCF_IRQ_STATE_VALUE_MASK 0xffffffff
#define  CORE_VCF_IRQ_STATE_VALUE_SHIFT 0
#define  CORE_VCF_IRQ_STATE_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_IRQ_STATE_VALUE_MASK) >> CORE_VCF_IRQ_STATE_VALUE_SHIFT)
#define  CORE_VCF_IRQ_STATE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_IRQ_STATE_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_IRQ_STATE_VALUE_SHIFT))

//====================================================================
//Register: VCF_MAINT_WINDOW (VCF_MAINT_WINDOW)
//====================================================================

#define  CORE_VCF_MAINT_WINDOW_VALUE_MASK 0xffffffff
#define  CORE_VCF_MAINT_WINDOW_VALUE_SHIFT 0
#define  CORE_VCF_MAINT_WINDOW_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_MAINT_WINDOW_VALUE_MASK) >> CORE_VCF_MAINT_WINDOW_VALUE_SHIFT)
#define  CORE_VCF_MAINT_WINDOW_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_MAINT_WINDOW_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_MAINT_WINDOW_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_ADJ (VCF_WARN_ADJ)
//====================================================================

#define  CORE_VCF_WARN_ADJ_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_ADJ_VALUE_SHIFT 0
#define  CORE_VCF_WARN_ADJ_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_ADJ_VALUE_MASK) >> CORE_VCF_WARN_ADJ_VALUE_SHIFT)
#define  CORE_VCF_WARN_ADJ_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_ADJ_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_ADJ_VALUE_SHIFT))

//====================================================================
//Register: VCF_DIAG (VCF_DIAG)
//====================================================================

#define  CORE_VCF_DIAG_VALUE_MASK 0xffffffff
#define  CORE_VCF_DIAG_VALUE_SHIFT 0
#define  CORE_VCF_DIAG_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_DIAG_VALUE_MASK) >> CORE_VCF_DIAG_VALUE_SHIFT)
#define  CORE_VCF_DIAG_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_DIAG_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_DIAG_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT0 (Instance 1 of 3) (VCF_FCERT00)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  CORE_VCF_FCERT00_VALUE_MASK 0xffffffff
#define  CORE_VCF_FCERT00_VALUE_SHIFT 0
#define  CORE_VCF_FCERT00_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FCERT00_VALUE_MASK) >> CORE_VCF_FCERT00_VALUE_SHIFT)
#define  CORE_VCF_FCERT00_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FCERT00_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FCERT00_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT0 (Instance 2 of 3) (VCF_FCERT01)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  CORE_VCF_FCERT01_VALUE_MASK 0xffffffff
#define  CORE_VCF_FCERT01_VALUE_SHIFT 0
#define  CORE_VCF_FCERT01_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FCERT01_VALUE_MASK) >> CORE_VCF_FCERT01_VALUE_SHIFT)
#define  CORE_VCF_FCERT01_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FCERT01_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FCERT01_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT0 (Instance 3 of 3) (VCF_FCERT02)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  CORE_VCF_FCERT02_VALUE_MASK 0xffffffff
#define  CORE_VCF_FCERT02_VALUE_SHIFT 0
#define  CORE_VCF_FCERT02_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FCERT02_VALUE_MASK) >> CORE_VCF_FCERT02_VALUE_SHIFT)
#define  CORE_VCF_FCERT02_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FCERT02_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FCERT02_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT1 (VCF_FCERT1)
//====================================================================

#define  CORE_VCF_FCERT1_VALUE_MASK 0xffffffff
#define  CORE_VCF_FCERT1_VALUE_SHIFT 0
#define  CORE_VCF_FCERT1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_FCERT1_VALUE_MASK) >> CORE_VCF_FCERT1_VALUE_SHIFT)
#define  CORE_VCF_FCERT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_FCERT1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_FCERT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 1 of 7) (VCF_CCERT00)
/** \brief  (Instance 1 of 7)*/
//====================================================================

#define  CORE_VCF_CCERT00_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT00_VALUE_SHIFT 0
#define  CORE_VCF_CCERT00_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT00_VALUE_MASK) >> CORE_VCF_CCERT00_VALUE_SHIFT)
#define  CORE_VCF_CCERT00_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT00_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT00_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 2 of 7) (VCF_CCERT01)
/** \brief  (Instance 2 of 7)*/
//====================================================================

#define  CORE_VCF_CCERT01_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT01_VALUE_SHIFT 0
#define  CORE_VCF_CCERT01_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT01_VALUE_MASK) >> CORE_VCF_CCERT01_VALUE_SHIFT)
#define  CORE_VCF_CCERT01_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT01_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT01_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 3 of 7) (VCF_CCERT02)
/** \brief  (Instance 3 of 7)*/
//====================================================================

#define  CORE_VCF_CCERT02_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT02_VALUE_SHIFT 0
#define  CORE_VCF_CCERT02_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT02_VALUE_MASK) >> CORE_VCF_CCERT02_VALUE_SHIFT)
#define  CORE_VCF_CCERT02_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT02_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT02_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 4 of 7) (VCF_CCERT03)
/** \brief  (Instance 4 of 7)*/
//====================================================================

#define  CORE_VCF_CCERT03_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT03_VALUE_SHIFT 0
#define  CORE_VCF_CCERT03_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT03_VALUE_MASK) >> CORE_VCF_CCERT03_VALUE_SHIFT)
#define  CORE_VCF_CCERT03_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT03_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT03_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 5 of 7) (VCF_CCERT04)
/** \brief  (Instance 5 of 7)*/
//====================================================================

#define  CORE_VCF_CCERT04_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT04_VALUE_SHIFT 0
#define  CORE_VCF_CCERT04_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT04_VALUE_MASK) >> CORE_VCF_CCERT04_VALUE_SHIFT)
#define  CORE_VCF_CCERT04_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT04_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT04_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 6 of 7) (VCF_CCERT05)
/** \brief  (Instance 6 of 7)*/
//====================================================================

#define  CORE_VCF_CCERT05_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT05_VALUE_SHIFT 0
#define  CORE_VCF_CCERT05_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT05_VALUE_MASK) >> CORE_VCF_CCERT05_VALUE_SHIFT)
#define  CORE_VCF_CCERT05_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT05_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT05_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 7 of 7) (VCF_CCERT06)
/** \brief  (Instance 7 of 7)*/
//====================================================================

#define  CORE_VCF_CCERT06_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT06_VALUE_SHIFT 0
#define  CORE_VCF_CCERT06_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT06_VALUE_MASK) >> CORE_VCF_CCERT06_VALUE_SHIFT)
#define  CORE_VCF_CCERT06_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT06_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT06_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 1 of 13) (VCF_CCERT10)
/** \brief  (Instance 1 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT10_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT10_VALUE_SHIFT 0
#define  CORE_VCF_CCERT10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT10_VALUE_MASK) >> CORE_VCF_CCERT10_VALUE_SHIFT)
#define  CORE_VCF_CCERT10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT10_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 2 of 13) (VCF_CCERT11)
/** \brief  (Instance 2 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT11_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT11_VALUE_SHIFT 0
#define  CORE_VCF_CCERT11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT11_VALUE_MASK) >> CORE_VCF_CCERT11_VALUE_SHIFT)
#define  CORE_VCF_CCERT11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT11_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 3 of 13) (VCF_CCERT12)
/** \brief  (Instance 3 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT12_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT12_VALUE_SHIFT 0
#define  CORE_VCF_CCERT12_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT12_VALUE_MASK) >> CORE_VCF_CCERT12_VALUE_SHIFT)
#define  CORE_VCF_CCERT12_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT12_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT12_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 4 of 13) (VCF_CCERT13)
/** \brief  (Instance 4 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT13_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT13_VALUE_SHIFT 0
#define  CORE_VCF_CCERT13_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT13_VALUE_MASK) >> CORE_VCF_CCERT13_VALUE_SHIFT)
#define  CORE_VCF_CCERT13_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT13_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT13_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 5 of 13) (VCF_CCERT14)
/** \brief  (Instance 5 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT14_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT14_VALUE_SHIFT 0
#define  CORE_VCF_CCERT14_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT14_VALUE_MASK) >> CORE_VCF_CCERT14_VALUE_SHIFT)
#define  CORE_VCF_CCERT14_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT14_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT14_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 6 of 13) (VCF_CCERT15)
/** \brief  (Instance 6 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT15_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT15_VALUE_SHIFT 0
#define  CORE_VCF_CCERT15_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT15_VALUE_MASK) >> CORE_VCF_CCERT15_VALUE_SHIFT)
#define  CORE_VCF_CCERT15_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT15_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT15_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 7 of 13) (VCF_CCERT16)
/** \brief  (Instance 7 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT16_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT16_VALUE_SHIFT 0
#define  CORE_VCF_CCERT16_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT16_VALUE_MASK) >> CORE_VCF_CCERT16_VALUE_SHIFT)
#define  CORE_VCF_CCERT16_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT16_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT16_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 8 of 13) (VCF_CCERT17)
/** \brief  (Instance 8 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT17_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT17_VALUE_SHIFT 0
#define  CORE_VCF_CCERT17_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT17_VALUE_MASK) >> CORE_VCF_CCERT17_VALUE_SHIFT)
#define  CORE_VCF_CCERT17_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT17_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT17_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 9 of 13) (VCF_CCERT18)
/** \brief  (Instance 9 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT18_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT18_VALUE_SHIFT 0
#define  CORE_VCF_CCERT18_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT18_VALUE_MASK) >> CORE_VCF_CCERT18_VALUE_SHIFT)
#define  CORE_VCF_CCERT18_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT18_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT18_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 10 of 13) (VCF_CCERT19)
/** \brief  (Instance 10 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT19_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT19_VALUE_SHIFT 0
#define  CORE_VCF_CCERT19_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT19_VALUE_MASK) >> CORE_VCF_CCERT19_VALUE_SHIFT)
#define  CORE_VCF_CCERT19_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT19_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT19_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 11 of 13) (VCF_CCERT110)
/** \brief  (Instance 11 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT110_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT110_VALUE_SHIFT 0
#define  CORE_VCF_CCERT110_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT110_VALUE_MASK) >> CORE_VCF_CCERT110_VALUE_SHIFT)
#define  CORE_VCF_CCERT110_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT110_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT110_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 12 of 13) (VCF_CCERT111)
/** \brief  (Instance 12 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT111_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT111_VALUE_SHIFT 0
#define  CORE_VCF_CCERT111_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT111_VALUE_MASK) >> CORE_VCF_CCERT111_VALUE_SHIFT)
#define  CORE_VCF_CCERT111_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT111_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT111_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 13 of 13) (VCF_CCERT112)
/** \brief  (Instance 13 of 13)*/
//====================================================================

#define  CORE_VCF_CCERT112_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT112_VALUE_SHIFT 0
#define  CORE_VCF_CCERT112_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT112_VALUE_MASK) >> CORE_VCF_CCERT112_VALUE_SHIFT)
#define  CORE_VCF_CCERT112_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT112_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT112_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT2 (VCF_CCERT2)
//====================================================================

#define  CORE_VCF_CCERT2_VALUE_MASK 0xffffffff
#define  CORE_VCF_CCERT2_VALUE_SHIFT 0
#define  CORE_VCF_CCERT2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CCERT2_VALUE_MASK) >> CORE_VCF_CCERT2_VALUE_SHIFT)
#define  CORE_VCF_CCERT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CCERT2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CCERT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 1 of 9) (VCF_VCERT00)
/** \brief  (Instance 1 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT00_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT00_VALUE_SHIFT 0
#define  CORE_VCF_VCERT00_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT00_VALUE_MASK) >> CORE_VCF_VCERT00_VALUE_SHIFT)
#define  CORE_VCF_VCERT00_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT00_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT00_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 2 of 9) (VCF_VCERT01)
/** \brief  (Instance 2 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT01_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT01_VALUE_SHIFT 0
#define  CORE_VCF_VCERT01_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT01_VALUE_MASK) >> CORE_VCF_VCERT01_VALUE_SHIFT)
#define  CORE_VCF_VCERT01_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT01_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT01_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 3 of 9) (VCF_VCERT02)
/** \brief  (Instance 3 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT02_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT02_VALUE_SHIFT 0
#define  CORE_VCF_VCERT02_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT02_VALUE_MASK) >> CORE_VCF_VCERT02_VALUE_SHIFT)
#define  CORE_VCF_VCERT02_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT02_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT02_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 4 of 9) (VCF_VCERT03)
/** \brief  (Instance 4 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT03_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT03_VALUE_SHIFT 0
#define  CORE_VCF_VCERT03_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT03_VALUE_MASK) >> CORE_VCF_VCERT03_VALUE_SHIFT)
#define  CORE_VCF_VCERT03_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT03_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT03_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 5 of 9) (VCF_VCERT04)
/** \brief  (Instance 5 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT04_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT04_VALUE_SHIFT 0
#define  CORE_VCF_VCERT04_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT04_VALUE_MASK) >> CORE_VCF_VCERT04_VALUE_SHIFT)
#define  CORE_VCF_VCERT04_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT04_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT04_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 6 of 9) (VCF_VCERT05)
/** \brief  (Instance 6 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT05_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT05_VALUE_SHIFT 0
#define  CORE_VCF_VCERT05_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT05_VALUE_MASK) >> CORE_VCF_VCERT05_VALUE_SHIFT)
#define  CORE_VCF_VCERT05_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT05_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT05_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 7 of 9) (VCF_VCERT06)
/** \brief  (Instance 7 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT06_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT06_VALUE_SHIFT 0
#define  CORE_VCF_VCERT06_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT06_VALUE_MASK) >> CORE_VCF_VCERT06_VALUE_SHIFT)
#define  CORE_VCF_VCERT06_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT06_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT06_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 8 of 9) (VCF_VCERT07)
/** \brief  (Instance 8 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT07_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT07_VALUE_SHIFT 0
#define  CORE_VCF_VCERT07_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT07_VALUE_MASK) >> CORE_VCF_VCERT07_VALUE_SHIFT)
#define  CORE_VCF_VCERT07_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT07_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT07_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 9 of 9) (VCF_VCERT08)
/** \brief  (Instance 9 of 9)*/
//====================================================================

#define  CORE_VCF_VCERT08_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT08_VALUE_SHIFT 0
#define  CORE_VCF_VCERT08_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT08_VALUE_MASK) >> CORE_VCF_VCERT08_VALUE_SHIFT)
#define  CORE_VCF_VCERT08_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT08_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT08_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_COMPAT (Instance 1 of 3) (VCF_VCERT_COMPAT0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  CORE_VCF_VCERT_COMPAT0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_COMPAT0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_COMPAT0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_COMPAT0_VALUE_MASK) >> CORE_VCF_VCERT_COMPAT0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_COMPAT0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_COMPAT0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_COMPAT0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_COMPAT (Instance 2 of 3) (VCF_VCERT_COMPAT1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  CORE_VCF_VCERT_COMPAT1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_COMPAT1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_COMPAT1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_COMPAT1_VALUE_MASK) >> CORE_VCF_VCERT_COMPAT1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_COMPAT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_COMPAT1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_COMPAT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_COMPAT (Instance 3 of 3) (VCF_VCERT_COMPAT2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  CORE_VCF_VCERT_COMPAT2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_COMPAT2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_COMPAT2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_COMPAT2_VALUE_MASK) >> CORE_VCF_VCERT_COMPAT2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_COMPAT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_COMPAT2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_COMPAT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_TIMER (VCF_VCERT_TIMER)
//====================================================================

#define  CORE_VCF_VCERT_TIMER_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_TIMER_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_TIMER_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_TIMER_VALUE_MASK) >> CORE_VCF_VCERT_TIMER_VALUE_SHIFT)
#define  CORE_VCF_VCERT_TIMER_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_TIMER_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_TIMER_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 1 of 4) (VCF_VCERT_MULT_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CL0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CL0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CL0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CL0_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CL0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CL0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 2 of 4) (VCF_VCERT_MULT_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CL1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CL1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CL1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CL1_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CL1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CL1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 3 of 4) (VCF_VCERT_MULT_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CL2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CL2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CL2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CL2_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CL2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CL2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 4 of 4) (VCF_VCERT_MULT_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CL3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CL3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CL3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CL3_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CL3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CL3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 1 of 12) (VCF_VCERT_MULT_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH0_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 2 of 12) (VCF_VCERT_MULT_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH1_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 3 of 12) (VCF_VCERT_MULT_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH2_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 4 of 12) (VCF_VCERT_MULT_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH3_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 5 of 12) (VCF_VCERT_MULT_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH4_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH4_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH4_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 6 of 12) (VCF_VCERT_MULT_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH5_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH5_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH5_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 7 of 12) (VCF_VCERT_MULT_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH6_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH6_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH6_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 8 of 12) (VCF_VCERT_MULT_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH7_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH7_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH7_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 9 of 12) (VCF_VCERT_MULT_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH8_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH8_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH8_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 10 of 12) (VCF_VCERT_MULT_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH9_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH9_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH9_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 11 of 12) (VCF_VCERT_MULT_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH10_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH10_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH10_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 12 of 12) (VCF_VCERT_MULT_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_MULT_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_MULT_CH11_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_MULT_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_MULT_CH11_VALUE_MASK) >> CORE_VCF_VCERT_MULT_CH11_VALUE_SHIFT)
#define  CORE_VCF_VCERT_MULT_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_MULT_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_MULT_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 1 of 4) (VCF_VCERT_USE_THRESH_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 2 of 4) (VCF_VCERT_USE_THRESH_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 3 of 4) (VCF_VCERT_USE_THRESH_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 4 of 4) (VCF_VCERT_USE_THRESH_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 1 of 12) (VCF_VCERT_USE_THRESH_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 2 of 12) (VCF_VCERT_USE_THRESH_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 3 of 12) (VCF_VCERT_USE_THRESH_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 4 of 12) (VCF_VCERT_USE_THRESH_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 5 of 12) (VCF_VCERT_USE_THRESH_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 6 of 12) (VCF_VCERT_USE_THRESH_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 7 of 12) (VCF_VCERT_USE_THRESH_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 8 of 12) (VCF_VCERT_USE_THRESH_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 9 of 12) (VCF_VCERT_USE_THRESH_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 10 of 12) (VCF_VCERT_USE_THRESH_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 11 of 12) (VCF_VCERT_USE_THRESH_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 12 of 12) (VCF_VCERT_USE_THRESH_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK) >> CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_SHIFT)
#define  CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 1 of 4) (VCF_VCERT_DATA1_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CL0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CL0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 2 of 4) (VCF_VCERT_DATA1_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CL1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CL1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 3 of 4) (VCF_VCERT_DATA1_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CL2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CL2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 4 of 4) (VCF_VCERT_DATA1_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CL3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CL3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 1 of 12) (VCF_VCERT_DATA1_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 2 of 12) (VCF_VCERT_DATA1_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 3 of 12) (VCF_VCERT_DATA1_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 4 of 12) (VCF_VCERT_DATA1_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 5 of 12) (VCF_VCERT_DATA1_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH4_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH4_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 6 of 12) (VCF_VCERT_DATA1_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH5_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH5_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 7 of 12) (VCF_VCERT_DATA1_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH6_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH6_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 8 of 12) (VCF_VCERT_DATA1_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH7_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH7_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 9 of 12) (VCF_VCERT_DATA1_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH8_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH8_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 10 of 12) (VCF_VCERT_DATA1_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH9_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH9_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 11 of 12) (VCF_VCERT_DATA1_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH10_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH10_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 12 of 12) (VCF_VCERT_DATA1_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA1_CH11_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK) >> CORE_VCF_VCERT_DATA1_CH11_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA1_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA1_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 1 of 4) (VCF_VCERT_DATA2_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CL0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CL0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 2 of 4) (VCF_VCERT_DATA2_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CL1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CL1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 3 of 4) (VCF_VCERT_DATA2_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CL2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CL2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 4 of 4) (VCF_VCERT_DATA2_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CL3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CL3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 1 of 12) (VCF_VCERT_DATA2_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH0_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH0_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 2 of 12) (VCF_VCERT_DATA2_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH1_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH1_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 3 of 12) (VCF_VCERT_DATA2_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH2_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH2_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 4 of 12) (VCF_VCERT_DATA2_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH3_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH3_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 5 of 12) (VCF_VCERT_DATA2_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH4_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH4_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 6 of 12) (VCF_VCERT_DATA2_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH5_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH5_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 7 of 12) (VCF_VCERT_DATA2_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH6_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH6_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 8 of 12) (VCF_VCERT_DATA2_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH7_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH7_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 9 of 12) (VCF_VCERT_DATA2_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH8_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH8_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 10 of 12) (VCF_VCERT_DATA2_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH9_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH9_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 11 of 12) (VCF_VCERT_DATA2_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH10_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH10_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 12 of 12) (VCF_VCERT_DATA2_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_VCERT_DATA2_CH11_VALUE_SHIFT 0
#define  CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK) >> CORE_VCF_VCERT_DATA2_CH11_VALUE_SHIFT)
#define  CORE_VCF_VCERT_DATA2_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_VCERT_DATA2_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 1 of 4) (VCF_WARN_USE_THRESH_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CL0_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CL0_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 2 of 4) (VCF_WARN_USE_THRESH_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CL1_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CL1_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 3 of 4) (VCF_WARN_USE_THRESH_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CL2_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CL2_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 4 of 4) (VCF_WARN_USE_THRESH_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CL3_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CL3_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 1 of 12) (VCF_WARN_USE_THRESH_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH0_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH0_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 2 of 12) (VCF_WARN_USE_THRESH_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH1_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH1_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 3 of 12) (VCF_WARN_USE_THRESH_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH2_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH2_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 4 of 12) (VCF_WARN_USE_THRESH_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH3_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH3_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 5 of 12) (VCF_WARN_USE_THRESH_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH4_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH4_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 6 of 12) (VCF_WARN_USE_THRESH_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH5_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH5_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 7 of 12) (VCF_WARN_USE_THRESH_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH6_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH6_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 8 of 12) (VCF_WARN_USE_THRESH_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH7_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH7_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 9 of 12) (VCF_WARN_USE_THRESH_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH8_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH8_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 10 of 12) (VCF_WARN_USE_THRESH_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH9_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH9_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 11 of 12) (VCF_WARN_USE_THRESH_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH10_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH10_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 12 of 12) (VCF_WARN_USE_THRESH_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_WARN_USE_THRESH_CH11_VALUE_SHIFT 0
#define  CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK) >> CORE_VCF_WARN_USE_THRESH_CH11_VALUE_SHIFT)
#define  CORE_VCF_WARN_USE_THRESH_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_WARN_USE_THRESH_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 1 of 12) (VCF_AUTH_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH0_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH0_VALUE_MASK) >> CORE_VCF_AUTH_CH0_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 2 of 12) (VCF_AUTH_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH1_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH1_VALUE_MASK) >> CORE_VCF_AUTH_CH1_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 3 of 12) (VCF_AUTH_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH2_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH2_VALUE_MASK) >> CORE_VCF_AUTH_CH2_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 4 of 12) (VCF_AUTH_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH3_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH3_VALUE_MASK) >> CORE_VCF_AUTH_CH3_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 5 of 12) (VCF_AUTH_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH4_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH4_VALUE_MASK) >> CORE_VCF_AUTH_CH4_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 6 of 12) (VCF_AUTH_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH5_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH5_VALUE_MASK) >> CORE_VCF_AUTH_CH5_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 7 of 12) (VCF_AUTH_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH6_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH6_VALUE_MASK) >> CORE_VCF_AUTH_CH6_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 8 of 12) (VCF_AUTH_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH7_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH7_VALUE_MASK) >> CORE_VCF_AUTH_CH7_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 9 of 12) (VCF_AUTH_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH8_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH8_VALUE_MASK) >> CORE_VCF_AUTH_CH8_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 10 of 12) (VCF_AUTH_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH9_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH9_VALUE_MASK) >> CORE_VCF_AUTH_CH9_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 11 of 12) (VCF_AUTH_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH10_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH10_VALUE_MASK) >> CORE_VCF_AUTH_CH10_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 12 of 12) (VCF_AUTH_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_AUTH_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTH_CH11_VALUE_SHIFT 0
#define  CORE_VCF_AUTH_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTH_CH11_VALUE_MASK) >> CORE_VCF_AUTH_CH11_VALUE_SHIFT)
#define  CORE_VCF_AUTH_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTH_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTH_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 1 of 12) (VCF_AUTHUSE_LO_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH0_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH0_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 2 of 12) (VCF_AUTHUSE_LO_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH1_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH1_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 3 of 12) (VCF_AUTHUSE_LO_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH2_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH2_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 4 of 12) (VCF_AUTHUSE_LO_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH3_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH3_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 5 of 12) (VCF_AUTHUSE_LO_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH4_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH4_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 6 of 12) (VCF_AUTHUSE_LO_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH5_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH5_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 7 of 12) (VCF_AUTHUSE_LO_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH6_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH6_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 8 of 12) (VCF_AUTHUSE_LO_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH7_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH7_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 9 of 12) (VCF_AUTHUSE_LO_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH8_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH8_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 10 of 12) (VCF_AUTHUSE_LO_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH9_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH9_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 11 of 12) (VCF_AUTHUSE_LO_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH10_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH10_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 12 of 12) (VCF_AUTHUSE_LO_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_LO_CH11_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK) >> CORE_VCF_AUTHUSE_LO_CH11_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_LO_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_LO_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 1 of 12) (VCF_AUTHUSE_HI_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH0_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH0_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 2 of 12) (VCF_AUTHUSE_HI_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH1_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH1_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 3 of 12) (VCF_AUTHUSE_HI_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH2_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH2_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 4 of 12) (VCF_AUTHUSE_HI_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH3_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH3_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 5 of 12) (VCF_AUTHUSE_HI_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH4_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH4_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 6 of 12) (VCF_AUTHUSE_HI_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH5_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH5_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 7 of 12) (VCF_AUTHUSE_HI_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH6_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH6_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 8 of 12) (VCF_AUTHUSE_HI_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH7_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH7_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 9 of 12) (VCF_AUTHUSE_HI_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH8_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH8_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 10 of 12) (VCF_AUTHUSE_HI_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH9_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH9_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 11 of 12) (VCF_AUTHUSE_HI_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH10_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH10_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 12 of 12) (VCF_AUTHUSE_HI_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK 0xffffffff
#define  CORE_VCF_AUTHUSE_HI_CH11_VALUE_SHIFT 0
#define  CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK) >> CORE_VCF_AUTHUSE_HI_CH11_VALUE_SHIFT)
#define  CORE_VCF_AUTHUSE_HI_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_AUTHUSE_HI_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_OP_METER_LO (VCF_OP_METER_LO)
//====================================================================

#define  CORE_VCF_OP_METER_LO_VALUE_MASK 0xffffffff
#define  CORE_VCF_OP_METER_LO_VALUE_SHIFT 0
#define  CORE_VCF_OP_METER_LO_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_OP_METER_LO_VALUE_MASK) >> CORE_VCF_OP_METER_LO_VALUE_SHIFT)
#define  CORE_VCF_OP_METER_LO_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_OP_METER_LO_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_OP_METER_LO_VALUE_SHIFT))

//====================================================================
//Register: VCF_OP_METER_HI (VCF_OP_METER_HI)
//====================================================================

#define  CORE_VCF_OP_METER_HI_VALUE_MASK 0xffffffff
#define  CORE_VCF_OP_METER_HI_VALUE_SHIFT 0
#define  CORE_VCF_OP_METER_HI_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_OP_METER_HI_VALUE_MASK) >> CORE_VCF_OP_METER_HI_VALUE_SHIFT)
#define  CORE_VCF_OP_METER_HI_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_OP_METER_HI_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_OP_METER_HI_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 1 of 10) (VCF_SCERT0)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT0_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT0_VALUE_SHIFT 0
#define  CORE_VCF_SCERT0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT0_VALUE_MASK) >> CORE_VCF_SCERT0_VALUE_SHIFT)
#define  CORE_VCF_SCERT0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT0_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 2 of 10) (VCF_SCERT1)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT1_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT1_VALUE_SHIFT 0
#define  CORE_VCF_SCERT1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT1_VALUE_MASK) >> CORE_VCF_SCERT1_VALUE_SHIFT)
#define  CORE_VCF_SCERT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 3 of 10) (VCF_SCERT2)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT2_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT2_VALUE_SHIFT 0
#define  CORE_VCF_SCERT2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT2_VALUE_MASK) >> CORE_VCF_SCERT2_VALUE_SHIFT)
#define  CORE_VCF_SCERT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 4 of 10) (VCF_SCERT3)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT3_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT3_VALUE_SHIFT 0
#define  CORE_VCF_SCERT3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT3_VALUE_MASK) >> CORE_VCF_SCERT3_VALUE_SHIFT)
#define  CORE_VCF_SCERT3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT3_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 5 of 10) (VCF_SCERT4)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT4_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT4_VALUE_SHIFT 0
#define  CORE_VCF_SCERT4_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT4_VALUE_MASK) >> CORE_VCF_SCERT4_VALUE_SHIFT)
#define  CORE_VCF_SCERT4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT4_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT4_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 6 of 10) (VCF_SCERT5)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT5_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT5_VALUE_SHIFT 0
#define  CORE_VCF_SCERT5_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT5_VALUE_MASK) >> CORE_VCF_SCERT5_VALUE_SHIFT)
#define  CORE_VCF_SCERT5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT5_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT5_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 7 of 10) (VCF_SCERT6)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT6_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT6_VALUE_SHIFT 0
#define  CORE_VCF_SCERT6_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT6_VALUE_MASK) >> CORE_VCF_SCERT6_VALUE_SHIFT)
#define  CORE_VCF_SCERT6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT6_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT6_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 8 of 10) (VCF_SCERT7)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT7_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT7_VALUE_SHIFT 0
#define  CORE_VCF_SCERT7_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT7_VALUE_MASK) >> CORE_VCF_SCERT7_VALUE_SHIFT)
#define  CORE_VCF_SCERT7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT7_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT7_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 9 of 10) (VCF_SCERT8)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT8_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT8_VALUE_SHIFT 0
#define  CORE_VCF_SCERT8_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT8_VALUE_MASK) >> CORE_VCF_SCERT8_VALUE_SHIFT)
#define  CORE_VCF_SCERT8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT8_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT8_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 10 of 10) (VCF_SCERT9)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  CORE_VCF_SCERT9_VALUE_MASK 0xffffffff
#define  CORE_VCF_SCERT9_VALUE_SHIFT 0
#define  CORE_VCF_SCERT9_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_SCERT9_VALUE_MASK) >> CORE_VCF_SCERT9_VALUE_SHIFT)
#define  CORE_VCF_SCERT9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_SCERT9_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_SCERT9_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 1 of 4) (VCF_CERT0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  CORE_VCF_CERT0_VALUE_MASK 0xffffffff
#define  CORE_VCF_CERT0_VALUE_SHIFT 0
#define  CORE_VCF_CERT0_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CERT0_VALUE_MASK) >> CORE_VCF_CERT0_VALUE_SHIFT)
#define  CORE_VCF_CERT0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CERT0_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CERT0_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 2 of 4) (VCF_CERT1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  CORE_VCF_CERT1_VALUE_MASK 0xffffffff
#define  CORE_VCF_CERT1_VALUE_SHIFT 0
#define  CORE_VCF_CERT1_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CERT1_VALUE_MASK) >> CORE_VCF_CERT1_VALUE_SHIFT)
#define  CORE_VCF_CERT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CERT1_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CERT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 3 of 4) (VCF_CERT2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  CORE_VCF_CERT2_VALUE_MASK 0xffffffff
#define  CORE_VCF_CERT2_VALUE_SHIFT 0
#define  CORE_VCF_CERT2_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CERT2_VALUE_MASK) >> CORE_VCF_CERT2_VALUE_SHIFT)
#define  CORE_VCF_CERT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CERT2_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CERT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 4 of 4) (VCF_CERT3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  CORE_VCF_CERT3_VALUE_MASK 0xffffffff
#define  CORE_VCF_CERT3_VALUE_SHIFT 0
#define  CORE_VCF_CERT3_VALUE_MASK_SHIFT(reg) (((reg) & CORE_VCF_CERT3_VALUE_MASK) >> CORE_VCF_CERT3_VALUE_SHIFT)
#define  CORE_VCF_CERT3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~CORE_VCF_CERT3_VALUE_MASK) | (((uint32_t)val) << CORE_VCF_CERT3_VALUE_SHIFT))

//====================================================================
//
//Register File: WRAPPER (WRAPPER)
//
//====================================================================

//====================================================================
//Register: VCF Accumulator Low Register (Instance 1 of 12) (VALR0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  WRAPPER_VALR0_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR0_VALUE_SHIFT 0
#define  WRAPPER_VALR0_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR0_VALUE_MASK) >> WRAPPER_VALR0_VALUE_SHIFT)
#define  WRAPPER_VALR0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR0_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR0_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 2 of 12) (VALR1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  WRAPPER_VALR1_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR1_VALUE_SHIFT 0
#define  WRAPPER_VALR1_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR1_VALUE_MASK) >> WRAPPER_VALR1_VALUE_SHIFT)
#define  WRAPPER_VALR1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR1_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR1_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 3 of 12) (VALR2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  WRAPPER_VALR2_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR2_VALUE_SHIFT 0
#define  WRAPPER_VALR2_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR2_VALUE_MASK) >> WRAPPER_VALR2_VALUE_SHIFT)
#define  WRAPPER_VALR2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR2_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR2_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 4 of 12) (VALR3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  WRAPPER_VALR3_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR3_VALUE_SHIFT 0
#define  WRAPPER_VALR3_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR3_VALUE_MASK) >> WRAPPER_VALR3_VALUE_SHIFT)
#define  WRAPPER_VALR3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR3_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR3_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 5 of 12) (VALR4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  WRAPPER_VALR4_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR4_VALUE_SHIFT 0
#define  WRAPPER_VALR4_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR4_VALUE_MASK) >> WRAPPER_VALR4_VALUE_SHIFT)
#define  WRAPPER_VALR4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR4_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR4_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 6 of 12) (VALR5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  WRAPPER_VALR5_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR5_VALUE_SHIFT 0
#define  WRAPPER_VALR5_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR5_VALUE_MASK) >> WRAPPER_VALR5_VALUE_SHIFT)
#define  WRAPPER_VALR5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR5_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR5_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 7 of 12) (VALR6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  WRAPPER_VALR6_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR6_VALUE_SHIFT 0
#define  WRAPPER_VALR6_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR6_VALUE_MASK) >> WRAPPER_VALR6_VALUE_SHIFT)
#define  WRAPPER_VALR6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR6_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR6_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 8 of 12) (VALR7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  WRAPPER_VALR7_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR7_VALUE_SHIFT 0
#define  WRAPPER_VALR7_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR7_VALUE_MASK) >> WRAPPER_VALR7_VALUE_SHIFT)
#define  WRAPPER_VALR7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR7_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR7_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 9 of 12) (VALR8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  WRAPPER_VALR8_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR8_VALUE_SHIFT 0
#define  WRAPPER_VALR8_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR8_VALUE_MASK) >> WRAPPER_VALR8_VALUE_SHIFT)
#define  WRAPPER_VALR8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR8_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR8_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 10 of 12) (VALR9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  WRAPPER_VALR9_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR9_VALUE_SHIFT 0
#define  WRAPPER_VALR9_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR9_VALUE_MASK) >> WRAPPER_VALR9_VALUE_SHIFT)
#define  WRAPPER_VALR9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR9_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR9_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 11 of 12) (VALR10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  WRAPPER_VALR10_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR10_VALUE_SHIFT 0
#define  WRAPPER_VALR10_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR10_VALUE_MASK) >> WRAPPER_VALR10_VALUE_SHIFT)
#define  WRAPPER_VALR10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR10_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR10_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Low Register (Instance 12 of 12) (VALR11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  WRAPPER_VALR11_VALUE_MASK 0xffffffff
#define  WRAPPER_VALR11_VALUE_SHIFT 0
#define  WRAPPER_VALR11_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VALR11_VALUE_MASK) >> WRAPPER_VALR11_VALUE_SHIFT)
#define  WRAPPER_VALR11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VALR11_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VALR11_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 1 of 12) (VAHR0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  WRAPPER_VAHR0_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR0_VALUE_SHIFT 0
#define  WRAPPER_VAHR0_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR0_VALUE_MASK) >> WRAPPER_VAHR0_VALUE_SHIFT)
#define  WRAPPER_VAHR0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR0_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR0_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 2 of 12) (VAHR1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  WRAPPER_VAHR1_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR1_VALUE_SHIFT 0
#define  WRAPPER_VAHR1_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR1_VALUE_MASK) >> WRAPPER_VAHR1_VALUE_SHIFT)
#define  WRAPPER_VAHR1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR1_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR1_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 3 of 12) (VAHR2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  WRAPPER_VAHR2_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR2_VALUE_SHIFT 0
#define  WRAPPER_VAHR2_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR2_VALUE_MASK) >> WRAPPER_VAHR2_VALUE_SHIFT)
#define  WRAPPER_VAHR2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR2_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR2_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 4 of 12) (VAHR3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  WRAPPER_VAHR3_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR3_VALUE_SHIFT 0
#define  WRAPPER_VAHR3_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR3_VALUE_MASK) >> WRAPPER_VAHR3_VALUE_SHIFT)
#define  WRAPPER_VAHR3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR3_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR3_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 5 of 12) (VAHR4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  WRAPPER_VAHR4_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR4_VALUE_SHIFT 0
#define  WRAPPER_VAHR4_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR4_VALUE_MASK) >> WRAPPER_VAHR4_VALUE_SHIFT)
#define  WRAPPER_VAHR4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR4_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR4_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 6 of 12) (VAHR5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  WRAPPER_VAHR5_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR5_VALUE_SHIFT 0
#define  WRAPPER_VAHR5_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR5_VALUE_MASK) >> WRAPPER_VAHR5_VALUE_SHIFT)
#define  WRAPPER_VAHR5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR5_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR5_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 7 of 12) (VAHR6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  WRAPPER_VAHR6_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR6_VALUE_SHIFT 0
#define  WRAPPER_VAHR6_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR6_VALUE_MASK) >> WRAPPER_VAHR6_VALUE_SHIFT)
#define  WRAPPER_VAHR6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR6_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR6_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 8 of 12) (VAHR7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  WRAPPER_VAHR7_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR7_VALUE_SHIFT 0
#define  WRAPPER_VAHR7_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR7_VALUE_MASK) >> WRAPPER_VAHR7_VALUE_SHIFT)
#define  WRAPPER_VAHR7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR7_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR7_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 9 of 12) (VAHR8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  WRAPPER_VAHR8_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR8_VALUE_SHIFT 0
#define  WRAPPER_VAHR8_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR8_VALUE_MASK) >> WRAPPER_VAHR8_VALUE_SHIFT)
#define  WRAPPER_VAHR8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR8_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR8_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 10 of 12) (VAHR9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  WRAPPER_VAHR9_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR9_VALUE_SHIFT 0
#define  WRAPPER_VAHR9_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR9_VALUE_MASK) >> WRAPPER_VAHR9_VALUE_SHIFT)
#define  WRAPPER_VAHR9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR9_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR9_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 11 of 12) (VAHR10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  WRAPPER_VAHR10_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR10_VALUE_SHIFT 0
#define  WRAPPER_VAHR10_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR10_VALUE_MASK) >> WRAPPER_VAHR10_VALUE_SHIFT)
#define  WRAPPER_VAHR10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR10_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR10_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator High Register (Instance 12 of 12) (VAHR11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  WRAPPER_VAHR11_VALUE_MASK 0xffffffff
#define  WRAPPER_VAHR11_VALUE_SHIFT 0
#define  WRAPPER_VAHR11_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAHR11_VALUE_MASK) >> WRAPPER_VAHR11_VALUE_SHIFT)
#define  WRAPPER_VAHR11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAHR11_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAHR11_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 1 of 12) (VAIR0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  WRAPPER_VAIR0_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR0_VALUE_SHIFT 0
#define  WRAPPER_VAIR0_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR0_VALUE_MASK) >> WRAPPER_VAIR0_VALUE_SHIFT)
#define  WRAPPER_VAIR0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR0_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR0_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 2 of 12) (VAIR1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  WRAPPER_VAIR1_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR1_VALUE_SHIFT 0
#define  WRAPPER_VAIR1_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR1_VALUE_MASK) >> WRAPPER_VAIR1_VALUE_SHIFT)
#define  WRAPPER_VAIR1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR1_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR1_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 3 of 12) (VAIR2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  WRAPPER_VAIR2_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR2_VALUE_SHIFT 0
#define  WRAPPER_VAIR2_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR2_VALUE_MASK) >> WRAPPER_VAIR2_VALUE_SHIFT)
#define  WRAPPER_VAIR2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR2_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR2_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 4 of 12) (VAIR3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  WRAPPER_VAIR3_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR3_VALUE_SHIFT 0
#define  WRAPPER_VAIR3_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR3_VALUE_MASK) >> WRAPPER_VAIR3_VALUE_SHIFT)
#define  WRAPPER_VAIR3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR3_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR3_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 5 of 12) (VAIR4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  WRAPPER_VAIR4_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR4_VALUE_SHIFT 0
#define  WRAPPER_VAIR4_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR4_VALUE_MASK) >> WRAPPER_VAIR4_VALUE_SHIFT)
#define  WRAPPER_VAIR4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR4_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR4_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 6 of 12) (VAIR5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  WRAPPER_VAIR5_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR5_VALUE_SHIFT 0
#define  WRAPPER_VAIR5_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR5_VALUE_MASK) >> WRAPPER_VAIR5_VALUE_SHIFT)
#define  WRAPPER_VAIR5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR5_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR5_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 7 of 12) (VAIR6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  WRAPPER_VAIR6_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR6_VALUE_SHIFT 0
#define  WRAPPER_VAIR6_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR6_VALUE_MASK) >> WRAPPER_VAIR6_VALUE_SHIFT)
#define  WRAPPER_VAIR6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR6_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR6_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 8 of 12) (VAIR7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  WRAPPER_VAIR7_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR7_VALUE_SHIFT 0
#define  WRAPPER_VAIR7_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR7_VALUE_MASK) >> WRAPPER_VAIR7_VALUE_SHIFT)
#define  WRAPPER_VAIR7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR7_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR7_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 9 of 12) (VAIR8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  WRAPPER_VAIR8_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR8_VALUE_SHIFT 0
#define  WRAPPER_VAIR8_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR8_VALUE_MASK) >> WRAPPER_VAIR8_VALUE_SHIFT)
#define  WRAPPER_VAIR8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR8_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR8_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 10 of 12) (VAIR9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  WRAPPER_VAIR9_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR9_VALUE_SHIFT 0
#define  WRAPPER_VAIR9_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR9_VALUE_MASK) >> WRAPPER_VAIR9_VALUE_SHIFT)
#define  WRAPPER_VAIR9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR9_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR9_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 11 of 12) (VAIR10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  WRAPPER_VAIR10_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR10_VALUE_SHIFT 0
#define  WRAPPER_VAIR10_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR10_VALUE_MASK) >> WRAPPER_VAIR10_VALUE_SHIFT)
#define  WRAPPER_VAIR10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR10_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR10_VALUE_SHIFT))

//====================================================================
//Register: VCF Accumulator Increment Register (Instance 12 of 12) (VAIR11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  WRAPPER_VAIR11_VALUE_MASK 0xffffffff
#define  WRAPPER_VAIR11_VALUE_SHIFT 0
#define  WRAPPER_VAIR11_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VAIR11_VALUE_MASK) >> WRAPPER_VAIR11_VALUE_SHIFT)
#define  WRAPPER_VAIR11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VAIR11_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VAIR11_VALUE_SHIFT))

//====================================================================
//Register: VCF MCPU Hash Register (Instance 1 of 6) (VMHR0)
/** \brief  (Instance 1 of 6)*/
//====================================================================

#define  WRAPPER_VMHR0_VALUE_MASK 0xffffffff
#define  WRAPPER_VMHR0_VALUE_SHIFT 0
#define  WRAPPER_VMHR0_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VMHR0_VALUE_MASK) >> WRAPPER_VMHR0_VALUE_SHIFT)
#define  WRAPPER_VMHR0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMHR0_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VMHR0_VALUE_SHIFT))

//====================================================================
//Register: VCF MCPU Hash Register (Instance 2 of 6) (VMHR1)
/** \brief  (Instance 2 of 6)*/
//====================================================================

#define  WRAPPER_VMHR1_VALUE_MASK 0xffffffff
#define  WRAPPER_VMHR1_VALUE_SHIFT 0
#define  WRAPPER_VMHR1_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VMHR1_VALUE_MASK) >> WRAPPER_VMHR1_VALUE_SHIFT)
#define  WRAPPER_VMHR1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMHR1_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VMHR1_VALUE_SHIFT))

//====================================================================
//Register: VCF MCPU Hash Register (Instance 3 of 6) (VMHR2)
/** \brief  (Instance 3 of 6)*/
//====================================================================

#define  WRAPPER_VMHR2_VALUE_MASK 0xffffffff
#define  WRAPPER_VMHR2_VALUE_SHIFT 0
#define  WRAPPER_VMHR2_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VMHR2_VALUE_MASK) >> WRAPPER_VMHR2_VALUE_SHIFT)
#define  WRAPPER_VMHR2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMHR2_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VMHR2_VALUE_SHIFT))

//====================================================================
//Register: VCF MCPU Hash Register (Instance 4 of 6) (VMHR3)
/** \brief  (Instance 4 of 6)*/
//====================================================================

#define  WRAPPER_VMHR3_VALUE_MASK 0xffffffff
#define  WRAPPER_VMHR3_VALUE_SHIFT 0
#define  WRAPPER_VMHR3_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VMHR3_VALUE_MASK) >> WRAPPER_VMHR3_VALUE_SHIFT)
#define  WRAPPER_VMHR3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMHR3_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VMHR3_VALUE_SHIFT))

//====================================================================
//Register: VCF MCPU Hash Register (Instance 5 of 6) (VMHR4)
/** \brief  (Instance 5 of 6)*/
//====================================================================

#define  WRAPPER_VMHR4_VALUE_MASK 0xffffffff
#define  WRAPPER_VMHR4_VALUE_SHIFT 0
#define  WRAPPER_VMHR4_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VMHR4_VALUE_MASK) >> WRAPPER_VMHR4_VALUE_SHIFT)
#define  WRAPPER_VMHR4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMHR4_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VMHR4_VALUE_SHIFT))

//====================================================================
//Register: VCF MCPU Hash Register (Instance 6 of 6) (VMHR5)
/** \brief  (Instance 6 of 6)*/
//====================================================================

#define  WRAPPER_VMHR5_VALUE_MASK 0xffffffff
#define  WRAPPER_VMHR5_VALUE_SHIFT 0
#define  WRAPPER_VMHR5_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VMHR5_VALUE_MASK) >> WRAPPER_VMHR5_VALUE_SHIFT)
#define  WRAPPER_VMHR5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMHR5_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VMHR5_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU0 Hash Register (Instance 1 of 6) (VIHR00)
/** \brief  (Instance 1 of 6)*/
//====================================================================

#define  WRAPPER_VIHR00_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR00_VALUE_SHIFT 0
#define  WRAPPER_VIHR00_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR00_VALUE_MASK) >> WRAPPER_VIHR00_VALUE_SHIFT)
#define  WRAPPER_VIHR00_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR00_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR00_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU0 Hash Register (Instance 2 of 6) (VIHR01)
/** \brief  (Instance 2 of 6)*/
//====================================================================

#define  WRAPPER_VIHR01_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR01_VALUE_SHIFT 0
#define  WRAPPER_VIHR01_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR01_VALUE_MASK) >> WRAPPER_VIHR01_VALUE_SHIFT)
#define  WRAPPER_VIHR01_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR01_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR01_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU0 Hash Register (Instance 3 of 6) (VIHR02)
/** \brief  (Instance 3 of 6)*/
//====================================================================

#define  WRAPPER_VIHR02_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR02_VALUE_SHIFT 0
#define  WRAPPER_VIHR02_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR02_VALUE_MASK) >> WRAPPER_VIHR02_VALUE_SHIFT)
#define  WRAPPER_VIHR02_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR02_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR02_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU0 Hash Register (Instance 4 of 6) (VIHR03)
/** \brief  (Instance 4 of 6)*/
//====================================================================

#define  WRAPPER_VIHR03_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR03_VALUE_SHIFT 0
#define  WRAPPER_VIHR03_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR03_VALUE_MASK) >> WRAPPER_VIHR03_VALUE_SHIFT)
#define  WRAPPER_VIHR03_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR03_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR03_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU0 Hash Register (Instance 5 of 6) (VIHR04)
/** \brief  (Instance 5 of 6)*/
//====================================================================

#define  WRAPPER_VIHR04_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR04_VALUE_SHIFT 0
#define  WRAPPER_VIHR04_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR04_VALUE_MASK) >> WRAPPER_VIHR04_VALUE_SHIFT)
#define  WRAPPER_VIHR04_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR04_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR04_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU0 Hash Register (Instance 6 of 6) (VIHR05)
/** \brief  (Instance 6 of 6)*/
//====================================================================

#define  WRAPPER_VIHR05_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR05_VALUE_SHIFT 0
#define  WRAPPER_VIHR05_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR05_VALUE_MASK) >> WRAPPER_VIHR05_VALUE_SHIFT)
#define  WRAPPER_VIHR05_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR05_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR05_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU1 Hash Register (Instance 1 of 6) (VIHR10)
/** \brief  (Instance 1 of 6)*/
//====================================================================

#define  WRAPPER_VIHR10_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR10_VALUE_SHIFT 0
#define  WRAPPER_VIHR10_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR10_VALUE_MASK) >> WRAPPER_VIHR10_VALUE_SHIFT)
#define  WRAPPER_VIHR10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR10_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR10_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU1 Hash Register (Instance 2 of 6) (VIHR11)
/** \brief  (Instance 2 of 6)*/
//====================================================================

#define  WRAPPER_VIHR11_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR11_VALUE_SHIFT 0
#define  WRAPPER_VIHR11_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR11_VALUE_MASK) >> WRAPPER_VIHR11_VALUE_SHIFT)
#define  WRAPPER_VIHR11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR11_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR11_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU1 Hash Register (Instance 3 of 6) (VIHR12)
/** \brief  (Instance 3 of 6)*/
//====================================================================

#define  WRAPPER_VIHR12_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR12_VALUE_SHIFT 0
#define  WRAPPER_VIHR12_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR12_VALUE_MASK) >> WRAPPER_VIHR12_VALUE_SHIFT)
#define  WRAPPER_VIHR12_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR12_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR12_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU1 Hash Register (Instance 4 of 6) (VIHR13)
/** \brief  (Instance 4 of 6)*/
//====================================================================

#define  WRAPPER_VIHR13_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR13_VALUE_SHIFT 0
#define  WRAPPER_VIHR13_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR13_VALUE_MASK) >> WRAPPER_VIHR13_VALUE_SHIFT)
#define  WRAPPER_VIHR13_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR13_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR13_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU1 Hash Register (Instance 5 of 6) (VIHR14)
/** \brief  (Instance 5 of 6)*/
//====================================================================

#define  WRAPPER_VIHR14_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR14_VALUE_SHIFT 0
#define  WRAPPER_VIHR14_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR14_VALUE_MASK) >> WRAPPER_VIHR14_VALUE_SHIFT)
#define  WRAPPER_VIHR14_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR14_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR14_VALUE_SHIFT))

//====================================================================
//Register: VCF ICPU1 Hash Register (Instance 6 of 6) (VIHR15)
/** \brief  (Instance 6 of 6)*/
//====================================================================

#define  WRAPPER_VIHR15_VALUE_MASK 0xffffffff
#define  WRAPPER_VIHR15_VALUE_SHIFT 0
#define  WRAPPER_VIHR15_VALUE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIHR15_VALUE_MASK) >> WRAPPER_VIHR15_VALUE_SHIFT)
#define  WRAPPER_VIHR15_VALUE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIHR15_VALUE_MASK) | (((uint32_t)val) << WRAPPER_VIHR15_VALUE_SHIFT))

//====================================================================
//Register: VCF Wrapper Status Register (VWSR)
//====================================================================

#define  WRAPPER_VWSR_RESERVED1_MASK 0xffffe000
#define  WRAPPER_VWSR_RESERVED1_SHIFT 13
#define  WRAPPER_VWSR_RESERVED1_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_RESERVED1_MASK) >> WRAPPER_VWSR_RESERVED1_SHIFT)
#define  WRAPPER_VWSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_RESERVED1_MASK) | (((uint32_t)val) << WRAPPER_VWSR_RESERVED1_SHIFT))

#define  WRAPPER_VWSR_LASERACCUMSEL_MASK 0x1000
#define  WRAPPER_VWSR_LASERACCUMSEL_SHIFT 12
#define  WRAPPER_VWSR_LASERACCUMSEL_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_LASERACCUMSEL_MASK) >> WRAPPER_VWSR_LASERACCUMSEL_SHIFT)
#define  WRAPPER_VWSR_LASERACCUMSEL_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_LASERACCUMSEL_MASK) | (((uint32_t)val) << WRAPPER_VWSR_LASERACCUMSEL_SHIFT))

#define  WRAPPER_VWSR_ALTINKMODE_MASK 0x800
#define  WRAPPER_VWSR_ALTINKMODE_SHIFT 11
#define  WRAPPER_VWSR_ALTINKMODE_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_ALTINKMODE_MASK) >> WRAPPER_VWSR_ALTINKMODE_SHIFT)
#define  WRAPPER_VWSR_ALTINKMODE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_ALTINKMODE_MASK) | (((uint32_t)val) << WRAPPER_VWSR_ALTINKMODE_SHIFT))

#define  WRAPPER_VWSR_SECUREDTCM_MASK 0x700
#define  WRAPPER_VWSR_SECUREDTCM_SHIFT 8
#define  WRAPPER_VWSR_SECUREDTCM_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_SECUREDTCM_MASK) >> WRAPPER_VWSR_SECUREDTCM_SHIFT)
#define  WRAPPER_VWSR_SECUREDTCM_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_SECUREDTCM_MASK) | (((uint32_t)val) << WRAPPER_VWSR_SECUREDTCM_SHIFT))

#define  WRAPPER_VWSR_IHASHBYPASS_MASK 0x80
#define  WRAPPER_VWSR_IHASHBYPASS_SHIFT 7
#define  WRAPPER_VWSR_IHASHBYPASS_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_IHASHBYPASS_MASK) >> WRAPPER_VWSR_IHASHBYPASS_SHIFT)
#define  WRAPPER_VWSR_IHASHBYPASS_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_IHASHBYPASS_MASK) | (((uint32_t)val) << WRAPPER_VWSR_IHASHBYPASS_SHIFT))

#define  WRAPPER_VWSR_MHASHBYPASS_MASK 0x40
#define  WRAPPER_VWSR_MHASHBYPASS_SHIFT 6
#define  WRAPPER_VWSR_MHASHBYPASS_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_MHASHBYPASS_MASK) >> WRAPPER_VWSR_MHASHBYPASS_SHIFT)
#define  WRAPPER_VWSR_MHASHBYPASS_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_MHASHBYPASS_MASK) | (((uint32_t)val) << WRAPPER_VWSR_MHASHBYPASS_SHIFT))

#define  WRAPPER_VWSR_MINFUNC_MASK 0x20
#define  WRAPPER_VWSR_MINFUNC_SHIFT 5
#define  WRAPPER_VWSR_MINFUNC_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_MINFUNC_MASK) >> WRAPPER_VWSR_MINFUNC_SHIFT)
#define  WRAPPER_VWSR_MINFUNC_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_MINFUNC_MASK) | (((uint32_t)val) << WRAPPER_VWSR_MINFUNC_SHIFT))

#define  WRAPPER_VWSR_REQUESTBLOCKALL_MASK 0x10
#define  WRAPPER_VWSR_REQUESTBLOCKALL_SHIFT 4
#define  WRAPPER_VWSR_REQUESTBLOCKALL_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_REQUESTBLOCKALL_MASK) >> WRAPPER_VWSR_REQUESTBLOCKALL_SHIFT)
#define  WRAPPER_VWSR_REQUESTBLOCKALL_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_REQUESTBLOCKALL_MASK) | (((uint32_t)val) << WRAPPER_VWSR_REQUESTBLOCKALL_SHIFT))

#define  WRAPPER_VWSR_STATEICPU1_MASK 0xc
#define  WRAPPER_VWSR_STATEICPU1_SHIFT 2
#define  WRAPPER_VWSR_STATEICPU1_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_STATEICPU1_MASK) >> WRAPPER_VWSR_STATEICPU1_SHIFT)
#define  WRAPPER_VWSR_STATEICPU1_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_STATEICPU1_MASK) | (((uint32_t)val) << WRAPPER_VWSR_STATEICPU1_SHIFT))

#define  WRAPPER_VWSR_STATEICPU0_MASK 0x3
#define  WRAPPER_VWSR_STATEICPU0_SHIFT 0
#define  WRAPPER_VWSR_STATEICPU0_MASK_SHIFT(reg) (((reg) & WRAPPER_VWSR_STATEICPU0_MASK) >> WRAPPER_VWSR_STATEICPU0_SHIFT)
#define  WRAPPER_VWSR_STATEICPU0_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VWSR_STATEICPU0_MASK) | (((uint32_t)val) << WRAPPER_VWSR_STATEICPU0_SHIFT))

//====================================================================
//Register: VCF Minimum Function Register (VMFR)
//====================================================================

#define  WRAPPER_VMFR_RESERVED1_MASK 0xfffffffe
#define  WRAPPER_VMFR_RESERVED1_SHIFT 1
#define  WRAPPER_VMFR_RESERVED1_MASK_SHIFT(reg) (((reg) & WRAPPER_VMFR_RESERVED1_MASK) >> WRAPPER_VMFR_RESERVED1_SHIFT)
#define  WRAPPER_VMFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMFR_RESERVED1_MASK) | (((uint32_t)val) << WRAPPER_VMFR_RESERVED1_SHIFT))

#define  WRAPPER_VMFR_MINFUNC_MASK 0x1
#define  WRAPPER_VMFR_MINFUNC_SHIFT 0
#define  WRAPPER_VMFR_MINFUNC_MASK_SHIFT(reg) (((reg) & WRAPPER_VMFR_MINFUNC_MASK) >> WRAPPER_VMFR_MINFUNC_SHIFT)
#define  WRAPPER_VMFR_MINFUNC_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VMFR_MINFUNC_MASK) | (((uint32_t)val) << WRAPPER_VMFR_MINFUNC_SHIFT))

//====================================================================
//Register: VCF Laser Map Register (VLMR)
//====================================================================

#define  WRAPPER_VLMR_RESERVED1_MASK 0x80000000
#define  WRAPPER_VLMR_RESERVED1_SHIFT 31
#define  WRAPPER_VLMR_RESERVED1_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED1_MASK) >> WRAPPER_VLMR_RESERVED1_SHIFT)
#define  WRAPPER_VLMR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED1_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED1_SHIFT))

#define  WRAPPER_VLMR_CHAN7_MASK 0x70000000
#define  WRAPPER_VLMR_CHAN7_SHIFT 28
#define  WRAPPER_VLMR_CHAN7_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN7_MASK) >> WRAPPER_VLMR_CHAN7_SHIFT)
#define  WRAPPER_VLMR_CHAN7_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN7_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN7_SHIFT))

#define  WRAPPER_VLMR_RESERVED2_MASK 0x8000000
#define  WRAPPER_VLMR_RESERVED2_SHIFT 27
#define  WRAPPER_VLMR_RESERVED2_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED2_MASK) >> WRAPPER_VLMR_RESERVED2_SHIFT)
#define  WRAPPER_VLMR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED2_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED2_SHIFT))

#define  WRAPPER_VLMR_CHAN6_MASK 0x7000000
#define  WRAPPER_VLMR_CHAN6_SHIFT 24
#define  WRAPPER_VLMR_CHAN6_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN6_MASK) >> WRAPPER_VLMR_CHAN6_SHIFT)
#define  WRAPPER_VLMR_CHAN6_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN6_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN6_SHIFT))

#define  WRAPPER_VLMR_RESERVED3_MASK 0x800000
#define  WRAPPER_VLMR_RESERVED3_SHIFT 23
#define  WRAPPER_VLMR_RESERVED3_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED3_MASK) >> WRAPPER_VLMR_RESERVED3_SHIFT)
#define  WRAPPER_VLMR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED3_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED3_SHIFT))

#define  WRAPPER_VLMR_CHAN5_MASK 0x700000
#define  WRAPPER_VLMR_CHAN5_SHIFT 20
#define  WRAPPER_VLMR_CHAN5_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN5_MASK) >> WRAPPER_VLMR_CHAN5_SHIFT)
#define  WRAPPER_VLMR_CHAN5_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN5_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN5_SHIFT))

#define  WRAPPER_VLMR_RESERVED4_MASK 0x80000
#define  WRAPPER_VLMR_RESERVED4_SHIFT 19
#define  WRAPPER_VLMR_RESERVED4_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED4_MASK) >> WRAPPER_VLMR_RESERVED4_SHIFT)
#define  WRAPPER_VLMR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED4_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED4_SHIFT))

#define  WRAPPER_VLMR_CHAN4_MASK 0x70000
#define  WRAPPER_VLMR_CHAN4_SHIFT 16
#define  WRAPPER_VLMR_CHAN4_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN4_MASK) >> WRAPPER_VLMR_CHAN4_SHIFT)
#define  WRAPPER_VLMR_CHAN4_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN4_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN4_SHIFT))

#define  WRAPPER_VLMR_RESERVED5_MASK 0x8000
#define  WRAPPER_VLMR_RESERVED5_SHIFT 15
#define  WRAPPER_VLMR_RESERVED5_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED5_MASK) >> WRAPPER_VLMR_RESERVED5_SHIFT)
#define  WRAPPER_VLMR_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED5_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED5_SHIFT))

#define  WRAPPER_VLMR_CHAN3_MASK 0x7000
#define  WRAPPER_VLMR_CHAN3_SHIFT 12
#define  WRAPPER_VLMR_CHAN3_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN3_MASK) >> WRAPPER_VLMR_CHAN3_SHIFT)
#define  WRAPPER_VLMR_CHAN3_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN3_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN3_SHIFT))

#define  WRAPPER_VLMR_RESERVED6_MASK 0x800
#define  WRAPPER_VLMR_RESERVED6_SHIFT 11
#define  WRAPPER_VLMR_RESERVED6_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED6_MASK) >> WRAPPER_VLMR_RESERVED6_SHIFT)
#define  WRAPPER_VLMR_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED6_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED6_SHIFT))

#define  WRAPPER_VLMR_CHAN2_MASK 0x700
#define  WRAPPER_VLMR_CHAN2_SHIFT 8
#define  WRAPPER_VLMR_CHAN2_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN2_MASK) >> WRAPPER_VLMR_CHAN2_SHIFT)
#define  WRAPPER_VLMR_CHAN2_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN2_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN2_SHIFT))

#define  WRAPPER_VLMR_RESERVED7_MASK 0x80
#define  WRAPPER_VLMR_RESERVED7_SHIFT 7
#define  WRAPPER_VLMR_RESERVED7_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED7_MASK) >> WRAPPER_VLMR_RESERVED7_SHIFT)
#define  WRAPPER_VLMR_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED7_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED7_SHIFT))

#define  WRAPPER_VLMR_CHAN1_MASK 0x70
#define  WRAPPER_VLMR_CHAN1_SHIFT 4
#define  WRAPPER_VLMR_CHAN1_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN1_MASK) >> WRAPPER_VLMR_CHAN1_SHIFT)
#define  WRAPPER_VLMR_CHAN1_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN1_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN1_SHIFT))

#define  WRAPPER_VLMR_RESERVED8_MASK 0x8
#define  WRAPPER_VLMR_RESERVED8_SHIFT 3
#define  WRAPPER_VLMR_RESERVED8_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_RESERVED8_MASK) >> WRAPPER_VLMR_RESERVED8_SHIFT)
#define  WRAPPER_VLMR_RESERVED8_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_RESERVED8_MASK) | (((uint32_t)val) << WRAPPER_VLMR_RESERVED8_SHIFT))

#define  WRAPPER_VLMR_CHAN0_MASK 0x7
#define  WRAPPER_VLMR_CHAN0_SHIFT 0
#define  WRAPPER_VLMR_CHAN0_MASK_SHIFT(reg) (((reg) & WRAPPER_VLMR_CHAN0_MASK) >> WRAPPER_VLMR_CHAN0_SHIFT)
#define  WRAPPER_VLMR_CHAN0_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLMR_CHAN0_MASK) | (((uint32_t)val) << WRAPPER_VLMR_CHAN0_SHIFT))

//====================================================================
//Register: VCF Laser Enforce Register (VLER)
//====================================================================

#define  WRAPPER_VLER_RESERVED1_MASK 0xffffff00
#define  WRAPPER_VLER_RESERVED1_SHIFT 8
#define  WRAPPER_VLER_RESERVED1_MASK_SHIFT(reg) (((reg) & WRAPPER_VLER_RESERVED1_MASK) >> WRAPPER_VLER_RESERVED1_SHIFT)
#define  WRAPPER_VLER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLER_RESERVED1_MASK) | (((uint32_t)val) << WRAPPER_VLER_RESERVED1_SHIFT))

#define  WRAPPER_VLER_LASERENFORCE_MASK 0xff
#define  WRAPPER_VLER_LASERENFORCE_SHIFT 0
#define  WRAPPER_VLER_LASERENFORCE_MASK_SHIFT(reg) (((reg) & WRAPPER_VLER_LASERENFORCE_MASK) >> WRAPPER_VLER_LASERENFORCE_SHIFT)
#define  WRAPPER_VLER_LASERENFORCE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VLER_LASERENFORCE_MASK) | (((uint32_t)val) << WRAPPER_VLER_LASERENFORCE_SHIFT))

//====================================================================
//Register: VCF Ink Enforce Register 0 (VIER0)
//====================================================================

#define  WRAPPER_VIER0_INKENFORCE_MASK 0xffffffff
#define  WRAPPER_VIER0_INKENFORCE_SHIFT 0
#define  WRAPPER_VIER0_INKENFORCE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIER0_INKENFORCE_MASK) >> WRAPPER_VIER0_INKENFORCE_SHIFT)
#define  WRAPPER_VIER0_INKENFORCE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIER0_INKENFORCE_MASK) | (((uint32_t)val) << WRAPPER_VIER0_INKENFORCE_SHIFT))

//====================================================================
//Register: VCF Ink Enforce Register 1 (VIER1)
//====================================================================

#define  WRAPPER_VIER1_RESERVED1_MASK 0xfffe0000
#define  WRAPPER_VIER1_RESERVED1_SHIFT 17
#define  WRAPPER_VIER1_RESERVED1_MASK_SHIFT(reg) (((reg) & WRAPPER_VIER1_RESERVED1_MASK) >> WRAPPER_VIER1_RESERVED1_SHIFT)
#define  WRAPPER_VIER1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIER1_RESERVED1_MASK) | (((uint32_t)val) << WRAPPER_VIER1_RESERVED1_SHIFT))

#define  WRAPPER_VIER1_INKENFORCEEXTRA_MASK 0x10000
#define  WRAPPER_VIER1_INKENFORCEEXTRA_SHIFT 16
#define  WRAPPER_VIER1_INKENFORCEEXTRA_MASK_SHIFT(reg) (((reg) & WRAPPER_VIER1_INKENFORCEEXTRA_MASK) >> WRAPPER_VIER1_INKENFORCEEXTRA_SHIFT)
#define  WRAPPER_VIER1_INKENFORCEEXTRA_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIER1_INKENFORCEEXTRA_MASK) | (((uint32_t)val) << WRAPPER_VIER1_INKENFORCEEXTRA_SHIFT))

#define  WRAPPER_VIER1_INKENFORCE_MASK 0xffff
#define  WRAPPER_VIER1_INKENFORCE_SHIFT 0
#define  WRAPPER_VIER1_INKENFORCE_MASK_SHIFT(reg) (((reg) & WRAPPER_VIER1_INKENFORCE_MASK) >> WRAPPER_VIER1_INKENFORCE_SHIFT)
#define  WRAPPER_VIER1_INKENFORCE_REPLACE_VAL(reg,val) (((reg) & ~WRAPPER_VIER1_INKENFORCE_MASK) | (((uint32_t)val) << WRAPPER_VIER1_INKENFORCE_SHIFT))

//====================================================================
//
//Register File: ROM (ROM)
//
//====================================================================

//====================================================================
//Register: ROM (ROM)
//====================================================================

#define  ROM_ROM_VALUE_MASK 0xffffffff
#define  ROM_ROM_VALUE_SHIFT 0
#define  ROM_ROM_VALUE_MASK_SHIFT(reg) (((reg) & ROM_ROM_VALUE_MASK) >> ROM_ROM_VALUE_SHIFT)
#define  ROM_ROM_VALUE_REPLACE_VAL(reg,val) (((reg) & ~ROM_ROM_VALUE_MASK) | (((uint32_t)val) << ROM_ROM_VALUE_SHIFT))

#endif // VCF
