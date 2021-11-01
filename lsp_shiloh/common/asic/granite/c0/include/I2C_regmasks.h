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
 * \file I2C_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _I2C_REGMASKS_H_
#define _I2C_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: I2C (I2C)
/** \brief Top-level register file for APB I2C Block*/
//
//====================================================================

//====================================================================
//Register: I2C Control Register (IC_CON)
/** \brief Register used to configure various aspects of the I2C block. It can only be written
when the i2c communications is disabled (IC_ENABLE register set to 0). Writes at
other times are ignored.*/
//====================================================================

#define  I2C_IC_CON_RESERVED1_MASK 0xffffff80
#define  I2C_IC_CON_RESERVED1_SHIFT 7
#define  I2C_IC_CON_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CON_RESERVED1_MASK) >> I2C_IC_CON_RESERVED1_SHIFT)
#define  I2C_IC_CON_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CON_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CON_RESERVED1_SHIFT))

#define  I2C_IC_CON_IC_SLAVE_DISABLE_MASK 0x40
#define  I2C_IC_CON_IC_SLAVE_DISABLE_SHIFT 6
#define  I2C_IC_CON_IC_SLAVE_DISABLE_MASK_SHIFT(reg) (((reg) & I2C_IC_CON_IC_SLAVE_DISABLE_MASK) >> I2C_IC_CON_IC_SLAVE_DISABLE_SHIFT)
#define  I2C_IC_CON_IC_SLAVE_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CON_IC_SLAVE_DISABLE_MASK) | (((uint32_t)val) << I2C_IC_CON_IC_SLAVE_DISABLE_SHIFT))

#define  I2C_IC_CON_IC_RESTART_MASK 0x20
#define  I2C_IC_CON_IC_RESTART_SHIFT 5
#define  I2C_IC_CON_IC_RESTART_MASK_SHIFT(reg) (((reg) & I2C_IC_CON_IC_RESTART_MASK) >> I2C_IC_CON_IC_RESTART_SHIFT)
#define  I2C_IC_CON_IC_RESTART_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CON_IC_RESTART_MASK) | (((uint32_t)val) << I2C_IC_CON_IC_RESTART_SHIFT))

#define  I2C_IC_CON_IC_10BITADDR_MASTER_MASK 0x10
#define  I2C_IC_CON_IC_10BITADDR_MASTER_SHIFT 4
#define  I2C_IC_CON_IC_10BITADDR_MASTER_MASK_SHIFT(reg) (((reg) & I2C_IC_CON_IC_10BITADDR_MASTER_MASK) >> I2C_IC_CON_IC_10BITADDR_MASTER_SHIFT)
#define  I2C_IC_CON_IC_10BITADDR_MASTER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CON_IC_10BITADDR_MASTER_MASK) | (((uint32_t)val) << I2C_IC_CON_IC_10BITADDR_MASTER_SHIFT))

#define  I2C_IC_CON_IC_10BITADDR_SLAVE_MASK 0x8
#define  I2C_IC_CON_IC_10BITADDR_SLAVE_SHIFT 3
#define  I2C_IC_CON_IC_10BITADDR_SLAVE_MASK_SHIFT(reg) (((reg) & I2C_IC_CON_IC_10BITADDR_SLAVE_MASK) >> I2C_IC_CON_IC_10BITADDR_SLAVE_SHIFT)
#define  I2C_IC_CON_IC_10BITADDR_SLAVE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CON_IC_10BITADDR_SLAVE_MASK) | (((uint32_t)val) << I2C_IC_CON_IC_10BITADDR_SLAVE_SHIFT))

#define  I2C_IC_CON_SPEED_MASK 0x6
#define  I2C_IC_CON_SPEED_SHIFT 1
#define  I2C_IC_CON_SPEED_MASK_SHIFT(reg) (((reg) & I2C_IC_CON_SPEED_MASK) >> I2C_IC_CON_SPEED_SHIFT)
#define  I2C_IC_CON_SPEED_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CON_SPEED_MASK) | (((uint32_t)val) << I2C_IC_CON_SPEED_SHIFT))

#define  I2C_IC_CON_MASTER_MODE_MASK 0x1
#define  I2C_IC_CON_MASTER_MODE_SHIFT 0
#define  I2C_IC_CON_MASTER_MODE_MASK_SHIFT(reg) (((reg) & I2C_IC_CON_MASTER_MODE_MASK) >> I2C_IC_CON_MASTER_MODE_SHIFT)
#define  I2C_IC_CON_MASTER_MODE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CON_MASTER_MODE_MASK) | (((uint32_t)val) << I2C_IC_CON_MASTER_MODE_SHIFT))

//====================================================================
//Register: Target Address Register (IC_TAR)
/** \brief This register sets the target address to be used when acting as a master.
IT also indicates if the transaction will use a General Call or Start Byte transaction.*/
//====================================================================

#define  I2C_IC_TAR_RESERVED1_MASK 0xffffe000
#define  I2C_IC_TAR_RESERVED1_SHIFT 13
#define  I2C_IC_TAR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_TAR_RESERVED1_MASK) >> I2C_IC_TAR_RESERVED1_SHIFT)
#define  I2C_IC_TAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TAR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_TAR_RESERVED1_SHIFT))

#define  I2C_IC_TAR_IC_10BITADDR_MASTER_MASK 0x1000
#define  I2C_IC_TAR_IC_10BITADDR_MASTER_SHIFT 12
#define  I2C_IC_TAR_IC_10BITADDR_MASTER_MASK_SHIFT(reg) (((reg) & I2C_IC_TAR_IC_10BITADDR_MASTER_MASK) >> I2C_IC_TAR_IC_10BITADDR_MASTER_SHIFT)
#define  I2C_IC_TAR_IC_10BITADDR_MASTER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TAR_IC_10BITADDR_MASTER_MASK) | (((uint32_t)val) << I2C_IC_TAR_IC_10BITADDR_MASTER_SHIFT))

#define  I2C_IC_TAR_SPECIAL_MASK 0x800
#define  I2C_IC_TAR_SPECIAL_SHIFT 11
#define  I2C_IC_TAR_SPECIAL_MASK_SHIFT(reg) (((reg) & I2C_IC_TAR_SPECIAL_MASK) >> I2C_IC_TAR_SPECIAL_SHIFT)
#define  I2C_IC_TAR_SPECIAL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TAR_SPECIAL_MASK) | (((uint32_t)val) << I2C_IC_TAR_SPECIAL_SHIFT))

#define  I2C_IC_TAR_GC_OR_START_MASK 0x400
#define  I2C_IC_TAR_GC_OR_START_SHIFT 10
#define  I2C_IC_TAR_GC_OR_START_MASK_SHIFT(reg) (((reg) & I2C_IC_TAR_GC_OR_START_MASK) >> I2C_IC_TAR_GC_OR_START_SHIFT)
#define  I2C_IC_TAR_GC_OR_START_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TAR_GC_OR_START_MASK) | (((uint32_t)val) << I2C_IC_TAR_GC_OR_START_SHIFT))

#define  I2C_IC_TAR_IC_TAR_MASK 0x3ff
#define  I2C_IC_TAR_IC_TAR_SHIFT 0
#define  I2C_IC_TAR_IC_TAR_MASK_SHIFT(reg) (((reg) & I2C_IC_TAR_IC_TAR_MASK) >> I2C_IC_TAR_IC_TAR_SHIFT)
#define  I2C_IC_TAR_IC_TAR_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TAR_IC_TAR_MASK) | (((uint32_t)val) << I2C_IC_TAR_IC_TAR_SHIFT))

//====================================================================
//Register: Slave Address Register (IC_SAR)
/** \brief This register indicates the address used when the I2C is acting as a Slave. This register can
only be written when the I2C interface is disabled i.e. IC_ENABLE register set to 0.
Writes at other times are ignored.*/
//====================================================================

#define  I2C_IC_SAR_RESERVED1_MASK 0xfffffc00
#define  I2C_IC_SAR_RESERVED1_SHIFT 10
#define  I2C_IC_SAR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_SAR_RESERVED1_MASK) >> I2C_IC_SAR_RESERVED1_SHIFT)
#define  I2C_IC_SAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SAR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_SAR_RESERVED1_SHIFT))

#define  I2C_IC_SAR_IC_SAR_MASK 0x3ff
#define  I2C_IC_SAR_IC_SAR_SHIFT 0
#define  I2C_IC_SAR_IC_SAR_MASK_SHIFT(reg) (((reg) & I2C_IC_SAR_IC_SAR_MASK) >> I2C_IC_SAR_IC_SAR_SHIFT)
#define  I2C_IC_SAR_IC_SAR_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SAR_IC_SAR_MASK) | (((uint32_t)val) << I2C_IC_SAR_IC_SAR_SHIFT))

//====================================================================
//Register: High Speed Mode Code Address Register (IC_HS_MADDR)
/** \brief Thjis register sets the High Speed Unique ID for a Master. Note HS Mode not Currently supported!!!*/
//====================================================================

#define  I2C_IC_HS_MADDR_RESERVED1_MASK 0xfffffff8
#define  I2C_IC_HS_MADDR_RESERVED1_SHIFT 3
#define  I2C_IC_HS_MADDR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_HS_MADDR_RESERVED1_MASK) >> I2C_IC_HS_MADDR_RESERVED1_SHIFT)
#define  I2C_IC_HS_MADDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_HS_MADDR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_HS_MADDR_RESERVED1_SHIFT))

