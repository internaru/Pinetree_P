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
 * \file SVDO_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SVDO_REGMASKS_H_
#define _SVDO_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SVDO (SVDO)
/** \brief Top-level register file for SVDO block*/
//
//====================================================================

//====================================================================
//Register: Video Configuration Register (SCFG)
/** \brief This register configures the major features of the SVDO module*/
//====================================================================

#define  SVDO_SCFG_RESERVED1_MASK 0xc0000000
#define  SVDO_SCFG_RESERVED1_SHIFT 30
#define  SVDO_SCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESERVED1_MASK) >> SVDO_SCFG_RESERVED1_SHIFT)
#define  SVDO_SCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESERVED1_MASK) | (((uint32_t)val) << SVDO_SCFG_RESERVED1_SHIFT))

#define  SVDO_SCFG_BURSTSZ_MASK 0x30000000
#define  SVDO_SCFG_BURSTSZ_SHIFT 28
#define  SVDO_SCFG_BURSTSZ_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_BURSTSZ_MASK) >> SVDO_SCFG_BURSTSZ_SHIFT)
#define  SVDO_SCFG_BURSTSZ_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_BURSTSZ_MASK) | (((uint32_t)val) << SVDO_SCFG_BURSTSZ_SHIFT))

#define  SVDO_SCFG_RESERVED2_MASK 0xc000000
#define  SVDO_SCFG_RESERVED2_SHIFT 26
#define  SVDO_SCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESERVED2_MASK) >> SVDO_SCFG_RESERVED2_SHIFT)
#define  SVDO_SCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESERVED2_MASK) | (((uint32_t)val) << SVDO_SCFG_RESERVED2_SHIFT))

#define  SVDO_SCFG_SERP_MASK 0x2000000
#define  SVDO_SCFG_SERP_SHIFT 25
#define  SVDO_SCFG_SERP_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_SERP_MASK) >> SVDO_SCFG_SERP_SHIFT)
#define  SVDO_SCFG_SERP_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_SERP_MASK) | (((uint32_t)val) << SVDO_SCFG_SERP_SHIFT))

#define  SVDO_SCFG_BOT2TOP_MASK 0x1000000
#define  SVDO_SCFG_BOT2TOP_SHIFT 24
#define  SVDO_SCFG_BOT2TOP_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_BOT2TOP_MASK) >> SVDO_SCFG_BOT2TOP_SHIFT)
#define  SVDO_SCFG_BOT2TOP_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_BOT2TOP_MASK) | (((uint32_t)val) << SVDO_SCFG_BOT2TOP_SHIFT))

#define  SVDO_SCFG_RESERVED3_MASK 0xc00000
#define  SVDO_SCFG_RESERVED3_SHIFT 22
#define  SVDO_SCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESERVED3_MASK) >> SVDO_SCFG_RESERVED3_SHIFT)
#define  SVDO_SCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESERVED3_MASK) | (((uint32_t)val) << SVDO_SCFG_RESERVED3_SHIFT))

#define  SVDO_SCFG_BPP_MASK 0x300000
#define  SVDO_SCFG_BPP_SHIFT 20
#define  SVDO_SCFG_BPP_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_BPP_MASK) >> SVDO_SCFG_BPP_SHIFT)
#define  SVDO_SCFG_BPP_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_BPP_MASK) | (((uint32_t)val) << SVDO_SCFG_BPP_SHIFT))

#define  SVDO_SCFG_RESERVED4_MASK 0x80000
#define  SVDO_SCFG_RESERVED4_SHIFT 19
#define  SVDO_SCFG_RESERVED4_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESERVED4_MASK) >> SVDO_SCFG_RESERVED4_SHIFT)
#define  SVDO_SCFG_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESERVED4_MASK) | (((uint32_t)val) << SVDO_SCFG_RESERVED4_SHIFT))

#define  SVDO_SCFG_LINESKIP_MASK 0x70000
#define  SVDO_SCFG_LINESKIP_SHIFT 16
#define  SVDO_SCFG_LINESKIP_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_LINESKIP_MASK) >> SVDO_SCFG_LINESKIP_SHIFT)
#define  SVDO_SCFG_LINESKIP_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_LINESKIP_MASK) | (((uint32_t)val) << SVDO_SCFG_LINESKIP_SHIFT))

#define  SVDO_SCFG_PIXDOUBLE_MASK 0x8000
#define  SVDO_SCFG_PIXDOUBLE_SHIFT 15
#define  SVDO_SCFG_PIXDOUBLE_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_PIXDOUBLE_MASK) >> SVDO_SCFG_PIXDOUBLE_SHIFT)
#define  SVDO_SCFG_PIXDOUBLE_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_PIXDOUBLE_MASK) | (((uint32_t)val) << SVDO_SCFG_PIXDOUBLE_SHIFT))

#define  SVDO_SCFG_PCODEREF_MASK 0x4000
#define  SVDO_SCFG_PCODEREF_SHIFT 14
#define  SVDO_SCFG_PCODEREF_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_PCODEREF_MASK) >> SVDO_SCFG_PCODEREF_SHIFT)
#define  SVDO_SCFG_PCODEREF_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_PCODEREF_MASK) | (((uint32_t)val) << SVDO_SCFG_PCODEREF_SHIFT))

#define  SVDO_SCFG_INITJUST_MASK 0x2000
#define  SVDO_SCFG_INITJUST_SHIFT 13
#define  SVDO_SCFG_INITJUST_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_INITJUST_MASK) >> SVDO_SCFG_INITJUST_SHIFT)
#define  SVDO_SCFG_INITJUST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_INITJUST_MASK) | (((uint32_t)val) << SVDO_SCFG_INITJUST_SHIFT))

#define  SVDO_SCFG_FORCEDJUST_MASK 0x1000
#define  SVDO_SCFG_FORCEDJUST_SHIFT 12
#define  SVDO_SCFG_FORCEDJUST_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_FORCEDJUST_MASK) >> SVDO_SCFG_FORCEDJUST_SHIFT)
#define  SVDO_SCFG_FORCEDJUST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_FORCEDJUST_MASK) | (((uint32_t)val) << SVDO_SCFG_FORCEDJUST_SHIFT))

#define  SVDO_SCFG_RESERVED5_MASK 0x800
#define  SVDO_SCFG_RESERVED5_SHIFT 11
#define  SVDO_SCFG_RESERVED5_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESERVED5_MASK) >> SVDO_SCFG_RESERVED5_SHIFT)
#define  SVDO_SCFG_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESERVED5_MASK) | (((uint32_t)val) << SVDO_SCFG_RESERVED5_SHIFT))

#define  SVDO_SCFG_DATASOURCE_MASK 0x400
#define  SVDO_SCFG_DATASOURCE_SHIFT 10
#define  SVDO_SCFG_DATASOURCE_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_DATASOURCE_MASK) >> SVDO_SCFG_DATASOURCE_SHIFT)
#define  SVDO_SCFG_DATASOURCE_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_DATASOURCE_MASK) | (((uint32_t)val) << SVDO_SCFG_DATASOURCE_SHIFT))

#define  SVDO_SCFG_TOGGLEJUST_MASK 0x300
#define  SVDO_SCFG_TOGGLEJUST_SHIFT 8
#define  SVDO_SCFG_TOGGLEJUST_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_TOGGLEJUST_MASK) >> SVDO_SCFG_TOGGLEJUST_SHIFT)
#define  SVDO_SCFG_TOGGLEJUST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_TOGGLEJUST_MASK) | (((uint32_t)val) << SVDO_SCFG_TOGGLEJUST_SHIFT))

