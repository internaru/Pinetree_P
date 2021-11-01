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
 * \file HVSync_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HVSYNC_REGMASKS_H_
#define _HVSYNC_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: HVSync (HVSync)
/** \brief Top-level register file for HVSync block*/
//
//====================================================================

//====================================================================
//Register: HVSync Configuration Register (CFG)
/** \brief Register used to configure various aspects of the HVSync block.*/
//====================================================================

#define  HVSYNC_CFG_RESERVED1_MASK 0xfffffe00
#define  HVSYNC_CFG_RESERVED1_SHIFT 9
#define  HVSYNC_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_CFG_RESERVED1_MASK) >> HVSYNC_CFG_RESERVED1_SHIFT)
#define  HVSYNC_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_CFG_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_CFG_RESERVED1_SHIFT))

#define  HVSYNC_CFG_MIRRORSEL_MASK 0x1fe
#define  HVSYNC_CFG_MIRRORSEL_SHIFT 1
#define  HVSYNC_CFG_MIRRORSEL_MASK_SHIFT(reg) (((reg) & HVSYNC_CFG_MIRRORSEL_MASK) >> HVSYNC_CFG_MIRRORSEL_SHIFT)
#define  HVSYNC_CFG_MIRRORSEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_CFG_MIRRORSEL_MASK) | (((uint32_t)val) << HVSYNC_CFG_MIRRORSEL_SHIFT))

#define  HVSYNC_CFG_HVSYNCENABLE_MASK 0x1
#define  HVSYNC_CFG_HVSYNCENABLE_SHIFT 0
#define  HVSYNC_CFG_HVSYNCENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_CFG_HVSYNCENABLE_MASK) >> HVSYNC_CFG_HVSYNCENABLE_SHIFT)
#define  HVSYNC_CFG_HVSYNCENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_CFG_HVSYNCENABLE_MASK) | (((uint32_t)val) << HVSYNC_CFG_HVSYNCENABLE_SHIFT))

//====================================================================
//Register: Mirror Configuration Register (Instance 1 of 2) (MIR_CFG0)
/** \brief Used to configure various aspects of the mirror measurement hardware. (Instance 1 of 2)*/
//====================================================================

#define  HVSYNC_MIR_CFG0_RESERVED1_MASK 0xfffffe00
#define  HVSYNC_MIR_CFG0_RESERVED1_SHIFT 9
#define  HVSYNC_MIR_CFG0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG0_RESERVED1_MASK) >> HVSYNC_MIR_CFG0_RESERVED1_SHIFT)
#define  HVSYNC_MIR_CFG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG0_RESERVED1_SHIFT))

#define  HVSYNC_MIR_CFG0_TOPSRC_MASK 0x180
#define  HVSYNC_MIR_CFG0_TOPSRC_SHIFT 7
#define  HVSYNC_MIR_CFG0_TOPSRC_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG0_TOPSRC_MASK) >> HVSYNC_MIR_CFG0_TOPSRC_SHIFT)
#define  HVSYNC_MIR_CFG0_TOPSRC_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG0_TOPSRC_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG0_TOPSRC_SHIFT))

#define  HVSYNC_MIR_CFG0_HSYNCSRC_MASK 0x60
#define  HVSYNC_MIR_CFG0_HSYNCSRC_SHIFT 5
#define  HVSYNC_MIR_CFG0_HSYNCSRC_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG0_HSYNCSRC_MASK) >> HVSYNC_MIR_CFG0_HSYNCSRC_SHIFT)
#define  HVSYNC_MIR_CFG0_HSYNCSRC_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG0_HSYNCSRC_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG0_HSYNCSRC_SHIFT))

#define  HVSYNC_MIR_CFG0_NUMFACETS_MASK 0x1e
#define  HVSYNC_MIR_CFG0_NUMFACETS_SHIFT 1
#define  HVSYNC_MIR_CFG0_NUMFACETS_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG0_NUMFACETS_MASK) >> HVSYNC_MIR_CFG0_NUMFACETS_SHIFT)
#define  HVSYNC_MIR_CFG0_NUMFACETS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG0_NUMFACETS_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG0_NUMFACETS_SHIFT))

#define  HVSYNC_MIR_CFG0_MEASRST_MASK 0x1
#define  HVSYNC_MIR_CFG0_MEASRST_SHIFT 0
#define  HVSYNC_MIR_CFG0_MEASRST_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG0_MEASRST_MASK) >> HVSYNC_MIR_CFG0_MEASRST_SHIFT)
#define  HVSYNC_MIR_CFG0_MEASRST_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG0_MEASRST_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG0_MEASRST_SHIFT))

//====================================================================
//Register: Mirror Configuration Register (Instance 2 of 2) (MIR_CFG1)
/** \brief Used to configure various aspects of the mirror measurement hardware. (Instance 2 of 2)*/
//====================================================================

#define  HVSYNC_MIR_CFG1_RESERVED1_MASK 0xfffffe00
#define  HVSYNC_MIR_CFG1_RESERVED1_SHIFT 9
#define  HVSYNC_MIR_CFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG1_RESERVED1_MASK) >> HVSYNC_MIR_CFG1_RESERVED1_SHIFT)
#define  HVSYNC_MIR_CFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG1_RESERVED1_SHIFT))

#define  HVSYNC_MIR_CFG1_TOPSRC_MASK 0x180
#define  HVSYNC_MIR_CFG1_TOPSRC_SHIFT 7
#define  HVSYNC_MIR_CFG1_TOPSRC_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG1_TOPSRC_MASK) >> HVSYNC_MIR_CFG1_TOPSRC_SHIFT)
#define  HVSYNC_MIR_CFG1_TOPSRC_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG1_TOPSRC_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG1_TOPSRC_SHIFT))

#define  HVSYNC_MIR_CFG1_HSYNCSRC_MASK 0x60
#define  HVSYNC_MIR_CFG1_HSYNCSRC_SHIFT 5
#define  HVSYNC_MIR_CFG1_HSYNCSRC_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG1_HSYNCSRC_MASK) >> HVSYNC_MIR_CFG1_HSYNCSRC_SHIFT)
#define  HVSYNC_MIR_CFG1_HSYNCSRC_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG1_HSYNCSRC_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG1_HSYNCSRC_SHIFT))

#define  HVSYNC_MIR_CFG1_NUMFACETS_MASK 0x1e
#define  HVSYNC_MIR_CFG1_NUMFACETS_SHIFT 1
#define  HVSYNC_MIR_CFG1_NUMFACETS_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG1_NUMFACETS_MASK) >> HVSYNC_MIR_CFG1_NUMFACETS_SHIFT)
#define  HVSYNC_MIR_CFG1_NUMFACETS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG1_NUMFACETS_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG1_NUMFACETS_SHIFT))

#define  HVSYNC_MIR_CFG1_MEASRST_MASK 0x1
#define  HVSYNC_MIR_CFG1_MEASRST_SHIFT 0
#define  HVSYNC_MIR_CFG1_MEASRST_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_CFG1_MEASRST_MASK) >> HVSYNC_MIR_CFG1_MEASRST_SHIFT)
#define  HVSYNC_MIR_CFG1_MEASRST_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_CFG1_MEASRST_MASK) | (((uint32_t)val) << HVSYNC_MIR_CFG1_MEASRST_SHIFT))

//====================================================================
//Register: Mirror Status Register (Instance 1 of 2) (MIR_STA0)
/** \brief Facet Tracker/Timer Status (Instance 1 of 2)*/
//====================================================================

#define  HVSYNC_MIR_STA0_RESERVED1_MASK 0xffffff00
#define  HVSYNC_MIR_STA0_RESERVED1_SHIFT 8
#define  HVSYNC_MIR_STA0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA0_RESERVED1_MASK) >> HVSYNC_MIR_STA0_RESERVED1_SHIFT)
#define  HVSYNC_MIR_STA0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA0_RESERVED1_SHIFT))

#define  HVSYNC_MIR_STA0_CURRFACET_MASK 0xf0
#define  HVSYNC_MIR_STA0_CURRFACET_SHIFT 4
#define  HVSYNC_MIR_STA0_CURRFACET_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA0_CURRFACET_MASK) >> HVSYNC_MIR_STA0_CURRFACET_SHIFT)
#define  HVSYNC_MIR_STA0_CURRFACET_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA0_CURRFACET_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA0_CURRFACET_SHIFT))

#define  HVSYNC_MIR_STA0_RESERVED2_MASK 0xc
#define  HVSYNC_MIR_STA0_RESERVED2_SHIFT 2
#define  HVSYNC_MIR_STA0_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA0_RESERVED2_MASK) >> HVSYNC_MIR_STA0_RESERVED2_SHIFT)
#define  HVSYNC_MIR_STA0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA0_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA0_RESERVED2_SHIFT))

#define  HVSYNC_MIR_STA0_TOPFIRST_MASK 0x2
#define  HVSYNC_MIR_STA0_TOPFIRST_SHIFT 1
#define  HVSYNC_MIR_STA0_TOPFIRST_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA0_TOPFIRST_MASK) >> HVSYNC_MIR_STA0_TOPFIRST_SHIFT)
#define  HVSYNC_MIR_STA0_TOPFIRST_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA0_TOPFIRST_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA0_TOPFIRST_SHIFT))

#define  HVSYNC_MIR_STA0_MEASACTIVE_MASK 0x1
#define  HVSYNC_MIR_STA0_MEASACTIVE_SHIFT 0
#define  HVSYNC_MIR_STA0_MEASACTIVE_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA0_MEASACTIVE_MASK) >> HVSYNC_MIR_STA0_MEASACTIVE_SHIFT)
#define  HVSYNC_MIR_STA0_MEASACTIVE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA0_MEASACTIVE_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA0_MEASACTIVE_SHIFT))

//====================================================================
//Register: Mirror Status Register (Instance 2 of 2) (MIR_STA1)
/** \brief Facet Tracker/Timer Status (Instance 2 of 2)*/
//====================================================================

#define  HVSYNC_MIR_STA1_RESERVED1_MASK 0xffffff00
#define  HVSYNC_MIR_STA1_RESERVED1_SHIFT 8
#define  HVSYNC_MIR_STA1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA1_RESERVED1_MASK) >> HVSYNC_MIR_STA1_RESERVED1_SHIFT)
#define  HVSYNC_MIR_STA1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA1_RESERVED1_SHIFT))

#define  HVSYNC_MIR_STA1_CURRFACET_MASK 0xf0
#define  HVSYNC_MIR_STA1_CURRFACET_SHIFT 4
#define  HVSYNC_MIR_STA1_CURRFACET_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA1_CURRFACET_MASK) >> HVSYNC_MIR_STA1_CURRFACET_SHIFT)
#define  HVSYNC_MIR_STA1_CURRFACET_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA1_CURRFACET_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA1_CURRFACET_SHIFT))

#define  HVSYNC_MIR_STA1_RESERVED2_MASK 0xc
#define  HVSYNC_MIR_STA1_RESERVED2_SHIFT 2
#define  HVSYNC_MIR_STA1_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA1_RESERVED2_MASK) >> HVSYNC_MIR_STA1_RESERVED2_SHIFT)
#define  HVSYNC_MIR_STA1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA1_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA1_RESERVED2_SHIFT))

#define  HVSYNC_MIR_STA1_TOPFIRST_MASK 0x2
#define  HVSYNC_MIR_STA1_TOPFIRST_SHIFT 1
#define  HVSYNC_MIR_STA1_TOPFIRST_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA1_TOPFIRST_MASK) >> HVSYNC_MIR_STA1_TOPFIRST_SHIFT)
#define  HVSYNC_MIR_STA1_TOPFIRST_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA1_TOPFIRST_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA1_TOPFIRST_SHIFT))

#define  HVSYNC_MIR_STA1_MEASACTIVE_MASK 0x1
#define  HVSYNC_MIR_STA1_MEASACTIVE_SHIFT 0
#define  HVSYNC_MIR_STA1_MEASACTIVE_MASK_SHIFT(reg) (((reg) & HVSYNC_MIR_STA1_MEASACTIVE_MASK) >> HVSYNC_MIR_STA1_MEASACTIVE_SHIFT)
#define  HVSYNC_MIR_STA1_MEASACTIVE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MIR_STA1_MEASACTIVE_MASK) | (((uint32_t)val) << HVSYNC_MIR_STA1_MEASACTIVE_SHIFT))

//====================================================================
//Register: Facet Tracker Reset Register (Instance 1 of 2) (FT_RST0)
/** \brief Used to manually reset the facet tracker counter. (Instance 1 of 2)*/
//====================================================================

#define  HVSYNC_FT_RST0_RESERVED1_MASK 0xfffffffe
#define  HVSYNC_FT_RST0_RESERVED1_SHIFT 1
#define  HVSYNC_FT_RST0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_FT_RST0_RESERVED1_MASK) >> HVSYNC_FT_RST0_RESERVED1_SHIFT)
#define  HVSYNC_FT_RST0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_FT_RST0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_FT_RST0_RESERVED1_SHIFT))

#define  HVSYNC_FT_RST0_RESETFT_MASK 0x1
#define  HVSYNC_FT_RST0_RESETFT_SHIFT 0
#define  HVSYNC_FT_RST0_RESETFT_MASK_SHIFT(reg) (((reg) & HVSYNC_FT_RST0_RESETFT_MASK) >> HVSYNC_FT_RST0_RESETFT_SHIFT)
#define  HVSYNC_FT_RST0_RESETFT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_FT_RST0_RESETFT_MASK) | (((uint32_t)val) << HVSYNC_FT_RST0_RESETFT_SHIFT))

//====================================================================
//Register: Facet Tracker Reset Register (Instance 2 of 2) (FT_RST1)
/** \brief Used to manually reset the facet tracker counter. (Instance 2 of 2)*/
//====================================================================

#define  HVSYNC_FT_RST1_RESERVED1_MASK 0xfffffffe
#define  HVSYNC_FT_RST1_RESERVED1_SHIFT 1
#define  HVSYNC_FT_RST1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_FT_RST1_RESERVED1_MASK) >> HVSYNC_FT_RST1_RESERVED1_SHIFT)
#define  HVSYNC_FT_RST1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_FT_RST1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_FT_RST1_RESERVED1_SHIFT))

#define  HVSYNC_FT_RST1_RESETFT_MASK 0x1
#define  HVSYNC_FT_RST1_RESETFT_SHIFT 0
#define  HVSYNC_FT_RST1_RESETFT_MASK_SHIFT(reg) (((reg) & HVSYNC_FT_RST1_RESETFT_MASK) >> HVSYNC_FT_RST1_RESETFT_SHIFT)
#define  HVSYNC_FT_RST1_RESETFT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_FT_RST1_RESETFT_MASK) | (((uint32_t)val) << HVSYNC_FT_RST1_RESETFT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 1 of 8) (OUT_SEL0)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 1 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL0_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL0_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL0_RESERVED1_MASK) >> HVSYNC_OUT_SEL0_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL0_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL0_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL0_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL0_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL0_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL0_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL0_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL0_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL0_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL0_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL0_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL0_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL0_TOPOUT_MASK) >> HVSYNC_OUT_SEL0_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL0_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL0_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL0_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL0_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL0_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL0_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL0_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL0_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL0_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL0_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL0_HSYNCOUT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 2 of 8) (OUT_SEL1)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 2 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL1_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL1_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL1_RESERVED1_MASK) >> HVSYNC_OUT_SEL1_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL1_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL1_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL1_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL1_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL1_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL1_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL1_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL1_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL1_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL1_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL1_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL1_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL1_TOPOUT_MASK) >> HVSYNC_OUT_SEL1_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL1_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL1_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL1_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL1_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL1_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL1_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL1_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL1_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL1_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL1_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL1_HSYNCOUT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 3 of 8) (OUT_SEL2)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 3 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL2_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL2_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL2_RESERVED1_MASK) >> HVSYNC_OUT_SEL2_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL2_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL2_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL2_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL2_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL2_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL2_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL2_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL2_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL2_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL2_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL2_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL2_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL2_TOPOUT_MASK) >> HVSYNC_OUT_SEL2_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL2_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL2_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL2_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL2_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL2_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL2_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL2_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL2_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL2_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL2_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL2_HSYNCOUT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 4 of 8) (OUT_SEL3)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 4 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL3_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL3_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL3_RESERVED1_MASK) >> HVSYNC_OUT_SEL3_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL3_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL3_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL3_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL3_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL3_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL3_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL3_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL3_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL3_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL3_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL3_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL3_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL3_TOPOUT_MASK) >> HVSYNC_OUT_SEL3_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL3_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL3_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL3_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL3_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL3_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL3_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL3_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL3_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL3_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL3_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL3_HSYNCOUT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 5 of 8) (OUT_SEL4)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 5 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL4_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL4_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL4_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL4_RESERVED1_MASK) >> HVSYNC_OUT_SEL4_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL4_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL4_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL4_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL4_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL4_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL4_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL4_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL4_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL4_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL4_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL4_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL4_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL4_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL4_TOPOUT_MASK) >> HVSYNC_OUT_SEL4_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL4_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL4_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL4_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL4_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL4_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL4_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL4_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL4_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL4_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL4_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL4_HSYNCOUT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 6 of 8) (OUT_SEL5)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 6 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL5_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL5_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL5_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL5_RESERVED1_MASK) >> HVSYNC_OUT_SEL5_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL5_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL5_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL5_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL5_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL5_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL5_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL5_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL5_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL5_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL5_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL5_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL5_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL5_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL5_TOPOUT_MASK) >> HVSYNC_OUT_SEL5_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL5_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL5_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL5_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL5_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL5_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL5_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL5_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL5_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL5_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL5_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL5_HSYNCOUT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 7 of 8) (OUT_SEL6)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 7 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL6_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL6_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL6_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL6_RESERVED1_MASK) >> HVSYNC_OUT_SEL6_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL6_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL6_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL6_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL6_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL6_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL6_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL6_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL6_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL6_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL6_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL6_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL6_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL6_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL6_TOPOUT_MASK) >> HVSYNC_OUT_SEL6_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL6_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL6_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL6_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL6_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL6_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL6_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL6_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL6_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL6_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL6_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL6_HSYNCOUT_SHIFT))

//====================================================================
//Register: Output Select Register (Instance 8 of 8) (OUT_SEL7)
/** \brief This register is used to select the source for the HSYNC, Top, and GalvoRtoL outputs to the respective video channel.
      OUT_SEL[0] supplies HSYNC, Top, and GalvoRtoL to video channel 0, OUT_SEL[1] to video channel 1, etc.
      <br>
      HSYNCA refers to the first active edge on HSYNC
      <br>
      HSYNCB refers to the second active edge on HSYNC
      <br>
      HSYNCC refers to the third active edge on HSYNC
      <br>
      HSYNCD refers to the fourth active edge on HSYNC
      <br>
      0-3 refer to which of the 4 HSYNC filters supplied the HSYNC. (Instance 8 of 8)*/
//====================================================================

#define  HVSYNC_OUT_SEL7_RESERVED1_MASK 0xffffff00
#define  HVSYNC_OUT_SEL7_RESERVED1_SHIFT 8
#define  HVSYNC_OUT_SEL7_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL7_RESERVED1_MASK) >> HVSYNC_OUT_SEL7_RESERVED1_SHIFT)
#define  HVSYNC_OUT_SEL7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL7_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL7_RESERVED1_SHIFT))

#define  HVSYNC_OUT_SEL7_GALVORTOLOUT_MASK 0xc0
#define  HVSYNC_OUT_SEL7_GALVORTOLOUT_SHIFT 6
#define  HVSYNC_OUT_SEL7_GALVORTOLOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL7_GALVORTOLOUT_MASK) >> HVSYNC_OUT_SEL7_GALVORTOLOUT_SHIFT)
#define  HVSYNC_OUT_SEL7_GALVORTOLOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL7_GALVORTOLOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL7_GALVORTOLOUT_SHIFT))

#define  HVSYNC_OUT_SEL7_TOPOUT_MASK 0x30
#define  HVSYNC_OUT_SEL7_TOPOUT_SHIFT 4
#define  HVSYNC_OUT_SEL7_TOPOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL7_TOPOUT_MASK) >> HVSYNC_OUT_SEL7_TOPOUT_SHIFT)
#define  HVSYNC_OUT_SEL7_TOPOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL7_TOPOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL7_TOPOUT_SHIFT))

#define  HVSYNC_OUT_SEL7_HSYNCOUT_MASK 0xf
#define  HVSYNC_OUT_SEL7_HSYNCOUT_SHIFT 0
#define  HVSYNC_OUT_SEL7_HSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_OUT_SEL7_HSYNCOUT_MASK) >> HVSYNC_OUT_SEL7_HSYNCOUT_SHIFT)
#define  HVSYNC_OUT_SEL7_HSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_OUT_SEL7_HSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_OUT_SEL7_HSYNCOUT_SHIFT))

//====================================================================
//Register: Top Queue Status Register (TQSTAT)
/** \brief Reflects the status of the Top Queue counters for each of the video channels
    If the respective status is 1, indicates the counter is available
    <br>
    TopQStatX[0] = TopCntrA[chanX] status
    <br>
    TopQStatX[2] = TopCntrC[chanX] status
    <br>
    TopQStatX[3] = TopCntrD[chanX] status*/
//====================================================================

#define  HVSYNC_TQSTAT_RESERVED1_MASK 0xffff0000
#define  HVSYNC_TQSTAT_RESERVED1_SHIFT 16
#define  HVSYNC_TQSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSTAT_RESERVED1_MASK) >> HVSYNC_TQSTAT_RESERVED1_SHIFT)
#define  HVSYNC_TQSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSTAT_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TQSTAT_RESERVED1_SHIFT))

#define  HVSYNC_TQSTAT_TOPQSTAT3_MASK 0xf000
#define  HVSYNC_TQSTAT_TOPQSTAT3_SHIFT 12
#define  HVSYNC_TQSTAT_TOPQSTAT3_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSTAT_TOPQSTAT3_MASK) >> HVSYNC_TQSTAT_TOPQSTAT3_SHIFT)
#define  HVSYNC_TQSTAT_TOPQSTAT3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSTAT_TOPQSTAT3_MASK) | (((uint32_t)val) << HVSYNC_TQSTAT_TOPQSTAT3_SHIFT))

#define  HVSYNC_TQSTAT_TOPQSTAT2_MASK 0xf00
#define  HVSYNC_TQSTAT_TOPQSTAT2_SHIFT 8
#define  HVSYNC_TQSTAT_TOPQSTAT2_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSTAT_TOPQSTAT2_MASK) >> HVSYNC_TQSTAT_TOPQSTAT2_SHIFT)
#define  HVSYNC_TQSTAT_TOPQSTAT2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSTAT_TOPQSTAT2_MASK) | (((uint32_t)val) << HVSYNC_TQSTAT_TOPQSTAT2_SHIFT))

