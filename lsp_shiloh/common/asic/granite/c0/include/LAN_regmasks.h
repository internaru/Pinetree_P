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
 * \file LAN_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _LAN_REGMASKS_H_
#define _LAN_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: LAN TOP Instantiation of Register Sets (LAN)
//
//====================================================================

//====================================================================
//
//Register File: LAN external-to-DWIP Registers (EXT)
/** \brief A handful of LAN external-to-DWIP registers exist namely: FTDLL, FTDLL_OBSV, DEL_TEST, SB. These registers are documented here.
    Also, if integrated, the Fast Ethernet PHY config and status registers, as well as a dual interface option with RGMII also.
    Note: the FTDLL, PHY and DUAL_IF registers are built by defualt, at build a -D 0=0, -D 0=0, and 0=0 will delete them.*/
//
//====================================================================

//====================================================================
//Register: Sideband Register (SB)
//====================================================================

#define  EXT_SB_RESERVED1_MASK 0xfffffff0
#define  EXT_SB_RESERVED1_SHIFT 4
#define  EXT_SB_RESERVED1_MASK_SHIFT(reg) (((reg) & EXT_SB_RESERVED1_MASK) >> EXT_SB_RESERVED1_SHIFT)
#define  EXT_SB_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EXT_SB_RESERVED1_MASK) | (((uint32_t)val) << EXT_SB_RESERVED1_SHIFT))

#define  EXT_SB_LAN_MAC_TX_CLK_STOP_EN_MASK 0x8
#define  EXT_SB_LAN_MAC_TX_CLK_STOP_EN_SHIFT 3
#define  EXT_SB_LAN_MAC_TX_CLK_STOP_EN_MASK_SHIFT(reg) (((reg) & EXT_SB_LAN_MAC_TX_CLK_STOP_EN_MASK) >> EXT_SB_LAN_MAC_TX_CLK_STOP_EN_SHIFT)
#define  EXT_SB_LAN_MAC_TX_CLK_STOP_EN_REPLACE_VAL(reg,val) (((reg) & ~EXT_SB_LAN_MAC_TX_CLK_STOP_EN_MASK) | (((uint32_t)val) << EXT_SB_LAN_MAC_TX_CLK_STOP_EN_SHIFT))

#define  EXT_SB_RESERVED2_MASK 0x4
#define  EXT_SB_RESERVED2_SHIFT 2
#define  EXT_SB_RESERVED2_MASK_SHIFT(reg) (((reg) & EXT_SB_RESERVED2_MASK) >> EXT_SB_RESERVED2_SHIFT)
#define  EXT_SB_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~EXT_SB_RESERVED2_MASK) | (((uint32_t)val) << EXT_SB_RESERVED2_SHIFT))

#define  EXT_SB_TXC_DLY_BYPASS_MASK 0x2
#define  EXT_SB_TXC_DLY_BYPASS_SHIFT 1
#define  EXT_SB_TXC_DLY_BYPASS_MASK_SHIFT(reg) (((reg) & EXT_SB_TXC_DLY_BYPASS_MASK) >> EXT_SB_TXC_DLY_BYPASS_SHIFT)
#define  EXT_SB_TXC_DLY_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~EXT_SB_TXC_DLY_BYPASS_MASK) | (((uint32_t)val) << EXT_SB_TXC_DLY_BYPASS_SHIFT))

#define  EXT_SB_RX_FLOW_CNTRL_MASK 0x1
#define  EXT_SB_RX_FLOW_CNTRL_SHIFT 0
#define  EXT_SB_RX_FLOW_CNTRL_MASK_SHIFT(reg) (((reg) & EXT_SB_RX_FLOW_CNTRL_MASK) >> EXT_SB_RX_FLOW_CNTRL_SHIFT)
#define  EXT_SB_RX_FLOW_CNTRL_REPLACE_VAL(reg,val) (((reg) & ~EXT_SB_RX_FLOW_CNTRL_MASK) | (((uint32_t)val) << EXT_SB_RX_FLOW_CNTRL_SHIFT))

//====================================================================
//
//Register File: LAN DWIP MAC Registers (MAC)
/** \brief TBD - more to add but this RegSet comment fields don't seem to be showing up.
    This section defines the bits for each MAC register.*/
//
//====================================================================

//====================================================================
//Register: MAC Configuration Register (r0)
/** \brief This is the operation mode register for the MAC.
The MAC Configuration register establishes receive and transmit operating modes.*/
//====================================================================

#define  MAC_R0_RESERVED1_MASK 0xff000000
#define  MAC_R0_RESERVED1_SHIFT 24
#define  MAC_R0_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R0_RESERVED1_MASK) >> MAC_R0_RESERVED1_SHIFT)
#define  MAC_R0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_RESERVED1_MASK) | (((uint32_t)val) << MAC_R0_RESERVED1_SHIFT))

#define  MAC_R0_WD_MASK 0x800000
#define  MAC_R0_WD_SHIFT 23
#define  MAC_R0_WD_MASK_SHIFT(reg) (((reg) & MAC_R0_WD_MASK) >> MAC_R0_WD_SHIFT)
#define  MAC_R0_WD_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_WD_MASK) | (((uint32_t)val) << MAC_R0_WD_SHIFT))

#define  MAC_R0_JD_MASK 0x400000
#define  MAC_R0_JD_SHIFT 22
#define  MAC_R0_JD_MASK_SHIFT(reg) (((reg) & MAC_R0_JD_MASK) >> MAC_R0_JD_SHIFT)
#define  MAC_R0_JD_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_JD_MASK) | (((uint32_t)val) << MAC_R0_JD_SHIFT))

#define  MAC_R0_BE_MASK 0x200000
#define  MAC_R0_BE_SHIFT 21
#define  MAC_R0_BE_MASK_SHIFT(reg) (((reg) & MAC_R0_BE_MASK) >> MAC_R0_BE_SHIFT)
#define  MAC_R0_BE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_BE_MASK) | (((uint32_t)val) << MAC_R0_BE_SHIFT))

#define  MAC_R0_JE_MASK 0x100000
#define  MAC_R0_JE_SHIFT 20
#define  MAC_R0_JE_MASK_SHIFT(reg) (((reg) & MAC_R0_JE_MASK) >> MAC_R0_JE_SHIFT)
#define  MAC_R0_JE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_JE_MASK) | (((uint32_t)val) << MAC_R0_JE_SHIFT))

#define  MAC_R0_IFG_MASK 0xe0000
#define  MAC_R0_IFG_SHIFT 17
#define  MAC_R0_IFG_MASK_SHIFT(reg) (((reg) & MAC_R0_IFG_MASK) >> MAC_R0_IFG_SHIFT)
#define  MAC_R0_IFG_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_IFG_MASK) | (((uint32_t)val) << MAC_R0_IFG_SHIFT))

#define  MAC_R0_DCRS_MASK 0x10000
#define  MAC_R0_DCRS_SHIFT 16
#define  MAC_R0_DCRS_MASK_SHIFT(reg) (((reg) & MAC_R0_DCRS_MASK) >> MAC_R0_DCRS_SHIFT)
#define  MAC_R0_DCRS_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_DCRS_MASK) | (((uint32_t)val) << MAC_R0_DCRS_SHIFT))

#define  MAC_R0_PS_MASK 0x8000
#define  MAC_R0_PS_SHIFT 15
#define  MAC_R0_PS_MASK_SHIFT(reg) (((reg) & MAC_R0_PS_MASK) >> MAC_R0_PS_SHIFT)
#define  MAC_R0_PS_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_PS_MASK) | (((uint32_t)val) << MAC_R0_PS_SHIFT))

#define  MAC_R0_FES_MASK 0x4000
#define  MAC_R0_FES_SHIFT 14
#define  MAC_R0_FES_MASK_SHIFT(reg) (((reg) & MAC_R0_FES_MASK) >> MAC_R0_FES_SHIFT)
#define  MAC_R0_FES_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_FES_MASK) | (((uint32_t)val) << MAC_R0_FES_SHIFT))

#define  MAC_R0_DO_MASK 0x2000
#define  MAC_R0_DO_SHIFT 13
#define  MAC_R0_DO_MASK_SHIFT(reg) (((reg) & MAC_R0_DO_MASK) >> MAC_R0_DO_SHIFT)
#define  MAC_R0_DO_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_DO_MASK) | (((uint32_t)val) << MAC_R0_DO_SHIFT))

#define  MAC_R0_LM_MASK 0x1000
#define  MAC_R0_LM_SHIFT 12
#define  MAC_R0_LM_MASK_SHIFT(reg) (((reg) & MAC_R0_LM_MASK) >> MAC_R0_LM_SHIFT)
#define  MAC_R0_LM_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_LM_MASK) | (((uint32_t)val) << MAC_R0_LM_SHIFT))

#define  MAC_R0_DM_MASK 0x800
#define  MAC_R0_DM_SHIFT 11
#define  MAC_R0_DM_MASK_SHIFT(reg) (((reg) & MAC_R0_DM_MASK) >> MAC_R0_DM_SHIFT)
#define  MAC_R0_DM_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_DM_MASK) | (((uint32_t)val) << MAC_R0_DM_SHIFT))

#define  MAC_R0_IPC_MASK 0x400
#define  MAC_R0_IPC_SHIFT 10
#define  MAC_R0_IPC_MASK_SHIFT(reg) (((reg) & MAC_R0_IPC_MASK) >> MAC_R0_IPC_SHIFT)
#define  MAC_R0_IPC_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_IPC_MASK) | (((uint32_t)val) << MAC_R0_IPC_SHIFT))

#define  MAC_R0_DR_MASK 0x200
#define  MAC_R0_DR_SHIFT 9
#define  MAC_R0_DR_MASK_SHIFT(reg) (((reg) & MAC_R0_DR_MASK) >> MAC_R0_DR_SHIFT)
#define  MAC_R0_DR_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_DR_MASK) | (((uint32_t)val) << MAC_R0_DR_SHIFT))

#define  MAC_R0_RESERVED2_MASK 0x100
#define  MAC_R0_RESERVED2_SHIFT 8
#define  MAC_R0_RESERVED2_MASK_SHIFT(reg) (((reg) & MAC_R0_RESERVED2_MASK) >> MAC_R0_RESERVED2_SHIFT)
#define  MAC_R0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_RESERVED2_MASK) | (((uint32_t)val) << MAC_R0_RESERVED2_SHIFT))

#define  MAC_R0_ACS_MASK 0x80
#define  MAC_R0_ACS_SHIFT 7
#define  MAC_R0_ACS_MASK_SHIFT(reg) (((reg) & MAC_R0_ACS_MASK) >> MAC_R0_ACS_SHIFT)
#define  MAC_R0_ACS_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_ACS_MASK) | (((uint32_t)val) << MAC_R0_ACS_SHIFT))

#define  MAC_R0_BL_MASK 0x60
#define  MAC_R0_BL_SHIFT 5
#define  MAC_R0_BL_MASK_SHIFT(reg) (((reg) & MAC_R0_BL_MASK) >> MAC_R0_BL_SHIFT)
#define  MAC_R0_BL_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_BL_MASK) | (((uint32_t)val) << MAC_R0_BL_SHIFT))

#define  MAC_R0_DC_MASK 0x10
#define  MAC_R0_DC_SHIFT 4
#define  MAC_R0_DC_MASK_SHIFT(reg) (((reg) & MAC_R0_DC_MASK) >> MAC_R0_DC_SHIFT)
#define  MAC_R0_DC_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_DC_MASK) | (((uint32_t)val) << MAC_R0_DC_SHIFT))

#define  MAC_R0_TE_MASK 0x8
#define  MAC_R0_TE_SHIFT 3
#define  MAC_R0_TE_MASK_SHIFT(reg) (((reg) & MAC_R0_TE_MASK) >> MAC_R0_TE_SHIFT)
#define  MAC_R0_TE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_TE_MASK) | (((uint32_t)val) << MAC_R0_TE_SHIFT))

#define  MAC_R0_RE_MASK 0x4
#define  MAC_R0_RE_SHIFT 2
#define  MAC_R0_RE_MASK_SHIFT(reg) (((reg) & MAC_R0_RE_MASK) >> MAC_R0_RE_SHIFT)
#define  MAC_R0_RE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_RE_MASK) | (((uint32_t)val) << MAC_R0_RE_SHIFT))

#define  MAC_R0_RESERVED3_MASK 0x3
#define  MAC_R0_RESERVED3_SHIFT 0
#define  MAC_R0_RESERVED3_MASK_SHIFT(reg) (((reg) & MAC_R0_RESERVED3_MASK) >> MAC_R0_RESERVED3_SHIFT)
#define  MAC_R0_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~MAC_R0_RESERVED3_MASK) | (((uint32_t)val) << MAC_R0_RESERVED3_SHIFT))

//====================================================================
//Register: MAC Frame Filter (r1)
/** \brief Contains the frame filtering controls.
The MAC Frame Filter register contains the filter controls for receiving frames. Some of the controls from this register go to the address check block of the MAC, which performs the first level of address filtering. The second level of filtering is performed on the incoming frame, based on other controls such as Pass Bad Frames and Pass Control Frames.*/
//====================================================================

#define  MAC_R1_RA_MASK 0x80000000
#define  MAC_R1_RA_SHIFT 31
#define  MAC_R1_RA_MASK_SHIFT(reg) (((reg) & MAC_R1_RA_MASK) >> MAC_R1_RA_SHIFT)
#define  MAC_R1_RA_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_RA_MASK) | (((uint32_t)val) << MAC_R1_RA_SHIFT))

#define  MAC_R1_RESERVED1_MASK 0x7ffff800
#define  MAC_R1_RESERVED1_SHIFT 11
#define  MAC_R1_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R1_RESERVED1_MASK) >> MAC_R1_RESERVED1_SHIFT)
#define  MAC_R1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_RESERVED1_MASK) | (((uint32_t)val) << MAC_R1_RESERVED1_SHIFT))

#define  MAC_R1_HPF_MASK 0x400
#define  MAC_R1_HPF_SHIFT 10
#define  MAC_R1_HPF_MASK_SHIFT(reg) (((reg) & MAC_R1_HPF_MASK) >> MAC_R1_HPF_SHIFT)
#define  MAC_R1_HPF_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_HPF_MASK) | (((uint32_t)val) << MAC_R1_HPF_SHIFT))

#define  MAC_R1_SAF_MASK 0x200
#define  MAC_R1_SAF_SHIFT 9
#define  MAC_R1_SAF_MASK_SHIFT(reg) (((reg) & MAC_R1_SAF_MASK) >> MAC_R1_SAF_SHIFT)
#define  MAC_R1_SAF_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_SAF_MASK) | (((uint32_t)val) << MAC_R1_SAF_SHIFT))

#define  MAC_R1_SAIF_MASK 0x100
#define  MAC_R1_SAIF_SHIFT 8
#define  MAC_R1_SAIF_MASK_SHIFT(reg) (((reg) & MAC_R1_SAIF_MASK) >> MAC_R1_SAIF_SHIFT)
#define  MAC_R1_SAIF_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_SAIF_MASK) | (((uint32_t)val) << MAC_R1_SAIF_SHIFT))

#define  MAC_R1_PCF_MASK 0xc0
#define  MAC_R1_PCF_SHIFT 6
#define  MAC_R1_PCF_MASK_SHIFT(reg) (((reg) & MAC_R1_PCF_MASK) >> MAC_R1_PCF_SHIFT)
#define  MAC_R1_PCF_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_PCF_MASK) | (((uint32_t)val) << MAC_R1_PCF_SHIFT))

#define  MAC_R1_DBF_MASK 0x20
#define  MAC_R1_DBF_SHIFT 5
#define  MAC_R1_DBF_MASK_SHIFT(reg) (((reg) & MAC_R1_DBF_MASK) >> MAC_R1_DBF_SHIFT)
#define  MAC_R1_DBF_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_DBF_MASK) | (((uint32_t)val) << MAC_R1_DBF_SHIFT))

#define  MAC_R1_PM_MASK 0x10
#define  MAC_R1_PM_SHIFT 4
#define  MAC_R1_PM_MASK_SHIFT(reg) (((reg) & MAC_R1_PM_MASK) >> MAC_R1_PM_SHIFT)
#define  MAC_R1_PM_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_PM_MASK) | (((uint32_t)val) << MAC_R1_PM_SHIFT))

#define  MAC_R1_DAIF_MASK 0x8
#define  MAC_R1_DAIF_SHIFT 3
#define  MAC_R1_DAIF_MASK_SHIFT(reg) (((reg) & MAC_R1_DAIF_MASK) >> MAC_R1_DAIF_SHIFT)
#define  MAC_R1_DAIF_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_DAIF_MASK) | (((uint32_t)val) << MAC_R1_DAIF_SHIFT))

#define  MAC_R1_HMC_MASK 0x4
#define  MAC_R1_HMC_SHIFT 2
#define  MAC_R1_HMC_MASK_SHIFT(reg) (((reg) & MAC_R1_HMC_MASK) >> MAC_R1_HMC_SHIFT)
#define  MAC_R1_HMC_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_HMC_MASK) | (((uint32_t)val) << MAC_R1_HMC_SHIFT))

#define  MAC_R1_HUC_MASK 0x2
#define  MAC_R1_HUC_SHIFT 1
#define  MAC_R1_HUC_MASK_SHIFT(reg) (((reg) & MAC_R1_HUC_MASK) >> MAC_R1_HUC_SHIFT)
#define  MAC_R1_HUC_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_HUC_MASK) | (((uint32_t)val) << MAC_R1_HUC_SHIFT))

#define  MAC_R1_PR_MASK 0x1
#define  MAC_R1_PR_SHIFT 0
#define  MAC_R1_PR_MASK_SHIFT(reg) (((reg) & MAC_R1_PR_MASK) >> MAC_R1_PR_SHIFT)
#define  MAC_R1_PR_REPLACE_VAL(reg,val) (((reg) & ~MAC_R1_PR_MASK) | (((uint32_t)val) << MAC_R1_PR_SHIFT))

//====================================================================
//Register: Hast Table High Register (r2)
/** \brief Contains the higher 32 bits of the Multicast Hash table.
The 64-bit Hash table is used for group address filtering. For hash filtering, the contents of the destination address in the incoming frame is passed through the CRC logic, and the upper 6 bits of the CRC register are used to index the contents of the Hash table. The most significant bit determines the register to be used (Hash Table High/Hash Table Low), and the other 5 bits determine which bit within the register. A hash value of 5b'00000 selects bit 0 of the selected register, and a value of 5b'11111 selects bit 31 of the selected register.
For example, if the DA of the incoming frame is received as 0x1F52419CB6AF (0x1F is the first byte received on GMII interface), then the internally calculated 6-bit Hash value is 0x2C and the HTH register bit[12] is checked for filtering. If the DA of the incoming frame is received as 0xA00A98000045, then the calculated 6-bit Hash value is 0x07 and the HTL register bit[7] is checked for filtering.
If the corresponding bit value of the register is 1'b1, the frame is accepted. Otherwise, it is rejected. If the PM (Pass All Multicast) bit is set in Register1, then all multicast frames are accepted regardless of the multicast hash values.
If the Hash Table register is configured to be double-synchronized to the (G)MII clock domain, the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big-Endian mode) of the Hash Table High/Low registers are written to. Please note that consecutive writes to these register should be performed only after at least 4 clock cycles in the destination clock domain when double-synchronization is enabled.
The Hash Table Hi register contains the higher 32 bits of the Hash table.*/
//====================================================================

#define  MAC_R2_HTH_MASK 0xffffffff
#define  MAC_R2_HTH_SHIFT 0
#define  MAC_R2_HTH_MASK_SHIFT(reg) (((reg) & MAC_R2_HTH_MASK) >> MAC_R2_HTH_SHIFT)
#define  MAC_R2_HTH_REPLACE_VAL(reg,val) (((reg) & ~MAC_R2_HTH_MASK) | (((uint32_t)val) << MAC_R2_HTH_SHIFT))

//====================================================================
//Register: Hash Table Low Register (r3)
/** \brief Contains the lower 32 bits of the Multicast Hash table.
The Hash Table Low register contains the lower 32 bits of the Hash table. Both Register2 and Register3
and corresponding HMC and HUC bits in Filter Register are reserved if the Hash Filter Function is
disabled during coreKit configuration.*/
//====================================================================

#define  MAC_R3_HTL_MASK 0xffffffff
#define  MAC_R3_HTL_SHIFT 0
#define  MAC_R3_HTL_MASK_SHIFT(reg) (((reg) & MAC_R3_HTL_MASK) >> MAC_R3_HTL_SHIFT)
#define  MAC_R3_HTL_REPLACE_VAL(reg,val) (((reg) & ~MAC_R3_HTL_MASK) | (((uint32_t)val) << MAC_R3_HTL_SHIFT))

//====================================================================
//Register: GMII Address Register (r4)
/** \brief Controls the management cycles to an external PHY.
The GMII Address register controls the management cycles to the external PHY through the management interface.*/
//====================================================================

#define  MAC_R4_RESERVED1_MASK 0xffff0000
#define  MAC_R4_RESERVED1_SHIFT 16
#define  MAC_R4_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R4_RESERVED1_MASK) >> MAC_R4_RESERVED1_SHIFT)
#define  MAC_R4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R4_RESERVED1_MASK) | (((uint32_t)val) << MAC_R4_RESERVED1_SHIFT))

#define  MAC_R4_PA_MASK 0xf800
#define  MAC_R4_PA_SHIFT 11
#define  MAC_R4_PA_MASK_SHIFT(reg) (((reg) & MAC_R4_PA_MASK) >> MAC_R4_PA_SHIFT)
#define  MAC_R4_PA_REPLACE_VAL(reg,val) (((reg) & ~MAC_R4_PA_MASK) | (((uint32_t)val) << MAC_R4_PA_SHIFT))

#define  MAC_R4_GR_MASK 0x7c0
#define  MAC_R4_GR_SHIFT 6
#define  MAC_R4_GR_MASK_SHIFT(reg) (((reg) & MAC_R4_GR_MASK) >> MAC_R4_GR_SHIFT)
#define  MAC_R4_GR_REPLACE_VAL(reg,val) (((reg) & ~MAC_R4_GR_MASK) | (((uint32_t)val) << MAC_R4_GR_SHIFT))

#define  MAC_R4_RESERVED2_MASK 0x20
#define  MAC_R4_RESERVED2_SHIFT 5
#define  MAC_R4_RESERVED2_MASK_SHIFT(reg) (((reg) & MAC_R4_RESERVED2_MASK) >> MAC_R4_RESERVED2_SHIFT)
#define  MAC_R4_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAC_R4_RESERVED2_MASK) | (((uint32_t)val) << MAC_R4_RESERVED2_SHIFT))

#define  MAC_R4_CR_MASK 0x1c
#define  MAC_R4_CR_SHIFT 2
#define  MAC_R4_CR_MASK_SHIFT(reg) (((reg) & MAC_R4_CR_MASK) >> MAC_R4_CR_SHIFT)
#define  MAC_R4_CR_REPLACE_VAL(reg,val) (((reg) & ~MAC_R4_CR_MASK) | (((uint32_t)val) << MAC_R4_CR_SHIFT))

#define  MAC_R4_GW_MASK 0x2
#define  MAC_R4_GW_SHIFT 1
#define  MAC_R4_GW_MASK_SHIFT(reg) (((reg) & MAC_R4_GW_MASK) >> MAC_R4_GW_SHIFT)
#define  MAC_R4_GW_REPLACE_VAL(reg,val) (((reg) & ~MAC_R4_GW_MASK) | (((uint32_t)val) << MAC_R4_GW_SHIFT))

#define  MAC_R4_GB_MASK 0x1
#define  MAC_R4_GB_SHIFT 0
#define  MAC_R4_GB_MASK_SHIFT(reg) (((reg) & MAC_R4_GB_MASK) >> MAC_R4_GB_SHIFT)
#define  MAC_R4_GB_REPLACE_VAL(reg,val) (((reg) & ~MAC_R4_GB_MASK) | (((uint32_t)val) << MAC_R4_GB_SHIFT))

//====================================================================
//Register: GMII Data Register (r5)
/** \brief Contains the data to be written to or read from the PHY register.
The GMII Data register stores Write data to be written to the PHY register located at the address specified in Register4. Register5 also stores Read data from the PHY register located at the address specified by Register4.*/
//====================================================================

#define  MAC_R5_RESERVED1_MASK 0xffff0000
#define  MAC_R5_RESERVED1_SHIFT 16
#define  MAC_R5_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R5_RESERVED1_MASK) >> MAC_R5_RESERVED1_SHIFT)
#define  MAC_R5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R5_RESERVED1_MASK) | (((uint32_t)val) << MAC_R5_RESERVED1_SHIFT))

#define  MAC_R5_GD_MASK 0xffff
#define  MAC_R5_GD_SHIFT 0
#define  MAC_R5_GD_MASK_SHIFT(reg) (((reg) & MAC_R5_GD_MASK) >> MAC_R5_GD_SHIFT)
#define  MAC_R5_GD_REPLACE_VAL(reg,val) (((reg) & ~MAC_R5_GD_MASK) | (((uint32_t)val) << MAC_R5_GD_SHIFT))

//====================================================================
//Register: Flow Control Register (r6)
/** \brief Controls the generation of control frames.
The Flow Control register controls the generation and reception of the Control (Pause Command) frames by the MAC's Flow control module. A Write to a register with the Busy bit set to '1' triggers the Flow Control block to generate a Pause Control frame. The fields of the control frame are selected as specified in the 802.3x specification, and the Pause Time value from this register is used in the Pause Time field of the control frame. The Busy bit remains set until the control frame is transferred onto the cable. The Host must make sure that the Busy bit is cleared before writing to the register.*/
//====================================================================

#define  MAC_R6_PT_MASK 0xffff0000
#define  MAC_R6_PT_SHIFT 16
#define  MAC_R6_PT_MASK_SHIFT(reg) (((reg) & MAC_R6_PT_MASK) >> MAC_R6_PT_SHIFT)
#define  MAC_R6_PT_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_PT_MASK) | (((uint32_t)val) << MAC_R6_PT_SHIFT))

#define  MAC_R6_RESERVED1_MASK 0xff00
#define  MAC_R6_RESERVED1_SHIFT 8
#define  MAC_R6_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R6_RESERVED1_MASK) >> MAC_R6_RESERVED1_SHIFT)
#define  MAC_R6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_RESERVED1_MASK) | (((uint32_t)val) << MAC_R6_RESERVED1_SHIFT))

#define  MAC_R6_DZPQ_MASK 0x80
#define  MAC_R6_DZPQ_SHIFT 7
#define  MAC_R6_DZPQ_MASK_SHIFT(reg) (((reg) & MAC_R6_DZPQ_MASK) >> MAC_R6_DZPQ_SHIFT)
#define  MAC_R6_DZPQ_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_DZPQ_MASK) | (((uint32_t)val) << MAC_R6_DZPQ_SHIFT))

#define  MAC_R6_RESERVED2_MASK 0x40
#define  MAC_R6_RESERVED2_SHIFT 6
#define  MAC_R6_RESERVED2_MASK_SHIFT(reg) (((reg) & MAC_R6_RESERVED2_MASK) >> MAC_R6_RESERVED2_SHIFT)
#define  MAC_R6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_RESERVED2_MASK) | (((uint32_t)val) << MAC_R6_RESERVED2_SHIFT))

#define  MAC_R6_PLT_MASK 0x30
#define  MAC_R6_PLT_SHIFT 4
#define  MAC_R6_PLT_MASK_SHIFT(reg) (((reg) & MAC_R6_PLT_MASK) >> MAC_R6_PLT_SHIFT)
#define  MAC_R6_PLT_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_PLT_MASK) | (((uint32_t)val) << MAC_R6_PLT_SHIFT))

#define  MAC_R6_UP_MASK 0x8
#define  MAC_R6_UP_SHIFT 3
#define  MAC_R6_UP_MASK_SHIFT(reg) (((reg) & MAC_R6_UP_MASK) >> MAC_R6_UP_SHIFT)
#define  MAC_R6_UP_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_UP_MASK) | (((uint32_t)val) << MAC_R6_UP_SHIFT))

#define  MAC_R6_RFE_MASK 0x4
#define  MAC_R6_RFE_SHIFT 2
#define  MAC_R6_RFE_MASK_SHIFT(reg) (((reg) & MAC_R6_RFE_MASK) >> MAC_R6_RFE_SHIFT)
#define  MAC_R6_RFE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_RFE_MASK) | (((uint32_t)val) << MAC_R6_RFE_SHIFT))

