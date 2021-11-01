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
 * \file VCF_coremasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_CORE_REGMASKS_H_
#define _VCF_CORE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF Core Registers (VCF_Core)
//
//====================================================================

//====================================================================
//Register: VCF_STATUS (VCF_STATUS)
//====================================================================

#define  VCF_CORE_VCF_STATUS_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_STATUS_VALUE_SHIFT 0
#define  VCF_CORE_VCF_STATUS_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_STATUS_VALUE_MASK) >> VCF_CORE_VCF_STATUS_VALUE_SHIFT)
#define  VCF_CORE_VCF_STATUS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_STATUS_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_STATUS_VALUE_SHIFT))

//====================================================================
//Register: VCF_ENFORCE (VCF_ENFORCE)
//====================================================================

#define  VCF_CORE_VCF_ENFORCE_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_ENFORCE_VALUE_SHIFT 0
#define  VCF_CORE_VCF_ENFORCE_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_ENFORCE_VALUE_MASK) >> VCF_CORE_VCF_ENFORCE_VALUE_SHIFT)
#define  VCF_CORE_VCF_ENFORCE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_ENFORCE_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_ENFORCE_VALUE_SHIFT))

//====================================================================
//Register: VCF_PRE_ENFORCE (VCF_PRE_ENFORCE)
//====================================================================

#define  VCF_CORE_VCF_PRE_ENFORCE_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_PRE_ENFORCE_VALUE_SHIFT 0
#define  VCF_CORE_VCF_PRE_ENFORCE_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_PRE_ENFORCE_VALUE_MASK) >> VCF_CORE_VCF_PRE_ENFORCE_VALUE_SHIFT)
#define  VCF_CORE_VCF_PRE_ENFORCE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_PRE_ENFORCE_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_PRE_ENFORCE_VALUE_SHIFT))

//====================================================================
//Register: VCF_TRIGGERS (VCF_TRIGGERS)
//====================================================================

#define  VCF_CORE_VCF_TRIGGERS_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_TRIGGERS_VALUE_SHIFT 0
#define  VCF_CORE_VCF_TRIGGERS_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_TRIGGERS_VALUE_MASK) >> VCF_CORE_VCF_TRIGGERS_VALUE_SHIFT)
#define  VCF_CORE_VCF_TRIGGERS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_TRIGGERS_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_TRIGGERS_VALUE_SHIFT))

//====================================================================
//Register: VCF_ERROR (VCF_ERROR)
//====================================================================

#define  VCF_CORE_VCF_ERROR_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_ERROR_VALUE_SHIFT 0
#define  VCF_CORE_VCF_ERROR_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_ERROR_VALUE_MASK) >> VCF_CORE_VCF_ERROR_VALUE_SHIFT)
#define  VCF_CORE_VCF_ERROR_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_ERROR_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_ERROR_VALUE_SHIFT))

//====================================================================
//Register: VCF_IRQ_SOURCE (VCF_IRQ_SOURCE)
//====================================================================

#define  VCF_CORE_VCF_IRQ_SOURCE_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_IRQ_SOURCE_VALUE_SHIFT 0
#define  VCF_CORE_VCF_IRQ_SOURCE_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_IRQ_SOURCE_VALUE_MASK) >> VCF_CORE_VCF_IRQ_SOURCE_VALUE_SHIFT)
#define  VCF_CORE_VCF_IRQ_SOURCE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_IRQ_SOURCE_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_IRQ_SOURCE_VALUE_SHIFT))

//====================================================================
//Register: VCF_PIN_MIRROR (VCF_PIN_MIRROR)
//====================================================================

#define  VCF_CORE_VCF_PIN_MIRROR_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_PIN_MIRROR_VALUE_SHIFT 0
#define  VCF_CORE_VCF_PIN_MIRROR_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_PIN_MIRROR_VALUE_MASK) >> VCF_CORE_VCF_PIN_MIRROR_VALUE_SHIFT)
#define  VCF_CORE_VCF_PIN_MIRROR_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_PIN_MIRROR_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_PIN_MIRROR_VALUE_SHIFT))

//====================================================================
//Register: VCF_PRNG (VCF_PRNG)
//====================================================================