#define  HVSYNC_TQSTAT_TOPQSTAT1_MASK 0xf0
#define  HVSYNC_TQSTAT_TOPQSTAT1_SHIFT 4
#define  HVSYNC_TQSTAT_TOPQSTAT1_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSTAT_TOPQSTAT1_MASK) >> HVSYNC_TQSTAT_TOPQSTAT1_SHIFT)
#define  HVSYNC_TQSTAT_TOPQSTAT1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSTAT_TOPQSTAT1_MASK) | (((uint32_t)val) << HVSYNC_TQSTAT_TOPQSTAT1_SHIFT))

#define  HVSYNC_TQSTAT_TOPQSTAT0_MASK 0xf
#define  HVSYNC_TQSTAT_TOPQSTAT0_SHIFT 0
#define  HVSYNC_TQSTAT_TOPQSTAT0_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSTAT_TOPQSTAT0_MASK) >> HVSYNC_TQSTAT_TOPQSTAT0_SHIFT)
#define  HVSYNC_TQSTAT_TOPQSTAT0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSTAT_TOPQSTAT0_MASK) | (((uint32_t)val) << HVSYNC_TQSTAT_TOPQSTAT0_SHIFT))

//====================================================================
//Register: Top Queue HSYNC select Register (TQSEL)
/** \brief Used to select the input HSYNC to the Top Queue module*/
//====================================================================

#define  HVSYNC_TQSEL_RESERVED1_MASK 0xfffff000
#define  HVSYNC_TQSEL_RESERVED1_SHIFT 12
#define  HVSYNC_TQSEL_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSEL_RESERVED1_MASK) >> HVSYNC_TQSEL_RESERVED1_SHIFT)
#define  HVSYNC_TQSEL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSEL_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TQSEL_RESERVED1_SHIFT))

#define  HVSYNC_TQSEL_TOPQHSYNCSEL3_MASK 0xe00
#define  HVSYNC_TQSEL_TOPQHSYNCSEL3_SHIFT 9
#define  HVSYNC_TQSEL_TOPQHSYNCSEL3_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSEL_TOPQHSYNCSEL3_MASK) >> HVSYNC_TQSEL_TOPQHSYNCSEL3_SHIFT)
#define  HVSYNC_TQSEL_TOPQHSYNCSEL3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSEL_TOPQHSYNCSEL3_MASK) | (((uint32_t)val) << HVSYNC_TQSEL_TOPQHSYNCSEL3_SHIFT))

#define  HVSYNC_TQSEL_TOPQHSYNCSEL2_MASK 0x1c0
#define  HVSYNC_TQSEL_TOPQHSYNCSEL2_SHIFT 6
#define  HVSYNC_TQSEL_TOPQHSYNCSEL2_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSEL_TOPQHSYNCSEL2_MASK) >> HVSYNC_TQSEL_TOPQHSYNCSEL2_SHIFT)
#define  HVSYNC_TQSEL_TOPQHSYNCSEL2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSEL_TOPQHSYNCSEL2_MASK) | (((uint32_t)val) << HVSYNC_TQSEL_TOPQHSYNCSEL2_SHIFT))

#define  HVSYNC_TQSEL_TOPQHSYNCSEL1_MASK 0x38
#define  HVSYNC_TQSEL_TOPQHSYNCSEL1_SHIFT 3
#define  HVSYNC_TQSEL_TOPQHSYNCSEL1_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSEL_TOPQHSYNCSEL1_MASK) >> HVSYNC_TQSEL_TOPQHSYNCSEL1_SHIFT)
#define  HVSYNC_TQSEL_TOPQHSYNCSEL1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSEL_TOPQHSYNCSEL1_MASK) | (((uint32_t)val) << HVSYNC_TQSEL_TOPQHSYNCSEL1_SHIFT))

#define  HVSYNC_TQSEL_TOPQHSYNCSEL0_MASK 0x7
#define  HVSYNC_TQSEL_TOPQHSYNCSEL0_SHIFT 0
#define  HVSYNC_TQSEL_TOPQHSYNCSEL0_MASK_SHIFT(reg) (((reg) & HVSYNC_TQSEL_TOPQHSYNCSEL0_MASK) >> HVSYNC_TQSEL_TOPQHSYNCSEL0_SHIFT)
#define  HVSYNC_TQSEL_TOPQHSYNCSEL0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TQSEL_TOPQHSYNCSEL0_MASK) | (((uint32_t)val) << HVSYNC_TQSEL_TOPQHSYNCSEL0_SHIFT))

//====================================================================
//Register: HVSync Interrupt Enable Register (HVSIER)
/** \brief HVSync Interrupt Enable Register.*/
//====================================================================

#define  HVSYNC_HVSIER_RESERVED1_MASK 0x80000000
#define  HVSYNC_HVSIER_RESERVED1_SHIFT 31
#define  HVSYNC_HVSIER_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_RESERVED1_MASK) >> HVSYNC_HVSIER_RESERVED1_SHIFT)
#define  HVSYNC_HVSIER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_RESERVED1_SHIFT))

#define  HVSYNC_HVSIER_MISSEDHSYNCINT_MASK 0x78000000
#define  HVSYNC_HVSIER_MISSEDHSYNCINT_SHIFT 27
#define  HVSYNC_HVSIER_MISSEDHSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_MISSEDHSYNCINT_MASK) >> HVSYNC_HVSIER_MISSEDHSYNCINT_SHIFT)
#define  HVSYNC_HVSIER_MISSEDHSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_MISSEDHSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_MISSEDHSYNCINT_SHIFT))

#define  HVSYNC_HVSIER_TOPDLYINT_MASK 0x7f80000
#define  HVSYNC_HVSIER_TOPDLYINT_SHIFT 19
#define  HVSYNC_HVSIER_TOPDLYINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_TOPDLYINT_MASK) >> HVSYNC_HVSIER_TOPDLYINT_SHIFT)
#define  HVSYNC_HVSIER_TOPDLYINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_TOPDLYINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_TOPDLYINT_SHIFT))

#define  HVSYNC_HVSIER_BOTINT_MASK 0x40000
#define  HVSYNC_HVSIER_BOTINT_SHIFT 18
#define  HVSYNC_HVSIER_BOTINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_BOTINT_MASK) >> HVSYNC_HVSIER_BOTINT_SHIFT)
#define  HVSYNC_HVSIER_BOTINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_BOTINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_BOTINT_SHIFT))

#define  HVSYNC_HVSIER_FTDONEINT_MASK 0x30000
#define  HVSYNC_HVSIER_FTDONEINT_SHIFT 16
#define  HVSYNC_HVSIER_FTDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_FTDONEINT_MASK) >> HVSYNC_HVSIER_FTDONEINT_SHIFT)
#define  HVSYNC_HVSIER_FTDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_FTDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_FTDONEINT_SHIFT))

#define  HVSYNC_HVSIER_TOPMEASDONEINT_MASK 0xc000
#define  HVSYNC_HVSIER_TOPMEASDONEINT_SHIFT 14
#define  HVSYNC_HVSIER_TOPMEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_TOPMEASDONEINT_MASK) >> HVSYNC_HVSIER_TOPMEASDONEINT_SHIFT)
#define  HVSYNC_HVSIER_TOPMEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_TOPMEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_TOPMEASDONEINT_SHIFT))

#define  HVSYNC_HVSIER_MEASDONEINT_MASK 0x3000
#define  HVSYNC_HVSIER_MEASDONEINT_SHIFT 12
#define  HVSYNC_HVSIER_MEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_MEASDONEINT_MASK) >> HVSYNC_HVSIER_MEASDONEINT_SHIFT)
#define  HVSYNC_HVSIER_MEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_MEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_MEASDONEINT_SHIFT))

#define  HVSYNC_HVSIER_TOPQFULLINT_MASK 0xf00
#define  HVSYNC_HVSIER_TOPQFULLINT_SHIFT 8
#define  HVSYNC_HVSIER_TOPQFULLINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_TOPQFULLINT_MASK) >> HVSYNC_HVSIER_TOPQFULLINT_SHIFT)
#define  HVSYNC_HVSIER_TOPQFULLINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_TOPQFULLINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_TOPQFULLINT_SHIFT))

#define  HVSYNC_HVSIER_TOPINT_MASK 0xf0
#define  HVSYNC_HVSIER_TOPINT_SHIFT 4
#define  HVSYNC_HVSIER_TOPINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_TOPINT_MASK) >> HVSYNC_HVSIER_TOPINT_SHIFT)
#define  HVSYNC_HVSIER_TOPINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_TOPINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_TOPINT_SHIFT))

#define  HVSYNC_HVSIER_HSYNCINT_MASK 0xf
#define  HVSYNC_HVSIER_HSYNCINT_SHIFT 0
#define  HVSYNC_HVSIER_HSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIER_HSYNCINT_MASK) >> HVSYNC_HVSIER_HSYNCINT_SHIFT)
#define  HVSYNC_HVSIER_HSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIER_HSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIER_HSYNCINT_SHIFT))

//====================================================================
//Register: HVSync Interrupt Pending Register (HVSIPR)
/** \brief HVSync Interrupt Pending Register.*/
//====================================================================

#define  HVSYNC_HVSIPR_RESERVED1_MASK 0x80000000
#define  HVSYNC_HVSIPR_RESERVED1_SHIFT 31
#define  HVSYNC_HVSIPR_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_RESERVED1_MASK) >> HVSYNC_HVSIPR_RESERVED1_SHIFT)
#define  HVSYNC_HVSIPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_RESERVED1_SHIFT))

#define  HVSYNC_HVSIPR_MISSEDHSYNCINT_MASK 0x78000000
#define  HVSYNC_HVSIPR_MISSEDHSYNCINT_SHIFT 27
#define  HVSYNC_HVSIPR_MISSEDHSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_MISSEDHSYNCINT_MASK) >> HVSYNC_HVSIPR_MISSEDHSYNCINT_SHIFT)
#define  HVSYNC_HVSIPR_MISSEDHSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_MISSEDHSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_MISSEDHSYNCINT_SHIFT))

#define  HVSYNC_HVSIPR_TOPDLYINT_MASK 0x7f80000
#define  HVSYNC_HVSIPR_TOPDLYINT_SHIFT 19
#define  HVSYNC_HVSIPR_TOPDLYINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_TOPDLYINT_MASK) >> HVSYNC_HVSIPR_TOPDLYINT_SHIFT)
#define  HVSYNC_HVSIPR_TOPDLYINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_TOPDLYINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_TOPDLYINT_SHIFT))

#define  HVSYNC_HVSIPR_BOTINT_MASK 0x40000
#define  HVSYNC_HVSIPR_BOTINT_SHIFT 18
#define  HVSYNC_HVSIPR_BOTINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_BOTINT_MASK) >> HVSYNC_HVSIPR_BOTINT_SHIFT)
#define  HVSYNC_HVSIPR_BOTINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_BOTINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_BOTINT_SHIFT))

#define  HVSYNC_HVSIPR_FTDONEINT_MASK 0x30000
#define  HVSYNC_HVSIPR_FTDONEINT_SHIFT 16
#define  HVSYNC_HVSIPR_FTDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_FTDONEINT_MASK) >> HVSYNC_HVSIPR_FTDONEINT_SHIFT)
#define  HVSYNC_HVSIPR_FTDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_FTDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_FTDONEINT_SHIFT))

#define  HVSYNC_HVSIPR_TOPMEASDONEINT_MASK 0xc000
#define  HVSYNC_HVSIPR_TOPMEASDONEINT_SHIFT 14
#define  HVSYNC_HVSIPR_TOPMEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_TOPMEASDONEINT_MASK) >> HVSYNC_HVSIPR_TOPMEASDONEINT_SHIFT)
#define  HVSYNC_HVSIPR_TOPMEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_TOPMEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_TOPMEASDONEINT_SHIFT))

#define  HVSYNC_HVSIPR_MEASDONEINT_MASK 0x3000
#define  HVSYNC_HVSIPR_MEASDONEINT_SHIFT 12
#define  HVSYNC_HVSIPR_MEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_MEASDONEINT_MASK) >> HVSYNC_HVSIPR_MEASDONEINT_SHIFT)
#define  HVSYNC_HVSIPR_MEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_MEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_MEASDONEINT_SHIFT))

#define  HVSYNC_HVSIPR_TOPQFULLINT_MASK 0xf00
#define  HVSYNC_HVSIPR_TOPQFULLINT_SHIFT 8
#define  HVSYNC_HVSIPR_TOPQFULLINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_TOPQFULLINT_MASK) >> HVSYNC_HVSIPR_TOPQFULLINT_SHIFT)
#define  HVSYNC_HVSIPR_TOPQFULLINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_TOPQFULLINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_TOPQFULLINT_SHIFT))

#define  HVSYNC_HVSIPR_TOPINT_MASK 0xf0
#define  HVSYNC_HVSIPR_TOPINT_SHIFT 4
#define  HVSYNC_HVSIPR_TOPINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_TOPINT_MASK) >> HVSYNC_HVSIPR_TOPINT_SHIFT)
#define  HVSYNC_HVSIPR_TOPINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_TOPINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_TOPINT_SHIFT))

#define  HVSYNC_HVSIPR_HSYNCINT_MASK 0xf
#define  HVSYNC_HVSIPR_HSYNCINT_SHIFT 0
#define  HVSYNC_HVSIPR_HSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIPR_HSYNCINT_MASK) >> HVSYNC_HVSIPR_HSYNCINT_SHIFT)
#define  HVSYNC_HVSIPR_HSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIPR_HSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIPR_HSYNCINT_SHIFT))

//====================================================================
//Register: HVSync Interrupt Acknowledge Register (HVSIAR)
/** \brief HVSync Interrupt Acknowledge Register.*/
//====================================================================

#define  HVSYNC_HVSIAR_RESERVED1_MASK 0x80000000
#define  HVSYNC_HVSIAR_RESERVED1_SHIFT 31
#define  HVSYNC_HVSIAR_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_RESERVED1_MASK) >> HVSYNC_HVSIAR_RESERVED1_SHIFT)
#define  HVSYNC_HVSIAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_RESERVED1_SHIFT))

#define  HVSYNC_HVSIAR_MISSEDHSYNCINT_MASK 0x78000000
#define  HVSYNC_HVSIAR_MISSEDHSYNCINT_SHIFT 27
#define  HVSYNC_HVSIAR_MISSEDHSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_MISSEDHSYNCINT_MASK) >> HVSYNC_HVSIAR_MISSEDHSYNCINT_SHIFT)
#define  HVSYNC_HVSIAR_MISSEDHSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_MISSEDHSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_MISSEDHSYNCINT_SHIFT))

#define  HVSYNC_HVSIAR_TOPDLYINT_MASK 0x7f80000
#define  HVSYNC_HVSIAR_TOPDLYINT_SHIFT 19
#define  HVSYNC_HVSIAR_TOPDLYINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_TOPDLYINT_MASK) >> HVSYNC_HVSIAR_TOPDLYINT_SHIFT)
#define  HVSYNC_HVSIAR_TOPDLYINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_TOPDLYINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_TOPDLYINT_SHIFT))

#define  HVSYNC_HVSIAR_BOTINT_MASK 0x40000
#define  HVSYNC_HVSIAR_BOTINT_SHIFT 18
#define  HVSYNC_HVSIAR_BOTINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_BOTINT_MASK) >> HVSYNC_HVSIAR_BOTINT_SHIFT)
#define  HVSYNC_HVSIAR_BOTINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_BOTINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_BOTINT_SHIFT))

#define  HVSYNC_HVSIAR_FTDONEINT_MASK 0x30000
#define  HVSYNC_HVSIAR_FTDONEINT_SHIFT 16
#define  HVSYNC_HVSIAR_FTDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_FTDONEINT_MASK) >> HVSYNC_HVSIAR_FTDONEINT_SHIFT)
#define  HVSYNC_HVSIAR_FTDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_FTDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_FTDONEINT_SHIFT))

#define  HVSYNC_HVSIAR_TOPMEASDONEINT_MASK 0xc000
#define  HVSYNC_HVSIAR_TOPMEASDONEINT_SHIFT 14
#define  HVSYNC_HVSIAR_TOPMEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_TOPMEASDONEINT_MASK) >> HVSYNC_HVSIAR_TOPMEASDONEINT_SHIFT)
#define  HVSYNC_HVSIAR_TOPMEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_TOPMEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_TOPMEASDONEINT_SHIFT))

#define  HVSYNC_HVSIAR_MEASDONEINT_MASK 0x3000
#define  HVSYNC_HVSIAR_MEASDONEINT_SHIFT 12
#define  HVSYNC_HVSIAR_MEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_MEASDONEINT_MASK) >> HVSYNC_HVSIAR_MEASDONEINT_SHIFT)
#define  HVSYNC_HVSIAR_MEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_MEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_MEASDONEINT_SHIFT))

#define  HVSYNC_HVSIAR_TOPQFULLINT_MASK 0xf00
#define  HVSYNC_HVSIAR_TOPQFULLINT_SHIFT 8
#define  HVSYNC_HVSIAR_TOPQFULLINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_TOPQFULLINT_MASK) >> HVSYNC_HVSIAR_TOPQFULLINT_SHIFT)
#define  HVSYNC_HVSIAR_TOPQFULLINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_TOPQFULLINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_TOPQFULLINT_SHIFT))

#define  HVSYNC_HVSIAR_TOPINT_MASK 0xf0
#define  HVSYNC_HVSIAR_TOPINT_SHIFT 4
#define  HVSYNC_HVSIAR_TOPINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_TOPINT_MASK) >> HVSYNC_HVSIAR_TOPINT_SHIFT)
#define  HVSYNC_HVSIAR_TOPINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_TOPINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_TOPINT_SHIFT))

#define  HVSYNC_HVSIAR_HSYNCINT_MASK 0xf
#define  HVSYNC_HVSIAR_HSYNCINT_SHIFT 0
#define  HVSYNC_HVSIAR_HSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIAR_HSYNCINT_MASK) >> HVSYNC_HVSIAR_HSYNCINT_SHIFT)
#define  HVSYNC_HVSIAR_HSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIAR_HSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIAR_HSYNCINT_SHIFT))

//====================================================================
//Register: HVSync Interrupt Force Register (HVSIFR)
/** \brief HVSync Interrupt Force Register.*/
//====================================================================

#define  HVSYNC_HVSIFR_RESERVED1_MASK 0x80000000
#define  HVSYNC_HVSIFR_RESERVED1_SHIFT 31
#define  HVSYNC_HVSIFR_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_RESERVED1_MASK) >> HVSYNC_HVSIFR_RESERVED1_SHIFT)
#define  HVSYNC_HVSIFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_RESERVED1_SHIFT))

#define  HVSYNC_HVSIFR_MISSEDHSYNCINT_MASK 0x78000000
#define  HVSYNC_HVSIFR_MISSEDHSYNCINT_SHIFT 27
#define  HVSYNC_HVSIFR_MISSEDHSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_MISSEDHSYNCINT_MASK) >> HVSYNC_HVSIFR_MISSEDHSYNCINT_SHIFT)
#define  HVSYNC_HVSIFR_MISSEDHSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_MISSEDHSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_MISSEDHSYNCINT_SHIFT))

#define  HVSYNC_HVSIFR_TOPDLYINT_MASK 0x7f80000
#define  HVSYNC_HVSIFR_TOPDLYINT_SHIFT 19
#define  HVSYNC_HVSIFR_TOPDLYINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_TOPDLYINT_MASK) >> HVSYNC_HVSIFR_TOPDLYINT_SHIFT)
#define  HVSYNC_HVSIFR_TOPDLYINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_TOPDLYINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_TOPDLYINT_SHIFT))

#define  HVSYNC_HVSIFR_BOTINT_MASK 0x40000
#define  HVSYNC_HVSIFR_BOTINT_SHIFT 18
#define  HVSYNC_HVSIFR_BOTINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_BOTINT_MASK) >> HVSYNC_HVSIFR_BOTINT_SHIFT)
#define  HVSYNC_HVSIFR_BOTINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_BOTINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_BOTINT_SHIFT))

#define  HVSYNC_HVSIFR_FTDONEINT_MASK 0x30000
#define  HVSYNC_HVSIFR_FTDONEINT_SHIFT 16
#define  HVSYNC_HVSIFR_FTDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_FTDONEINT_MASK) >> HVSYNC_HVSIFR_FTDONEINT_SHIFT)
#define  HVSYNC_HVSIFR_FTDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_FTDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_FTDONEINT_SHIFT))

#define  HVSYNC_HVSIFR_TOPMEASDONEINT_MASK 0xc000
#define  HVSYNC_HVSIFR_TOPMEASDONEINT_SHIFT 14
#define  HVSYNC_HVSIFR_TOPMEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_TOPMEASDONEINT_MASK) >> HVSYNC_HVSIFR_TOPMEASDONEINT_SHIFT)
#define  HVSYNC_HVSIFR_TOPMEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_TOPMEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_TOPMEASDONEINT_SHIFT))

#define  HVSYNC_HVSIFR_MEASDONEINT_MASK 0x3000
#define  HVSYNC_HVSIFR_MEASDONEINT_SHIFT 12
#define  HVSYNC_HVSIFR_MEASDONEINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_MEASDONEINT_MASK) >> HVSYNC_HVSIFR_MEASDONEINT_SHIFT)
#define  HVSYNC_HVSIFR_MEASDONEINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_MEASDONEINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_MEASDONEINT_SHIFT))

#define  HVSYNC_HVSIFR_TOPQFULLINT_MASK 0xf00
#define  HVSYNC_HVSIFR_TOPQFULLINT_SHIFT 8
#define  HVSYNC_HVSIFR_TOPQFULLINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_TOPQFULLINT_MASK) >> HVSYNC_HVSIFR_TOPQFULLINT_SHIFT)
#define  HVSYNC_HVSIFR_TOPQFULLINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_TOPQFULLINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_TOPQFULLINT_SHIFT))

#define  HVSYNC_HVSIFR_TOPINT_MASK 0xf0
#define  HVSYNC_HVSIFR_TOPINT_SHIFT 4
#define  HVSYNC_HVSIFR_TOPINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_TOPINT_MASK) >> HVSYNC_HVSIFR_TOPINT_SHIFT)
#define  HVSYNC_HVSIFR_TOPINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_TOPINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_TOPINT_SHIFT))