#define  MAC_R6_TFE_MASK 0x2
#define  MAC_R6_TFE_SHIFT 1
#define  MAC_R6_TFE_MASK_SHIFT(reg) (((reg) & MAC_R6_TFE_MASK) >> MAC_R6_TFE_SHIFT)
#define  MAC_R6_TFE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_TFE_MASK) | (((uint32_t)val) << MAC_R6_TFE_SHIFT))

#define  MAC_R6_FCB_BPA_MASK 0x1
#define  MAC_R6_FCB_BPA_SHIFT 0
#define  MAC_R6_FCB_BPA_MASK_SHIFT(reg) (((reg) & MAC_R6_FCB_BPA_MASK) >> MAC_R6_FCB_BPA_SHIFT)
#define  MAC_R6_FCB_BPA_REPLACE_VAL(reg,val) (((reg) & ~MAC_R6_FCB_BPA_MASK) | (((uint32_t)val) << MAC_R6_FCB_BPA_SHIFT))

//====================================================================
//Register: VLAN Tag Register (r7)
/** \brief Identifies IEEE 802.1Q VLAN type frames.
The VLAN Tag register contains the IEEE 802.1Q VLAN Tag to identify the VLAN frames. The MAC compares the 13th and 14th bytes of the receiving frame (Length/Type) with 16h8100, and the following 2 bytes are compared with the VLAN tag; if a match occurs, it sets the received VLAN bit in the receive frame status. The legal length of the frame is increased from 1518 bytes to 1522 bytes.
If the VLAN Tag register is configured to be double-synchronized to the (G)MII clock domain, then consecutive writes to these register should be performed only after at least 4 clock cycles in the destination clock domain.*/
//====================================================================

#define  MAC_R7_RESERVED1_MASK 0xffff0000
#define  MAC_R7_RESERVED1_SHIFT 16
#define  MAC_R7_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R7_RESERVED1_MASK) >> MAC_R7_RESERVED1_SHIFT)
#define  MAC_R7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R7_RESERVED1_MASK) | (((uint32_t)val) << MAC_R7_RESERVED1_SHIFT))

#define  MAC_R7_VL_MASK 0xffff
#define  MAC_R7_VL_SHIFT 0
#define  MAC_R7_VL_MASK_SHIFT(reg) (((reg) & MAC_R7_VL_MASK) >> MAC_R7_VL_SHIFT)
#define  MAC_R7_VL_REPLACE_VAL(reg,val) (((reg) & ~MAC_R7_VL_MASK) | (((uint32_t)val) << MAC_R7_VL_SHIFT))

//====================================================================
//Register: Version Register (r8)
/** \brief Identifies the version of the Core.
The Version registers contents identify the version of the core. This register contains two bytes, one of which Synopsys uses to identify the core release number, and the other of which you set during coreKit configuration.*/
//====================================================================

#define  MAC_R8_RESERVED1_MASK 0xffff0000
#define  MAC_R8_RESERVED1_SHIFT 16
#define  MAC_R8_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R8_RESERVED1_MASK) >> MAC_R8_RESERVED1_SHIFT)
#define  MAC_R8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R8_RESERVED1_MASK) | (((uint32_t)val) << MAC_R8_RESERVED1_SHIFT))

#define  MAC_R8_UDID_MASK 0xff00
#define  MAC_R8_UDID_SHIFT 8
#define  MAC_R8_UDID_MASK_SHIFT(reg) (((reg) & MAC_R8_UDID_MASK) >> MAC_R8_UDID_SHIFT)
#define  MAC_R8_UDID_REPLACE_VAL(reg,val) (((reg) & ~MAC_R8_UDID_MASK) | (((uint32_t)val) << MAC_R8_UDID_SHIFT))

#define  MAC_R8_SDID_MASK 0xff
#define  MAC_R8_SDID_SHIFT 0
#define  MAC_R8_SDID_MASK_SHIFT(reg) (((reg) & MAC_R8_SDID_MASK) >> MAC_R8_SDID_SHIFT)
#define  MAC_R8_SDID_REPLACE_VAL(reg,val) (((reg) & ~MAC_R8_SDID_MASK) | (((uint32_t)val) << MAC_R8_SDID_SHIFT))

//====================================================================
//Register: Remote Wake-Up Frame Filter (r10)
/** \brief This is the address through which the remote Wake-up Frame Filter registers(wkupfmfilter_reg) are written/read by the Application. wkupfmfilter_reg is actually a pointer to eight (not transparent) such wkupfmfilter_reg registers. Eight sequential Writes to this address (028) will write all wkupfmfilter_reg registers. Eight sequential Reads from this address (028) will read all wkupfmfilter_reg registers.
This register is present only when the PMT module Remote Wake-up feature is selected in coreConsultant.
The register wkupfmfilter_reg, address (028H), loads the Wake-up Frame Filter register. To load values in a Wake-up Frame Filter register, the entire register (wkupfmfilter_reg) must be written. The wkupfmfilter_reg register is loaded by sequentially loading the eight register values in address (028) for wkupfmfilter_reg0, wkupfmfilter_reg1, ... wkupfmfilter_reg7, respectively. wkupfmfilter_reg is read in the same way.*/
//====================================================================

#define  MAC_R10_BIT_DATA_8_32_MASK 0xffffffff
#define  MAC_R10_BIT_DATA_8_32_SHIFT 0
#define  MAC_R10_BIT_DATA_8_32_MASK_SHIFT(reg) (((reg) & MAC_R10_BIT_DATA_8_32_MASK) >> MAC_R10_BIT_DATA_8_32_SHIFT)
#define  MAC_R10_BIT_DATA_8_32_REPLACE_VAL(reg,val) (((reg) & ~MAC_R10_BIT_DATA_8_32_MASK) | (((uint32_t)val) << MAC_R10_BIT_DATA_8_32_SHIFT))

//====================================================================
//Register: PMT Control and Status (r11)
/** \brief This register is present only when the PMT module is selected in coreConsultant.
The PMT CSR program the request wake-up events and monitor the wake-up events.*/
//====================================================================

#define  MAC_R11_PR_MASK 0x80000000
#define  MAC_R11_PR_SHIFT 31
#define  MAC_R11_PR_MASK_SHIFT(reg) (((reg) & MAC_R11_PR_MASK) >> MAC_R11_PR_SHIFT)
#define  MAC_R11_PR_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_PR_MASK) | (((uint32_t)val) << MAC_R11_PR_SHIFT))

#define  MAC_R11_RESERVED1_MASK 0x7ffffc00
#define  MAC_R11_RESERVED1_SHIFT 10
#define  MAC_R11_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R11_RESERVED1_MASK) >> MAC_R11_RESERVED1_SHIFT)
#define  MAC_R11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_RESERVED1_MASK) | (((uint32_t)val) << MAC_R11_RESERVED1_SHIFT))

#define  MAC_R11_GU_MASK 0x200
#define  MAC_R11_GU_SHIFT 9
#define  MAC_R11_GU_MASK_SHIFT(reg) (((reg) & MAC_R11_GU_MASK) >> MAC_R11_GU_SHIFT)
#define  MAC_R11_GU_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_GU_MASK) | (((uint32_t)val) << MAC_R11_GU_SHIFT))

#define  MAC_R11_RESERVED2_MASK 0x180
#define  MAC_R11_RESERVED2_SHIFT 7
#define  MAC_R11_RESERVED2_MASK_SHIFT(reg) (((reg) & MAC_R11_RESERVED2_MASK) >> MAC_R11_RESERVED2_SHIFT)
#define  MAC_R11_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_RESERVED2_MASK) | (((uint32_t)val) << MAC_R11_RESERVED2_SHIFT))

#define  MAC_R11_WUFR_MASK 0x40
#define  MAC_R11_WUFR_SHIFT 6
#define  MAC_R11_WUFR_MASK_SHIFT(reg) (((reg) & MAC_R11_WUFR_MASK) >> MAC_R11_WUFR_SHIFT)
#define  MAC_R11_WUFR_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_WUFR_MASK) | (((uint32_t)val) << MAC_R11_WUFR_SHIFT))

#define  MAC_R11_MPR_MASK 0x20
#define  MAC_R11_MPR_SHIFT 5
#define  MAC_R11_MPR_MASK_SHIFT(reg) (((reg) & MAC_R11_MPR_MASK) >> MAC_R11_MPR_SHIFT)
#define  MAC_R11_MPR_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_MPR_MASK) | (((uint32_t)val) << MAC_R11_MPR_SHIFT))

#define  MAC_R11_RESERVED3_MASK 0x18
#define  MAC_R11_RESERVED3_SHIFT 3
#define  MAC_R11_RESERVED3_MASK_SHIFT(reg) (((reg) & MAC_R11_RESERVED3_MASK) >> MAC_R11_RESERVED3_SHIFT)
#define  MAC_R11_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_RESERVED3_MASK) | (((uint32_t)val) << MAC_R11_RESERVED3_SHIFT))

#define  MAC_R11_WUFE_MASK 0x4
#define  MAC_R11_WUFE_SHIFT 2
#define  MAC_R11_WUFE_MASK_SHIFT(reg) (((reg) & MAC_R11_WUFE_MASK) >> MAC_R11_WUFE_SHIFT)
#define  MAC_R11_WUFE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_WUFE_MASK) | (((uint32_t)val) << MAC_R11_WUFE_SHIFT))

#define  MAC_R11_MPE_MASK 0x2
#define  MAC_R11_MPE_SHIFT 1
#define  MAC_R11_MPE_MASK_SHIFT(reg) (((reg) & MAC_R11_MPE_MASK) >> MAC_R11_MPE_SHIFT)
#define  MAC_R11_MPE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_MPE_MASK) | (((uint32_t)val) << MAC_R11_MPE_SHIFT))

#define  MAC_R11_PWRDN_MASK 0x1
#define  MAC_R11_PWRDN_SHIFT 0
#define  MAC_R11_PWRDN_MASK_SHIFT(reg) (((reg) & MAC_R11_PWRDN_MASK) >> MAC_R11_PWRDN_SHIFT)
#define  MAC_R11_PWRDN_REPLACE_VAL(reg,val) (((reg) & ~MAC_R11_PWRDN_MASK) | (((uint32_t)val) << MAC_R11_PWRDN_SHIFT))

//====================================================================
//Register: Interrrupt Status Register (r14)
/** \brief Contains the interrupt status.
The Interrupt Status register contents identify the events in the MAC-CORE that can generate interrupt. Note that all the interrupt events are generated only when the corresponding optional feature is selected during coreKit configuration and enabled during operation. Hence, these bits are reserved when the corresponding features is not present in the core.*/
//====================================================================

#define  MAC_R14_RESERVED1_MASK 0xffffff00
#define  MAC_R14_RESERVED1_SHIFT 8
#define  MAC_R14_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R14_RESERVED1_MASK) >> MAC_R14_RESERVED1_SHIFT)
#define  MAC_R14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_RESERVED1_MASK) | (((uint32_t)val) << MAC_R14_RESERVED1_SHIFT))

#define  MAC_R14_MMCRXCSO_MASK 0x80
#define  MAC_R14_MMCRXCSO_SHIFT 7
#define  MAC_R14_MMCRXCSO_MASK_SHIFT(reg) (((reg) & MAC_R14_MMCRXCSO_MASK) >> MAC_R14_MMCRXCSO_SHIFT)
#define  MAC_R14_MMCRXCSO_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_MMCRXCSO_MASK) | (((uint32_t)val) << MAC_R14_MMCRXCSO_SHIFT))

#define  MAC_R14_MMCTX_MASK 0x40
#define  MAC_R14_MMCTX_SHIFT 6
#define  MAC_R14_MMCTX_MASK_SHIFT(reg) (((reg) & MAC_R14_MMCTX_MASK) >> MAC_R14_MMCTX_SHIFT)
#define  MAC_R14_MMCTX_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_MMCTX_MASK) | (((uint32_t)val) << MAC_R14_MMCTX_SHIFT))

#define  MAC_R14_MMCRX_MASK 0x20
#define  MAC_R14_MMCRX_SHIFT 5
#define  MAC_R14_MMCRX_MASK_SHIFT(reg) (((reg) & MAC_R14_MMCRX_MASK) >> MAC_R14_MMCRX_SHIFT)
#define  MAC_R14_MMCRX_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_MMCRX_MASK) | (((uint32_t)val) << MAC_R14_MMCRX_SHIFT))

#define  MAC_R14_MMC_MASK 0x10
#define  MAC_R14_MMC_SHIFT 4
#define  MAC_R14_MMC_MASK_SHIFT(reg) (((reg) & MAC_R14_MMC_MASK) >> MAC_R14_MMC_SHIFT)
#define  MAC_R14_MMC_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_MMC_MASK) | (((uint32_t)val) << MAC_R14_MMC_SHIFT))

#define  MAC_R14_PMT_MASK 0x8
#define  MAC_R14_PMT_SHIFT 3
#define  MAC_R14_PMT_MASK_SHIFT(reg) (((reg) & MAC_R14_PMT_MASK) >> MAC_R14_PMT_SHIFT)
#define  MAC_R14_PMT_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_PMT_MASK) | (((uint32_t)val) << MAC_R14_PMT_SHIFT))

#define  MAC_R14_RESERVED2_MASK 0x6
#define  MAC_R14_RESERVED2_SHIFT 1
#define  MAC_R14_RESERVED2_MASK_SHIFT(reg) (((reg) & MAC_R14_RESERVED2_MASK) >> MAC_R14_RESERVED2_SHIFT)
#define  MAC_R14_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_RESERVED2_MASK) | (((uint32_t)val) << MAC_R14_RESERVED2_SHIFT))

#define  MAC_R14_RGMII_MASK 0x1
#define  MAC_R14_RGMII_SHIFT 0
#define  MAC_R14_RGMII_MASK_SHIFT(reg) (((reg) & MAC_R14_RGMII_MASK) >> MAC_R14_RGMII_SHIFT)
#define  MAC_R14_RGMII_REPLACE_VAL(reg,val) (((reg) & ~MAC_R14_RGMII_MASK) | (((uint32_t)val) << MAC_R14_RGMII_SHIFT))

//====================================================================
//Register: Interrupt Mask Register (r15)
/** \brief The Interrupt Mask Register bits enables the user to mask the interrupt signal due to the corresponding event in the Interrupt Status Register. The interrupt signal is sbd_intr_o in MAC-AHB and MAC-DMA configuration while the interrupt signal is mci_intr_o in the MAC-MTL and MAC-CORE configuration.*/
//====================================================================

#define  MAC_R15_RESERVED1_MASK 0xfffffff0
#define  MAC_R15_RESERVED1_SHIFT 4
#define  MAC_R15_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R15_RESERVED1_MASK) >> MAC_R15_RESERVED1_SHIFT)
#define  MAC_R15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R15_RESERVED1_MASK) | (((uint32_t)val) << MAC_R15_RESERVED1_SHIFT))

#define  MAC_R15_PMT_MASK 0x8
#define  MAC_R15_PMT_SHIFT 3
#define  MAC_R15_PMT_MASK_SHIFT(reg) (((reg) & MAC_R15_PMT_MASK) >> MAC_R15_PMT_SHIFT)
#define  MAC_R15_PMT_REPLACE_VAL(reg,val) (((reg) & ~MAC_R15_PMT_MASK) | (((uint32_t)val) << MAC_R15_PMT_SHIFT))

#define  MAC_R15_RESERVED2_MASK 0x6
#define  MAC_R15_RESERVED2_SHIFT 1
#define  MAC_R15_RESERVED2_MASK_SHIFT(reg) (((reg) & MAC_R15_RESERVED2_MASK) >> MAC_R15_RESERVED2_SHIFT)
#define  MAC_R15_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAC_R15_RESERVED2_MASK) | (((uint32_t)val) << MAC_R15_RESERVED2_SHIFT))

#define  MAC_R15_RESERVED3_MASK 0x1
#define  MAC_R15_RESERVED3_SHIFT 0
#define  MAC_R15_RESERVED3_MASK_SHIFT(reg) (((reg) & MAC_R15_RESERVED3_MASK) >> MAC_R15_RESERVED3_SHIFT)
#define  MAC_R15_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~MAC_R15_RESERVED3_MASK) | (((uint32_t)val) << MAC_R15_RESERVED3_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC0)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC0_MO_MASK 0x80000000
#define  MAC_ADDRHI_MAC0_MO_SHIFT 31
#define  MAC_ADDRHI_MAC0_MO_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC0_MO_MASK) >> MAC_ADDRHI_MAC0_MO_SHIFT)
#define  MAC_ADDRHI_MAC0_MO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC0_MO_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC0_MO_SHIFT))

#define  MAC_ADDRHI_MAC0_RESERVED1_MASK 0x7fff0000
#define  MAC_ADDRHI_MAC0_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC0_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC0_RESERVED1_MASK) >> MAC_ADDRHI_MAC0_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC0_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC0_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC0_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC0_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC0_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC0_MA_HI_MASK) >> MAC_ADDRHI_MAC0_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC0_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC0_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC0_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC1)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC1_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC1_AE_SHIFT 31
#define  MAC_ADDRHI_MAC1_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC1_AE_MASK) >> MAC_ADDRHI_MAC1_AE_SHIFT)
#define  MAC_ADDRHI_MAC1_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC1_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC1_AE_SHIFT))

#define  MAC_ADDRHI_MAC1_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC1_SA_SHIFT 30
#define  MAC_ADDRHI_MAC1_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC1_SA_MASK) >> MAC_ADDRHI_MAC1_SA_SHIFT)
#define  MAC_ADDRHI_MAC1_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC1_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC1_SA_SHIFT))

#define  MAC_ADDRHI_MAC1_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC1_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC1_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC1_MBC_MASK) >> MAC_ADDRHI_MAC1_MBC_SHIFT)
#define  MAC_ADDRHI_MAC1_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC1_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC1_MBC_SHIFT))

#define  MAC_ADDRHI_MAC1_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC1_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC1_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC1_RESERVED1_MASK) >> MAC_ADDRHI_MAC1_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC1_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC1_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC1_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC1_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC1_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC1_MA_HI_MASK) >> MAC_ADDRHI_MAC1_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC1_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC1_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC1_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC2)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC2_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC2_AE_SHIFT 31
#define  MAC_ADDRHI_MAC2_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC2_AE_MASK) >> MAC_ADDRHI_MAC2_AE_SHIFT)
#define  MAC_ADDRHI_MAC2_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC2_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC2_AE_SHIFT))

#define  MAC_ADDRHI_MAC2_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC2_SA_SHIFT 30
#define  MAC_ADDRHI_MAC2_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC2_SA_MASK) >> MAC_ADDRHI_MAC2_SA_SHIFT)
#define  MAC_ADDRHI_MAC2_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC2_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC2_SA_SHIFT))

#define  MAC_ADDRHI_MAC2_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC2_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC2_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC2_MBC_MASK) >> MAC_ADDRHI_MAC2_MBC_SHIFT)
#define  MAC_ADDRHI_MAC2_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC2_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC2_MBC_SHIFT))

#define  MAC_ADDRHI_MAC2_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC2_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC2_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC2_RESERVED1_MASK) >> MAC_ADDRHI_MAC2_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC2_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC2_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC2_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC2_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC2_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC2_MA_HI_MASK) >> MAC_ADDRHI_MAC2_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC2_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC2_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC2_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC3)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC3_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC3_AE_SHIFT 31
#define  MAC_ADDRHI_MAC3_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC3_AE_MASK) >> MAC_ADDRHI_MAC3_AE_SHIFT)
#define  MAC_ADDRHI_MAC3_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC3_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC3_AE_SHIFT))

#define  MAC_ADDRHI_MAC3_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC3_SA_SHIFT 30
#define  MAC_ADDRHI_MAC3_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC3_SA_MASK) >> MAC_ADDRHI_MAC3_SA_SHIFT)
#define  MAC_ADDRHI_MAC3_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC3_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC3_SA_SHIFT))

#define  MAC_ADDRHI_MAC3_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC3_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC3_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC3_MBC_MASK) >> MAC_ADDRHI_MAC3_MBC_SHIFT)
#define  MAC_ADDRHI_MAC3_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC3_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC3_MBC_SHIFT))

#define  MAC_ADDRHI_MAC3_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC3_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC3_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC3_RESERVED1_MASK) >> MAC_ADDRHI_MAC3_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC3_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC3_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC3_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC3_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC3_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC3_MA_HI_MASK) >> MAC_ADDRHI_MAC3_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC3_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC3_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC3_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC4)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC4_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC4_AE_SHIFT 31
#define  MAC_ADDRHI_MAC4_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC4_AE_MASK) >> MAC_ADDRHI_MAC4_AE_SHIFT)
#define  MAC_ADDRHI_MAC4_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC4_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC4_AE_SHIFT))

#define  MAC_ADDRHI_MAC4_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC4_SA_SHIFT 30
#define  MAC_ADDRHI_MAC4_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC4_SA_MASK) >> MAC_ADDRHI_MAC4_SA_SHIFT)
#define  MAC_ADDRHI_MAC4_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC4_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC4_SA_SHIFT))

#define  MAC_ADDRHI_MAC4_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC4_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC4_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC4_MBC_MASK) >> MAC_ADDRHI_MAC4_MBC_SHIFT)
#define  MAC_ADDRHI_MAC4_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC4_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC4_MBC_SHIFT))

#define  MAC_ADDRHI_MAC4_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC4_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC4_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC4_RESERVED1_MASK) >> MAC_ADDRHI_MAC4_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC4_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC4_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC4_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC4_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC4_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC4_MA_HI_MASK) >> MAC_ADDRHI_MAC4_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC4_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC4_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC4_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC5)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC5_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC5_AE_SHIFT 31
#define  MAC_ADDRHI_MAC5_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC5_AE_MASK) >> MAC_ADDRHI_MAC5_AE_SHIFT)
#define  MAC_ADDRHI_MAC5_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC5_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC5_AE_SHIFT))

#define  MAC_ADDRHI_MAC5_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC5_SA_SHIFT 30
#define  MAC_ADDRHI_MAC5_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC5_SA_MASK) >> MAC_ADDRHI_MAC5_SA_SHIFT)
#define  MAC_ADDRHI_MAC5_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC5_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC5_SA_SHIFT))

#define  MAC_ADDRHI_MAC5_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC5_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC5_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC5_MBC_MASK) >> MAC_ADDRHI_MAC5_MBC_SHIFT)
#define  MAC_ADDRHI_MAC5_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC5_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC5_MBC_SHIFT))

#define  MAC_ADDRHI_MAC5_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC5_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC5_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC5_RESERVED1_MASK) >> MAC_ADDRHI_MAC5_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC5_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC5_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC5_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC5_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC5_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC5_MA_HI_MASK) >> MAC_ADDRHI_MAC5_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC5_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC5_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC5_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC6)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC6_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC6_AE_SHIFT 31
#define  MAC_ADDRHI_MAC6_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC6_AE_MASK) >> MAC_ADDRHI_MAC6_AE_SHIFT)
#define  MAC_ADDRHI_MAC6_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC6_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC6_AE_SHIFT))

#define  MAC_ADDRHI_MAC6_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC6_SA_SHIFT 30
#define  MAC_ADDRHI_MAC6_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC6_SA_MASK) >> MAC_ADDRHI_MAC6_SA_SHIFT)
#define  MAC_ADDRHI_MAC6_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC6_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC6_SA_SHIFT))

#define  MAC_ADDRHI_MAC6_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC6_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC6_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC6_MBC_MASK) >> MAC_ADDRHI_MAC6_MBC_SHIFT)
#define  MAC_ADDRHI_MAC6_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC6_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC6_MBC_SHIFT))

#define  MAC_ADDRHI_MAC6_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC6_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC6_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC6_RESERVED1_MASK) >> MAC_ADDRHI_MAC6_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC6_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC6_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC6_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC6_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC6_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC6_MA_HI_MASK) >> MAC_ADDRHI_MAC6_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC6_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC6_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC6_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC7)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC7_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC7_AE_SHIFT 31
#define  MAC_ADDRHI_MAC7_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC7_AE_MASK) >> MAC_ADDRHI_MAC7_AE_SHIFT)
#define  MAC_ADDRHI_MAC7_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC7_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC7_AE_SHIFT))

#define  MAC_ADDRHI_MAC7_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC7_SA_SHIFT 30
#define  MAC_ADDRHI_MAC7_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC7_SA_MASK) >> MAC_ADDRHI_MAC7_SA_SHIFT)
#define  MAC_ADDRHI_MAC7_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC7_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC7_SA_SHIFT))

#define  MAC_ADDRHI_MAC7_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC7_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC7_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC7_MBC_MASK) >> MAC_ADDRHI_MAC7_MBC_SHIFT)
#define  MAC_ADDRHI_MAC7_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC7_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC7_MBC_SHIFT))

#define  MAC_ADDRHI_MAC7_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC7_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC7_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC7_RESERVED1_MASK) >> MAC_ADDRHI_MAC7_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC7_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC7_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC7_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC7_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC7_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC7_MA_HI_MASK) >> MAC_ADDRHI_MAC7_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC7_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC7_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC7_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC8)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC8_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC8_AE_SHIFT 31
#define  MAC_ADDRHI_MAC8_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC8_AE_MASK) >> MAC_ADDRHI_MAC8_AE_SHIFT)
#define  MAC_ADDRHI_MAC8_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC8_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC8_AE_SHIFT))

#define  MAC_ADDRHI_MAC8_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC8_SA_SHIFT 30
#define  MAC_ADDRHI_MAC8_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC8_SA_MASK) >> MAC_ADDRHI_MAC8_SA_SHIFT)
#define  MAC_ADDRHI_MAC8_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC8_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC8_SA_SHIFT))

#define  MAC_ADDRHI_MAC8_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC8_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC8_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC8_MBC_MASK) >> MAC_ADDRHI_MAC8_MBC_SHIFT)
#define  MAC_ADDRHI_MAC8_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC8_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC8_MBC_SHIFT))

#define  MAC_ADDRHI_MAC8_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC8_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC8_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC8_RESERVED1_MASK) >> MAC_ADDRHI_MAC8_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC8_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC8_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC8_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC8_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC8_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC8_MA_HI_MASK) >> MAC_ADDRHI_MAC8_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC8_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC8_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC8_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC9)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC9_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC9_AE_SHIFT 31
#define  MAC_ADDRHI_MAC9_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC9_AE_MASK) >> MAC_ADDRHI_MAC9_AE_SHIFT)
#define  MAC_ADDRHI_MAC9_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC9_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC9_AE_SHIFT))

#define  MAC_ADDRHI_MAC9_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC9_SA_SHIFT 30
#define  MAC_ADDRHI_MAC9_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC9_SA_MASK) >> MAC_ADDRHI_MAC9_SA_SHIFT)
#define  MAC_ADDRHI_MAC9_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC9_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC9_SA_SHIFT))

#define  MAC_ADDRHI_MAC9_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC9_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC9_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC9_MBC_MASK) >> MAC_ADDRHI_MAC9_MBC_SHIFT)
#define  MAC_ADDRHI_MAC9_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC9_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC9_MBC_SHIFT))

#define  MAC_ADDRHI_MAC9_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC9_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC9_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC9_RESERVED1_MASK) >> MAC_ADDRHI_MAC9_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC9_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC9_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC9_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC9_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC9_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC9_MA_HI_MASK) >> MAC_ADDRHI_MAC9_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC9_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC9_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC9_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC10)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC10_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC10_AE_SHIFT 31
#define  MAC_ADDRHI_MAC10_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC10_AE_MASK) >> MAC_ADDRHI_MAC10_AE_SHIFT)
#define  MAC_ADDRHI_MAC10_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC10_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC10_AE_SHIFT))

#define  MAC_ADDRHI_MAC10_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC10_SA_SHIFT 30
#define  MAC_ADDRHI_MAC10_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC10_SA_MASK) >> MAC_ADDRHI_MAC10_SA_SHIFT)
#define  MAC_ADDRHI_MAC10_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC10_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC10_SA_SHIFT))

#define  MAC_ADDRHI_MAC10_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC10_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC10_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC10_MBC_MASK) >> MAC_ADDRHI_MAC10_MBC_SHIFT)
#define  MAC_ADDRHI_MAC10_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC10_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC10_MBC_SHIFT))