#define  VCF_CORE_VCF_PRNG_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_PRNG_VALUE_SHIFT 0
#define  VCF_CORE_VCF_PRNG_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_PRNG_VALUE_MASK) >> VCF_CORE_VCF_PRNG_VALUE_SHIFT)
#define  VCF_CORE_VCF_PRNG_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_PRNG_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_PRNG_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 1 of 4) (VCF_FUSE0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_FUSE0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FUSE0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FUSE0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FUSE0_VALUE_MASK) >> VCF_CORE_VCF_FUSE0_VALUE_SHIFT)
#define  VCF_CORE_VCF_FUSE0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FUSE0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FUSE0_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 2 of 4) (VCF_FUSE1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_FUSE1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FUSE1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FUSE1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FUSE1_VALUE_MASK) >> VCF_CORE_VCF_FUSE1_VALUE_SHIFT)
#define  VCF_CORE_VCF_FUSE1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FUSE1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FUSE1_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 3 of 4) (VCF_FUSE2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_FUSE2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FUSE2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FUSE2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FUSE2_VALUE_MASK) >> VCF_CORE_VCF_FUSE2_VALUE_SHIFT)
#define  VCF_CORE_VCF_FUSE2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FUSE2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FUSE2_VALUE_SHIFT))

//====================================================================
//Register: VCF_FUSE (Instance 4 of 4) (VCF_FUSE3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_FUSE3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FUSE3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FUSE3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FUSE3_VALUE_MASK) >> VCF_CORE_VCF_FUSE3_VALUE_SHIFT)
#define  VCF_CORE_VCF_FUSE3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FUSE3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FUSE3_VALUE_SHIFT))

//====================================================================
//Register: VCF_EVENT (VCF_EVENT)
//====================================================================

#define  VCF_CORE_VCF_EVENT_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_EVENT_VALUE_SHIFT 0
#define  VCF_CORE_VCF_EVENT_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_EVENT_VALUE_MASK) >> VCF_CORE_VCF_EVENT_VALUE_SHIFT)
#define  VCF_CORE_VCF_EVENT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_EVENT_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_EVENT_VALUE_SHIFT))

//====================================================================
//Register: VCF_TIMERS (VCF_TIMERS)
//====================================================================

#define  VCF_CORE_VCF_TIMERS_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_TIMERS_VALUE_SHIFT 0
#define  VCF_CORE_VCF_TIMERS_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_TIMERS_VALUE_MASK) >> VCF_CORE_VCF_TIMERS_VALUE_SHIFT)
#define  VCF_CORE_VCF_TIMERS_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_TIMERS_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_TIMERS_VALUE_SHIFT))

//====================================================================
//Register: VCF_VERSION (VCF_VERSION)
//====================================================================

#define  VCF_CORE_VCF_VERSION_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VERSION_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VERSION_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VERSION_VALUE_MASK) >> VCF_CORE_VCF_VERSION_VALUE_SHIFT)
#define  VCF_CORE_VCF_VERSION_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VERSION_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VERSION_VALUE_SHIFT))

//====================================================================
//Register: VCF_RECON_DATA (Instance 1 of 3) (VCF_RECON_DATA0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_RECON_DATA0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_RECON_DATA0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_RECON_DATA0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_RECON_DATA0_VALUE_MASK) >> VCF_CORE_VCF_RECON_DATA0_VALUE_SHIFT)
#define  VCF_CORE_VCF_RECON_DATA0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_RECON_DATA0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_RECON_DATA0_VALUE_SHIFT))

//====================================================================
//Register: VCF_RECON_DATA (Instance 2 of 3) (VCF_RECON_DATA1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_RECON_DATA1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_RECON_DATA1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_RECON_DATA1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_RECON_DATA1_VALUE_MASK) >> VCF_CORE_VCF_RECON_DATA1_VALUE_SHIFT)
#define  VCF_CORE_VCF_RECON_DATA1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_RECON_DATA1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_RECON_DATA1_VALUE_SHIFT))

//====================================================================
//Register: VCF_RECON_DATA (Instance 3 of 3) (VCF_RECON_DATA2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_RECON_DATA2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_RECON_DATA2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_RECON_DATA2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_RECON_DATA2_VALUE_MASK) >> VCF_CORE_VCF_RECON_DATA2_VALUE_SHIFT)
#define  VCF_CORE_VCF_RECON_DATA2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_RECON_DATA2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_RECON_DATA2_VALUE_SHIFT))

//====================================================================
//Register: VCF_INPUT (VCF_INPUT)
//====================================================================