#define  SVDO_SCFG_RESERVED6_MASK 0xc0
#define  SVDO_SCFG_RESERVED6_SHIFT 6
#define  SVDO_SCFG_RESERVED6_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESERVED6_MASK) >> SVDO_SCFG_RESERVED6_SHIFT)
#define  SVDO_SCFG_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESERVED6_MASK) | (((uint32_t)val) << SVDO_SCFG_RESERVED6_SHIFT))

#define  SVDO_SCFG_LINEREPL_MASK 0x30
#define  SVDO_SCFG_LINEREPL_SHIFT 4
#define  SVDO_SCFG_LINEREPL_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_LINEREPL_MASK) >> SVDO_SCFG_LINEREPL_SHIFT)
#define  SVDO_SCFG_LINEREPL_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_LINEREPL_MASK) | (((uint32_t)val) << SVDO_SCFG_LINEREPL_SHIFT))

#define  SVDO_SCFG_RESERVED7_MASK 0xc
#define  SVDO_SCFG_RESERVED7_SHIFT 2
#define  SVDO_SCFG_RESERVED7_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESERVED7_MASK) >> SVDO_SCFG_RESERVED7_SHIFT)
#define  SVDO_SCFG_RESERVED7_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESERVED7_MASK) | (((uint32_t)val) << SVDO_SCFG_RESERVED7_SHIFT))

#define  SVDO_SCFG_RESETDMA_MASK 0x2
#define  SVDO_SCFG_RESETDMA_SHIFT 1
#define  SVDO_SCFG_RESETDMA_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_RESETDMA_MASK) >> SVDO_SCFG_RESETDMA_SHIFT)
#define  SVDO_SCFG_RESETDMA_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_RESETDMA_MASK) | (((uint32_t)val) << SVDO_SCFG_RESETDMA_SHIFT))

#define  SVDO_SCFG_SVDOENABLE_MASK 0x1
#define  SVDO_SCFG_SVDOENABLE_SHIFT 0
#define  SVDO_SCFG_SVDOENABLE_MASK_SHIFT(reg) (((reg) & SVDO_SCFG_SVDOENABLE_MASK) >> SVDO_SCFG_SVDOENABLE_SHIFT)
#define  SVDO_SCFG_SVDOENABLE_REPLACE_VAL(reg,val) (((reg) & ~SVDO_SCFG_SVDOENABLE_MASK) | (((uint32_t)val) << SVDO_SCFG_SVDOENABLE_SHIFT))

//====================================================================
//Register: Video Status Register (STAT)
/** \brief Contains DMA control word status along with FSM states from various state machines in the module for debug purposes only.*/
//====================================================================

#define  SVDO_STAT_SKIPST_MASK 0xe0000000
#define  SVDO_STAT_SKIPST_SHIFT 29
#define  SVDO_STAT_SKIPST_MASK_SHIFT(reg) (((reg) & SVDO_STAT_SKIPST_MASK) >> SVDO_STAT_SKIPST_SHIFT)
#define  SVDO_STAT_SKIPST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_SKIPST_MASK) | (((uint32_t)val) << SVDO_STAT_SKIPST_SHIFT))

#define  SVDO_STAT_PIXPIPEST_MASK 0x1c000000
#define  SVDO_STAT_PIXPIPEST_SHIFT 26
#define  SVDO_STAT_PIXPIPEST_MASK_SHIFT(reg) (((reg) & SVDO_STAT_PIXPIPEST_MASK) >> SVDO_STAT_PIXPIPEST_SHIFT)
#define  SVDO_STAT_PIXPIPEST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_PIXPIPEST_MASK) | (((uint32_t)val) << SVDO_STAT_PIXPIPEST_SHIFT))

#define  SVDO_STAT_DATAFIFOENT_MASK 0x3e00000
#define  SVDO_STAT_DATAFIFOENT_SHIFT 21
#define  SVDO_STAT_DATAFIFOENT_MASK_SHIFT(reg) (((reg) & SVDO_STAT_DATAFIFOENT_MASK) >> SVDO_STAT_DATAFIFOENT_SHIFT)
#define  SVDO_STAT_DATAFIFOENT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_DATAFIFOENT_MASK) | (((uint32_t)val) << SVDO_STAT_DATAFIFOENT_SHIFT))

#define  SVDO_STAT_REFIFOENT_MASK 0x1f0000
#define  SVDO_STAT_REFIFOENT_SHIFT 16
#define  SVDO_STAT_REFIFOENT_MASK_SHIFT(reg) (((reg) & SVDO_STAT_REFIFOENT_MASK) >> SVDO_STAT_REFIFOENT_SHIFT)
#define  SVDO_STAT_REFIFOENT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_REFIFOENT_MASK) | (((uint32_t)val) << SVDO_STAT_REFIFOENT_SHIFT))

#define  SVDO_STAT_ADDRST_MASK 0xc000
#define  SVDO_STAT_ADDRST_SHIFT 14
#define  SVDO_STAT_ADDRST_MASK_SHIFT(reg) (((reg) & SVDO_STAT_ADDRST_MASK) >> SVDO_STAT_ADDRST_SHIFT)
#define  SVDO_STAT_ADDRST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_ADDRST_MASK) | (((uint32_t)val) << SVDO_STAT_ADDRST_SHIFT))

#define  SVDO_STAT_DATAST_MASK 0x3800
#define  SVDO_STAT_DATAST_SHIFT 11
#define  SVDO_STAT_DATAST_MASK_SHIFT(reg) (((reg) & SVDO_STAT_DATAST_MASK) >> SVDO_STAT_DATAST_SHIFT)
#define  SVDO_STAT_DATAST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_DATAST_MASK) | (((uint32_t)val) << SVDO_STAT_DATAST_SHIFT))

#define  SVDO_STAT_CMDST_MASK 0x700
#define  SVDO_STAT_CMDST_SHIFT 8
#define  SVDO_STAT_CMDST_MASK_SHIFT(reg) (((reg) & SVDO_STAT_CMDST_MASK) >> SVDO_STAT_CMDST_SHIFT)
#define  SVDO_STAT_CMDST_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_CMDST_MASK) | (((uint32_t)val) << SVDO_STAT_CMDST_SHIFT))

#define  SVDO_STAT_DMAOWN_MASK 0x80
#define  SVDO_STAT_DMAOWN_SHIFT 7
#define  SVDO_STAT_DMAOWN_MASK_SHIFT(reg) (((reg) & SVDO_STAT_DMAOWN_MASK) >> SVDO_STAT_DMAOWN_SHIFT)
#define  SVDO_STAT_DMAOWN_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_DMAOWN_MASK) | (((uint32_t)val) << SVDO_STAT_DMAOWN_SHIFT))

#define  SVDO_STAT_DMAACTIVE_MASK 0x40
#define  SVDO_STAT_DMAACTIVE_SHIFT 6
#define  SVDO_STAT_DMAACTIVE_MASK_SHIFT(reg) (((reg) & SVDO_STAT_DMAACTIVE_MASK) >> SVDO_STAT_DMAACTIVE_SHIFT)
#define  SVDO_STAT_DMAACTIVE_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_DMAACTIVE_MASK) | (((uint32_t)val) << SVDO_STAT_DMAACTIVE_SHIFT))

#define  SVDO_STAT_LASTBUFFER_MASK 0x20
#define  SVDO_STAT_LASTBUFFER_SHIFT 5
#define  SVDO_STAT_LASTBUFFER_MASK_SHIFT(reg) (((reg) & SVDO_STAT_LASTBUFFER_MASK) >> SVDO_STAT_LASTBUFFER_SHIFT)
#define  SVDO_STAT_LASTBUFFER_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_LASTBUFFER_MASK) | (((uint32_t)val) << SVDO_STAT_LASTBUFFER_SHIFT))

