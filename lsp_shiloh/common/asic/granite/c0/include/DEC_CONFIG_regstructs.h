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
 * \file DEC_CONFIG_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _DEC_CONFIG_REGS_REGSTRUCTS_H_
#define _DEC_CONFIG_REGS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: dec_config_regs (dec_config_regs)
//
//====================================================================

typedef struct DEC_CONFIG_REGS_REGS_s
{
  volatile uint32_t top_sync0;  ///< 0x0 [R/W]: Top Synchronization register (Instance 1 of 4)
  volatile uint32_t top_sync1;  ///< 0x4 [R/W]: Top Synchronization register (Instance 2 of 4)
  volatile uint32_t top_sync2;  ///< 0x8 [R/W]: Top Synchronization register (Instance 3 of 4)
  volatile uint32_t top_sync3;  ///< 0xc [R/W]: Top Synchronization register (Instance 4 of 4)
  volatile uint32_t bottom_sync0;  ///< 0x10 [R/W]: Bottom Synchronization register (Instance 1 of 4)
  volatile uint32_t bottom_sync1;  ///< 0x14 [R/W]: Bottom Synchronization register (Instance 2 of 4)
  volatile uint32_t bottom_sync2;  ///< 0x18 [R/W]: Bottom Synchronization register (Instance 3 of 4)
  volatile uint32_t bottom_sync3;  ///< 0x1c [R/W]: Bottom Synchronization register (Instance 4 of 4)
  volatile uint32_t oe_misc;  ///< 0x20 [R/W]: Miscellaneous Output Enable register
  volatile uint32_t missing;  ///< 0x24 [R/W]: Missing HSync Merge register
  volatile uint32_t stretch;  ///< 0x28 [R/W]: HSync_missed Stretch register
} DEC_CONFIG_REGS_REGS_t;

#endif // DEC_CONFIG_REGS