#define  MAC_ADDRHI_MAC10_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC10_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC10_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC10_RESERVED1_MASK) >> MAC_ADDRHI_MAC10_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC10_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC10_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC10_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC10_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC10_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC10_MA_HI_MASK) >> MAC_ADDRHI_MAC10_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC10_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC10_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC10_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC11)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC11_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC11_AE_SHIFT 31
#define  MAC_ADDRHI_MAC11_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC11_AE_MASK) >> MAC_ADDRHI_MAC11_AE_SHIFT)
#define  MAC_ADDRHI_MAC11_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC11_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC11_AE_SHIFT))

#define  MAC_ADDRHI_MAC11_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC11_SA_SHIFT 30
#define  MAC_ADDRHI_MAC11_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC11_SA_MASK) >> MAC_ADDRHI_MAC11_SA_SHIFT)
#define  MAC_ADDRHI_MAC11_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC11_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC11_SA_SHIFT))

#define  MAC_ADDRHI_MAC11_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC11_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC11_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC11_MBC_MASK) >> MAC_ADDRHI_MAC11_MBC_SHIFT)
#define  MAC_ADDRHI_MAC11_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC11_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC11_MBC_SHIFT))

#define  MAC_ADDRHI_MAC11_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC11_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC11_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC11_RESERVED1_MASK) >> MAC_ADDRHI_MAC11_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC11_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC11_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC11_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC11_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC11_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC11_MA_HI_MASK) >> MAC_ADDRHI_MAC11_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC11_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC11_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC11_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC12)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC12_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC12_AE_SHIFT 31
#define  MAC_ADDRHI_MAC12_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC12_AE_MASK) >> MAC_ADDRHI_MAC12_AE_SHIFT)
#define  MAC_ADDRHI_MAC12_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC12_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC12_AE_SHIFT))

#define  MAC_ADDRHI_MAC12_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC12_SA_SHIFT 30
#define  MAC_ADDRHI_MAC12_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC12_SA_MASK) >> MAC_ADDRHI_MAC12_SA_SHIFT)
#define  MAC_ADDRHI_MAC12_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC12_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC12_SA_SHIFT))

#define  MAC_ADDRHI_MAC12_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC12_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC12_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC12_MBC_MASK) >> MAC_ADDRHI_MAC12_MBC_SHIFT)
#define  MAC_ADDRHI_MAC12_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC12_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC12_MBC_SHIFT))

#define  MAC_ADDRHI_MAC12_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC12_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC12_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC12_RESERVED1_MASK) >> MAC_ADDRHI_MAC12_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC12_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC12_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC12_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC12_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC12_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC12_MA_HI_MASK) >> MAC_ADDRHI_MAC12_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC12_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC12_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC12_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC13)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC13_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC13_AE_SHIFT 31
#define  MAC_ADDRHI_MAC13_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC13_AE_MASK) >> MAC_ADDRHI_MAC13_AE_SHIFT)
#define  MAC_ADDRHI_MAC13_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC13_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC13_AE_SHIFT))

#define  MAC_ADDRHI_MAC13_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC13_SA_SHIFT 30
#define  MAC_ADDRHI_MAC13_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC13_SA_MASK) >> MAC_ADDRHI_MAC13_SA_SHIFT)
#define  MAC_ADDRHI_MAC13_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC13_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC13_SA_SHIFT))

#define  MAC_ADDRHI_MAC13_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC13_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC13_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC13_MBC_MASK) >> MAC_ADDRHI_MAC13_MBC_SHIFT)
#define  MAC_ADDRHI_MAC13_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC13_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC13_MBC_SHIFT))

#define  MAC_ADDRHI_MAC13_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC13_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC13_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC13_RESERVED1_MASK) >> MAC_ADDRHI_MAC13_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC13_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC13_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC13_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC13_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC13_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC13_MA_HI_MASK) >> MAC_ADDRHI_MAC13_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC13_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC13_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC13_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC14)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC14_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC14_AE_SHIFT 31
#define  MAC_ADDRHI_MAC14_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC14_AE_MASK) >> MAC_ADDRHI_MAC14_AE_SHIFT)
#define  MAC_ADDRHI_MAC14_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC14_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC14_AE_SHIFT))

#define  MAC_ADDRHI_MAC14_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC14_SA_SHIFT 30
#define  MAC_ADDRHI_MAC14_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC14_SA_MASK) >> MAC_ADDRHI_MAC14_SA_SHIFT)
#define  MAC_ADDRHI_MAC14_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC14_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC14_SA_SHIFT))

#define  MAC_ADDRHI_MAC14_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC14_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC14_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC14_MBC_MASK) >> MAC_ADDRHI_MAC14_MBC_SHIFT)
#define  MAC_ADDRHI_MAC14_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC14_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC14_MBC_SHIFT))

#define  MAC_ADDRHI_MAC14_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC14_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC14_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC14_RESERVED1_MASK) >> MAC_ADDRHI_MAC14_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC14_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC14_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC14_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC14_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC14_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC14_MA_HI_MASK) >> MAC_ADDRHI_MAC14_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC14_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC14_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC14_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (AddrHi_MAC15)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates.*/
//====================================================================

#define  MAC_ADDRHI_MAC15_AE_MASK 0x80000000
#define  MAC_ADDRHI_MAC15_AE_SHIFT 31
#define  MAC_ADDRHI_MAC15_AE_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC15_AE_MASK) >> MAC_ADDRHI_MAC15_AE_SHIFT)
#define  MAC_ADDRHI_MAC15_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC15_AE_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC15_AE_SHIFT))

#define  MAC_ADDRHI_MAC15_SA_MASK 0x40000000
#define  MAC_ADDRHI_MAC15_SA_SHIFT 30
#define  MAC_ADDRHI_MAC15_SA_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC15_SA_MASK) >> MAC_ADDRHI_MAC15_SA_SHIFT)
#define  MAC_ADDRHI_MAC15_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC15_SA_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC15_SA_SHIFT))

#define  MAC_ADDRHI_MAC15_MBC_MASK 0x3f000000
#define  MAC_ADDRHI_MAC15_MBC_SHIFT 24
#define  MAC_ADDRHI_MAC15_MBC_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC15_MBC_MASK) >> MAC_ADDRHI_MAC15_MBC_SHIFT)
#define  MAC_ADDRHI_MAC15_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC15_MBC_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC15_MBC_SHIFT))

#define  MAC_ADDRHI_MAC15_RESERVED1_MASK 0xff0000
#define  MAC_ADDRHI_MAC15_RESERVED1_SHIFT 16
#define  MAC_ADDRHI_MAC15_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC15_RESERVED1_MASK) >> MAC_ADDRHI_MAC15_RESERVED1_SHIFT)
#define  MAC_ADDRHI_MAC15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC15_RESERVED1_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC15_RESERVED1_SHIFT))

#define  MAC_ADDRHI_MAC15_MA_HI_MASK 0xffff
#define  MAC_ADDRHI_MAC15_MA_HI_SHIFT 0
#define  MAC_ADDRHI_MAC15_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_ADDRHI_MAC15_MA_HI_MASK) >> MAC_ADDRHI_MAC15_MA_HI_SHIFT)
#define  MAC_ADDRHI_MAC15_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRHI_MAC15_MA_HI_MASK) | (((uint32_t)val) << MAC_ADDRHI_MAC15_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 1 of 16) (AddrLo_MAC0)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 1 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC0_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC0_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC0_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC0_MA_LO_MASK) >> MAC_ADDRLO_MAC0_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC0_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC0_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC0_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 2 of 16) (AddrLo_MAC1)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 2 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC1_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC1_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC1_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC1_MA_LO_MASK) >> MAC_ADDRLO_MAC1_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC1_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC1_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC1_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 3 of 16) (AddrLo_MAC2)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 3 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC2_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC2_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC2_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC2_MA_LO_MASK) >> MAC_ADDRLO_MAC2_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC2_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC2_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC2_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 4 of 16) (AddrLo_MAC3)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 4 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC3_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC3_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC3_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC3_MA_LO_MASK) >> MAC_ADDRLO_MAC3_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC3_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC3_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC3_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 5 of 16) (AddrLo_MAC4)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 5 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC4_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC4_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC4_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC4_MA_LO_MASK) >> MAC_ADDRLO_MAC4_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC4_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC4_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC4_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 6 of 16) (AddrLo_MAC5)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 6 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC5_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC5_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC5_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC5_MA_LO_MASK) >> MAC_ADDRLO_MAC5_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC5_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC5_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC5_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 7 of 16) (AddrLo_MAC6)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 7 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC6_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC6_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC6_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC6_MA_LO_MASK) >> MAC_ADDRLO_MAC6_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC6_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC6_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC6_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 8 of 16) (AddrLo_MAC7)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 8 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC7_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC7_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC7_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC7_MA_LO_MASK) >> MAC_ADDRLO_MAC7_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC7_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC7_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC7_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 9 of 16) (AddrLo_MAC8)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 9 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC8_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC8_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC8_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC8_MA_LO_MASK) >> MAC_ADDRLO_MAC8_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC8_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC8_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC8_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 10 of 16) (AddrLo_MAC9)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 10 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC9_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC9_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC9_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC9_MA_LO_MASK) >> MAC_ADDRLO_MAC9_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC9_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC9_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC9_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 11 of 16) (AddrLo_MAC10)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 11 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC10_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC10_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC10_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC10_MA_LO_MASK) >> MAC_ADDRLO_MAC10_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC10_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC10_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC10_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 12 of 16) (AddrLo_MAC11)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 12 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC11_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC11_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC11_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC11_MA_LO_MASK) >> MAC_ADDRLO_MAC11_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC11_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC11_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC11_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 13 of 16) (AddrLo_MAC12)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 13 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC12_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC12_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC12_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC12_MA_LO_MASK) >> MAC_ADDRLO_MAC12_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC12_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC12_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC12_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 14 of 16) (AddrLo_MAC13)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 14 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC13_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC13_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC13_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC13_MA_LO_MASK) >> MAC_ADDRLO_MAC13_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC13_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC13_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC13_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 15 of 16) (AddrLo_MAC14)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 15 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC14_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC14_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC14_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC14_MA_LO_MASK) >> MAC_ADDRLO_MAC14_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC14_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC14_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC14_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 16 of 16) (AddrLo_MAC15)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 16 of 16)*/
//====================================================================

#define  MAC_ADDRLO_MAC15_MA_LO_MASK 0xffffffff
#define  MAC_ADDRLO_MAC15_MA_LO_SHIFT 0
#define  MAC_ADDRLO_MAC15_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_ADDRLO_MAC15_MA_LO_MASK) >> MAC_ADDRLO_MAC15_MA_LO_SHIFT)
#define  MAC_ADDRLO_MAC15_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_ADDRLO_MAC15_MA_LO_MASK) | (((uint32_t)val) << MAC_ADDRLO_MAC15_MA_LO_SHIFT))

//====================================================================
//Register: SGMII/RGMII Status Register (r54)
/** \brief Indicates the status signals received from the PHY through the SGMII/RGMII interface.
The SGMII/RGMII Status register indicates the status signals received by the SGMII/RGMII (whichever is selected at reset) from the PHY. This register is optional and is present only when the core is configured for RGMII/SGMII PHY interfaces.*/
//====================================================================

#define  MAC_R54_RESERVED1_MASK 0xfffffff0
#define  MAC_R54_RESERVED1_SHIFT 4
#define  MAC_R54_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_R54_RESERVED1_MASK) >> MAC_R54_RESERVED1_SHIFT)
#define  MAC_R54_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_R54_RESERVED1_MASK) | (((uint32_t)val) << MAC_R54_RESERVED1_SHIFT))

#define  MAC_R54_LINK_MASK 0x8
#define  MAC_R54_LINK_SHIFT 3
#define  MAC_R54_LINK_MASK_SHIFT(reg) (((reg) & MAC_R54_LINK_MASK) >> MAC_R54_LINK_SHIFT)
#define  MAC_R54_LINK_REPLACE_VAL(reg,val) (((reg) & ~MAC_R54_LINK_MASK) | (((uint32_t)val) << MAC_R54_LINK_SHIFT))

#define  MAC_R54_SPEED_MASK 0x6
#define  MAC_R54_SPEED_SHIFT 1
#define  MAC_R54_SPEED_MASK_SHIFT(reg) (((reg) & MAC_R54_SPEED_MASK) >> MAC_R54_SPEED_SHIFT)
#define  MAC_R54_SPEED_REPLACE_VAL(reg,val) (((reg) & ~MAC_R54_SPEED_MASK) | (((uint32_t)val) << MAC_R54_SPEED_SHIFT))

#define  MAC_R54_MODE_MASK 0x1
#define  MAC_R54_MODE_SHIFT 0
#define  MAC_R54_MODE_MASK_SHIFT(reg) (((reg) & MAC_R54_MODE_MASK) >> MAC_R54_MODE_SHIFT)
#define  MAC_R54_MODE_REPLACE_VAL(reg,val) (((reg) & ~MAC_R54_MODE_MASK) | (((uint32_t)val) << MAC_R54_MODE_SHIFT))

//====================================================================
//
//Register File: LAN DWIP MMC Registers (MMC)
/** \brief This section defines the bits for each MMC register.
MMC address range is 0100 to 02cf
The counters in the MAC Management Counters (MMC) module can be viewed as an extension of the register address space of the CSR module. The MMC module maintains a set of registers for gathering statistics on the received and transmitted frames. These include a control register for controlling the behavior of the registers, two 32-bit registers containing interrupts generated (receive and transmit), and two 32-bit registers containing masks for the Interrupt register (receive and transmit). These registers are accessible from the Application through the MAC Control Interface (MCI). Each register is 32 bits wide. The write data is qualified with the corresponding mci_be_i signals. Thus, non-32 bit accesses are allowed as long as the address is word-aligned.
The MMCs are accessed using transactions, in the same way the CSR address space is accessed. The following sections in the chapter describe the various counters and list the address for each of the statistics counters. This address will be used for Read/Write accesses to the desired transmit/receive counter.
The Receive MMC counters are updated for frames that are passed by the Address Filter (AFM) block. Statistics of frames that are dropped by the AFM module are not updated unless they are runt frames of less than 6 bytes (DA bytes are not received fully).
The MMC module gathers statistics on encapsulated IPv4, IPv6, TCP, UDP, or ICMP payloads in received Ethernet frames. This gathering is only enabled when Full Checksum Offload Engine is selected during RTL configuration. The address map of the corresponding registers is 0x0200 to 0x02FC.
The MMC register naming convention is as follows.
"tx" as a prefix or suffix indicates counters associated with transmission
"rx" as a prefix or suffix indicates counters associated with reception
"_g" as a suffix indicates registers that count good frames only
"_gb" as a suffix indicates registers that count frames regardless of whether they are good or bad
Transmitted frames are considered "Bad" (and are thus aborted) if one or more of the following conditions exists.
Jabber Timeout
No Carrier/Loss of Carrier
Late Collision
Frame Underflow error
Received frames are considered "Bad" if one of the following conditions exists.
CRC error
Length error
Watchdog timeout
Missed frame error
Maximum frame size is dependent on the frame type, as follows.
Untagged frame maxsize = 1518
VLAN Frame maxsize = 1522
Jumbo Frame maxsize = 9018
JumboVLAN Frame maxsize = 9022*/
//
//====================================================================

//====================================================================
//Register: MMC Control Register (cntrl)
/** \brief Establishes the operating mode of MMC.*/
//====================================================================

#define  MMC_CNTRL_RESERVED1_MASK 0xfffffff0
#define  MMC_CNTRL_RESERVED1_SHIFT 4
#define  MMC_CNTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & MMC_CNTRL_RESERVED1_MASK) >> MMC_CNTRL_RESERVED1_SHIFT)
#define  MMC_CNTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MMC_CNTRL_RESERVED1_MASK) | (((uint32_t)val) << MMC_CNTRL_RESERVED1_SHIFT))

#define  MMC_CNTRL_MMCFRZ_MASK 0x8
#define  MMC_CNTRL_MMCFRZ_SHIFT 3
#define  MMC_CNTRL_MMCFRZ_MASK_SHIFT(reg) (((reg) & MMC_CNTRL_MMCFRZ_MASK) >> MMC_CNTRL_MMCFRZ_SHIFT)
#define  MMC_CNTRL_MMCFRZ_REPLACE_VAL(reg,val) (((reg) & ~MMC_CNTRL_MMCFRZ_MASK) | (((uint32_t)val) << MMC_CNTRL_MMCFRZ_SHIFT))

#define  MMC_CNTRL_RST_ON_RD_MASK 0x4
#define  MMC_CNTRL_RST_ON_RD_SHIFT 2
#define  MMC_CNTRL_RST_ON_RD_MASK_SHIFT(reg) (((reg) & MMC_CNTRL_RST_ON_RD_MASK) >> MMC_CNTRL_RST_ON_RD_SHIFT)
#define  MMC_CNTRL_RST_ON_RD_REPLACE_VAL(reg,val) (((reg) & ~MMC_CNTRL_RST_ON_RD_MASK) | (((uint32_t)val) << MMC_CNTRL_RST_ON_RD_SHIFT))

#define  MMC_CNTRL_STOP_RO_MASK 0x2
#define  MMC_CNTRL_STOP_RO_SHIFT 1
#define  MMC_CNTRL_STOP_RO_MASK_SHIFT(reg) (((reg) & MMC_CNTRL_STOP_RO_MASK) >> MMC_CNTRL_STOP_RO_SHIFT)
#define  MMC_CNTRL_STOP_RO_REPLACE_VAL(reg,val) (((reg) & ~MMC_CNTRL_STOP_RO_MASK) | (((uint32_t)val) << MMC_CNTRL_STOP_RO_SHIFT))

#define  MMC_CNTRL_RESET_MASK 0x1
#define  MMC_CNTRL_RESET_SHIFT 0
#define  MMC_CNTRL_RESET_MASK_SHIFT(reg) (((reg) & MMC_CNTRL_RESET_MASK) >> MMC_CNTRL_RESET_SHIFT)
#define  MMC_CNTRL_RESET_REPLACE_VAL(reg,val) (((reg) & ~MMC_CNTRL_RESET_MASK) | (((uint32_t)val) << MMC_CNTRL_RESET_SHIFT))

//====================================================================
//Register: MMC Receive Interrupt Register (intr_rx)
/** \brief Maintains the interrupt generated from all of the receive statisic counters.
The MMC Receive Interrupt register maintains the interrupts generated when receive statistic counters reach half their maximum values. (MSB of the counter is set.) It is a 32-bit wide register. An interrupt bit is cleared when the respective MMC counter that caused the interrupt is read. The least significant byte lane (bits[7:0]) of the respective counter must be read in order to clear the interrupt bit.
Note R_SS_RC means that this register bit is set internally and is cleared when the Counter register is read.*/
//====================================================================

#define  MMC_INTR_RX_RESERVED1_MASK 0xff000000
#define  MMC_INTR_RX_RESERVED1_SHIFT 24
#define  MMC_INTR_RX_RESERVED1_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RESERVED1_MASK) >> MMC_INTR_RX_RESERVED1_SHIFT)
#define  MMC_INTR_RX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RESERVED1_MASK) | (((uint32_t)val) << MMC_INTR_RX_RESERVED1_SHIFT))

#define  MMC_INTR_RX_RXWATCHDOGERROR_MASK 0x800000
#define  MMC_INTR_RX_RXWATCHDOGERROR_SHIFT 23
#define  MMC_INTR_RX_RXWATCHDOGERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXWATCHDOGERROR_MASK) >> MMC_INTR_RX_RXWATCHDOGERROR_SHIFT)
#define  MMC_INTR_RX_RXWATCHDOGERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXWATCHDOGERROR_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXWATCHDOGERROR_SHIFT))

#define  MMC_INTR_RX_RXVLANFRAMES_GB_MASK 0x400000
#define  MMC_INTR_RX_RXVLANFRAMES_GB_SHIFT 22
#define  MMC_INTR_RX_RXVLANFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXVLANFRAMES_GB_MASK) >> MMC_INTR_RX_RXVLANFRAMES_GB_SHIFT)
#define  MMC_INTR_RX_RXVLANFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXVLANFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXVLANFRAMES_GB_SHIFT))

#define  MMC_INTR_RX_RXFIFOOVERFLOW_MASK 0x200000
#define  MMC_INTR_RX_RXFIFOOVERFLOW_SHIFT 21
#define  MMC_INTR_RX_RXFIFOOVERFLOW_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXFIFOOVERFLOW_MASK) >> MMC_INTR_RX_RXFIFOOVERFLOW_SHIFT)
#define  MMC_INTR_RX_RXFIFOOVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXFIFOOVERFLOW_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXFIFOOVERFLOW_SHIFT))

#define  MMC_INTR_RX_RXPAUSEFRAMES_MASK 0x100000
#define  MMC_INTR_RX_RXPAUSEFRAMES_SHIFT 20
#define  MMC_INTR_RX_RXPAUSEFRAMES_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXPAUSEFRAMES_MASK) >> MMC_INTR_RX_RXPAUSEFRAMES_SHIFT)
#define  MMC_INTR_RX_RXPAUSEFRAMES_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXPAUSEFRAMES_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXPAUSEFRAMES_SHIFT))

#define  MMC_INTR_RX_RXOUTOFRANGETYPE_MASK 0x80000
#define  MMC_INTR_RX_RXOUTOFRANGETYPE_SHIFT 19
#define  MMC_INTR_RX_RXOUTOFRANGETYPE_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXOUTOFRANGETYPE_MASK) >> MMC_INTR_RX_RXOUTOFRANGETYPE_SHIFT)
#define  MMC_INTR_RX_RXOUTOFRANGETYPE_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXOUTOFRANGETYPE_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXOUTOFRANGETYPE_SHIFT))

#define  MMC_INTR_RX_RXLENGTHERROR_MASK 0x40000
#define  MMC_INTR_RX_RXLENGTHERROR_SHIFT 18
#define  MMC_INTR_RX_RXLENGTHERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXLENGTHERROR_MASK) >> MMC_INTR_RX_RXLENGTHERROR_SHIFT)
#define  MMC_INTR_RX_RXLENGTHERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXLENGTHERROR_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXLENGTHERROR_SHIFT))

#define  MMC_INTR_RX_RXUNICASTFRAMES_G_MASK 0x20000
#define  MMC_INTR_RX_RXUNICASTFRAMES_G_SHIFT 17
#define  MMC_INTR_RX_RXUNICASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXUNICASTFRAMES_G_MASK) >> MMC_INTR_RX_RXUNICASTFRAMES_G_SHIFT)
#define  MMC_INTR_RX_RXUNICASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXUNICASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXUNICASTFRAMES_G_SHIFT))

#define  MMC_INTR_RX_RX1024TOMAXOCTETS_GB_MASK 0x10000
#define  MMC_INTR_RX_RX1024TOMAXOCTETS_GB_SHIFT 16
#define  MMC_INTR_RX_RX1024TOMAXOCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RX1024TOMAXOCTETS_GB_MASK) >> MMC_INTR_RX_RX1024TOMAXOCTETS_GB_SHIFT)
#define  MMC_INTR_RX_RX1024TOMAXOCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RX1024TOMAXOCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RX1024TOMAXOCTETS_GB_SHIFT))

#define  MMC_INTR_RX_RX512TO1023OCTETS_GB_MASK 0x8000
#define  MMC_INTR_RX_RX512TO1023OCTETS_GB_SHIFT 15
#define  MMC_INTR_RX_RX512TO1023OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RX512TO1023OCTETS_GB_MASK) >> MMC_INTR_RX_RX512TO1023OCTETS_GB_SHIFT)
#define  MMC_INTR_RX_RX512TO1023OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RX512TO1023OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RX512TO1023OCTETS_GB_SHIFT))

#define  MMC_INTR_RX_RX256TO511OCTETS_GB_MASK 0x4000
#define  MMC_INTR_RX_RX256TO511OCTETS_GB_SHIFT 14
#define  MMC_INTR_RX_RX256TO511OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RX256TO511OCTETS_GB_MASK) >> MMC_INTR_RX_RX256TO511OCTETS_GB_SHIFT)
#define  MMC_INTR_RX_RX256TO511OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RX256TO511OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RX256TO511OCTETS_GB_SHIFT))

#define  MMC_INTR_RX_RX128TO255OCTETS_GB_MASK 0x2000
#define  MMC_INTR_RX_RX128TO255OCTETS_GB_SHIFT 13
#define  MMC_INTR_RX_RX128TO255OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RX128TO255OCTETS_GB_MASK) >> MMC_INTR_RX_RX128TO255OCTETS_GB_SHIFT)
#define  MMC_INTR_RX_RX128TO255OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RX128TO255OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RX128TO255OCTETS_GB_SHIFT))

#define  MMC_INTR_RX_RX65TO127OCTETS_GB_MASK 0x1000
#define  MMC_INTR_RX_RX65TO127OCTETS_GB_SHIFT 12
#define  MMC_INTR_RX_RX65TO127OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RX65TO127OCTETS_GB_MASK) >> MMC_INTR_RX_RX65TO127OCTETS_GB_SHIFT)
#define  MMC_INTR_RX_RX65TO127OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RX65TO127OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RX65TO127OCTETS_GB_SHIFT))

#define  MMC_INTR_RX_RX64OCTETS_GB_MASK 0x800
#define  MMC_INTR_RX_RX64OCTETS_GB_SHIFT 11
#define  MMC_INTR_RX_RX64OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RX64OCTETS_GB_MASK) >> MMC_INTR_RX_RX64OCTETS_GB_SHIFT)
#define  MMC_INTR_RX_RX64OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RX64OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RX64OCTETS_GB_SHIFT))

#define  MMC_INTR_RX_RXOVERSIZE_G_MASK 0x400
#define  MMC_INTR_RX_RXOVERSIZE_G_SHIFT 10
#define  MMC_INTR_RX_RXOVERSIZE_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXOVERSIZE_G_MASK) >> MMC_INTR_RX_RXOVERSIZE_G_SHIFT)
#define  MMC_INTR_RX_RXOVERSIZE_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXOVERSIZE_G_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXOVERSIZE_G_SHIFT))

#define  MMC_INTR_RX_RXUNDERSIZE_G_MASK 0x200
#define  MMC_INTR_RX_RXUNDERSIZE_G_SHIFT 9
#define  MMC_INTR_RX_RXUNDERSIZE_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXUNDERSIZE_G_MASK) >> MMC_INTR_RX_RXUNDERSIZE_G_SHIFT)
#define  MMC_INTR_RX_RXUNDERSIZE_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXUNDERSIZE_G_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXUNDERSIZE_G_SHIFT))

#define  MMC_INTR_RX_RXJABBERERROR_MASK 0x100
#define  MMC_INTR_RX_RXJABBERERROR_SHIFT 8
#define  MMC_INTR_RX_RXJABBERERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXJABBERERROR_MASK) >> MMC_INTR_RX_RXJABBERERROR_SHIFT)
#define  MMC_INTR_RX_RXJABBERERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXJABBERERROR_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXJABBERERROR_SHIFT))

#define  MMC_INTR_RX_RXRUNTERROR_MASK 0x80
#define  MMC_INTR_RX_RXRUNTERROR_SHIFT 7
#define  MMC_INTR_RX_RXRUNTERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXRUNTERROR_MASK) >> MMC_INTR_RX_RXRUNTERROR_SHIFT)
#define  MMC_INTR_RX_RXRUNTERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXRUNTERROR_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXRUNTERROR_SHIFT))

#define  MMC_INTR_RX_RXALIGNMENTERROR_MASK 0x40
#define  MMC_INTR_RX_RXALIGNMENTERROR_SHIFT 6
#define  MMC_INTR_RX_RXALIGNMENTERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXALIGNMENTERROR_MASK) >> MMC_INTR_RX_RXALIGNMENTERROR_SHIFT)
#define  MMC_INTR_RX_RXALIGNMENTERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXALIGNMENTERROR_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXALIGNMENTERROR_SHIFT))

#define  MMC_INTR_RX_RXCRCERROR_MASK 0x20
#define  MMC_INTR_RX_RXCRCERROR_SHIFT 5
#define  MMC_INTR_RX_RXCRCERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXCRCERROR_MASK) >> MMC_INTR_RX_RXCRCERROR_SHIFT)
#define  MMC_INTR_RX_RXCRCERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXCRCERROR_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXCRCERROR_SHIFT))