#define  SVDO_STAT_FIRSTBUFFER_MASK 0x10
#define  SVDO_STAT_FIRSTBUFFER_SHIFT 4
#define  SVDO_STAT_FIRSTBUFFER_MASK_SHIFT(reg) (((reg) & SVDO_STAT_FIRSTBUFFER_MASK) >> SVDO_STAT_FIRSTBUFFER_SHIFT)
#define  SVDO_STAT_FIRSTBUFFER_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_FIRSTBUFFER_MASK) | (((uint32_t)val) << SVDO_STAT_FIRSTBUFFER_SHIFT))

#define  SVDO_STAT_RTOL_MASK 0x8
#define  SVDO_STAT_RTOL_SHIFT 3
#define  SVDO_STAT_RTOL_MASK_SHIFT(reg) (((reg) & SVDO_STAT_RTOL_MASK) >> SVDO_STAT_RTOL_SHIFT)
#define  SVDO_STAT_RTOL_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_RTOL_MASK) | (((uint32_t)val) << SVDO_STAT_RTOL_SHIFT))

#define  SVDO_STAT_BLANK_MASK 0x4
#define  SVDO_STAT_BLANK_SHIFT 2
#define  SVDO_STAT_BLANK_MASK_SHIFT(reg) (((reg) & SVDO_STAT_BLANK_MASK) >> SVDO_STAT_BLANK_SHIFT)
#define  SVDO_STAT_BLANK_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_BLANK_MASK) | (((uint32_t)val) << SVDO_STAT_BLANK_SHIFT))

#define  SVDO_STAT_INTONFINISH_MASK 0x2
#define  SVDO_STAT_INTONFINISH_SHIFT 1
#define  SVDO_STAT_INTONFINISH_MASK_SHIFT(reg) (((reg) & SVDO_STAT_INTONFINISH_MASK) >> SVDO_STAT_INTONFINISH_SHIFT)
#define  SVDO_STAT_INTONFINISH_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_INTONFINISH_MASK) | (((uint32_t)val) << SVDO_STAT_INTONFINISH_SHIFT))

#define  SVDO_STAT_STOPONFINISH_MASK 0x1
#define  SVDO_STAT_STOPONFINISH_SHIFT 0
#define  SVDO_STAT_STOPONFINISH_MASK_SHIFT(reg) (((reg) & SVDO_STAT_STOPONFINISH_MASK) >> SVDO_STAT_STOPONFINISH_SHIFT)
#define  SVDO_STAT_STOPONFINISH_REPLACE_VAL(reg,val) (((reg) & ~SVDO_STAT_STOPONFINISH_MASK) | (((uint32_t)val) << SVDO_STAT_STOPONFINISH_SHIFT))

//====================================================================
//Register: Video DMA Descriptor Write Address Register (VDWR)
/** \brief These bits are always 0, to align pointer to a 16B address.*/
//====================================================================

#define  SVDO_VDWR_WRDESCADDR_MASK 0xfffffff0
#define  SVDO_VDWR_WRDESCADDR_SHIFT 4
#define  SVDO_VDWR_WRDESCADDR_MASK_SHIFT(reg) (((reg) & SVDO_VDWR_WRDESCADDR_MASK) >> SVDO_VDWR_WRDESCADDR_SHIFT)
#define  SVDO_VDWR_WRDESCADDR_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDWR_WRDESCADDR_MASK) | (((uint32_t)val) << SVDO_VDWR_WRDESCADDR_SHIFT))

#define  SVDO_VDWR_RESERVED1_MASK 0xf
#define  SVDO_VDWR_RESERVED1_SHIFT 0
#define  SVDO_VDWR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VDWR_RESERVED1_MASK) >> SVDO_VDWR_RESERVED1_SHIFT)
#define  SVDO_VDWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDWR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VDWR_RESERVED1_SHIFT))

//====================================================================
//Register: Video DMA Current Descriptor Address Register (VDAR)
/** \brief Indicates the address of the current descriptor*/
//====================================================================

#define  SVDO_VDAR_CURRDESCADDR_MASK 0xfffffff0
#define  SVDO_VDAR_CURRDESCADDR_SHIFT 4
#define  SVDO_VDAR_CURRDESCADDR_MASK_SHIFT(reg) (((reg) & SVDO_VDAR_CURRDESCADDR_MASK) >> SVDO_VDAR_CURRDESCADDR_SHIFT)
#define  SVDO_VDAR_CURRDESCADDR_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDAR_CURRDESCADDR_MASK) | (((uint32_t)val) << SVDO_VDAR_CURRDESCADDR_SHIFT))

#define  SVDO_VDAR_RESERVED1_MASK 0xf
#define  SVDO_VDAR_RESERVED1_SHIFT 0
#define  SVDO_VDAR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VDAR_RESERVED1_MASK) >> SVDO_VDAR_RESERVED1_SHIFT)
#define  SVDO_VDAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDAR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VDAR_RESERVED1_SHIFT))

//====================================================================
//Register: Video DMA Bytes Left to Transfer Register (VDBR)
/** \brief Indicates the number of bytes left to transfer in the current line. The total number of bytes remaining in the buffer can be calculated as follows:
      <br>
        BufferBytesRemaining = (LinesRemaining*LineWidth) + CurrentLineBytesRemaining*/
//====================================================================

#define  SVDO_VDBR_RESERVED1_MASK 0xffff0000
#define  SVDO_VDBR_RESERVED1_SHIFT 16
#define  SVDO_VDBR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VDBR_RESERVED1_MASK) >> SVDO_VDBR_RESERVED1_SHIFT)
#define  SVDO_VDBR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDBR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VDBR_RESERVED1_SHIFT))

#define  SVDO_VDBR_BYTESLEFT_MASK 0xffff
#define  SVDO_VDBR_BYTESLEFT_SHIFT 0
#define  SVDO_VDBR_BYTESLEFT_MASK_SHIFT(reg) (((reg) & SVDO_VDBR_BYTESLEFT_MASK) >> SVDO_VDBR_BYTESLEFT_SHIFT)
#define  SVDO_VDBR_BYTESLEFT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDBR_BYTESLEFT_MASK) | (((uint32_t)val) << SVDO_VDBR_BYTESLEFT_SHIFT))

//====================================================================
//Register: Video DMA Lines Left to Transfer Register (VDLR)
/** \brief Indicates the number of lines left to transfer in the current buffer. The total number of bytes remaining in the buffer can be calculated as follows:
      <br>
        BufferBytesRemaining = (LinesRemaining*LineWidth) + CurrentLineBytesRemaining*/
//====================================================================

#define  SVDO_VDLR_RESERVED1_MASK 0xffffc000
#define  SVDO_VDLR_RESERVED1_SHIFT 14
#define  SVDO_VDLR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VDLR_RESERVED1_MASK) >> SVDO_VDLR_RESERVED1_SHIFT)
#define  SVDO_VDLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDLR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VDLR_RESERVED1_SHIFT))

#define  SVDO_VDLR_LINESLEFT_MASK 0x3fff
#define  SVDO_VDLR_LINESLEFT_SHIFT 0
#define  SVDO_VDLR_LINESLEFT_MASK_SHIFT(reg) (((reg) & SVDO_VDLR_LINESLEFT_MASK) >> SVDO_VDLR_LINESLEFT_SHIFT)
#define  SVDO_VDLR_LINESLEFT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDLR_LINESLEFT_MASK) | (((uint32_t)val) << SVDO_VDLR_LINESLEFT_SHIFT))

//====================================================================
//Register: Video DMA Next Descriptor Address Register (VDNR)
/** \brief Indicates the address of the next DMA descriptor*/
//====================================================================

