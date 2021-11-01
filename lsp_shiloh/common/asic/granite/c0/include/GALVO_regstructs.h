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
 * \file GALVO_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _GALVO_REGSTRUCTS_H_
#define _GALVO_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: GALVO (GALVO)
//
//====================================================================

typedef struct GALVO_REGS_s
{
  volatile uint32_t gv_dsscntl;  ///< 0x0 [R/W]: Galvo Digital Synthesis Control 
  volatile uint32_t gv_dssadder;  ///< 0x4 [R/W]: Digital Synthesis Adder
  volatile uint32_t gv_timeinfo;  ///< 0x8 [R]: Galvo Time Info
  volatile uint32_t gv_drvlimit;  ///< 0xc [R/W]: Galvo Drive Limit
  volatile uint32_t gv_profram;  ///< 0x10 [W]: Galvo Profile Ram
  volatile uint32_t gv_pwcntl;  ///< 0x14 [R/W]: Galvo Pulse Width Control
  volatile uint32_t gv_aligncntl;  ///< 0x18 [R/W]: Galvo Alignment Control 
  volatile uint32_t gv_align_fb;  ///< 0x1c [R]: Galvo Alignment Feedback 
  volatile uint32_t gv_align_misccntl;  ///< 0x20 [R/W]: Galvo Miscellaneous Control
  volatile uint32_t gv_xshifts;  ///< 0x24 [R/W]: Galvo XNOR Shift 
  volatile uint32_t gv_xcount;  ///< 0x28 [R/W]: Galvo XNOR Count 
  volatile uint32_t gv_simcntl;  ///< 0x2c [R/W]: 
  volatile uint32_t gv_cap0_1;  ///< 0x30 [R]: Galvo Capture Interval 0:1 
  volatile uint32_t gv_cap2_3;  ///< 0x34 [R]: Galvo Capture Interval 2:3
  volatile uint32_t reserved0;
  volatile uint32_t galvcntl;  ///< 0x3c [R/W]: Galvo Control
  volatile uint32_t reserved1;
  volatile uint32_t gv_lascntl1;  ///< 0x44 [R/W]: Galvo Laser Control 1 
  volatile uint32_t gv_lascntl2;  ///< 0x48 [R/W]: Galvo Laser Control 2 
  volatile uint32_t gv_girqcntl;  ///< 0x4c [R/W]: Galvo Interrupt Control
  volatile uint32_t gv_zone_2_3;  ///< 0x50 [R/W]: 
  volatile uint32_t gv_zone_5_6;  ///< 0x54 [R/W]: 
  volatile uint32_t gv_zone_C_4;  ///< 0x58 [R/W]: 
  volatile uint32_t gv_zone_E_F;  ///< 0x5c [R/W]: 
  volatile uint32_t gv_zone_9_A;  ///< 0x60 [R/W]: 
  volatile uint32_t gv_zone_0_8;  ///< 0x64 [R/W]: 
  volatile uint32_t gv_adjwidth;  ///< 0x68 [R/W]: 
  volatile uint32_t gv_picntl1;  ///< 0x6c [R/W]: Galvo Proportional Integral Control 1 
  volatile uint32_t gv_pitarget1;  ///< 0x70 [R/W]: Galvo Proportional Integral Target 1 
  volatile uint32_t gv_picntl2;  ///< 0x74 [R/W]: Galvo Proportional Integral Control 2
  volatile uint32_t gv_pitarget2;  ///< 0x78 [R/W]: Galvo Proportional Integral Target 2 
  volatile uint32_t gv_piaccum1;  ///< 0x7c [R]: Galvo Proportional Integral Accumlator 1 
  volatile uint32_t gv_piaccum2;  ///< 0x80 [R]: Galvo Proportional Integral Accumlator 2
  volatile uint32_t gv_err_msk;  ///< 0x84 [R/W]: 
  volatile uint32_t gv_err_stat1;  ///< 0x88 [R]: 
  volatile uint32_t gv_err_stat2;  ///< 0x8c [R]: 
  volatile uint32_t reserved2[6];
  volatile uint32_t gv_statcntl;  ///< 0xa8 [R/W]: Galvo Status/Control 
  volatile uint32_t gv_cap0_sum;  ///< 0xac [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Sum Data 
  volatile uint32_t gv_cap1_sum;  ///< 0xb0 [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Sum Data 
  volatile uint32_t gv_cap2_sum;  ///< 0xb4 [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Sum Data 
  volatile uint32_t gv_cap3_sum;  ///< 0xb8 [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Sum Data 
  volatile uint32_t gv_cap0_hilo;  ///< 0xbc [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Min/Max Data 
  volatile uint32_t gv_cap1_hilo;  ///< 0xc0 [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Min/Max Data 
  volatile uint32_t gv_cap2_hilo;  ///< 0xc4 [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Min/Max Data 
  volatile uint32_t gv_cap3_hilo;  ///< 0xc8 [R]: Galvo CAPA:CAPB:ALIGN:PHASE:AMP:OFFSET Min/Max Data 
  volatile uint32_t gv_amp_sum;  ///< 0xcc [R]: 
  volatile uint32_t gv_off_sum;  ///< 0xd0 [R]: 
  volatile uint32_t gv_amp_hilo;  ///< 0xd4 [R]: 
  volatile uint32_t gv_off_hilo;  ///< 0xd8 [R]: 
  volatile uint32_t test_reg_0;  ///< 0xdc [R/W]: tr0
  volatile uint32_t test_reg_1;  ///< 0xe0 [R/W]: tr1
} GALVO_REGS_t;

#endif // GALVO