#define  HVSYNC_HVSIFR_HSYNCINT_MASK 0xf
#define  HVSYNC_HVSIFR_HSYNCINT_SHIFT 0
#define  HVSYNC_HVSIFR_HSYNCINT_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSIFR_HSYNCINT_MASK) >> HVSYNC_HVSIFR_HSYNCINT_SHIFT)
#define  HVSYNC_HVSIFR_HSYNCINT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSIFR_HSYNCINT_MASK) | (((uint32_t)val) << HVSYNC_HVSIFR_HSYNCINT_SHIFT))

//====================================================================
//Register: Top Margin Register (Instance 1 of 4) (HVSTPMR0)
/** \brief Top Margins Register (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HVSTPMR0_RESERVED1_MASK 0xffff0000
#define  HVSYNC_HVSTPMR0_RESERVED1_SHIFT 16
#define  HVSYNC_HVSTPMR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR0_RESERVED1_MASK) >> HVSYNC_HVSTPMR0_RESERVED1_SHIFT)
#define  HVSYNC_HVSTPMR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR0_RESERVED1_SHIFT))

#define  HVSYNC_HVSTPMR0_TOPMARGIN_MASK 0xffff
#define  HVSYNC_HVSTPMR0_TOPMARGIN_SHIFT 0
#define  HVSYNC_HVSTPMR0_TOPMARGIN_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR0_TOPMARGIN_MASK) >> HVSYNC_HVSTPMR0_TOPMARGIN_SHIFT)
#define  HVSYNC_HVSTPMR0_TOPMARGIN_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR0_TOPMARGIN_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR0_TOPMARGIN_SHIFT))

//====================================================================
//Register: Top Margin Register (Instance 2 of 4) (HVSTPMR1)
/** \brief Top Margins Register (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HVSTPMR1_RESERVED1_MASK 0xffff0000
#define  HVSYNC_HVSTPMR1_RESERVED1_SHIFT 16
#define  HVSYNC_HVSTPMR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR1_RESERVED1_MASK) >> HVSYNC_HVSTPMR1_RESERVED1_SHIFT)
#define  HVSYNC_HVSTPMR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR1_RESERVED1_SHIFT))

#define  HVSYNC_HVSTPMR1_TOPMARGIN_MASK 0xffff
#define  HVSYNC_HVSTPMR1_TOPMARGIN_SHIFT 0
#define  HVSYNC_HVSTPMR1_TOPMARGIN_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR1_TOPMARGIN_MASK) >> HVSYNC_HVSTPMR1_TOPMARGIN_SHIFT)
#define  HVSYNC_HVSTPMR1_TOPMARGIN_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR1_TOPMARGIN_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR1_TOPMARGIN_SHIFT))

//====================================================================
//Register: Top Margin Register (Instance 3 of 4) (HVSTPMR2)
/** \brief Top Margins Register (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HVSTPMR2_RESERVED1_MASK 0xffff0000
#define  HVSYNC_HVSTPMR2_RESERVED1_SHIFT 16
#define  HVSYNC_HVSTPMR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR2_RESERVED1_MASK) >> HVSYNC_HVSTPMR2_RESERVED1_SHIFT)
#define  HVSYNC_HVSTPMR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR2_RESERVED1_SHIFT))

#define  HVSYNC_HVSTPMR2_TOPMARGIN_MASK 0xffff
#define  HVSYNC_HVSTPMR2_TOPMARGIN_SHIFT 0
#define  HVSYNC_HVSTPMR2_TOPMARGIN_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR2_TOPMARGIN_MASK) >> HVSYNC_HVSTPMR2_TOPMARGIN_SHIFT)
#define  HVSYNC_HVSTPMR2_TOPMARGIN_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR2_TOPMARGIN_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR2_TOPMARGIN_SHIFT))

//====================================================================
//Register: Top Margin Register (Instance 4 of 4) (HVSTPMR3)
/** \brief Top Margins Register (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HVSTPMR3_RESERVED1_MASK 0xffff0000
#define  HVSYNC_HVSTPMR3_RESERVED1_SHIFT 16
#define  HVSYNC_HVSTPMR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR3_RESERVED1_MASK) >> HVSYNC_HVSTPMR3_RESERVED1_SHIFT)
#define  HVSYNC_HVSTPMR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR3_RESERVED1_SHIFT))

#define  HVSYNC_HVSTPMR3_TOPMARGIN_MASK 0xffff
#define  HVSYNC_HVSTPMR3_TOPMARGIN_SHIFT 0
#define  HVSYNC_HVSTPMR3_TOPMARGIN_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTPMR3_TOPMARGIN_MASK) >> HVSYNC_HVSTPMR3_TOPMARGIN_SHIFT)
#define  HVSYNC_HVSTPMR3_TOPMARGIN_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTPMR3_TOPMARGIN_MASK) | (((uint32_t)val) << HVSYNC_HVSTPMR3_TOPMARGIN_SHIFT))

//====================================================================
//Register: HSYNC Filter Configuration Register (Instance 1 of 4) (HFCR0)
/** \brief Configures the HSYNC Filter (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HFCR0_RESERVED1_MASK 0xf0000000
#define  HVSYNC_HFCR0_RESERVED1_SHIFT 28
#define  HVSYNC_HFCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_RESERVED1_MASK) >> HVSYNC_HFCR0_RESERVED1_SHIFT)
#define  HVSYNC_HFCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_RESERVED1_SHIFT))

#define  HVSYNC_HFCR0_HIGHSTABLE_MASK 0xf800000
#define  HVSYNC_HFCR0_HIGHSTABLE_SHIFT 23
#define  HVSYNC_HFCR0_HIGHSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_HIGHSTABLE_MASK) >> HVSYNC_HFCR0_HIGHSTABLE_SHIFT)
#define  HVSYNC_HFCR0_HIGHSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_HIGHSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_HIGHSTABLE_SHIFT))

#define  HVSYNC_HFCR0_LOWSTABLE_MASK 0x7c0000
#define  HVSYNC_HFCR0_LOWSTABLE_SHIFT 18
#define  HVSYNC_HFCR0_LOWSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_LOWSTABLE_MASK) >> HVSYNC_HFCR0_LOWSTABLE_SHIFT)
#define  HVSYNC_HFCR0_LOWSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_LOWSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_LOWSTABLE_SHIFT))

#define  HVSYNC_HFCR0_H2LTHRESH_MASK 0x3e000
#define  HVSYNC_HFCR0_H2LTHRESH_SHIFT 13
#define  HVSYNC_HFCR0_H2LTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_H2LTHRESH_MASK) >> HVSYNC_HFCR0_H2LTHRESH_SHIFT)
#define  HVSYNC_HFCR0_H2LTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_H2LTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_H2LTHRESH_SHIFT))

#define  HVSYNC_HFCR0_L2HTHRESH_MASK 0x1f00
#define  HVSYNC_HFCR0_L2HTHRESH_SHIFT 8
#define  HVSYNC_HFCR0_L2HTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_L2HTHRESH_MASK) >> HVSYNC_HFCR0_L2HTHRESH_SHIFT)
#define  HVSYNC_HFCR0_L2HTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_L2HTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_L2HTHRESH_SHIFT))

#define  HVSYNC_HFCR0_RESERVED2_MASK 0x80
#define  HVSYNC_HFCR0_RESERVED2_SHIFT 7
#define  HVSYNC_HFCR0_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_RESERVED2_MASK) >> HVSYNC_HFCR0_RESERVED2_SHIFT)
#define  HVSYNC_HFCR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_RESERVED2_SHIFT))

#define  HVSYNC_HFCR0_ENGHSYNCOUT_MASK 0x70
#define  HVSYNC_HFCR0_ENGHSYNCOUT_SHIFT 4
#define  HVSYNC_HFCR0_ENGHSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_ENGHSYNCOUT_MASK) >> HVSYNC_HFCR0_ENGHSYNCOUT_SHIFT)
#define  HVSYNC_HFCR0_ENGHSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_ENGHSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_ENGHSYNCOUT_SHIFT))

#define  HVSYNC_HFCR0_NUMEDGE_MASK 0xc
#define  HVSYNC_HFCR0_NUMEDGE_SHIFT 2
#define  HVSYNC_HFCR0_NUMEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_NUMEDGE_MASK) >> HVSYNC_HFCR0_NUMEDGE_SHIFT)
#define  HVSYNC_HFCR0_NUMEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_NUMEDGE_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_NUMEDGE_SHIFT))

#define  HVSYNC_HFCR0_HSYNCEDGESEL_MASK 0x2
#define  HVSYNC_HFCR0_HSYNCEDGESEL_SHIFT 1
#define  HVSYNC_HFCR0_HSYNCEDGESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_HSYNCEDGESEL_MASK) >> HVSYNC_HFCR0_HSYNCEDGESEL_SHIFT)
#define  HVSYNC_HFCR0_HSYNCEDGESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_HSYNCEDGESEL_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_HSYNCEDGESEL_SHIFT))

#define  HVSYNC_HFCR0_HSYNCENABLE_MASK 0x1
#define  HVSYNC_HFCR0_HSYNCENABLE_SHIFT 0
#define  HVSYNC_HFCR0_HSYNCENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR0_HSYNCENABLE_MASK) >> HVSYNC_HFCR0_HSYNCENABLE_SHIFT)
#define  HVSYNC_HFCR0_HSYNCENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR0_HSYNCENABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR0_HSYNCENABLE_SHIFT))

//====================================================================
//Register: HSYNC Filter Configuration Register (Instance 2 of 4) (HFCR1)
/** \brief Configures the HSYNC Filter (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HFCR1_RESERVED1_MASK 0xf0000000
#define  HVSYNC_HFCR1_RESERVED1_SHIFT 28
#define  HVSYNC_HFCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_RESERVED1_MASK) >> HVSYNC_HFCR1_RESERVED1_SHIFT)
#define  HVSYNC_HFCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_RESERVED1_SHIFT))

#define  HVSYNC_HFCR1_HIGHSTABLE_MASK 0xf800000
#define  HVSYNC_HFCR1_HIGHSTABLE_SHIFT 23
#define  HVSYNC_HFCR1_HIGHSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_HIGHSTABLE_MASK) >> HVSYNC_HFCR1_HIGHSTABLE_SHIFT)
#define  HVSYNC_HFCR1_HIGHSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_HIGHSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_HIGHSTABLE_SHIFT))

#define  HVSYNC_HFCR1_LOWSTABLE_MASK 0x7c0000
#define  HVSYNC_HFCR1_LOWSTABLE_SHIFT 18
#define  HVSYNC_HFCR1_LOWSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_LOWSTABLE_MASK) >> HVSYNC_HFCR1_LOWSTABLE_SHIFT)
#define  HVSYNC_HFCR1_LOWSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_LOWSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_LOWSTABLE_SHIFT))

#define  HVSYNC_HFCR1_H2LTHRESH_MASK 0x3e000
#define  HVSYNC_HFCR1_H2LTHRESH_SHIFT 13
#define  HVSYNC_HFCR1_H2LTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_H2LTHRESH_MASK) >> HVSYNC_HFCR1_H2LTHRESH_SHIFT)
#define  HVSYNC_HFCR1_H2LTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_H2LTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_H2LTHRESH_SHIFT))

#define  HVSYNC_HFCR1_L2HTHRESH_MASK 0x1f00
#define  HVSYNC_HFCR1_L2HTHRESH_SHIFT 8
#define  HVSYNC_HFCR1_L2HTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_L2HTHRESH_MASK) >> HVSYNC_HFCR1_L2HTHRESH_SHIFT)
#define  HVSYNC_HFCR1_L2HTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_L2HTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_L2HTHRESH_SHIFT))

#define  HVSYNC_HFCR1_RESERVED2_MASK 0x80
#define  HVSYNC_HFCR1_RESERVED2_SHIFT 7
#define  HVSYNC_HFCR1_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_RESERVED2_MASK) >> HVSYNC_HFCR1_RESERVED2_SHIFT)
#define  HVSYNC_HFCR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_RESERVED2_SHIFT))

#define  HVSYNC_HFCR1_ENGHSYNCOUT_MASK 0x70
#define  HVSYNC_HFCR1_ENGHSYNCOUT_SHIFT 4
#define  HVSYNC_HFCR1_ENGHSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_ENGHSYNCOUT_MASK) >> HVSYNC_HFCR1_ENGHSYNCOUT_SHIFT)
#define  HVSYNC_HFCR1_ENGHSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_ENGHSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_ENGHSYNCOUT_SHIFT))

#define  HVSYNC_HFCR1_NUMEDGE_MASK 0xc
#define  HVSYNC_HFCR1_NUMEDGE_SHIFT 2
#define  HVSYNC_HFCR1_NUMEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_NUMEDGE_MASK) >> HVSYNC_HFCR1_NUMEDGE_SHIFT)
#define  HVSYNC_HFCR1_NUMEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_NUMEDGE_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_NUMEDGE_SHIFT))

#define  HVSYNC_HFCR1_HSYNCEDGESEL_MASK 0x2
#define  HVSYNC_HFCR1_HSYNCEDGESEL_SHIFT 1
#define  HVSYNC_HFCR1_HSYNCEDGESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_HSYNCEDGESEL_MASK) >> HVSYNC_HFCR1_HSYNCEDGESEL_SHIFT)
#define  HVSYNC_HFCR1_HSYNCEDGESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_HSYNCEDGESEL_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_HSYNCEDGESEL_SHIFT))

#define  HVSYNC_HFCR1_HSYNCENABLE_MASK 0x1
#define  HVSYNC_HFCR1_HSYNCENABLE_SHIFT 0
#define  HVSYNC_HFCR1_HSYNCENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR1_HSYNCENABLE_MASK) >> HVSYNC_HFCR1_HSYNCENABLE_SHIFT)
#define  HVSYNC_HFCR1_HSYNCENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR1_HSYNCENABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR1_HSYNCENABLE_SHIFT))

//====================================================================
//Register: HSYNC Filter Configuration Register (Instance 3 of 4) (HFCR2)
/** \brief Configures the HSYNC Filter (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HFCR2_RESERVED1_MASK 0xf0000000
#define  HVSYNC_HFCR2_RESERVED1_SHIFT 28
#define  HVSYNC_HFCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_RESERVED1_MASK) >> HVSYNC_HFCR2_RESERVED1_SHIFT)
#define  HVSYNC_HFCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_RESERVED1_SHIFT))

#define  HVSYNC_HFCR2_HIGHSTABLE_MASK 0xf800000
#define  HVSYNC_HFCR2_HIGHSTABLE_SHIFT 23
#define  HVSYNC_HFCR2_HIGHSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_HIGHSTABLE_MASK) >> HVSYNC_HFCR2_HIGHSTABLE_SHIFT)
#define  HVSYNC_HFCR2_HIGHSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_HIGHSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_HIGHSTABLE_SHIFT))

#define  HVSYNC_HFCR2_LOWSTABLE_MASK 0x7c0000
#define  HVSYNC_HFCR2_LOWSTABLE_SHIFT 18
#define  HVSYNC_HFCR2_LOWSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_LOWSTABLE_MASK) >> HVSYNC_HFCR2_LOWSTABLE_SHIFT)
#define  HVSYNC_HFCR2_LOWSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_LOWSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_LOWSTABLE_SHIFT))

#define  HVSYNC_HFCR2_H2LTHRESH_MASK 0x3e000
#define  HVSYNC_HFCR2_H2LTHRESH_SHIFT 13
#define  HVSYNC_HFCR2_H2LTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_H2LTHRESH_MASK) >> HVSYNC_HFCR2_H2LTHRESH_SHIFT)
#define  HVSYNC_HFCR2_H2LTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_H2LTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_H2LTHRESH_SHIFT))

#define  HVSYNC_HFCR2_L2HTHRESH_MASK 0x1f00
#define  HVSYNC_HFCR2_L2HTHRESH_SHIFT 8
#define  HVSYNC_HFCR2_L2HTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_L2HTHRESH_MASK) >> HVSYNC_HFCR2_L2HTHRESH_SHIFT)
#define  HVSYNC_HFCR2_L2HTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_L2HTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_L2HTHRESH_SHIFT))

#define  HVSYNC_HFCR2_RESERVED2_MASK 0x80
#define  HVSYNC_HFCR2_RESERVED2_SHIFT 7
#define  HVSYNC_HFCR2_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_RESERVED2_MASK) >> HVSYNC_HFCR2_RESERVED2_SHIFT)
#define  HVSYNC_HFCR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_RESERVED2_SHIFT))

#define  HVSYNC_HFCR2_ENGHSYNCOUT_MASK 0x70
#define  HVSYNC_HFCR2_ENGHSYNCOUT_SHIFT 4
#define  HVSYNC_HFCR2_ENGHSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_ENGHSYNCOUT_MASK) >> HVSYNC_HFCR2_ENGHSYNCOUT_SHIFT)
#define  HVSYNC_HFCR2_ENGHSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_ENGHSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_ENGHSYNCOUT_SHIFT))

#define  HVSYNC_HFCR2_NUMEDGE_MASK 0xc
#define  HVSYNC_HFCR2_NUMEDGE_SHIFT 2
#define  HVSYNC_HFCR2_NUMEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_NUMEDGE_MASK) >> HVSYNC_HFCR2_NUMEDGE_SHIFT)
#define  HVSYNC_HFCR2_NUMEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_NUMEDGE_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_NUMEDGE_SHIFT))

#define  HVSYNC_HFCR2_HSYNCEDGESEL_MASK 0x2
#define  HVSYNC_HFCR2_HSYNCEDGESEL_SHIFT 1
#define  HVSYNC_HFCR2_HSYNCEDGESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_HSYNCEDGESEL_MASK) >> HVSYNC_HFCR2_HSYNCEDGESEL_SHIFT)
#define  HVSYNC_HFCR2_HSYNCEDGESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_HSYNCEDGESEL_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_HSYNCEDGESEL_SHIFT))

#define  HVSYNC_HFCR2_HSYNCENABLE_MASK 0x1
#define  HVSYNC_HFCR2_HSYNCENABLE_SHIFT 0
#define  HVSYNC_HFCR2_HSYNCENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR2_HSYNCENABLE_MASK) >> HVSYNC_HFCR2_HSYNCENABLE_SHIFT)
#define  HVSYNC_HFCR2_HSYNCENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR2_HSYNCENABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR2_HSYNCENABLE_SHIFT))

//====================================================================
//Register: HSYNC Filter Configuration Register (Instance 4 of 4) (HFCR3)
/** \brief Configures the HSYNC Filter (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HFCR3_RESERVED1_MASK 0xf0000000
#define  HVSYNC_HFCR3_RESERVED1_SHIFT 28
#define  HVSYNC_HFCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_RESERVED1_MASK) >> HVSYNC_HFCR3_RESERVED1_SHIFT)
#define  HVSYNC_HFCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_RESERVED1_SHIFT))

#define  HVSYNC_HFCR3_HIGHSTABLE_MASK 0xf800000
#define  HVSYNC_HFCR3_HIGHSTABLE_SHIFT 23
#define  HVSYNC_HFCR3_HIGHSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_HIGHSTABLE_MASK) >> HVSYNC_HFCR3_HIGHSTABLE_SHIFT)
#define  HVSYNC_HFCR3_HIGHSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_HIGHSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_HIGHSTABLE_SHIFT))

#define  HVSYNC_HFCR3_LOWSTABLE_MASK 0x7c0000
#define  HVSYNC_HFCR3_LOWSTABLE_SHIFT 18
#define  HVSYNC_HFCR3_LOWSTABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_LOWSTABLE_MASK) >> HVSYNC_HFCR3_LOWSTABLE_SHIFT)
#define  HVSYNC_HFCR3_LOWSTABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_LOWSTABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_LOWSTABLE_SHIFT))

#define  HVSYNC_HFCR3_H2LTHRESH_MASK 0x3e000
#define  HVSYNC_HFCR3_H2LTHRESH_SHIFT 13
#define  HVSYNC_HFCR3_H2LTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_H2LTHRESH_MASK) >> HVSYNC_HFCR3_H2LTHRESH_SHIFT)
#define  HVSYNC_HFCR3_H2LTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_H2LTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_H2LTHRESH_SHIFT))

#define  HVSYNC_HFCR3_L2HTHRESH_MASK 0x1f00
#define  HVSYNC_HFCR3_L2HTHRESH_SHIFT 8
#define  HVSYNC_HFCR3_L2HTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_L2HTHRESH_MASK) >> HVSYNC_HFCR3_L2HTHRESH_SHIFT)
#define  HVSYNC_HFCR3_L2HTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_L2HTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_L2HTHRESH_SHIFT))

#define  HVSYNC_HFCR3_RESERVED2_MASK 0x80
#define  HVSYNC_HFCR3_RESERVED2_SHIFT 7
#define  HVSYNC_HFCR3_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_RESERVED2_MASK) >> HVSYNC_HFCR3_RESERVED2_SHIFT)
#define  HVSYNC_HFCR3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_RESERVED2_SHIFT))

#define  HVSYNC_HFCR3_ENGHSYNCOUT_MASK 0x70
#define  HVSYNC_HFCR3_ENGHSYNCOUT_SHIFT 4
#define  HVSYNC_HFCR3_ENGHSYNCOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_ENGHSYNCOUT_MASK) >> HVSYNC_HFCR3_ENGHSYNCOUT_SHIFT)
#define  HVSYNC_HFCR3_ENGHSYNCOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_ENGHSYNCOUT_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_ENGHSYNCOUT_SHIFT))

#define  HVSYNC_HFCR3_NUMEDGE_MASK 0xc
#define  HVSYNC_HFCR3_NUMEDGE_SHIFT 2
#define  HVSYNC_HFCR3_NUMEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_NUMEDGE_MASK) >> HVSYNC_HFCR3_NUMEDGE_SHIFT)
#define  HVSYNC_HFCR3_NUMEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_NUMEDGE_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_NUMEDGE_SHIFT))

#define  HVSYNC_HFCR3_HSYNCEDGESEL_MASK 0x2
#define  HVSYNC_HFCR3_HSYNCEDGESEL_SHIFT 1
#define  HVSYNC_HFCR3_HSYNCEDGESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_HSYNCEDGESEL_MASK) >> HVSYNC_HFCR3_HSYNCEDGESEL_SHIFT)
#define  HVSYNC_HFCR3_HSYNCEDGESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_HSYNCEDGESEL_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_HSYNCEDGESEL_SHIFT))

#define  HVSYNC_HFCR3_HSYNCENABLE_MASK 0x1
#define  HVSYNC_HFCR3_HSYNCENABLE_SHIFT 0
#define  HVSYNC_HFCR3_HSYNCENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFCR3_HSYNCENABLE_MASK) >> HVSYNC_HFCR3_HSYNCENABLE_SHIFT)
#define  HVSYNC_HFCR3_HSYNCENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFCR3_HSYNCENABLE_MASK) | (((uint32_t)val) << HVSYNC_HFCR3_HSYNCENABLE_SHIFT))

//====================================================================
//Register: HSYNC FIR Configuration Register (Instance 1 of 4) (HFFCR0)
/** \brief Configures the HSYNC FIR Filter (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HFFCR0_RESERVED1_MASK 0xff000000
#define  HVSYNC_HFFCR0_RESERVED1_SHIFT 24
#define  HVSYNC_HFFCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_RESERVED1_MASK) >> HVSYNC_HFFCR0_RESERVED1_SHIFT)
#define  HVSYNC_HFFCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_RESERVED1_SHIFT))

#define  HVSYNC_HFFCR0_H2LFIRTHRESH_MASK 0xf00000
#define  HVSYNC_HFFCR0_H2LFIRTHRESH_SHIFT 20
#define  HVSYNC_HFFCR0_H2LFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_H2LFIRTHRESH_MASK) >> HVSYNC_HFFCR0_H2LFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR0_H2LFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_H2LFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_H2LFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR0_L2HFIRTHRESH_MASK 0xf0000
#define  HVSYNC_HFFCR0_L2HFIRTHRESH_SHIFT 16
#define  HVSYNC_HFFCR0_L2HFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_L2HFIRTHRESH_MASK) >> HVSYNC_HFFCR0_L2HFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR0_L2HFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_L2HFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_L2HFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR0_RESERVED2_MASK 0xc000
#define  HVSYNC_HFFCR0_RESERVED2_SHIFT 14
#define  HVSYNC_HFFCR0_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_RESERVED2_MASK) >> HVSYNC_HFFCR0_RESERVED2_SHIFT)
#define  HVSYNC_HFFCR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_RESERVED2_SHIFT))

#define  HVSYNC_HFFCR0_WEIGHT4_MASK 0x3000
#define  HVSYNC_HFFCR0_WEIGHT4_SHIFT 12
#define  HVSYNC_HFFCR0_WEIGHT4_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_WEIGHT4_MASK) >> HVSYNC_HFFCR0_WEIGHT4_SHIFT)
#define  HVSYNC_HFFCR0_WEIGHT4_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_WEIGHT4_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_WEIGHT4_SHIFT))

#define  HVSYNC_HFFCR0_WEIGHT3_MASK 0xc00
#define  HVSYNC_HFFCR0_WEIGHT3_SHIFT 10
#define  HVSYNC_HFFCR0_WEIGHT3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_WEIGHT3_MASK) >> HVSYNC_HFFCR0_WEIGHT3_SHIFT)
#define  HVSYNC_HFFCR0_WEIGHT3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_WEIGHT3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_WEIGHT3_SHIFT))

#define  HVSYNC_HFFCR0_WEIGHT2_MASK 0x300
#define  HVSYNC_HFFCR0_WEIGHT2_SHIFT 8
#define  HVSYNC_HFFCR0_WEIGHT2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_WEIGHT2_MASK) >> HVSYNC_HFFCR0_WEIGHT2_SHIFT)
#define  HVSYNC_HFFCR0_WEIGHT2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_WEIGHT2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_WEIGHT2_SHIFT))

#define  HVSYNC_HFFCR0_WEIGHT1_MASK 0xc0
#define  HVSYNC_HFFCR0_WEIGHT1_SHIFT 6
#define  HVSYNC_HFFCR0_WEIGHT1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_WEIGHT1_MASK) >> HVSYNC_HFFCR0_WEIGHT1_SHIFT)
#define  HVSYNC_HFFCR0_WEIGHT1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_WEIGHT1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_WEIGHT1_SHIFT))

#define  HVSYNC_HFFCR0_WEIGHT0_MASK 0x30
#define  HVSYNC_HFFCR0_WEIGHT0_SHIFT 4
#define  HVSYNC_HFFCR0_WEIGHT0_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_WEIGHT0_MASK) >> HVSYNC_HFFCR0_WEIGHT0_SHIFT)
#define  HVSYNC_HFFCR0_WEIGHT0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_WEIGHT0_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_WEIGHT0_SHIFT))

#define  HVSYNC_HFFCR0_RESERVED3_MASK 0xe
#define  HVSYNC_HFFCR0_RESERVED3_SHIFT 1
#define  HVSYNC_HFFCR0_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_RESERVED3_MASK) >> HVSYNC_HFFCR0_RESERVED3_SHIFT)
#define  HVSYNC_HFFCR0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_RESERVED3_SHIFT))

#define  HVSYNC_HFFCR0_FIRBYPASS_MASK 0x1
#define  HVSYNC_HFFCR0_FIRBYPASS_SHIFT 0
#define  HVSYNC_HFFCR0_FIRBYPASS_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR0_FIRBYPASS_MASK) >> HVSYNC_HFFCR0_FIRBYPASS_SHIFT)
#define  HVSYNC_HFFCR0_FIRBYPASS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR0_FIRBYPASS_MASK) | (((uint32_t)val) << HVSYNC_HFFCR0_FIRBYPASS_SHIFT))

//====================================================================
//Register: HSYNC FIR Configuration Register (Instance 2 of 4) (HFFCR1)
/** \brief Configures the HSYNC FIR Filter (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HFFCR1_RESERVED1_MASK 0xff000000
#define  HVSYNC_HFFCR1_RESERVED1_SHIFT 24
#define  HVSYNC_HFFCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_RESERVED1_MASK) >> HVSYNC_HFFCR1_RESERVED1_SHIFT)
#define  HVSYNC_HFFCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_RESERVED1_SHIFT))

#define  HVSYNC_HFFCR1_H2LFIRTHRESH_MASK 0xf00000
#define  HVSYNC_HFFCR1_H2LFIRTHRESH_SHIFT 20
#define  HVSYNC_HFFCR1_H2LFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_H2LFIRTHRESH_MASK) >> HVSYNC_HFFCR1_H2LFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR1_H2LFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_H2LFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_H2LFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR1_L2HFIRTHRESH_MASK 0xf0000
#define  HVSYNC_HFFCR1_L2HFIRTHRESH_SHIFT 16
#define  HVSYNC_HFFCR1_L2HFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_L2HFIRTHRESH_MASK) >> HVSYNC_HFFCR1_L2HFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR1_L2HFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_L2HFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_L2HFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR1_RESERVED2_MASK 0xc000
#define  HVSYNC_HFFCR1_RESERVED2_SHIFT 14
#define  HVSYNC_HFFCR1_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_RESERVED2_MASK) >> HVSYNC_HFFCR1_RESERVED2_SHIFT)
#define  HVSYNC_HFFCR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_RESERVED2_SHIFT))

#define  HVSYNC_HFFCR1_WEIGHT4_MASK 0x3000
#define  HVSYNC_HFFCR1_WEIGHT4_SHIFT 12
#define  HVSYNC_HFFCR1_WEIGHT4_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_WEIGHT4_MASK) >> HVSYNC_HFFCR1_WEIGHT4_SHIFT)
#define  HVSYNC_HFFCR1_WEIGHT4_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_WEIGHT4_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_WEIGHT4_SHIFT))

#define  HVSYNC_HFFCR1_WEIGHT3_MASK 0xc00
#define  HVSYNC_HFFCR1_WEIGHT3_SHIFT 10
#define  HVSYNC_HFFCR1_WEIGHT3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_WEIGHT3_MASK) >> HVSYNC_HFFCR1_WEIGHT3_SHIFT)
#define  HVSYNC_HFFCR1_WEIGHT3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_WEIGHT3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_WEIGHT3_SHIFT))

#define  HVSYNC_HFFCR1_WEIGHT2_MASK 0x300
#define  HVSYNC_HFFCR1_WEIGHT2_SHIFT 8
#define  HVSYNC_HFFCR1_WEIGHT2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_WEIGHT2_MASK) >> HVSYNC_HFFCR1_WEIGHT2_SHIFT)
#define  HVSYNC_HFFCR1_WEIGHT2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_WEIGHT2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_WEIGHT2_SHIFT))

#define  HVSYNC_HFFCR1_WEIGHT1_MASK 0xc0
#define  HVSYNC_HFFCR1_WEIGHT1_SHIFT 6
#define  HVSYNC_HFFCR1_WEIGHT1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_WEIGHT1_MASK) >> HVSYNC_HFFCR1_WEIGHT1_SHIFT)
#define  HVSYNC_HFFCR1_WEIGHT1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_WEIGHT1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_WEIGHT1_SHIFT))

#define  HVSYNC_HFFCR1_WEIGHT0_MASK 0x30
#define  HVSYNC_HFFCR1_WEIGHT0_SHIFT 4
#define  HVSYNC_HFFCR1_WEIGHT0_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_WEIGHT0_MASK) >> HVSYNC_HFFCR1_WEIGHT0_SHIFT)
#define  HVSYNC_HFFCR1_WEIGHT0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_WEIGHT0_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_WEIGHT0_SHIFT))

#define  HVSYNC_HFFCR1_RESERVED3_MASK 0xe
#define  HVSYNC_HFFCR1_RESERVED3_SHIFT 1
#define  HVSYNC_HFFCR1_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_RESERVED3_MASK) >> HVSYNC_HFFCR1_RESERVED3_SHIFT)
#define  HVSYNC_HFFCR1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_RESERVED3_SHIFT))

#define  HVSYNC_HFFCR1_FIRBYPASS_MASK 0x1
#define  HVSYNC_HFFCR1_FIRBYPASS_SHIFT 0
#define  HVSYNC_HFFCR1_FIRBYPASS_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR1_FIRBYPASS_MASK) >> HVSYNC_HFFCR1_FIRBYPASS_SHIFT)
#define  HVSYNC_HFFCR1_FIRBYPASS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR1_FIRBYPASS_MASK) | (((uint32_t)val) << HVSYNC_HFFCR1_FIRBYPASS_SHIFT))

//====================================================================
//Register: HSYNC FIR Configuration Register (Instance 3 of 4) (HFFCR2)
/** \brief Configures the HSYNC FIR Filter (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HFFCR2_RESERVED1_MASK 0xff000000
#define  HVSYNC_HFFCR2_RESERVED1_SHIFT 24
#define  HVSYNC_HFFCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_RESERVED1_MASK) >> HVSYNC_HFFCR2_RESERVED1_SHIFT)
#define  HVSYNC_HFFCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_RESERVED1_SHIFT))

#define  HVSYNC_HFFCR2_H2LFIRTHRESH_MASK 0xf00000
#define  HVSYNC_HFFCR2_H2LFIRTHRESH_SHIFT 20
#define  HVSYNC_HFFCR2_H2LFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_H2LFIRTHRESH_MASK) >> HVSYNC_HFFCR2_H2LFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR2_H2LFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_H2LFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_H2LFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR2_L2HFIRTHRESH_MASK 0xf0000
#define  HVSYNC_HFFCR2_L2HFIRTHRESH_SHIFT 16
#define  HVSYNC_HFFCR2_L2HFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_L2HFIRTHRESH_MASK) >> HVSYNC_HFFCR2_L2HFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR2_L2HFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_L2HFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_L2HFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR2_RESERVED2_MASK 0xc000
#define  HVSYNC_HFFCR2_RESERVED2_SHIFT 14
#define  HVSYNC_HFFCR2_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_RESERVED2_MASK) >> HVSYNC_HFFCR2_RESERVED2_SHIFT)
#define  HVSYNC_HFFCR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_RESERVED2_SHIFT))

#define  HVSYNC_HFFCR2_WEIGHT4_MASK 0x3000
#define  HVSYNC_HFFCR2_WEIGHT4_SHIFT 12
#define  HVSYNC_HFFCR2_WEIGHT4_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_WEIGHT4_MASK) >> HVSYNC_HFFCR2_WEIGHT4_SHIFT)
#define  HVSYNC_HFFCR2_WEIGHT4_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_WEIGHT4_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_WEIGHT4_SHIFT))

#define  HVSYNC_HFFCR2_WEIGHT3_MASK 0xc00
#define  HVSYNC_HFFCR2_WEIGHT3_SHIFT 10
#define  HVSYNC_HFFCR2_WEIGHT3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_WEIGHT3_MASK) >> HVSYNC_HFFCR2_WEIGHT3_SHIFT)
#define  HVSYNC_HFFCR2_WEIGHT3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_WEIGHT3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_WEIGHT3_SHIFT))

#define  HVSYNC_HFFCR2_WEIGHT2_MASK 0x300
#define  HVSYNC_HFFCR2_WEIGHT2_SHIFT 8
#define  HVSYNC_HFFCR2_WEIGHT2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_WEIGHT2_MASK) >> HVSYNC_HFFCR2_WEIGHT2_SHIFT)
#define  HVSYNC_HFFCR2_WEIGHT2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_WEIGHT2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_WEIGHT2_SHIFT))

#define  HVSYNC_HFFCR2_WEIGHT1_MASK 0xc0
#define  HVSYNC_HFFCR2_WEIGHT1_SHIFT 6
#define  HVSYNC_HFFCR2_WEIGHT1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_WEIGHT1_MASK) >> HVSYNC_HFFCR2_WEIGHT1_SHIFT)
#define  HVSYNC_HFFCR2_WEIGHT1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_WEIGHT1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_WEIGHT1_SHIFT))

#define  HVSYNC_HFFCR2_WEIGHT0_MASK 0x30
#define  HVSYNC_HFFCR2_WEIGHT0_SHIFT 4
#define  HVSYNC_HFFCR2_WEIGHT0_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_WEIGHT0_MASK) >> HVSYNC_HFFCR2_WEIGHT0_SHIFT)
#define  HVSYNC_HFFCR2_WEIGHT0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_WEIGHT0_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_WEIGHT0_SHIFT))

#define  HVSYNC_HFFCR2_RESERVED3_MASK 0xe
#define  HVSYNC_HFFCR2_RESERVED3_SHIFT 1
#define  HVSYNC_HFFCR2_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_RESERVED3_MASK) >> HVSYNC_HFFCR2_RESERVED3_SHIFT)
#define  HVSYNC_HFFCR2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_RESERVED3_SHIFT))

#define  HVSYNC_HFFCR2_FIRBYPASS_MASK 0x1
#define  HVSYNC_HFFCR2_FIRBYPASS_SHIFT 0
#define  HVSYNC_HFFCR2_FIRBYPASS_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR2_FIRBYPASS_MASK) >> HVSYNC_HFFCR2_FIRBYPASS_SHIFT)
#define  HVSYNC_HFFCR2_FIRBYPASS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR2_FIRBYPASS_MASK) | (((uint32_t)val) << HVSYNC_HFFCR2_FIRBYPASS_SHIFT))

//====================================================================
//Register: HSYNC FIR Configuration Register (Instance 4 of 4) (HFFCR3)
/** \brief Configures the HSYNC FIR Filter (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HFFCR3_RESERVED1_MASK 0xff000000
#define  HVSYNC_HFFCR3_RESERVED1_SHIFT 24
#define  HVSYNC_HFFCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_RESERVED1_MASK) >> HVSYNC_HFFCR3_RESERVED1_SHIFT)
#define  HVSYNC_HFFCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_RESERVED1_SHIFT))

#define  HVSYNC_HFFCR3_H2LFIRTHRESH_MASK 0xf00000
#define  HVSYNC_HFFCR3_H2LFIRTHRESH_SHIFT 20
#define  HVSYNC_HFFCR3_H2LFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_H2LFIRTHRESH_MASK) >> HVSYNC_HFFCR3_H2LFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR3_H2LFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_H2LFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_H2LFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR3_L2HFIRTHRESH_MASK 0xf0000
#define  HVSYNC_HFFCR3_L2HFIRTHRESH_SHIFT 16
#define  HVSYNC_HFFCR3_L2HFIRTHRESH_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_L2HFIRTHRESH_MASK) >> HVSYNC_HFFCR3_L2HFIRTHRESH_SHIFT)
#define  HVSYNC_HFFCR3_L2HFIRTHRESH_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_L2HFIRTHRESH_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_L2HFIRTHRESH_SHIFT))

#define  HVSYNC_HFFCR3_RESERVED2_MASK 0xc000
#define  HVSYNC_HFFCR3_RESERVED2_SHIFT 14
#define  HVSYNC_HFFCR3_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_RESERVED2_MASK) >> HVSYNC_HFFCR3_RESERVED2_SHIFT)
#define  HVSYNC_HFFCR3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_RESERVED2_SHIFT))

#define  HVSYNC_HFFCR3_WEIGHT4_MASK 0x3000
#define  HVSYNC_HFFCR3_WEIGHT4_SHIFT 12
#define  HVSYNC_HFFCR3_WEIGHT4_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_WEIGHT4_MASK) >> HVSYNC_HFFCR3_WEIGHT4_SHIFT)
#define  HVSYNC_HFFCR3_WEIGHT4_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_WEIGHT4_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_WEIGHT4_SHIFT))

#define  HVSYNC_HFFCR3_WEIGHT3_MASK 0xc00
#define  HVSYNC_HFFCR3_WEIGHT3_SHIFT 10
#define  HVSYNC_HFFCR3_WEIGHT3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_WEIGHT3_MASK) >> HVSYNC_HFFCR3_WEIGHT3_SHIFT)
#define  HVSYNC_HFFCR3_WEIGHT3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_WEIGHT3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_WEIGHT3_SHIFT))

#define  HVSYNC_HFFCR3_WEIGHT2_MASK 0x300
#define  HVSYNC_HFFCR3_WEIGHT2_SHIFT 8
#define  HVSYNC_HFFCR3_WEIGHT2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_WEIGHT2_MASK) >> HVSYNC_HFFCR3_WEIGHT2_SHIFT)
#define  HVSYNC_HFFCR3_WEIGHT2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_WEIGHT2_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_WEIGHT2_SHIFT))

#define  HVSYNC_HFFCR3_WEIGHT1_MASK 0xc0
#define  HVSYNC_HFFCR3_WEIGHT1_SHIFT 6
#define  HVSYNC_HFFCR3_WEIGHT1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_WEIGHT1_MASK) >> HVSYNC_HFFCR3_WEIGHT1_SHIFT)
#define  HVSYNC_HFFCR3_WEIGHT1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_WEIGHT1_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_WEIGHT1_SHIFT))

#define  HVSYNC_HFFCR3_WEIGHT0_MASK 0x30
#define  HVSYNC_HFFCR3_WEIGHT0_SHIFT 4
#define  HVSYNC_HFFCR3_WEIGHT0_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_WEIGHT0_MASK) >> HVSYNC_HFFCR3_WEIGHT0_SHIFT)
#define  HVSYNC_HFFCR3_WEIGHT0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_WEIGHT0_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_WEIGHT0_SHIFT))

#define  HVSYNC_HFFCR3_RESERVED3_MASK 0xe
#define  HVSYNC_HFFCR3_RESERVED3_SHIFT 1
#define  HVSYNC_HFFCR3_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_RESERVED3_MASK) >> HVSYNC_HFFCR3_RESERVED3_SHIFT)
#define  HVSYNC_HFFCR3_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_RESERVED3_SHIFT))

#define  HVSYNC_HFFCR3_FIRBYPASS_MASK 0x1
#define  HVSYNC_HFFCR3_FIRBYPASS_SHIFT 0
#define  HVSYNC_HFFCR3_FIRBYPASS_MASK_SHIFT(reg) (((reg) & HVSYNC_HFFCR3_FIRBYPASS_MASK) >> HVSYNC_HFFCR3_FIRBYPASS_SHIFT)
#define  HVSYNC_HFFCR3_FIRBYPASS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFFCR3_FIRBYPASS_MASK) | (((uint32_t)val) << HVSYNC_HFFCR3_FIRBYPASS_SHIFT))

//====================================================================
//Register: HSYNC Lockout Register (Instance 1 of 4) (HLOR0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HLOR0_RESERVED1_MASK 0xfff00000
#define  HVSYNC_HLOR0_RESERVED1_SHIFT 20
#define  HVSYNC_HLOR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR0_RESERVED1_MASK) >> HVSYNC_HLOR0_RESERVED1_SHIFT)
#define  HVSYNC_HLOR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HLOR0_RESERVED1_SHIFT))

#define  HVSYNC_HLOR0_HSYNCLOCKOUT_MASK 0xfffff
#define  HVSYNC_HLOR0_HSYNCLOCKOUT_SHIFT 0
#define  HVSYNC_HLOR0_HSYNCLOCKOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR0_HSYNCLOCKOUT_MASK) >> HVSYNC_HLOR0_HSYNCLOCKOUT_SHIFT)
#define  HVSYNC_HLOR0_HSYNCLOCKOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR0_HSYNCLOCKOUT_MASK) | (((uint32_t)val) << HVSYNC_HLOR0_HSYNCLOCKOUT_SHIFT))

//====================================================================
//Register: HSYNC Lockout Register (Instance 2 of 4) (HLOR1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HLOR1_RESERVED1_MASK 0xfff00000
#define  HVSYNC_HLOR1_RESERVED1_SHIFT 20
#define  HVSYNC_HLOR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR1_RESERVED1_MASK) >> HVSYNC_HLOR1_RESERVED1_SHIFT)
#define  HVSYNC_HLOR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HLOR1_RESERVED1_SHIFT))

#define  HVSYNC_HLOR1_HSYNCLOCKOUT_MASK 0xfffff
#define  HVSYNC_HLOR1_HSYNCLOCKOUT_SHIFT 0
#define  HVSYNC_HLOR1_HSYNCLOCKOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR1_HSYNCLOCKOUT_MASK) >> HVSYNC_HLOR1_HSYNCLOCKOUT_SHIFT)
#define  HVSYNC_HLOR1_HSYNCLOCKOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR1_HSYNCLOCKOUT_MASK) | (((uint32_t)val) << HVSYNC_HLOR1_HSYNCLOCKOUT_SHIFT))

//====================================================================
//Register: HSYNC Lockout Register (Instance 3 of 4) (HLOR2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HLOR2_RESERVED1_MASK 0xfff00000
#define  HVSYNC_HLOR2_RESERVED1_SHIFT 20
#define  HVSYNC_HLOR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR2_RESERVED1_MASK) >> HVSYNC_HLOR2_RESERVED1_SHIFT)
#define  HVSYNC_HLOR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HLOR2_RESERVED1_SHIFT))

#define  HVSYNC_HLOR2_HSYNCLOCKOUT_MASK 0xfffff
#define  HVSYNC_HLOR2_HSYNCLOCKOUT_SHIFT 0
#define  HVSYNC_HLOR2_HSYNCLOCKOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR2_HSYNCLOCKOUT_MASK) >> HVSYNC_HLOR2_HSYNCLOCKOUT_SHIFT)
#define  HVSYNC_HLOR2_HSYNCLOCKOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR2_HSYNCLOCKOUT_MASK) | (((uint32_t)val) << HVSYNC_HLOR2_HSYNCLOCKOUT_SHIFT))

//====================================================================
//Register: HSYNC Lockout Register (Instance 4 of 4) (HLOR3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HLOR3_RESERVED1_MASK 0xfff00000
#define  HVSYNC_HLOR3_RESERVED1_SHIFT 20
#define  HVSYNC_HLOR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR3_RESERVED1_MASK) >> HVSYNC_HLOR3_RESERVED1_SHIFT)
#define  HVSYNC_HLOR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HLOR3_RESERVED1_SHIFT))

#define  HVSYNC_HLOR3_HSYNCLOCKOUT_MASK 0xfffff
#define  HVSYNC_HLOR3_HSYNCLOCKOUT_SHIFT 0
#define  HVSYNC_HLOR3_HSYNCLOCKOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HLOR3_HSYNCLOCKOUT_MASK) >> HVSYNC_HLOR3_HSYNCLOCKOUT_SHIFT)
#define  HVSYNC_HLOR3_HSYNCLOCKOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HLOR3_HSYNCLOCKOUT_MASK) | (((uint32_t)val) << HVSYNC_HLOR3_HSYNCLOCKOUT_SHIFT))

//====================================================================
//Register: HSYNC Capture Configuration Register (Instance 1 of 4) (HCCR0)
/** \brief This register configures the HSYNC filter capture hardware. This is only used for debug purposes. (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HCCR0_RESERVED1_MASK 0xfffffc00
#define  HVSYNC_HCCR0_RESERVED1_SHIFT 10
#define  HVSYNC_HCCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR0_RESERVED1_MASK) >> HVSYNC_HCCR0_RESERVED1_SHIFT)
#define  HVSYNC_HCCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HCCR0_RESERVED1_SHIFT))

#define  HVSYNC_HCCR0_XLO_MASK 0x200
#define  HVSYNC_HCCR0_XLO_SHIFT 9
#define  HVSYNC_HCCR0_XLO_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR0_XLO_MASK) >> HVSYNC_HCCR0_XLO_SHIFT)
#define  HVSYNC_HCCR0_XLO_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR0_XLO_MASK) | (((uint32_t)val) << HVSYNC_HCCR0_XLO_SHIFT))

#define  HVSYNC_HCCR0_DELAYSAMPLE_MASK 0x1f0
#define  HVSYNC_HCCR0_DELAYSAMPLE_SHIFT 4
#define  HVSYNC_HCCR0_DELAYSAMPLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR0_DELAYSAMPLE_MASK) >> HVSYNC_HCCR0_DELAYSAMPLE_SHIFT)
#define  HVSYNC_HCCR0_DELAYSAMPLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR0_DELAYSAMPLE_MASK) | (((uint32_t)val) << HVSYNC_HCCR0_DELAYSAMPLE_SHIFT))

#define  HVSYNC_HCCR0_CAPTURESEL_MASK 0xf
#define  HVSYNC_HCCR0_CAPTURESEL_SHIFT 0
#define  HVSYNC_HCCR0_CAPTURESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR0_CAPTURESEL_MASK) >> HVSYNC_HCCR0_CAPTURESEL_SHIFT)
#define  HVSYNC_HCCR0_CAPTURESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR0_CAPTURESEL_MASK) | (((uint32_t)val) << HVSYNC_HCCR0_CAPTURESEL_SHIFT))

//====================================================================
//Register: HSYNC Capture Configuration Register (Instance 2 of 4) (HCCR1)
/** \brief This register configures the HSYNC filter capture hardware. This is only used for debug purposes. (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HCCR1_RESERVED1_MASK 0xfffffc00
#define  HVSYNC_HCCR1_RESERVED1_SHIFT 10
#define  HVSYNC_HCCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR1_RESERVED1_MASK) >> HVSYNC_HCCR1_RESERVED1_SHIFT)
#define  HVSYNC_HCCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HCCR1_RESERVED1_SHIFT))

#define  HVSYNC_HCCR1_XLO_MASK 0x200
#define  HVSYNC_HCCR1_XLO_SHIFT 9
#define  HVSYNC_HCCR1_XLO_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR1_XLO_MASK) >> HVSYNC_HCCR1_XLO_SHIFT)
#define  HVSYNC_HCCR1_XLO_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR1_XLO_MASK) | (((uint32_t)val) << HVSYNC_HCCR1_XLO_SHIFT))

#define  HVSYNC_HCCR1_DELAYSAMPLE_MASK 0x1f0
#define  HVSYNC_HCCR1_DELAYSAMPLE_SHIFT 4
#define  HVSYNC_HCCR1_DELAYSAMPLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR1_DELAYSAMPLE_MASK) >> HVSYNC_HCCR1_DELAYSAMPLE_SHIFT)
#define  HVSYNC_HCCR1_DELAYSAMPLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR1_DELAYSAMPLE_MASK) | (((uint32_t)val) << HVSYNC_HCCR1_DELAYSAMPLE_SHIFT))

#define  HVSYNC_HCCR1_CAPTURESEL_MASK 0xf
#define  HVSYNC_HCCR1_CAPTURESEL_SHIFT 0
#define  HVSYNC_HCCR1_CAPTURESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR1_CAPTURESEL_MASK) >> HVSYNC_HCCR1_CAPTURESEL_SHIFT)
#define  HVSYNC_HCCR1_CAPTURESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR1_CAPTURESEL_MASK) | (((uint32_t)val) << HVSYNC_HCCR1_CAPTURESEL_SHIFT))

//====================================================================
//Register: HSYNC Capture Configuration Register (Instance 3 of 4) (HCCR2)
/** \brief This register configures the HSYNC filter capture hardware. This is only used for debug purposes. (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HCCR2_RESERVED1_MASK 0xfffffc00
#define  HVSYNC_HCCR2_RESERVED1_SHIFT 10
#define  HVSYNC_HCCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR2_RESERVED1_MASK) >> HVSYNC_HCCR2_RESERVED1_SHIFT)
#define  HVSYNC_HCCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HCCR2_RESERVED1_SHIFT))

#define  HVSYNC_HCCR2_XLO_MASK 0x200
#define  HVSYNC_HCCR2_XLO_SHIFT 9
#define  HVSYNC_HCCR2_XLO_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR2_XLO_MASK) >> HVSYNC_HCCR2_XLO_SHIFT)
#define  HVSYNC_HCCR2_XLO_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR2_XLO_MASK) | (((uint32_t)val) << HVSYNC_HCCR2_XLO_SHIFT))

#define  HVSYNC_HCCR2_DELAYSAMPLE_MASK 0x1f0
#define  HVSYNC_HCCR2_DELAYSAMPLE_SHIFT 4
#define  HVSYNC_HCCR2_DELAYSAMPLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR2_DELAYSAMPLE_MASK) >> HVSYNC_HCCR2_DELAYSAMPLE_SHIFT)
#define  HVSYNC_HCCR2_DELAYSAMPLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR2_DELAYSAMPLE_MASK) | (((uint32_t)val) << HVSYNC_HCCR2_DELAYSAMPLE_SHIFT))

#define  HVSYNC_HCCR2_CAPTURESEL_MASK 0xf
#define  HVSYNC_HCCR2_CAPTURESEL_SHIFT 0
#define  HVSYNC_HCCR2_CAPTURESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR2_CAPTURESEL_MASK) >> HVSYNC_HCCR2_CAPTURESEL_SHIFT)
#define  HVSYNC_HCCR2_CAPTURESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR2_CAPTURESEL_MASK) | (((uint32_t)val) << HVSYNC_HCCR2_CAPTURESEL_SHIFT))

//====================================================================
//Register: HSYNC Capture Configuration Register (Instance 4 of 4) (HCCR3)
/** \brief This register configures the HSYNC filter capture hardware. This is only used for debug purposes. (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HCCR3_RESERVED1_MASK 0xfffffc00
#define  HVSYNC_HCCR3_RESERVED1_SHIFT 10
#define  HVSYNC_HCCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR3_RESERVED1_MASK) >> HVSYNC_HCCR3_RESERVED1_SHIFT)
#define  HVSYNC_HCCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HCCR3_RESERVED1_SHIFT))

#define  HVSYNC_HCCR3_XLO_MASK 0x200
#define  HVSYNC_HCCR3_XLO_SHIFT 9
#define  HVSYNC_HCCR3_XLO_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR3_XLO_MASK) >> HVSYNC_HCCR3_XLO_SHIFT)
#define  HVSYNC_HCCR3_XLO_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR3_XLO_MASK) | (((uint32_t)val) << HVSYNC_HCCR3_XLO_SHIFT))

#define  HVSYNC_HCCR3_DELAYSAMPLE_MASK 0x1f0
#define  HVSYNC_HCCR3_DELAYSAMPLE_SHIFT 4
#define  HVSYNC_HCCR3_DELAYSAMPLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR3_DELAYSAMPLE_MASK) >> HVSYNC_HCCR3_DELAYSAMPLE_SHIFT)
#define  HVSYNC_HCCR3_DELAYSAMPLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR3_DELAYSAMPLE_MASK) | (((uint32_t)val) << HVSYNC_HCCR3_DELAYSAMPLE_SHIFT))

#define  HVSYNC_HCCR3_CAPTURESEL_MASK 0xf
#define  HVSYNC_HCCR3_CAPTURESEL_SHIFT 0
#define  HVSYNC_HCCR3_CAPTURESEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HCCR3_CAPTURESEL_MASK) >> HVSYNC_HCCR3_CAPTURESEL_SHIFT)
#define  HVSYNC_HCCR3_CAPTURESEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HCCR3_CAPTURESEL_MASK) | (((uint32_t)val) << HVSYNC_HCCR3_CAPTURESEL_SHIFT))

//====================================================================
//Register: HSYNC Edge Capture Register (Instance 1 of 4) (HECR0)
/** \brief Contains the two 16-bit values of the HiPS sampled HSYNC used in detecting the HSYNC edge (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HECR0_SAMPLE1_MASK 0xffff0000
#define  HVSYNC_HECR0_SAMPLE1_SHIFT 16
#define  HVSYNC_HECR0_SAMPLE1_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR0_SAMPLE1_MASK) >> HVSYNC_HECR0_SAMPLE1_SHIFT)
#define  HVSYNC_HECR0_SAMPLE1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR0_SAMPLE1_MASK) | (((uint32_t)val) << HVSYNC_HECR0_SAMPLE1_SHIFT))

#define  HVSYNC_HECR0_SAMPLE0_MASK 0xffff
#define  HVSYNC_HECR0_SAMPLE0_SHIFT 0
#define  HVSYNC_HECR0_SAMPLE0_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR0_SAMPLE0_MASK) >> HVSYNC_HECR0_SAMPLE0_SHIFT)
#define  HVSYNC_HECR0_SAMPLE0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR0_SAMPLE0_MASK) | (((uint32_t)val) << HVSYNC_HECR0_SAMPLE0_SHIFT))

//====================================================================
//Register: HSYNC Edge Capture Register (Instance 2 of 4) (HECR1)
/** \brief Contains the two 16-bit values of the HiPS sampled HSYNC used in detecting the HSYNC edge (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HECR1_SAMPLE1_MASK 0xffff0000
#define  HVSYNC_HECR1_SAMPLE1_SHIFT 16
#define  HVSYNC_HECR1_SAMPLE1_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR1_SAMPLE1_MASK) >> HVSYNC_HECR1_SAMPLE1_SHIFT)
#define  HVSYNC_HECR1_SAMPLE1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR1_SAMPLE1_MASK) | (((uint32_t)val) << HVSYNC_HECR1_SAMPLE1_SHIFT))

#define  HVSYNC_HECR1_SAMPLE0_MASK 0xffff
#define  HVSYNC_HECR1_SAMPLE0_SHIFT 0
#define  HVSYNC_HECR1_SAMPLE0_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR1_SAMPLE0_MASK) >> HVSYNC_HECR1_SAMPLE0_SHIFT)
#define  HVSYNC_HECR1_SAMPLE0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR1_SAMPLE0_MASK) | (((uint32_t)val) << HVSYNC_HECR1_SAMPLE0_SHIFT))

//====================================================================
//Register: HSYNC Edge Capture Register (Instance 3 of 4) (HECR2)
/** \brief Contains the two 16-bit values of the HiPS sampled HSYNC used in detecting the HSYNC edge (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HECR2_SAMPLE1_MASK 0xffff0000
#define  HVSYNC_HECR2_SAMPLE1_SHIFT 16
#define  HVSYNC_HECR2_SAMPLE1_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR2_SAMPLE1_MASK) >> HVSYNC_HECR2_SAMPLE1_SHIFT)
#define  HVSYNC_HECR2_SAMPLE1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR2_SAMPLE1_MASK) | (((uint32_t)val) << HVSYNC_HECR2_SAMPLE1_SHIFT))

#define  HVSYNC_HECR2_SAMPLE0_MASK 0xffff
#define  HVSYNC_HECR2_SAMPLE0_SHIFT 0
#define  HVSYNC_HECR2_SAMPLE0_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR2_SAMPLE0_MASK) >> HVSYNC_HECR2_SAMPLE0_SHIFT)
#define  HVSYNC_HECR2_SAMPLE0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR2_SAMPLE0_MASK) | (((uint32_t)val) << HVSYNC_HECR2_SAMPLE0_SHIFT))

//====================================================================
//Register: HSYNC Edge Capture Register (Instance 4 of 4) (HECR3)
/** \brief Contains the two 16-bit values of the HiPS sampled HSYNC used in detecting the HSYNC edge (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HECR3_SAMPLE1_MASK 0xffff0000
#define  HVSYNC_HECR3_SAMPLE1_SHIFT 16
#define  HVSYNC_HECR3_SAMPLE1_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR3_SAMPLE1_MASK) >> HVSYNC_HECR3_SAMPLE1_SHIFT)
#define  HVSYNC_HECR3_SAMPLE1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR3_SAMPLE1_MASK) | (((uint32_t)val) << HVSYNC_HECR3_SAMPLE1_SHIFT))

#define  HVSYNC_HECR3_SAMPLE0_MASK 0xffff
#define  HVSYNC_HECR3_SAMPLE0_SHIFT 0
#define  HVSYNC_HECR3_SAMPLE0_MASK_SHIFT(reg) (((reg) & HVSYNC_HECR3_SAMPLE0_MASK) >> HVSYNC_HECR3_SAMPLE0_SHIFT)
#define  HVSYNC_HECR3_SAMPLE0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HECR3_SAMPLE0_MASK) | (((uint32_t)val) << HVSYNC_HECR3_SAMPLE0_SHIFT))

//====================================================================
//Register: HSYNC Offset Capture Register (Instance 1 of 4) (HOCR0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HOCR0_RESERVED1_MASK 0xffc00000
#define  HVSYNC_HOCR0_RESERVED1_SHIFT 22
#define  HVSYNC_HOCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR0_RESERVED1_MASK) >> HVSYNC_HOCR0_RESERVED1_SHIFT)
#define  HVSYNC_HOCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HOCR0_RESERVED1_SHIFT))

#define  HVSYNC_HOCR0_LOCKOUTEDGE_MASK 0x200000
#define  HVSYNC_HOCR0_LOCKOUTEDGE_SHIFT 21
#define  HVSYNC_HOCR0_LOCKOUTEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR0_LOCKOUTEDGE_MASK) >> HVSYNC_HOCR0_LOCKOUTEDGE_SHIFT)
#define  HVSYNC_HOCR0_LOCKOUTEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR0_LOCKOUTEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR0_LOCKOUTEDGE_SHIFT))

#define  HVSYNC_HOCR0_FALSEEDGE_MASK 0x100000
#define  HVSYNC_HOCR0_FALSEEDGE_SHIFT 20
#define  HVSYNC_HOCR0_FALSEEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR0_FALSEEDGE_MASK) >> HVSYNC_HOCR0_FALSEEDGE_SHIFT)
#define  HVSYNC_HOCR0_FALSEEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR0_FALSEEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR0_FALSEEDGE_SHIFT))

#define  HVSYNC_HOCR0_OFFSETCAP_MASK 0xf0000
#define  HVSYNC_HOCR0_OFFSETCAP_SHIFT 16
#define  HVSYNC_HOCR0_OFFSETCAP_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR0_OFFSETCAP_MASK) >> HVSYNC_HOCR0_OFFSETCAP_SHIFT)
#define  HVSYNC_HOCR0_OFFSETCAP_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR0_OFFSETCAP_MASK) | (((uint32_t)val) << HVSYNC_HOCR0_OFFSETCAP_SHIFT))

#define  HVSYNC_HOCR0_SAMPLE2_MASK 0xffff
#define  HVSYNC_HOCR0_SAMPLE2_SHIFT 0
#define  HVSYNC_HOCR0_SAMPLE2_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR0_SAMPLE2_MASK) >> HVSYNC_HOCR0_SAMPLE2_SHIFT)
#define  HVSYNC_HOCR0_SAMPLE2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR0_SAMPLE2_MASK) | (((uint32_t)val) << HVSYNC_HOCR0_SAMPLE2_SHIFT))

//====================================================================
//Register: HSYNC Offset Capture Register (Instance 2 of 4) (HOCR1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HOCR1_RESERVED1_MASK 0xffc00000
#define  HVSYNC_HOCR1_RESERVED1_SHIFT 22
#define  HVSYNC_HOCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR1_RESERVED1_MASK) >> HVSYNC_HOCR1_RESERVED1_SHIFT)
#define  HVSYNC_HOCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HOCR1_RESERVED1_SHIFT))

#define  HVSYNC_HOCR1_LOCKOUTEDGE_MASK 0x200000
#define  HVSYNC_HOCR1_LOCKOUTEDGE_SHIFT 21
#define  HVSYNC_HOCR1_LOCKOUTEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR1_LOCKOUTEDGE_MASK) >> HVSYNC_HOCR1_LOCKOUTEDGE_SHIFT)
#define  HVSYNC_HOCR1_LOCKOUTEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR1_LOCKOUTEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR1_LOCKOUTEDGE_SHIFT))

#define  HVSYNC_HOCR1_FALSEEDGE_MASK 0x100000
#define  HVSYNC_HOCR1_FALSEEDGE_SHIFT 20
#define  HVSYNC_HOCR1_FALSEEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR1_FALSEEDGE_MASK) >> HVSYNC_HOCR1_FALSEEDGE_SHIFT)
#define  HVSYNC_HOCR1_FALSEEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR1_FALSEEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR1_FALSEEDGE_SHIFT))

#define  HVSYNC_HOCR1_OFFSETCAP_MASK 0xf0000
#define  HVSYNC_HOCR1_OFFSETCAP_SHIFT 16
#define  HVSYNC_HOCR1_OFFSETCAP_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR1_OFFSETCAP_MASK) >> HVSYNC_HOCR1_OFFSETCAP_SHIFT)
#define  HVSYNC_HOCR1_OFFSETCAP_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR1_OFFSETCAP_MASK) | (((uint32_t)val) << HVSYNC_HOCR1_OFFSETCAP_SHIFT))

#define  HVSYNC_HOCR1_SAMPLE2_MASK 0xffff
#define  HVSYNC_HOCR1_SAMPLE2_SHIFT 0
#define  HVSYNC_HOCR1_SAMPLE2_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR1_SAMPLE2_MASK) >> HVSYNC_HOCR1_SAMPLE2_SHIFT)
#define  HVSYNC_HOCR1_SAMPLE2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR1_SAMPLE2_MASK) | (((uint32_t)val) << HVSYNC_HOCR1_SAMPLE2_SHIFT))

//====================================================================
//Register: HSYNC Offset Capture Register (Instance 3 of 4) (HOCR2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HOCR2_RESERVED1_MASK 0xffc00000
#define  HVSYNC_HOCR2_RESERVED1_SHIFT 22
#define  HVSYNC_HOCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR2_RESERVED1_MASK) >> HVSYNC_HOCR2_RESERVED1_SHIFT)
#define  HVSYNC_HOCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HOCR2_RESERVED1_SHIFT))

#define  HVSYNC_HOCR2_LOCKOUTEDGE_MASK 0x200000
#define  HVSYNC_HOCR2_LOCKOUTEDGE_SHIFT 21
#define  HVSYNC_HOCR2_LOCKOUTEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR2_LOCKOUTEDGE_MASK) >> HVSYNC_HOCR2_LOCKOUTEDGE_SHIFT)
#define  HVSYNC_HOCR2_LOCKOUTEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR2_LOCKOUTEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR2_LOCKOUTEDGE_SHIFT))

#define  HVSYNC_HOCR2_FALSEEDGE_MASK 0x100000
#define  HVSYNC_HOCR2_FALSEEDGE_SHIFT 20
#define  HVSYNC_HOCR2_FALSEEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR2_FALSEEDGE_MASK) >> HVSYNC_HOCR2_FALSEEDGE_SHIFT)
#define  HVSYNC_HOCR2_FALSEEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR2_FALSEEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR2_FALSEEDGE_SHIFT))

#define  HVSYNC_HOCR2_OFFSETCAP_MASK 0xf0000
#define  HVSYNC_HOCR2_OFFSETCAP_SHIFT 16
#define  HVSYNC_HOCR2_OFFSETCAP_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR2_OFFSETCAP_MASK) >> HVSYNC_HOCR2_OFFSETCAP_SHIFT)
#define  HVSYNC_HOCR2_OFFSETCAP_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR2_OFFSETCAP_MASK) | (((uint32_t)val) << HVSYNC_HOCR2_OFFSETCAP_SHIFT))

#define  HVSYNC_HOCR2_SAMPLE2_MASK 0xffff
#define  HVSYNC_HOCR2_SAMPLE2_SHIFT 0
#define  HVSYNC_HOCR2_SAMPLE2_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR2_SAMPLE2_MASK) >> HVSYNC_HOCR2_SAMPLE2_SHIFT)
#define  HVSYNC_HOCR2_SAMPLE2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR2_SAMPLE2_MASK) | (((uint32_t)val) << HVSYNC_HOCR2_SAMPLE2_SHIFT))

//====================================================================
//Register: HSYNC Offset Capture Register (Instance 4 of 4) (HOCR3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HOCR3_RESERVED1_MASK 0xffc00000
#define  HVSYNC_HOCR3_RESERVED1_SHIFT 22
#define  HVSYNC_HOCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR3_RESERVED1_MASK) >> HVSYNC_HOCR3_RESERVED1_SHIFT)
#define  HVSYNC_HOCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HOCR3_RESERVED1_SHIFT))

#define  HVSYNC_HOCR3_LOCKOUTEDGE_MASK 0x200000
#define  HVSYNC_HOCR3_LOCKOUTEDGE_SHIFT 21
#define  HVSYNC_HOCR3_LOCKOUTEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR3_LOCKOUTEDGE_MASK) >> HVSYNC_HOCR3_LOCKOUTEDGE_SHIFT)
#define  HVSYNC_HOCR3_LOCKOUTEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR3_LOCKOUTEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR3_LOCKOUTEDGE_SHIFT))

#define  HVSYNC_HOCR3_FALSEEDGE_MASK 0x100000
#define  HVSYNC_HOCR3_FALSEEDGE_SHIFT 20
#define  HVSYNC_HOCR3_FALSEEDGE_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR3_FALSEEDGE_MASK) >> HVSYNC_HOCR3_FALSEEDGE_SHIFT)
#define  HVSYNC_HOCR3_FALSEEDGE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR3_FALSEEDGE_MASK) | (((uint32_t)val) << HVSYNC_HOCR3_FALSEEDGE_SHIFT))

#define  HVSYNC_HOCR3_OFFSETCAP_MASK 0xf0000
#define  HVSYNC_HOCR3_OFFSETCAP_SHIFT 16
#define  HVSYNC_HOCR3_OFFSETCAP_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR3_OFFSETCAP_MASK) >> HVSYNC_HOCR3_OFFSETCAP_SHIFT)
#define  HVSYNC_HOCR3_OFFSETCAP_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR3_OFFSETCAP_MASK) | (((uint32_t)val) << HVSYNC_HOCR3_OFFSETCAP_SHIFT))

#define  HVSYNC_HOCR3_SAMPLE2_MASK 0xffff
#define  HVSYNC_HOCR3_SAMPLE2_SHIFT 0
#define  HVSYNC_HOCR3_SAMPLE2_MASK_SHIFT(reg) (((reg) & HVSYNC_HOCR3_SAMPLE2_MASK) >> HVSYNC_HOCR3_SAMPLE2_SHIFT)
#define  HVSYNC_HOCR3_SAMPLE2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HOCR3_SAMPLE2_MASK) | (((uint32_t)val) << HVSYNC_HOCR3_SAMPLE2_SHIFT))

//====================================================================
//Register: HSYNC Filter Status Register (Instance 1 of 4) (HFSR0)
/** \brief  (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HFSR0_RESERVED1_MASK 0xfffff800
#define  HVSYNC_HFSR0_RESERVED1_SHIFT 11
#define  HVSYNC_HFSR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR0_RESERVED1_MASK) >> HVSYNC_HFSR0_RESERVED1_SHIFT)
#define  HVSYNC_HFSR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFSR0_RESERVED1_SHIFT))

#define  HVSYNC_HFSR0_HSYNCV_STATE_MASK 0x700
#define  HVSYNC_HFSR0_HSYNCV_STATE_SHIFT 8
#define  HVSYNC_HFSR0_HSYNCV_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR0_HSYNCV_STATE_MASK) >> HVSYNC_HFSR0_HSYNCV_STATE_SHIFT)
#define  HVSYNC_HFSR0_HSYNCV_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR0_HSYNCV_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR0_HSYNCV_STATE_SHIFT))

#define  HVSYNC_HFSR0_RESERVED2_MASK 0x80
#define  HVSYNC_HFSR0_RESERVED2_SHIFT 7
#define  HVSYNC_HFSR0_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR0_RESERVED2_MASK) >> HVSYNC_HFSR0_RESERVED2_SHIFT)
#define  HVSYNC_HFSR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR0_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFSR0_RESERVED2_SHIFT))

#define  HVSYNC_HFSR0_HSYNCS_STATE_MASK 0x70
#define  HVSYNC_HFSR0_HSYNCS_STATE_SHIFT 4
#define  HVSYNC_HFSR0_HSYNCS_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR0_HSYNCS_STATE_MASK) >> HVSYNC_HFSR0_HSYNCS_STATE_SHIFT)
#define  HVSYNC_HFSR0_HSYNCS_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR0_HSYNCS_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR0_HSYNCS_STATE_SHIFT))

#define  HVSYNC_HFSR0_RESERVED3_MASK 0xc
#define  HVSYNC_HFSR0_RESERVED3_SHIFT 2
#define  HVSYNC_HFSR0_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR0_RESERVED3_MASK) >> HVSYNC_HFSR0_RESERVED3_SHIFT)
#define  HVSYNC_HFSR0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR0_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFSR0_RESERVED3_SHIFT))

#define  HVSYNC_HFSR0_HSYNC_STATE_MASK 0x3
#define  HVSYNC_HFSR0_HSYNC_STATE_SHIFT 0
#define  HVSYNC_HFSR0_HSYNC_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR0_HSYNC_STATE_MASK) >> HVSYNC_HFSR0_HSYNC_STATE_SHIFT)
#define  HVSYNC_HFSR0_HSYNC_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR0_HSYNC_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR0_HSYNC_STATE_SHIFT))

//====================================================================
//Register: HSYNC Filter Status Register (Instance 2 of 4) (HFSR1)
/** \brief  (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HFSR1_RESERVED1_MASK 0xfffff800
#define  HVSYNC_HFSR1_RESERVED1_SHIFT 11
#define  HVSYNC_HFSR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR1_RESERVED1_MASK) >> HVSYNC_HFSR1_RESERVED1_SHIFT)
#define  HVSYNC_HFSR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFSR1_RESERVED1_SHIFT))

#define  HVSYNC_HFSR1_HSYNCV_STATE_MASK 0x700
#define  HVSYNC_HFSR1_HSYNCV_STATE_SHIFT 8
#define  HVSYNC_HFSR1_HSYNCV_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR1_HSYNCV_STATE_MASK) >> HVSYNC_HFSR1_HSYNCV_STATE_SHIFT)
#define  HVSYNC_HFSR1_HSYNCV_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR1_HSYNCV_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR1_HSYNCV_STATE_SHIFT))

#define  HVSYNC_HFSR1_RESERVED2_MASK 0x80
#define  HVSYNC_HFSR1_RESERVED2_SHIFT 7
#define  HVSYNC_HFSR1_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR1_RESERVED2_MASK) >> HVSYNC_HFSR1_RESERVED2_SHIFT)
#define  HVSYNC_HFSR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR1_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFSR1_RESERVED2_SHIFT))

#define  HVSYNC_HFSR1_HSYNCS_STATE_MASK 0x70
#define  HVSYNC_HFSR1_HSYNCS_STATE_SHIFT 4
#define  HVSYNC_HFSR1_HSYNCS_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR1_HSYNCS_STATE_MASK) >> HVSYNC_HFSR1_HSYNCS_STATE_SHIFT)
#define  HVSYNC_HFSR1_HSYNCS_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR1_HSYNCS_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR1_HSYNCS_STATE_SHIFT))

#define  HVSYNC_HFSR1_RESERVED3_MASK 0xc
#define  HVSYNC_HFSR1_RESERVED3_SHIFT 2
#define  HVSYNC_HFSR1_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR1_RESERVED3_MASK) >> HVSYNC_HFSR1_RESERVED3_SHIFT)
#define  HVSYNC_HFSR1_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR1_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFSR1_RESERVED3_SHIFT))

#define  HVSYNC_HFSR1_HSYNC_STATE_MASK 0x3
#define  HVSYNC_HFSR1_HSYNC_STATE_SHIFT 0
#define  HVSYNC_HFSR1_HSYNC_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR1_HSYNC_STATE_MASK) >> HVSYNC_HFSR1_HSYNC_STATE_SHIFT)
#define  HVSYNC_HFSR1_HSYNC_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR1_HSYNC_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR1_HSYNC_STATE_SHIFT))

//====================================================================
//Register: HSYNC Filter Status Register (Instance 3 of 4) (HFSR2)
/** \brief  (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HFSR2_RESERVED1_MASK 0xfffff800
#define  HVSYNC_HFSR2_RESERVED1_SHIFT 11
#define  HVSYNC_HFSR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR2_RESERVED1_MASK) >> HVSYNC_HFSR2_RESERVED1_SHIFT)
#define  HVSYNC_HFSR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFSR2_RESERVED1_SHIFT))

#define  HVSYNC_HFSR2_HSYNCV_STATE_MASK 0x700
#define  HVSYNC_HFSR2_HSYNCV_STATE_SHIFT 8
#define  HVSYNC_HFSR2_HSYNCV_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR2_HSYNCV_STATE_MASK) >> HVSYNC_HFSR2_HSYNCV_STATE_SHIFT)
#define  HVSYNC_HFSR2_HSYNCV_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR2_HSYNCV_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR2_HSYNCV_STATE_SHIFT))

#define  HVSYNC_HFSR2_RESERVED2_MASK 0x80
#define  HVSYNC_HFSR2_RESERVED2_SHIFT 7
#define  HVSYNC_HFSR2_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR2_RESERVED2_MASK) >> HVSYNC_HFSR2_RESERVED2_SHIFT)
#define  HVSYNC_HFSR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR2_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFSR2_RESERVED2_SHIFT))

#define  HVSYNC_HFSR2_HSYNCS_STATE_MASK 0x70
#define  HVSYNC_HFSR2_HSYNCS_STATE_SHIFT 4
#define  HVSYNC_HFSR2_HSYNCS_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR2_HSYNCS_STATE_MASK) >> HVSYNC_HFSR2_HSYNCS_STATE_SHIFT)
#define  HVSYNC_HFSR2_HSYNCS_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR2_HSYNCS_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR2_HSYNCS_STATE_SHIFT))

#define  HVSYNC_HFSR2_RESERVED3_MASK 0xc
#define  HVSYNC_HFSR2_RESERVED3_SHIFT 2
#define  HVSYNC_HFSR2_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR2_RESERVED3_MASK) >> HVSYNC_HFSR2_RESERVED3_SHIFT)
#define  HVSYNC_HFSR2_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR2_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFSR2_RESERVED3_SHIFT))

#define  HVSYNC_HFSR2_HSYNC_STATE_MASK 0x3
#define  HVSYNC_HFSR2_HSYNC_STATE_SHIFT 0
#define  HVSYNC_HFSR2_HSYNC_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR2_HSYNC_STATE_MASK) >> HVSYNC_HFSR2_HSYNC_STATE_SHIFT)
#define  HVSYNC_HFSR2_HSYNC_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR2_HSYNC_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR2_HSYNC_STATE_SHIFT))

//====================================================================
//Register: HSYNC Filter Status Register (Instance 4 of 4) (HFSR3)
/** \brief  (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HFSR3_RESERVED1_MASK 0xfffff800
#define  HVSYNC_HFSR3_RESERVED1_SHIFT 11
#define  HVSYNC_HFSR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR3_RESERVED1_MASK) >> HVSYNC_HFSR3_RESERVED1_SHIFT)
#define  HVSYNC_HFSR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HFSR3_RESERVED1_SHIFT))

#define  HVSYNC_HFSR3_HSYNCV_STATE_MASK 0x700
#define  HVSYNC_HFSR3_HSYNCV_STATE_SHIFT 8
#define  HVSYNC_HFSR3_HSYNCV_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR3_HSYNCV_STATE_MASK) >> HVSYNC_HFSR3_HSYNCV_STATE_SHIFT)
#define  HVSYNC_HFSR3_HSYNCV_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR3_HSYNCV_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR3_HSYNCV_STATE_SHIFT))

#define  HVSYNC_HFSR3_RESERVED2_MASK 0x80
#define  HVSYNC_HFSR3_RESERVED2_SHIFT 7
#define  HVSYNC_HFSR3_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR3_RESERVED2_MASK) >> HVSYNC_HFSR3_RESERVED2_SHIFT)
#define  HVSYNC_HFSR3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR3_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HFSR3_RESERVED2_SHIFT))

#define  HVSYNC_HFSR3_HSYNCS_STATE_MASK 0x70
#define  HVSYNC_HFSR3_HSYNCS_STATE_SHIFT 4
#define  HVSYNC_HFSR3_HSYNCS_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR3_HSYNCS_STATE_MASK) >> HVSYNC_HFSR3_HSYNCS_STATE_SHIFT)
#define  HVSYNC_HFSR3_HSYNCS_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR3_HSYNCS_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR3_HSYNCS_STATE_SHIFT))

#define  HVSYNC_HFSR3_RESERVED3_MASK 0xc
#define  HVSYNC_HFSR3_RESERVED3_SHIFT 2
#define  HVSYNC_HFSR3_RESERVED3_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR3_RESERVED3_MASK) >> HVSYNC_HFSR3_RESERVED3_SHIFT)
#define  HVSYNC_HFSR3_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR3_RESERVED3_MASK) | (((uint32_t)val) << HVSYNC_HFSR3_RESERVED3_SHIFT))

#define  HVSYNC_HFSR3_HSYNC_STATE_MASK 0x3
#define  HVSYNC_HFSR3_HSYNC_STATE_SHIFT 0
#define  HVSYNC_HFSR3_HSYNC_STATE_MASK_SHIFT(reg) (((reg) & HVSYNC_HFSR3_HSYNC_STATE_MASK) >> HVSYNC_HFSR3_HSYNC_STATE_SHIFT)
#define  HVSYNC_HFSR3_HSYNC_STATE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HFSR3_HSYNC_STATE_MASK) | (((uint32_t)val) << HVSYNC_HFSR3_HSYNC_STATE_SHIFT))

//====================================================================
//Register: HSYNC SOS To EOS Time (Instance 1 of 4) (HSET0)
/** \brief This register sets the count value used by the HSYNC filter to differentiate
        between a Start of Scan (SOS) and End of Scan (EOS) pulse.
        <br>
        If the time from EOS to SOS is less than the time from SOS to EOS, the EOStoSOS field
        should be set to a time that is greater than EOS to SOS but less than SOS to EOS. The
        EOSGr bit should be set to 0.
        <br>
        If the time from SOS to EOS is less than the time from EOS to SOS, the EOStoSOS field
        should be set to a time that is greater than SOS to EOS but less than EOS to SOS. The
        EOSGr bit should be set to 1.
        <br>
        The value is in 156.25MHz clock periods. If the HSYNC signal does not contain
        an EOS this register must be set to zero. (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HSET0_RESERVED1_MASK 0xfff80000
#define  HVSYNC_HSET0_RESERVED1_SHIFT 19
#define  HVSYNC_HSET0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET0_RESERVED1_MASK) >> HVSYNC_HSET0_RESERVED1_SHIFT)
#define  HVSYNC_HSET0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HSET0_RESERVED1_SHIFT))

#define  HVSYNC_HSET0_EOSGR_MASK 0x40000
#define  HVSYNC_HSET0_EOSGR_SHIFT 18
#define  HVSYNC_HSET0_EOSGR_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET0_EOSGR_MASK) >> HVSYNC_HSET0_EOSGR_SHIFT)
#define  HVSYNC_HSET0_EOSGR_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET0_EOSGR_MASK) | (((uint32_t)val) << HVSYNC_HSET0_EOSGR_SHIFT))

#define  HVSYNC_HSET0_EOSTOSOS_MASK 0x3ffff
#define  HVSYNC_HSET0_EOSTOSOS_SHIFT 0
#define  HVSYNC_HSET0_EOSTOSOS_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET0_EOSTOSOS_MASK) >> HVSYNC_HSET0_EOSTOSOS_SHIFT)
#define  HVSYNC_HSET0_EOSTOSOS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET0_EOSTOSOS_MASK) | (((uint32_t)val) << HVSYNC_HSET0_EOSTOSOS_SHIFT))

//====================================================================
//Register: HSYNC SOS To EOS Time (Instance 2 of 4) (HSET1)
/** \brief This register sets the count value used by the HSYNC filter to differentiate
        between a Start of Scan (SOS) and End of Scan (EOS) pulse.
        <br>
        If the time from EOS to SOS is less than the time from SOS to EOS, the EOStoSOS field
        should be set to a time that is greater than EOS to SOS but less than SOS to EOS. The
        EOSGr bit should be set to 0.
        <br>
        If the time from SOS to EOS is less than the time from EOS to SOS, the EOStoSOS field
        should be set to a time that is greater than SOS to EOS but less than EOS to SOS. The
        EOSGr bit should be set to 1.
        <br>
        The value is in 156.25MHz clock periods. If the HSYNC signal does not contain
        an EOS this register must be set to zero. (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HSET1_RESERVED1_MASK 0xfff80000
#define  HVSYNC_HSET1_RESERVED1_SHIFT 19
#define  HVSYNC_HSET1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET1_RESERVED1_MASK) >> HVSYNC_HSET1_RESERVED1_SHIFT)
#define  HVSYNC_HSET1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HSET1_RESERVED1_SHIFT))

#define  HVSYNC_HSET1_EOSGR_MASK 0x40000
#define  HVSYNC_HSET1_EOSGR_SHIFT 18
#define  HVSYNC_HSET1_EOSGR_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET1_EOSGR_MASK) >> HVSYNC_HSET1_EOSGR_SHIFT)
#define  HVSYNC_HSET1_EOSGR_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET1_EOSGR_MASK) | (((uint32_t)val) << HVSYNC_HSET1_EOSGR_SHIFT))

#define  HVSYNC_HSET1_EOSTOSOS_MASK 0x3ffff
#define  HVSYNC_HSET1_EOSTOSOS_SHIFT 0
#define  HVSYNC_HSET1_EOSTOSOS_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET1_EOSTOSOS_MASK) >> HVSYNC_HSET1_EOSTOSOS_SHIFT)
#define  HVSYNC_HSET1_EOSTOSOS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET1_EOSTOSOS_MASK) | (((uint32_t)val) << HVSYNC_HSET1_EOSTOSOS_SHIFT))

//====================================================================
//Register: HSYNC SOS To EOS Time (Instance 3 of 4) (HSET2)
/** \brief This register sets the count value used by the HSYNC filter to differentiate
        between a Start of Scan (SOS) and End of Scan (EOS) pulse.
        <br>
        If the time from EOS to SOS is less than the time from SOS to EOS, the EOStoSOS field
        should be set to a time that is greater than EOS to SOS but less than SOS to EOS. The
        EOSGr bit should be set to 0.
        <br>
        If the time from SOS to EOS is less than the time from EOS to SOS, the EOStoSOS field
        should be set to a time that is greater than SOS to EOS but less than EOS to SOS. The
        EOSGr bit should be set to 1.
        <br>
        The value is in 156.25MHz clock periods. If the HSYNC signal does not contain
        an EOS this register must be set to zero. (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HSET2_RESERVED1_MASK 0xfff80000
#define  HVSYNC_HSET2_RESERVED1_SHIFT 19
#define  HVSYNC_HSET2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET2_RESERVED1_MASK) >> HVSYNC_HSET2_RESERVED1_SHIFT)
#define  HVSYNC_HSET2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HSET2_RESERVED1_SHIFT))

#define  HVSYNC_HSET2_EOSGR_MASK 0x40000
#define  HVSYNC_HSET2_EOSGR_SHIFT 18
#define  HVSYNC_HSET2_EOSGR_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET2_EOSGR_MASK) >> HVSYNC_HSET2_EOSGR_SHIFT)
#define  HVSYNC_HSET2_EOSGR_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET2_EOSGR_MASK) | (((uint32_t)val) << HVSYNC_HSET2_EOSGR_SHIFT))

#define  HVSYNC_HSET2_EOSTOSOS_MASK 0x3ffff
#define  HVSYNC_HSET2_EOSTOSOS_SHIFT 0
#define  HVSYNC_HSET2_EOSTOSOS_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET2_EOSTOSOS_MASK) >> HVSYNC_HSET2_EOSTOSOS_SHIFT)
#define  HVSYNC_HSET2_EOSTOSOS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET2_EOSTOSOS_MASK) | (((uint32_t)val) << HVSYNC_HSET2_EOSTOSOS_SHIFT))

//====================================================================
//Register: HSYNC SOS To EOS Time (Instance 4 of 4) (HSET3)
/** \brief This register sets the count value used by the HSYNC filter to differentiate
        between a Start of Scan (SOS) and End of Scan (EOS) pulse.
        <br>
        If the time from EOS to SOS is less than the time from SOS to EOS, the EOStoSOS field
        should be set to a time that is greater than EOS to SOS but less than SOS to EOS. The
        EOSGr bit should be set to 0.
        <br>
        If the time from SOS to EOS is less than the time from EOS to SOS, the EOStoSOS field
        should be set to a time that is greater than SOS to EOS but less than EOS to SOS. The
        EOSGr bit should be set to 1.
        <br>
        The value is in 156.25MHz clock periods. If the HSYNC signal does not contain
        an EOS this register must be set to zero. (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HSET3_RESERVED1_MASK 0xfff80000
#define  HVSYNC_HSET3_RESERVED1_SHIFT 19
#define  HVSYNC_HSET3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET3_RESERVED1_MASK) >> HVSYNC_HSET3_RESERVED1_SHIFT)
#define  HVSYNC_HSET3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HSET3_RESERVED1_SHIFT))

#define  HVSYNC_HSET3_EOSGR_MASK 0x40000
#define  HVSYNC_HSET3_EOSGR_SHIFT 18
#define  HVSYNC_HSET3_EOSGR_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET3_EOSGR_MASK) >> HVSYNC_HSET3_EOSGR_SHIFT)
#define  HVSYNC_HSET3_EOSGR_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET3_EOSGR_MASK) | (((uint32_t)val) << HVSYNC_HSET3_EOSGR_SHIFT))

#define  HVSYNC_HSET3_EOSTOSOS_MASK 0x3ffff
#define  HVSYNC_HSET3_EOSTOSOS_SHIFT 0
#define  HVSYNC_HSET3_EOSTOSOS_MASK_SHIFT(reg) (((reg) & HVSYNC_HSET3_EOSTOSOS_MASK) >> HVSYNC_HSET3_EOSTOSOS_SHIFT)
#define  HVSYNC_HSET3_EOSTOSOS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HSET3_EOSTOSOS_MASK) | (((uint32_t)val) << HVSYNC_HSET3_EOSTOSOS_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 1 of 8) (MASK_CFG0)
/** \brief Configures the HSYNC Masking block (Instance 1 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG0_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG0_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG0_RESERVED1_MASK) >> HVSYNC_MASK_CFG0_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG0_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG0_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG0_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG0_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG0_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG0_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG0_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG0_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG0_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG0_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG0_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG0_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG0_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG0_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG0_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG0_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG0_MASKINGENABLE_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 2 of 8) (MASK_CFG1)
/** \brief Configures the HSYNC Masking block (Instance 2 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG1_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG1_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG1_RESERVED1_MASK) >> HVSYNC_MASK_CFG1_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG1_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG1_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG1_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG1_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG1_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG1_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG1_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG1_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG1_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG1_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG1_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG1_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG1_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG1_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG1_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG1_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG1_MASKINGENABLE_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 3 of 8) (MASK_CFG2)
/** \brief Configures the HSYNC Masking block (Instance 3 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG2_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG2_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG2_RESERVED1_MASK) >> HVSYNC_MASK_CFG2_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG2_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG2_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG2_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG2_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG2_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG2_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG2_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG2_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG2_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG2_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG2_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG2_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG2_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG2_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG2_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG2_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG2_MASKINGENABLE_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 4 of 8) (MASK_CFG3)
/** \brief Configures the HSYNC Masking block (Instance 4 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG3_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG3_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG3_RESERVED1_MASK) >> HVSYNC_MASK_CFG3_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG3_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG3_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG3_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG3_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG3_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG3_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG3_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG3_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG3_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG3_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG3_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG3_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG3_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG3_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG3_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG3_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG3_MASKINGENABLE_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 5 of 8) (MASK_CFG4)
/** \brief Configures the HSYNC Masking block (Instance 5 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG4_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG4_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG4_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG4_RESERVED1_MASK) >> HVSYNC_MASK_CFG4_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG4_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG4_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG4_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG4_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG4_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG4_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG4_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG4_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG4_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG4_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG4_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG4_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG4_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG4_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG4_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG4_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG4_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG4_MASKINGENABLE_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 6 of 8) (MASK_CFG5)
/** \brief Configures the HSYNC Masking block (Instance 6 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG5_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG5_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG5_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG5_RESERVED1_MASK) >> HVSYNC_MASK_CFG5_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG5_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG5_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG5_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG5_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG5_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG5_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG5_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG5_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG5_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG5_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG5_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG5_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG5_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG5_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG5_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG5_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG5_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG5_MASKINGENABLE_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 7 of 8) (MASK_CFG6)
/** \brief Configures the HSYNC Masking block (Instance 7 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG6_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG6_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG6_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG6_RESERVED1_MASK) >> HVSYNC_MASK_CFG6_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG6_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG6_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG6_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG6_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG6_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG6_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG6_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG6_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG6_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG6_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG6_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG6_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG6_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG6_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG6_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG6_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG6_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG6_MASKINGENABLE_SHIFT))

//====================================================================
//Register: HSYNC Masking Configuration Register (Instance 8 of 8) (MASK_CFG7)
/** \brief Configures the HSYNC Masking block (Instance 8 of 8)*/
//====================================================================