#define  SVDO_VDNR_DESCADDR_MASK 0xfffffff0
#define  SVDO_VDNR_DESCADDR_SHIFT 4
#define  SVDO_VDNR_DESCADDR_MASK_SHIFT(reg) (((reg) & SVDO_VDNR_DESCADDR_MASK) >> SVDO_VDNR_DESCADDR_SHIFT)
#define  SVDO_VDNR_DESCADDR_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDNR_DESCADDR_MASK) | (((uint32_t)val) << SVDO_VDNR_DESCADDR_SHIFT))

#define  SVDO_VDNR_RESERVED1_MASK 0xf
#define  SVDO_VDNR_RESERVED1_SHIFT 0
#define  SVDO_VDNR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VDNR_RESERVED1_MASK) >> SVDO_VDNR_RESERVED1_SHIFT)
#define  SVDO_VDNR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VDNR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VDNR_RESERVED1_SHIFT))

//====================================================================
//Register: Video Interrupt Enable Register (VIER)
/** \brief Video Interrupt Enable Register*/
//====================================================================

#define  SVDO_VIER_RESERVED1_MASK 0xffffff00
#define  SVDO_VIER_RESERVED1_SHIFT 8
#define  SVDO_VIER_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VIER_RESERVED1_MASK) >> SVDO_VIER_RESERVED1_SHIFT)
#define  SVDO_VIER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VIER_RESERVED1_SHIFT))

#define  SVDO_VIER_TOPERRINT_MASK 0x80
#define  SVDO_VIER_TOPERRINT_SHIFT 7
#define  SVDO_VIER_TOPERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_TOPERRINT_MASK) >> SVDO_VIER_TOPERRINT_SHIFT)
#define  SVDO_VIER_TOPERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_TOPERRINT_MASK) | (((uint32_t)val) << SVDO_VIER_TOPERRINT_SHIFT))

#define  SVDO_VIER_BOTERRINT_MASK 0x40
#define  SVDO_VIER_BOTERRINT_SHIFT 6
#define  SVDO_VIER_BOTERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_BOTERRINT_MASK) >> SVDO_VIER_BOTERRINT_SHIFT)
#define  SVDO_VIER_BOTERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_BOTERRINT_MASK) | (((uint32_t)val) << SVDO_VIER_BOTERRINT_SHIFT))

#define  SVDO_VIER_HSYNCERRINT_MASK 0x20
#define  SVDO_VIER_HSYNCERRINT_SHIFT 5
#define  SVDO_VIER_HSYNCERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_HSYNCERRINT_MASK) >> SVDO_VIER_HSYNCERRINT_SHIFT)
#define  SVDO_VIER_HSYNCERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_HSYNCERRINT_MASK) | (((uint32_t)val) << SVDO_VIER_HSYNCERRINT_SHIFT))

#define  SVDO_VIER_BENDERUNDERRUNINT_MASK 0x10
#define  SVDO_VIER_BENDERUNDERRUNINT_SHIFT 4
#define  SVDO_VIER_BENDERUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_BENDERUNDERRUNINT_MASK) >> SVDO_VIER_BENDERUNDERRUNINT_SHIFT)
#define  SVDO_VIER_BENDERUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_BENDERUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIER_BENDERUNDERRUNINT_SHIFT))

#define  SVDO_VIER_EOIINT_MASK 0x8
#define  SVDO_VIER_EOIINT_SHIFT 3
#define  SVDO_VIER_EOIINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_EOIINT_MASK) >> SVDO_VIER_EOIINT_SHIFT)
#define  SVDO_VIER_EOIINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_EOIINT_MASK) | (((uint32_t)val) << SVDO_VIER_EOIINT_SHIFT))

#define  SVDO_VIER_PTUNDERRUNINT_MASK 0x4
#define  SVDO_VIER_PTUNDERRUNINT_SHIFT 2
#define  SVDO_VIER_PTUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_PTUNDERRUNINT_MASK) >> SVDO_VIER_PTUNDERRUNINT_SHIFT)
#define  SVDO_VIER_PTUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_PTUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIER_PTUNDERRUNINT_SHIFT))

#define  SVDO_VIER_DESCINT_MASK 0x2
#define  SVDO_VIER_DESCINT_SHIFT 1
#define  SVDO_VIER_DESCINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_DESCINT_MASK) >> SVDO_VIER_DESCINT_SHIFT)
#define  SVDO_VIER_DESCINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_DESCINT_MASK) | (((uint32_t)val) << SVDO_VIER_DESCINT_SHIFT))

#define  SVDO_VIER_OWNINT_MASK 0x1
#define  SVDO_VIER_OWNINT_SHIFT 0
#define  SVDO_VIER_OWNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIER_OWNINT_MASK) >> SVDO_VIER_OWNINT_SHIFT)
#define  SVDO_VIER_OWNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIER_OWNINT_MASK) | (((uint32_t)val) << SVDO_VIER_OWNINT_SHIFT))

//====================================================================
//Register: Video Interrupt Pending Register (VIPR)
/** \brief Video Interrupt Pending Register*/
//====================================================================

#define  SVDO_VIPR_RESERVED1_MASK 0xffffff00
#define  SVDO_VIPR_RESERVED1_SHIFT 8
#define  SVDO_VIPR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_RESERVED1_MASK) >> SVDO_VIPR_RESERVED1_SHIFT)
#define  SVDO_VIPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VIPR_RESERVED1_SHIFT))

#define  SVDO_VIPR_TOPERRINT_MASK 0x80
#define  SVDO_VIPR_TOPERRINT_SHIFT 7
#define  SVDO_VIPR_TOPERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_TOPERRINT_MASK) >> SVDO_VIPR_TOPERRINT_SHIFT)
#define  SVDO_VIPR_TOPERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_TOPERRINT_MASK) | (((uint32_t)val) << SVDO_VIPR_TOPERRINT_SHIFT))

#define  SVDO_VIPR_BOTERRINT_MASK 0x40
#define  SVDO_VIPR_BOTERRINT_SHIFT 6
#define  SVDO_VIPR_BOTERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_BOTERRINT_MASK) >> SVDO_VIPR_BOTERRINT_SHIFT)
#define  SVDO_VIPR_BOTERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_BOTERRINT_MASK) | (((uint32_t)val) << SVDO_VIPR_BOTERRINT_SHIFT))

#define  SVDO_VIPR_HSYNCERRINT_MASK 0x20
#define  SVDO_VIPR_HSYNCERRINT_SHIFT 5
#define  SVDO_VIPR_HSYNCERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_HSYNCERRINT_MASK) >> SVDO_VIPR_HSYNCERRINT_SHIFT)
#define  SVDO_VIPR_HSYNCERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_HSYNCERRINT_MASK) | (((uint32_t)val) << SVDO_VIPR_HSYNCERRINT_SHIFT))

#define  SVDO_VIPR_BENDERUNDERRUNINT_MASK 0x10
#define  SVDO_VIPR_BENDERUNDERRUNINT_SHIFT 4
#define  SVDO_VIPR_BENDERUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_BENDERUNDERRUNINT_MASK) >> SVDO_VIPR_BENDERUNDERRUNINT_SHIFT)
#define  SVDO_VIPR_BENDERUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_BENDERUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIPR_BENDERUNDERRUNINT_SHIFT))

#define  SVDO_VIPR_EOIINT_MASK 0x8
#define  SVDO_VIPR_EOIINT_SHIFT 3
#define  SVDO_VIPR_EOIINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_EOIINT_MASK) >> SVDO_VIPR_EOIINT_SHIFT)
#define  SVDO_VIPR_EOIINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_EOIINT_MASK) | (((uint32_t)val) << SVDO_VIPR_EOIINT_SHIFT))