#define  I2C_IC_HS_MADDR_IC_HS_MAR_MASK 0x7
#define  I2C_IC_HS_MADDR_IC_HS_MAR_SHIFT 0
#define  I2C_IC_HS_MADDR_IC_HS_MAR_MASK_SHIFT(reg) (((reg) & I2C_IC_HS_MADDR_IC_HS_MAR_MASK) >> I2C_IC_HS_MADDR_IC_HS_MAR_SHIFT)
#define  I2C_IC_HS_MADDR_IC_HS_MAR_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_HS_MADDR_IC_HS_MAR_MASK) | (((uint32_t)val) << I2C_IC_HS_MADDR_IC_HS_MAR_SHIFT))

//====================================================================
//Register: Tx/Rx DataBuffer and Command register (IC_DATA_CMD)
/** \brief This is the register written by the CPU when filling the Tx FIFO, and read from when empting the Rx FIFO*/
//====================================================================

#define  I2C_IC_DATA_CMD_RESERVED1_MASK 0xfffffe00
#define  I2C_IC_DATA_CMD_RESERVED1_SHIFT 9
#define  I2C_IC_DATA_CMD_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_DATA_CMD_RESERVED1_MASK) >> I2C_IC_DATA_CMD_RESERVED1_SHIFT)
#define  I2C_IC_DATA_CMD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DATA_CMD_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_DATA_CMD_RESERVED1_SHIFT))

#define  I2C_IC_DATA_CMD_CMD_MASK 0x100
#define  I2C_IC_DATA_CMD_CMD_SHIFT 8
#define  I2C_IC_DATA_CMD_CMD_MASK_SHIFT(reg) (((reg) & I2C_IC_DATA_CMD_CMD_MASK) >> I2C_IC_DATA_CMD_CMD_SHIFT)
#define  I2C_IC_DATA_CMD_CMD_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DATA_CMD_CMD_MASK) | (((uint32_t)val) << I2C_IC_DATA_CMD_CMD_SHIFT))

#define  I2C_IC_DATA_CMD_DAT_MASK 0xff
#define  I2C_IC_DATA_CMD_DAT_SHIFT 0
#define  I2C_IC_DATA_CMD_DAT_MASK_SHIFT(reg) (((reg) & I2C_IC_DATA_CMD_DAT_MASK) >> I2C_IC_DATA_CMD_DAT_SHIFT)
#define  I2C_IC_DATA_CMD_DAT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DATA_CMD_DAT_MASK) | (((uint32_t)val) << I2C_IC_DATA_CMD_DAT_SHIFT))

//====================================================================
//Register: Standard Speed SCL HIGH Count (IC_SS_SCL_HCNT)
/** \brief This register sets the SCL clock high period count for standard speed transactions.
This value plus 8 is the number of ic_clk periods used for the SCL high time. This register can
only be written when the I2C bus is disabled (i.e. IC_ENABLE register is cleared).*/
//====================================================================

#define  I2C_IC_SS_SCL_HCNT_RESERVED1_MASK 0xffff0000
#define  I2C_IC_SS_SCL_HCNT_RESERVED1_SHIFT 16
#define  I2C_IC_SS_SCL_HCNT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_SS_SCL_HCNT_RESERVED1_MASK) >> I2C_IC_SS_SCL_HCNT_RESERVED1_SHIFT)
#define  I2C_IC_SS_SCL_HCNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SS_SCL_HCNT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_SS_SCL_HCNT_RESERVED1_SHIFT))

#define  I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_MASK 0xffff
#define  I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_SHIFT 0
#define  I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_MASK_SHIFT(reg) (((reg) & I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_MASK) >> I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_SHIFT)
#define  I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_MASK) | (((uint32_t)val) << I2C_IC_SS_SCL_HCNT_IC_SS_SCL_HCNT_SHIFT))

//====================================================================
//Register: Standard Speed SCL Low Count (IC_SS_SCL_LCNT)
/** \brief This register sets the SCL clock low period count for standard speed transactions.
This value plus 1 is the number of ic_clk periods used for the SCL low time. This register can
only be written when the I2C bus is disabled (i.e. IC_ENABLE registeris cleared).*/
//====================================================================

#define  I2C_IC_SS_SCL_LCNT_RESERVED1_MASK 0xffff0000
#define  I2C_IC_SS_SCL_LCNT_RESERVED1_SHIFT 16
#define  I2C_IC_SS_SCL_LCNT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_SS_SCL_LCNT_RESERVED1_MASK) >> I2C_IC_SS_SCL_LCNT_RESERVED1_SHIFT)
#define  I2C_IC_SS_SCL_LCNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SS_SCL_LCNT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_SS_SCL_LCNT_RESERVED1_SHIFT))

#define  I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_MASK 0xffff
#define  I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_SHIFT 0
#define  I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_MASK_SHIFT(reg) (((reg) & I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_MASK) >> I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_SHIFT)
#define  I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_MASK) | (((uint32_t)val) << I2C_IC_SS_SCL_LCNT_IC_SS_SCL_LCNT_SHIFT))

//====================================================================
//Register: Fast Speed SCL HIGH Count (IC_FS_SCL_HCNT)
/** \brief This register sets the SCL clock high period count for fast speed transactions.
This value plus 8 is the number of ic_clk periods used for the SCL high time. This register can
only be written when the I2C bus is disabled (i.e. IC_ENABLE register is cleared).*/
//====================================================================

#define  I2C_IC_FS_SCL_HCNT_RESERVED1_MASK 0xffff0000
#define  I2C_IC_FS_SCL_HCNT_RESERVED1_SHIFT 16
#define  I2C_IC_FS_SCL_HCNT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_FS_SCL_HCNT_RESERVED1_MASK) >> I2C_IC_FS_SCL_HCNT_RESERVED1_SHIFT)
#define  I2C_IC_FS_SCL_HCNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_FS_SCL_HCNT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_FS_SCL_HCNT_RESERVED1_SHIFT))

#define  I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_MASK 0xffff
#define  I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_SHIFT 0
#define  I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_MASK_SHIFT(reg) (((reg) & I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_MASK) >> I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_SHIFT)
#define  I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_MASK) | (((uint32_t)val) << I2C_IC_FS_SCL_HCNT_IC_FS_SCL_HCNT_SHIFT))

//====================================================================
//Register: FAST Speed SCL Low Count (IC_FS_SCL_LCNT)
/** \brief This register sets the SCL clock low period count for fast speed transactions.
This value plus 1 is the number of ic_clk periods used for the SCL low time. This register can
only be written when the I2C bus is disabled (i.e. IC_ENABLE register is cleared).*/
//====================================================================

#define  I2C_IC_FS_SCL_LCNT_RESERVED1_MASK 0xffff0000
#define  I2C_IC_FS_SCL_LCNT_RESERVED1_SHIFT 16
#define  I2C_IC_FS_SCL_LCNT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_FS_SCL_LCNT_RESERVED1_MASK) >> I2C_IC_FS_SCL_LCNT_RESERVED1_SHIFT)
#define  I2C_IC_FS_SCL_LCNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_FS_SCL_LCNT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_FS_SCL_LCNT_RESERVED1_SHIFT))

#define  I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_MASK 0xffff
#define  I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_SHIFT 0
#define  I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_MASK_SHIFT(reg) (((reg) & I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_MASK) >> I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_SHIFT)
#define  I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_MASK) | (((uint32_t)val) << I2C_IC_FS_SCL_LCNT_IC_FS_SCL_LCNT_SHIFT))

//====================================================================
//Register: HIGH Speed SCL HIGH Count (IC_HS_SCL_HCNT)
/** \brief This register sets the SCL clock high period count for high speed transactions.
This value plus 8 is the number of ic_clk periods used for the SCL high time. This register can
only be written when the I2C bus is disabled (i.e. IC_ENABLE register is cleared).*/
//====================================================================

#define  I2C_IC_HS_SCL_HCNT_RESERVED1_MASK 0xffff0000
#define  I2C_IC_HS_SCL_HCNT_RESERVED1_SHIFT 16
#define  I2C_IC_HS_SCL_HCNT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_HS_SCL_HCNT_RESERVED1_MASK) >> I2C_IC_HS_SCL_HCNT_RESERVED1_SHIFT)
#define  I2C_IC_HS_SCL_HCNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_HS_SCL_HCNT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_HS_SCL_HCNT_RESERVED1_SHIFT))

#define  I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_MASK 0xffff
#define  I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_SHIFT 0
#define  I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_MASK_SHIFT(reg) (((reg) & I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_MASK) >> I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_SHIFT)
#define  I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_MASK) | (((uint32_t)val) << I2C_IC_HS_SCL_HCNT_IC_HS_SCL_HCNT_SHIFT))

//====================================================================
//Register: High Speed SCL Low Count (IC_HS_SCL_LCNT)
/** \brief This register sets the SCL clock low period count for high speed transactions.
This value plus 1 is the number of ic_clk periods used for the SCL low time. This register can
only be written when the I2C bus is disabled (i.e. IC_ENABLE register is cleared).*/
//====================================================================

#define  I2C_IC_HS_SCL_LCNT_RESERVED1_MASK 0xffff0000
#define  I2C_IC_HS_SCL_LCNT_RESERVED1_SHIFT 16
#define  I2C_IC_HS_SCL_LCNT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_HS_SCL_LCNT_RESERVED1_MASK) >> I2C_IC_HS_SCL_LCNT_RESERVED1_SHIFT)
#define  I2C_IC_HS_SCL_LCNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_HS_SCL_LCNT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_HS_SCL_LCNT_RESERVED1_SHIFT))