#define  HVSYNC_MASK_CFG7_RESERVED1_MASK 0xfffff800
#define  HVSYNC_MASK_CFG7_RESERVED1_SHIFT 11
#define  HVSYNC_MASK_CFG7_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG7_RESERVED1_MASK) >> HVSYNC_MASK_CFG7_RESERVED1_SHIFT)
#define  HVSYNC_MASK_CFG7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG7_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG7_RESERVED1_SHIFT))

#define  HVSYNC_MASK_CFG7_MASKINGCOUNT_MASK 0x700
#define  HVSYNC_MASK_CFG7_MASKINGCOUNT_SHIFT 8
#define  HVSYNC_MASK_CFG7_MASKINGCOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG7_MASKINGCOUNT_MASK) >> HVSYNC_MASK_CFG7_MASKINGCOUNT_SHIFT)
#define  HVSYNC_MASK_CFG7_MASKINGCOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG7_MASKINGCOUNT_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG7_MASKINGCOUNT_SHIFT))

#define  HVSYNC_MASK_CFG7_MASKINGENABLE_MASK 0xff
#define  HVSYNC_MASK_CFG7_MASKINGENABLE_SHIFT 0
#define  HVSYNC_MASK_CFG7_MASKINGENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_MASK_CFG7_MASKINGENABLE_MASK) >> HVSYNC_MASK_CFG7_MASKINGENABLE_SHIFT)
#define  HVSYNC_MASK_CFG7_MASKINGENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_MASK_CFG7_MASKINGENABLE_MASK) | (((uint32_t)val) << HVSYNC_MASK_CFG7_MASKINGENABLE_SHIFT))