#define  SVDO_VIPR_PTUNDERRUNINT_MASK 0x4
#define  SVDO_VIPR_PTUNDERRUNINT_SHIFT 2
#define  SVDO_VIPR_PTUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_PTUNDERRUNINT_MASK) >> SVDO_VIPR_PTUNDERRUNINT_SHIFT)
#define  SVDO_VIPR_PTUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_PTUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIPR_PTUNDERRUNINT_SHIFT))

#define  SVDO_VIPR_DESCINT_MASK 0x2
#define  SVDO_VIPR_DESCINT_SHIFT 1
#define  SVDO_VIPR_DESCINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_DESCINT_MASK) >> SVDO_VIPR_DESCINT_SHIFT)
#define  SVDO_VIPR_DESCINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_DESCINT_MASK) | (((uint32_t)val) << SVDO_VIPR_DESCINT_SHIFT))

#define  SVDO_VIPR_OWNINT_MASK 0x1
#define  SVDO_VIPR_OWNINT_SHIFT 0
#define  SVDO_VIPR_OWNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIPR_OWNINT_MASK) >> SVDO_VIPR_OWNINT_SHIFT)
#define  SVDO_VIPR_OWNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIPR_OWNINT_MASK) | (((uint32_t)val) << SVDO_VIPR_OWNINT_SHIFT))

//====================================================================
//Register: Video Interrupt Acknowledge Register (VIAR)
/** \brief Video Interrupt Acknowledge Register*/
//====================================================================

#define  SVDO_VIAR_RESERVED1_MASK 0xffffff00
#define  SVDO_VIAR_RESERVED1_SHIFT 8
#define  SVDO_VIAR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_RESERVED1_MASK) >> SVDO_VIAR_RESERVED1_SHIFT)
#define  SVDO_VIAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VIAR_RESERVED1_SHIFT))

#define  SVDO_VIAR_TOPERRINT_MASK 0x80
#define  SVDO_VIAR_TOPERRINT_SHIFT 7
#define  SVDO_VIAR_TOPERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_TOPERRINT_MASK) >> SVDO_VIAR_TOPERRINT_SHIFT)
#define  SVDO_VIAR_TOPERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_TOPERRINT_MASK) | (((uint32_t)val) << SVDO_VIAR_TOPERRINT_SHIFT))

#define  SVDO_VIAR_BOTERRINT_MASK 0x40
#define  SVDO_VIAR_BOTERRINT_SHIFT 6
#define  SVDO_VIAR_BOTERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_BOTERRINT_MASK) >> SVDO_VIAR_BOTERRINT_SHIFT)
#define  SVDO_VIAR_BOTERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_BOTERRINT_MASK) | (((uint32_t)val) << SVDO_VIAR_BOTERRINT_SHIFT))

#define  SVDO_VIAR_HSYNCERRINT_MASK 0x20
#define  SVDO_VIAR_HSYNCERRINT_SHIFT 5
#define  SVDO_VIAR_HSYNCERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_HSYNCERRINT_MASK) >> SVDO_VIAR_HSYNCERRINT_SHIFT)
#define  SVDO_VIAR_HSYNCERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_HSYNCERRINT_MASK) | (((uint32_t)val) << SVDO_VIAR_HSYNCERRINT_SHIFT))

#define  SVDO_VIAR_BENDERUNDERRUNINT_MASK 0x10
#define  SVDO_VIAR_BENDERUNDERRUNINT_SHIFT 4
#define  SVDO_VIAR_BENDERUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_BENDERUNDERRUNINT_MASK) >> SVDO_VIAR_BENDERUNDERRUNINT_SHIFT)
#define  SVDO_VIAR_BENDERUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_BENDERUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIAR_BENDERUNDERRUNINT_SHIFT))

#define  SVDO_VIAR_EOIINT_MASK 0x8
#define  SVDO_VIAR_EOIINT_SHIFT 3
#define  SVDO_VIAR_EOIINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_EOIINT_MASK) >> SVDO_VIAR_EOIINT_SHIFT)
#define  SVDO_VIAR_EOIINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_EOIINT_MASK) | (((uint32_t)val) << SVDO_VIAR_EOIINT_SHIFT))

#define  SVDO_VIAR_PTUNDERRUNINT_MASK 0x4
#define  SVDO_VIAR_PTUNDERRUNINT_SHIFT 2
#define  SVDO_VIAR_PTUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_PTUNDERRUNINT_MASK) >> SVDO_VIAR_PTUNDERRUNINT_SHIFT)
#define  SVDO_VIAR_PTUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_PTUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIAR_PTUNDERRUNINT_SHIFT))

#define  SVDO_VIAR_DESCINT_MASK 0x2
#define  SVDO_VIAR_DESCINT_SHIFT 1
#define  SVDO_VIAR_DESCINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_DESCINT_MASK) >> SVDO_VIAR_DESCINT_SHIFT)
#define  SVDO_VIAR_DESCINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_DESCINT_MASK) | (((uint32_t)val) << SVDO_VIAR_DESCINT_SHIFT))

#define  SVDO_VIAR_OWNINT_MASK 0x1
#define  SVDO_VIAR_OWNINT_SHIFT 0
#define  SVDO_VIAR_OWNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIAR_OWNINT_MASK) >> SVDO_VIAR_OWNINT_SHIFT)
#define  SVDO_VIAR_OWNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIAR_OWNINT_MASK) | (((uint32_t)val) << SVDO_VIAR_OWNINT_SHIFT))

//====================================================================
//Register: Video Interrupt Force Register (VIFR)
/** \brief Video Interrupt Test Register*/
//====================================================================

#define  SVDO_VIFR_RESERVED1_MASK 0xffffff00
#define  SVDO_VIFR_RESERVED1_SHIFT 8
#define  SVDO_VIFR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_RESERVED1_MASK) >> SVDO_VIFR_RESERVED1_SHIFT)
#define  SVDO_VIFR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VIFR_RESERVED1_SHIFT))

#define  SVDO_VIFR_TOPERRINT_MASK 0x80
#define  SVDO_VIFR_TOPERRINT_SHIFT 7
#define  SVDO_VIFR_TOPERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_TOPERRINT_MASK) >> SVDO_VIFR_TOPERRINT_SHIFT)
#define  SVDO_VIFR_TOPERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_TOPERRINT_MASK) | (((uint32_t)val) << SVDO_VIFR_TOPERRINT_SHIFT))

#define  SVDO_VIFR_BOTERRINT_MASK 0x40
#define  SVDO_VIFR_BOTERRINT_SHIFT 6
#define  SVDO_VIFR_BOTERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_BOTERRINT_MASK) >> SVDO_VIFR_BOTERRINT_SHIFT)
#define  SVDO_VIFR_BOTERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_BOTERRINT_MASK) | (((uint32_t)val) << SVDO_VIFR_BOTERRINT_SHIFT))

#define  SVDO_VIFR_HSYNCERRINT_MASK 0x20
#define  SVDO_VIFR_HSYNCERRINT_SHIFT 5
#define  SVDO_VIFR_HSYNCERRINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_HSYNCERRINT_MASK) >> SVDO_VIFR_HSYNCERRINT_SHIFT)
#define  SVDO_VIFR_HSYNCERRINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_HSYNCERRINT_MASK) | (((uint32_t)val) << SVDO_VIFR_HSYNCERRINT_SHIFT))

#define  SVDO_VIFR_BENDERUNDERRUNINT_MASK 0x10
#define  SVDO_VIFR_BENDERUNDERRUNINT_SHIFT 4
#define  SVDO_VIFR_BENDERUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_BENDERUNDERRUNINT_MASK) >> SVDO_VIFR_BENDERUNDERRUNINT_SHIFT)
#define  SVDO_VIFR_BENDERUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_BENDERUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIFR_BENDERUNDERRUNINT_SHIFT))