#define  I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_MASK 0xffff
#define  I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_SHIFT 0
#define  I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_MASK_SHIFT(reg) (((reg) & I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_MASK) >> I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_SHIFT)
#define  I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_MASK) | (((uint32_t)val) << I2C_IC_HS_SCL_LCNT_IC_HS_SCL_LCNT_SHIFT))

//====================================================================
//Register: I2C Interrupt Status Register (IC_INTR_STAT)
/** \brief Register contains the current interrupt status. If a IRQ is masked in the IC_INTR_MASK register it will
not be shown here....*/
//====================================================================

#define  I2C_IC_INTR_STAT_RESERVED1_MASK 0xfffff000
#define  I2C_IC_INTR_STAT_RESERVED1_SHIFT 12
#define  I2C_IC_INTR_STAT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_RESERVED1_MASK) >> I2C_IC_INTR_STAT_RESERVED1_SHIFT)
#define  I2C_IC_INTR_STAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_RESERVED1_SHIFT))

#define  I2C_IC_INTR_STAT_R_GEN_CALL_MASK 0x800
#define  I2C_IC_INTR_STAT_R_GEN_CALL_SHIFT 11
#define  I2C_IC_INTR_STAT_R_GEN_CALL_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_GEN_CALL_MASK) >> I2C_IC_INTR_STAT_R_GEN_CALL_SHIFT)
#define  I2C_IC_INTR_STAT_R_GEN_CALL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_GEN_CALL_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_GEN_CALL_SHIFT))

#define  I2C_IC_INTR_STAT_R_START_DET_MASK 0x400
#define  I2C_IC_INTR_STAT_R_START_DET_SHIFT 10
#define  I2C_IC_INTR_STAT_R_START_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_START_DET_MASK) >> I2C_IC_INTR_STAT_R_START_DET_SHIFT)
#define  I2C_IC_INTR_STAT_R_START_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_START_DET_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_START_DET_SHIFT))

#define  I2C_IC_INTR_STAT_R_STOP_DET_MASK 0x200
#define  I2C_IC_INTR_STAT_R_STOP_DET_SHIFT 9
#define  I2C_IC_INTR_STAT_R_STOP_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_STOP_DET_MASK) >> I2C_IC_INTR_STAT_R_STOP_DET_SHIFT)
#define  I2C_IC_INTR_STAT_R_STOP_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_STOP_DET_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_STOP_DET_SHIFT))

#define  I2C_IC_INTR_STAT_R_ACTIVITY_MASK 0x100
#define  I2C_IC_INTR_STAT_R_ACTIVITY_SHIFT 8
#define  I2C_IC_INTR_STAT_R_ACTIVITY_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_ACTIVITY_MASK) >> I2C_IC_INTR_STAT_R_ACTIVITY_SHIFT)
#define  I2C_IC_INTR_STAT_R_ACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_ACTIVITY_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_ACTIVITY_SHIFT))

#define  I2C_IC_INTR_STAT_R_RX_DONE_MASK 0x80
#define  I2C_IC_INTR_STAT_R_RX_DONE_SHIFT 7
#define  I2C_IC_INTR_STAT_R_RX_DONE_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_RX_DONE_MASK) >> I2C_IC_INTR_STAT_R_RX_DONE_SHIFT)
#define  I2C_IC_INTR_STAT_R_RX_DONE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_RX_DONE_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_RX_DONE_SHIFT))

#define  I2C_IC_INTR_STAT_R_TX_ABRT_MASK 0x40
#define  I2C_IC_INTR_STAT_R_TX_ABRT_SHIFT 6
#define  I2C_IC_INTR_STAT_R_TX_ABRT_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_TX_ABRT_MASK) >> I2C_IC_INTR_STAT_R_TX_ABRT_SHIFT)
#define  I2C_IC_INTR_STAT_R_TX_ABRT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_TX_ABRT_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_TX_ABRT_SHIFT))

#define  I2C_IC_INTR_STAT_R_RD_REQ_MASK 0x20
#define  I2C_IC_INTR_STAT_R_RD_REQ_SHIFT 5
#define  I2C_IC_INTR_STAT_R_RD_REQ_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_RD_REQ_MASK) >> I2C_IC_INTR_STAT_R_RD_REQ_SHIFT)
#define  I2C_IC_INTR_STAT_R_RD_REQ_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_RD_REQ_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_RD_REQ_SHIFT))

#define  I2C_IC_INTR_STAT_R_TX_EMPTY_MASK 0x10
#define  I2C_IC_INTR_STAT_R_TX_EMPTY_SHIFT 4
#define  I2C_IC_INTR_STAT_R_TX_EMPTY_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_TX_EMPTY_MASK) >> I2C_IC_INTR_STAT_R_TX_EMPTY_SHIFT)
#define  I2C_IC_INTR_STAT_R_TX_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_TX_EMPTY_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_TX_EMPTY_SHIFT))

#define  I2C_IC_INTR_STAT_R_TX_OVER_MASK 0x8
#define  I2C_IC_INTR_STAT_R_TX_OVER_SHIFT 3
#define  I2C_IC_INTR_STAT_R_TX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_TX_OVER_MASK) >> I2C_IC_INTR_STAT_R_TX_OVER_SHIFT)
#define  I2C_IC_INTR_STAT_R_TX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_TX_OVER_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_TX_OVER_SHIFT))

#define  I2C_IC_INTR_STAT_R_RX_FULL_MASK 0x4
#define  I2C_IC_INTR_STAT_R_RX_FULL_SHIFT 2
#define  I2C_IC_INTR_STAT_R_RX_FULL_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_RX_FULL_MASK) >> I2C_IC_INTR_STAT_R_RX_FULL_SHIFT)
#define  I2C_IC_INTR_STAT_R_RX_FULL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_RX_FULL_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_RX_FULL_SHIFT))

#define  I2C_IC_INTR_STAT_R_RX_OVER_MASK 0x2
#define  I2C_IC_INTR_STAT_R_RX_OVER_SHIFT 1
#define  I2C_IC_INTR_STAT_R_RX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_RX_OVER_MASK) >> I2C_IC_INTR_STAT_R_RX_OVER_SHIFT)
#define  I2C_IC_INTR_STAT_R_RX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_RX_OVER_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_RX_OVER_SHIFT))

#define  I2C_IC_INTR_STAT_R_RX_UNDER_MASK 0x1
#define  I2C_IC_INTR_STAT_R_RX_UNDER_SHIFT 0
#define  I2C_IC_INTR_STAT_R_RX_UNDER_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_STAT_R_RX_UNDER_MASK) >> I2C_IC_INTR_STAT_R_RX_UNDER_SHIFT)
#define  I2C_IC_INTR_STAT_R_RX_UNDER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_STAT_R_RX_UNDER_MASK) | (((uint32_t)val) << I2C_IC_INTR_STAT_R_RX_UNDER_SHIFT))

//====================================================================
//Register: I2C Interrupt Mask Register (IC_INTR_MASK)
/** \brief Register contains the mask bits for the interrupts. Setting bit masks the IRQ.*/
//====================================================================

#define  I2C_IC_INTR_MASK_RESERVED1_MASK 0xfffff000
#define  I2C_IC_INTR_MASK_RESERVED1_SHIFT 12
#define  I2C_IC_INTR_MASK_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_RESERVED1_MASK) >> I2C_IC_INTR_MASK_RESERVED1_SHIFT)
#define  I2C_IC_INTR_MASK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_RESERVED1_SHIFT))

#define  I2C_IC_INTR_MASK_M_GEN_CALL_MASK 0x800
#define  I2C_IC_INTR_MASK_M_GEN_CALL_SHIFT 11
#define  I2C_IC_INTR_MASK_M_GEN_CALL_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_GEN_CALL_MASK) >> I2C_IC_INTR_MASK_M_GEN_CALL_SHIFT)
#define  I2C_IC_INTR_MASK_M_GEN_CALL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_GEN_CALL_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_GEN_CALL_SHIFT))

#define  I2C_IC_INTR_MASK_M_START_DET_MASK 0x400
#define  I2C_IC_INTR_MASK_M_START_DET_SHIFT 10
#define  I2C_IC_INTR_MASK_M_START_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_START_DET_MASK) >> I2C_IC_INTR_MASK_M_START_DET_SHIFT)
#define  I2C_IC_INTR_MASK_M_START_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_START_DET_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_START_DET_SHIFT))

#define  I2C_IC_INTR_MASK_M_STOP_DET_MASK 0x200
#define  I2C_IC_INTR_MASK_M_STOP_DET_SHIFT 9
#define  I2C_IC_INTR_MASK_M_STOP_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_STOP_DET_MASK) >> I2C_IC_INTR_MASK_M_STOP_DET_SHIFT)
#define  I2C_IC_INTR_MASK_M_STOP_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_STOP_DET_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_STOP_DET_SHIFT))

#define  I2C_IC_INTR_MASK_M_ACTIVITY_MASK 0x100
#define  I2C_IC_INTR_MASK_M_ACTIVITY_SHIFT 8
#define  I2C_IC_INTR_MASK_M_ACTIVITY_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_ACTIVITY_MASK) >> I2C_IC_INTR_MASK_M_ACTIVITY_SHIFT)
#define  I2C_IC_INTR_MASK_M_ACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_ACTIVITY_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_ACTIVITY_SHIFT))

#define  I2C_IC_INTR_MASK_M_RX_DONE_MASK 0x80
#define  I2C_IC_INTR_MASK_M_RX_DONE_SHIFT 7
#define  I2C_IC_INTR_MASK_M_RX_DONE_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_RX_DONE_MASK) >> I2C_IC_INTR_MASK_M_RX_DONE_SHIFT)
#define  I2C_IC_INTR_MASK_M_RX_DONE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_RX_DONE_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_RX_DONE_SHIFT))

