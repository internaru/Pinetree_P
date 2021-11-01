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
 * \file MVDO_top_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _MVDO_TOP_REGMASKS_H_
#define _MVDO_TOP_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: MVDO_top (MVDO_top)
//
//====================================================================

//====================================================================
//Register: MVDO Interrupt Status Register (MVDOInt)
/** \brief Contains the status of current MVDO interrupts*/
//====================================================================

#define  MVDO_TOP_MVDOINT_RESERVED1_MASK 0xfffffe00
#define  MVDO_TOP_MVDOINT_RESERVED1_SHIFT 9
#define  MVDO_TOP_MVDOINT_RESERVED1_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOINT_RESERVED1_MASK) >> MVDO_TOP_MVDOINT_RESERVED1_SHIFT)
#define  MVDO_TOP_MVDOINT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOINT_RESERVED1_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOINT_RESERVED1_SHIFT))

#define  MVDO_TOP_MVDOINT_HVSYNCINT_MASK 0x100
#define  MVDO_TOP_MVDOINT_HVSYNCINT_SHIFT 8
#define  MVDO_TOP_MVDOINT_HVSYNCINT_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOINT_HVSYNCINT_MASK) >> MVDO_TOP_MVDOINT_HVSYNCINT_SHIFT)
#define  MVDO_TOP_MVDOINT_HVSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOINT_HVSYNCINT_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOINT_HVSYNCINT_SHIFT))

#define  MVDO_TOP_MVDOINT_VDOINT_MASK 0xff
#define  MVDO_TOP_MVDOINT_VDOINT_SHIFT 0
#define  MVDO_TOP_MVDOINT_VDOINT_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOINT_VDOINT_MASK) >> MVDO_TOP_MVDOINT_VDOINT_SHIFT)
#define  MVDO_TOP_MVDOINT_VDOINT_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOINT_VDOINT_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOINT_VDOINT_SHIFT))

//====================================================================
//Register: MVDO Parameter Register (MVDOPR)
/** \brief Contains the parameter settings for this instantiation of MVDO*/
//====================================================================

#define  MVDO_TOP_MVDOPR_RESERVED1_MASK 0xfff80000
#define  MVDO_TOP_MVDOPR_RESERVED1_SHIFT 19
#define  MVDO_TOP_MVDOPR_RESERVED1_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_RESERVED1_MASK) >> MVDO_TOP_MVDOPR_RESERVED1_SHIFT)
#define  MVDO_TOP_MVDOPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_RESERVED1_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_RESERVED1_SHIFT))

#define  MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_MASK 0x70000
#define  MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_SHIFT 16
#define  MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_MASK) >> MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_SHIFT)
#define  MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_TOPQUEUEDEPTH_SHIFT))

#define  MVDO_TOP_MVDOPR_HSYNCINSERT_MASK 0x8000
#define  MVDO_TOP_MVDOPR_HSYNCINSERT_SHIFT 15
#define  MVDO_TOP_MVDOPR_HSYNCINSERT_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_HSYNCINSERT_MASK) >> MVDO_TOP_MVDOPR_HSYNCINSERT_SHIFT)
#define  MVDO_TOP_MVDOPR_HSYNCINSERT_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_HSYNCINSERT_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_HSYNCINSERT_SHIFT))

#define  MVDO_TOP_MVDOPR_EDGETIMER_MASK 0x4000
#define  MVDO_TOP_MVDOPR_EDGETIMER_SHIFT 14
#define  MVDO_TOP_MVDOPR_EDGETIMER_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_EDGETIMER_MASK) >> MVDO_TOP_MVDOPR_EDGETIMER_SHIFT)
#define  MVDO_TOP_MVDOPR_EDGETIMER_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_EDGETIMER_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_EDGETIMER_SHIFT))

#define  MVDO_TOP_MVDOPR_NUMMIRROR_MASK 0x3000
#define  MVDO_TOP_MVDOPR_NUMMIRROR_SHIFT 12
#define  MVDO_TOP_MVDOPR_NUMMIRROR_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_NUMMIRROR_MASK) >> MVDO_TOP_MVDOPR_NUMMIRROR_SHIFT)
#define  MVDO_TOP_MVDOPR_NUMMIRROR_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_NUMMIRROR_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_NUMMIRROR_SHIFT))

#define  MVDO_TOP_MVDOPR_MAXFACET_MASK 0xf00
#define  MVDO_TOP_MVDOPR_MAXFACET_SHIFT 8
#define  MVDO_TOP_MVDOPR_MAXFACET_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_MAXFACET_MASK) >> MVDO_TOP_MVDOPR_MAXFACET_SHIFT)
#define  MVDO_TOP_MVDOPR_MAXFACET_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_MAXFACET_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_MAXFACET_SHIFT))

#define  MVDO_TOP_MVDOPR_RESERVED2_MASK 0x80
#define  MVDO_TOP_MVDOPR_RESERVED2_SHIFT 7
#define  MVDO_TOP_MVDOPR_RESERVED2_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_RESERVED2_MASK) >> MVDO_TOP_MVDOPR_RESERVED2_SHIFT)
#define  MVDO_TOP_MVDOPR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_RESERVED2_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_RESERVED2_SHIFT))

#define  MVDO_TOP_MVDOPR_NUMHSYNCIN_MASK 0x70
#define  MVDO_TOP_MVDOPR_NUMHSYNCIN_SHIFT 4
#define  MVDO_TOP_MVDOPR_NUMHSYNCIN_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_NUMHSYNCIN_MASK) >> MVDO_TOP_MVDOPR_NUMHSYNCIN_SHIFT)
#define  MVDO_TOP_MVDOPR_NUMHSYNCIN_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_NUMHSYNCIN_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_NUMHSYNCIN_SHIFT))

#define  MVDO_TOP_MVDOPR_NUMCHAN_MASK 0xf
#define  MVDO_TOP_MVDOPR_NUMCHAN_SHIFT 0
#define  MVDO_TOP_MVDOPR_NUMCHAN_MASK_SHIFT(reg) (((reg) & MVDO_TOP_MVDOPR_NUMCHAN_MASK) >> MVDO_TOP_MVDOPR_NUMCHAN_SHIFT)
#define  MVDO_TOP_MVDOPR_NUMCHAN_REPLACE_VAL(reg,val) (((reg) & ~MVDO_TOP_MVDOPR_NUMCHAN_MASK) | (((uint32_t)val) << MVDO_TOP_MVDOPR_NUMCHAN_SHIFT))

#endif // MVDO_TOP