#define  MMC_INTR_RX_RXMULTICASTFRAMES_G_MASK 0x10
#define  MMC_INTR_RX_RXMULTICASTFRAMES_G_SHIFT 4
#define  MMC_INTR_RX_RXMULTICASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXMULTICASTFRAMES_G_MASK) >> MMC_INTR_RX_RXMULTICASTFRAMES_G_SHIFT)
#define  MMC_INTR_RX_RXMULTICASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXMULTICASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXMULTICASTFRAMES_G_SHIFT))

#define  MMC_INTR_RX_RXBROADCASTFRAMES_G_MASK 0x8
#define  MMC_INTR_RX_RXBROADCASTFRAMES_G_SHIFT 3
#define  MMC_INTR_RX_RXBROADCASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXBROADCASTFRAMES_G_MASK) >> MMC_INTR_RX_RXBROADCASTFRAMES_G_SHIFT)
#define  MMC_INTR_RX_RXBROADCASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXBROADCASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXBROADCASTFRAMES_G_SHIFT))

#define  MMC_INTR_RX_RXOCTETCOUNT_G_MASK 0x4
#define  MMC_INTR_RX_RXOCTETCOUNT_G_SHIFT 2
#define  MMC_INTR_RX_RXOCTETCOUNT_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXOCTETCOUNT_G_MASK) >> MMC_INTR_RX_RXOCTETCOUNT_G_SHIFT)
#define  MMC_INTR_RX_RXOCTETCOUNT_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXOCTETCOUNT_G_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXOCTETCOUNT_G_SHIFT))

#define  MMC_INTR_RX_RXOCTETCOUNT_GB_MASK 0x2
#define  MMC_INTR_RX_RXOCTETCOUNT_GB_SHIFT 1
#define  MMC_INTR_RX_RXOCTETCOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXOCTETCOUNT_GB_MASK) >> MMC_INTR_RX_RXOCTETCOUNT_GB_SHIFT)
#define  MMC_INTR_RX_RXOCTETCOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXOCTETCOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXOCTETCOUNT_GB_SHIFT))

#define  MMC_INTR_RX_RXFRAMECOUNT_GB_MASK 0x1
#define  MMC_INTR_RX_RXFRAMECOUNT_GB_SHIFT 0
#define  MMC_INTR_RX_RXFRAMECOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_RX_RXFRAMECOUNT_GB_MASK) >> MMC_INTR_RX_RXFRAMECOUNT_GB_SHIFT)
#define  MMC_INTR_RX_RXFRAMECOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_RX_RXFRAMECOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_RX_RXFRAMECOUNT_GB_SHIFT))

//====================================================================
//Register: MMC Transmit Interrupt Register (intr_tx)
/** \brief Maintains the interrupt generated from all of the transmit statisic counters.
The MMC Transmit Interrupt register maintains the interrupts generated when transmit statistic counters reach half their maximum values. (MSB of the counter is set.) It is a 32-bit wide register. An interrupt bit is cleared when the respective MMC counter that caused the interrupt is read. The least significant byte lane (bits[7:0]) of the respective counter must be read in order to clear the interrupt bit.
Note R_SS_RC means that this register bit is set internally and is cleared when the Counter register is read.*/
//====================================================================

#define  MMC_INTR_TX_RESERVED1_MASK 0xfe000000
#define  MMC_INTR_TX_RESERVED1_SHIFT 25
#define  MMC_INTR_TX_RESERVED1_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_RESERVED1_MASK) >> MMC_INTR_TX_RESERVED1_SHIFT)
#define  MMC_INTR_TX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_RESERVED1_MASK) | (((uint32_t)val) << MMC_INTR_TX_RESERVED1_SHIFT))

#define  MMC_INTR_TX_TXVLANFRAMES_G_MASK 0x1000000
#define  MMC_INTR_TX_TXVLANFRAMES_G_SHIFT 24
#define  MMC_INTR_TX_TXVLANFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXVLANFRAMES_G_MASK) >> MMC_INTR_TX_TXVLANFRAMES_G_SHIFT)
#define  MMC_INTR_TX_TXVLANFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXVLANFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXVLANFRAMES_G_SHIFT))

#define  MMC_INTR_TX_TXPAUSEFRAMES_MASK 0x800000
#define  MMC_INTR_TX_TXPAUSEFRAMES_SHIFT 23
#define  MMC_INTR_TX_TXPAUSEFRAMES_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXPAUSEFRAMES_MASK) >> MMC_INTR_TX_TXPAUSEFRAMES_SHIFT)
#define  MMC_INTR_TX_TXPAUSEFRAMES_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXPAUSEFRAMES_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXPAUSEFRAMES_SHIFT))

#define  MMC_INTR_TX_TXEXCESSDEF_MASK 0x400000
#define  MMC_INTR_TX_TXEXCESSDEF_SHIFT 22
#define  MMC_INTR_TX_TXEXCESSDEF_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXEXCESSDEF_MASK) >> MMC_INTR_TX_TXEXCESSDEF_SHIFT)
#define  MMC_INTR_TX_TXEXCESSDEF_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXEXCESSDEF_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXEXCESSDEF_SHIFT))

#define  MMC_INTR_TX_TXFRAMECOUNT_G_MASK 0x200000
#define  MMC_INTR_TX_TXFRAMECOUNT_G_SHIFT 21
#define  MMC_INTR_TX_TXFRAMECOUNT_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXFRAMECOUNT_G_MASK) >> MMC_INTR_TX_TXFRAMECOUNT_G_SHIFT)
#define  MMC_INTR_TX_TXFRAMECOUNT_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXFRAMECOUNT_G_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXFRAMECOUNT_G_SHIFT))

#define  MMC_INTR_TX_TXOCTETCOUNT_G_MASK 0x100000
#define  MMC_INTR_TX_TXOCTETCOUNT_G_SHIFT 20
#define  MMC_INTR_TX_TXOCTETCOUNT_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXOCTETCOUNT_G_MASK) >> MMC_INTR_TX_TXOCTETCOUNT_G_SHIFT)
#define  MMC_INTR_TX_TXOCTETCOUNT_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXOCTETCOUNT_G_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXOCTETCOUNT_G_SHIFT))

#define  MMC_INTR_TX_TXCARRIERERROR_MASK 0x80000
#define  MMC_INTR_TX_TXCARRIERERROR_SHIFT 19
#define  MMC_INTR_TX_TXCARRIERERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXCARRIERERROR_MASK) >> MMC_INTR_TX_TXCARRIERERROR_SHIFT)
#define  MMC_INTR_TX_TXCARRIERERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXCARRIERERROR_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXCARRIERERROR_SHIFT))

#define  MMC_INTR_TX_TXEXESSCOL_MASK 0x40000
#define  MMC_INTR_TX_TXEXESSCOL_SHIFT 18
#define  MMC_INTR_TX_TXEXESSCOL_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXEXESSCOL_MASK) >> MMC_INTR_TX_TXEXESSCOL_SHIFT)
#define  MMC_INTR_TX_TXEXESSCOL_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXEXESSCOL_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXEXESSCOL_SHIFT))

#define  MMC_INTR_TX_TXLATECOL_MASK 0x20000
#define  MMC_INTR_TX_TXLATECOL_SHIFT 17
#define  MMC_INTR_TX_TXLATECOL_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXLATECOL_MASK) >> MMC_INTR_TX_TXLATECOL_SHIFT)
#define  MMC_INTR_TX_TXLATECOL_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXLATECOL_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXLATECOL_SHIFT))

#define  MMC_INTR_TX_TXDEFERRED_MASK 0x10000
#define  MMC_INTR_TX_TXDEFERRED_SHIFT 16
#define  MMC_INTR_TX_TXDEFERRED_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXDEFERRED_MASK) >> MMC_INTR_TX_TXDEFERRED_SHIFT)
#define  MMC_INTR_TX_TXDEFERRED_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXDEFERRED_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXDEFERRED_SHIFT))

#define  MMC_INTR_TX_TXMULTICOL_G_MASK 0x8000
#define  MMC_INTR_TX_TXMULTICOL_G_SHIFT 15
#define  MMC_INTR_TX_TXMULTICOL_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXMULTICOL_G_MASK) >> MMC_INTR_TX_TXMULTICOL_G_SHIFT)
#define  MMC_INTR_TX_TXMULTICOL_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXMULTICOL_G_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXMULTICOL_G_SHIFT))

#define  MMC_INTR_TX_TXSINGLECOL_G_MASK 0x4000
#define  MMC_INTR_TX_TXSINGLECOL_G_SHIFT 14
#define  MMC_INTR_TX_TXSINGLECOL_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXSINGLECOL_G_MASK) >> MMC_INTR_TX_TXSINGLECOL_G_SHIFT)
#define  MMC_INTR_TX_TXSINGLECOL_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXSINGLECOL_G_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXSINGLECOL_G_SHIFT))

#define  MMC_INTR_TX_TXUNDERFLOWERROR_MASK 0x2000
#define  MMC_INTR_TX_TXUNDERFLOWERROR_SHIFT 13
#define  MMC_INTR_TX_TXUNDERFLOWERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXUNDERFLOWERROR_MASK) >> MMC_INTR_TX_TXUNDERFLOWERROR_SHIFT)
#define  MMC_INTR_TX_TXUNDERFLOWERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXUNDERFLOWERROR_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXUNDERFLOWERROR_SHIFT))

#define  MMC_INTR_TX_TXBROADCASTFRAMES_GB_MASK 0x1000
#define  MMC_INTR_TX_TXBROADCASTFRAMES_GB_SHIFT 12
#define  MMC_INTR_TX_TXBROADCASTFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXBROADCASTFRAMES_GB_MASK) >> MMC_INTR_TX_TXBROADCASTFRAMES_GB_SHIFT)
#define  MMC_INTR_TX_TXBROADCASTFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXBROADCASTFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXBROADCASTFRAMES_GB_SHIFT))

#define  MMC_INTR_TX_TXMULTICASTFRAMES_GB_MASK 0x800
#define  MMC_INTR_TX_TXMULTICASTFRAMES_GB_SHIFT 11
#define  MMC_INTR_TX_TXMULTICASTFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXMULTICASTFRAMES_GB_MASK) >> MMC_INTR_TX_TXMULTICASTFRAMES_GB_SHIFT)
#define  MMC_INTR_TX_TXMULTICASTFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXMULTICASTFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXMULTICASTFRAMES_GB_SHIFT))

#define  MMC_INTR_TX_TXUNICASTFRAMES_GB_MASK 0x400
#define  MMC_INTR_TX_TXUNICASTFRAMES_GB_SHIFT 10
#define  MMC_INTR_TX_TXUNICASTFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXUNICASTFRAMES_GB_MASK) >> MMC_INTR_TX_TXUNICASTFRAMES_GB_SHIFT)
#define  MMC_INTR_TX_TXUNICASTFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXUNICASTFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXUNICASTFRAMES_GB_SHIFT))

#define  MMC_INTR_TX_TX1024TOMAXOCTETS_GB_MASK 0x200
#define  MMC_INTR_TX_TX1024TOMAXOCTETS_GB_SHIFT 9
#define  MMC_INTR_TX_TX1024TOMAXOCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TX1024TOMAXOCTETS_GB_MASK) >> MMC_INTR_TX_TX1024TOMAXOCTETS_GB_SHIFT)
#define  MMC_INTR_TX_TX1024TOMAXOCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TX1024TOMAXOCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TX1024TOMAXOCTETS_GB_SHIFT))

#define  MMC_INTR_TX_TX512TO1023OCTETS_GB_MASK 0x100
#define  MMC_INTR_TX_TX512TO1023OCTETS_GB_SHIFT 8
#define  MMC_INTR_TX_TX512TO1023OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TX512TO1023OCTETS_GB_MASK) >> MMC_INTR_TX_TX512TO1023OCTETS_GB_SHIFT)
#define  MMC_INTR_TX_TX512TO1023OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TX512TO1023OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TX512TO1023OCTETS_GB_SHIFT))

#define  MMC_INTR_TX_TX256TO511OCTETS_GB_MASK 0x80
#define  MMC_INTR_TX_TX256TO511OCTETS_GB_SHIFT 7
#define  MMC_INTR_TX_TX256TO511OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TX256TO511OCTETS_GB_MASK) >> MMC_INTR_TX_TX256TO511OCTETS_GB_SHIFT)
#define  MMC_INTR_TX_TX256TO511OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TX256TO511OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TX256TO511OCTETS_GB_SHIFT))

#define  MMC_INTR_TX_TX128TO255OCTETS_GB_MASK 0x40
#define  MMC_INTR_TX_TX128TO255OCTETS_GB_SHIFT 6
#define  MMC_INTR_TX_TX128TO255OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TX128TO255OCTETS_GB_MASK) >> MMC_INTR_TX_TX128TO255OCTETS_GB_SHIFT)
#define  MMC_INTR_TX_TX128TO255OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TX128TO255OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TX128TO255OCTETS_GB_SHIFT))

#define  MMC_INTR_TX_TX65TO127OCTETS_GB_MASK 0x20
#define  MMC_INTR_TX_TX65TO127OCTETS_GB_SHIFT 5
#define  MMC_INTR_TX_TX65TO127OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TX65TO127OCTETS_GB_MASK) >> MMC_INTR_TX_TX65TO127OCTETS_GB_SHIFT)
#define  MMC_INTR_TX_TX65TO127OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TX65TO127OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TX65TO127OCTETS_GB_SHIFT))

#define  MMC_INTR_TX_TX64OCTETS_GB_MASK 0x10
#define  MMC_INTR_TX_TX64OCTETS_GB_SHIFT 4
#define  MMC_INTR_TX_TX64OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TX64OCTETS_GB_MASK) >> MMC_INTR_TX_TX64OCTETS_GB_SHIFT)
#define  MMC_INTR_TX_TX64OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TX64OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TX64OCTETS_GB_SHIFT))

#define  MMC_INTR_TX_TXMULTICASTFRAMES_G_MASK 0x8
#define  MMC_INTR_TX_TXMULTICASTFRAMES_G_SHIFT 3
#define  MMC_INTR_TX_TXMULTICASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXMULTICASTFRAMES_G_MASK) >> MMC_INTR_TX_TXMULTICASTFRAMES_G_SHIFT)
#define  MMC_INTR_TX_TXMULTICASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXMULTICASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXMULTICASTFRAMES_G_SHIFT))

#define  MMC_INTR_TX_TXBROADCASTFRAMES_G_MASK 0x4
#define  MMC_INTR_TX_TXBROADCASTFRAMES_G_SHIFT 2
#define  MMC_INTR_TX_TXBROADCASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXBROADCASTFRAMES_G_MASK) >> MMC_INTR_TX_TXBROADCASTFRAMES_G_SHIFT)
#define  MMC_INTR_TX_TXBROADCASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXBROADCASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXBROADCASTFRAMES_G_SHIFT))

#define  MMC_INTR_TX_TXFRAMECOUNT_GB_MASK 0x2
#define  MMC_INTR_TX_TXFRAMECOUNT_GB_SHIFT 1
#define  MMC_INTR_TX_TXFRAMECOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXFRAMECOUNT_GB_MASK) >> MMC_INTR_TX_TXFRAMECOUNT_GB_SHIFT)
#define  MMC_INTR_TX_TXFRAMECOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXFRAMECOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXFRAMECOUNT_GB_SHIFT))

#define  MMC_INTR_TX_TXOCTETCOUNT_GB_MASK 0x1
#define  MMC_INTR_TX_TXOCTETCOUNT_GB_SHIFT 0
#define  MMC_INTR_TX_TXOCTETCOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_TX_TXOCTETCOUNT_GB_MASK) >> MMC_INTR_TX_TXOCTETCOUNT_GB_SHIFT)
#define  MMC_INTR_TX_TXOCTETCOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_TX_TXOCTETCOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_TX_TXOCTETCOUNT_GB_SHIFT))

//====================================================================
//Register: MMC Receive Interrupt Mask Register (intr_mask_rx)
/** \brief Maintains the mask for the interrupt generated from all of the receive statisic counters.
The MMC Receive Interrupt Mask register maintains the masks for the interrupts generated when receive statistic counters reach half their maximum value. (MSB of the counter is set.) It is a 32-bit wide register.*/
//====================================================================

#define  MMC_INTR_MASK_RX_RESERVED1_MASK 0xff000000
#define  MMC_INTR_MASK_RX_RESERVED1_SHIFT 24
#define  MMC_INTR_MASK_RX_RESERVED1_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RESERVED1_MASK) >> MMC_INTR_MASK_RX_RESERVED1_SHIFT)
#define  MMC_INTR_MASK_RX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RESERVED1_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RESERVED1_SHIFT))

#define  MMC_INTR_MASK_RX_RXWATCHDOGERROR_MASK 0x800000
#define  MMC_INTR_MASK_RX_RXWATCHDOGERROR_SHIFT 23
#define  MMC_INTR_MASK_RX_RXWATCHDOGERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXWATCHDOGERROR_MASK) >> MMC_INTR_MASK_RX_RXWATCHDOGERROR_SHIFT)
#define  MMC_INTR_MASK_RX_RXWATCHDOGERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXWATCHDOGERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXWATCHDOGERROR_SHIFT))

#define  MMC_INTR_MASK_RX_RXVLANFRAMES_GB_MASK 0x400000
#define  MMC_INTR_MASK_RX_RXVLANFRAMES_GB_SHIFT 22
#define  MMC_INTR_MASK_RX_RXVLANFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXVLANFRAMES_GB_MASK) >> MMC_INTR_MASK_RX_RXVLANFRAMES_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RXVLANFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXVLANFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXVLANFRAMES_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RXFIFOOVERFLOW_MASK 0x200000
#define  MMC_INTR_MASK_RX_RXFIFOOVERFLOW_SHIFT 21
#define  MMC_INTR_MASK_RX_RXFIFOOVERFLOW_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXFIFOOVERFLOW_MASK) >> MMC_INTR_MASK_RX_RXFIFOOVERFLOW_SHIFT)
#define  MMC_INTR_MASK_RX_RXFIFOOVERFLOW_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXFIFOOVERFLOW_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXFIFOOVERFLOW_SHIFT))

#define  MMC_INTR_MASK_RX_RXPAUSEFRAMES_MASK 0x100000
#define  MMC_INTR_MASK_RX_RXPAUSEFRAMES_SHIFT 20
#define  MMC_INTR_MASK_RX_RXPAUSEFRAMES_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXPAUSEFRAMES_MASK) >> MMC_INTR_MASK_RX_RXPAUSEFRAMES_SHIFT)
#define  MMC_INTR_MASK_RX_RXPAUSEFRAMES_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXPAUSEFRAMES_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXPAUSEFRAMES_SHIFT))

#define  MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_MASK 0x80000
#define  MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_SHIFT 19
#define  MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_MASK) >> MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_SHIFT)
#define  MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXOUTOFRANGETYPE_SHIFT))

#define  MMC_INTR_MASK_RX_RXLENGTHERROR_MASK 0x40000
#define  MMC_INTR_MASK_RX_RXLENGTHERROR_SHIFT 18
#define  MMC_INTR_MASK_RX_RXLENGTHERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXLENGTHERROR_MASK) >> MMC_INTR_MASK_RX_RXLENGTHERROR_SHIFT)
#define  MMC_INTR_MASK_RX_RXLENGTHERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXLENGTHERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXLENGTHERROR_SHIFT))

#define  MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_MASK 0x20000
#define  MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_SHIFT 17
#define  MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_MASK) >> MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_SHIFT)
#define  MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXUNICASTFRAMES_G_SHIFT))

#define  MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_MASK 0x10000
#define  MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_SHIFT 16
#define  MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_MASK) >> MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RX1024TOMAXOCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_MASK 0x8000
#define  MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_SHIFT 15
#define  MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_MASK) >> MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RX512TO1023OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_MASK 0x4000
#define  MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_SHIFT 14
#define  MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_MASK) >> MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RX256TO511OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_MASK 0x2000
#define  MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_SHIFT 13
#define  MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_MASK) >> MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RX128TO255OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_MASK 0x1000
#define  MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_SHIFT 12
#define  MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_MASK) >> MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RX65TO127OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RX64OCTETS_GB_MASK 0x800
#define  MMC_INTR_MASK_RX_RX64OCTETS_GB_SHIFT 11
#define  MMC_INTR_MASK_RX_RX64OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RX64OCTETS_GB_MASK) >> MMC_INTR_MASK_RX_RX64OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RX64OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RX64OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RX64OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RXOVERSIZE_G_MASK 0x400
#define  MMC_INTR_MASK_RX_RXOVERSIZE_G_SHIFT 10
#define  MMC_INTR_MASK_RX_RXOVERSIZE_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXOVERSIZE_G_MASK) >> MMC_INTR_MASK_RX_RXOVERSIZE_G_SHIFT)
#define  MMC_INTR_MASK_RX_RXOVERSIZE_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXOVERSIZE_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXOVERSIZE_G_SHIFT))

#define  MMC_INTR_MASK_RX_RXUNDERSIZE_G_MASK 0x200
#define  MMC_INTR_MASK_RX_RXUNDERSIZE_G_SHIFT 9
#define  MMC_INTR_MASK_RX_RXUNDERSIZE_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXUNDERSIZE_G_MASK) >> MMC_INTR_MASK_RX_RXUNDERSIZE_G_SHIFT)
#define  MMC_INTR_MASK_RX_RXUNDERSIZE_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXUNDERSIZE_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXUNDERSIZE_G_SHIFT))

#define  MMC_INTR_MASK_RX_RXJABBERERROR_MASK 0x100
#define  MMC_INTR_MASK_RX_RXJABBERERROR_SHIFT 8
#define  MMC_INTR_MASK_RX_RXJABBERERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXJABBERERROR_MASK) >> MMC_INTR_MASK_RX_RXJABBERERROR_SHIFT)
#define  MMC_INTR_MASK_RX_RXJABBERERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXJABBERERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXJABBERERROR_SHIFT))

#define  MMC_INTR_MASK_RX_RXRUNTERROR_MASK 0x80
#define  MMC_INTR_MASK_RX_RXRUNTERROR_SHIFT 7
#define  MMC_INTR_MASK_RX_RXRUNTERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXRUNTERROR_MASK) >> MMC_INTR_MASK_RX_RXRUNTERROR_SHIFT)
#define  MMC_INTR_MASK_RX_RXRUNTERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXRUNTERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXRUNTERROR_SHIFT))

#define  MMC_INTR_MASK_RX_RXALIGNMENTERROR_MASK 0x40
#define  MMC_INTR_MASK_RX_RXALIGNMENTERROR_SHIFT 6
#define  MMC_INTR_MASK_RX_RXALIGNMENTERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXALIGNMENTERROR_MASK) >> MMC_INTR_MASK_RX_RXALIGNMENTERROR_SHIFT)
#define  MMC_INTR_MASK_RX_RXALIGNMENTERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXALIGNMENTERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXALIGNMENTERROR_SHIFT))

#define  MMC_INTR_MASK_RX_RXCRCERROR_MASK 0x20
#define  MMC_INTR_MASK_RX_RXCRCERROR_SHIFT 5
#define  MMC_INTR_MASK_RX_RXCRCERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXCRCERROR_MASK) >> MMC_INTR_MASK_RX_RXCRCERROR_SHIFT)
#define  MMC_INTR_MASK_RX_RXCRCERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXCRCERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXCRCERROR_SHIFT))

#define  MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_MASK 0x10
#define  MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_SHIFT 4
#define  MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_MASK) >> MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_SHIFT)
#define  MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXMULTICASTFRAMES_G_SHIFT))

#define  MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_MASK 0x8
#define  MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_SHIFT 3
#define  MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_MASK) >> MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_SHIFT)
#define  MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXBROADCASTFRAMES_G_SHIFT))

#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_G_MASK 0x4
#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_G_SHIFT 2
#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXOCTETCOUNT_G_MASK) >> MMC_INTR_MASK_RX_RXOCTETCOUNT_G_SHIFT)
#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXOCTETCOUNT_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXOCTETCOUNT_G_SHIFT))

#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_MASK 0x2
#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_SHIFT 1
#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_MASK) >> MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXOCTETCOUNT_GB_SHIFT))

#define  MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_MASK 0x1
#define  MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_SHIFT 0
#define  MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_MASK) >> MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_SHIFT)
#define  MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_RX_RXFRAMECOUNT_GB_SHIFT))

//====================================================================
//Register: MMC Transmit Interrupt Mask Register (intr_mask_tx)
/** \brief Maintains the mask for the interrupt generated from all of the transmit statisic counters.
The MMC Transmit Interrupt Mask register maintains the masks for the interrupts generated when transmit statistic counters reach half their maximum value. (MSB of the counter is set). It is a 32-bit wide register.*/
//====================================================================

#define  MMC_INTR_MASK_TX_RESERVED1_MASK 0xfe000000
#define  MMC_INTR_MASK_TX_RESERVED1_SHIFT 25
#define  MMC_INTR_MASK_TX_RESERVED1_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_RESERVED1_MASK) >> MMC_INTR_MASK_TX_RESERVED1_SHIFT)
#define  MMC_INTR_MASK_TX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_RESERVED1_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_RESERVED1_SHIFT))

#define  MMC_INTR_MASK_TX_TXVLANFRAMES_G_MASK 0x1000000
#define  MMC_INTR_MASK_TX_TXVLANFRAMES_G_SHIFT 24
#define  MMC_INTR_MASK_TX_TXVLANFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXVLANFRAMES_G_MASK) >> MMC_INTR_MASK_TX_TXVLANFRAMES_G_SHIFT)
#define  MMC_INTR_MASK_TX_TXVLANFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXVLANFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXVLANFRAMES_G_SHIFT))

#define  MMC_INTR_MASK_TX_TXPAUSEFRAMES_MASK 0x800000
#define  MMC_INTR_MASK_TX_TXPAUSEFRAMES_SHIFT 23
#define  MMC_INTR_MASK_TX_TXPAUSEFRAMES_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXPAUSEFRAMES_MASK) >> MMC_INTR_MASK_TX_TXPAUSEFRAMES_SHIFT)
#define  MMC_INTR_MASK_TX_TXPAUSEFRAMES_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXPAUSEFRAMES_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXPAUSEFRAMES_SHIFT))

#define  MMC_INTR_MASK_TX_TXEXCESSDEF_MASK 0x400000
#define  MMC_INTR_MASK_TX_TXEXCESSDEF_SHIFT 22
#define  MMC_INTR_MASK_TX_TXEXCESSDEF_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXEXCESSDEF_MASK) >> MMC_INTR_MASK_TX_TXEXCESSDEF_SHIFT)
#define  MMC_INTR_MASK_TX_TXEXCESSDEF_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXEXCESSDEF_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXEXCESSDEF_SHIFT))

#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_G_MASK 0x200000
#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_G_SHIFT 21
#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXFRAMECOUNT_G_MASK) >> MMC_INTR_MASK_TX_TXFRAMECOUNT_G_SHIFT)
#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXFRAMECOUNT_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXFRAMECOUNT_G_SHIFT))

#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_G_MASK 0x100000
#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_G_SHIFT 20
#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXOCTETCOUNT_G_MASK) >> MMC_INTR_MASK_TX_TXOCTETCOUNT_G_SHIFT)
#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXOCTETCOUNT_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXOCTETCOUNT_G_SHIFT))

#define  MMC_INTR_MASK_TX_TXCARRIERERROR_MASK 0x80000
#define  MMC_INTR_MASK_TX_TXCARRIERERROR_SHIFT 19
#define  MMC_INTR_MASK_TX_TXCARRIERERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXCARRIERERROR_MASK) >> MMC_INTR_MASK_TX_TXCARRIERERROR_SHIFT)
#define  MMC_INTR_MASK_TX_TXCARRIERERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXCARRIERERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXCARRIERERROR_SHIFT))

#define  MMC_INTR_MASK_TX_TXEXESSCOL_MASK 0x40000
#define  MMC_INTR_MASK_TX_TXEXESSCOL_SHIFT 18
#define  MMC_INTR_MASK_TX_TXEXESSCOL_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXEXESSCOL_MASK) >> MMC_INTR_MASK_TX_TXEXESSCOL_SHIFT)
#define  MMC_INTR_MASK_TX_TXEXESSCOL_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXEXESSCOL_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXEXESSCOL_SHIFT))