#define  VCF_CORE_VCF_INPUT_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_INPUT_VALUE_SHIFT 0
#define  VCF_CORE_VCF_INPUT_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_INPUT_VALUE_MASK) >> VCF_CORE_VCF_INPUT_VALUE_SHIFT)
#define  VCF_CORE_VCF_INPUT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_INPUT_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_INPUT_VALUE_SHIFT))

//====================================================================
//Register: VCF_OUTPUT (VCF_OUTPUT)
//====================================================================

#define  VCF_CORE_VCF_OUTPUT_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_OUTPUT_VALUE_SHIFT 0
#define  VCF_CORE_VCF_OUTPUT_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_OUTPUT_VALUE_MASK) >> VCF_CORE_VCF_OUTPUT_VALUE_SHIFT)
#define  VCF_CORE_VCF_OUTPUT_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_OUTPUT_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_OUTPUT_VALUE_SHIFT))

//====================================================================
//Register: VCF_IRQ_MASK (VCF_IRQ_MASK)
//====================================================================

#define  VCF_CORE_VCF_IRQ_MASK_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_IRQ_MASK_VALUE_SHIFT 0
#define  VCF_CORE_VCF_IRQ_MASK_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_IRQ_MASK_VALUE_MASK) >> VCF_CORE_VCF_IRQ_MASK_VALUE_SHIFT)
#define  VCF_CORE_VCF_IRQ_MASK_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_IRQ_MASK_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_IRQ_MASK_VALUE_SHIFT))

//====================================================================
//Register: VCF_IRQ_STATE (VCF_IRQ_STATE)
//====================================================================

#define  VCF_CORE_VCF_IRQ_STATE_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_IRQ_STATE_VALUE_SHIFT 0
#define  VCF_CORE_VCF_IRQ_STATE_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_IRQ_STATE_VALUE_MASK) >> VCF_CORE_VCF_IRQ_STATE_VALUE_SHIFT)
#define  VCF_CORE_VCF_IRQ_STATE_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_IRQ_STATE_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_IRQ_STATE_VALUE_SHIFT))

//====================================================================
//Register: VCF_MAINT_WINDOW (VCF_MAINT_WINDOW)
//====================================================================

#define  VCF_CORE_VCF_MAINT_WINDOW_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_MAINT_WINDOW_VALUE_SHIFT 0
#define  VCF_CORE_VCF_MAINT_WINDOW_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_MAINT_WINDOW_VALUE_MASK) >> VCF_CORE_VCF_MAINT_WINDOW_VALUE_SHIFT)
#define  VCF_CORE_VCF_MAINT_WINDOW_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_MAINT_WINDOW_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_MAINT_WINDOW_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_ADJ (VCF_WARN_ADJ)
//====================================================================

#define  VCF_CORE_VCF_WARN_ADJ_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_ADJ_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_ADJ_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_ADJ_VALUE_MASK) >> VCF_CORE_VCF_WARN_ADJ_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_ADJ_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_ADJ_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_ADJ_VALUE_SHIFT))

//====================================================================
//Register: VCF_DIAG (VCF_DIAG)
//====================================================================

#define  VCF_CORE_VCF_DIAG_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_DIAG_VALUE_SHIFT 0
#define  VCF_CORE_VCF_DIAG_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_DIAG_VALUE_MASK) >> VCF_CORE_VCF_DIAG_VALUE_SHIFT)
#define  VCF_CORE_VCF_DIAG_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_DIAG_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_DIAG_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT0 (Instance 1 of 3) (VCF_FCERT00)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_FCERT00_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FCERT00_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FCERT00_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FCERT00_VALUE_MASK) >> VCF_CORE_VCF_FCERT00_VALUE_SHIFT)
#define  VCF_CORE_VCF_FCERT00_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FCERT00_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FCERT00_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT0 (Instance 2 of 3) (VCF_FCERT01)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_FCERT01_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FCERT01_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FCERT01_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FCERT01_VALUE_MASK) >> VCF_CORE_VCF_FCERT01_VALUE_SHIFT)
#define  VCF_CORE_VCF_FCERT01_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FCERT01_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FCERT01_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT0 (Instance 3 of 3) (VCF_FCERT02)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_FCERT02_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FCERT02_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FCERT02_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FCERT02_VALUE_MASK) >> VCF_CORE_VCF_FCERT02_VALUE_SHIFT)
#define  VCF_CORE_VCF_FCERT02_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FCERT02_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FCERT02_VALUE_SHIFT))

//====================================================================
//Register: VCF_FCERT1 (VCF_FCERT1)
//====================================================================

