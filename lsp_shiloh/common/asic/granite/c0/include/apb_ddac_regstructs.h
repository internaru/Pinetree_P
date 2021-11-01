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
 * \file apb_ddac_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _APB_DDAC_REG_REGSTRUCTS_H_
#define _APB_DDAC_REG_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Audio DDAC Registers (apb_ddac_reg)
/** \brief Registers for the audio DDAC module*/
//
//====================================================================

typedef struct APB_DDAC_REG_REGS_s
{
  volatile uint32_t AOCR;  ///< 0x0 [R/W]: Audio Output Configuration Register
  volatile uint32_t AODR;  ///< 0x4 [W]: Audio Output Data Register
  volatile uint32_t AOSR;  ///< 0x8 [R]: Audio Output Status Register
  volatile uint32_t AOCR2;  ///< 0xc [R/W]: Audio Output Configuration2 Register
} APB_DDAC_REG_REGS_t;

#endif // APB_DDAC_REG