#define  MMC_INTR_MASK_TX_TXLATECOL_MASK 0x20000
#define  MMC_INTR_MASK_TX_TXLATECOL_SHIFT 17
#define  MMC_INTR_MASK_TX_TXLATECOL_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXLATECOL_MASK) >> MMC_INTR_MASK_TX_TXLATECOL_SHIFT)
#define  MMC_INTR_MASK_TX_TXLATECOL_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXLATECOL_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXLATECOL_SHIFT))

#define  MMC_INTR_MASK_TX_TXDEFERRED_MASK 0x10000
#define  MMC_INTR_MASK_TX_TXDEFERRED_SHIFT 16
#define  MMC_INTR_MASK_TX_TXDEFERRED_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXDEFERRED_MASK) >> MMC_INTR_MASK_TX_TXDEFERRED_SHIFT)
#define  MMC_INTR_MASK_TX_TXDEFERRED_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXDEFERRED_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXDEFERRED_SHIFT))

#define  MMC_INTR_MASK_TX_TXMULTICOL_G_MASK 0x8000
#define  MMC_INTR_MASK_TX_TXMULTICOL_G_SHIFT 15
#define  MMC_INTR_MASK_TX_TXMULTICOL_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXMULTICOL_G_MASK) >> MMC_INTR_MASK_TX_TXMULTICOL_G_SHIFT)
#define  MMC_INTR_MASK_TX_TXMULTICOL_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXMULTICOL_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXMULTICOL_G_SHIFT))

#define  MMC_INTR_MASK_TX_TXSINGLECOL_G_MASK 0x4000
#define  MMC_INTR_MASK_TX_TXSINGLECOL_G_SHIFT 14
#define  MMC_INTR_MASK_TX_TXSINGLECOL_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXSINGLECOL_G_MASK) >> MMC_INTR_MASK_TX_TXSINGLECOL_G_SHIFT)
#define  MMC_INTR_MASK_TX_TXSINGLECOL_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXSINGLECOL_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXSINGLECOL_G_SHIFT))

#define  MMC_INTR_MASK_TX_TXUNDERFLOWERROR_MASK 0x2000
#define  MMC_INTR_MASK_TX_TXUNDERFLOWERROR_SHIFT 13
#define  MMC_INTR_MASK_TX_TXUNDERFLOWERROR_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXUNDERFLOWERROR_MASK) >> MMC_INTR_MASK_TX_TXUNDERFLOWERROR_SHIFT)
#define  MMC_INTR_MASK_TX_TXUNDERFLOWERROR_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXUNDERFLOWERROR_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXUNDERFLOWERROR_SHIFT))

#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_MASK 0x1000
#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_SHIFT 12
#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_MASK) >> MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXBROADCASTFRAMES_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_MASK 0x800
#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_SHIFT 11
#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_MASK) >> MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXMULTICASTFRAMES_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_MASK 0x400
#define  MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_SHIFT 10
#define  MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_MASK) >> MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXUNICASTFRAMES_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_MASK 0x200
#define  MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_SHIFT 9
#define  MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_MASK) >> MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TX1024TOMAXOCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_MASK 0x100
#define  MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_SHIFT 8
#define  MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_MASK) >> MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TX512TO1023OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_MASK 0x80
#define  MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_SHIFT 7
#define  MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_MASK) >> MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TX256TO511OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_MASK 0x40
#define  MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_SHIFT 6
#define  MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_MASK) >> MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TX128TO255OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_MASK 0x20
#define  MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_SHIFT 5
#define  MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_MASK) >> MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TX65TO127OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TX64OCTETS_GB_MASK 0x10
#define  MMC_INTR_MASK_TX_TX64OCTETS_GB_SHIFT 4
#define  MMC_INTR_MASK_TX_TX64OCTETS_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TX64OCTETS_GB_MASK) >> MMC_INTR_MASK_TX_TX64OCTETS_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TX64OCTETS_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TX64OCTETS_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TX64OCTETS_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_MASK 0x8
#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_SHIFT 3
#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_MASK) >> MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_SHIFT)
#define  MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXMULTICASTFRAMES_G_SHIFT))

#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_MASK 0x4
#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_SHIFT 2
#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_MASK) >> MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_SHIFT)
#define  MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXBROADCASTFRAMES_G_SHIFT))

#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_MASK 0x2
#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_SHIFT 1
#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_MASK) >> MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXFRAMECOUNT_GB_SHIFT))

#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_MASK 0x1
#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_SHIFT 0
#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_MASK_SHIFT(reg) (((reg) & MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_MASK) >> MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_SHIFT)
#define  MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_REPLACE_VAL(reg,val) (((reg) & ~MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_MASK) | (((uint32_t)val) << MMC_INTR_MASK_TX_TXOCTETCOUNT_GB_SHIFT))

//====================================================================
//Register: Transmit Byte Count Register (txoctetcount_gb)
/** \brief Number of bytes transmitted, exclusive of preamble and retried bytes, in good and bad frames.*/
//====================================================================

#define  MMC_TXOCTETCOUNT_GB_VAL_MASK 0xffffffff
#define  MMC_TXOCTETCOUNT_GB_VAL_SHIFT 0
#define  MMC_TXOCTETCOUNT_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXOCTETCOUNT_GB_VAL_MASK) >> MMC_TXOCTETCOUNT_GB_VAL_SHIFT)
#define  MMC_TXOCTETCOUNT_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXOCTETCOUNT_GB_VAL_MASK) | (((uint32_t)val) << MMC_TXOCTETCOUNT_GB_VAL_SHIFT))

//====================================================================
//Register: Transmit Frame Count Register (txframecount_gb)
/** \brief Number of good and bad frames transmitted, exclusive of retried frames.*/
//====================================================================

#define  MMC_TXFRAMECOUNT_GB_VAL_MASK 0xffffffff
#define  MMC_TXFRAMECOUNT_GB_VAL_SHIFT 0
#define  MMC_TXFRAMECOUNT_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXFRAMECOUNT_GB_VAL_MASK) >> MMC_TXFRAMECOUNT_GB_VAL_SHIFT)
#define  MMC_TXFRAMECOUNT_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXFRAMECOUNT_GB_VAL_MASK) | (((uint32_t)val) << MMC_TXFRAMECOUNT_GB_VAL_SHIFT))

//====================================================================
//Register: Good Broadcast Transmit Frame Count Register (txbroadcastframes_g)
/** \brief Number of good broadcast frames transmitted.*/
//====================================================================

#define  MMC_TXBROADCASTFRAMES_G_VAL_MASK 0xffffffff
#define  MMC_TXBROADCASTFRAMES_G_VAL_SHIFT 0
#define  MMC_TXBROADCASTFRAMES_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXBROADCASTFRAMES_G_VAL_MASK) >> MMC_TXBROADCASTFRAMES_G_VAL_SHIFT)
#define  MMC_TXBROADCASTFRAMES_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXBROADCASTFRAMES_G_VAL_MASK) | (((uint32_t)val) << MMC_TXBROADCASTFRAMES_G_VAL_SHIFT))

//====================================================================
//Register: Good Multicast Transmit Frame Count Register (txmulticastframes_g)
/** \brief Number of good multicast frames transmitted.*/
//====================================================================

#define  MMC_TXMULTICASTFRAMES_G_VAL_MASK 0xffffffff
#define  MMC_TXMULTICASTFRAMES_G_VAL_SHIFT 0
#define  MMC_TXMULTICASTFRAMES_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXMULTICASTFRAMES_G_VAL_MASK) >> MMC_TXMULTICASTFRAMES_G_VAL_SHIFT)
#define  MMC_TXMULTICASTFRAMES_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXMULTICASTFRAMES_G_VAL_MASK) | (((uint32_t)val) << MMC_TXMULTICASTFRAMES_G_VAL_SHIFT))

//====================================================================
//Register: 64 byte Transmit Frame Count Register (tx64octets_gb)
/** \brief Number of good and bad frames transmitted with length.*/
//====================================================================

#define  MMC_TX64OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_TX64OCTETS_GB_VAL_SHIFT 0
#define  MMC_TX64OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TX64OCTETS_GB_VAL_MASK) >> MMC_TX64OCTETS_GB_VAL_SHIFT)
#define  MMC_TX64OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TX64OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_TX64OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 65 to 127 Byte Transmit Frame Count Register (tx65to127octets_gb)
/** \brief Number of good and bad frames transmitted with length between 65 and 127 (inclusive) bytes, exclusive of preamble and retried frames.*/
//====================================================================

#define  MMC_TX65TO127OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_TX65TO127OCTETS_GB_VAL_SHIFT 0
#define  MMC_TX65TO127OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TX65TO127OCTETS_GB_VAL_MASK) >> MMC_TX65TO127OCTETS_GB_VAL_SHIFT)
#define  MMC_TX65TO127OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TX65TO127OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_TX65TO127OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 128 to 255 Byte Transmit Frame Count Register (tx128to255octets_gb)
/** \brief Number of good and bad frames transmitted with length between 128 and 255 (inclusive) bytes, exclusive of preamble and retried frames.*/
//====================================================================

#define  MMC_TX128TO255OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_TX128TO255OCTETS_GB_VAL_SHIFT 0
#define  MMC_TX128TO255OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TX128TO255OCTETS_GB_VAL_MASK) >> MMC_TX128TO255OCTETS_GB_VAL_SHIFT)
#define  MMC_TX128TO255OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TX128TO255OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_TX128TO255OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 256 to 511 Byte Transmit Frame Count Register (tx256to511octets_gb)
/** \brief Number of good and bad frames transmitted with length between 256 and 511 (inclusive) bytes, exclusive of preamble and retried frames.*/
//====================================================================

#define  MMC_TX256TO511OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_TX256TO511OCTETS_GB_VAL_SHIFT 0
#define  MMC_TX256TO511OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TX256TO511OCTETS_GB_VAL_MASK) >> MMC_TX256TO511OCTETS_GB_VAL_SHIFT)
#define  MMC_TX256TO511OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TX256TO511OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_TX256TO511OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 512 to 1023 Byte Transmit Frame Count Register (tx512to1023octets_gb)
/** \brief Number of good and bad frames transmitted with length between 512 and 1,023 (inclusive) bytes, exclusive of preamble and retried frames.*/
//====================================================================

#define  MMC_TX512TO1023OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_TX512TO1023OCTETS_GB_VAL_SHIFT 0
#define  MMC_TX512TO1023OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TX512TO1023OCTETS_GB_VAL_MASK) >> MMC_TX512TO1023OCTETS_GB_VAL_SHIFT)
#define  MMC_TX512TO1023OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TX512TO1023OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_TX512TO1023OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 1024 to Max Byte Transmit Frame Count Register (tx1024tomaxoctets_gb)
/** \brief Number of good and bad frames transmitted with length between 1,024 and maxsize (inclusive) bytes, exclusive of preamble and retried frames.*/
//====================================================================

#define  MMC_TX1024TOMAXOCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_TX1024TOMAXOCTETS_GB_VAL_SHIFT 0
#define  MMC_TX1024TOMAXOCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TX1024TOMAXOCTETS_GB_VAL_MASK) >> MMC_TX1024TOMAXOCTETS_GB_VAL_SHIFT)
#define  MMC_TX1024TOMAXOCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TX1024TOMAXOCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_TX1024TOMAXOCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: Unicast Transmit Frame Count Register (txunicastframes_gb)
/** \brief Number of good and bad unicast frames transmitted.*/
//====================================================================

#define  MMC_TXUNICASTFRAMES_GB_VAL_MASK 0xffffffff
#define  MMC_TXUNICASTFRAMES_GB_VAL_SHIFT 0
#define  MMC_TXUNICASTFRAMES_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXUNICASTFRAMES_GB_VAL_MASK) >> MMC_TXUNICASTFRAMES_GB_VAL_SHIFT)
#define  MMC_TXUNICASTFRAMES_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXUNICASTFRAMES_GB_VAL_MASK) | (((uint32_t)val) << MMC_TXUNICASTFRAMES_GB_VAL_SHIFT))

//====================================================================
//Register: Multicast Transmit Frame Count Register (txmulticastframes_gb)
/** \brief 80 0x0140 Number of good and bad multicast frames transmitted.*/
//====================================================================

#define  MMC_TXMULTICASTFRAMES_GB_VAL_MASK 0xffffffff
#define  MMC_TXMULTICASTFRAMES_GB_VAL_SHIFT 0
#define  MMC_TXMULTICASTFRAMES_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXMULTICASTFRAMES_GB_VAL_MASK) >> MMC_TXMULTICASTFRAMES_GB_VAL_SHIFT)
#define  MMC_TXMULTICASTFRAMES_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXMULTICASTFRAMES_GB_VAL_MASK) | (((uint32_t)val) << MMC_TXMULTICASTFRAMES_GB_VAL_SHIFT))

//====================================================================
//Register: Broadcast Transmit Frame Count (txbroadcastframes_gb)
/** \brief Number of good and bad broadcast frames transmitted.*/
//====================================================================

#define  MMC_TXBROADCASTFRAMES_GB_VAL_MASK 0xffffffff
#define  MMC_TXBROADCASTFRAMES_GB_VAL_SHIFT 0
#define  MMC_TXBROADCASTFRAMES_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXBROADCASTFRAMES_GB_VAL_MASK) >> MMC_TXBROADCASTFRAMES_GB_VAL_SHIFT)
#define  MMC_TXBROADCASTFRAMES_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXBROADCASTFRAMES_GB_VAL_MASK) | (((uint32_t)val) << MMC_TXBROADCASTFRAMES_GB_VAL_SHIFT))

//====================================================================
//Register: Aborted Underflow Transmit Frame Count Register (txunderflowerror)
/** \brief Number of frames aborted due to frame underflow error.*/
//====================================================================

#define  MMC_TXUNDERFLOWERROR_VAL_MASK 0xffffffff
#define  MMC_TXUNDERFLOWERROR_VAL_SHIFT 0
#define  MMC_TXUNDERFLOWERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXUNDERFLOWERROR_VAL_MASK) >> MMC_TXUNDERFLOWERROR_VAL_SHIFT)
#define  MMC_TXUNDERFLOWERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXUNDERFLOWERROR_VAL_MASK) | (((uint32_t)val) << MMC_TXUNDERFLOWERROR_VAL_SHIFT))

//====================================================================
//Register: Single Collision Successful Transmit Frame Count Register (txsinglecol_g)
/** \brief Number of successfully transmitted frames after a single collision in Half-duplex mode.*/
//====================================================================

#define  MMC_TXSINGLECOL_G_VAL_MASK 0xffffffff
#define  MMC_TXSINGLECOL_G_VAL_SHIFT 0
#define  MMC_TXSINGLECOL_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXSINGLECOL_G_VAL_MASK) >> MMC_TXSINGLECOL_G_VAL_SHIFT)
#define  MMC_TXSINGLECOL_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXSINGLECOL_G_VAL_MASK) | (((uint32_t)val) << MMC_TXSINGLECOL_G_VAL_SHIFT))

//====================================================================
//Register: Multi-collision Successful Transmit Frame Count Register (txmulticol_g)
/** \brief Number of successfully transmitted frames after more than a single collision in Half-duplex mode.*/
//====================================================================

#define  MMC_TXMULTICOL_G_VAL_MASK 0xffffffff
#define  MMC_TXMULTICOL_G_VAL_SHIFT 0
#define  MMC_TXMULTICOL_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXMULTICOL_G_VAL_MASK) >> MMC_TXMULTICOL_G_VAL_SHIFT)
#define  MMC_TXMULTICOL_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXMULTICOL_G_VAL_MASK) | (((uint32_t)val) << MMC_TXMULTICOL_G_VAL_SHIFT))

//====================================================================
//Register: Deferred Successful Half Duplex Transmit Frame Count Register (txdeferred)
/** \brief Number of successfully transmitted frames after a deferral in Half-duplex mode.*/
//====================================================================

#define  MMC_TXDEFERRED_VAL_MASK 0xffffffff
#define  MMC_TXDEFERRED_VAL_SHIFT 0
#define  MMC_TXDEFERRED_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXDEFERRED_VAL_MASK) >> MMC_TXDEFERRED_VAL_SHIFT)
#define  MMC_TXDEFERRED_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXDEFERRED_VAL_MASK) | (((uint32_t)val) << MMC_TXDEFERRED_VAL_SHIFT))

//====================================================================
//Register: Late Collision Aborted Transmit Frame Count Register (txlatecol)
/** \brief Number of frames aborted due to late collision error.*/
//====================================================================

#define  MMC_TXLATECOL_VAL_MASK 0xffffffff
#define  MMC_TXLATECOL_VAL_SHIFT 0
#define  MMC_TXLATECOL_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXLATECOL_VAL_MASK) >> MMC_TXLATECOL_VAL_SHIFT)
#define  MMC_TXLATECOL_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXLATECOL_VAL_MASK) | (((uint32_t)val) << MMC_TXLATECOL_VAL_SHIFT))

//====================================================================
//Register: Excessive Collision Aborted Transmit Frame Count Register (txexesscol)
/** \brief Number of frames aborted due to excessive (16) collision errors.*/
//====================================================================

#define  MMC_TXEXESSCOL_VAL_MASK 0xffffffff
#define  MMC_TXEXESSCOL_VAL_SHIFT 0
#define  MMC_TXEXESSCOL_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXEXESSCOL_VAL_MASK) >> MMC_TXEXESSCOL_VAL_SHIFT)
#define  MMC_TXEXESSCOL_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXEXESSCOL_VAL_MASK) | (((uint32_t)val) << MMC_TXEXESSCOL_VAL_SHIFT))

//====================================================================
//Register: Carrier Sense Error Aborted Transmit Frame Count Register (txcarriererror)
/** \brief Number of frames aborted due to carrier sense error (no carrier or loss of carrier).*/
//====================================================================

#define  MMC_TXCARRIERERROR_VAL_MASK 0xffffffff
#define  MMC_TXCARRIERERROR_VAL_SHIFT 0
#define  MMC_TXCARRIERERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXCARRIERERROR_VAL_MASK) >> MMC_TXCARRIERERROR_VAL_SHIFT)
#define  MMC_TXCARRIERERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXCARRIERERROR_VAL_MASK) | (((uint32_t)val) << MMC_TXCARRIERERROR_VAL_SHIFT))

//====================================================================
//Register: Good Transmit Frame Byte Count Register (txoctetcount_g)
/** \brief Number of bytes transmitted, exclusive of preamble, in good frames only.*/
//====================================================================

#define  MMC_TXOCTETCOUNT_G_VAL_MASK 0xffffffff
#define  MMC_TXOCTETCOUNT_G_VAL_SHIFT 0
#define  MMC_TXOCTETCOUNT_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXOCTETCOUNT_G_VAL_MASK) >> MMC_TXOCTETCOUNT_G_VAL_SHIFT)
#define  MMC_TXOCTETCOUNT_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXOCTETCOUNT_G_VAL_MASK) | (((uint32_t)val) << MMC_TXOCTETCOUNT_G_VAL_SHIFT))

//====================================================================
//Register: Good Transmit Frame Count Register (txframecount_g)
/** \brief Number of good frames transmitted.*/
//====================================================================

#define  MMC_TXFRAMECOUNT_G_VAL_MASK 0xffffffff
#define  MMC_TXFRAMECOUNT_G_VAL_SHIFT 0
#define  MMC_TXFRAMECOUNT_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXFRAMECOUNT_G_VAL_MASK) >> MMC_TXFRAMECOUNT_G_VAL_SHIFT)
#define  MMC_TXFRAMECOUNT_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXFRAMECOUNT_G_VAL_MASK) | (((uint32_t)val) << MMC_TXFRAMECOUNT_G_VAL_SHIFT))

//====================================================================
//Register: Excessive Deferral Error Aborted Transmit Frame Count Register (txexcessdef)
/** \brief Number of frames aborted due to excessive deferral error (deferred for more than two max-sized frame times).*/
//====================================================================

#define  MMC_TXEXCESSDEF_VAL_MASK 0xffffffff
#define  MMC_TXEXCESSDEF_VAL_SHIFT 0
#define  MMC_TXEXCESSDEF_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXEXCESSDEF_VAL_MASK) >> MMC_TXEXCESSDEF_VAL_SHIFT)
#define  MMC_TXEXCESSDEF_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXEXCESSDEF_VAL_MASK) | (((uint32_t)val) << MMC_TXEXCESSDEF_VAL_SHIFT))

//====================================================================
//Register: Good PAUSE Frame Trasmit Count Register (txpauseframes)
/** \brief Number of good PAUSE frames transmitted.*/
//====================================================================

#define  MMC_TXPAUSEFRAMES_VAL_MASK 0xffffffff
#define  MMC_TXPAUSEFRAMES_VAL_SHIFT 0
#define  MMC_TXPAUSEFRAMES_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXPAUSEFRAMES_VAL_MASK) >> MMC_TXPAUSEFRAMES_VAL_SHIFT)
#define  MMC_TXPAUSEFRAMES_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXPAUSEFRAMES_VAL_MASK) | (((uint32_t)val) << MMC_TXPAUSEFRAMES_VAL_SHIFT))

//====================================================================
//Register: Good Transmit VLAN Frame Count Register (txvlanframes_g)
/** \brief Number of good VLAN frames transmitted, exclusive of retried frames.*/
//====================================================================

#define  MMC_TXVLANFRAMES_G_VAL_MASK 0xffffffff
#define  MMC_TXVLANFRAMES_G_VAL_SHIFT 0
#define  MMC_TXVLANFRAMES_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_TXVLANFRAMES_G_VAL_MASK) >> MMC_TXVLANFRAMES_G_VAL_SHIFT)
#define  MMC_TXVLANFRAMES_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_TXVLANFRAMES_G_VAL_MASK) | (((uint32_t)val) << MMC_TXVLANFRAMES_G_VAL_SHIFT))

//====================================================================
//Register: Receive Frame Count Register (rxframecount_gb)
/** \brief Number of good and bad frames received.*/
//====================================================================

#define  MMC_RXFRAMECOUNT_GB_VAL_MASK 0xffffffff
#define  MMC_RXFRAMECOUNT_GB_VAL_SHIFT 0
#define  MMC_RXFRAMECOUNT_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXFRAMECOUNT_GB_VAL_MASK) >> MMC_RXFRAMECOUNT_GB_VAL_SHIFT)
#define  MMC_RXFRAMECOUNT_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXFRAMECOUNT_GB_VAL_MASK) | (((uint32_t)val) << MMC_RXFRAMECOUNT_GB_VAL_SHIFT))

//====================================================================
//Register: Receive Byte Count Register (rxoctetcount_gb)
/** \brief Number of bytes received, exclusive of preamble, in good and bad frames.*/
//====================================================================

#define  MMC_RXOCTETCOUNT_GB_VAL_MASK 0xffffffff
#define  MMC_RXOCTETCOUNT_GB_VAL_SHIFT 0
#define  MMC_RXOCTETCOUNT_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXOCTETCOUNT_GB_VAL_MASK) >> MMC_RXOCTETCOUNT_GB_VAL_SHIFT)
#define  MMC_RXOCTETCOUNT_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXOCTETCOUNT_GB_VAL_MASK) | (((uint32_t)val) << MMC_RXOCTETCOUNT_GB_VAL_SHIFT))

//====================================================================
//Register: Good Receive Frame Count Register (rxoctetcount_g)
/** \brief Number of bytes received, exclusive of preamble, only in good frames.*/
//====================================================================

#define  MMC_RXOCTETCOUNT_G_VAL_MASK 0xffffffff
#define  MMC_RXOCTETCOUNT_G_VAL_SHIFT 0
#define  MMC_RXOCTETCOUNT_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXOCTETCOUNT_G_VAL_MASK) >> MMC_RXOCTETCOUNT_G_VAL_SHIFT)
#define  MMC_RXOCTETCOUNT_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXOCTETCOUNT_G_VAL_MASK) | (((uint32_t)val) << MMC_RXOCTETCOUNT_G_VAL_SHIFT))

//====================================================================
//Register: Good Broadcast Receive Frame Count Register (rxbroadcastframes_g)
/** \brief Number of good broadcast frames received.*/
//====================================================================

#define  MMC_RXBROADCASTFRAMES_G_VAL_MASK 0xffffffff
#define  MMC_RXBROADCASTFRAMES_G_VAL_SHIFT 0
#define  MMC_RXBROADCASTFRAMES_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXBROADCASTFRAMES_G_VAL_MASK) >> MMC_RXBROADCASTFRAMES_G_VAL_SHIFT)
#define  MMC_RXBROADCASTFRAMES_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXBROADCASTFRAMES_G_VAL_MASK) | (((uint32_t)val) << MMC_RXBROADCASTFRAMES_G_VAL_SHIFT))

//====================================================================
//Register: Good Multicast Receive Frame Count Register (rxmulticastframes_g)
/** \brief Number of good multicast frames received.*/
//====================================================================

#define  MMC_RXMULTICASTFRAMES_G_VAL_MASK 0xffffffff
#define  MMC_RXMULTICASTFRAMES_G_VAL_SHIFT 0
#define  MMC_RXMULTICASTFRAMES_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXMULTICASTFRAMES_G_VAL_MASK) >> MMC_RXMULTICASTFRAMES_G_VAL_SHIFT)
#define  MMC_RXMULTICASTFRAMES_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXMULTICASTFRAMES_G_VAL_MASK) | (((uint32_t)val) << MMC_RXMULTICASTFRAMES_G_VAL_SHIFT))

//====================================================================
//Register: CRC Error Receive Frame Count Register (rxcrcerror)
/** \brief Number of frames received with CRC error.*/
//====================================================================

#define  MMC_RXCRCERROR_VAL_MASK 0xffffffff
#define  MMC_RXCRCERROR_VAL_SHIFT 0
#define  MMC_RXCRCERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXCRCERROR_VAL_MASK) >> MMC_RXCRCERROR_VAL_SHIFT)
#define  MMC_RXCRCERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXCRCERROR_VAL_MASK) | (((uint32_t)val) << MMC_RXCRCERROR_VAL_SHIFT))

//====================================================================
//Register: Alignment Error Receive Frame Count Register (rxalignmenterror)
/** \brief Number of frames received with alignment (dribble) error. Valid only in 10/100 mode.*/
//====================================================================

#define  MMC_RXALIGNMENTERROR_VAL_MASK 0xffffffff
#define  MMC_RXALIGNMENTERROR_VAL_SHIFT 0
#define  MMC_RXALIGNMENTERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXALIGNMENTERROR_VAL_MASK) >> MMC_RXALIGNMENTERROR_VAL_SHIFT)
#define  MMC_RXALIGNMENTERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXALIGNMENTERROR_VAL_MASK) | (((uint32_t)val) << MMC_RXALIGNMENTERROR_VAL_SHIFT))

//====================================================================
//Register: Runt Error Receive Frame Count Register (rxrunterror)
/** \brief Number of frames received with runt (less than 64 bytes and CRC error) error.*/
//====================================================================

#define  MMC_RXRUNTERROR_VAL_MASK 0xffffffff
#define  MMC_RXRUNTERROR_VAL_SHIFT 0
#define  MMC_RXRUNTERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXRUNTERROR_VAL_MASK) >> MMC_RXRUNTERROR_VAL_SHIFT)
#define  MMC_RXRUNTERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXRUNTERROR_VAL_MASK) | (((uint32_t)val) << MMC_RXRUNTERROR_VAL_SHIFT))

//====================================================================
//Register: CRC Error Giant Receive Frame Count Register (rxjabbererror)
/** \brief Number of giant frames received with length (including CRC) greater than 1,518 bytes (1,522 bytes for VLAN tagged) and with CRC error. If Jumbo Frame mode is enabled, then frames of length greater than 9,018 bytes (9,022 for VLAN tagged) are considered as giant frames.*/
//====================================================================

#define  MMC_RXJABBERERROR_VAL_MASK 0xffffffff
#define  MMC_RXJABBERERROR_VAL_SHIFT 0
#define  MMC_RXJABBERERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXJABBERERROR_VAL_MASK) >> MMC_RXJABBERERROR_VAL_SHIFT)
#define  MMC_RXJABBERERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXJABBERERROR_VAL_MASK) | (((uint32_t)val) << MMC_RXJABBERERROR_VAL_SHIFT))

//====================================================================
//Register: Undersized Receive Frame Count Register (rxundersize_g)
/** \brief Number of frames received with length less than 64 bytes, without any errors.*/
//====================================================================

