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
 * \file efuse_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _EFUSE_REGMASKS_H_
#define _EFUSE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: APB EFUSE registers (EFUSE)
/** \brief Registers for EFUSE Programming block which provides hardware assist
    for porogramming and reading the Id_efuse macro.*/
//
//====================================================================

//====================================================================
//Register: Efuse Top-level Control Register (TOP_Control)
/** \brief This register controls overall apb_efuse block operation.*/
//====================================================================

#define  EFUSE_TOP_CONTROL_RESERVED1_MASK 0xffffff00
#define  EFUSE_TOP_CONTROL_RESERVED1_SHIFT 8
#define  EFUSE_TOP_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_CONTROL_RESERVED1_MASK) >> EFUSE_TOP_CONTROL_RESERVED1_SHIFT)
#define  EFUSE_TOP_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_TOP_CONTROL_RESERVED1_SHIFT))

#define  EFUSE_TOP_CONTROL_TEST_MASK 0xf0
#define  EFUSE_TOP_CONTROL_TEST_SHIFT 4
#define  EFUSE_TOP_CONTROL_TEST_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_CONTROL_TEST_MASK) >> EFUSE_TOP_CONTROL_TEST_SHIFT)
#define  EFUSE_TOP_CONTROL_TEST_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_CONTROL_TEST_MASK) | (((uint32_t)val) << EFUSE_TOP_CONTROL_TEST_SHIFT))

#define  EFUSE_TOP_CONTROL_RESERVED2_MASK 0x8
#define  EFUSE_TOP_CONTROL_RESERVED2_SHIFT 3
#define  EFUSE_TOP_CONTROL_RESERVED2_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_CONTROL_RESERVED2_MASK) >> EFUSE_TOP_CONTROL_RESERVED2_SHIFT)
#define  EFUSE_TOP_CONTROL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_CONTROL_RESERVED2_MASK) | (((uint32_t)val) << EFUSE_TOP_CONTROL_RESERVED2_SHIFT))

#define  EFUSE_TOP_CONTROL_BANK_CS_MASK 0x6
#define  EFUSE_TOP_CONTROL_BANK_CS_SHIFT 1
#define  EFUSE_TOP_CONTROL_BANK_CS_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_CONTROL_BANK_CS_MASK) >> EFUSE_TOP_CONTROL_BANK_CS_SHIFT)
#define  EFUSE_TOP_CONTROL_BANK_CS_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_CONTROL_BANK_CS_MASK) | (((uint32_t)val) << EFUSE_TOP_CONTROL_BANK_CS_SHIFT))

#define  EFUSE_TOP_CONTROL_FWNAUTO_MODE_MASK 0x1
#define  EFUSE_TOP_CONTROL_FWNAUTO_MODE_SHIFT 0
#define  EFUSE_TOP_CONTROL_FWNAUTO_MODE_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_CONTROL_FWNAUTO_MODE_MASK) >> EFUSE_TOP_CONTROL_FWNAUTO_MODE_SHIFT)
#define  EFUSE_TOP_CONTROL_FWNAUTO_MODE_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_CONTROL_FWNAUTO_MODE_MASK) | (((uint32_t)val) << EFUSE_TOP_CONTROL_FWNAUTO_MODE_SHIFT))

//====================================================================
//Register: Firmware Control Register (FW_Control)
/** \brief This register directly controls the macro's input ports when the block 
     is put in in FIRMWARE mode.*/
//====================================================================

#define  EFUSE_FW_CONTROL_RESERVED1_MASK 0xffffffe0
#define  EFUSE_FW_CONTROL_RESERVED1_SHIFT 5
#define  EFUSE_FW_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_FW_CONTROL_RESERVED1_MASK) >> EFUSE_FW_CONTROL_RESERVED1_SHIFT)
#define  EFUSE_FW_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_FW_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_FW_CONTROL_RESERVED1_SHIFT))

#define  EFUSE_FW_CONTROL_WPROT_MASK 0x10
#define  EFUSE_FW_CONTROL_WPROT_SHIFT 4
#define  EFUSE_FW_CONTROL_WPROT_MASK_SHIFT(reg) (((reg) & EFUSE_FW_CONTROL_WPROT_MASK) >> EFUSE_FW_CONTROL_WPROT_SHIFT)
#define  EFUSE_FW_CONTROL_WPROT_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_FW_CONTROL_WPROT_MASK) | (((uint32_t)val) << EFUSE_FW_CONTROL_WPROT_SHIFT))