#define  VCF_CORE_VCF_FCERT1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_FCERT1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_FCERT1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_FCERT1_VALUE_MASK) >> VCF_CORE_VCF_FCERT1_VALUE_SHIFT)
#define  VCF_CORE_VCF_FCERT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_FCERT1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_FCERT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 1 of 7) (VCF_CCERT00)
/** \brief  (Instance 1 of 7)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT00_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT00_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT00_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT00_VALUE_MASK) >> VCF_CORE_VCF_CCERT00_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT00_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT00_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT00_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 2 of 7) (VCF_CCERT01)
/** \brief  (Instance 2 of 7)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT01_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT01_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT01_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT01_VALUE_MASK) >> VCF_CORE_VCF_CCERT01_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT01_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT01_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT01_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 3 of 7) (VCF_CCERT02)
/** \brief  (Instance 3 of 7)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT02_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT02_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT02_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT02_VALUE_MASK) >> VCF_CORE_VCF_CCERT02_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT02_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT02_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT02_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 4 of 7) (VCF_CCERT03)
/** \brief  (Instance 4 of 7)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT03_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT03_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT03_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT03_VALUE_MASK) >> VCF_CORE_VCF_CCERT03_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT03_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT03_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT03_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 5 of 7) (VCF_CCERT04)
/** \brief  (Instance 5 of 7)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT04_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT04_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT04_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT04_VALUE_MASK) >> VCF_CORE_VCF_CCERT04_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT04_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT04_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT04_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 6 of 7) (VCF_CCERT05)
/** \brief  (Instance 6 of 7)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT05_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT05_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT05_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT05_VALUE_MASK) >> VCF_CORE_VCF_CCERT05_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT05_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT05_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT05_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT0 (Instance 7 of 7) (VCF_CCERT06)
/** \brief  (Instance 7 of 7)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT06_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT06_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT06_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT06_VALUE_MASK) >> VCF_CORE_VCF_CCERT06_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT06_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT06_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT06_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 1 of 13) (VCF_CCERT10)
/** \brief  (Instance 1 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT10_VALUE_MASK) >> VCF_CORE_VCF_CCERT10_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT10_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 2 of 13) (VCF_CCERT11)
/** \brief  (Instance 2 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT11_VALUE_MASK) >> VCF_CORE_VCF_CCERT11_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT11_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 3 of 13) (VCF_CCERT12)
/** \brief  (Instance 3 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT12_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT12_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT12_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT12_VALUE_MASK) >> VCF_CORE_VCF_CCERT12_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT12_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT12_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT12_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 4 of 13) (VCF_CCERT13)
/** \brief  (Instance 4 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT13_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT13_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT13_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT13_VALUE_MASK) >> VCF_CORE_VCF_CCERT13_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT13_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT13_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT13_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 5 of 13) (VCF_CCERT14)
/** \brief  (Instance 5 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT14_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT14_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT14_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT14_VALUE_MASK) >> VCF_CORE_VCF_CCERT14_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT14_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT14_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT14_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 6 of 13) (VCF_CCERT15)
/** \brief  (Instance 6 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT15_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT15_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT15_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT15_VALUE_MASK) >> VCF_CORE_VCF_CCERT15_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT15_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT15_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT15_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 7 of 13) (VCF_CCERT16)
/** \brief  (Instance 7 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT16_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT16_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT16_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT16_VALUE_MASK) >> VCF_CORE_VCF_CCERT16_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT16_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT16_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT16_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 8 of 13) (VCF_CCERT17)
/** \brief  (Instance 8 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT17_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT17_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT17_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT17_VALUE_MASK) >> VCF_CORE_VCF_CCERT17_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT17_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT17_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT17_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 9 of 13) (VCF_CCERT18)
/** \brief  (Instance 9 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT18_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT18_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT18_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT18_VALUE_MASK) >> VCF_CORE_VCF_CCERT18_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT18_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT18_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT18_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 10 of 13) (VCF_CCERT19)
/** \brief  (Instance 10 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT19_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT19_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT19_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT19_VALUE_MASK) >> VCF_CORE_VCF_CCERT19_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT19_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT19_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT19_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 11 of 13) (VCF_CCERT110)
/** \brief  (Instance 11 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT110_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT110_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT110_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT110_VALUE_MASK) >> VCF_CORE_VCF_CCERT110_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT110_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT110_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT110_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 12 of 13) (VCF_CCERT111)
/** \brief  (Instance 12 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT111_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT111_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT111_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT111_VALUE_MASK) >> VCF_CORE_VCF_CCERT111_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT111_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT111_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT111_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT1 (Instance 13 of 13) (VCF_CCERT112)
/** \brief  (Instance 13 of 13)*/
//====================================================================