#define  I2C_IC_INTR_MASK_M_TX_ABRT_MASK 0x40
#define  I2C_IC_INTR_MASK_M_TX_ABRT_SHIFT 6
#define  I2C_IC_INTR_MASK_M_TX_ABRT_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_TX_ABRT_MASK) >> I2C_IC_INTR_MASK_M_TX_ABRT_SHIFT)
#define  I2C_IC_INTR_MASK_M_TX_ABRT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_TX_ABRT_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_TX_ABRT_SHIFT))

#define  I2C_IC_INTR_MASK_M_RD_REQ_MASK 0x20
#define  I2C_IC_INTR_MASK_M_RD_REQ_SHIFT 5
#define  I2C_IC_INTR_MASK_M_RD_REQ_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_RD_REQ_MASK) >> I2C_IC_INTR_MASK_M_RD_REQ_SHIFT)
#define  I2C_IC_INTR_MASK_M_RD_REQ_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_RD_REQ_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_RD_REQ_SHIFT))

#define  I2C_IC_INTR_MASK_M_TX_EMPTY_MASK 0x10
#define  I2C_IC_INTR_MASK_M_TX_EMPTY_SHIFT 4
#define  I2C_IC_INTR_MASK_M_TX_EMPTY_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_TX_EMPTY_MASK) >> I2C_IC_INTR_MASK_M_TX_EMPTY_SHIFT)
#define  I2C_IC_INTR_MASK_M_TX_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_TX_EMPTY_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_TX_EMPTY_SHIFT))

#define  I2C_IC_INTR_MASK_M_TX_OVER_MASK 0x8
#define  I2C_IC_INTR_MASK_M_TX_OVER_SHIFT 3
#define  I2C_IC_INTR_MASK_M_TX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_TX_OVER_MASK) >> I2C_IC_INTR_MASK_M_TX_OVER_SHIFT)
#define  I2C_IC_INTR_MASK_M_TX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_TX_OVER_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_TX_OVER_SHIFT))

#define  I2C_IC_INTR_MASK_M_RX_FULL_MASK 0x4
#define  I2C_IC_INTR_MASK_M_RX_FULL_SHIFT 2
#define  I2C_IC_INTR_MASK_M_RX_FULL_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_RX_FULL_MASK) >> I2C_IC_INTR_MASK_M_RX_FULL_SHIFT)
#define  I2C_IC_INTR_MASK_M_RX_FULL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_RX_FULL_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_RX_FULL_SHIFT))

#define  I2C_IC_INTR_MASK_M_RX_OVER_MASK 0x2
#define  I2C_IC_INTR_MASK_M_RX_OVER_SHIFT 1
#define  I2C_IC_INTR_MASK_M_RX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_RX_OVER_MASK) >> I2C_IC_INTR_MASK_M_RX_OVER_SHIFT)
#define  I2C_IC_INTR_MASK_M_RX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_RX_OVER_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_RX_OVER_SHIFT))

#define  I2C_IC_INTR_MASK_M_RX_UNDER_MASK 0x1
#define  I2C_IC_INTR_MASK_M_RX_UNDER_SHIFT 0
#define  I2C_IC_INTR_MASK_M_RX_UNDER_MASK_SHIFT(reg) (((reg) & I2C_IC_INTR_MASK_M_RX_UNDER_MASK) >> I2C_IC_INTR_MASK_M_RX_UNDER_SHIFT)
#define  I2C_IC_INTR_MASK_M_RX_UNDER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_INTR_MASK_M_RX_UNDER_MASK) | (((uint32_t)val) << I2C_IC_INTR_MASK_M_RX_UNDER_SHIFT))

//====================================================================
//Register: I2C Raw Interrupt Status Register (IC_RAW_INTR_STAT)
/** \brief Unlike the IC_INTR_STATUS register these bits are not masked so they always show the true staus of the I2C.*/
//====================================================================

#define  I2C_IC_RAW_INTR_STAT_RESERVED1_MASK 0xfffff000
#define  I2C_IC_RAW_INTR_STAT_RESERVED1_SHIFT 12
#define  I2C_IC_RAW_INTR_STAT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_RESERVED1_MASK) >> I2C_IC_RAW_INTR_STAT_RESERVED1_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_RESERVED1_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_GEN_CALL_MASK 0x800
#define  I2C_IC_RAW_INTR_STAT_GEN_CALL_SHIFT 11
#define  I2C_IC_RAW_INTR_STAT_GEN_CALL_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_GEN_CALL_MASK) >> I2C_IC_RAW_INTR_STAT_GEN_CALL_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_GEN_CALL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_GEN_CALL_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_GEN_CALL_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_START_DET_MASK 0x400
#define  I2C_IC_RAW_INTR_STAT_START_DET_SHIFT 10
#define  I2C_IC_RAW_INTR_STAT_START_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_START_DET_MASK) >> I2C_IC_RAW_INTR_STAT_START_DET_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_START_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_START_DET_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_START_DET_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_STOP_DET_MASK 0x200
#define  I2C_IC_RAW_INTR_STAT_STOP_DET_SHIFT 9
#define  I2C_IC_RAW_INTR_STAT_STOP_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_STOP_DET_MASK) >> I2C_IC_RAW_INTR_STAT_STOP_DET_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_STOP_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_STOP_DET_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_STOP_DET_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_ACTIVITY_MASK 0x100
#define  I2C_IC_RAW_INTR_STAT_ACTIVITY_SHIFT 8
#define  I2C_IC_RAW_INTR_STAT_ACTIVITY_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_ACTIVITY_MASK) >> I2C_IC_RAW_INTR_STAT_ACTIVITY_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_ACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_ACTIVITY_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_ACTIVITY_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_RX_DONE_MASK 0x80
#define  I2C_IC_RAW_INTR_STAT_RX_DONE_SHIFT 7
#define  I2C_IC_RAW_INTR_STAT_RX_DONE_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_RX_DONE_MASK) >> I2C_IC_RAW_INTR_STAT_RX_DONE_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_RX_DONE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_RX_DONE_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_RX_DONE_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_TX_ABRT_MASK 0x40
#define  I2C_IC_RAW_INTR_STAT_TX_ABRT_SHIFT 6
#define  I2C_IC_RAW_INTR_STAT_TX_ABRT_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_TX_ABRT_MASK) >> I2C_IC_RAW_INTR_STAT_TX_ABRT_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_TX_ABRT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_TX_ABRT_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_TX_ABRT_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_RD_REQ_MASK 0x20
#define  I2C_IC_RAW_INTR_STAT_RD_REQ_SHIFT 5
#define  I2C_IC_RAW_INTR_STAT_RD_REQ_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_RD_REQ_MASK) >> I2C_IC_RAW_INTR_STAT_RD_REQ_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_RD_REQ_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_RD_REQ_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_RD_REQ_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_TX_EMPTY_MASK 0x10
#define  I2C_IC_RAW_INTR_STAT_TX_EMPTY_SHIFT 4
#define  I2C_IC_RAW_INTR_STAT_TX_EMPTY_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_TX_EMPTY_MASK) >> I2C_IC_RAW_INTR_STAT_TX_EMPTY_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_TX_EMPTY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_TX_EMPTY_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_TX_EMPTY_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_TX_OVER_MASK 0x8
#define  I2C_IC_RAW_INTR_STAT_TX_OVER_SHIFT 3
#define  I2C_IC_RAW_INTR_STAT_TX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_TX_OVER_MASK) >> I2C_IC_RAW_INTR_STAT_TX_OVER_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_TX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_TX_OVER_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_TX_OVER_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_RX_FULL_MASK 0x4
#define  I2C_IC_RAW_INTR_STAT_RX_FULL_SHIFT 2
#define  I2C_IC_RAW_INTR_STAT_RX_FULL_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_RX_FULL_MASK) >> I2C_IC_RAW_INTR_STAT_RX_FULL_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_RX_FULL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_RX_FULL_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_RX_FULL_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_RX_OVER_MASK 0x2
#define  I2C_IC_RAW_INTR_STAT_RX_OVER_SHIFT 1
#define  I2C_IC_RAW_INTR_STAT_RX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_RX_OVER_MASK) >> I2C_IC_RAW_INTR_STAT_RX_OVER_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_RX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_RX_OVER_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_RX_OVER_SHIFT))

#define  I2C_IC_RAW_INTR_STAT_RX_UNDER_MASK 0x1
#define  I2C_IC_RAW_INTR_STAT_RX_UNDER_SHIFT 0
#define  I2C_IC_RAW_INTR_STAT_RX_UNDER_MASK_SHIFT(reg) (((reg) & I2C_IC_RAW_INTR_STAT_RX_UNDER_MASK) >> I2C_IC_RAW_INTR_STAT_RX_UNDER_SHIFT)
#define  I2C_IC_RAW_INTR_STAT_RX_UNDER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RAW_INTR_STAT_RX_UNDER_MASK) | (((uint32_t)val) << I2C_IC_RAW_INTR_STAT_RX_UNDER_SHIFT))

//====================================================================
//Register: Rx FIFO Full Threshold (IC_RX_TL)
/** \brief Controls the level on Rx FIFO entries (or above) that triggers the RX_FULL IRQ.*/
//====================================================================

