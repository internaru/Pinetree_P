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
 * \file BLOOM_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BLOOM_REGSTRUCTS_H_
#define _BLOOM_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: BLOOM (BLOOM)
/** \brief Register descriptions for the BLOOM block.*/
//
//====================================================================

typedef struct BLOOM_REGS_s
{
  volatile uint32_t BCR;  ///< 0x0 [R/W]: BLOOM Configuration Register
  volatile uint32_t BIIC;  ///< 0x4 [R/W]: Input Image Configuration Register
  volatile uint32_t reserved0[2];
  volatile uint32_t BLBC;  ///< 0x10 [R/W]: Line Buffer Access Control Register
  volatile uint32_t BLBA;  ///< 0x14 [W]:  Line Buffer Address Register
  volatile uint32_t BLBAS;  ///< 0x18 [R]: Line Buffer Address Status Register
  volatile uint32_t BLBWD;  ///< 0x1c [W]: Line Buffer Write Data
  volatile uint32_t BLBRD;  ///< 0x20 [R]: Line Buffer Read Data
  volatile uint32_t BSRA;  ///< 0x24 [R/W]: BLOOM Save Restore Address
  volatile uint32_t BSRR;  ///< 0x28 [R]: BLOOM Save Restore Read
  volatile uint32_t BSRW;  ///< 0x2c [W]: BLOOM Save Restore Write
  volatile uint32_t BER;  ///< 0x30 [R]: BLOOM Error Register
  volatile uint32_t BSD;  ///< 0x34 [R]: BLOOM State Debug
} BLOOM_REGS_t;

#endif // BLOOM