#define  EFUSE_FW_CONTROL_SCLK_MASK 0x8
#define  EFUSE_FW_CONTROL_SCLK_SHIFT 3
#define  EFUSE_FW_CONTROL_SCLK_MASK_SHIFT(reg) (((reg) & EFUSE_FW_CONTROL_SCLK_MASK) >> EFUSE_FW_CONTROL_SCLK_SHIFT)
#define  EFUSE_FW_CONTROL_SCLK_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_FW_CONTROL_SCLK_MASK) | (((uint32_t)val) << EFUSE_FW_CONTROL_SCLK_SHIFT))

#define  EFUSE_FW_CONTROL_PGM_MASK 0x4
#define  EFUSE_FW_CONTROL_PGM_SHIFT 2
#define  EFUSE_FW_CONTROL_PGM_MASK_SHIFT(reg) (((reg) & EFUSE_FW_CONTROL_PGM_MASK) >> EFUSE_FW_CONTROL_PGM_SHIFT)
#define  EFUSE_FW_CONTROL_PGM_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_FW_CONTROL_PGM_MASK) | (((uint32_t)val) << EFUSE_FW_CONTROL_PGM_SHIFT))

#define  EFUSE_FW_CONTROL_CSB_MASK 0x2
#define  EFUSE_FW_CONTROL_CSB_SHIFT 1
#define  EFUSE_FW_CONTROL_CSB_MASK_SHIFT(reg) (((reg) & EFUSE_FW_CONTROL_CSB_MASK) >> EFUSE_FW_CONTROL_CSB_SHIFT)
#define  EFUSE_FW_CONTROL_CSB_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_FW_CONTROL_CSB_MASK) | (((uint32_t)val) << EFUSE_FW_CONTROL_CSB_SHIFT))

#define  EFUSE_FW_CONTROL_RSTB_MASK 0x1
#define  EFUSE_FW_CONTROL_RSTB_SHIFT 0
#define  EFUSE_FW_CONTROL_RSTB_MASK_SHIFT(reg) (((reg) & EFUSE_FW_CONTROL_RSTB_MASK) >> EFUSE_FW_CONTROL_RSTB_SHIFT)
#define  EFUSE_FW_CONTROL_RSTB_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_FW_CONTROL_RSTB_MASK) | (((uint32_t)val) << EFUSE_FW_CONTROL_RSTB_SHIFT))

//====================================================================
//Register: Automatic Mode Control Register (AUTO_Control)
/** \brief This register supports AUTOMATIC mode of operation. <BR>*/
//====================================================================

#define  EFUSE_AUTO_CONTROL_RESERVED1_MASK 0xfffffff8
#define  EFUSE_AUTO_CONTROL_RESERVED1_SHIFT 3
#define  EFUSE_AUTO_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_AUTO_CONTROL_RESERVED1_MASK) >> EFUSE_AUTO_CONTROL_RESERVED1_SHIFT)
#define  EFUSE_AUTO_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_AUTO_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_AUTO_CONTROL_RESERVED1_SHIFT))

#define  EFUSE_AUTO_CONTROL_SFB_MASK 0x4
#define  EFUSE_AUTO_CONTROL_SFB_SHIFT 2
#define  EFUSE_AUTO_CONTROL_SFB_MASK_SHIFT(reg) (((reg) & EFUSE_AUTO_CONTROL_SFB_MASK) >> EFUSE_AUTO_CONTROL_SFB_SHIFT)
#define  EFUSE_AUTO_CONTROL_SFB_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_AUTO_CONTROL_SFB_MASK) | (((uint32_t)val) << EFUSE_AUTO_CONTROL_SFB_SHIFT))

#define  EFUSE_AUTO_CONTROL_RESERVED2_MASK 0x2
#define  EFUSE_AUTO_CONTROL_RESERVED2_SHIFT 1
#define  EFUSE_AUTO_CONTROL_RESERVED2_MASK_SHIFT(reg) (((reg) & EFUSE_AUTO_CONTROL_RESERVED2_MASK) >> EFUSE_AUTO_CONTROL_RESERVED2_SHIFT)
#define  EFUSE_AUTO_CONTROL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_AUTO_CONTROL_RESERVED2_MASK) | (((uint32_t)val) << EFUSE_AUTO_CONTROL_RESERVED2_SHIFT))