#define  SVDO_VIFR_EOIINT_MASK 0x8
#define  SVDO_VIFR_EOIINT_SHIFT 3
#define  SVDO_VIFR_EOIINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_EOIINT_MASK) >> SVDO_VIFR_EOIINT_SHIFT)
#define  SVDO_VIFR_EOIINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_EOIINT_MASK) | (((uint32_t)val) << SVDO_VIFR_EOIINT_SHIFT))

#define  SVDO_VIFR_PTUNDERRUNINT_MASK 0x4
#define  SVDO_VIFR_PTUNDERRUNINT_SHIFT 2
#define  SVDO_VIFR_PTUNDERRUNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_PTUNDERRUNINT_MASK) >> SVDO_VIFR_PTUNDERRUNINT_SHIFT)
#define  SVDO_VIFR_PTUNDERRUNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_PTUNDERRUNINT_MASK) | (((uint32_t)val) << SVDO_VIFR_PTUNDERRUNINT_SHIFT))

#define  SVDO_VIFR_DESCINT_MASK 0x2
#define  SVDO_VIFR_DESCINT_SHIFT 1
#define  SVDO_VIFR_DESCINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_DESCINT_MASK) >> SVDO_VIFR_DESCINT_SHIFT)
#define  SVDO_VIFR_DESCINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_DESCINT_MASK) | (((uint32_t)val) << SVDO_VIFR_DESCINT_SHIFT))

#define  SVDO_VIFR_OWNINT_MASK 0x1
#define  SVDO_VIFR_OWNINT_SHIFT 0
#define  SVDO_VIFR_OWNINT_MASK_SHIFT(reg) (((reg) & SVDO_VIFR_OWNINT_MASK) >> SVDO_VIFR_OWNINT_SHIFT)
#define  SVDO_VIFR_OWNINT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VIFR_OWNINT_MASK) | (((uint32_t)val) << SVDO_VIFR_OWNINT_SHIFT))

//====================================================================
//Register: Video Line Width Register (VLWR)
/** \brief This register specifies the number of bytes in a single video line.
        The width must be a multiple of 4 bytes and must be at least one
        burst long. For example, if the burst size is set to 8 words, the
        line width must be at least 32 bytes. The block is limited to 64K
        bytes per line. This limit will only allow print widths of up to:
      <br>
          600 dpi: max print width = 218.4 inches
      <br>
          1200 dpi: max print width = 109.2 inches
      <br>
          2400 dpi: max print width = 54.6 inches*/
//====================================================================

#define  SVDO_VLWR_RESERVED1_MASK 0xffff0000
#define  SVDO_VLWR_RESERVED1_SHIFT 16
#define  SVDO_VLWR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VLWR_RESERVED1_MASK) >> SVDO_VLWR_RESERVED1_SHIFT)
#define  SVDO_VLWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VLWR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VLWR_RESERVED1_SHIFT))

#define  SVDO_VLWR_LINEWID_MASK 0xfffc
#define  SVDO_VLWR_LINEWID_SHIFT 2
#define  SVDO_VLWR_LINEWID_MASK_SHIFT(reg) (((reg) & SVDO_VLWR_LINEWID_MASK) >> SVDO_VLWR_LINEWID_SHIFT)
#define  SVDO_VLWR_LINEWID_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VLWR_LINEWID_MASK) | (((uint32_t)val) << SVDO_VLWR_LINEWID_SHIFT))

#define  SVDO_VLWR_RESERVED2_MASK 0x3
#define  SVDO_VLWR_RESERVED2_SHIFT 0
#define  SVDO_VLWR_RESERVED2_MASK_SHIFT(reg) (((reg) & SVDO_VLWR_RESERVED2_MASK) >> SVDO_VLWR_RESERVED2_SHIFT)
#define  SVDO_VLWR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VLWR_RESERVED2_MASK) | (((uint32_t)val) << SVDO_VLWR_RESERVED2_SHIFT))

//====================================================================
//Register: Video Toner Count Accumulation Register (VTCA)
/** \brief Accumulation of Pixel weights on the last page printed. The MSbits of the
        accumulator value are read back here. The value is normalized by 2^4
        (size of weight).*/
//====================================================================

#define  SVDO_VTCA_TCACCUM_MASK 0xffffffff
#define  SVDO_VTCA_TCACCUM_SHIFT 0
#define  SVDO_VTCA_TCACCUM_MASK_SHIFT(reg) (((reg) & SVDO_VTCA_TCACCUM_MASK) >> SVDO_VTCA_TCACCUM_SHIFT)
#define  SVDO_VTCA_TCACCUM_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VTCA_TCACCUM_MASK) | (((uint32_t)val) << SVDO_VTCA_TCACCUM_SHIFT))

//====================================================================
//Register: Video Toner Count Non-Blank Pixels Register (VTCP)
/** \brief Number of Non-Blank Pixels on the last page printed.*/
//====================================================================

#define  SVDO_VTCP_TCNONBLANKPIX_MASK 0xffffffff
#define  SVDO_VTCP_TCNONBLANKPIX_SHIFT 0
#define  SVDO_VTCP_TCNONBLANKPIX_MASK_SHIFT(reg) (((reg) & SVDO_VTCP_TCNONBLANKPIX_MASK) >> SVDO_VTCP_TCNONBLANKPIX_SHIFT)
#define  SVDO_VTCP_TCNONBLANKPIX_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VTCP_TCNONBLANKPIX_MASK) | (((uint32_t)val) << SVDO_VTCP_TCNONBLANKPIX_SHIFT))

//====================================================================
//Register: Video DMA Source Address Register (VSAR)
/** \brief Indicates the source address in the current DMA descriptor*/
//====================================================================

#define  SVDO_VSAR_SOURCEADDR_MASK 0xfffffffc
#define  SVDO_VSAR_SOURCEADDR_SHIFT 2
#define  SVDO_VSAR_SOURCEADDR_MASK_SHIFT(reg) (((reg) & SVDO_VSAR_SOURCEADDR_MASK) >> SVDO_VSAR_SOURCEADDR_SHIFT)
#define  SVDO_VSAR_SOURCEADDR_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VSAR_SOURCEADDR_MASK) | (((uint32_t)val) << SVDO_VSAR_SOURCEADDR_SHIFT))

#define  SVDO_VSAR_RESERVED1_MASK 0x3
#define  SVDO_VSAR_RESERVED1_SHIFT 0
#define  SVDO_VSAR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VSAR_RESERVED1_MASK) >> SVDO_VSAR_RESERVED1_SHIFT)
#define  SVDO_VSAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VSAR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VSAR_RESERVED1_SHIFT))

//====================================================================
//Register: Video Pixel Skip Register (VPKR)
/** \brief Controls the number of pixels that may be skipped at the start and end of each line*/
//====================================================================

#define  SVDO_VPKR_LASTLINEPIXVAL_MASK 0xf0000000
#define  SVDO_VPKR_LASTLINEPIXVAL_SHIFT 28
#define  SVDO_VPKR_LASTLINEPIXVAL_MASK_SHIFT(reg) (((reg) & SVDO_VPKR_LASTLINEPIXVAL_MASK) >> SVDO_VPKR_LASTLINEPIXVAL_SHIFT)
#define  SVDO_VPKR_LASTLINEPIXVAL_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VPKR_LASTLINEPIXVAL_MASK) | (((uint32_t)val) << SVDO_VPKR_LASTLINEPIXVAL_SHIFT))

