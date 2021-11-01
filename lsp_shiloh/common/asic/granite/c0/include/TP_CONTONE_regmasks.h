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
 * \file TP_CONTONE_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TP_CONTONE_REGMASKS_H_
#define _TP_CONTONE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Contone Pad (TP_CONTONE)
/** \brief Top-level register file for the Text Pad block*/
//
//====================================================================

//====================================================================
//Register: Text Pad Configuration Register (Cfg)
/** \brief This register configures the major features of the Text Pad module*/
//====================================================================

#define  TP_CONTONE_CFG_RESERVED1_MASK 0xfffffe00
#define  TP_CONTONE_CFG_RESERVED1_SHIFT 9
#define  TP_CONTONE_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & TP_CONTONE_CFG_RESERVED1_MASK) >> TP_CONTONE_CFG_RESERVED1_SHIFT)
#define  TP_CONTONE_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_CFG_RESERVED1_MASK) | (((uint32_t)val) << TP_CONTONE_CFG_RESERVED1_SHIFT))

#define  TP_CONTONE_CFG_PIXORDER_MASK 0x100
#define  TP_CONTONE_CFG_PIXORDER_SHIFT 8
#define  TP_CONTONE_CFG_PIXORDER_MASK_SHIFT(reg) (((reg) & TP_CONTONE_CFG_PIXORDER_MASK) >> TP_CONTONE_CFG_PIXORDER_SHIFT)
#define  TP_CONTONE_CFG_PIXORDER_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_CFG_PIXORDER_MASK) | (((uint32_t)val) << TP_CONTONE_CFG_PIXORDER_SHIFT))

#define  TP_CONTONE_CFG_RESERVED2_MASK 0xc0
#define  TP_CONTONE_CFG_RESERVED2_SHIFT 6
#define  TP_CONTONE_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & TP_CONTONE_CFG_RESERVED2_MASK) >> TP_CONTONE_CFG_RESERVED2_SHIFT)
#define  TP_CONTONE_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_CFG_RESERVED2_MASK) | (((uint32_t)val) << TP_CONTONE_CFG_RESERVED2_SHIFT))

#define  TP_CONTONE_CFG_NIBBLESEL_MASK 0x20
#define  TP_CONTONE_CFG_NIBBLESEL_SHIFT 5
#define  TP_CONTONE_CFG_NIBBLESEL_MASK_SHIFT(reg) (((reg) & TP_CONTONE_CFG_NIBBLESEL_MASK) >> TP_CONTONE_CFG_NIBBLESEL_SHIFT)
#define  TP_CONTONE_CFG_NIBBLESEL_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_CFG_NIBBLESEL_MASK) | (((uint32_t)val) << TP_CONTONE_CFG_NIBBLESEL_SHIFT))

#define  TP_CONTONE_CFG_OUTSEL_MASK 0x10
#define  TP_CONTONE_CFG_OUTSEL_SHIFT 4
#define  TP_CONTONE_CFG_OUTSEL_MASK_SHIFT(reg) (((reg) & TP_CONTONE_CFG_OUTSEL_MASK) >> TP_CONTONE_CFG_OUTSEL_SHIFT)
#define  TP_CONTONE_CFG_OUTSEL_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_CFG_OUTSEL_MASK) | (((uint32_t)val) << TP_CONTONE_CFG_OUTSEL_SHIFT))

#define  TP_CONTONE_CFG_RESERVED3_MASK 0xc
#define  TP_CONTONE_CFG_RESERVED3_SHIFT 2
#define  TP_CONTONE_CFG_RESERVED3_MASK_SHIFT(reg) (((reg) & TP_CONTONE_CFG_RESERVED3_MASK) >> TP_CONTONE_CFG_RESERVED3_SHIFT)
#define  TP_CONTONE_CFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_CFG_RESERVED3_MASK) | (((uint32_t)val) << TP_CONTONE_CFG_RESERVED3_SHIFT))

#define  TP_CONTONE_CFG_BPP_MASK 0x3
#define  TP_CONTONE_CFG_BPP_SHIFT 0
#define  TP_CONTONE_CFG_BPP_MASK_SHIFT(reg) (((reg) & TP_CONTONE_CFG_BPP_MASK) >> TP_CONTONE_CFG_BPP_SHIFT)
#define  TP_CONTONE_CFG_BPP_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_CFG_BPP_MASK) | (((uint32_t)val) << TP_CONTONE_CFG_BPP_SHIFT))

//====================================================================
//Register: Left Side Padding Register (LPad)
/** \brief This register configures left side padding.*/
//====================================================================