#define  EFUSE_AUTO_CONTROL_AUTO_CMD_MASK 0x1
#define  EFUSE_AUTO_CONTROL_AUTO_CMD_SHIFT 0
#define  EFUSE_AUTO_CONTROL_AUTO_CMD_MASK_SHIFT(reg) (((reg) & EFUSE_AUTO_CONTROL_AUTO_CMD_MASK) >> EFUSE_AUTO_CONTROL_AUTO_CMD_SHIFT)
#define  EFUSE_AUTO_CONTROL_AUTO_CMD_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_AUTO_CONTROL_AUTO_CMD_MASK) | (((uint32_t)val) << EFUSE_AUTO_CONTROL_AUTO_CMD_SHIFT))

//====================================================================
//Register: Program Data0 Register (ProgData0)
/** \brief This register is used only in AUTOMATIC mode.*/
//====================================================================

#define  EFUSE_PROGDATA0_DATA_MASK 0xffffffff
#define  EFUSE_PROGDATA0_DATA_SHIFT 0
#define  EFUSE_PROGDATA0_DATA_MASK_SHIFT(reg) (((reg) & EFUSE_PROGDATA0_DATA_MASK) >> EFUSE_PROGDATA0_DATA_SHIFT)
#define  EFUSE_PROGDATA0_DATA_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_PROGDATA0_DATA_MASK) | (((uint32_t)val) << EFUSE_PROGDATA0_DATA_SHIFT))

//====================================================================
//Register: Program Data1 Register (ProgData1)
/** \brief This register is used only in AUTOMATIC mode.*/
//====================================================================

#define  EFUSE_PROGDATA1_DATA_MASK 0xffffffff
#define  EFUSE_PROGDATA1_DATA_SHIFT 0
#define  EFUSE_PROGDATA1_DATA_MASK_SHIFT(reg) (((reg) & EFUSE_PROGDATA1_DATA_MASK) >> EFUSE_PROGDATA1_DATA_SHIFT)
#define  EFUSE_PROGDATA1_DATA_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_PROGDATA1_DATA_MASK) | (((uint32_t)val) << EFUSE_PROGDATA1_DATA_SHIFT))

//====================================================================
//Register: Program Data2 Register (ProgData2)
/** \brief This register is used only in AUTOMATIC mode.*/
//====================================================================

#define  EFUSE_PROGDATA2_DATA_MASK 0xffffffff
#define  EFUSE_PROGDATA2_DATA_SHIFT 0
#define  EFUSE_PROGDATA2_DATA_MASK_SHIFT(reg) (((reg) & EFUSE_PROGDATA2_DATA_MASK) >> EFUSE_PROGDATA2_DATA_SHIFT)
#define  EFUSE_PROGDATA2_DATA_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_PROGDATA2_DATA_MASK) | (((uint32_t)val) << EFUSE_PROGDATA2_DATA_SHIFT))

//====================================================================
//Register: Efuse Top-level Status Register (TOP_Status)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes.*/
//====================================================================

#define  EFUSE_TOP_STATUS_RESERVED1_MASK 0xfffffe00
#define  EFUSE_TOP_STATUS_RESERVED1_SHIFT 9
#define  EFUSE_TOP_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_STATUS_RESERVED1_MASK) >> EFUSE_TOP_STATUS_RESERVED1_SHIFT)
#define  EFUSE_TOP_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_STATUS_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_TOP_STATUS_RESERVED1_SHIFT))

#define  EFUSE_TOP_STATUS_BUSY_MASK 0x100
#define  EFUSE_TOP_STATUS_BUSY_SHIFT 8
#define  EFUSE_TOP_STATUS_BUSY_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_STATUS_BUSY_MASK) >> EFUSE_TOP_STATUS_BUSY_SHIFT)
#define  EFUSE_TOP_STATUS_BUSY_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_STATUS_BUSY_MASK) | (((uint32_t)val) << EFUSE_TOP_STATUS_BUSY_SHIFT))