#define  SVDO_VPKR_RESERVED1_MASK 0xf000000
#define  SVDO_VPKR_RESERVED1_SHIFT 24
#define  SVDO_VPKR_RESERVED1_MASK_SHIFT(reg) (((reg) & SVDO_VPKR_RESERVED1_MASK) >> SVDO_VPKR_RESERVED1_SHIFT)
#define  SVDO_VPKR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VPKR_RESERVED1_MASK) | (((uint32_t)val) << SVDO_VPKR_RESERVED1_SHIFT))

#define  SVDO_VPKR_ENDSKIPLEN_MASK 0xfff000
#define  SVDO_VPKR_ENDSKIPLEN_SHIFT 12
#define  SVDO_VPKR_ENDSKIPLEN_MASK_SHIFT(reg) (((reg) & SVDO_VPKR_ENDSKIPLEN_MASK) >> SVDO_VPKR_ENDSKIPLEN_SHIFT)
#define  SVDO_VPKR_ENDSKIPLEN_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VPKR_ENDSKIPLEN_MASK) | (((uint32_t)val) << SVDO_VPKR_ENDSKIPLEN_SHIFT))

#define  SVDO_VPKR_STARTSKIPLEN_MASK 0xfff
#define  SVDO_VPKR_STARTSKIPLEN_SHIFT 0
#define  SVDO_VPKR_STARTSKIPLEN_MASK_SHIFT(reg) (((reg) & SVDO_VPKR_STARTSKIPLEN_MASK) >> SVDO_VPKR_STARTSKIPLEN_SHIFT)
#define  SVDO_VPKR_STARTSKIPLEN_REPLACE_VAL(reg,val) (((reg) & ~SVDO_VPKR_STARTSKIPLEN_MASK) | (((uint32_t)val) << SVDO_VPKR_STARTSKIPLEN_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 1 of 16) (JUST0)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 1 of 16)*/
//====================================================================

#define  SVDO_JUST0_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST0_JUSTLUT_SHIFT 0
#define  SVDO_JUST0_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST0_JUSTLUT_MASK) >> SVDO_JUST0_JUSTLUT_SHIFT)
#define  SVDO_JUST0_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST0_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST0_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 2 of 16) (JUST1)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 2 of 16)*/
//====================================================================

#define  SVDO_JUST1_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST1_JUSTLUT_SHIFT 0
#define  SVDO_JUST1_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST1_JUSTLUT_MASK) >> SVDO_JUST1_JUSTLUT_SHIFT)
#define  SVDO_JUST1_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST1_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST1_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 3 of 16) (JUST2)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 3 of 16)*/
//====================================================================

#define  SVDO_JUST2_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST2_JUSTLUT_SHIFT 0
#define  SVDO_JUST2_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST2_JUSTLUT_MASK) >> SVDO_JUST2_JUSTLUT_SHIFT)
#define  SVDO_JUST2_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST2_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST2_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 4 of 16) (JUST3)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 4 of 16)*/
//====================================================================

#define  SVDO_JUST3_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST3_JUSTLUT_SHIFT 0
#define  SVDO_JUST3_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST3_JUSTLUT_MASK) >> SVDO_JUST3_JUSTLUT_SHIFT)
#define  SVDO_JUST3_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST3_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST3_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 5 of 16) (JUST4)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 5 of 16)*/
//====================================================================

#define  SVDO_JUST4_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST4_JUSTLUT_SHIFT 0
#define  SVDO_JUST4_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST4_JUSTLUT_MASK) >> SVDO_JUST4_JUSTLUT_SHIFT)
#define  SVDO_JUST4_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST4_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST4_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 6 of 16) (JUST5)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 6 of 16)*/
//====================================================================

#define  SVDO_JUST5_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST5_JUSTLUT_SHIFT 0
#define  SVDO_JUST5_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST5_JUSTLUT_MASK) >> SVDO_JUST5_JUSTLUT_SHIFT)
#define  SVDO_JUST5_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST5_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST5_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 7 of 16) (JUST6)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 7 of 16)*/
//====================================================================

#define  SVDO_JUST6_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST6_JUSTLUT_SHIFT 0
#define  SVDO_JUST6_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST6_JUSTLUT_MASK) >> SVDO_JUST6_JUSTLUT_SHIFT)
#define  SVDO_JUST6_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST6_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST6_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 8 of 16) (JUST7)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 8 of 16)*/
//====================================================================

#define  SVDO_JUST7_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST7_JUSTLUT_SHIFT 0
#define  SVDO_JUST7_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST7_JUSTLUT_MASK) >> SVDO_JUST7_JUSTLUT_SHIFT)
#define  SVDO_JUST7_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST7_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST7_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 9 of 16) (JUST8)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 9 of 16)*/
//====================================================================

#define  SVDO_JUST8_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST8_JUSTLUT_SHIFT 0
#define  SVDO_JUST8_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST8_JUSTLUT_MASK) >> SVDO_JUST8_JUSTLUT_SHIFT)
#define  SVDO_JUST8_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST8_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST8_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 10 of 16) (JUST9)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 10 of 16)*/
//====================================================================

#define  SVDO_JUST9_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST9_JUSTLUT_SHIFT 0
#define  SVDO_JUST9_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST9_JUSTLUT_MASK) >> SVDO_JUST9_JUSTLUT_SHIFT)
#define  SVDO_JUST9_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST9_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST9_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 11 of 16) (JUST10)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 11 of 16)*/
//====================================================================

#define  SVDO_JUST10_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST10_JUSTLUT_SHIFT 0
#define  SVDO_JUST10_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST10_JUSTLUT_MASK) >> SVDO_JUST10_JUSTLUT_SHIFT)
#define  SVDO_JUST10_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST10_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST10_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 12 of 16) (JUST11)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 12 of 16)*/
//====================================================================

#define  SVDO_JUST11_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST11_JUSTLUT_SHIFT 0
#define  SVDO_JUST11_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST11_JUSTLUT_MASK) >> SVDO_JUST11_JUSTLUT_SHIFT)
#define  SVDO_JUST11_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST11_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST11_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 13 of 16) (JUST12)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 13 of 16)*/
//====================================================================

#define  SVDO_JUST12_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST12_JUSTLUT_SHIFT 0
#define  SVDO_JUST12_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST12_JUSTLUT_MASK) >> SVDO_JUST12_JUSTLUT_SHIFT)
#define  SVDO_JUST12_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST12_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST12_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 14 of 16) (JUST13)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 14 of 16)*/
//====================================================================

#define  SVDO_JUST13_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST13_JUSTLUT_SHIFT 0
#define  SVDO_JUST13_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST13_JUSTLUT_MASK) >> SVDO_JUST13_JUSTLUT_SHIFT)
#define  SVDO_JUST13_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST13_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST13_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 15 of 16) (JUST14)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 15 of 16)*/
//====================================================================

#define  SVDO_JUST14_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST14_JUSTLUT_SHIFT 0
#define  SVDO_JUST14_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST14_JUSTLUT_MASK) >> SVDO_JUST14_JUSTLUT_SHIFT)
#define  SVDO_JUST14_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST14_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST14_JUSTLUT_SHIFT))

//====================================================================
//Register: Justification LUT (Instance 16 of 16) (JUST15)
/** \brief This justification table represents a 256x2 LUT. It is packed such that the
      least significant bits are applied to the least significant pixel. For
      example if data is 4 bits per pixel, J[1:0] are appended to Din[3:0].
      <br>
      Justification Values:
      <br>
      00 = Center
      <br>
      01 = Late
      <br>
      10 = Early
      <br>
      11 = Split (Instance 16 of 16)*/
//====================================================================