#define  I2C_IC_RX_TL_RESERVED1_MASK 0xffffff00
#define  I2C_IC_RX_TL_RESERVED1_SHIFT 8
#define  I2C_IC_RX_TL_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_RX_TL_RESERVED1_MASK) >> I2C_IC_RX_TL_RESERVED1_SHIFT)
#define  I2C_IC_RX_TL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RX_TL_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_RX_TL_RESERVED1_SHIFT))

#define  I2C_IC_RX_TL_RX_TL_MASK 0xff
#define  I2C_IC_RX_TL_RX_TL_SHIFT 0
#define  I2C_IC_RX_TL_RX_TL_MASK_SHIFT(reg) (((reg) & I2C_IC_RX_TL_RX_TL_MASK) >> I2C_IC_RX_TL_RX_TL_SHIFT)
#define  I2C_IC_RX_TL_RX_TL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RX_TL_RX_TL_MASK) | (((uint32_t)val) << I2C_IC_RX_TL_RX_TL_SHIFT))

//====================================================================
//Register: Tx FIFO Empty Threshold (IC_TX_TL)
/** \brief Controls the level on Tx FIFO entries (or below) that triggers the TX_EMPTY IRQ.*/
//====================================================================

#define  I2C_IC_TX_TL_RESERVED1_MASK 0xffffff00
#define  I2C_IC_TX_TL_RESERVED1_SHIFT 8
#define  I2C_IC_TX_TL_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_TL_RESERVED1_MASK) >> I2C_IC_TX_TL_RESERVED1_SHIFT)
#define  I2C_IC_TX_TL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_TL_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_TX_TL_RESERVED1_SHIFT))

#define  I2C_IC_TX_TL_TX_TL_MASK 0xff
#define  I2C_IC_TX_TL_TX_TL_SHIFT 0
#define  I2C_IC_TX_TL_TX_TL_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_TL_TX_TL_MASK) >> I2C_IC_TX_TL_TX_TL_SHIFT)
#define  I2C_IC_TX_TL_TX_TL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_TL_TX_TL_MASK) | (((uint32_t)val) << I2C_IC_TX_TL_TX_TL_SHIFT))

//====================================================================
//Register: Clear Combined or Individual IRQ Register (IC_CLR_INTR)
/** \brief Read only register for clearing combined or individual IRQ's.*/
//====================================================================

#define  I2C_IC_CLR_INTR_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_INTR_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_INTR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_INTR_RESERVED1_MASK) >> I2C_IC_CLR_INTR_RESERVED1_SHIFT)
#define  I2C_IC_CLR_INTR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_INTR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_INTR_RESERVED1_SHIFT))

#define  I2C_IC_CLR_INTR_CLR_INTR_MASK 0x1
#define  I2C_IC_CLR_INTR_CLR_INTR_SHIFT 0
#define  I2C_IC_CLR_INTR_CLR_INTR_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_INTR_CLR_INTR_MASK) >> I2C_IC_CLR_INTR_CLR_INTR_SHIFT)
#define  I2C_IC_CLR_INTR_CLR_INTR_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_INTR_CLR_INTR_MASK) | (((uint32_t)val) << I2C_IC_CLR_INTR_CLR_INTR_SHIFT))

//====================================================================
//Register: Clear Rx Underrun IRQ Register (IC_CLR_RX_UNDER)
/** \brief Read only register for clearing Rx Underrun IRQ.*/
//====================================================================

#define  I2C_IC_CLR_RX_UNDER_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_RX_UNDER_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_RX_UNDER_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RX_UNDER_RESERVED1_MASK) >> I2C_IC_CLR_RX_UNDER_RESERVED1_SHIFT)
#define  I2C_IC_CLR_RX_UNDER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RX_UNDER_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_RX_UNDER_RESERVED1_SHIFT))

#define  I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_MASK 0x1
#define  I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_SHIFT 0
#define  I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_MASK) >> I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_SHIFT)
#define  I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_MASK) | (((uint32_t)val) << I2C_IC_CLR_RX_UNDER_CLR_RX_UNDER_SHIFT))

//====================================================================
//Register: Clear Rx Overrun IRQ Register (IC_CLR_RX_OVER)
/** \brief Read only register for clearing Rx Overrun IRQ.*/
//====================================================================

#define  I2C_IC_CLR_RX_OVER_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_RX_OVER_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_RX_OVER_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RX_OVER_RESERVED1_MASK) >> I2C_IC_CLR_RX_OVER_RESERVED1_SHIFT)
#define  I2C_IC_CLR_RX_OVER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RX_OVER_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_RX_OVER_RESERVED1_SHIFT))

#define  I2C_IC_CLR_RX_OVER_CLR_RX_OVER_MASK 0x1
#define  I2C_IC_CLR_RX_OVER_CLR_RX_OVER_SHIFT 0
#define  I2C_IC_CLR_RX_OVER_CLR_RX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RX_OVER_CLR_RX_OVER_MASK) >> I2C_IC_CLR_RX_OVER_CLR_RX_OVER_SHIFT)
#define  I2C_IC_CLR_RX_OVER_CLR_RX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RX_OVER_CLR_RX_OVER_MASK) | (((uint32_t)val) << I2C_IC_CLR_RX_OVER_CLR_RX_OVER_SHIFT))

//====================================================================
//Register: Clear Tx Overrun IRQ Register (IC_CLR_TX_OVER)
/** \brief Read only register for clearing Tx Overrun IRQ.*/
//====================================================================

#define  I2C_IC_CLR_TX_OVER_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_TX_OVER_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_TX_OVER_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_TX_OVER_RESERVED1_MASK) >> I2C_IC_CLR_TX_OVER_RESERVED1_SHIFT)
#define  I2C_IC_CLR_TX_OVER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_TX_OVER_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_TX_OVER_RESERVED1_SHIFT))

#define  I2C_IC_CLR_TX_OVER_CLR_TX_OVER_MASK 0x1
#define  I2C_IC_CLR_TX_OVER_CLR_TX_OVER_SHIFT 0
#define  I2C_IC_CLR_TX_OVER_CLR_TX_OVER_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_TX_OVER_CLR_TX_OVER_MASK) >> I2C_IC_CLR_TX_OVER_CLR_TX_OVER_SHIFT)
#define  I2C_IC_CLR_TX_OVER_CLR_TX_OVER_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_TX_OVER_CLR_TX_OVER_MASK) | (((uint32_t)val) << I2C_IC_CLR_TX_OVER_CLR_TX_OVER_SHIFT))

//====================================================================
//Register: Clear Read Request IRQ Register (IC_CLR_RD_REQ)
/** \brief Read only register for clearing Slave Read Request IRQ.*/
//====================================================================

#define  I2C_IC_CLR_RD_REQ_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_RD_REQ_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_RD_REQ_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RD_REQ_RESERVED1_MASK) >> I2C_IC_CLR_RD_REQ_RESERVED1_SHIFT)
#define  I2C_IC_CLR_RD_REQ_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RD_REQ_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_RD_REQ_RESERVED1_SHIFT))

#define  I2C_IC_CLR_RD_REQ_CLR_RD_REQ_MASK 0x1
#define  I2C_IC_CLR_RD_REQ_CLR_RD_REQ_SHIFT 0
#define  I2C_IC_CLR_RD_REQ_CLR_RD_REQ_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RD_REQ_CLR_RD_REQ_MASK) >> I2C_IC_CLR_RD_REQ_CLR_RD_REQ_SHIFT)
#define  I2C_IC_CLR_RD_REQ_CLR_RD_REQ_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RD_REQ_CLR_RD_REQ_MASK) | (((uint32_t)val) << I2C_IC_CLR_RD_REQ_CLR_RD_REQ_SHIFT))

//====================================================================
//Register: Clear Tx Abort IRQ Register (IC_CLR_TX_ABRT)
/** \brief Read only register for clearing Tx Abort IRQ.*/
//====================================================================

#define  I2C_IC_CLR_TX_ABRT_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_TX_ABRT_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_TX_ABRT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_TX_ABRT_RESERVED1_MASK) >> I2C_IC_CLR_TX_ABRT_RESERVED1_SHIFT)
#define  I2C_IC_CLR_TX_ABRT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_TX_ABRT_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_TX_ABRT_RESERVED1_SHIFT))

#define  I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_MASK 0x1
#define  I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_SHIFT 0
#define  I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_MASK) >> I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_SHIFT)
#define  I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_MASK) | (((uint32_t)val) << I2C_IC_CLR_TX_ABRT_CLR_TX_ABRT_SHIFT))

//====================================================================
//Register: Clear Rx Done IRQ Register (IC_CLR_RX_DONE)
/** \brief Read only register for clearing Rx Done IRQ.*/
//====================================================================

#define  I2C_IC_CLR_RX_DONE_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_RX_DONE_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_RX_DONE_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RX_DONE_RESERVED1_MASK) >> I2C_IC_CLR_RX_DONE_RESERVED1_SHIFT)
#define  I2C_IC_CLR_RX_DONE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RX_DONE_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_RX_DONE_RESERVED1_SHIFT))

#define  I2C_IC_CLR_RX_DONE_CLR_RX_DONE_MASK 0x1
#define  I2C_IC_CLR_RX_DONE_CLR_RX_DONE_SHIFT 0
#define  I2C_IC_CLR_RX_DONE_CLR_RX_DONE_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_RX_DONE_CLR_RX_DONE_MASK) >> I2C_IC_CLR_RX_DONE_CLR_RX_DONE_SHIFT)
#define  I2C_IC_CLR_RX_DONE_CLR_RX_DONE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_RX_DONE_CLR_RX_DONE_MASK) | (((uint32_t)val) << I2C_IC_CLR_RX_DONE_CLR_RX_DONE_SHIFT))