#define  EFUSE_TOP_STATUS_RESERVED2_MASK 0xc0
#define  EFUSE_TOP_STATUS_RESERVED2_SHIFT 6
#define  EFUSE_TOP_STATUS_RESERVED2_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_STATUS_RESERVED2_MASK) >> EFUSE_TOP_STATUS_RESERVED2_SHIFT)
#define  EFUSE_TOP_STATUS_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_STATUS_RESERVED2_MASK) | (((uint32_t)val) << EFUSE_TOP_STATUS_RESERVED2_SHIFT))

#define  EFUSE_TOP_STATUS_SCLK_AUTO_MASK 0x20
#define  EFUSE_TOP_STATUS_SCLK_AUTO_SHIFT 5
#define  EFUSE_TOP_STATUS_SCLK_AUTO_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_STATUS_SCLK_AUTO_MASK) >> EFUSE_TOP_STATUS_SCLK_AUTO_SHIFT)
#define  EFUSE_TOP_STATUS_SCLK_AUTO_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_STATUS_SCLK_AUTO_MASK) | (((uint32_t)val) << EFUSE_TOP_STATUS_SCLK_AUTO_SHIFT))

#define  EFUSE_TOP_STATUS_SCLK_FW_MASK 0x10
#define  EFUSE_TOP_STATUS_SCLK_FW_SHIFT 4
#define  EFUSE_TOP_STATUS_SCLK_FW_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_STATUS_SCLK_FW_MASK) >> EFUSE_TOP_STATUS_SCLK_FW_SHIFT)
#define  EFUSE_TOP_STATUS_SCLK_FW_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_STATUS_SCLK_FW_MASK) | (((uint32_t)val) << EFUSE_TOP_STATUS_SCLK_FW_SHIFT))

#define  EFUSE_TOP_STATUS_DOUT_MASK 0xf
#define  EFUSE_TOP_STATUS_DOUT_SHIFT 0
#define  EFUSE_TOP_STATUS_DOUT_MASK_SHIFT(reg) (((reg) & EFUSE_TOP_STATUS_DOUT_MASK) >> EFUSE_TOP_STATUS_DOUT_SHIFT)
#define  EFUSE_TOP_STATUS_DOUT_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_TOP_STATUS_DOUT_MASK) | (((uint32_t)val) << EFUSE_TOP_STATUS_DOUT_SHIFT))

//====================================================================
//Register: Efuse bits[31:0] Status Register (Instance 1 of 4) (EfuseStatus31to0_Bank0)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 1 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS31TO0_BANK0_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS31TO0_BANK0_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS31TO0_BANK0_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS31TO0_BANK0_Q_MASK) >> EFUSE_EFUSESTATUS31TO0_BANK0_Q_SHIFT)
#define  EFUSE_EFUSESTATUS31TO0_BANK0_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS31TO0_BANK0_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS31TO0_BANK0_Q_SHIFT))

//====================================================================
//Register: Efuse bits[31:0] Status Register (Instance 2 of 4) (EfuseStatus31to0_Bank1)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 2 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS31TO0_BANK1_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS31TO0_BANK1_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS31TO0_BANK1_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS31TO0_BANK1_Q_MASK) >> EFUSE_EFUSESTATUS31TO0_BANK1_Q_SHIFT)
#define  EFUSE_EFUSESTATUS31TO0_BANK1_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS31TO0_BANK1_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS31TO0_BANK1_Q_SHIFT))

//====================================================================
//Register: Efuse bits[31:0] Status Register (Instance 3 of 4) (EfuseStatus31to0_Bank2)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 3 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS31TO0_BANK2_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS31TO0_BANK2_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS31TO0_BANK2_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS31TO0_BANK2_Q_MASK) >> EFUSE_EFUSESTATUS31TO0_BANK2_Q_SHIFT)
#define  EFUSE_EFUSESTATUS31TO0_BANK2_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS31TO0_BANK2_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS31TO0_BANK2_Q_SHIFT))

//====================================================================
//Register: Efuse bits[31:0] Status Register (Instance 4 of 4) (EfuseStatus31to0_Bank3)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 4 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS31TO0_BANK3_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS31TO0_BANK3_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS31TO0_BANK3_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS31TO0_BANK3_Q_MASK) >> EFUSE_EFUSESTATUS31TO0_BANK3_Q_SHIFT)
#define  EFUSE_EFUSESTATUS31TO0_BANK3_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS31TO0_BANK3_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS31TO0_BANK3_Q_SHIFT))