//====================================================================
//Register: Top Time Register (Instance 1 of 2) (TOP_TM0)
/** \brief Time from last HSYNC to receipt of Top (Instance 1 of 2)*/
//====================================================================

#define  HVSYNC_TOP_TM0_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TOP_TM0_RESERVED1_SHIFT 22
#define  HVSYNC_TOP_TM0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TOP_TM0_RESERVED1_MASK) >> HVSYNC_TOP_TM0_RESERVED1_SHIFT)
#define  HVSYNC_TOP_TM0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TOP_TM0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TOP_TM0_RESERVED1_SHIFT))

#define  HVSYNC_TOP_TM0_TOPTIMECOUNT_MASK 0x3ffff0
#define  HVSYNC_TOP_TM0_TOPTIMECOUNT_SHIFT 4
#define  HVSYNC_TOP_TM0_TOPTIMECOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_TOP_TM0_TOPTIMECOUNT_MASK) >> HVSYNC_TOP_TM0_TOPTIMECOUNT_SHIFT)
#define  HVSYNC_TOP_TM0_TOPTIMECOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TOP_TM0_TOPTIMECOUNT_MASK) | (((uint32_t)val) << HVSYNC_TOP_TM0_TOPTIMECOUNT_SHIFT))

#define  HVSYNC_TOP_TM0_RESERVED2_MASK 0xf
#define  HVSYNC_TOP_TM0_RESERVED2_SHIFT 0
#define  HVSYNC_TOP_TM0_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_TOP_TM0_RESERVED2_MASK) >> HVSYNC_TOP_TM0_RESERVED2_SHIFT)
#define  HVSYNC_TOP_TM0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TOP_TM0_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_TOP_TM0_RESERVED2_SHIFT))