#define  TP_CONTONE_LPAD_RESERVED1_MASK 0xfffe0000
#define  TP_CONTONE_LPAD_RESERVED1_SHIFT 17
#define  TP_CONTONE_LPAD_RESERVED1_MASK_SHIFT(reg) (((reg) & TP_CONTONE_LPAD_RESERVED1_MASK) >> TP_CONTONE_LPAD_RESERVED1_SHIFT)
#define  TP_CONTONE_LPAD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_LPAD_RESERVED1_MASK) | (((uint32_t)val) << TP_CONTONE_LPAD_RESERVED1_SHIFT))

#define  TP_CONTONE_LPAD_LEFTEDGEFILL_MASK 0x10000
#define  TP_CONTONE_LPAD_LEFTEDGEFILL_SHIFT 16
#define  TP_CONTONE_LPAD_LEFTEDGEFILL_MASK_SHIFT(reg) (((reg) & TP_CONTONE_LPAD_LEFTEDGEFILL_MASK) >> TP_CONTONE_LPAD_LEFTEDGEFILL_SHIFT)
#define  TP_CONTONE_LPAD_LEFTEDGEFILL_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_LPAD_LEFTEDGEFILL_MASK) | (((uint32_t)val) << TP_CONTONE_LPAD_LEFTEDGEFILL_SHIFT))

#define  TP_CONTONE_LPAD_LEFTLEN_MASK 0xffff
#define  TP_CONTONE_LPAD_LEFTLEN_SHIFT 0
#define  TP_CONTONE_LPAD_LEFTLEN_MASK_SHIFT(reg) (((reg) & TP_CONTONE_LPAD_LEFTLEN_MASK) >> TP_CONTONE_LPAD_LEFTLEN_SHIFT)
#define  TP_CONTONE_LPAD_LEFTLEN_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_LPAD_LEFTLEN_MASK) | (((uint32_t)val) << TP_CONTONE_LPAD_LEFTLEN_SHIFT))

//====================================================================
//Register: Left Side Padding Value Register (LVal)
/** \brief This register contains the pad value for the left side if leftEdgeFill is not set.*/
//====================================================================

#define  TP_CONTONE_LVAL_LEFTVAL_MASK 0xffffffff
#define  TP_CONTONE_LVAL_LEFTVAL_SHIFT 0
#define  TP_CONTONE_LVAL_LEFTVAL_MASK_SHIFT(reg) (((reg) & TP_CONTONE_LVAL_LEFTVAL_MASK) >> TP_CONTONE_LVAL_LEFTVAL_SHIFT)
#define  TP_CONTONE_LVAL_LEFTVAL_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_LVAL_LEFTVAL_MASK) | (((uint32_t)val) << TP_CONTONE_LVAL_LEFTVAL_SHIFT))

//====================================================================
//Register: Right Side Padding Register (RPad)
/** \brief This register configures right side padding.*/
//====================================================================

#define  TP_CONTONE_RPAD_RESERVED1_MASK 0xfffe0000
#define  TP_CONTONE_RPAD_RESERVED1_SHIFT 17
#define  TP_CONTONE_RPAD_RESERVED1_MASK_SHIFT(reg) (((reg) & TP_CONTONE_RPAD_RESERVED1_MASK) >> TP_CONTONE_RPAD_RESERVED1_SHIFT)
#define  TP_CONTONE_RPAD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_RPAD_RESERVED1_MASK) | (((uint32_t)val) << TP_CONTONE_RPAD_RESERVED1_SHIFT))

#define  TP_CONTONE_RPAD_RIGHTEDGEFILL_MASK 0x10000
#define  TP_CONTONE_RPAD_RIGHTEDGEFILL_SHIFT 16
#define  TP_CONTONE_RPAD_RIGHTEDGEFILL_MASK_SHIFT(reg) (((reg) & TP_CONTONE_RPAD_RIGHTEDGEFILL_MASK) >> TP_CONTONE_RPAD_RIGHTEDGEFILL_SHIFT)
#define  TP_CONTONE_RPAD_RIGHTEDGEFILL_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_RPAD_RIGHTEDGEFILL_MASK) | (((uint32_t)val) << TP_CONTONE_RPAD_RIGHTEDGEFILL_SHIFT))

#define  TP_CONTONE_RPAD_RIGHTLEN_MASK 0xffff
#define  TP_CONTONE_RPAD_RIGHTLEN_SHIFT 0
#define  TP_CONTONE_RPAD_RIGHTLEN_MASK_SHIFT(reg) (((reg) & TP_CONTONE_RPAD_RIGHTLEN_MASK) >> TP_CONTONE_RPAD_RIGHTLEN_SHIFT)
#define  TP_CONTONE_RPAD_RIGHTLEN_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_RPAD_RIGHTLEN_MASK) | (((uint32_t)val) << TP_CONTONE_RPAD_RIGHTLEN_SHIFT))