//====================================================================
//Register: Efuse bits[63:32] Status Register (Instance 1 of 4) (EfuseStatus63to32_Bank0)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 1 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS63TO32_BANK0_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS63TO32_BANK0_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS63TO32_BANK0_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS63TO32_BANK0_Q_MASK) >> EFUSE_EFUSESTATUS63TO32_BANK0_Q_SHIFT)
#define  EFUSE_EFUSESTATUS63TO32_BANK0_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS63TO32_BANK0_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS63TO32_BANK0_Q_SHIFT))

//====================================================================
//Register: Efuse bits[63:32] Status Register (Instance 2 of 4) (EfuseStatus63to32_Bank1)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 2 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS63TO32_BANK1_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS63TO32_BANK1_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS63TO32_BANK1_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS63TO32_BANK1_Q_MASK) >> EFUSE_EFUSESTATUS63TO32_BANK1_Q_SHIFT)
#define  EFUSE_EFUSESTATUS63TO32_BANK1_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS63TO32_BANK1_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS63TO32_BANK1_Q_SHIFT))

//====================================================================
//Register: Efuse bits[63:32] Status Register (Instance 3 of 4) (EfuseStatus63to32_Bank2)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 3 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS63TO32_BANK2_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS63TO32_BANK2_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS63TO32_BANK2_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS63TO32_BANK2_Q_MASK) >> EFUSE_EFUSESTATUS63TO32_BANK2_Q_SHIFT)
#define  EFUSE_EFUSESTATUS63TO32_BANK2_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS63TO32_BANK2_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS63TO32_BANK2_Q_SHIFT))

//====================================================================
//Register: Efuse bits[63:32] Status Register (Instance 4 of 4) (EfuseStatus63to32_Bank3)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 4 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS63TO32_BANK3_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS63TO32_BANK3_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS63TO32_BANK3_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS63TO32_BANK3_Q_MASK) >> EFUSE_EFUSESTATUS63TO32_BANK3_Q_SHIFT)
#define  EFUSE_EFUSESTATUS63TO32_BANK3_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS63TO32_BANK3_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS63TO32_BANK3_Q_SHIFT))

//====================================================================
//Register: Efuse bits[95:64] Status Register (Instance 1 of 4) (EfuseStatus95to64_Bank0)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 1 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS95TO64_BANK0_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS95TO64_BANK0_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS95TO64_BANK0_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS95TO64_BANK0_Q_MASK) >> EFUSE_EFUSESTATUS95TO64_BANK0_Q_SHIFT)
#define  EFUSE_EFUSESTATUS95TO64_BANK0_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS95TO64_BANK0_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS95TO64_BANK0_Q_SHIFT))

//====================================================================
//Register: Efuse bits[95:64] Status Register (Instance 2 of 4) (EfuseStatus95to64_Bank1)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 2 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS95TO64_BANK1_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS95TO64_BANK1_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS95TO64_BANK1_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS95TO64_BANK1_Q_MASK) >> EFUSE_EFUSESTATUS95TO64_BANK1_Q_SHIFT)
#define  EFUSE_EFUSESTATUS95TO64_BANK1_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS95TO64_BANK1_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS95TO64_BANK1_Q_SHIFT))

//====================================================================
//Register: Efuse bits[95:64] Status Register (Instance 3 of 4) (EfuseStatus95to64_Bank2)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 3 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS95TO64_BANK2_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS95TO64_BANK2_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS95TO64_BANK2_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS95TO64_BANK2_Q_MASK) >> EFUSE_EFUSESTATUS95TO64_BANK2_Q_SHIFT)
#define  EFUSE_EFUSESTATUS95TO64_BANK2_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS95TO64_BANK2_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS95TO64_BANK2_Q_SHIFT))

//====================================================================
//Register: Efuse bits[95:64] Status Register (Instance 4 of 4) (EfuseStatus95to64_Bank3)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 4 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS95TO64_BANK3_Q_MASK 0xffffffff
#define  EFUSE_EFUSESTATUS95TO64_BANK3_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS95TO64_BANK3_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS95TO64_BANK3_Q_MASK) >> EFUSE_EFUSESTATUS95TO64_BANK3_Q_SHIFT)
#define  EFUSE_EFUSESTATUS95TO64_BANK3_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS95TO64_BANK3_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS95TO64_BANK3_Q_SHIFT))