#define  SVDO_JUST15_JUSTLUT_MASK 0xffffffff
#define  SVDO_JUST15_JUSTLUT_SHIFT 0
#define  SVDO_JUST15_JUSTLUT_MASK_SHIFT(reg) (((reg) & SVDO_JUST15_JUSTLUT_MASK) >> SVDO_JUST15_JUSTLUT_SHIFT)
#define  SVDO_JUST15_JUSTLUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_JUST15_JUSTLUT_MASK) | (((uint32_t)val) << SVDO_JUST15_JUSTLUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 1 of 16) (PCODE0)
/** \brief This pcode table represents a 64x8 LUT. (Instance 1 of 16)*/
//====================================================================

#define  SVDO_PCODE0_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE0_PCODELUT_SHIFT 0
#define  SVDO_PCODE0_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE0_PCODELUT_MASK) >> SVDO_PCODE0_PCODELUT_SHIFT)
#define  SVDO_PCODE0_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE0_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE0_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 2 of 16) (PCODE1)
/** \brief This pcode table represents a 64x8 LUT. (Instance 2 of 16)*/
//====================================================================

#define  SVDO_PCODE1_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE1_PCODELUT_SHIFT 0
#define  SVDO_PCODE1_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE1_PCODELUT_MASK) >> SVDO_PCODE1_PCODELUT_SHIFT)
#define  SVDO_PCODE1_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE1_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE1_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 3 of 16) (PCODE2)
/** \brief This pcode table represents a 64x8 LUT. (Instance 3 of 16)*/
//====================================================================

#define  SVDO_PCODE2_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE2_PCODELUT_SHIFT 0
#define  SVDO_PCODE2_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE2_PCODELUT_MASK) >> SVDO_PCODE2_PCODELUT_SHIFT)
#define  SVDO_PCODE2_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE2_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE2_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 4 of 16) (PCODE3)
/** \brief This pcode table represents a 64x8 LUT. (Instance 4 of 16)*/
//====================================================================

#define  SVDO_PCODE3_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE3_PCODELUT_SHIFT 0
#define  SVDO_PCODE3_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE3_PCODELUT_MASK) >> SVDO_PCODE3_PCODELUT_SHIFT)
#define  SVDO_PCODE3_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE3_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE3_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 5 of 16) (PCODE4)
/** \brief This pcode table represents a 64x8 LUT. (Instance 5 of 16)*/
//====================================================================

#define  SVDO_PCODE4_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE4_PCODELUT_SHIFT 0
#define  SVDO_PCODE4_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE4_PCODELUT_MASK) >> SVDO_PCODE4_PCODELUT_SHIFT)
#define  SVDO_PCODE4_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE4_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE4_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 6 of 16) (PCODE5)
/** \brief This pcode table represents a 64x8 LUT. (Instance 6 of 16)*/
//====================================================================

#define  SVDO_PCODE5_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE5_PCODELUT_SHIFT 0
#define  SVDO_PCODE5_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE5_PCODELUT_MASK) >> SVDO_PCODE5_PCODELUT_SHIFT)
#define  SVDO_PCODE5_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE5_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE5_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 7 of 16) (PCODE6)
/** \brief This pcode table represents a 64x8 LUT. (Instance 7 of 16)*/
//====================================================================

#define  SVDO_PCODE6_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE6_PCODELUT_SHIFT 0
#define  SVDO_PCODE6_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE6_PCODELUT_MASK) >> SVDO_PCODE6_PCODELUT_SHIFT)
#define  SVDO_PCODE6_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE6_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE6_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 8 of 16) (PCODE7)
/** \brief This pcode table represents a 64x8 LUT. (Instance 8 of 16)*/
//====================================================================

#define  SVDO_PCODE7_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE7_PCODELUT_SHIFT 0
#define  SVDO_PCODE7_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE7_PCODELUT_MASK) >> SVDO_PCODE7_PCODELUT_SHIFT)
#define  SVDO_PCODE7_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE7_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE7_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 9 of 16) (PCODE8)
/** \brief This pcode table represents a 64x8 LUT. (Instance 9 of 16)*/
//====================================================================

#define  SVDO_PCODE8_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE8_PCODELUT_SHIFT 0
#define  SVDO_PCODE8_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE8_PCODELUT_MASK) >> SVDO_PCODE8_PCODELUT_SHIFT)
#define  SVDO_PCODE8_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE8_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE8_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 10 of 16) (PCODE9)
/** \brief This pcode table represents a 64x8 LUT. (Instance 10 of 16)*/
//====================================================================

#define  SVDO_PCODE9_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE9_PCODELUT_SHIFT 0
#define  SVDO_PCODE9_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE9_PCODELUT_MASK) >> SVDO_PCODE9_PCODELUT_SHIFT)
#define  SVDO_PCODE9_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE9_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE9_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 11 of 16) (PCODE10)
/** \brief This pcode table represents a 64x8 LUT. (Instance 11 of 16)*/
//====================================================================

#define  SVDO_PCODE10_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE10_PCODELUT_SHIFT 0
#define  SVDO_PCODE10_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE10_PCODELUT_MASK) >> SVDO_PCODE10_PCODELUT_SHIFT)
#define  SVDO_PCODE10_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE10_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE10_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 12 of 16) (PCODE11)
/** \brief This pcode table represents a 64x8 LUT. (Instance 12 of 16)*/
//====================================================================

#define  SVDO_PCODE11_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE11_PCODELUT_SHIFT 0
#define  SVDO_PCODE11_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE11_PCODELUT_MASK) >> SVDO_PCODE11_PCODELUT_SHIFT)
#define  SVDO_PCODE11_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE11_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE11_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 13 of 16) (PCODE12)
/** \brief This pcode table represents a 64x8 LUT. (Instance 13 of 16)*/
//====================================================================

#define  SVDO_PCODE12_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE12_PCODELUT_SHIFT 0
#define  SVDO_PCODE12_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE12_PCODELUT_MASK) >> SVDO_PCODE12_PCODELUT_SHIFT)
#define  SVDO_PCODE12_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE12_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE12_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 14 of 16) (PCODE13)
/** \brief This pcode table represents a 64x8 LUT. (Instance 14 of 16)*/
//====================================================================

#define  SVDO_PCODE13_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE13_PCODELUT_SHIFT 0
#define  SVDO_PCODE13_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE13_PCODELUT_MASK) >> SVDO_PCODE13_PCODELUT_SHIFT)
#define  SVDO_PCODE13_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE13_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE13_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 15 of 16) (PCODE14)
/** \brief This pcode table represents a 64x8 LUT. (Instance 15 of 16)*/
//====================================================================

#define  SVDO_PCODE14_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE14_PCODELUT_SHIFT 0
#define  SVDO_PCODE14_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE14_PCODELUT_MASK) >> SVDO_PCODE14_PCODELUT_SHIFT)
#define  SVDO_PCODE14_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE14_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE14_PCODELUT_SHIFT))

//====================================================================
//Register: PCODE LUT (Instance 16 of 16) (PCODE15)
/** \brief This pcode table represents a 64x8 LUT. (Instance 16 of 16)*/
//====================================================================

#define  SVDO_PCODE15_PCODELUT_MASK 0xffffffff
#define  SVDO_PCODE15_PCODELUT_SHIFT 0
#define  SVDO_PCODE15_PCODELUT_MASK_SHIFT(reg) (((reg) & SVDO_PCODE15_PCODELUT_MASK) >> SVDO_PCODE15_PCODELUT_SHIFT)
#define  SVDO_PCODE15_PCODELUT_REPLACE_VAL(reg,val) (((reg) & ~SVDO_PCODE15_PCODELUT_MASK) | (((uint32_t)val) << SVDO_PCODE15_PCODELUT_SHIFT))

#endif // SVDO