//====================================================================
//Register: Right Side Padding Value Register (RVal)
/** \brief This register contains the pad value for the right side if rightEdgeFill is not set.*/
//====================================================================

#define  TP_CONTONE_RVAL_RIGHTVAL_MASK 0xffffffff
#define  TP_CONTONE_RVAL_RIGHTVAL_SHIFT 0
#define  TP_CONTONE_RVAL_RIGHTVAL_MASK_SHIFT(reg) (((reg) & TP_CONTONE_RVAL_RIGHTVAL_MASK) >> TP_CONTONE_RVAL_RIGHTVAL_SHIFT)
#define  TP_CONTONE_RVAL_RIGHTVAL_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_RVAL_RIGHTVAL_MASK) | (((uint32_t)val) << TP_CONTONE_RVAL_RIGHTVAL_SHIFT))

//====================================================================
//Register: Text Pad Parameter Register (TPPR)
/** \brief Contains the parameter settings for this instantiation of Text Pad*/
//====================================================================

#define  TP_CONTONE_TPPR_RESERVED1_MASK 0xfffc0000
#define  TP_CONTONE_TPPR_RESERVED1_SHIFT 18
#define  TP_CONTONE_TPPR_RESERVED1_MASK_SHIFT(reg) (((reg) & TP_CONTONE_TPPR_RESERVED1_MASK) >> TP_CONTONE_TPPR_RESERVED1_SHIFT)
#define  TP_CONTONE_TPPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_TPPR_RESERVED1_MASK) | (((uint32_t)val) << TP_CONTONE_TPPR_RESERVED1_SHIFT))

#define  TP_CONTONE_TPPR_OUTWIDTH_MASK 0x3f000
#define  TP_CONTONE_TPPR_OUTWIDTH_SHIFT 12
#define  TP_CONTONE_TPPR_OUTWIDTH_MASK_SHIFT(reg) (((reg) & TP_CONTONE_TPPR_OUTWIDTH_MASK) >> TP_CONTONE_TPPR_OUTWIDTH_SHIFT)
#define  TP_CONTONE_TPPR_OUTWIDTH_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_TPPR_OUTWIDTH_MASK) | (((uint32_t)val) << TP_CONTONE_TPPR_OUTWIDTH_SHIFT))

#define  TP_CONTONE_TPPR_RESERVED2_MASK 0xc00
#define  TP_CONTONE_TPPR_RESERVED2_SHIFT 10
#define  TP_CONTONE_TPPR_RESERVED2_MASK_SHIFT(reg) (((reg) & TP_CONTONE_TPPR_RESERVED2_MASK) >> TP_CONTONE_TPPR_RESERVED2_SHIFT)
#define  TP_CONTONE_TPPR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_TPPR_RESERVED2_MASK) | (((uint32_t)val) << TP_CONTONE_TPPR_RESERVED2_SHIFT))

#define  TP_CONTONE_TPPR_INWIDTH_MASK 0x3f0
#define  TP_CONTONE_TPPR_INWIDTH_SHIFT 4
#define  TP_CONTONE_TPPR_INWIDTH_MASK_SHIFT(reg) (((reg) & TP_CONTONE_TPPR_INWIDTH_MASK) >> TP_CONTONE_TPPR_INWIDTH_SHIFT)
#define  TP_CONTONE_TPPR_INWIDTH_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_TPPR_INWIDTH_MASK) | (((uint32_t)val) << TP_CONTONE_TPPR_INWIDTH_SHIFT))

#define  TP_CONTONE_TPPR_RESERVED3_MASK 0xe
#define  TP_CONTONE_TPPR_RESERVED3_SHIFT 1
#define  TP_CONTONE_TPPR_RESERVED3_MASK_SHIFT(reg) (((reg) & TP_CONTONE_TPPR_RESERVED3_MASK) >> TP_CONTONE_TPPR_RESERVED3_SHIFT)
#define  TP_CONTONE_TPPR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_TPPR_RESERVED3_MASK) | (((uint32_t)val) << TP_CONTONE_TPPR_RESERVED3_SHIFT))

#define  TP_CONTONE_TPPR_UNPACK_MASK 0x1
#define  TP_CONTONE_TPPR_UNPACK_SHIFT 0
#define  TP_CONTONE_TPPR_UNPACK_MASK_SHIFT(reg) (((reg) & TP_CONTONE_TPPR_UNPACK_MASK) >> TP_CONTONE_TPPR_UNPACK_SHIFT)
#define  TP_CONTONE_TPPR_UNPACK_REPLACE_VAL(reg,val) (((reg) & ~TP_CONTONE_TPPR_UNPACK_MASK) | (((uint32_t)val) << TP_CONTONE_TPPR_UNPACK_SHIFT))

#endif // TP_CONTONE