//====================================================================
//Register: Efuse bit[96] Status Register (Instance 1 of 4) (EfuseStatus96_Bank0)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 1 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS96_BANK0_RESERVED1_MASK 0xfffffffe
#define  EFUSE_EFUSESTATUS96_BANK0_RESERVED1_SHIFT 1
#define  EFUSE_EFUSESTATUS96_BANK0_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK0_RESERVED1_MASK) >> EFUSE_EFUSESTATUS96_BANK0_RESERVED1_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK0_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK0_RESERVED1_SHIFT))

#define  EFUSE_EFUSESTATUS96_BANK0_Q_MASK 0x1
#define  EFUSE_EFUSESTATUS96_BANK0_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS96_BANK0_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK0_Q_MASK) >> EFUSE_EFUSESTATUS96_BANK0_Q_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK0_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK0_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK0_Q_SHIFT))

//====================================================================
//Register: Efuse bit[96] Status Register (Instance 2 of 4) (EfuseStatus96_Bank1)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 2 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS96_BANK1_RESERVED1_MASK 0xfffffffe
#define  EFUSE_EFUSESTATUS96_BANK1_RESERVED1_SHIFT 1
#define  EFUSE_EFUSESTATUS96_BANK1_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK1_RESERVED1_MASK) >> EFUSE_EFUSESTATUS96_BANK1_RESERVED1_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK1_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK1_RESERVED1_SHIFT))

#define  EFUSE_EFUSESTATUS96_BANK1_Q_MASK 0x1
#define  EFUSE_EFUSESTATUS96_BANK1_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS96_BANK1_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK1_Q_MASK) >> EFUSE_EFUSESTATUS96_BANK1_Q_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK1_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK1_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK1_Q_SHIFT))

//====================================================================
//Register: Efuse bit[96] Status Register (Instance 3 of 4) (EfuseStatus96_Bank2)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 3 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS96_BANK2_RESERVED1_MASK 0xfffffffe
#define  EFUSE_EFUSESTATUS96_BANK2_RESERVED1_SHIFT 1
#define  EFUSE_EFUSESTATUS96_BANK2_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK2_RESERVED1_MASK) >> EFUSE_EFUSESTATUS96_BANK2_RESERVED1_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK2_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK2_RESERVED1_SHIFT))

#define  EFUSE_EFUSESTATUS96_BANK2_Q_MASK 0x1
#define  EFUSE_EFUSESTATUS96_BANK2_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS96_BANK2_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK2_Q_MASK) >> EFUSE_EFUSESTATUS96_BANK2_Q_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK2_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK2_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK2_Q_SHIFT))

//====================================================================
//Register: Efuse bit[96] Status Register (Instance 4 of 4) (EfuseStatus96_Bank3)
/** \brief This register is useful in both AUTOMATIC and FIRMWARE modes. (Instance 4 of 4)*/
//====================================================================

#define  EFUSE_EFUSESTATUS96_BANK3_RESERVED1_MASK 0xfffffffe
#define  EFUSE_EFUSESTATUS96_BANK3_RESERVED1_SHIFT 1
#define  EFUSE_EFUSESTATUS96_BANK3_RESERVED1_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK3_RESERVED1_MASK) >> EFUSE_EFUSESTATUS96_BANK3_RESERVED1_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK3_RESERVED1_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK3_RESERVED1_SHIFT))

#define  EFUSE_EFUSESTATUS96_BANK3_Q_MASK 0x1
#define  EFUSE_EFUSESTATUS96_BANK3_Q_SHIFT 0
#define  EFUSE_EFUSESTATUS96_BANK3_Q_MASK_SHIFT(reg) (((reg) & EFUSE_EFUSESTATUS96_BANK3_Q_MASK) >> EFUSE_EFUSESTATUS96_BANK3_Q_SHIFT)
#define  EFUSE_EFUSESTATUS96_BANK3_Q_REPLACE_VAL(reg,val) (((reg) & ~EFUSE_EFUSESTATUS96_BANK3_Q_MASK) | (((uint32_t)val) << EFUSE_EFUSESTATUS96_BANK3_Q_SHIFT))

#endif // EFUSE
