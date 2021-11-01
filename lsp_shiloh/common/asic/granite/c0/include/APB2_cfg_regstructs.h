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
 * \file APB2_cfg_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _APB2_TOP_REGSTRUCTS_H_
#define _APB2_TOP_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: APB2 Top Control/Interrupt Registers (APB2_TOP)
/** \brief Register Descriptions for the APB2 Control and Interrupt block*/
//
//====================================================================

typedef struct APB2_TOP_REGS_s
{
  volatile uint32_t rb4APB2MiscCtrl;  ///< 0x0 [R/W]: APB2 Miscellaneous Control Register
  volatile uint32_t rb4APB2IrqEnb;  ///< 0x4 [R/W]: APB2 Top Interrupt Enable Register
  volatile uint32_t rb4APB2IrqPnd;  ///< 0x8 [R]: APB2 Top Interrupt Pending Register
  volatile uint32_t rb4APB2IrqAck;  ///< 0xc [W]: APB2 Top Interrupt Acknowledge Register
  volatile uint32_t rb4APB2IrqForce;  ///< 0x10 [W]: APB2 Top Interrupt Acknowledge Register
} APB2_TOP_REGS_t;

#endif // APB2_TOP