#define  VCF_CORE_VCF_CCERT112_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT112_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT112_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT112_VALUE_MASK) >> VCF_CORE_VCF_CCERT112_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT112_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT112_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT112_VALUE_SHIFT))

//====================================================================
//Register: VCF_CCERT2 (VCF_CCERT2)
//====================================================================

#define  VCF_CORE_VCF_CCERT2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CCERT2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CCERT2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CCERT2_VALUE_MASK) >> VCF_CORE_VCF_CCERT2_VALUE_SHIFT)
#define  VCF_CORE_VCF_CCERT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CCERT2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CCERT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 1 of 9) (VCF_VCERT00)
/** \brief  (Instance 1 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT00_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT00_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT00_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT00_VALUE_MASK) >> VCF_CORE_VCF_VCERT00_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT00_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT00_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT00_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 2 of 9) (VCF_VCERT01)
/** \brief  (Instance 2 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT01_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT01_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT01_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT01_VALUE_MASK) >> VCF_CORE_VCF_VCERT01_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT01_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT01_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT01_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 3 of 9) (VCF_VCERT02)
/** \brief  (Instance 3 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT02_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT02_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT02_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT02_VALUE_MASK) >> VCF_CORE_VCF_VCERT02_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT02_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT02_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT02_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 4 of 9) (VCF_VCERT03)
/** \brief  (Instance 4 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT03_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT03_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT03_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT03_VALUE_MASK) >> VCF_CORE_VCF_VCERT03_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT03_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT03_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT03_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 5 of 9) (VCF_VCERT04)
/** \brief  (Instance 5 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT04_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT04_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT04_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT04_VALUE_MASK) >> VCF_CORE_VCF_VCERT04_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT04_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT04_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT04_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 6 of 9) (VCF_VCERT05)
/** \brief  (Instance 6 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT05_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT05_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT05_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT05_VALUE_MASK) >> VCF_CORE_VCF_VCERT05_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT05_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT05_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT05_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 7 of 9) (VCF_VCERT06)
/** \brief  (Instance 7 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT06_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT06_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT06_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT06_VALUE_MASK) >> VCF_CORE_VCF_VCERT06_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT06_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT06_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT06_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 8 of 9) (VCF_VCERT07)
/** \brief  (Instance 8 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT07_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT07_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT07_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT07_VALUE_MASK) >> VCF_CORE_VCF_VCERT07_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT07_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT07_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT07_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT0 (Instance 9 of 9) (VCF_VCERT08)
/** \brief  (Instance 9 of 9)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT08_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT08_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT08_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT08_VALUE_MASK) >> VCF_CORE_VCF_VCERT08_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT08_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT08_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT08_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_COMPAT (Instance 1 of 3) (VCF_VCERT_COMPAT0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_COMPAT0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_COMPAT0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_COMPAT0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_COMPAT0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_COMPAT0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_COMPAT0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_COMPAT0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_COMPAT0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_COMPAT (Instance 2 of 3) (VCF_VCERT_COMPAT1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_COMPAT1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_COMPAT1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_COMPAT1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_COMPAT1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_COMPAT1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_COMPAT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_COMPAT1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_COMPAT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_COMPAT (Instance 3 of 3) (VCF_VCERT_COMPAT2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_COMPAT2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_COMPAT2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_COMPAT2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_COMPAT2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_COMPAT2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_COMPAT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_COMPAT2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_COMPAT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_TIMER (VCF_VCERT_TIMER)
//====================================================================

#define  VCF_CORE_VCF_VCERT_TIMER_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_TIMER_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_TIMER_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_TIMER_VALUE_MASK) >> VCF_CORE_VCF_VCERT_TIMER_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_TIMER_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_TIMER_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_TIMER_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 1 of 4) (VCF_VCERT_MULT_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 2 of 4) (VCF_VCERT_MULT_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 3 of 4) (VCF_VCERT_MULT_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CL (Instance 4 of 4) (VCF_VCERT_MULT_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 1 of 12) (VCF_VCERT_MULT_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 2 of 12) (VCF_VCERT_MULT_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 3 of 12) (VCF_VCERT_MULT_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 4 of 12) (VCF_VCERT_MULT_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 5 of 12) (VCF_VCERT_MULT_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 6 of 12) (VCF_VCERT_MULT_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 7 of 12) (VCF_VCERT_MULT_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 8 of 12) (VCF_VCERT_MULT_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 9 of 12) (VCF_VCERT_MULT_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 10 of 12) (VCF_VCERT_MULT_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 11 of 12) (VCF_VCERT_MULT_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_MULT_CH (Instance 12 of 12) (VCF_VCERT_MULT_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_MASK) >> VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_MULT_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 1 of 4) (VCF_VCERT_USE_THRESH_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 2 of 4) (VCF_VCERT_USE_THRESH_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 3 of 4) (VCF_VCERT_USE_THRESH_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CL (Instance 4 of 4) (VCF_VCERT_USE_THRESH_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 1 of 12) (VCF_VCERT_USE_THRESH_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 2 of 12) (VCF_VCERT_USE_THRESH_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 3 of 12) (VCF_VCERT_USE_THRESH_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 4 of 12) (VCF_VCERT_USE_THRESH_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 5 of 12) (VCF_VCERT_USE_THRESH_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 6 of 12) (VCF_VCERT_USE_THRESH_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 7 of 12) (VCF_VCERT_USE_THRESH_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 8 of 12) (VCF_VCERT_USE_THRESH_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 9 of 12) (VCF_VCERT_USE_THRESH_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 10 of 12) (VCF_VCERT_USE_THRESH_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 11 of 12) (VCF_VCERT_USE_THRESH_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_USE_THRESH_CH (Instance 12 of 12) (VCF_VCERT_USE_THRESH_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK) >> VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_USE_THRESH_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 1 of 4) (VCF_VCERT_DATA1_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 2 of 4) (VCF_VCERT_DATA1_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 3 of 4) (VCF_VCERT_DATA1_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CL (Instance 4 of 4) (VCF_VCERT_DATA1_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 1 of 12) (VCF_VCERT_DATA1_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 2 of 12) (VCF_VCERT_DATA1_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 3 of 12) (VCF_VCERT_DATA1_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 4 of 12) (VCF_VCERT_DATA1_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 5 of 12) (VCF_VCERT_DATA1_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 6 of 12) (VCF_VCERT_DATA1_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 7 of 12) (VCF_VCERT_DATA1_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 8 of 12) (VCF_VCERT_DATA1_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 9 of 12) (VCF_VCERT_DATA1_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 10 of 12) (VCF_VCERT_DATA1_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 11 of 12) (VCF_VCERT_DATA1_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA1_CH (Instance 12 of 12) (VCF_VCERT_DATA1_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA1_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 1 of 4) (VCF_VCERT_DATA2_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 2 of 4) (VCF_VCERT_DATA2_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 3 of 4) (VCF_VCERT_DATA2_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CL (Instance 4 of 4) (VCF_VCERT_DATA2_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 1 of 12) (VCF_VCERT_DATA2_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 2 of 12) (VCF_VCERT_DATA2_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 3 of 12) (VCF_VCERT_DATA2_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 4 of 12) (VCF_VCERT_DATA2_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 5 of 12) (VCF_VCERT_DATA2_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 6 of 12) (VCF_VCERT_DATA2_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 7 of 12) (VCF_VCERT_DATA2_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 8 of 12) (VCF_VCERT_DATA2_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 9 of 12) (VCF_VCERT_DATA2_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 10 of 12) (VCF_VCERT_DATA2_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 11 of 12) (VCF_VCERT_DATA2_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_VCERT_DATA2_CH (Instance 12 of 12) (VCF_VCERT_DATA2_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK) >> VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_VCERT_DATA2_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 1 of 4) (VCF_WARN_USE_THRESH_CL0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CL0_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 2 of 4) (VCF_WARN_USE_THRESH_CL1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CL1_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 3 of 4) (VCF_WARN_USE_THRESH_CL2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CL2_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CL (Instance 4 of 4) (VCF_WARN_USE_THRESH_CL3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CL3_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 1 of 12) (VCF_WARN_USE_THRESH_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 2 of 12) (VCF_WARN_USE_THRESH_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 3 of 12) (VCF_WARN_USE_THRESH_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 4 of 12) (VCF_WARN_USE_THRESH_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 5 of 12) (VCF_WARN_USE_THRESH_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 6 of 12) (VCF_WARN_USE_THRESH_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 7 of 12) (VCF_WARN_USE_THRESH_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 8 of 12) (VCF_WARN_USE_THRESH_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 9 of 12) (VCF_WARN_USE_THRESH_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 10 of 12) (VCF_WARN_USE_THRESH_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 11 of 12) (VCF_WARN_USE_THRESH_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_WARN_USE_THRESH_CH (Instance 12 of 12) (VCF_WARN_USE_THRESH_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK) >> VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_WARN_USE_THRESH_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 1 of 12) (VCF_AUTH_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH0_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 2 of 12) (VCF_AUTH_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH1_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 3 of 12) (VCF_AUTH_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH2_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 4 of 12) (VCF_AUTH_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH3_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 5 of 12) (VCF_AUTH_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH4_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 6 of 12) (VCF_AUTH_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH5_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 7 of 12) (VCF_AUTH_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH6_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 8 of 12) (VCF_AUTH_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH7_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 9 of 12) (VCF_AUTH_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH8_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 10 of 12) (VCF_AUTH_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH9_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 11 of 12) (VCF_AUTH_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH10_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTH_CH (Instance 12 of 12) (VCF_AUTH_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTH_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTH_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTH_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTH_CH11_VALUE_MASK) >> VCF_CORE_VCF_AUTH_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTH_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTH_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTH_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 1 of 12) (VCF_AUTHUSE_LO_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 2 of 12) (VCF_AUTHUSE_LO_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 3 of 12) (VCF_AUTHUSE_LO_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 4 of 12) (VCF_AUTHUSE_LO_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 5 of 12) (VCF_AUTHUSE_LO_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 6 of 12) (VCF_AUTHUSE_LO_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 7 of 12) (VCF_AUTHUSE_LO_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 8 of 12) (VCF_AUTHUSE_LO_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 9 of 12) (VCF_AUTHUSE_LO_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 10 of 12) (VCF_AUTHUSE_LO_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 11 of 12) (VCF_AUTHUSE_LO_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_LO_CH (Instance 12 of 12) (VCF_AUTHUSE_LO_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_LO_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 1 of 12) (VCF_AUTHUSE_HI_CH0)
/** \brief  (Instance 1 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH0_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 2 of 12) (VCF_AUTHUSE_HI_CH1)
/** \brief  (Instance 2 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH1_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 3 of 12) (VCF_AUTHUSE_HI_CH2)
/** \brief  (Instance 3 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH2_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 4 of 12) (VCF_AUTHUSE_HI_CH3)
/** \brief  (Instance 4 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH3_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 5 of 12) (VCF_AUTHUSE_HI_CH4)
/** \brief  (Instance 5 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH4_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 6 of 12) (VCF_AUTHUSE_HI_CH5)
/** \brief  (Instance 6 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH5_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 7 of 12) (VCF_AUTHUSE_HI_CH6)
/** \brief  (Instance 7 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH6_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 8 of 12) (VCF_AUTHUSE_HI_CH7)
/** \brief  (Instance 8 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH7_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 9 of 12) (VCF_AUTHUSE_HI_CH8)
/** \brief  (Instance 9 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH8_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 10 of 12) (VCF_AUTHUSE_HI_CH9)
/** \brief  (Instance 10 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH9_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 11 of 12) (VCF_AUTHUSE_HI_CH10)
/** \brief  (Instance 11 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH10_VALUE_SHIFT))

//====================================================================
//Register: VCF_AUTHUSE_HI_CH (Instance 12 of 12) (VCF_AUTHUSE_HI_CH11)
/** \brief  (Instance 12 of 12)*/
//====================================================================