//====================================================================
//Register: Top Time Register (Instance 2 of 2) (TOP_TM1)
/** \brief Time from last HSYNC to receipt of Top (Instance 2 of 2)*/
//====================================================================

#define  HVSYNC_TOP_TM1_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TOP_TM1_RESERVED1_SHIFT 22
#define  HVSYNC_TOP_TM1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TOP_TM1_RESERVED1_MASK) >> HVSYNC_TOP_TM1_RESERVED1_SHIFT)
#define  HVSYNC_TOP_TM1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TOP_TM1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TOP_TM1_RESERVED1_SHIFT))

#define  HVSYNC_TOP_TM1_TOPTIMECOUNT_MASK 0x3ffff0
#define  HVSYNC_TOP_TM1_TOPTIMECOUNT_SHIFT 4
#define  HVSYNC_TOP_TM1_TOPTIMECOUNT_MASK_SHIFT(reg) (((reg) & HVSYNC_TOP_TM1_TOPTIMECOUNT_MASK) >> HVSYNC_TOP_TM1_TOPTIMECOUNT_SHIFT)
#define  HVSYNC_TOP_TM1_TOPTIMECOUNT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TOP_TM1_TOPTIMECOUNT_MASK) | (((uint32_t)val) << HVSYNC_TOP_TM1_TOPTIMECOUNT_SHIFT))

