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
 * \file CDMA_Top_Int_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CDMA_TOP_REG_REGMASKS_H_
#define _CDMA_TOP_REG_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CDMA Top Registers (CDMA_Top_Reg)
/** \brief CDMA Top Level Register Set*/
//
//====================================================================

//====================================================================
//Register: CDMA Interrupt Status Register (CDMAInt)
/** \brief Contains the status of current CDMA interrupts*/
//====================================================================

#define  CDMA_TOP_REG_CDMAINT_RESERVED1_MASK 0xffffff00
#define  CDMA_TOP_REG_CDMAINT_RESERVED1_SHIFT 8
#define  CDMA_TOP_REG_CDMAINT_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_TOP_REG_CDMAINT_RESERVED1_MASK) >> CDMA_TOP_REG_CDMAINT_RESERVED1_SHIFT)
#define  CDMA_TOP_REG_CDMAINT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TOP_REG_CDMAINT_RESERVED1_MASK) | (((uint32_t)val) << CDMA_TOP_REG_CDMAINT_RESERVED1_SHIFT))

#define  CDMA_TOP_REG_CDMAINT_CDMAINT_MASK 0xff
#define  CDMA_TOP_REG_CDMAINT_CDMAINT_SHIFT 0
#define  CDMA_TOP_REG_CDMAINT_CDMAINT_MASK_SHIFT(reg) (((reg) & CDMA_TOP_REG_CDMAINT_CDMAINT_MASK) >> CDMA_TOP_REG_CDMAINT_CDMAINT_SHIFT)
#define  CDMA_TOP_REG_CDMAINT_CDMAINT_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TOP_REG_CDMAINT_CDMAINT_MASK) | (((uint32_t)val) << CDMA_TOP_REG_CDMAINT_CDMAINT_SHIFT))

//====================================================================
//Register: CDMA Parameter Register (CDMAPR)
/** \brief Contains the parameter settings for this instantiation of CDMA*/
//====================================================================

#define  CDMA_TOP_REG_CDMAPR_RESERVED1_MASK 0xfffffff0
#define  CDMA_TOP_REG_CDMAPR_RESERVED1_SHIFT 4
#define  CDMA_TOP_REG_CDMAPR_RESERVED1_MASK_SHIFT(reg) (((reg) & CDMA_TOP_REG_CDMAPR_RESERVED1_MASK) >> CDMA_TOP_REG_CDMAPR_RESERVED1_SHIFT)
#define  CDMA_TOP_REG_CDMAPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TOP_REG_CDMAPR_RESERVED1_MASK) | (((uint32_t)val) << CDMA_TOP_REG_CDMAPR_RESERVED1_SHIFT))

#define  CDMA_TOP_REG_CDMAPR_NUMCHAN_MASK 0xf
#define  CDMA_TOP_REG_CDMAPR_NUMCHAN_SHIFT 0
#define  CDMA_TOP_REG_CDMAPR_NUMCHAN_MASK_SHIFT(reg) (((reg) & CDMA_TOP_REG_CDMAPR_NUMCHAN_MASK) >> CDMA_TOP_REG_CDMAPR_NUMCHAN_SHIFT)
#define  CDMA_TOP_REG_CDMAPR_NUMCHAN_REPLACE_VAL(reg,val) (((reg) & ~CDMA_TOP_REG_CDMAPR_NUMCHAN_MASK) | (((uint32_t)val) << CDMA_TOP_REG_CDMAPR_NUMCHAN_SHIFT))

#endif // CDMA_TOP_REG