#define  VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_SHIFT 0
#define  VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK) >> VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_SHIFT)
#define  VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_AUTHUSE_HI_CH11_VALUE_SHIFT))

//====================================================================
//Register: VCF_OP_METER_LO (VCF_OP_METER_LO)
//====================================================================

#define  VCF_CORE_VCF_OP_METER_LO_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_OP_METER_LO_VALUE_SHIFT 0
#define  VCF_CORE_VCF_OP_METER_LO_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_OP_METER_LO_VALUE_MASK) >> VCF_CORE_VCF_OP_METER_LO_VALUE_SHIFT)
#define  VCF_CORE_VCF_OP_METER_LO_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_OP_METER_LO_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_OP_METER_LO_VALUE_SHIFT))

//====================================================================
//Register: VCF_OP_METER_HI (VCF_OP_METER_HI)
//====================================================================

#define  VCF_CORE_VCF_OP_METER_HI_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_OP_METER_HI_VALUE_SHIFT 0
#define  VCF_CORE_VCF_OP_METER_HI_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_OP_METER_HI_VALUE_MASK) >> VCF_CORE_VCF_OP_METER_HI_VALUE_SHIFT)
#define  VCF_CORE_VCF_OP_METER_HI_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_OP_METER_HI_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_OP_METER_HI_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 1 of 10) (VCF_SCERT0)
/** \brief  (Instance 1 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT0_VALUE_MASK) >> VCF_CORE_VCF_SCERT0_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT0_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 2 of 10) (VCF_SCERT1)
/** \brief  (Instance 2 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT1_VALUE_MASK) >> VCF_CORE_VCF_SCERT1_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 3 of 10) (VCF_SCERT2)
/** \brief  (Instance 3 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT2_VALUE_MASK) >> VCF_CORE_VCF_SCERT2_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 4 of 10) (VCF_SCERT3)
/** \brief  (Instance 4 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT3_VALUE_MASK) >> VCF_CORE_VCF_SCERT3_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT3_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 5 of 10) (VCF_SCERT4)
/** \brief  (Instance 5 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT4_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT4_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT4_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT4_VALUE_MASK) >> VCF_CORE_VCF_SCERT4_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT4_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT4_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT4_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 6 of 10) (VCF_SCERT5)
/** \brief  (Instance 6 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT5_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT5_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT5_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT5_VALUE_MASK) >> VCF_CORE_VCF_SCERT5_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT5_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT5_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT5_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 7 of 10) (VCF_SCERT6)
/** \brief  (Instance 7 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT6_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT6_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT6_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT6_VALUE_MASK) >> VCF_CORE_VCF_SCERT6_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT6_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT6_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT6_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 8 of 10) (VCF_SCERT7)
/** \brief  (Instance 8 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT7_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT7_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT7_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT7_VALUE_MASK) >> VCF_CORE_VCF_SCERT7_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT7_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT7_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT7_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 9 of 10) (VCF_SCERT8)
/** \brief  (Instance 9 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT8_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT8_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT8_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT8_VALUE_MASK) >> VCF_CORE_VCF_SCERT8_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT8_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT8_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT8_VALUE_SHIFT))

//====================================================================
//Register: VCF_SCERT (Instance 10 of 10) (VCF_SCERT9)
/** \brief  (Instance 10 of 10)*/
//====================================================================