#define  HVSYNC_TOP_TM1_RESERVED2_MASK 0xf
#define  HVSYNC_TOP_TM1_RESERVED2_SHIFT 0
#define  HVSYNC_TOP_TM1_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_TOP_TM1_RESERVED2_MASK) >> HVSYNC_TOP_TM1_RESERVED2_SHIFT)
#define  HVSYNC_TOP_TM1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TOP_TM1_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_TOP_TM1_RESERVED2_SHIFT))

//====================================================================
//Register: HVSync Status Register (HVSTAT)
/** \brief HVSync status register*/
//====================================================================

#define  HVSYNC_HVSTAT_RESERVED1_MASK 0xfffffff0
#define  HVSYNC_HVSTAT_RESERVED1_SHIFT 4
#define  HVSYNC_HVSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTAT_RESERVED1_MASK) >> HVSYNC_HVSTAT_RESERVED1_SHIFT)
#define  HVSYNC_HVSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTAT_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HVSTAT_RESERVED1_SHIFT))

#define  HVSYNC_HVSTAT_GALVORTOL_MASK 0xf
#define  HVSYNC_HVSTAT_GALVORTOL_SHIFT 0
#define  HVSYNC_HVSTAT_GALVORTOL_MASK_SHIFT(reg) (((reg) & HVSYNC_HVSTAT_GALVORTOL_MASK) >> HVSYNC_HVSTAT_GALVORTOL_SHIFT)
#define  HVSYNC_HVSTAT_GALVORTOL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HVSTAT_GALVORTOL_MASK) | (((uint32_t)val) << HVSYNC_HVSTAT_GALVORTOL_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 1 of 12) (TM0_CNT0)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 1 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT0_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT0_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT0_RESERVED1_MASK) >> HVSYNC_TM0_CNT0_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT0_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT0_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT0_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT0_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT0_FACETTIME_MASK) >> HVSYNC_TM0_CNT0_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT0_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT0_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT0_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 2 of 12) (TM0_CNT1)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 2 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT1_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT1_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT1_RESERVED1_MASK) >> HVSYNC_TM0_CNT1_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT1_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT1_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT1_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT1_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT1_FACETTIME_MASK) >> HVSYNC_TM0_CNT1_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT1_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT1_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT1_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 3 of 12) (TM0_CNT2)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 3 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT2_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT2_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT2_RESERVED1_MASK) >> HVSYNC_TM0_CNT2_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT2_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT2_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT2_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT2_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT2_FACETTIME_MASK) >> HVSYNC_TM0_CNT2_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT2_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT2_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT2_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 4 of 12) (TM0_CNT3)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 4 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT3_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT3_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT3_RESERVED1_MASK) >> HVSYNC_TM0_CNT3_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT3_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT3_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT3_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT3_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT3_FACETTIME_MASK) >> HVSYNC_TM0_CNT3_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT3_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT3_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT3_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 5 of 12) (TM0_CNT4)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 5 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT4_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT4_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT4_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT4_RESERVED1_MASK) >> HVSYNC_TM0_CNT4_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT4_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT4_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT4_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT4_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT4_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT4_FACETTIME_MASK) >> HVSYNC_TM0_CNT4_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT4_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT4_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT4_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 6 of 12) (TM0_CNT5)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 6 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT5_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT5_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT5_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT5_RESERVED1_MASK) >> HVSYNC_TM0_CNT5_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT5_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT5_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT5_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT5_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT5_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT5_FACETTIME_MASK) >> HVSYNC_TM0_CNT5_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT5_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT5_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT5_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 7 of 12) (TM0_CNT6)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 7 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT6_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT6_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT6_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT6_RESERVED1_MASK) >> HVSYNC_TM0_CNT6_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT6_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT6_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT6_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT6_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT6_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT6_FACETTIME_MASK) >> HVSYNC_TM0_CNT6_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT6_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT6_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT6_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 8 of 12) (TM0_CNT7)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 8 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT7_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT7_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT7_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT7_RESERVED1_MASK) >> HVSYNC_TM0_CNT7_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT7_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT7_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT7_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT7_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT7_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT7_FACETTIME_MASK) >> HVSYNC_TM0_CNT7_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT7_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT7_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT7_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 9 of 12) (TM0_CNT8)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 9 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT8_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT8_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT8_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT8_RESERVED1_MASK) >> HVSYNC_TM0_CNT8_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT8_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT8_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT8_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT8_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT8_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT8_FACETTIME_MASK) >> HVSYNC_TM0_CNT8_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT8_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT8_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT8_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 10 of 12) (TM0_CNT9)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 10 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT9_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT9_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT9_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT9_RESERVED1_MASK) >> HVSYNC_TM0_CNT9_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT9_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT9_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT9_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT9_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT9_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT9_FACETTIME_MASK) >> HVSYNC_TM0_CNT9_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT9_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT9_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT9_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 11 of 12) (TM0_CNT10)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 11 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT10_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT10_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT10_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT10_RESERVED1_MASK) >> HVSYNC_TM0_CNT10_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT10_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT10_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT10_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT10_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT10_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT10_FACETTIME_MASK) >> HVSYNC_TM0_CNT10_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT10_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT10_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT10_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 12 of 12) (TM0_CNT11)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 12 of 12)*/
//====================================================================

#define  HVSYNC_TM0_CNT11_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM0_CNT11_RESERVED1_SHIFT 22
#define  HVSYNC_TM0_CNT11_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT11_RESERVED1_MASK) >> HVSYNC_TM0_CNT11_RESERVED1_SHIFT)
#define  HVSYNC_TM0_CNT11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT11_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT11_RESERVED1_SHIFT))

#define  HVSYNC_TM0_CNT11_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM0_CNT11_FACETTIME_SHIFT 0
#define  HVSYNC_TM0_CNT11_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM0_CNT11_FACETTIME_MASK) >> HVSYNC_TM0_CNT11_FACETTIME_SHIFT)
#define  HVSYNC_TM0_CNT11_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM0_CNT11_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM0_CNT11_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 1 of 12) (TM1_CNT0)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 1 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT0_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT0_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT0_RESERVED1_MASK) >> HVSYNC_TM1_CNT0_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT0_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT0_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT0_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT0_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT0_FACETTIME_MASK) >> HVSYNC_TM1_CNT0_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT0_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT0_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT0_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 2 of 12) (TM1_CNT1)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 2 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT1_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT1_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT1_RESERVED1_MASK) >> HVSYNC_TM1_CNT1_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT1_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT1_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT1_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT1_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT1_FACETTIME_MASK) >> HVSYNC_TM1_CNT1_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT1_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT1_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT1_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 3 of 12) (TM1_CNT2)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 3 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT2_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT2_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT2_RESERVED1_MASK) >> HVSYNC_TM1_CNT2_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT2_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT2_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT2_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT2_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT2_FACETTIME_MASK) >> HVSYNC_TM1_CNT2_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT2_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT2_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT2_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 4 of 12) (TM1_CNT3)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 4 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT3_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT3_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT3_RESERVED1_MASK) >> HVSYNC_TM1_CNT3_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT3_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT3_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT3_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT3_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT3_FACETTIME_MASK) >> HVSYNC_TM1_CNT3_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT3_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT3_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT3_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 5 of 12) (TM1_CNT4)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 5 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT4_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT4_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT4_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT4_RESERVED1_MASK) >> HVSYNC_TM1_CNT4_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT4_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT4_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT4_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT4_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT4_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT4_FACETTIME_MASK) >> HVSYNC_TM1_CNT4_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT4_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT4_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT4_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 6 of 12) (TM1_CNT5)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 6 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT5_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT5_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT5_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT5_RESERVED1_MASK) >> HVSYNC_TM1_CNT5_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT5_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT5_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT5_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT5_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT5_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT5_FACETTIME_MASK) >> HVSYNC_TM1_CNT5_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT5_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT5_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT5_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 7 of 12) (TM1_CNT6)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 7 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT6_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT6_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT6_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT6_RESERVED1_MASK) >> HVSYNC_TM1_CNT6_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT6_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT6_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT6_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT6_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT6_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT6_FACETTIME_MASK) >> HVSYNC_TM1_CNT6_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT6_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT6_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT6_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 8 of 12) (TM1_CNT7)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 8 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT7_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT7_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT7_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT7_RESERVED1_MASK) >> HVSYNC_TM1_CNT7_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT7_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT7_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT7_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT7_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT7_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT7_FACETTIME_MASK) >> HVSYNC_TM1_CNT7_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT7_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT7_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT7_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 9 of 12) (TM1_CNT8)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 9 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT8_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT8_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT8_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT8_RESERVED1_MASK) >> HVSYNC_TM1_CNT8_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT8_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT8_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT8_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT8_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT8_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT8_FACETTIME_MASK) >> HVSYNC_TM1_CNT8_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT8_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT8_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT8_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 10 of 12) (TM1_CNT9)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 10 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT9_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT9_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT9_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT9_RESERVED1_MASK) >> HVSYNC_TM1_CNT9_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT9_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT9_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT9_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT9_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT9_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT9_FACETTIME_MASK) >> HVSYNC_TM1_CNT9_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT9_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT9_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT9_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 11 of 12) (TM1_CNT10)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 11 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT10_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT10_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT10_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT10_RESERVED1_MASK) >> HVSYNC_TM1_CNT10_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT10_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT10_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT10_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT10_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT10_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT10_FACETTIME_MASK) >> HVSYNC_TM1_CNT10_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT10_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT10_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT10_FACETTIME_SHIFT))

//====================================================================
//Register: Facet Time Register (Instance 12 of 12) (TM1_CNT11)
/** \brief Facet Time Register - Contains the last valid time for this facet. The register is not synchronized to the system clock domain. The value may not be stable if the register is being updated when read. It should be read multiple times to make sure the data is valid. (Instance 12 of 12)*/
//====================================================================

#define  HVSYNC_TM1_CNT11_RESERVED1_MASK 0xffc00000
#define  HVSYNC_TM1_CNT11_RESERVED1_SHIFT 22
#define  HVSYNC_TM1_CNT11_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT11_RESERVED1_MASK) >> HVSYNC_TM1_CNT11_RESERVED1_SHIFT)
#define  HVSYNC_TM1_CNT11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT11_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT11_RESERVED1_SHIFT))

#define  HVSYNC_TM1_CNT11_FACETTIME_MASK 0x3fffff
#define  HVSYNC_TM1_CNT11_FACETTIME_SHIFT 0
#define  HVSYNC_TM1_CNT11_FACETTIME_MASK_SHIFT(reg) (((reg) & HVSYNC_TM1_CNT11_FACETTIME_MASK) >> HVSYNC_TM1_CNT11_FACETTIME_SHIFT)
#define  HVSYNC_TM1_CNT11_FACETTIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_TM1_CNT11_FACETTIME_MASK) | (((uint32_t)val) << HVSYNC_TM1_CNT11_FACETTIME_SHIFT))

//====================================================================
//Register: HSYNC Delay Register (Instance 1 of 2) (HDLR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  HVSYNC_HDLR0_RESERVED1_MASK 0xffe00000
#define  HVSYNC_HDLR0_RESERVED1_SHIFT 21
#define  HVSYNC_HDLR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR0_RESERVED1_MASK) >> HVSYNC_HDLR0_RESERVED1_SHIFT)
#define  HVSYNC_HDLR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HDLR0_RESERVED1_SHIFT))

#define  HVSYNC_HDLR0_HDLYINPUTSEL_MASK 0x100000
#define  HVSYNC_HDLR0_HDLYINPUTSEL_SHIFT 20
#define  HVSYNC_HDLR0_HDLYINPUTSEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR0_HDLYINPUTSEL_MASK) >> HVSYNC_HDLR0_HDLYINPUTSEL_SHIFT)
#define  HVSYNC_HDLR0_HDLYINPUTSEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR0_HDLYINPUTSEL_MASK) | (((uint32_t)val) << HVSYNC_HDLR0_HDLYINPUTSEL_SHIFT))

#define  HVSYNC_HDLR0_RESERVED2_MASK 0xc0000
#define  HVSYNC_HDLR0_RESERVED2_SHIFT 18
#define  HVSYNC_HDLR0_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR0_RESERVED2_MASK) >> HVSYNC_HDLR0_RESERVED2_SHIFT)
#define  HVSYNC_HDLR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR0_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HDLR0_RESERVED2_SHIFT))

#define  HVSYNC_HDLR0_HSYNCDLY_MASK 0x3ffff
#define  HVSYNC_HDLR0_HSYNCDLY_SHIFT 0
#define  HVSYNC_HDLR0_HSYNCDLY_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR0_HSYNCDLY_MASK) >> HVSYNC_HDLR0_HSYNCDLY_SHIFT)
#define  HVSYNC_HDLR0_HSYNCDLY_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR0_HSYNCDLY_MASK) | (((uint32_t)val) << HVSYNC_HDLR0_HSYNCDLY_SHIFT))

//====================================================================
//Register: HSYNC Delay Register (Instance 2 of 2) (HDLR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  HVSYNC_HDLR1_RESERVED1_MASK 0xffe00000
#define  HVSYNC_HDLR1_RESERVED1_SHIFT 21
#define  HVSYNC_HDLR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR1_RESERVED1_MASK) >> HVSYNC_HDLR1_RESERVED1_SHIFT)
#define  HVSYNC_HDLR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HDLR1_RESERVED1_SHIFT))

#define  HVSYNC_HDLR1_HDLYINPUTSEL_MASK 0x100000
#define  HVSYNC_HDLR1_HDLYINPUTSEL_SHIFT 20
#define  HVSYNC_HDLR1_HDLYINPUTSEL_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR1_HDLYINPUTSEL_MASK) >> HVSYNC_HDLR1_HDLYINPUTSEL_SHIFT)
#define  HVSYNC_HDLR1_HDLYINPUTSEL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR1_HDLYINPUTSEL_MASK) | (((uint32_t)val) << HVSYNC_HDLR1_HDLYINPUTSEL_SHIFT))

#define  HVSYNC_HDLR1_RESERVED2_MASK 0xc0000
#define  HVSYNC_HDLR1_RESERVED2_SHIFT 18
#define  HVSYNC_HDLR1_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR1_RESERVED2_MASK) >> HVSYNC_HDLR1_RESERVED2_SHIFT)
#define  HVSYNC_HDLR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR1_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HDLR1_RESERVED2_SHIFT))

#define  HVSYNC_HDLR1_HSYNCDLY_MASK 0x3ffff
#define  HVSYNC_HDLR1_HSYNCDLY_SHIFT 0
#define  HVSYNC_HDLR1_HSYNCDLY_MASK_SHIFT(reg) (((reg) & HVSYNC_HDLR1_HSYNCDLY_MASK) >> HVSYNC_HDLR1_HSYNCDLY_SHIFT)
#define  HVSYNC_HDLR1_HSYNCDLY_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HDLR1_HSYNCDLY_MASK) | (((uint32_t)val) << HVSYNC_HDLR1_HSYNCDLY_SHIFT))