//====================================================================
//Register: Clear Activity IRQ Register (IC_CLR_ACTIVITY)
/** \brief Read only register for clearing the Activity IRQ.*/
//====================================================================

#define  I2C_IC_CLR_ACTIVITY_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_ACTIVITY_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_ACTIVITY_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_ACTIVITY_RESERVED1_MASK) >> I2C_IC_CLR_ACTIVITY_RESERVED1_SHIFT)
#define  I2C_IC_CLR_ACTIVITY_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_ACTIVITY_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_ACTIVITY_RESERVED1_SHIFT))

#define  I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_MASK 0x1
#define  I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_SHIFT 0
#define  I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_MASK) >> I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_SHIFT)
#define  I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_MASK) | (((uint32_t)val) << I2C_IC_CLR_ACTIVITY_CLR_ACTIVITY_SHIFT))

//====================================================================
//Register: Clear STOP Detect IRQ Register (IC_CLR_STOP_DET)
/** \brief Read only register for clearing Stop Detect IRQ.*/
//====================================================================

#define  I2C_IC_CLR_STOP_DET_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_STOP_DET_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_STOP_DET_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_STOP_DET_RESERVED1_MASK) >> I2C_IC_CLR_STOP_DET_RESERVED1_SHIFT)
#define  I2C_IC_CLR_STOP_DET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_STOP_DET_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_STOP_DET_RESERVED1_SHIFT))

#define  I2C_IC_CLR_STOP_DET_CLR_STOP_DET_MASK 0x1
#define  I2C_IC_CLR_STOP_DET_CLR_STOP_DET_SHIFT 0
#define  I2C_IC_CLR_STOP_DET_CLR_STOP_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_STOP_DET_CLR_STOP_DET_MASK) >> I2C_IC_CLR_STOP_DET_CLR_STOP_DET_SHIFT)
#define  I2C_IC_CLR_STOP_DET_CLR_STOP_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_STOP_DET_CLR_STOP_DET_MASK) | (((uint32_t)val) << I2C_IC_CLR_STOP_DET_CLR_STOP_DET_SHIFT))

//====================================================================
//Register: Clear START Detect IRQ Register (IC_CLR_START_DET)
/** \brief Read only register for clearing the START Detect IRQ.*/
//====================================================================

#define  I2C_IC_CLR_START_DET_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_START_DET_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_START_DET_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_START_DET_RESERVED1_MASK) >> I2C_IC_CLR_START_DET_RESERVED1_SHIFT)
#define  I2C_IC_CLR_START_DET_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_START_DET_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_START_DET_RESERVED1_SHIFT))

#define  I2C_IC_CLR_START_DET_CLR_START_DET_MASK 0x1
#define  I2C_IC_CLR_START_DET_CLR_START_DET_SHIFT 0
#define  I2C_IC_CLR_START_DET_CLR_START_DET_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_START_DET_CLR_START_DET_MASK) >> I2C_IC_CLR_START_DET_CLR_START_DET_SHIFT)
#define  I2C_IC_CLR_START_DET_CLR_START_DET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_START_DET_CLR_START_DET_MASK) | (((uint32_t)val) << I2C_IC_CLR_START_DET_CLR_START_DET_SHIFT))

//====================================================================
//Register: Clear General Call IRQ Register (IC_CLR_GEN_CALL)
/** \brief Read only register for clearing General Call IRQ.*/
//====================================================================

#define  I2C_IC_CLR_GEN_CALL_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_CLR_GEN_CALL_RESERVED1_SHIFT 1
#define  I2C_IC_CLR_GEN_CALL_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_GEN_CALL_RESERVED1_MASK) >> I2C_IC_CLR_GEN_CALL_RESERVED1_SHIFT)
#define  I2C_IC_CLR_GEN_CALL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_GEN_CALL_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_CLR_GEN_CALL_RESERVED1_SHIFT))

#define  I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_MASK 0x1
#define  I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_SHIFT 0
#define  I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_MASK_SHIFT(reg) (((reg) & I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_MASK) >> I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_SHIFT)
#define  I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_MASK) | (((uint32_t)val) << I2C_IC_CLR_GEN_CALL_CLR_GEN_CALL_SHIFT))

//====================================================================
//Register: I2C Enable Register (IC_ENABLE)
/** \brief Controls whether the I2C block is enabled*/
//====================================================================

#define  I2C_IC_ENABLE_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_ENABLE_RESERVED1_SHIFT 1
#define  I2C_IC_ENABLE_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_ENABLE_RESERVED1_MASK) >> I2C_IC_ENABLE_RESERVED1_SHIFT)
#define  I2C_IC_ENABLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ENABLE_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_ENABLE_RESERVED1_SHIFT))

#define  I2C_IC_ENABLE_ENABLE_MASK 0x1
#define  I2C_IC_ENABLE_ENABLE_SHIFT 0
#define  I2C_IC_ENABLE_ENABLE_MASK_SHIFT(reg) (((reg) & I2C_IC_ENABLE_ENABLE_MASK) >> I2C_IC_ENABLE_ENABLE_SHIFT)
#define  I2C_IC_ENABLE_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ENABLE_ENABLE_MASK) | (((uint32_t)val) << I2C_IC_ENABLE_ENABLE_SHIFT))

//====================================================================
//Register: I2C Status Register (IC_STATUS)
/** \brief Read only register used to indicate the current transfer status.*/
//====================================================================

#define  I2C_IC_STATUS_RESERVED1_MASK 0xffffff80
#define  I2C_IC_STATUS_RESERVED1_SHIFT 7
#define  I2C_IC_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_RESERVED1_MASK) >> I2C_IC_STATUS_RESERVED1_SHIFT)
#define  I2C_IC_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_STATUS_RESERVED1_SHIFT))

#define  I2C_IC_STATUS_SLV_ACTIVITY_MASK 0x40
#define  I2C_IC_STATUS_SLV_ACTIVITY_SHIFT 6
#define  I2C_IC_STATUS_SLV_ACTIVITY_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_SLV_ACTIVITY_MASK) >> I2C_IC_STATUS_SLV_ACTIVITY_SHIFT)
#define  I2C_IC_STATUS_SLV_ACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_SLV_ACTIVITY_MASK) | (((uint32_t)val) << I2C_IC_STATUS_SLV_ACTIVITY_SHIFT))

#define  I2C_IC_STATUS_MST_ACTIVITY_MASK 0x20
#define  I2C_IC_STATUS_MST_ACTIVITY_SHIFT 5
#define  I2C_IC_STATUS_MST_ACTIVITY_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_MST_ACTIVITY_MASK) >> I2C_IC_STATUS_MST_ACTIVITY_SHIFT)
#define  I2C_IC_STATUS_MST_ACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_MST_ACTIVITY_MASK) | (((uint32_t)val) << I2C_IC_STATUS_MST_ACTIVITY_SHIFT))

#define  I2C_IC_STATUS_RFF_MASK 0x10
#define  I2C_IC_STATUS_RFF_SHIFT 4
#define  I2C_IC_STATUS_RFF_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_RFF_MASK) >> I2C_IC_STATUS_RFF_SHIFT)
#define  I2C_IC_STATUS_RFF_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_RFF_MASK) | (((uint32_t)val) << I2C_IC_STATUS_RFF_SHIFT))

#define  I2C_IC_STATUS_RFNE_MASK 0x8
#define  I2C_IC_STATUS_RFNE_SHIFT 3
#define  I2C_IC_STATUS_RFNE_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_RFNE_MASK) >> I2C_IC_STATUS_RFNE_SHIFT)
#define  I2C_IC_STATUS_RFNE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_RFNE_MASK) | (((uint32_t)val) << I2C_IC_STATUS_RFNE_SHIFT))

#define  I2C_IC_STATUS_TFE_MASK 0x4
#define  I2C_IC_STATUS_TFE_SHIFT 2
#define  I2C_IC_STATUS_TFE_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_TFE_MASK) >> I2C_IC_STATUS_TFE_SHIFT)
#define  I2C_IC_STATUS_TFE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_TFE_MASK) | (((uint32_t)val) << I2C_IC_STATUS_TFE_SHIFT))

#define  I2C_IC_STATUS_TFNF_MASK 0x2
#define  I2C_IC_STATUS_TFNF_SHIFT 1
#define  I2C_IC_STATUS_TFNF_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_TFNF_MASK) >> I2C_IC_STATUS_TFNF_SHIFT)
#define  I2C_IC_STATUS_TFNF_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_TFNF_MASK) | (((uint32_t)val) << I2C_IC_STATUS_TFNF_SHIFT))

#define  I2C_IC_STATUS_ACTIVITY_MASK 0x1
#define  I2C_IC_STATUS_ACTIVITY_SHIFT 0
#define  I2C_IC_STATUS_ACTIVITY_MASK_SHIFT(reg) (((reg) & I2C_IC_STATUS_ACTIVITY_MASK) >> I2C_IC_STATUS_ACTIVITY_SHIFT)
#define  I2C_IC_STATUS_ACTIVITY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_STATUS_ACTIVITY_MASK) | (((uint32_t)val) << I2C_IC_STATUS_ACTIVITY_SHIFT))

//====================================================================
//Register: Tx FIFO Level Register (IC_TXFLR)
/** \brief Indicates the number of valid entries in the Tx FIFO*/
//====================================================================

#define  I2C_IC_TXFLR_RESERVED1_MASK 0xfffffff0
#define  I2C_IC_TXFLR_RESERVED1_SHIFT 4
#define  I2C_IC_TXFLR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_TXFLR_RESERVED1_MASK) >> I2C_IC_TXFLR_RESERVED1_SHIFT)
#define  I2C_IC_TXFLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TXFLR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_TXFLR_RESERVED1_SHIFT))