#define  VCF_CORE_VCF_SCERT9_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_SCERT9_VALUE_SHIFT 0
#define  VCF_CORE_VCF_SCERT9_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_SCERT9_VALUE_MASK) >> VCF_CORE_VCF_SCERT9_VALUE_SHIFT)
#define  VCF_CORE_VCF_SCERT9_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_SCERT9_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_SCERT9_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 1 of 4) (VCF_CERT0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_CERT0_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CERT0_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CERT0_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CERT0_VALUE_MASK) >> VCF_CORE_VCF_CERT0_VALUE_SHIFT)
#define  VCF_CORE_VCF_CERT0_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CERT0_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CERT0_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 2 of 4) (VCF_CERT1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_CERT1_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CERT1_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CERT1_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CERT1_VALUE_MASK) >> VCF_CORE_VCF_CERT1_VALUE_SHIFT)
#define  VCF_CORE_VCF_CERT1_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CERT1_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CERT1_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 3 of 4) (VCF_CERT2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_CERT2_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CERT2_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CERT2_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CERT2_VALUE_MASK) >> VCF_CORE_VCF_CERT2_VALUE_SHIFT)
#define  VCF_CORE_VCF_CERT2_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CERT2_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CERT2_VALUE_SHIFT))

//====================================================================
//Register: VCF_CERT (Instance 4 of 4) (VCF_CERT3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  VCF_CORE_VCF_CERT3_VALUE_MASK 0xffffffff
#define  VCF_CORE_VCF_CERT3_VALUE_SHIFT 0
#define  VCF_CORE_VCF_CERT3_VALUE_MASK_SHIFT(reg) (((reg) & VCF_CORE_VCF_CERT3_VALUE_MASK) >> VCF_CORE_VCF_CERT3_VALUE_SHIFT)
#define  VCF_CORE_VCF_CERT3_VALUE_REPLACE_VAL(reg,val) (((reg) & ~VCF_CORE_VCF_CERT3_VALUE_MASK) | (((uint32_t)val) << VCF_CORE_VCF_CERT3_VALUE_SHIFT))

#endif // VCF_CORE