//====================================================================
//Register: HVSync Edge Timer Data Register (ETDATA)
/** \brief Register access to Edge Timer data FIFO*/
//====================================================================

#define  HVSYNC_ETDATA_RESERVED1_MASK 0xfc000000
#define  HVSYNC_ETDATA_RESERVED1_SHIFT 26
#define  HVSYNC_ETDATA_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_ETDATA_RESERVED1_MASK) >> HVSYNC_ETDATA_RESERVED1_SHIFT)
#define  HVSYNC_ETDATA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETDATA_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_ETDATA_RESERVED1_SHIFT))

#define  HVSYNC_ETDATA_EDGETYPE_MASK 0x3000000
#define  HVSYNC_ETDATA_EDGETYPE_SHIFT 24
#define  HVSYNC_ETDATA_EDGETYPE_MASK_SHIFT(reg) (((reg) & HVSYNC_ETDATA_EDGETYPE_MASK) >> HVSYNC_ETDATA_EDGETYPE_SHIFT)
#define  HVSYNC_ETDATA_EDGETYPE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETDATA_EDGETYPE_MASK) | (((uint32_t)val) << HVSYNC_ETDATA_EDGETYPE_SHIFT))

#define  HVSYNC_ETDATA_EDGESOURCE_MASK 0xc00000
#define  HVSYNC_ETDATA_EDGESOURCE_SHIFT 22
#define  HVSYNC_ETDATA_EDGESOURCE_MASK_SHIFT(reg) (((reg) & HVSYNC_ETDATA_EDGESOURCE_MASK) >> HVSYNC_ETDATA_EDGESOURCE_SHIFT)
#define  HVSYNC_ETDATA_EDGESOURCE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETDATA_EDGESOURCE_MASK) | (((uint32_t)val) << HVSYNC_ETDATA_EDGESOURCE_SHIFT))

#define  HVSYNC_ETDATA_EDGETIME_MASK 0x3fffff
#define  HVSYNC_ETDATA_EDGETIME_SHIFT 0
#define  HVSYNC_ETDATA_EDGETIME_MASK_SHIFT(reg) (((reg) & HVSYNC_ETDATA_EDGETIME_MASK) >> HVSYNC_ETDATA_EDGETIME_SHIFT)
#define  HVSYNC_ETDATA_EDGETIME_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETDATA_EDGETIME_MASK) | (((uint32_t)val) << HVSYNC_ETDATA_EDGETIME_SHIFT))

//====================================================================
//Register: HVSync Edge Timer Configuration Register (ETCFG)
/** \brief Used to configure the Edge Timer measurement hardware*/
//====================================================================

#define  HVSYNC_ETCFG_RESERVED1_MASK 0xffffc000
#define  HVSYNC_ETCFG_RESERVED1_SHIFT 14
#define  HVSYNC_ETCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_RESERVED1_MASK) >> HVSYNC_ETCFG_RESERVED1_SHIFT)
#define  HVSYNC_ETCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_RESERVED1_SHIFT))

#define  HVSYNC_ETCFG_EDGEFIFOACCESS_MASK 0x2000
#define  HVSYNC_ETCFG_EDGEFIFOACCESS_SHIFT 13
#define  HVSYNC_ETCFG_EDGEFIFOACCESS_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_EDGEFIFOACCESS_MASK) >> HVSYNC_ETCFG_EDGEFIFOACCESS_SHIFT)
#define  HVSYNC_ETCFG_EDGEFIFOACCESS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_EDGEFIFOACCESS_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_EDGEFIFOACCESS_SHIFT))

#define  HVSYNC_ETCFG_EDGETMRRST_MASK 0x1000
#define  HVSYNC_ETCFG_EDGETMRRST_SHIFT 12
#define  HVSYNC_ETCFG_EDGETMRRST_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_EDGETMRRST_MASK) >> HVSYNC_ETCFG_EDGETMRRST_SHIFT)
#define  HVSYNC_ETCFG_EDGETMRRST_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_EDGETMRRST_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_EDGETMRRST_SHIFT))

#define  HVSYNC_ETCFG_EDGETIMEREN_MASK 0xf00
#define  HVSYNC_ETCFG_EDGETIMEREN_SHIFT 8
#define  HVSYNC_ETCFG_EDGETIMEREN_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_EDGETIMEREN_MASK) >> HVSYNC_ETCFG_EDGETIMEREN_SHIFT)
#define  HVSYNC_ETCFG_EDGETIMEREN_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_EDGETIMEREN_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_EDGETIMEREN_SHIFT))

#define  HVSYNC_ETCFG_EDGEMASK3_MASK 0xc0
#define  HVSYNC_ETCFG_EDGEMASK3_SHIFT 6
#define  HVSYNC_ETCFG_EDGEMASK3_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_EDGEMASK3_MASK) >> HVSYNC_ETCFG_EDGEMASK3_SHIFT)
#define  HVSYNC_ETCFG_EDGEMASK3_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_EDGEMASK3_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_EDGEMASK3_SHIFT))

#define  HVSYNC_ETCFG_EDGEMASK2_MASK 0x30
#define  HVSYNC_ETCFG_EDGEMASK2_SHIFT 4
#define  HVSYNC_ETCFG_EDGEMASK2_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_EDGEMASK2_MASK) >> HVSYNC_ETCFG_EDGEMASK2_SHIFT)
#define  HVSYNC_ETCFG_EDGEMASK2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_EDGEMASK2_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_EDGEMASK2_SHIFT))

#define  HVSYNC_ETCFG_EDGEMASK1_MASK 0xc
#define  HVSYNC_ETCFG_EDGEMASK1_SHIFT 2
#define  HVSYNC_ETCFG_EDGEMASK1_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_EDGEMASK1_MASK) >> HVSYNC_ETCFG_EDGEMASK1_SHIFT)
#define  HVSYNC_ETCFG_EDGEMASK1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_EDGEMASK1_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_EDGEMASK1_SHIFT))

#define  HVSYNC_ETCFG_EDGEMASK0_MASK 0x3
#define  HVSYNC_ETCFG_EDGEMASK0_SHIFT 0
#define  HVSYNC_ETCFG_EDGEMASK0_MASK_SHIFT(reg) (((reg) & HVSYNC_ETCFG_EDGEMASK0_MASK) >> HVSYNC_ETCFG_EDGEMASK0_SHIFT)
#define  HVSYNC_ETCFG_EDGEMASK0_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETCFG_EDGEMASK0_MASK) | (((uint32_t)val) << HVSYNC_ETCFG_EDGEMASK0_SHIFT))

//====================================================================
//Register: HVSync Edge Timer FIFO Status Register (ETSTAT)
/** \brief Status of Edge Timer data FIFO*/
//====================================================================

#define  HVSYNC_ETSTAT_RESERVED1_MASK 0xffffffe0
#define  HVSYNC_ETSTAT_RESERVED1_SHIFT 5
#define  HVSYNC_ETSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_ETSTAT_RESERVED1_MASK) >> HVSYNC_ETSTAT_RESERVED1_SHIFT)
#define  HVSYNC_ETSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETSTAT_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_ETSTAT_RESERVED1_SHIFT))

#define  HVSYNC_ETSTAT_EDGELOSS_MASK 0x10
#define  HVSYNC_ETSTAT_EDGELOSS_SHIFT 4
#define  HVSYNC_ETSTAT_EDGELOSS_MASK_SHIFT(reg) (((reg) & HVSYNC_ETSTAT_EDGELOSS_MASK) >> HVSYNC_ETSTAT_EDGELOSS_SHIFT)
#define  HVSYNC_ETSTAT_EDGELOSS_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETSTAT_EDGELOSS_MASK) | (((uint32_t)val) << HVSYNC_ETSTAT_EDGELOSS_SHIFT))

#define  HVSYNC_ETSTAT_ALMOSTFULL_MASK 0x8
#define  HVSYNC_ETSTAT_ALMOSTFULL_SHIFT 3
#define  HVSYNC_ETSTAT_ALMOSTFULL_MASK_SHIFT(reg) (((reg) & HVSYNC_ETSTAT_ALMOSTFULL_MASK) >> HVSYNC_ETSTAT_ALMOSTFULL_SHIFT)
#define  HVSYNC_ETSTAT_ALMOSTFULL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETSTAT_ALMOSTFULL_MASK) | (((uint32_t)val) << HVSYNC_ETSTAT_ALMOSTFULL_SHIFT))

#define  HVSYNC_ETSTAT_FULL_MASK 0x4
#define  HVSYNC_ETSTAT_FULL_SHIFT 2
#define  HVSYNC_ETSTAT_FULL_MASK_SHIFT(reg) (((reg) & HVSYNC_ETSTAT_FULL_MASK) >> HVSYNC_ETSTAT_FULL_SHIFT)
#define  HVSYNC_ETSTAT_FULL_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETSTAT_FULL_MASK) | (((uint32_t)val) << HVSYNC_ETSTAT_FULL_SHIFT))

#define  HVSYNC_ETSTAT_ALMOSTEMPTY_MASK 0x2
#define  HVSYNC_ETSTAT_ALMOSTEMPTY_SHIFT 1
#define  HVSYNC_ETSTAT_ALMOSTEMPTY_MASK_SHIFT(reg) (((reg) & HVSYNC_ETSTAT_ALMOSTEMPTY_MASK) >> HVSYNC_ETSTAT_ALMOSTEMPTY_SHIFT)
#define  HVSYNC_ETSTAT_ALMOSTEMPTY_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETSTAT_ALMOSTEMPTY_MASK) | (((uint32_t)val) << HVSYNC_ETSTAT_ALMOSTEMPTY_SHIFT))

#define  HVSYNC_ETSTAT_EMPTY_MASK 0x1
#define  HVSYNC_ETSTAT_EMPTY_SHIFT 0
#define  HVSYNC_ETSTAT_EMPTY_MASK_SHIFT(reg) (((reg) & HVSYNC_ETSTAT_EMPTY_MASK) >> HVSYNC_ETSTAT_EMPTY_SHIFT)
#define  HVSYNC_ETSTAT_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_ETSTAT_EMPTY_MASK) | (((uint32_t)val) << HVSYNC_ETSTAT_EMPTY_SHIFT))

//====================================================================
//Register: HSYNC Watchdog Configuration Register (Instance 1 of 4) (HWCR0)
/** \brief Used to configure the HSYNC Watchdog Timer hardware (Instance 1 of 4)*/
//====================================================================

#define  HVSYNC_HWCR0_RESERVED1_MASK 0xff000000
#define  HVSYNC_HWCR0_RESERVED1_SHIFT 24
#define  HVSYNC_HWCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR0_RESERVED1_MASK) >> HVSYNC_HWCR0_RESERVED1_SHIFT)
#define  HVSYNC_HWCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR0_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HWCR0_RESERVED1_SHIFT))

#define  HVSYNC_HWCR0_HTIMEOUT_MASK 0xfffff0
#define  HVSYNC_HWCR0_HTIMEOUT_SHIFT 4
#define  HVSYNC_HWCR0_HTIMEOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR0_HTIMEOUT_MASK) >> HVSYNC_HWCR0_HTIMEOUT_SHIFT)
#define  HVSYNC_HWCR0_HTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR0_HTIMEOUT_MASK) | (((uint32_t)val) << HVSYNC_HWCR0_HTIMEOUT_SHIFT))

#define  HVSYNC_HWCR0_RESERVED2_MASK 0x8
#define  HVSYNC_HWCR0_RESERVED2_SHIFT 3
#define  HVSYNC_HWCR0_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR0_RESERVED2_MASK) >> HVSYNC_HWCR0_RESERVED2_SHIFT)
#define  HVSYNC_HWCR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR0_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HWCR0_RESERVED2_SHIFT))

#define  HVSYNC_HWCR0_FILTERFB_MASK 0x4
#define  HVSYNC_HWCR0_FILTERFB_SHIFT 2
#define  HVSYNC_HWCR0_FILTERFB_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR0_FILTERFB_MASK) >> HVSYNC_HWCR0_FILTERFB_SHIFT)
#define  HVSYNC_HWCR0_FILTERFB_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR0_FILTERFB_MASK) | (((uint32_t)val) << HVSYNC_HWCR0_FILTERFB_SHIFT))

#define  HVSYNC_HWCR0_ALLOW_MASK 0x2
#define  HVSYNC_HWCR0_ALLOW_SHIFT 1
#define  HVSYNC_HWCR0_ALLOW_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR0_ALLOW_MASK) >> HVSYNC_HWCR0_ALLOW_SHIFT)
#define  HVSYNC_HWCR0_ALLOW_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR0_ALLOW_MASK) | (((uint32_t)val) << HVSYNC_HWCR0_ALLOW_SHIFT))

#define  HVSYNC_HWCR0_ENABLE_MASK 0x1
#define  HVSYNC_HWCR0_ENABLE_SHIFT 0
#define  HVSYNC_HWCR0_ENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR0_ENABLE_MASK) >> HVSYNC_HWCR0_ENABLE_SHIFT)
#define  HVSYNC_HWCR0_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR0_ENABLE_MASK) | (((uint32_t)val) << HVSYNC_HWCR0_ENABLE_SHIFT))

//====================================================================
//Register: HSYNC Watchdog Configuration Register (Instance 2 of 4) (HWCR1)
/** \brief Used to configure the HSYNC Watchdog Timer hardware (Instance 2 of 4)*/
//====================================================================

#define  HVSYNC_HWCR1_RESERVED1_MASK 0xff000000
#define  HVSYNC_HWCR1_RESERVED1_SHIFT 24
#define  HVSYNC_HWCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR1_RESERVED1_MASK) >> HVSYNC_HWCR1_RESERVED1_SHIFT)
#define  HVSYNC_HWCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR1_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HWCR1_RESERVED1_SHIFT))

#define  HVSYNC_HWCR1_HTIMEOUT_MASK 0xfffff0
#define  HVSYNC_HWCR1_HTIMEOUT_SHIFT 4
#define  HVSYNC_HWCR1_HTIMEOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR1_HTIMEOUT_MASK) >> HVSYNC_HWCR1_HTIMEOUT_SHIFT)
#define  HVSYNC_HWCR1_HTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR1_HTIMEOUT_MASK) | (((uint32_t)val) << HVSYNC_HWCR1_HTIMEOUT_SHIFT))

#define  HVSYNC_HWCR1_RESERVED2_MASK 0x8
#define  HVSYNC_HWCR1_RESERVED2_SHIFT 3
#define  HVSYNC_HWCR1_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR1_RESERVED2_MASK) >> HVSYNC_HWCR1_RESERVED2_SHIFT)
#define  HVSYNC_HWCR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR1_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HWCR1_RESERVED2_SHIFT))

#define  HVSYNC_HWCR1_FILTERFB_MASK 0x4
#define  HVSYNC_HWCR1_FILTERFB_SHIFT 2
#define  HVSYNC_HWCR1_FILTERFB_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR1_FILTERFB_MASK) >> HVSYNC_HWCR1_FILTERFB_SHIFT)
#define  HVSYNC_HWCR1_FILTERFB_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR1_FILTERFB_MASK) | (((uint32_t)val) << HVSYNC_HWCR1_FILTERFB_SHIFT))

#define  HVSYNC_HWCR1_ALLOW_MASK 0x2
#define  HVSYNC_HWCR1_ALLOW_SHIFT 1
#define  HVSYNC_HWCR1_ALLOW_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR1_ALLOW_MASK) >> HVSYNC_HWCR1_ALLOW_SHIFT)
#define  HVSYNC_HWCR1_ALLOW_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR1_ALLOW_MASK) | (((uint32_t)val) << HVSYNC_HWCR1_ALLOW_SHIFT))

#define  HVSYNC_HWCR1_ENABLE_MASK 0x1
#define  HVSYNC_HWCR1_ENABLE_SHIFT 0
#define  HVSYNC_HWCR1_ENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR1_ENABLE_MASK) >> HVSYNC_HWCR1_ENABLE_SHIFT)
#define  HVSYNC_HWCR1_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR1_ENABLE_MASK) | (((uint32_t)val) << HVSYNC_HWCR1_ENABLE_SHIFT))

//====================================================================
//Register: HSYNC Watchdog Configuration Register (Instance 3 of 4) (HWCR2)
/** \brief Used to configure the HSYNC Watchdog Timer hardware (Instance 3 of 4)*/
//====================================================================

#define  HVSYNC_HWCR2_RESERVED1_MASK 0xff000000
#define  HVSYNC_HWCR2_RESERVED1_SHIFT 24
#define  HVSYNC_HWCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR2_RESERVED1_MASK) >> HVSYNC_HWCR2_RESERVED1_SHIFT)
#define  HVSYNC_HWCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR2_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HWCR2_RESERVED1_SHIFT))

#define  HVSYNC_HWCR2_HTIMEOUT_MASK 0xfffff0
#define  HVSYNC_HWCR2_HTIMEOUT_SHIFT 4
#define  HVSYNC_HWCR2_HTIMEOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR2_HTIMEOUT_MASK) >> HVSYNC_HWCR2_HTIMEOUT_SHIFT)
#define  HVSYNC_HWCR2_HTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR2_HTIMEOUT_MASK) | (((uint32_t)val) << HVSYNC_HWCR2_HTIMEOUT_SHIFT))

#define  HVSYNC_HWCR2_RESERVED2_MASK 0x8
#define  HVSYNC_HWCR2_RESERVED2_SHIFT 3
#define  HVSYNC_HWCR2_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR2_RESERVED2_MASK) >> HVSYNC_HWCR2_RESERVED2_SHIFT)
#define  HVSYNC_HWCR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR2_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HWCR2_RESERVED2_SHIFT))

#define  HVSYNC_HWCR2_FILTERFB_MASK 0x4
#define  HVSYNC_HWCR2_FILTERFB_SHIFT 2
#define  HVSYNC_HWCR2_FILTERFB_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR2_FILTERFB_MASK) >> HVSYNC_HWCR2_FILTERFB_SHIFT)
#define  HVSYNC_HWCR2_FILTERFB_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR2_FILTERFB_MASK) | (((uint32_t)val) << HVSYNC_HWCR2_FILTERFB_SHIFT))

#define  HVSYNC_HWCR2_ALLOW_MASK 0x2
#define  HVSYNC_HWCR2_ALLOW_SHIFT 1
#define  HVSYNC_HWCR2_ALLOW_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR2_ALLOW_MASK) >> HVSYNC_HWCR2_ALLOW_SHIFT)
#define  HVSYNC_HWCR2_ALLOW_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR2_ALLOW_MASK) | (((uint32_t)val) << HVSYNC_HWCR2_ALLOW_SHIFT))

#define  HVSYNC_HWCR2_ENABLE_MASK 0x1
#define  HVSYNC_HWCR2_ENABLE_SHIFT 0
#define  HVSYNC_HWCR2_ENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR2_ENABLE_MASK) >> HVSYNC_HWCR2_ENABLE_SHIFT)
#define  HVSYNC_HWCR2_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR2_ENABLE_MASK) | (((uint32_t)val) << HVSYNC_HWCR2_ENABLE_SHIFT))

//====================================================================
//Register: HSYNC Watchdog Configuration Register (Instance 4 of 4) (HWCR3)
/** \brief Used to configure the HSYNC Watchdog Timer hardware (Instance 4 of 4)*/
//====================================================================

#define  HVSYNC_HWCR3_RESERVED1_MASK 0xff000000
#define  HVSYNC_HWCR3_RESERVED1_SHIFT 24
#define  HVSYNC_HWCR3_RESERVED1_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR3_RESERVED1_MASK) >> HVSYNC_HWCR3_RESERVED1_SHIFT)
#define  HVSYNC_HWCR3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR3_RESERVED1_MASK) | (((uint32_t)val) << HVSYNC_HWCR3_RESERVED1_SHIFT))

#define  HVSYNC_HWCR3_HTIMEOUT_MASK 0xfffff0
#define  HVSYNC_HWCR3_HTIMEOUT_SHIFT 4
#define  HVSYNC_HWCR3_HTIMEOUT_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR3_HTIMEOUT_MASK) >> HVSYNC_HWCR3_HTIMEOUT_SHIFT)
#define  HVSYNC_HWCR3_HTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR3_HTIMEOUT_MASK) | (((uint32_t)val) << HVSYNC_HWCR3_HTIMEOUT_SHIFT))

#define  HVSYNC_HWCR3_RESERVED2_MASK 0x8
#define  HVSYNC_HWCR3_RESERVED2_SHIFT 3
#define  HVSYNC_HWCR3_RESERVED2_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR3_RESERVED2_MASK) >> HVSYNC_HWCR3_RESERVED2_SHIFT)
#define  HVSYNC_HWCR3_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR3_RESERVED2_MASK) | (((uint32_t)val) << HVSYNC_HWCR3_RESERVED2_SHIFT))

#define  HVSYNC_HWCR3_FILTERFB_MASK 0x4
#define  HVSYNC_HWCR3_FILTERFB_SHIFT 2
#define  HVSYNC_HWCR3_FILTERFB_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR3_FILTERFB_MASK) >> HVSYNC_HWCR3_FILTERFB_SHIFT)
#define  HVSYNC_HWCR3_FILTERFB_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR3_FILTERFB_MASK) | (((uint32_t)val) << HVSYNC_HWCR3_FILTERFB_SHIFT))

#define  HVSYNC_HWCR3_ALLOW_MASK 0x2
#define  HVSYNC_HWCR3_ALLOW_SHIFT 1
#define  HVSYNC_HWCR3_ALLOW_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR3_ALLOW_MASK) >> HVSYNC_HWCR3_ALLOW_SHIFT)
#define  HVSYNC_HWCR3_ALLOW_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR3_ALLOW_MASK) | (((uint32_t)val) << HVSYNC_HWCR3_ALLOW_SHIFT))

#define  HVSYNC_HWCR3_ENABLE_MASK 0x1
#define  HVSYNC_HWCR3_ENABLE_SHIFT 0
#define  HVSYNC_HWCR3_ENABLE_MASK_SHIFT(reg) (((reg) & HVSYNC_HWCR3_ENABLE_MASK) >> HVSYNC_HWCR3_ENABLE_SHIFT)
#define  HVSYNC_HWCR3_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HVSYNC_HWCR3_ENABLE_MASK) | (((uint32_t)val) << HVSYNC_HWCR3_ENABLE_SHIFT))

#endif // HVSYNC