#define  MMC_RXUNDERSIZE_G_VAL_MASK 0xffffffff
#define  MMC_RXUNDERSIZE_G_VAL_SHIFT 0
#define  MMC_RXUNDERSIZE_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXUNDERSIZE_G_VAL_MASK) >> MMC_RXUNDERSIZE_G_VAL_SHIFT)
#define  MMC_RXUNDERSIZE_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXUNDERSIZE_G_VAL_MASK) | (((uint32_t)val) << MMC_RXUNDERSIZE_G_VAL_SHIFT))

//====================================================================
//Register: Oversized Receive Frame Count Register (rxoversize_g)
/** \brief Number of frames received with length greater than the maxsize (1,518 or 1,522 for VLAN tagged frames), without errors.*/
//====================================================================

#define  MMC_RXOVERSIZE_G_VAL_MASK 0xffffffff
#define  MMC_RXOVERSIZE_G_VAL_SHIFT 0
#define  MMC_RXOVERSIZE_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXOVERSIZE_G_VAL_MASK) >> MMC_RXOVERSIZE_G_VAL_SHIFT)
#define  MMC_RXOVERSIZE_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXOVERSIZE_G_VAL_MASK) | (((uint32_t)val) << MMC_RXOVERSIZE_G_VAL_SHIFT))

//====================================================================
//Register: 64 Byte Receive Frame Count Register (rx64octets_gb)
/** \brief Number of good and bad frames received with length 64 bytes, exclusive of preamble.*/
//====================================================================

#define  MMC_RX64OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_RX64OCTETS_GB_VAL_SHIFT 0
#define  MMC_RX64OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RX64OCTETS_GB_VAL_MASK) >> MMC_RX64OCTETS_GB_VAL_SHIFT)
#define  MMC_RX64OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RX64OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_RX64OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 65 to 127 Byte Receive Frame Count Register (rx65to127octets_gb)
/** \brief Number of good and bad frames received with length between 65 and 127 (inclusive) bytes, exclusive of preamble.*/
//====================================================================

#define  MMC_RX65TO127OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_RX65TO127OCTETS_GB_VAL_SHIFT 0
#define  MMC_RX65TO127OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RX65TO127OCTETS_GB_VAL_MASK) >> MMC_RX65TO127OCTETS_GB_VAL_SHIFT)
#define  MMC_RX65TO127OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RX65TO127OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_RX65TO127OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 128 to 255 Byte Receive Frame Count Register (rx128to255octets_gb)
/** \brief Number of good and bad frames received with length between 128 and 255 (inclusive) bytes, exclusive of preamble.*/
//====================================================================

#define  MMC_RX128TO255OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_RX128TO255OCTETS_GB_VAL_SHIFT 0
#define  MMC_RX128TO255OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RX128TO255OCTETS_GB_VAL_MASK) >> MMC_RX128TO255OCTETS_GB_VAL_SHIFT)
#define  MMC_RX128TO255OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RX128TO255OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_RX128TO255OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 256 to 511 Byte Receive Frame Count Register (rx256to511octets_gb)
/** \brief Number of good and bad frames received with length between 256 and 511 (inclusive) bytes, exclusive of preamble.*/
//====================================================================

#define  MMC_RX256TO511OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_RX256TO511OCTETS_GB_VAL_SHIFT 0
#define  MMC_RX256TO511OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RX256TO511OCTETS_GB_VAL_MASK) >> MMC_RX256TO511OCTETS_GB_VAL_SHIFT)
#define  MMC_RX256TO511OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RX256TO511OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_RX256TO511OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 512 to 1023 Byte Receive Frame Count Register (rx512to1023octets_gb)
/** \brief Number of good and bad frames received with length between 512 and 1,023 (inclusive) bytes, exclusive of preamble.*/
//====================================================================

#define  MMC_RX512TO1023OCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_RX512TO1023OCTETS_GB_VAL_SHIFT 0
#define  MMC_RX512TO1023OCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RX512TO1023OCTETS_GB_VAL_MASK) >> MMC_RX512TO1023OCTETS_GB_VAL_SHIFT)
#define  MMC_RX512TO1023OCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RX512TO1023OCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_RX512TO1023OCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: 1024 to Max Byte Receive Frame Count Register (rx1024tomaxoctets_gb)
/** \brief Number of good and bad frames received with length between 1,024 and maxsize (inclusive) bytes, exclusive of preamble and retried frames.*/
//====================================================================

#define  MMC_RX1024TOMAXOCTETS_GB_VAL_MASK 0xffffffff
#define  MMC_RX1024TOMAXOCTETS_GB_VAL_SHIFT 0
#define  MMC_RX1024TOMAXOCTETS_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RX1024TOMAXOCTETS_GB_VAL_MASK) >> MMC_RX1024TOMAXOCTETS_GB_VAL_SHIFT)
#define  MMC_RX1024TOMAXOCTETS_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RX1024TOMAXOCTETS_GB_VAL_MASK) | (((uint32_t)val) << MMC_RX1024TOMAXOCTETS_GB_VAL_SHIFT))

//====================================================================
//Register: Good Unicast Receive Frame Count Register (rxunicastframes_g)
/** \brief Number of good unicast frames received.*/
//====================================================================

#define  MMC_RXUNICASTFRAMES_G_VAL_MASK 0xffffffff
#define  MMC_RXUNICASTFRAMES_G_VAL_SHIFT 0
#define  MMC_RXUNICASTFRAMES_G_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXUNICASTFRAMES_G_VAL_MASK) >> MMC_RXUNICASTFRAMES_G_VAL_SHIFT)
#define  MMC_RXUNICASTFRAMES_G_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXUNICASTFRAMES_G_VAL_MASK) | (((uint32_t)val) << MMC_RXUNICASTFRAMES_G_VAL_SHIFT))

//====================================================================
//Register: Length Field Error Receive Frame Count Register (rxlengtherror)
/** \brief Number of frames received with length error (Length type field not equal to frame size), for all frames with valid length field.*/
//====================================================================

#define  MMC_RXLENGTHERROR_VAL_MASK 0xffffffff
#define  MMC_RXLENGTHERROR_VAL_SHIFT 0
#define  MMC_RXLENGTHERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXLENGTHERROR_VAL_MASK) >> MMC_RXLENGTHERROR_VAL_SHIFT)
#define  MMC_RXLENGTHERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXLENGTHERROR_VAL_MASK) | (((uint32_t)val) << MMC_RXLENGTHERROR_VAL_SHIFT))

//====================================================================
//Register: 1501 to 1535 Length Mismatch Receive Frame Count Register (rxoutofrangetype)
/** \brief Number of frames received with length field not equal to the valid frame size (greater than 1,500 but less than 1,536).*/
//====================================================================

#define  MMC_RXOUTOFRANGETYPE_VAL_MASK 0xffffffff
#define  MMC_RXOUTOFRANGETYPE_VAL_SHIFT 0
#define  MMC_RXOUTOFRANGETYPE_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXOUTOFRANGETYPE_VAL_MASK) >> MMC_RXOUTOFRANGETYPE_VAL_SHIFT)
#define  MMC_RXOUTOFRANGETYPE_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXOUTOFRANGETYPE_VAL_MASK) | (((uint32_t)val) << MMC_RXOUTOFRANGETYPE_VAL_SHIFT))

//====================================================================
//Register: Good Valid PAUSE Receive Frame Count Register (rxpauseframes)
/** \brief Number of good and valid PAUSE frames received.*/
//====================================================================

#define  MMC_RXPAUSEFRAMES_VAL_MASK 0xffffffff
#define  MMC_RXPAUSEFRAMES_VAL_SHIFT 0
#define  MMC_RXPAUSEFRAMES_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXPAUSEFRAMES_VAL_MASK) >> MMC_RXPAUSEFRAMES_VAL_SHIFT)
#define  MMC_RXPAUSEFRAMES_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXPAUSEFRAMES_VAL_MASK) | (((uint32_t)val) << MMC_RXPAUSEFRAMES_VAL_SHIFT))

//====================================================================
//Register: FIFO Overflow Missed Receive Frame Count Register (rxfifooverflow)
/** \brief Number of missed received frames due to FIFO overflow. This counter is not present in the MAC-CORE configuration.*/
//====================================================================

#define  MMC_RXFIFOOVERFLOW_VAL_MASK 0xffffffff
#define  MMC_RXFIFOOVERFLOW_VAL_SHIFT 0
#define  MMC_RXFIFOOVERFLOW_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXFIFOOVERFLOW_VAL_MASK) >> MMC_RXFIFOOVERFLOW_VAL_SHIFT)
#define  MMC_RXFIFOOVERFLOW_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXFIFOOVERFLOW_VAL_MASK) | (((uint32_t)val) << MMC_RXFIFOOVERFLOW_VAL_SHIFT))

//====================================================================
//Register: VLAN Receive Frame Count Register (rxvlanframes_gb)
/** \brief Number of good and bad VLAN frames received.*/
//====================================================================

#define  MMC_RXVLANFRAMES_GB_VAL_MASK 0xffffffff
#define  MMC_RXVLANFRAMES_GB_VAL_SHIFT 0
#define  MMC_RXVLANFRAMES_GB_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXVLANFRAMES_GB_VAL_MASK) >> MMC_RXVLANFRAMES_GB_VAL_SHIFT)
#define  MMC_RXVLANFRAMES_GB_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXVLANFRAMES_GB_VAL_MASK) | (((uint32_t)val) << MMC_RXVLANFRAMES_GB_VAL_SHIFT))

//====================================================================
//Register: Watchdog Timeout Error Receive Frame Count Register (rxwatchdogerror)
/** \brief Number of frames received with error due to watchdog timeout error (frames with a data load larger than 2,048 bytes).*/
//====================================================================

#define  MMC_RXWATCHDOGERROR_VAL_MASK 0xffffffff
#define  MMC_RXWATCHDOGERROR_VAL_SHIFT 0
#define  MMC_RXWATCHDOGERROR_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXWATCHDOGERROR_VAL_MASK) >> MMC_RXWATCHDOGERROR_VAL_SHIFT)
#define  MMC_RXWATCHDOGERROR_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXWATCHDOGERROR_VAL_MASK) | (((uint32_t)val) << MMC_RXWATCHDOGERROR_VAL_SHIFT))

//====================================================================
//Register: IPC Receive Checksum Offload Interrupt Mask Register (ipc_intr_mask_rx)
/** \brief MMC IPC Receive Checksum Offload Interrupt Mask maintains the mask for the interrupt generated from the receive IPC statistic counters. See Table 3-14 for further detail.
The MMC Receive Checksum Offload Interrupt Mask register maintains the masks for the interrupts generated when the receive IPC (Checksum Offload) statistic counters reach half their maximum value. (the counters MSB is set.) This register is 32 bits wide.*/
//====================================================================

#define  MMC_IPC_INTR_MASK_RX_RESERVED1_MASK 0xc0000000
#define  MMC_IPC_INTR_MASK_RX_RESERVED1_SHIFT 30
#define  MMC_IPC_INTR_MASK_RX_RESERVED1_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RESERVED1_MASK) >> MMC_IPC_INTR_MASK_RX_RESERVED1_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RESERVED1_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RESERVED1_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_MASK 0x20000000
#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_SHIFT 29
#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXICMP_ERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_MASK 0x10000000
#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_SHIFT 28
#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXICMP_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_MASK 0x8000000
#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_SHIFT 27
#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXTCP_ERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_MASK 0x4000000
#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_SHIFT 26
#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXTCP_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_MASK 0x2000000
#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_SHIFT 25
#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXUDP_ERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_MASK 0x1000000
#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_SHIFT 24
#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXUDP_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_MASK 0x800000
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_SHIFT 23
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_MASK 0x400000
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_SHIFT 22
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_MASK 0x200000
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_SHIFT 21
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV6_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_MASK 0x100000
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_SHIFT 20
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_MASK 0x80000
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_SHIFT 19
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_MASK 0x40000
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_SHIFT 18
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_MASK 0x20000
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_SHIFT 17
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_MASK 0x10000
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_SHIFT 16
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RESERVED2_MASK 0xc000
#define  MMC_IPC_INTR_MASK_RX_RESERVED2_SHIFT 14
#define  MMC_IPC_INTR_MASK_RX_RESERVED2_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RESERVED2_MASK) >> MMC_IPC_INTR_MASK_RX_RESERVED2_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RESERVED2_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RESERVED2_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_MASK 0x2000
#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_SHIFT 13
#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXICMP_ERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_MASK 0x1000
#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_SHIFT 12
#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXICMP_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_MASK 0x800
#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_SHIFT 11
#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXTCP_ERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_MASK 0x400
#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_SHIFT 10
#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXTCP_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_MASK 0x200
#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_SHIFT 9
#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXUDP_ERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_MASK 0x100
#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_SHIFT 8
#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXUDP_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_MASK 0x80
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_SHIFT 7
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV6_NOPAY_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_MASK 0x40
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_SHIFT 6
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV6_HDRERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_MASK 0x20
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_SHIFT 5
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV6_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_MASK 0x10
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_SHIFT 4
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_UDSBL_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_MASK 0x8
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_SHIFT 3
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_FRAG_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_MASK 0x4
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_SHIFT 2
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_NOPAY_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_MASK 0x2
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_SHIFT 1
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_HDRERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_MASK 0x1
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_SHIFT 0
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_MASK) >> MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_MASK_RX_RXIPV4_GD_FRMS_SHIFT))

//====================================================================
//Register: IPC Receive Checksum Offload Interrupt Register (ipc_intr_rx)
/** \brief MMC Receive Checksum Offload Interrupt maintains the interrupt that the receive IPC statistic counters generate.
The MMC Receive Checksum Offload Interrupt register maintains the interrupts generated when receive IPC statistic counters reach half their maximum values (the counters MSB is set). This register is 32 bits wide. When the MMC IPC counter that caused the interrupt is read, its corresponding interrupt bit is cleared. The counters least-significant byte lane (bits[7:0]) must be read to clear the interrupt bit.*/
//====================================================================

#define  MMC_IPC_INTR_RX_RESERVED1_MASK 0xc0000000
#define  MMC_IPC_INTR_RX_RESERVED1_SHIFT 30
#define  MMC_IPC_INTR_RX_RESERVED1_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RESERVED1_MASK) >> MMC_IPC_INTR_RX_RESERVED1_SHIFT)
#define  MMC_IPC_INTR_RX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RESERVED1_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RESERVED1_SHIFT))

#define  MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_MASK 0x20000000
#define  MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_SHIFT 29
#define  MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXICMP_ERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_MASK 0x10000000
#define  MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_SHIFT 28
#define  MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXICMP_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_MASK 0x8000000
#define  MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_SHIFT 27
#define  MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXTCP_ERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_MASK 0x4000000
#define  MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_SHIFT 26
#define  MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXTCP_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_MASK 0x2000000
#define  MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_SHIFT 25
#define  MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXUDP_ERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_MASK 0x1000000
#define  MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_SHIFT 24
#define  MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXUDP_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_MASK 0x800000
#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_SHIFT 23
#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV6_NOPAY_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_MASK 0x400000
#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_SHIFT 22
#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV6_HDRERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_MASK 0x200000
#define  MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_SHIFT 21
#define  MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV6_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_MASK 0x100000
#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_SHIFT 20
#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_UDSBL_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_MASK 0x80000
#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_SHIFT 19
#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_FRAG_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_MASK 0x40000
#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_SHIFT 18
#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_NOPAY_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_MASK 0x20000
#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_SHIFT 17
#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_HDRERR_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_MASK 0x10000
#define  MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_SHIFT 16
#define  MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_GD_OCTETS_SHIFT))

#define  MMC_IPC_INTR_RX_RESERVED2_MASK 0xc000
#define  MMC_IPC_INTR_RX_RESERVED2_SHIFT 14
#define  MMC_IPC_INTR_RX_RESERVED2_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RESERVED2_MASK) >> MMC_IPC_INTR_RX_RESERVED2_SHIFT)
#define  MMC_IPC_INTR_RX_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RESERVED2_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RESERVED2_SHIFT))

#define  MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_MASK 0x2000
#define  MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_SHIFT 13
#define  MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_MASK) >> MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXICMP_ERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXICMP_GD_FRMS_MASK 0x1000
#define  MMC_IPC_INTR_RX_RXICMP_GD_FRMS_SHIFT 12
#define  MMC_IPC_INTR_RX_RXICMP_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXICMP_GD_FRMS_MASK) >> MMC_IPC_INTR_RX_RXICMP_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXICMP_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXICMP_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXICMP_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_MASK 0x800
#define  MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_SHIFT 11
#define  MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_MASK) >> MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXTCP_ERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXTCP_GD_FRMS_MASK 0x400
#define  MMC_IPC_INTR_RX_RXTCP_GD_FRMS_SHIFT 10
#define  MMC_IPC_INTR_RX_RXTCP_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXTCP_GD_FRMS_MASK) >> MMC_IPC_INTR_RX_RXTCP_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXTCP_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXTCP_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXTCP_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_MASK 0x200
#define  MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_SHIFT 9
#define  MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_MASK) >> MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXUDP_ERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXUDP_GD_FRMS_MASK 0x100
#define  MMC_IPC_INTR_RX_RXUDP_GD_FRMS_SHIFT 8
#define  MMC_IPC_INTR_RX_RXUDP_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXUDP_GD_FRMS_MASK) >> MMC_IPC_INTR_RX_RXUDP_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXUDP_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXUDP_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXUDP_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_MASK 0x80
#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_SHIFT 7
#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV6_NOPAY_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_MASK 0x40
#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_SHIFT 6
#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV6_HDRERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_MASK 0x20
#define  MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_SHIFT 5
#define  MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV6_GD_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_MASK 0x10
#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_SHIFT 4
#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_UDSBL_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_MASK 0x8
#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_SHIFT 3
#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_FRAG_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_MASK 0x4
#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_SHIFT 2
#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_NOPAY_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_MASK 0x2
#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_SHIFT 1
#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_HDRERR_FRMS_SHIFT))

#define  MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_MASK 0x1
#define  MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_SHIFT 0
#define  MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_MASK_SHIFT(reg) (((reg) & MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_MASK) >> MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_SHIFT)
#define  MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_REPLACE_VAL(reg,val) (((reg) & ~MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_MASK) | (((uint32_t)val) << MMC_IPC_INTR_RX_RXIPV4_GD_FRMS_SHIFT))

//====================================================================
//Register: Good IPv4 Receive Datagram Count Register (rxipv4_gd_frms)
/** \brief Number of good IPv4 datagrams received with the TCP, UDP, or ICMP payload*/
//====================================================================

#define  MMC_RXIPV4_GD_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_GD_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV4_GD_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_GD_FRMS_VAL_MASK) >> MMC_RXIPV4_GD_FRMS_VAL_SHIFT)
#define  MMC_RXIPV4_GD_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_GD_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_GD_FRMS_VAL_SHIFT))

//====================================================================
//Register: Header Error IPv4 Receive Datagram Count Register (rxipv4_hdrerr_frms)
/** \brief Number of IPv4 datagrams received with header (checksum, length, or version mismatch) errors*/
//====================================================================

#define  MMC_RXIPV4_HDRERR_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_HDRERR_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV4_HDRERR_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_HDRERR_FRMS_VAL_MASK) >> MMC_RXIPV4_HDRERR_FRMS_VAL_SHIFT)
#define  MMC_RXIPV4_HDRERR_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_HDRERR_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_HDRERR_FRMS_VAL_SHIFT))

//====================================================================
//Register: Non-TCP, UDP, ICMP payload IPv4 Receive Datagram Count Register (rxipv4_nopay_frms)
/** \brief Number of good IPv4 datagrams received that did not have a TCP, UDP, or ICMP payload processed by the Checksum engine*/
//====================================================================

#define  MMC_RXIPV4_NOPAY_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_NOPAY_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV4_NOPAY_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_NOPAY_FRMS_VAL_MASK) >> MMC_RXIPV4_NOPAY_FRMS_VAL_SHIFT)
#define  MMC_RXIPV4_NOPAY_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_NOPAY_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_NOPAY_FRMS_VAL_SHIFT))

//====================================================================
//Register: Fragmented IPv4 Receive Datagram Count Register (rxipv4_frag_frms)
/** \brief Number of good IPv4 datagrams with fragmentation*/
//====================================================================

#define  MMC_RXIPV4_FRAG_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_FRAG_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV4_FRAG_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_FRAG_FRMS_VAL_MASK) >> MMC_RXIPV4_FRAG_FRMS_VAL_SHIFT)
#define  MMC_RXIPV4_FRAG_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_FRAG_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_FRAG_FRMS_VAL_SHIFT))

//====================================================================
//Register: Disabled Checksum UDP Payload IPv4 Receive Datagram Count Register (rxipv4_udsbl_frms)
/** \brief Number of good IPv4 datagrams received that had a UDP payload with checksum disabled*/
//====================================================================

#define  MMC_RXIPV4_UDSBL_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_UDSBL_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV4_UDSBL_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_UDSBL_FRMS_VAL_MASK) >> MMC_RXIPV4_UDSBL_FRMS_VAL_SHIFT)
#define  MMC_RXIPV4_UDSBL_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_UDSBL_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_UDSBL_FRMS_VAL_SHIFT))

//====================================================================
//Register: Good TCP, UDP, or ICMP payload IPv6 Receive Datagram Count Register (rxipv6_gd_frms)
/** \brief Number of good IPv6 datagrams received with TCP, UDP, or ICMP payloads*/
//====================================================================

#define  MMC_RXIPV6_GD_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV6_GD_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV6_GD_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV6_GD_FRMS_VAL_MASK) >> MMC_RXIPV6_GD_FRMS_VAL_SHIFT)
#define  MMC_RXIPV6_GD_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV6_GD_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV6_GD_FRMS_VAL_SHIFT))

//====================================================================
//Register: Header Error IPv6 Receiv Datagram Count Register (rxipv6_hdrerr_frms)
/** \brief Number of IPv6 datagrams received with header errors (length or version mismatch)*/
//====================================================================

#define  MMC_RXIPV6_HDRERR_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV6_HDRERR_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV6_HDRERR_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV6_HDRERR_FRMS_VAL_MASK) >> MMC_RXIPV6_HDRERR_FRMS_VAL_SHIFT)
#define  MMC_RXIPV6_HDRERR_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV6_HDRERR_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV6_HDRERR_FRMS_VAL_SHIFT))

//====================================================================
//Register: Non-TCP, UDP or ICMP payload IPv6 Receive Datagram Count Register (rxipv6_nopay_frms)
/** \brief Number of good IPv6 datagrams received that did not have a TCP, UDP, or ICMP payload. This includes all IPv6 datagrams with fragmentation or security extension headers*/
//====================================================================

#define  MMC_RXIPV6_NOPAY_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXIPV6_NOPAY_FRMS_VAL_SHIFT 0
#define  MMC_RXIPV6_NOPAY_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV6_NOPAY_FRMS_VAL_MASK) >> MMC_RXIPV6_NOPAY_FRMS_VAL_SHIFT)
#define  MMC_RXIPV6_NOPAY_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV6_NOPAY_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV6_NOPAY_FRMS_VAL_SHIFT))

//====================================================================
//Register: Good UDP Payload IP Receive Datagram Count Register (rxudp_gd_frms)
/** \brief Number of good IP datagrams with a good UDP payload. This counter is not updated when the rxipv4_udsbl_frms counter is incremented.*/
//====================================================================

#define  MMC_RXUDP_GD_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXUDP_GD_FRMS_VAL_SHIFT 0
#define  MMC_RXUDP_GD_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXUDP_GD_FRMS_VAL_MASK) >> MMC_RXUDP_GD_FRMS_VAL_SHIFT)
#define  MMC_RXUDP_GD_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXUDP_GD_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXUDP_GD_FRMS_VAL_SHIFT))

//====================================================================
//Register: Checksum Error UDP Payload IP Receive Datagram Count Register (rxudp_err_frms)
/** \brief Number of good IP datagrams whose UDP payload has a checksum error*/
//====================================================================

#define  MMC_RXUDP_ERR_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXUDP_ERR_FRMS_VAL_SHIFT 0
#define  MMC_RXUDP_ERR_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXUDP_ERR_FRMS_VAL_MASK) >> MMC_RXUDP_ERR_FRMS_VAL_SHIFT)
#define  MMC_RXUDP_ERR_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXUDP_ERR_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXUDP_ERR_FRMS_VAL_SHIFT))

//====================================================================
//Register: Good TCP Payload IP Receieve Datagram Count Register (rxtcp_gd_frms)
/** \brief Number of good IP datagrams with a good TCP payload*/
//====================================================================

#define  MMC_RXTCP_GD_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXTCP_GD_FRMS_VAL_SHIFT 0
#define  MMC_RXTCP_GD_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXTCP_GD_FRMS_VAL_MASK) >> MMC_RXTCP_GD_FRMS_VAL_SHIFT)
#define  MMC_RXTCP_GD_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXTCP_GD_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXTCP_GD_FRMS_VAL_SHIFT))

//====================================================================
//Register: Checksum Error TCP Payload IP Receieve Datagram Count Register (rxtcp_err_frms)
/** \brief Number of good IP datagrams whose TCP payload has a checksum error*/
//====================================================================

#define  MMC_RXTCP_ERR_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXTCP_ERR_FRMS_VAL_SHIFT 0
#define  MMC_RXTCP_ERR_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXTCP_ERR_FRMS_VAL_MASK) >> MMC_RXTCP_ERR_FRMS_VAL_SHIFT)
#define  MMC_RXTCP_ERR_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXTCP_ERR_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXTCP_ERR_FRMS_VAL_SHIFT))

//====================================================================
//Register: Good ICMP Payload IP Receive Datagram Count Register (rxicmp_gd_frms)
/** \brief Number of good IP datagrams with a good ICMP payload*/
//====================================================================

#define  MMC_RXICMP_GD_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXICMP_GD_FRMS_VAL_SHIFT 0
#define  MMC_RXICMP_GD_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXICMP_GD_FRMS_VAL_MASK) >> MMC_RXICMP_GD_FRMS_VAL_SHIFT)
#define  MMC_RXICMP_GD_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXICMP_GD_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXICMP_GD_FRMS_VAL_SHIFT))

//====================================================================
//Register: Checksum Error ICMP Payload IP Receive Datagram Count Register (rxicmp_err_frms)
/** \brief Number of good IP datagrams whose ICMP payload has a checksum error*/
//====================================================================

#define  MMC_RXICMP_ERR_FRMS_VAL_MASK 0xffffffff
#define  MMC_RXICMP_ERR_FRMS_VAL_SHIFT 0
#define  MMC_RXICMP_ERR_FRMS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXICMP_ERR_FRMS_VAL_MASK) >> MMC_RXICMP_ERR_FRMS_VAL_SHIFT)
#define  MMC_RXICMP_ERR_FRMS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXICMP_ERR_FRMS_VAL_MASK) | (((uint32_t)val) << MMC_RXICMP_ERR_FRMS_VAL_SHIFT))

//====================================================================
//Register: Good TCP, UDP or ICMP (IPv4) Data Byte Count Register (rxipv4_gd_octets)
/** \brief Number of bytes received in good IPv4 datagrams encapsulating TCP, UDP, or ICMP data. (Ethernet header, FCS, pad, or IP pad bytes are not included in this counter or in the octet counters listed below).*/
//====================================================================

#define  MMC_RXIPV4_GD_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_GD_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV4_GD_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_GD_OCTETS_VAL_MASK) >> MMC_RXIPV4_GD_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV4_GD_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_GD_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_GD_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Header Error IPv4 Receive Datagram Byte Count Register (rxipv4_hdrerr_octets)
/** \brief Number of bytes received in IPv4 datagrams with header errors (checksum, length, version mismatch). The value in the Length field of IPv4 header is used to update this counter.*/
//====================================================================

#define  MMC_RXIPV4_HDRERR_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_HDRERR_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV4_HDRERR_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_HDRERR_OCTETS_VAL_MASK) >> MMC_RXIPV4_HDRERR_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV4_HDRERR_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_HDRERR_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_HDRERR_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Non-TCP, UDP, or ICMP Payload IPv4 Receive Datagram Accumulative Length Byte Count Register (rxipv4_nopay_octets)
/** \brief Number of bytes received in IPv4 datagrams that did not have a TCP, UDP, or ICMP payload. The value in the IPv4 header's Length field is used to update this counter.*/
//====================================================================

