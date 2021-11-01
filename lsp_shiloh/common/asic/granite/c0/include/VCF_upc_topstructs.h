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
 * \file VCF_upc_topstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _VCF_UPC_TOP_REGSTRUCTS_H_
#define _VCF_UPC_TOP_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: VCF UPC Registers (VCF_UPC_TOP)
//
//====================================================================

//====================================================================
//
//Register File: UPC (UPC)
//
//====================================================================

typedef struct UPC_REGS_s
{
  volatile uint32_t VUCR;  ///< 0x0 [R/W]: VCF UPC Config Register
  volatile uint32_t VUHR0;  ///< 0x4 [W]: VCF UPC Hash Register (Instance 1 of 2)
  volatile uint32_t VUHR1;  ///< 0x8 [W]: VCF UPC Hash Register (Instance 2 of 2)
  volatile uint32_t VULR;  ///< 0xc [R/W]: VCF UPC Laser Register
} UPC_REGS_t;

//====================================================================
//
//Register File: IRQ (IRQ)
//
//====================================================================

typedef struct IRQ_REGS_s
{
  volatile uint32_t VIER;  ///< 0x0 [R/W]: VCF Interrupt Enable Register
  volatile uint32_t VIPR;  ///< 0x4 [R]: VCF Interrupt Pending Register
  volatile uint32_t VICR;  ///< 0x8 [W]: VCF Interrupt Clear Register
  volatile uint32_t VIFR;  ///< 0xc [W]: VCF Interrupt Force Register
  volatile uint32_t VIIR0;  ///< 0x10 [R]: VCF IRQ Input Register (Instance 1 of 2)
  volatile uint32_t VIIR1;  ///< 0x14 [R]: VCF IRQ Input Register (Instance 2 of 2)
  volatile uint32_t VIMR0;  ///< 0x18 [R/W]: VCF IRQ Mux Register (Instance 1 of 4)
  volatile uint32_t VIMR1;  ///< 0x1c [R/W]: VCF IRQ Mux Register (Instance 2 of 4)
  volatile uint32_t VIMR2;  ///< 0x20 [R/W]: VCF IRQ Mux Register (Instance 3 of 4)
  volatile uint32_t VIMR3;  ///< 0x24 [R/W]: VCF IRQ Mux Register (Instance 4 of 4)
} IRQ_REGS_t;

#endif // VCF_UPC_TOP