#define  I2C_IC_TXFLR_TXFLR_MASK 0xf
#define  I2C_IC_TXFLR_TXFLR_SHIFT 0
#define  I2C_IC_TXFLR_TXFLR_MASK_SHIFT(reg) (((reg) & I2C_IC_TXFLR_TXFLR_MASK) >> I2C_IC_TXFLR_TXFLR_SHIFT)
#define  I2C_IC_TXFLR_TXFLR_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TXFLR_TXFLR_MASK) | (((uint32_t)val) << I2C_IC_TXFLR_TXFLR_SHIFT))

//====================================================================
//Register: Rx FIFO Level Register (IC_RXFLR)
/** \brief Indicates the number of valid entries in the Rx FIFO*/
//====================================================================

#define  I2C_IC_RXFLR_RESERVED1_MASK 0xfffffff0
#define  I2C_IC_RXFLR_RESERVED1_SHIFT 4
#define  I2C_IC_RXFLR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_RXFLR_RESERVED1_MASK) >> I2C_IC_RXFLR_RESERVED1_SHIFT)
#define  I2C_IC_RXFLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RXFLR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_RXFLR_RESERVED1_SHIFT))

#define  I2C_IC_RXFLR_RXFLR_MASK 0xf
#define  I2C_IC_RXFLR_RXFLR_SHIFT 0
#define  I2C_IC_RXFLR_RXFLR_MASK_SHIFT(reg) (((reg) & I2C_IC_RXFLR_RXFLR_MASK) >> I2C_IC_RXFLR_RXFLR_SHIFT)
#define  I2C_IC_RXFLR_RXFLR_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_RXFLR_RXFLR_MASK) | (((uint32_t)val) << I2C_IC_RXFLR_RXFLR_SHIFT))

//====================================================================
//Register: Tx Abort Source Register (IC_TX_ABRT_SOURCE)
/** \brief 16 bit register indicating the source of a TX_ABRT IRQ.*/
//====================================================================

#define  I2C_IC_TX_ABRT_SOURCE_RESERVED1_MASK 0xffff0000
#define  I2C_IC_TX_ABRT_SOURCE_RESERVED1_SHIFT 16
#define  I2C_IC_TX_ABRT_SOURCE_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_RESERVED1_MASK) >> I2C_IC_TX_ABRT_SOURCE_RESERVED1_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_RESERVED1_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_MASK 0x8000
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_SHIFT 15
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_SLVRD_INTX_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_MASK 0x4000
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_SHIFT 14
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_SLV_ARBLOST_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_MASK 0x2000
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_SHIFT 13
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_SLVFLUSH_TXFIFO_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ARB_LOST_MASK 0x1000
#define  I2C_IC_TX_ABRT_SOURCE_ARB_LOST_SHIFT 12
#define  I2C_IC_TX_ABRT_SOURCE_ARB_LOST_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ARB_LOST_MASK) >> I2C_IC_TX_ABRT_SOURCE_ARB_LOST_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ARB_LOST_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ARB_LOST_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ARB_LOST_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_MASK 0x800
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_SHIFT 11
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_MASTER_DIS_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_MASK 0x400
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_SHIFT 10
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_10B_RD_NORSTRT_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_MASK 0x200
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_SHIFT 9
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_NORSTRT_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_MASK 0x100
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_SHIFT 8
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_HS_NORSTRT_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_MASK 0x80
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_SHIFT 7
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_SBYTE_ACKDET_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_MASK 0x40
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_SHIFT 6
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_HS_ACKDET_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_MASK 0x20
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_SHIFT 5
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_READ_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_MASK 0x10
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_SHIFT 4
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_GCALL_NOACK_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_MASK 0x8
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_SHIFT 3
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_TXDATA_NOACK_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_MASK 0x4
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_SHIFT 2
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR2_NOACK_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_MASK 0x2
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_SHIFT 1
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_10ADDR1_NOACK_SHIFT))

#define  I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_MASK 0x1
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_SHIFT 0
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_MASK_SHIFT(reg) (((reg) & I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_MASK) >> I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_SHIFT)
#define  I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_MASK) | (((uint32_t)val) << I2C_IC_TX_ABRT_SOURCE_ABRT_7B_ADDR_NOACK_SHIFT))

//====================================================================
//Register: Generate Slave Data NACK Register (IC_SLV_DATA_NACK_ONLY)
/** \brief Used to generate a NACK for the data part of a transfer when I2C is acting as a Slave-receiver.*/
//====================================================================

#define  I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_SHIFT 1
#define  I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_MASK) >> I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_SHIFT)
#define  I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_SLV_DATA_NACK_ONLY_RESERVED1_SHIFT))

#define  I2C_IC_SLV_DATA_NACK_ONLY_NACK_MASK 0x1
#define  I2C_IC_SLV_DATA_NACK_ONLY_NACK_SHIFT 0
#define  I2C_IC_SLV_DATA_NACK_ONLY_NACK_MASK_SHIFT(reg) (((reg) & I2C_IC_SLV_DATA_NACK_ONLY_NACK_MASK) >> I2C_IC_SLV_DATA_NACK_ONLY_NACK_SHIFT)
#define  I2C_IC_SLV_DATA_NACK_ONLY_NACK_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SLV_DATA_NACK_ONLY_NACK_MASK) | (((uint32_t)val) << I2C_IC_SLV_DATA_NACK_ONLY_NACK_SHIFT))

//====================================================================
//Register: DMA Control Register (IC_DMA_CR)
/** \brief This register is used to enable the DMA Controller interface operation.*/
//====================================================================

#define  I2C_IC_DMA_CR_RESERVED1_MASK 0xfffffffc
#define  I2C_IC_DMA_CR_RESERVED1_SHIFT 2
#define  I2C_IC_DMA_CR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_DMA_CR_RESERVED1_MASK) >> I2C_IC_DMA_CR_RESERVED1_SHIFT)
#define  I2C_IC_DMA_CR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DMA_CR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_DMA_CR_RESERVED1_SHIFT))

#define  I2C_IC_DMA_CR_TDMAE_MASK 0x2
#define  I2C_IC_DMA_CR_TDMAE_SHIFT 1
#define  I2C_IC_DMA_CR_TDMAE_MASK_SHIFT(reg) (((reg) & I2C_IC_DMA_CR_TDMAE_MASK) >> I2C_IC_DMA_CR_TDMAE_SHIFT)
#define  I2C_IC_DMA_CR_TDMAE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DMA_CR_TDMAE_MASK) | (((uint32_t)val) << I2C_IC_DMA_CR_TDMAE_SHIFT))

#define  I2C_IC_DMA_CR_RDMAE_MASK 0x1
#define  I2C_IC_DMA_CR_RDMAE_SHIFT 0
#define  I2C_IC_DMA_CR_RDMAE_MASK_SHIFT(reg) (((reg) & I2C_IC_DMA_CR_RDMAE_MASK) >> I2C_IC_DMA_CR_RDMAE_SHIFT)
#define  I2C_IC_DMA_CR_RDMAE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DMA_CR_RDMAE_MASK) | (((uint32_t)val) << I2C_IC_DMA_CR_RDMAE_SHIFT))

//====================================================================
//Register: DMA Transmit Data Level Register (IC_DMA_TDLR)
/** \brief This register controls the TX FIFO watermark level at which a DMA request is generated.*/
//====================================================================

#define  I2C_IC_DMA_TDLR_RESERVED1_MASK 0xfffffff8
#define  I2C_IC_DMA_TDLR_RESERVED1_SHIFT 3
#define  I2C_IC_DMA_TDLR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_DMA_TDLR_RESERVED1_MASK) >> I2C_IC_DMA_TDLR_RESERVED1_SHIFT)
#define  I2C_IC_DMA_TDLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DMA_TDLR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_DMA_TDLR_RESERVED1_SHIFT))

#define  I2C_IC_DMA_TDLR_DMATDL_MASK 0x7
#define  I2C_IC_DMA_TDLR_DMATDL_SHIFT 0
#define  I2C_IC_DMA_TDLR_DMATDL_MASK_SHIFT(reg) (((reg) & I2C_IC_DMA_TDLR_DMATDL_MASK) >> I2C_IC_DMA_TDLR_DMATDL_SHIFT)
#define  I2C_IC_DMA_TDLR_DMATDL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DMA_TDLR_DMATDL_MASK) | (((uint32_t)val) << I2C_IC_DMA_TDLR_DMATDL_SHIFT))

//====================================================================
//Register: DMA Receive Data Level Register (IC_DMA_RDLR)
/** \brief This register controls the RX FIFO watermark level at which a DMA request is generated.*/
//====================================================================

#define  I2C_IC_DMA_RDLR_RESERVED1_MASK 0xfffffff8
#define  I2C_IC_DMA_RDLR_RESERVED1_SHIFT 3
#define  I2C_IC_DMA_RDLR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_DMA_RDLR_RESERVED1_MASK) >> I2C_IC_DMA_RDLR_RESERVED1_SHIFT)
#define  I2C_IC_DMA_RDLR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DMA_RDLR_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_DMA_RDLR_RESERVED1_SHIFT))