#define  MMC_RXIPV4_NOPAY_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_NOPAY_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV4_NOPAY_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_NOPAY_OCTETS_VAL_MASK) >> MMC_RXIPV4_NOPAY_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV4_NOPAY_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_NOPAY_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_NOPAY_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Fragmented IPv4 Receive Datagram Accumulative Length Byte Count Register (rxipv4_frag_octets)
/** \brief Number of bytes received in fragmented IPv4 datagrams. The value in the IPv4 header's Length field is used to update this counter.*/
//====================================================================

#define  MMC_RXIPV4_FRAG_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_FRAG_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV4_FRAG_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_FRAG_OCTETS_VAL_MASK) >> MMC_RXIPV4_FRAG_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV4_FRAG_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_FRAG_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_FRAG_OCTETS_VAL_SHIFT))

//====================================================================
//Register: UDP Checksum Disable Receive Byte Count Register (rxipv4_udsbl_octets)
/** \brief Number of bytes received in a UDP segment that had the UDP checksum disabled. This counter does not count IP Header bytes.*/
//====================================================================

#define  MMC_RXIPV4_UDSBL_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV4_UDSBL_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV4_UDSBL_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV4_UDSBL_OCTETS_VAL_MASK) >> MMC_RXIPV4_UDSBL_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV4_UDSBL_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV4_UDSBL_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV4_UDSBL_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Good TCP, UDP or ICMPv6 IPv6 Receive Datagram Byte Count Register (rxipv6_gd_octets)
/** \brief Number of bytes received in good IPv6 datagrams encapsulating TCP, UDP or ICMPv6 data*/
//====================================================================

#define  MMC_RXIPV6_GD_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV6_GD_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV6_GD_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV6_GD_OCTETS_VAL_MASK) >> MMC_RXIPV6_GD_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV6_GD_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV6_GD_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV6_GD_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Header Error IPv6 Receive Datagram Byte Count Register (rxipv6_hdrerr_octets)
/** \brief Number of bytes received in IPv6 datagrams with header errors (length, version mismatch). The value in the IPv6 header's Length field is used to update this counter.*/
//====================================================================

#define  MMC_RXIPV6_HDRERR_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV6_HDRERR_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV6_HDRERR_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV6_HDRERR_OCTETS_VAL_MASK) >> MMC_RXIPV6_HDRERR_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV6_HDRERR_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV6_HDRERR_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV6_HDRERR_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Non-TCP, UDP or ICMP Payload IPv6 Receive Datagram Accumulative Length Count Register (rxipv6_nopay_octets)
/** \brief Number of bytes received in IPv6 datagrams that did not have a TCP, UDP, or ICMP payload. The value in the IPv6 header's Length field is used to update this counter.*/
//====================================================================

#define  MMC_RXIPV6_NOPAY_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXIPV6_NOPAY_OCTETS_VAL_SHIFT 0
#define  MMC_RXIPV6_NOPAY_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXIPV6_NOPAY_OCTETS_VAL_MASK) >> MMC_RXIPV6_NOPAY_OCTETS_VAL_SHIFT)
#define  MMC_RXIPV6_NOPAY_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXIPV6_NOPAY_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXIPV6_NOPAY_OCTETS_VAL_SHIFT))

//====================================================================
//Register: UDP Receive Byte Count Register (rxudp_gd_octets)
/** \brief Number of bytes received in a good UDP segment. This counter (and the counters below) does not count IP header bytes.*/
//====================================================================

#define  MMC_RXUDP_GD_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXUDP_GD_OCTETS_VAL_SHIFT 0
#define  MMC_RXUDP_GD_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXUDP_GD_OCTETS_VAL_MASK) >> MMC_RXUDP_GD_OCTETS_VAL_SHIFT)
#define  MMC_RXUDP_GD_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXUDP_GD_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXUDP_GD_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Checksum Error UDP Receive Byte Count Register (rxudp_err_octets)
/** \brief Number of bytes received in a UDP segment that had checksum errors*/
//====================================================================

#define  MMC_RXUDP_ERR_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXUDP_ERR_OCTETS_VAL_SHIFT 0
#define  MMC_RXUDP_ERR_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXUDP_ERR_OCTETS_VAL_MASK) >> MMC_RXUDP_ERR_OCTETS_VAL_SHIFT)
#define  MMC_RXUDP_ERR_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXUDP_ERR_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXUDP_ERR_OCTETS_VAL_SHIFT))

//====================================================================
//Register: TCP Receive Byte Count Register (rxtcp_gd_octets)
/** \brief Number of bytes received in a good TCP segment*/
//====================================================================

#define  MMC_RXTCP_GD_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXTCP_GD_OCTETS_VAL_SHIFT 0
#define  MMC_RXTCP_GD_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXTCP_GD_OCTETS_VAL_MASK) >> MMC_RXTCP_GD_OCTETS_VAL_SHIFT)
#define  MMC_RXTCP_GD_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXTCP_GD_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXTCP_GD_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Checksum Error TCP Receive Byte Count Register (rxtcp_err_octets)
/** \brief Number of bytes received in a TCP segment with checksum errors*/
//====================================================================

#define  MMC_RXTCP_ERR_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXTCP_ERR_OCTETS_VAL_SHIFT 0
#define  MMC_RXTCP_ERR_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXTCP_ERR_OCTETS_VAL_MASK) >> MMC_RXTCP_ERR_OCTETS_VAL_SHIFT)
#define  MMC_RXTCP_ERR_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXTCP_ERR_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXTCP_ERR_OCTETS_VAL_SHIFT))

//====================================================================
//Register: ICMP Receive Byte Count Register (rxicmp_gd_octets)
/** \brief Number of bytes received in a good ICMP segment*/
//====================================================================

#define  MMC_RXICMP_GD_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXICMP_GD_OCTETS_VAL_SHIFT 0
#define  MMC_RXICMP_GD_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXICMP_GD_OCTETS_VAL_MASK) >> MMC_RXICMP_GD_OCTETS_VAL_SHIFT)
#define  MMC_RXICMP_GD_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXICMP_GD_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXICMP_GD_OCTETS_VAL_SHIFT))

//====================================================================
//Register: Checksum Error ICMP Receive Byte Count Register (rxicmp_err_octets)
/** \brief Number of bytes received in an ICMP segment with checksum errors*/
//====================================================================

#define  MMC_RXICMP_ERR_OCTETS_VAL_MASK 0xffffffff
#define  MMC_RXICMP_ERR_OCTETS_VAL_SHIFT 0
#define  MMC_RXICMP_ERR_OCTETS_VAL_MASK_SHIFT(reg) (((reg) & MMC_RXICMP_ERR_OCTETS_VAL_MASK) >> MMC_RXICMP_ERR_OCTETS_VAL_SHIFT)
#define  MMC_RXICMP_ERR_OCTETS_VAL_REPLACE_VAL(reg,val) (((reg) & ~MMC_RXICMP_ERR_OCTETS_VAL_MASK) | (((uint32_t)val) << MMC_RXICMP_ERR_OCTETS_VAL_SHIFT))

//====================================================================
//
//Register File: LAN DWIP MAC 16 plus Registers (MAC_16plus)
/** \brief This section defines the bits for additional MAC Address registers beyond 16.*/
//
//====================================================================

//====================================================================
//Register: MAC Address High Register (Instance 1 of 8) (AddrHi_MAC16)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 1 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC16_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC16_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC16_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC16_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC16_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC16_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC16_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC16_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC16_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC16_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC16_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC16_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC16_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC16_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC16_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC16_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC16_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC16_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC16_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC16_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC16_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC16_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC16_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC16_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC16_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC16_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC16_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC16_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC16_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC16_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC16_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC16_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC16_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC16_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC16_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC16_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC16_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC16_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC16_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (Instance 2 of 8) (AddrHi_MAC17)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 2 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC17_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC17_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC17_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC17_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC17_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC17_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC17_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC17_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC17_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC17_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC17_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC17_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC17_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC17_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC17_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC17_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC17_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC17_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC17_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC17_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC17_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC17_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC17_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC17_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC17_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC17_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC17_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC17_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC17_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC17_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC17_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC17_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC17_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC17_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC17_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC17_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC17_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC17_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC17_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (Instance 3 of 8) (AddrHi_MAC18)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 3 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC18_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC18_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC18_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC18_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC18_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC18_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC18_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC18_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC18_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC18_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC18_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC18_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC18_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC18_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC18_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC18_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC18_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC18_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC18_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC18_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC18_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC18_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC18_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC18_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC18_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC18_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC18_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC18_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC18_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC18_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC18_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC18_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC18_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC18_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC18_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC18_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC18_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC18_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC18_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (Instance 4 of 8) (AddrHi_MAC19)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 4 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC19_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC19_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC19_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC19_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC19_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC19_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC19_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC19_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC19_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC19_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC19_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC19_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC19_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC19_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC19_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC19_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC19_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC19_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC19_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC19_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC19_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC19_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC19_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC19_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC19_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC19_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC19_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC19_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC19_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC19_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC19_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC19_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC19_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC19_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC19_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC19_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC19_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC19_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC19_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (Instance 5 of 8) (AddrHi_MAC20)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 5 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC20_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC20_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC20_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC20_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC20_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC20_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC20_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC20_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC20_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC20_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC20_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC20_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC20_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC20_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC20_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC20_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC20_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC20_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC20_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC20_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC20_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC20_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC20_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC20_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC20_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC20_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC20_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC20_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC20_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC20_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC20_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC20_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC20_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC20_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC20_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC20_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC20_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC20_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC20_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (Instance 6 of 8) (AddrHi_MAC21)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 6 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC21_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC21_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC21_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC21_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC21_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC21_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC21_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC21_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC21_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC21_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC21_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC21_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC21_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC21_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC21_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC21_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC21_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC21_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC21_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC21_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC21_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC21_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC21_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC21_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC21_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC21_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC21_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC21_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC21_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC21_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC21_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC21_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC21_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC21_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC21_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC21_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC21_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC21_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC21_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (Instance 7 of 8) (AddrHi_MAC22)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 7 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC22_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC22_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC22_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC22_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC22_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC22_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC22_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC22_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC22_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC22_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC22_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC22_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC22_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC22_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC22_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC22_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC22_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC22_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC22_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC22_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC22_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC22_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC22_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC22_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC22_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC22_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC22_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC22_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC22_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC22_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC22_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC22_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC22_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC22_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC22_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC22_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC22_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC22_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC22_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address High Register (Instance 8 of 8) (AddrHi_MAC23)
/** \brief Contains the higher 16 bits of the MAC address.
The MAC Address High register holds the upper 16 bits of the 6-byte first MAC address of the station. Note that the first DA byte that is received on the (G)MII interface corresponds to the LS Byte (Bits [7:0]) of the MAC Address Low register. For example, if 0x112233445566 is received (0x11 is the first byte) on the (G)MII as the destination address, then the MacAddress Register [47:0] is compared with 0x665544332211.
If the MAC address registers are configured to be double-synchronized to the (G)MII clock domains, then the synchronization is triggered only when Bits[31:24] (in Little-Endian mode) or Bits[7:0] (in Big- Endian mode) of the MAC Address Low Register (next register) are written to. Please note that consecutive writes to this Address Low Register should be performed only after at least 4 clock cycles in the destination clock domain for proper synchronization updates. (Instance 8 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRHI_MAC23_AE_MASK 0x80000000
#define  MAC_16PLUS_ADDRHI_MAC23_AE_SHIFT 31
#define  MAC_16PLUS_ADDRHI_MAC23_AE_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC23_AE_MASK) >> MAC_16PLUS_ADDRHI_MAC23_AE_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC23_AE_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC23_AE_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC23_AE_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC23_SA_MASK 0x40000000
#define  MAC_16PLUS_ADDRHI_MAC23_SA_SHIFT 30
#define  MAC_16PLUS_ADDRHI_MAC23_SA_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC23_SA_MASK) >> MAC_16PLUS_ADDRHI_MAC23_SA_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC23_SA_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC23_SA_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC23_SA_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC23_MBC_MASK 0x3f000000
#define  MAC_16PLUS_ADDRHI_MAC23_MBC_SHIFT 24
#define  MAC_16PLUS_ADDRHI_MAC23_MBC_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC23_MBC_MASK) >> MAC_16PLUS_ADDRHI_MAC23_MBC_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC23_MBC_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC23_MBC_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC23_MBC_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC23_RESERVED1_MASK 0xff0000
#define  MAC_16PLUS_ADDRHI_MAC23_RESERVED1_SHIFT 16
#define  MAC_16PLUS_ADDRHI_MAC23_RESERVED1_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC23_RESERVED1_MASK) >> MAC_16PLUS_ADDRHI_MAC23_RESERVED1_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC23_RESERVED1_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC23_RESERVED1_SHIFT))

#define  MAC_16PLUS_ADDRHI_MAC23_MA_HI_MASK 0xffff
#define  MAC_16PLUS_ADDRHI_MAC23_MA_HI_SHIFT 0
#define  MAC_16PLUS_ADDRHI_MAC23_MA_HI_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRHI_MAC23_MA_HI_MASK) >> MAC_16PLUS_ADDRHI_MAC23_MA_HI_SHIFT)
#define  MAC_16PLUS_ADDRHI_MAC23_MA_HI_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRHI_MAC23_MA_HI_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRHI_MAC23_MA_HI_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 1 of 8) (AddrLo_MAC16)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 1 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC16_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC16_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC16_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC16_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC16_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC16_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC16_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC16_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 2 of 8) (AddrLo_MAC17)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 2 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC17_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC17_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC17_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC17_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC17_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC17_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC17_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC17_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 3 of 8) (AddrLo_MAC18)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 3 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC18_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC18_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC18_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC18_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC18_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC18_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC18_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC18_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 4 of 8) (AddrLo_MAC19)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 4 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC19_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC19_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC19_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC19_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC19_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC19_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC19_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC19_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 5 of 8) (AddrLo_MAC20)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 5 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC20_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC20_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC20_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC20_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC20_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC20_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC20_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC20_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 6 of 8) (AddrLo_MAC21)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 6 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC21_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC21_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC21_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC21_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC21_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC21_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC21_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC21_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 7 of 8) (AddrLo_MAC22)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 7 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC22_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC22_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC22_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC22_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC22_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC22_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC22_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC22_MA_LO_SHIFT))

//====================================================================
//Register: MAC Address Low Register (Instance 8 of 8) (AddrLo_MAC23)
/** \brief Contains the lower 32 bits of the MAC address. (Instance 8 of 8)*/
//====================================================================

#define  MAC_16PLUS_ADDRLO_MAC23_MA_LO_MASK 0xffffffff
#define  MAC_16PLUS_ADDRLO_MAC23_MA_LO_SHIFT 0
#define  MAC_16PLUS_ADDRLO_MAC23_MA_LO_MASK_SHIFT(reg) (((reg) & MAC_16PLUS_ADDRLO_MAC23_MA_LO_MASK) >> MAC_16PLUS_ADDRLO_MAC23_MA_LO_SHIFT)
#define  MAC_16PLUS_ADDRLO_MAC23_MA_LO_REPLACE_VAL(reg,val) (((reg) & ~MAC_16PLUS_ADDRLO_MAC23_MA_LO_MASK) | (((uint32_t)val) << MAC_16PLUS_ADDRLO_MAC23_MA_LO_SHIFT))

//====================================================================
//
//Register File: LAN DWIP DMA Registers (MAD)
/** \brief This section defines the bits for each DMA register.*/
//
//====================================================================

//====================================================================
//Register: Bus Mode Register (r0)
/** \brief Controls the Host Interface Mode
      The Bus Mode register establishes the bus operating modes for the DMA.*/
//====================================================================

#define  MAD_R0_RESERVED1_MASK 0xfc000000
#define  MAD_R0_RESERVED1_SHIFT 26
#define  MAD_R0_RESERVED1_MASK_SHIFT(reg) (((reg) & MAD_R0_RESERVED1_MASK) >> MAD_R0_RESERVED1_SHIFT)
#define  MAD_R0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_RESERVED1_MASK) | (((uint32_t)val) << MAD_R0_RESERVED1_SHIFT))

#define  MAD_R0_AAL_MASK 0x2000000
#define  MAD_R0_AAL_SHIFT 25
#define  MAD_R0_AAL_MASK_SHIFT(reg) (((reg) & MAD_R0_AAL_MASK) >> MAD_R0_AAL_SHIFT)
#define  MAD_R0_AAL_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_AAL_MASK) | (((uint32_t)val) << MAD_R0_AAL_SHIFT))

#define  MAD_R0_MUL4_PBL_MASK 0x1000000
#define  MAD_R0_MUL4_PBL_SHIFT 24
#define  MAD_R0_MUL4_PBL_MASK_SHIFT(reg) (((reg) & MAD_R0_MUL4_PBL_MASK) >> MAD_R0_MUL4_PBL_SHIFT)
#define  MAD_R0_MUL4_PBL_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_MUL4_PBL_MASK) | (((uint32_t)val) << MAD_R0_MUL4_PBL_SHIFT))

#define  MAD_R0_USP_MASK 0x800000
#define  MAD_R0_USP_SHIFT 23
#define  MAD_R0_USP_MASK_SHIFT(reg) (((reg) & MAD_R0_USP_MASK) >> MAD_R0_USP_SHIFT)
#define  MAD_R0_USP_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_USP_MASK) | (((uint32_t)val) << MAD_R0_USP_SHIFT))

#define  MAD_R0_RPBL_MASK 0x7e0000
#define  MAD_R0_RPBL_SHIFT 17
#define  MAD_R0_RPBL_MASK_SHIFT(reg) (((reg) & MAD_R0_RPBL_MASK) >> MAD_R0_RPBL_SHIFT)
#define  MAD_R0_RPBL_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_RPBL_MASK) | (((uint32_t)val) << MAD_R0_RPBL_SHIFT))

#define  MAD_R0_FB_MASK 0x10000
#define  MAD_R0_FB_SHIFT 16
#define  MAD_R0_FB_MASK_SHIFT(reg) (((reg) & MAD_R0_FB_MASK) >> MAD_R0_FB_SHIFT)
#define  MAD_R0_FB_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_FB_MASK) | (((uint32_t)val) << MAD_R0_FB_SHIFT))

#define  MAD_R0_PR_MASK 0xc000
#define  MAD_R0_PR_SHIFT 14
#define  MAD_R0_PR_MASK_SHIFT(reg) (((reg) & MAD_R0_PR_MASK) >> MAD_R0_PR_SHIFT)
#define  MAD_R0_PR_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_PR_MASK) | (((uint32_t)val) << MAD_R0_PR_SHIFT))

#define  MAD_R0_PBL_MASK 0x3f00
#define  MAD_R0_PBL_SHIFT 8
#define  MAD_R0_PBL_MASK_SHIFT(reg) (((reg) & MAD_R0_PBL_MASK) >> MAD_R0_PBL_SHIFT)
#define  MAD_R0_PBL_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_PBL_MASK) | (((uint32_t)val) << MAD_R0_PBL_SHIFT))

#define  MAD_R0_RESERVED2_MASK 0x80
#define  MAD_R0_RESERVED2_SHIFT 7
#define  MAD_R0_RESERVED2_MASK_SHIFT(reg) (((reg) & MAD_R0_RESERVED2_MASK) >> MAD_R0_RESERVED2_SHIFT)
#define  MAD_R0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_RESERVED2_MASK) | (((uint32_t)val) << MAD_R0_RESERVED2_SHIFT))

#define  MAD_R0_DSL_MASK 0x7c
#define  MAD_R0_DSL_SHIFT 2
#define  MAD_R0_DSL_MASK_SHIFT(reg) (((reg) & MAD_R0_DSL_MASK) >> MAD_R0_DSL_SHIFT)
#define  MAD_R0_DSL_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_DSL_MASK) | (((uint32_t)val) << MAD_R0_DSL_SHIFT))

#define  MAD_R0_DA_MASK 0x2
#define  MAD_R0_DA_SHIFT 1
#define  MAD_R0_DA_MASK_SHIFT(reg) (((reg) & MAD_R0_DA_MASK) >> MAD_R0_DA_SHIFT)
#define  MAD_R0_DA_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_DA_MASK) | (((uint32_t)val) << MAD_R0_DA_SHIFT))

#define  MAD_R0_SWR_MASK 0x1
#define  MAD_R0_SWR_SHIFT 0
#define  MAD_R0_SWR_MASK_SHIFT(reg) (((reg) & MAD_R0_SWR_MASK) >> MAD_R0_SWR_SHIFT)
#define  MAD_R0_SWR_REPLACE_VAL(reg,val) (((reg) & ~MAD_R0_SWR_MASK) | (((uint32_t)val) << MAD_R0_SWR_SHIFT))

//====================================================================
//Register: Transmit Poll Demand Register (r1)
/** \brief Used by the host to instruct the DMA to poll the Transmit Descriptor List.
The Transmit Poll Demand register enables the Transmit DMA to check whether or not the current descriptor is owned by DMA. The Transmit Poll Demand command is given to wake up the TxDMA if it is in Suspend mode. The TxDMA can go into Suspend mode due to an Underflow error in a transmitted frame or due to the unavailability of descriptors owned by Transmit DMA. You can give this command anytime and the TxDMA will reset this command once it starts re-fetching the current descriptor from host memory.*/
//====================================================================

#define  MAD_R1_TPD_MASK 0xffffffff
#define  MAD_R1_TPD_SHIFT 0
#define  MAD_R1_TPD_MASK_SHIFT(reg) (((reg) & MAD_R1_TPD_MASK) >> MAD_R1_TPD_SHIFT)
#define  MAD_R1_TPD_REPLACE_VAL(reg,val) (((reg) & ~MAD_R1_TPD_MASK) | (((uint32_t)val) << MAD_R1_TPD_SHIFT))

//====================================================================
//Register: Recieve Poll Demand Register (r2)
/** \brief Used by the Host to instruct the DMA to poll the Receive Descriptor list.
The Receive Poll Demand register enables the receive DMA to check for new descriptors. This command is given to wake up the RxDMA from SUSPEND state. The RxDMA can go into SUSPEND state only due to the unavailability of descriptors owned by it.*/
//====================================================================

#define  MAD_R2_RPD_MASK 0xffffffff
#define  MAD_R2_RPD_SHIFT 0
#define  MAD_R2_RPD_MASK_SHIFT(reg) (((reg) & MAD_R2_RPD_MASK) >> MAD_R2_RPD_SHIFT)
#define  MAD_R2_RPD_REPLACE_VAL(reg,val) (((reg) & ~MAD_R2_RPD_MASK) | (((uint32_t)val) << MAD_R2_RPD_SHIFT))

//====================================================================
//Register: Receive Descriptor List Address Register (r3)
/** \brief Points the DMA to the start of the Receive Descriptor list.
The Receive Descriptor List Address register points to the start of the Receive Descriptor List. The descriptor lists reside in the host's physical memory space and must be Word/Dword/Lword-aligned (for 32/64/128-bit data bus). The DMA internally converts it to bus width aligned address by making the corresponding LS bits low. Writing to Register3 is permitted only when reception is stopped. When stopped, Register3 must be written to before the receive Start command is given.*/
//====================================================================

#define  MAD_R3_SORL_MASK 0xfffffffc
#define  MAD_R3_SORL_SHIFT 2
#define  MAD_R3_SORL_MASK_SHIFT(reg) (((reg) & MAD_R3_SORL_MASK) >> MAD_R3_SORL_SHIFT)
#define  MAD_R3_SORL_REPLACE_VAL(reg,val) (((reg) & ~MAD_R3_SORL_MASK) | (((uint32_t)val) << MAD_R3_SORL_SHIFT))

#define  MAD_R3_RESERVED1_MASK 0x3
#define  MAD_R3_RESERVED1_SHIFT 0
#define  MAD_R3_RESERVED1_MASK_SHIFT(reg) (((reg) & MAD_R3_RESERVED1_MASK) >> MAD_R3_RESERVED1_SHIFT)
#define  MAD_R3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAD_R3_RESERVED1_MASK) | (((uint32_t)val) << MAD_R3_RESERVED1_SHIFT))

//====================================================================
//Register: Transmit Descriptor List Address Register (r4)
/** \brief Points the DMA to the start of the Transmit Descriptor List.
The Transmit Descriptor List Address register points to the start of the Transmit Descriptor List. The descriptor lists reside in the host's physical memory space and must be Word/DWORD/LWORDaligned (for 32/64/128-bit data bus). The DMA internally converts it to bus width aligned address by making the corresponding LSB to low. Writing to Register4 is permitted only when transmission has stopped. When stopped, Register4 can be written before the transmission Start command is given.*/
//====================================================================

#define  MAD_R4_SOTL_MASK 0xfffffffc
#define  MAD_R4_SOTL_SHIFT 2
#define  MAD_R4_SOTL_MASK_SHIFT(reg) (((reg) & MAD_R4_SOTL_MASK) >> MAD_R4_SOTL_SHIFT)
#define  MAD_R4_SOTL_REPLACE_VAL(reg,val) (((reg) & ~MAD_R4_SOTL_MASK) | (((uint32_t)val) << MAD_R4_SOTL_SHIFT))

#define  MAD_R4_RESERVED1_MASK 0x3
#define  MAD_R4_RESERVED1_SHIFT 0
#define  MAD_R4_RESERVED1_MASK_SHIFT(reg) (((reg) & MAD_R4_RESERVED1_MASK) >> MAD_R4_RESERVED1_SHIFT)
#define  MAD_R4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAD_R4_RESERVED1_MASK) | (((uint32_t)val) << MAD_R4_RESERVED1_SHIFT))

//====================================================================
//Register: Status Register (r5)
/** \brief The Software driver (application) reads this register during interrupt service routine or polling to determine the status of the DMA.
The Status register contains all the status bits that the DMA reports to the host. Register5 and is usually read by the Software driver during an interrupt service routine or polling. Most of the fields in this register cause the host to be interrupted. Register5 bits are not cleared when read. Writing 1'b1 to (unreserved) bits in Register5[16:0] clears them and writing 1'b0 has no effect. Each field (bits[16:0]) can be masked by masking the appropriate bit in Register7.*/
//====================================================================

#define  MAD_R5_RESERVED1_MASK 0xe0000000
#define  MAD_R5_RESERVED1_SHIFT 29
#define  MAD_R5_RESERVED1_MASK_SHIFT(reg) (((reg) & MAD_R5_RESERVED1_MASK) >> MAD_R5_RESERVED1_SHIFT)
#define  MAD_R5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_RESERVED1_MASK) | (((uint32_t)val) << MAD_R5_RESERVED1_SHIFT))

#define  MAD_R5_GPI_MASK 0x10000000
#define  MAD_R5_GPI_SHIFT 28
#define  MAD_R5_GPI_MASK_SHIFT(reg) (((reg) & MAD_R5_GPI_MASK) >> MAD_R5_GPI_SHIFT)
#define  MAD_R5_GPI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_GPI_MASK) | (((uint32_t)val) << MAD_R5_GPI_SHIFT))

#define  MAD_R5_GMI_MASK 0x8000000
#define  MAD_R5_GMI_SHIFT 27
#define  MAD_R5_GMI_MASK_SHIFT(reg) (((reg) & MAD_R5_GMI_MASK) >> MAD_R5_GMI_SHIFT)
#define  MAD_R5_GMI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_GMI_MASK) | (((uint32_t)val) << MAD_R5_GMI_SHIFT))

#define  MAD_R5_GLI_MASK 0x4000000
#define  MAD_R5_GLI_SHIFT 26
#define  MAD_R5_GLI_MASK_SHIFT(reg) (((reg) & MAD_R5_GLI_MASK) >> MAD_R5_GLI_SHIFT)
#define  MAD_R5_GLI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_GLI_MASK) | (((uint32_t)val) << MAD_R5_GLI_SHIFT))

#define  MAD_R5_EB_MASK 0x3800000
#define  MAD_R5_EB_SHIFT 23
#define  MAD_R5_EB_MASK_SHIFT(reg) (((reg) & MAD_R5_EB_MASK) >> MAD_R5_EB_SHIFT)
#define  MAD_R5_EB_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_EB_MASK) | (((uint32_t)val) << MAD_R5_EB_SHIFT))

#define  MAD_R5_TS_MASK 0x700000
#define  MAD_R5_TS_SHIFT 20
#define  MAD_R5_TS_MASK_SHIFT(reg) (((reg) & MAD_R5_TS_MASK) >> MAD_R5_TS_SHIFT)
#define  MAD_R5_TS_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_TS_MASK) | (((uint32_t)val) << MAD_R5_TS_SHIFT))

