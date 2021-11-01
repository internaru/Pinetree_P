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
 * \file PNU2D_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PNU2D_REGSTRUCTS_H_
#define _PNU2D_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PNU2D: Camera PRNU/DSNU Support (PNU2D)
/** \brief Top-level register file for the Camera PRNU/DSNU Support block*/
//
//====================================================================

typedef struct PNU2D_REGS_s
{
  volatile uint32_t PNU2DCFG;  ///< 0x0 [R/W]: Configuration Register
  volatile uint32_t PNU2DBCFG;  ///< 0x4 [R/W]: PNU2D Bayer Configuration Register
  volatile uint32_t PNU2DBGN;  ///< 0x8 [R/W]: PNU2D PRNU Bayer Gain Register
  volatile uint32_t PNU2DBOS1;  ///< 0xc [R/W]: PNU2D PRNU Bayer Offset Register 1
  volatile uint32_t PNU2DBOS2;  ///< 0x10 [R/W]: PNU2D PRNU Bayer Offset Register 2
  volatile uint32_t PNU2DST;  ///< 0x14 [R]: PNU2D Status Register
} PNU2D_REGS_t;

#endif // PNU2D