#define  I2C_IC_DMA_RDLR_DMARDL_MASK 0x7
#define  I2C_IC_DMA_RDLR_DMARDL_SHIFT 0
#define  I2C_IC_DMA_RDLR_DMARDL_MASK_SHIFT(reg) (((reg) & I2C_IC_DMA_RDLR_DMARDL_MASK) >> I2C_IC_DMA_RDLR_DMARDL_SHIFT)
#define  I2C_IC_DMA_RDLR_DMARDL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_DMA_RDLR_DMARDL_MASK) | (((uint32_t)val) << I2C_IC_DMA_RDLR_DMARDL_SHIFT))

//====================================================================
//Register: SDA Setup Timing Register (IC_SDA_SETUP)
/** \brief This register controls the amount of time delay in (ic_clk periods) introdiced in the rising edge of SCL, relative to SDA changing.*/
//====================================================================

#define  I2C_IC_SDA_SETUP_RESERVED1_MASK 0xffffff00
#define  I2C_IC_SDA_SETUP_RESERVED1_SHIFT 8
#define  I2C_IC_SDA_SETUP_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_SDA_SETUP_RESERVED1_MASK) >> I2C_IC_SDA_SETUP_RESERVED1_SHIFT)
#define  I2C_IC_SDA_SETUP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SDA_SETUP_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_SDA_SETUP_RESERVED1_SHIFT))

#define  I2C_IC_SDA_SETUP_SDA_SETUP_MASK 0xff
#define  I2C_IC_SDA_SETUP_SDA_SETUP_SHIFT 0
#define  I2C_IC_SDA_SETUP_SDA_SETUP_MASK_SHIFT(reg) (((reg) & I2C_IC_SDA_SETUP_SDA_SETUP_MASK) >> I2C_IC_SDA_SETUP_SDA_SETUP_SHIFT)
#define  I2C_IC_SDA_SETUP_SDA_SETUP_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_SDA_SETUP_SDA_SETUP_MASK) | (((uint32_t)val) << I2C_IC_SDA_SETUP_SDA_SETUP_SHIFT))

//====================================================================
//Register: General call Register (IC_ACK_GENERAL_CALL)
/** \brief Thsi register controls whether the I2C responds with ACK or NACK when a General Call is received.*/
//====================================================================

#define  I2C_IC_ACK_GENERAL_CALL_RESERVED1_MASK 0xfffffffe
#define  I2C_IC_ACK_GENERAL_CALL_RESERVED1_SHIFT 1
#define  I2C_IC_ACK_GENERAL_CALL_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_ACK_GENERAL_CALL_RESERVED1_MASK) >> I2C_IC_ACK_GENERAL_CALL_RESERVED1_SHIFT)
#define  I2C_IC_ACK_GENERAL_CALL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ACK_GENERAL_CALL_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_ACK_GENERAL_CALL_RESERVED1_SHIFT))

#define  I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_MASK 0x1
#define  I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_SHIFT 0
#define  I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_MASK_SHIFT(reg) (((reg) & I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_MASK) >> I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_SHIFT)
#define  I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_MASK) | (((uint32_t)val) << I2C_IC_ACK_GENERAL_CALL_ACK_GEN_CALL_SHIFT))

//====================================================================
//Register: Enable Status Register (IC_ENABLE_STATUS)
/** \brief Thsi register is used to report the I2C hardware status when IC_ENABLE register is cleared (disabled).*/
//====================================================================

#define  I2C_IC_ENABLE_STATUS_RESERVED1_MASK 0xfffffff8
#define  I2C_IC_ENABLE_STATUS_RESERVED1_SHIFT 3
#define  I2C_IC_ENABLE_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_ENABLE_STATUS_RESERVED1_MASK) >> I2C_IC_ENABLE_STATUS_RESERVED1_SHIFT)
#define  I2C_IC_ENABLE_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ENABLE_STATUS_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_ENABLE_STATUS_RESERVED1_SHIFT))

#define  I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_MASK 0x4
#define  I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_SHIFT 2
#define  I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_MASK_SHIFT(reg) (((reg) & I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_MASK) >> I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_SHIFT)
#define  I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_MASK) | (((uint32_t)val) << I2C_IC_ENABLE_STATUS_SLV_RX_DATA_LOST_SHIFT))

#define  I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_MASK 0x2
#define  I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_SHIFT 1
#define  I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_MASK_SHIFT(reg) (((reg) & I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_MASK) >> I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_SHIFT)
#define  I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_MASK) | (((uint32_t)val) << I2C_IC_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_SHIFT))

#define  I2C_IC_ENABLE_STATUS_IC_EN_MASK 0x1
#define  I2C_IC_ENABLE_STATUS_IC_EN_SHIFT 0
#define  I2C_IC_ENABLE_STATUS_IC_EN_MASK_SHIFT(reg) (((reg) & I2C_IC_ENABLE_STATUS_IC_EN_MASK) >> I2C_IC_ENABLE_STATUS_IC_EN_SHIFT)
#define  I2C_IC_ENABLE_STATUS_IC_EN_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_ENABLE_STATUS_IC_EN_MASK) | (((uint32_t)val) << I2C_IC_ENABLE_STATUS_IC_EN_SHIFT))

//====================================================================
//Register: Component Parameter Register (IC_COMP_PARAM_1)
/** \brief Thsi register contains the encoded information about the block when it was generated.*/
//====================================================================

#define  I2C_IC_COMP_PARAM_1_RESERVED1_MASK 0xff000000
#define  I2C_IC_COMP_PARAM_1_RESERVED1_SHIFT 24
#define  I2C_IC_COMP_PARAM_1_RESERVED1_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_RESERVED1_MASK) >> I2C_IC_COMP_PARAM_1_RESERVED1_SHIFT)
#define  I2C_IC_COMP_PARAM_1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_RESERVED1_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_RESERVED1_SHIFT))

#define  I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_MASK 0xff0000
#define  I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_SHIFT 16
#define  I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_MASK) >> I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_SHIFT)
#define  I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_TX_BUFFER_DEPTH_SHIFT))

#define  I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_MASK 0xff00
#define  I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_SHIFT 8
#define  I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_MASK) >> I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_SHIFT)
#define  I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_RX_BUFFER_DEPTH_SHIFT))

#define  I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_MASK 0x80
#define  I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_SHIFT 7
#define  I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_MASK) >> I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_SHIFT)
#define  I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_ADD_ENCODED_PARAMS_SHIFT))

#define  I2C_IC_COMP_PARAM_1_HAS_DMA_MASK 0x40
#define  I2C_IC_COMP_PARAM_1_HAS_DMA_SHIFT 6
#define  I2C_IC_COMP_PARAM_1_HAS_DMA_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_HAS_DMA_MASK) >> I2C_IC_COMP_PARAM_1_HAS_DMA_SHIFT)
#define  I2C_IC_COMP_PARAM_1_HAS_DMA_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_HAS_DMA_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_HAS_DMA_SHIFT))

#define  I2C_IC_COMP_PARAM_1_INTR_IO_MASK 0x20
#define  I2C_IC_COMP_PARAM_1_INTR_IO_SHIFT 5
#define  I2C_IC_COMP_PARAM_1_INTR_IO_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_INTR_IO_MASK) >> I2C_IC_COMP_PARAM_1_INTR_IO_SHIFT)
#define  I2C_IC_COMP_PARAM_1_INTR_IO_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_INTR_IO_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_INTR_IO_SHIFT))

#define  I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_MASK 0x10
#define  I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_SHIFT 4
#define  I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_MASK) >> I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_SHIFT)
#define  I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_HC_COUNT_VALUES_SHIFT))

#define  I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_MASK 0xc
#define  I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_SHIFT 2
#define  I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_MASK) >> I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_SHIFT)
#define  I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_MAX_SPEED_MODE_SHIFT))

#define  I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_MASK 0x3
#define  I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_SHIFT 0
#define  I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_MASK) >> I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_SHIFT)
#define  I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_MASK) | (((uint32_t)val) << I2C_IC_COMP_PARAM_1_APB_DATA_WIDTH_SHIFT))

//====================================================================
//Register: Component Version Register (IC_COMP_VERSION)
/** \brief Version of the code used to generate this block.*/
//====================================================================

#define  I2C_IC_COMP_VERSION_IC_COMP_VERSION_MASK 0xffffffff
#define  I2C_IC_COMP_VERSION_IC_COMP_VERSION_SHIFT 0
#define  I2C_IC_COMP_VERSION_IC_COMP_VERSION_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_VERSION_IC_COMP_VERSION_MASK) >> I2C_IC_COMP_VERSION_IC_COMP_VERSION_SHIFT)
#define  I2C_IC_COMP_VERSION_IC_COMP_VERSION_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_VERSION_IC_COMP_VERSION_MASK) | (((uint32_t)val) << I2C_IC_COMP_VERSION_IC_COMP_VERSION_SHIFT))

//====================================================================
//Register: Component Type Register (IC_COMP_TYPE)
/** \brief Indicate Component Type.*/
//====================================================================

#define  I2C_IC_COMP_TYPE_IC_COMP_TYPE_MASK 0xffffffff
#define  I2C_IC_COMP_TYPE_IC_COMP_TYPE_SHIFT 0
#define  I2C_IC_COMP_TYPE_IC_COMP_TYPE_MASK_SHIFT(reg) (((reg) & I2C_IC_COMP_TYPE_IC_COMP_TYPE_MASK) >> I2C_IC_COMP_TYPE_IC_COMP_TYPE_SHIFT)
#define  I2C_IC_COMP_TYPE_IC_COMP_TYPE_REPLACE_VAL(reg,val) (((reg) & ~I2C_IC_COMP_TYPE_IC_COMP_TYPE_MASK) | (((uint32_t)val) << I2C_IC_COMP_TYPE_IC_COMP_TYPE_SHIFT))

#endif // I2C