#define  MAD_R5_RS_MASK 0xe0000
#define  MAD_R5_RS_SHIFT 17
#define  MAD_R5_RS_MASK_SHIFT(reg) (((reg) & MAD_R5_RS_MASK) >> MAD_R5_RS_SHIFT)
#define  MAD_R5_RS_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_RS_MASK) | (((uint32_t)val) << MAD_R5_RS_SHIFT))

#define  MAD_R5_NIS_MASK 0x10000
#define  MAD_R5_NIS_SHIFT 16
#define  MAD_R5_NIS_MASK_SHIFT(reg) (((reg) & MAD_R5_NIS_MASK) >> MAD_R5_NIS_SHIFT)
#define  MAD_R5_NIS_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_NIS_MASK) | (((uint32_t)val) << MAD_R5_NIS_SHIFT))

#define  MAD_R5_AIS_MASK 0x8000
#define  MAD_R5_AIS_SHIFT 15
#define  MAD_R5_AIS_MASK_SHIFT(reg) (((reg) & MAD_R5_AIS_MASK) >> MAD_R5_AIS_SHIFT)
#define  MAD_R5_AIS_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_AIS_MASK) | (((uint32_t)val) << MAD_R5_AIS_SHIFT))

#define  MAD_R5_ERI_MASK 0x4000
#define  MAD_R5_ERI_SHIFT 14
#define  MAD_R5_ERI_MASK_SHIFT(reg) (((reg) & MAD_R5_ERI_MASK) >> MAD_R5_ERI_SHIFT)
#define  MAD_R5_ERI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_ERI_MASK) | (((uint32_t)val) << MAD_R5_ERI_SHIFT))

#define  MAD_R5_FBI_MASK 0x2000
#define  MAD_R5_FBI_SHIFT 13
#define  MAD_R5_FBI_MASK_SHIFT(reg) (((reg) & MAD_R5_FBI_MASK) >> MAD_R5_FBI_SHIFT)
#define  MAD_R5_FBI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_FBI_MASK) | (((uint32_t)val) << MAD_R5_FBI_SHIFT))

#define  MAD_R5_RESERVED2_MASK 0x1800
#define  MAD_R5_RESERVED2_SHIFT 11
#define  MAD_R5_RESERVED2_MASK_SHIFT(reg) (((reg) & MAD_R5_RESERVED2_MASK) >> MAD_R5_RESERVED2_SHIFT)
#define  MAD_R5_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_RESERVED2_MASK) | (((uint32_t)val) << MAD_R5_RESERVED2_SHIFT))

#define  MAD_R5_ETI_MASK 0x400
#define  MAD_R5_ETI_SHIFT 10
#define  MAD_R5_ETI_MASK_SHIFT(reg) (((reg) & MAD_R5_ETI_MASK) >> MAD_R5_ETI_SHIFT)
#define  MAD_R5_ETI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_ETI_MASK) | (((uint32_t)val) << MAD_R5_ETI_SHIFT))

#define  MAD_R5_RWT_MASK 0x200
#define  MAD_R5_RWT_SHIFT 9
#define  MAD_R5_RWT_MASK_SHIFT(reg) (((reg) & MAD_R5_RWT_MASK) >> MAD_R5_RWT_SHIFT)
#define  MAD_R5_RWT_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_RWT_MASK) | (((uint32_t)val) << MAD_R5_RWT_SHIFT))

#define  MAD_R5_RPS_MASK 0x100
#define  MAD_R5_RPS_SHIFT 8
#define  MAD_R5_RPS_MASK_SHIFT(reg) (((reg) & MAD_R5_RPS_MASK) >> MAD_R5_RPS_SHIFT)
#define  MAD_R5_RPS_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_RPS_MASK) | (((uint32_t)val) << MAD_R5_RPS_SHIFT))

#define  MAD_R5_RU_MASK 0x80
#define  MAD_R5_RU_SHIFT 7
#define  MAD_R5_RU_MASK_SHIFT(reg) (((reg) & MAD_R5_RU_MASK) >> MAD_R5_RU_SHIFT)
#define  MAD_R5_RU_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_RU_MASK) | (((uint32_t)val) << MAD_R5_RU_SHIFT))

#define  MAD_R5_RI_MASK 0x40
#define  MAD_R5_RI_SHIFT 6
#define  MAD_R5_RI_MASK_SHIFT(reg) (((reg) & MAD_R5_RI_MASK) >> MAD_R5_RI_SHIFT)
#define  MAD_R5_RI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_RI_MASK) | (((uint32_t)val) << MAD_R5_RI_SHIFT))

#define  MAD_R5_UNF_MASK 0x20
#define  MAD_R5_UNF_SHIFT 5
#define  MAD_R5_UNF_MASK_SHIFT(reg) (((reg) & MAD_R5_UNF_MASK) >> MAD_R5_UNF_SHIFT)
#define  MAD_R5_UNF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_UNF_MASK) | (((uint32_t)val) << MAD_R5_UNF_SHIFT))

#define  MAD_R5_OVF_MASK 0x10
#define  MAD_R5_OVF_SHIFT 4
#define  MAD_R5_OVF_MASK_SHIFT(reg) (((reg) & MAD_R5_OVF_MASK) >> MAD_R5_OVF_SHIFT)
#define  MAD_R5_OVF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_OVF_MASK) | (((uint32_t)val) << MAD_R5_OVF_SHIFT))

#define  MAD_R5_TJT_MASK 0x8
#define  MAD_R5_TJT_SHIFT 3
#define  MAD_R5_TJT_MASK_SHIFT(reg) (((reg) & MAD_R5_TJT_MASK) >> MAD_R5_TJT_SHIFT)
#define  MAD_R5_TJT_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_TJT_MASK) | (((uint32_t)val) << MAD_R5_TJT_SHIFT))

#define  MAD_R5_TU_MASK 0x4
#define  MAD_R5_TU_SHIFT 2
#define  MAD_R5_TU_MASK_SHIFT(reg) (((reg) & MAD_R5_TU_MASK) >> MAD_R5_TU_SHIFT)
#define  MAD_R5_TU_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_TU_MASK) | (((uint32_t)val) << MAD_R5_TU_SHIFT))

#define  MAD_R5_TPS_MASK 0x2
#define  MAD_R5_TPS_SHIFT 1
#define  MAD_R5_TPS_MASK_SHIFT(reg) (((reg) & MAD_R5_TPS_MASK) >> MAD_R5_TPS_SHIFT)
#define  MAD_R5_TPS_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_TPS_MASK) | (((uint32_t)val) << MAD_R5_TPS_SHIFT))

#define  MAD_R5_TI_MASK 0x1
#define  MAD_R5_TI_SHIFT 0
#define  MAD_R5_TI_MASK_SHIFT(reg) (((reg) & MAD_R5_TI_MASK) >> MAD_R5_TI_SHIFT)
#define  MAD_R5_TI_REPLACE_VAL(reg,val) (((reg) & ~MAD_R5_TI_MASK) | (((uint32_t)val) << MAD_R5_TI_SHIFT))

//====================================================================
//Register: Operation Mode Register (r6)
/** \brief Establishes the Receive and Transmit operating modes and command.
The Operation Mode register establishes the Transmit and Receive operating modes and commands.
Register6 should be the last CSR to be written as part of DMA initialization. This register is also present in the MAC-MTL configuration with Bits 13, 2, and 1 unused and reserved.*/
//====================================================================

#define  MAD_R6_RESERVED1_MASK 0xf8000000
#define  MAD_R6_RESERVED1_SHIFT 27
#define  MAD_R6_RESERVED1_MASK_SHIFT(reg) (((reg) & MAD_R6_RESERVED1_MASK) >> MAD_R6_RESERVED1_SHIFT)
#define  MAD_R6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RESERVED1_MASK) | (((uint32_t)val) << MAD_R6_RESERVED1_SHIFT))

#define  MAD_R6_DT_MASK 0x4000000
#define  MAD_R6_DT_SHIFT 26
#define  MAD_R6_DT_MASK_SHIFT(reg) (((reg) & MAD_R6_DT_MASK) >> MAD_R6_DT_SHIFT)
#define  MAD_R6_DT_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_DT_MASK) | (((uint32_t)val) << MAD_R6_DT_SHIFT))

#define  MAD_R6_RSF_MASK 0x2000000
#define  MAD_R6_RSF_SHIFT 25
#define  MAD_R6_RSF_MASK_SHIFT(reg) (((reg) & MAD_R6_RSF_MASK) >> MAD_R6_RSF_SHIFT)
#define  MAD_R6_RSF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RSF_MASK) | (((uint32_t)val) << MAD_R6_RSF_SHIFT))

#define  MAD_R6_DFF_MASK 0x1000000
#define  MAD_R6_DFF_SHIFT 24
#define  MAD_R6_DFF_MASK_SHIFT(reg) (((reg) & MAD_R6_DFF_MASK) >> MAD_R6_DFF_SHIFT)
#define  MAD_R6_DFF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_DFF_MASK) | (((uint32_t)val) << MAD_R6_DFF_SHIFT))

#define  MAD_R6_RESERVED2_MASK 0xc00000
#define  MAD_R6_RESERVED2_SHIFT 22
#define  MAD_R6_RESERVED2_MASK_SHIFT(reg) (((reg) & MAD_R6_RESERVED2_MASK) >> MAD_R6_RESERVED2_SHIFT)
#define  MAD_R6_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RESERVED2_MASK) | (((uint32_t)val) << MAD_R6_RESERVED2_SHIFT))

#define  MAD_R6_TSF_MASK 0x200000
#define  MAD_R6_TSF_SHIFT 21
#define  MAD_R6_TSF_MASK_SHIFT(reg) (((reg) & MAD_R6_TSF_MASK) >> MAD_R6_TSF_SHIFT)
#define  MAD_R6_TSF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_TSF_MASK) | (((uint32_t)val) << MAD_R6_TSF_SHIFT))

#define  MAD_R6_FTF_MASK 0x100000
#define  MAD_R6_FTF_SHIFT 20
#define  MAD_R6_FTF_MASK_SHIFT(reg) (((reg) & MAD_R6_FTF_MASK) >> MAD_R6_FTF_SHIFT)
#define  MAD_R6_FTF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_FTF_MASK) | (((uint32_t)val) << MAD_R6_FTF_SHIFT))

#define  MAD_R6_RESERVED3_MASK 0xe0000
#define  MAD_R6_RESERVED3_SHIFT 17
#define  MAD_R6_RESERVED3_MASK_SHIFT(reg) (((reg) & MAD_R6_RESERVED3_MASK) >> MAD_R6_RESERVED3_SHIFT)
#define  MAD_R6_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RESERVED3_MASK) | (((uint32_t)val) << MAD_R6_RESERVED3_SHIFT))

#define  MAD_R6_TTC_MASK 0x1c000
#define  MAD_R6_TTC_SHIFT 14
#define  MAD_R6_TTC_MASK_SHIFT(reg) (((reg) & MAD_R6_TTC_MASK) >> MAD_R6_TTC_SHIFT)
#define  MAD_R6_TTC_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_TTC_MASK) | (((uint32_t)val) << MAD_R6_TTC_SHIFT))

#define  MAD_R6_ST_MASK 0x2000
#define  MAD_R6_ST_SHIFT 13
#define  MAD_R6_ST_MASK_SHIFT(reg) (((reg) & MAD_R6_ST_MASK) >> MAD_R6_ST_SHIFT)
#define  MAD_R6_ST_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_ST_MASK) | (((uint32_t)val) << MAD_R6_ST_SHIFT))

#define  MAD_R6_RESERVED4_MASK 0x1f00
#define  MAD_R6_RESERVED4_SHIFT 8
#define  MAD_R6_RESERVED4_MASK_SHIFT(reg) (((reg) & MAD_R6_RESERVED4_MASK) >> MAD_R6_RESERVED4_SHIFT)
#define  MAD_R6_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RESERVED4_MASK) | (((uint32_t)val) << MAD_R6_RESERVED4_SHIFT))

#define  MAD_R6_FEF_MASK 0x80
#define  MAD_R6_FEF_SHIFT 7
#define  MAD_R6_FEF_MASK_SHIFT(reg) (((reg) & MAD_R6_FEF_MASK) >> MAD_R6_FEF_SHIFT)
#define  MAD_R6_FEF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_FEF_MASK) | (((uint32_t)val) << MAD_R6_FEF_SHIFT))

#define  MAD_R6_FUF_MASK 0x40
#define  MAD_R6_FUF_SHIFT 6
#define  MAD_R6_FUF_MASK_SHIFT(reg) (((reg) & MAD_R6_FUF_MASK) >> MAD_R6_FUF_SHIFT)
#define  MAD_R6_FUF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_FUF_MASK) | (((uint32_t)val) << MAD_R6_FUF_SHIFT))

#define  MAD_R6_RESERVED5_MASK 0x20
#define  MAD_R6_RESERVED5_SHIFT 5
#define  MAD_R6_RESERVED5_MASK_SHIFT(reg) (((reg) & MAD_R6_RESERVED5_MASK) >> MAD_R6_RESERVED5_SHIFT)
#define  MAD_R6_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RESERVED5_MASK) | (((uint32_t)val) << MAD_R6_RESERVED5_SHIFT))

#define  MAD_R6_RTC_MASK 0x18
#define  MAD_R6_RTC_SHIFT 3
#define  MAD_R6_RTC_MASK_SHIFT(reg) (((reg) & MAD_R6_RTC_MASK) >> MAD_R6_RTC_SHIFT)
#define  MAD_R6_RTC_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RTC_MASK) | (((uint32_t)val) << MAD_R6_RTC_SHIFT))

#define  MAD_R6_OSF_MASK 0x4
#define  MAD_R6_OSF_SHIFT 2
#define  MAD_R6_OSF_MASK_SHIFT(reg) (((reg) & MAD_R6_OSF_MASK) >> MAD_R6_OSF_SHIFT)
#define  MAD_R6_OSF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_OSF_MASK) | (((uint32_t)val) << MAD_R6_OSF_SHIFT))

#define  MAD_R6_SR_MASK 0x2
#define  MAD_R6_SR_SHIFT 1
#define  MAD_R6_SR_MASK_SHIFT(reg) (((reg) & MAD_R6_SR_MASK) >> MAD_R6_SR_SHIFT)
#define  MAD_R6_SR_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_SR_MASK) | (((uint32_t)val) << MAD_R6_SR_SHIFT))

#define  MAD_R6_RESERVED6_MASK 0x1
#define  MAD_R6_RESERVED6_SHIFT 0
#define  MAD_R6_RESERVED6_MASK_SHIFT(reg) (((reg) & MAD_R6_RESERVED6_MASK) >> MAD_R6_RESERVED6_SHIFT)
#define  MAD_R6_RESERVED6_REPLACE_VAL(reg,val) (((reg) & ~MAD_R6_RESERVED6_MASK) | (((uint32_t)val) << MAD_R6_RESERVED6_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (r7)
/** \brief Enables the interrupts reported by the Status Register.
The Interrupt Enable register enables the interrupts reported by Register5. Setting a bit to 1'b1 enables a corresponding interrupt. After a hardware or software reset, all interrupts are disabled.*/
//====================================================================

#define  MAD_R7_RESERVED1_MASK 0xfffe0000
#define  MAD_R7_RESERVED1_SHIFT 17
#define  MAD_R7_RESERVED1_MASK_SHIFT(reg) (((reg) & MAD_R7_RESERVED1_MASK) >> MAD_R7_RESERVED1_SHIFT)
#define  MAD_R7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_RESERVED1_MASK) | (((uint32_t)val) << MAD_R7_RESERVED1_SHIFT))

#define  MAD_R7_NIE_MASK 0x10000
#define  MAD_R7_NIE_SHIFT 16
#define  MAD_R7_NIE_MASK_SHIFT(reg) (((reg) & MAD_R7_NIE_MASK) >> MAD_R7_NIE_SHIFT)
#define  MAD_R7_NIE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_NIE_MASK) | (((uint32_t)val) << MAD_R7_NIE_SHIFT))

#define  MAD_R7_AIE_MASK 0x8000
#define  MAD_R7_AIE_SHIFT 15
#define  MAD_R7_AIE_MASK_SHIFT(reg) (((reg) & MAD_R7_AIE_MASK) >> MAD_R7_AIE_SHIFT)
#define  MAD_R7_AIE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_AIE_MASK) | (((uint32_t)val) << MAD_R7_AIE_SHIFT))

#define  MAD_R7_ERE_MASK 0x4000
#define  MAD_R7_ERE_SHIFT 14
#define  MAD_R7_ERE_MASK_SHIFT(reg) (((reg) & MAD_R7_ERE_MASK) >> MAD_R7_ERE_SHIFT)
#define  MAD_R7_ERE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_ERE_MASK) | (((uint32_t)val) << MAD_R7_ERE_SHIFT))

#define  MAD_R7_FBE_MASK 0x2000
#define  MAD_R7_FBE_SHIFT 13
#define  MAD_R7_FBE_MASK_SHIFT(reg) (((reg) & MAD_R7_FBE_MASK) >> MAD_R7_FBE_SHIFT)
#define  MAD_R7_FBE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_FBE_MASK) | (((uint32_t)val) << MAD_R7_FBE_SHIFT))

#define  MAD_R7_RESERVED2_MASK 0x1800
#define  MAD_R7_RESERVED2_SHIFT 11
#define  MAD_R7_RESERVED2_MASK_SHIFT(reg) (((reg) & MAD_R7_RESERVED2_MASK) >> MAD_R7_RESERVED2_SHIFT)
#define  MAD_R7_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_RESERVED2_MASK) | (((uint32_t)val) << MAD_R7_RESERVED2_SHIFT))

#define  MAD_R7_ETE_MASK 0x400
#define  MAD_R7_ETE_SHIFT 10
#define  MAD_R7_ETE_MASK_SHIFT(reg) (((reg) & MAD_R7_ETE_MASK) >> MAD_R7_ETE_SHIFT)
#define  MAD_R7_ETE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_ETE_MASK) | (((uint32_t)val) << MAD_R7_ETE_SHIFT))

#define  MAD_R7_RWE_MASK 0x200
#define  MAD_R7_RWE_SHIFT 9
#define  MAD_R7_RWE_MASK_SHIFT(reg) (((reg) & MAD_R7_RWE_MASK) >> MAD_R7_RWE_SHIFT)
#define  MAD_R7_RWE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_RWE_MASK) | (((uint32_t)val) << MAD_R7_RWE_SHIFT))

#define  MAD_R7_RSE_MASK 0x100
#define  MAD_R7_RSE_SHIFT 8
#define  MAD_R7_RSE_MASK_SHIFT(reg) (((reg) & MAD_R7_RSE_MASK) >> MAD_R7_RSE_SHIFT)
#define  MAD_R7_RSE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_RSE_MASK) | (((uint32_t)val) << MAD_R7_RSE_SHIFT))

#define  MAD_R7_RUE_MASK 0x80
#define  MAD_R7_RUE_SHIFT 7
#define  MAD_R7_RUE_MASK_SHIFT(reg) (((reg) & MAD_R7_RUE_MASK) >> MAD_R7_RUE_SHIFT)
#define  MAD_R7_RUE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_RUE_MASK) | (((uint32_t)val) << MAD_R7_RUE_SHIFT))

#define  MAD_R7_RIE_MASK 0x40
#define  MAD_R7_RIE_SHIFT 6
#define  MAD_R7_RIE_MASK_SHIFT(reg) (((reg) & MAD_R7_RIE_MASK) >> MAD_R7_RIE_SHIFT)
#define  MAD_R7_RIE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_RIE_MASK) | (((uint32_t)val) << MAD_R7_RIE_SHIFT))

#define  MAD_R7_UNE_MASK 0x20
#define  MAD_R7_UNE_SHIFT 5
#define  MAD_R7_UNE_MASK_SHIFT(reg) (((reg) & MAD_R7_UNE_MASK) >> MAD_R7_UNE_SHIFT)
#define  MAD_R7_UNE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_UNE_MASK) | (((uint32_t)val) << MAD_R7_UNE_SHIFT))

#define  MAD_R7_OVE_MASK 0x10
#define  MAD_R7_OVE_SHIFT 4
#define  MAD_R7_OVE_MASK_SHIFT(reg) (((reg) & MAD_R7_OVE_MASK) >> MAD_R7_OVE_SHIFT)
#define  MAD_R7_OVE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_OVE_MASK) | (((uint32_t)val) << MAD_R7_OVE_SHIFT))

#define  MAD_R7_TJE_MASK 0x8
#define  MAD_R7_TJE_SHIFT 3
#define  MAD_R7_TJE_MASK_SHIFT(reg) (((reg) & MAD_R7_TJE_MASK) >> MAD_R7_TJE_SHIFT)
#define  MAD_R7_TJE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_TJE_MASK) | (((uint32_t)val) << MAD_R7_TJE_SHIFT))

#define  MAD_R7_TUE_MASK 0x4
#define  MAD_R7_TUE_SHIFT 2
#define  MAD_R7_TUE_MASK_SHIFT(reg) (((reg) & MAD_R7_TUE_MASK) >> MAD_R7_TUE_SHIFT)
#define  MAD_R7_TUE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_TUE_MASK) | (((uint32_t)val) << MAD_R7_TUE_SHIFT))

#define  MAD_R7_TSE_MASK 0x2
#define  MAD_R7_TSE_SHIFT 1
#define  MAD_R7_TSE_MASK_SHIFT(reg) (((reg) & MAD_R7_TSE_MASK) >> MAD_R7_TSE_SHIFT)
#define  MAD_R7_TSE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_TSE_MASK) | (((uint32_t)val) << MAD_R7_TSE_SHIFT))

#define  MAD_R7_TIE_MASK 0x1
#define  MAD_R7_TIE_SHIFT 0
#define  MAD_R7_TIE_MASK_SHIFT(reg) (((reg) & MAD_R7_TIE_MASK) >> MAD_R7_TIE_SHIFT)
#define  MAD_R7_TIE_REPLACE_VAL(reg,val) (((reg) & ~MAD_R7_TIE_MASK) | (((uint32_t)val) << MAD_R7_TIE_SHIFT))

//====================================================================
//Register: Missed Frame and Buffer Overflow Counter Register (r8)
/** \brief Contains the counters for discarded frames because no host Receive Descriptor was available, and discarded frames because of Receive FIFO Overflow.
The DMA maintains two counters to track the number of missed frames during reception. This register reports the current value of the counter. The counter is used for diagnostic purposes. Bits[15:0] indicate missed frames due to the host buffer being unavailable. Bits[27:17] indicate missed frames due to buffer overflow conditions (MTL and MAC) and runt frames (good frames of less than 64 bytes) dropped by the MTL.*/
//====================================================================

#define  MAD_R8_RESERVED1_MASK 0xe0000000
#define  MAD_R8_RESERVED1_SHIFT 29
#define  MAD_R8_RESERVED1_MASK_SHIFT(reg) (((reg) & MAD_R8_RESERVED1_MASK) >> MAD_R8_RESERVED1_SHIFT)
#define  MAD_R8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~MAD_R8_RESERVED1_MASK) | (((uint32_t)val) << MAD_R8_RESERVED1_SHIFT))

#define  MAD_R8_OBFFOC_MASK 0x10000000
#define  MAD_R8_OBFFOC_SHIFT 28
#define  MAD_R8_OBFFOC_MASK_SHIFT(reg) (((reg) & MAD_R8_OBFFOC_MASK) >> MAD_R8_OBFFOC_SHIFT)
#define  MAD_R8_OBFFOC_REPLACE_VAL(reg,val) (((reg) & ~MAD_R8_OBFFOC_MASK) | (((uint32_t)val) << MAD_R8_OBFFOC_SHIFT))

#define  MAD_R8_NOMF_MASK 0xffe0000
#define  MAD_R8_NOMF_SHIFT 17
#define  MAD_R8_NOMF_MASK_SHIFT(reg) (((reg) & MAD_R8_NOMF_MASK) >> MAD_R8_NOMF_SHIFT)
#define  MAD_R8_NOMF_REPLACE_VAL(reg,val) (((reg) & ~MAD_R8_NOMF_MASK) | (((uint32_t)val) << MAD_R8_NOMF_SHIFT))

#define  MAD_R8_OBFMFC_MASK 0x10000
#define  MAD_R8_OBFMFC_SHIFT 16
#define  MAD_R8_OBFMFC_MASK_SHIFT(reg) (((reg) & MAD_R8_OBFMFC_MASK) >> MAD_R8_OBFMFC_SHIFT)
#define  MAD_R8_OBFMFC_REPLACE_VAL(reg,val) (((reg) & ~MAD_R8_OBFMFC_MASK) | (((uint32_t)val) << MAD_R8_OBFMFC_SHIFT))

#define  MAD_R8_NOMFHBU_MASK 0xffff
#define  MAD_R8_NOMFHBU_SHIFT 0
#define  MAD_R8_NOMFHBU_MASK_SHIFT(reg) (((reg) & MAD_R8_NOMFHBU_MASK) >> MAD_R8_NOMFHBU_SHIFT)
#define  MAD_R8_NOMFHBU_REPLACE_VAL(reg,val) (((reg) & ~MAD_R8_NOMFHBU_MASK) | (((uint32_t)val) << MAD_R8_NOMFHBU_SHIFT))

//====================================================================
//Register: Current Host Transmit Descriptor Register (r18)
/** \brief Points to the start of current Transmit Descriptor read by the DMA.
The Current Host Transmit Descriptor register points to the start address of the current Transmit Descriptor read by the DMA.*/
//====================================================================

#define  MAD_R18_HTDAP_MASK 0xffffffff
#define  MAD_R18_HTDAP_SHIFT 0
#define  MAD_R18_HTDAP_MASK_SHIFT(reg) (((reg) & MAD_R18_HTDAP_MASK) >> MAD_R18_HTDAP_SHIFT)
#define  MAD_R18_HTDAP_REPLACE_VAL(reg,val) (((reg) & ~MAD_R18_HTDAP_MASK) | (((uint32_t)val) << MAD_R18_HTDAP_SHIFT))

//====================================================================
//Register: Current Host Receive Descriptor Register (r19)
/** \brief Points to the start of current Receive Descriptor read by the DMA.
The Current Host Receive Descriptor register points to the start address of the current Receive Descriptor read by the DMA.*/
//====================================================================

#define  MAD_R19_HRDAP_MASK 0xffffffff
#define  MAD_R19_HRDAP_SHIFT 0
#define  MAD_R19_HRDAP_MASK_SHIFT(reg) (((reg) & MAD_R19_HRDAP_MASK) >> MAD_R19_HRDAP_SHIFT)
#define  MAD_R19_HRDAP_REPLACE_VAL(reg,val) (((reg) & ~MAD_R19_HRDAP_MASK) | (((uint32_t)val) << MAD_R19_HRDAP_SHIFT))

//====================================================================
//Register: Current Host Transmit Buffer Address Register (r20)
/** \brief Points to the current Transmit Buffer address read by the DMA.
The Current Host Transmit Buffer Address register points to the current Transmit Buffer Address being read by the DMA.*/
//====================================================================

#define  MAD_R20_HTBAP_MASK 0xffffffff
#define  MAD_R20_HTBAP_SHIFT 0
#define  MAD_R20_HTBAP_MASK_SHIFT(reg) (((reg) & MAD_R20_HTBAP_MASK) >> MAD_R20_HTBAP_SHIFT)
#define  MAD_R20_HTBAP_REPLACE_VAL(reg,val) (((reg) & ~MAD_R20_HTBAP_MASK) | (((uint32_t)val) << MAD_R20_HTBAP_SHIFT))

//====================================================================
//Register: Current Host Receive Buffer Address Register (r21)
/** \brief Points to the current Receive Buffer address read by the DMA.
The Current Host Receive Buffer Address register points to the current Receive Buffer address being read by the DMA.*/
//====================================================================

#define  MAD_R21_HRBAP_MASK 0xffffffff
#define  MAD_R21_HRBAP_SHIFT 0
#define  MAD_R21_HRBAP_MASK_SHIFT(reg) (((reg) & MAD_R21_HRBAP_MASK) >> MAD_R21_HRBAP_SHIFT)
#define  MAD_R21_HRBAP_REPLACE_VAL(reg,val) (((reg) & ~MAD_R21_HRBAP_MASK) | (((uint32_t)val) << MAD_R21_HRBAP_SHIFT))

#endif // LAN
