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
 * \file PCIE_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _PCIE_REGMASKS_H_
#define _PCIE_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: PCIE (PCIE)
/** \brief Top-level register file for PCIE*/
//
//====================================================================

//====================================================================
//
//Register File: PCIE Dual Mode Core (PCIE_DM)
/** \brief Top-level register file for PCIE_DM*/
//
//====================================================================

//====================================================================
//Register: DM Core Target Configuration Register 00 (TCfg00)
/** \brief Start of the DM Core Target EP Cfg Header Space per PCIe 2.0 Specification for Type 0 Headers.*/
//====================================================================

#define  PCIE_DM_TCFG00_DEVICEID_MASK 0xffff0000
#define  PCIE_DM_TCFG00_DEVICEID_SHIFT 16
#define  PCIE_DM_TCFG00_DEVICEID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG00_DEVICEID_MASK) >> PCIE_DM_TCFG00_DEVICEID_SHIFT)
#define  PCIE_DM_TCFG00_DEVICEID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG00_DEVICEID_MASK) | (((uint32_t)val) << PCIE_DM_TCFG00_DEVICEID_SHIFT))

#define  PCIE_DM_TCFG00_VENDORID_MASK 0xffff
#define  PCIE_DM_TCFG00_VENDORID_SHIFT 0
#define  PCIE_DM_TCFG00_VENDORID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG00_VENDORID_MASK) >> PCIE_DM_TCFG00_VENDORID_SHIFT)
#define  PCIE_DM_TCFG00_VENDORID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG00_VENDORID_MASK) | (((uint32_t)val) << PCIE_DM_TCFG00_VENDORID_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 04 (TCfg04)
/** \brief Status and Command Registers for Type 0 Header*/
//====================================================================

#define  PCIE_DM_TCFG04_STATUS_MASK 0xffff0000
#define  PCIE_DM_TCFG04_STATUS_SHIFT 16
#define  PCIE_DM_TCFG04_STATUS_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG04_STATUS_MASK) >> PCIE_DM_TCFG04_STATUS_SHIFT)
#define  PCIE_DM_TCFG04_STATUS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG04_STATUS_MASK) | (((uint32_t)val) << PCIE_DM_TCFG04_STATUS_SHIFT))

#define  PCIE_DM_TCFG04_COMMAND_MASK 0xffff
#define  PCIE_DM_TCFG04_COMMAND_SHIFT 0
#define  PCIE_DM_TCFG04_COMMAND_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG04_COMMAND_MASK) >> PCIE_DM_TCFG04_COMMAND_SHIFT)
#define  PCIE_DM_TCFG04_COMMAND_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG04_COMMAND_MASK) | (((uint32_t)val) << PCIE_DM_TCFG04_COMMAND_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 08 (TCfg08)
/** \brief Class Code and Revision ID for Type 0 Header*/
//====================================================================

#define  PCIE_DM_TCFG08_CLASSCODE_MASK 0xffffff00
#define  PCIE_DM_TCFG08_CLASSCODE_SHIFT 8
#define  PCIE_DM_TCFG08_CLASSCODE_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG08_CLASSCODE_MASK) >> PCIE_DM_TCFG08_CLASSCODE_SHIFT)
#define  PCIE_DM_TCFG08_CLASSCODE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG08_CLASSCODE_MASK) | (((uint32_t)val) << PCIE_DM_TCFG08_CLASSCODE_SHIFT))

#define  PCIE_DM_TCFG08_REVID_MASK 0xff
#define  PCIE_DM_TCFG08_REVID_SHIFT 0
#define  PCIE_DM_TCFG08_REVID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG08_REVID_MASK) >> PCIE_DM_TCFG08_REVID_SHIFT)
#define  PCIE_DM_TCFG08_REVID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG08_REVID_MASK) | (((uint32_t)val) << PCIE_DM_TCFG08_REVID_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 0C (TCfg0C)
/** \brief BIST, Header Type, Latency Timer and Cacheline Size for Type 0 Header*/
//====================================================================

#define  PCIE_DM_TCFG0C_BIST_MASK 0xff000000
#define  PCIE_DM_TCFG0C_BIST_SHIFT 24
#define  PCIE_DM_TCFG0C_BIST_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG0C_BIST_MASK) >> PCIE_DM_TCFG0C_BIST_SHIFT)
#define  PCIE_DM_TCFG0C_BIST_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG0C_BIST_MASK) | (((uint32_t)val) << PCIE_DM_TCFG0C_BIST_SHIFT))

#define  PCIE_DM_TCFG0C_HDRTYPE_MASK 0xff0000
#define  PCIE_DM_TCFG0C_HDRTYPE_SHIFT 16
#define  PCIE_DM_TCFG0C_HDRTYPE_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG0C_HDRTYPE_MASK) >> PCIE_DM_TCFG0C_HDRTYPE_SHIFT)
#define  PCIE_DM_TCFG0C_HDRTYPE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG0C_HDRTYPE_MASK) | (((uint32_t)val) << PCIE_DM_TCFG0C_HDRTYPE_SHIFT))

#define  PCIE_DM_TCFG0C_LATTIMER_MASK 0xff00
#define  PCIE_DM_TCFG0C_LATTIMER_SHIFT 8
#define  PCIE_DM_TCFG0C_LATTIMER_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG0C_LATTIMER_MASK) >> PCIE_DM_TCFG0C_LATTIMER_SHIFT)
#define  PCIE_DM_TCFG0C_LATTIMER_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG0C_LATTIMER_MASK) | (((uint32_t)val) << PCIE_DM_TCFG0C_LATTIMER_SHIFT))

#define  PCIE_DM_TCFG0C_CACHELINESIZE_MASK 0xff
#define  PCIE_DM_TCFG0C_CACHELINESIZE_SHIFT 0
#define  PCIE_DM_TCFG0C_CACHELINESIZE_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG0C_CACHELINESIZE_MASK) >> PCIE_DM_TCFG0C_CACHELINESIZE_SHIFT)
#define  PCIE_DM_TCFG0C_CACHELINESIZE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG0C_CACHELINESIZE_MASK) | (((uint32_t)val) << PCIE_DM_TCFG0C_CACHELINESIZE_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 10 is BAR0 (TCfg10)
/** \brief BAR0 Register*/
//====================================================================

#define  PCIE_DM_TCFG10_BAR0_MASK 0xffffffff
#define  PCIE_DM_TCFG10_BAR0_SHIFT 0
#define  PCIE_DM_TCFG10_BAR0_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG10_BAR0_MASK) >> PCIE_DM_TCFG10_BAR0_SHIFT)
#define  PCIE_DM_TCFG10_BAR0_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG10_BAR0_MASK) | (((uint32_t)val) << PCIE_DM_TCFG10_BAR0_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 14 is BAR1 (TCfg14)
/** \brief BAR0 Register*/
//====================================================================

#define  PCIE_DM_TCFG14_BAR1_MASK 0xffffffff
#define  PCIE_DM_TCFG14_BAR1_SHIFT 0
#define  PCIE_DM_TCFG14_BAR1_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG14_BAR1_MASK) >> PCIE_DM_TCFG14_BAR1_SHIFT)
#define  PCIE_DM_TCFG14_BAR1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG14_BAR1_MASK) | (((uint32_t)val) << PCIE_DM_TCFG14_BAR1_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 2C, Subsystem ID and Subsystem Vendor ID (TCfg2C)
/** \brief Subsystem ID/Vendor ID*/
//====================================================================

#define  PCIE_DM_TCFG2C_SUBSYSID_MASK 0xffff0000
#define  PCIE_DM_TCFG2C_SUBSYSID_SHIFT 16
#define  PCIE_DM_TCFG2C_SUBSYSID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG2C_SUBSYSID_MASK) >> PCIE_DM_TCFG2C_SUBSYSID_SHIFT)
#define  PCIE_DM_TCFG2C_SUBSYSID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG2C_SUBSYSID_MASK) | (((uint32_t)val) << PCIE_DM_TCFG2C_SUBSYSID_SHIFT))

#define  PCIE_DM_TCFG2C_SUBSYSVID_MASK 0xffff
#define  PCIE_DM_TCFG2C_SUBSYSVID_SHIFT 0
#define  PCIE_DM_TCFG2C_SUBSYSVID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG2C_SUBSYSVID_MASK) >> PCIE_DM_TCFG2C_SUBSYSVID_SHIFT)
#define  PCIE_DM_TCFG2C_SUBSYSVID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG2C_SUBSYSVID_MASK) | (((uint32_t)val) << PCIE_DM_TCFG2C_SUBSYSVID_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 34 (TCfg34)
/** \brief PCIe Cfg Header Capabilities Pointer - contents point to Capabilities Registers.
Must be > 0x3F to be outside basic Type 0 Cfg space.*/
//====================================================================

#define  PCIE_DM_TCFG34_RESERVED1_MASK 0xffffff00
#define  PCIE_DM_TCFG34_RESERVED1_SHIFT 8
#define  PCIE_DM_TCFG34_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG34_RESERVED1_MASK) >> PCIE_DM_TCFG34_RESERVED1_SHIFT)
#define  PCIE_DM_TCFG34_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG34_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_TCFG34_RESERVED1_SHIFT))

#define  PCIE_DM_TCFG34_CAPABPTR_MASK 0xff
#define  PCIE_DM_TCFG34_CAPABPTR_SHIFT 0
#define  PCIE_DM_TCFG34_CAPABPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG34_CAPABPTR_MASK) >> PCIE_DM_TCFG34_CAPABPTR_SHIFT)
#define  PCIE_DM_TCFG34_CAPABPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG34_CAPABPTR_MASK) | (((uint32_t)val) << PCIE_DM_TCFG34_CAPABPTR_SHIFT))

//====================================================================
//Register: DM Core Target Configuration Register 3C, Interrupt Pin  and Interrupt Line (TCfg3C)
/** \brief Interrupt Pin and Interrupt Line*/
//====================================================================

#define  PCIE_DM_TCFG3C_RESERVED1_MASK 0xffff0000
#define  PCIE_DM_TCFG3C_RESERVED1_SHIFT 16
#define  PCIE_DM_TCFG3C_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG3C_RESERVED1_MASK) >> PCIE_DM_TCFG3C_RESERVED1_SHIFT)
#define  PCIE_DM_TCFG3C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG3C_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_TCFG3C_RESERVED1_SHIFT))

#define  PCIE_DM_TCFG3C_INTPIN_MASK 0xff00
#define  PCIE_DM_TCFG3C_INTPIN_SHIFT 8
#define  PCIE_DM_TCFG3C_INTPIN_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG3C_INTPIN_MASK) >> PCIE_DM_TCFG3C_INTPIN_SHIFT)
#define  PCIE_DM_TCFG3C_INTPIN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG3C_INTPIN_MASK) | (((uint32_t)val) << PCIE_DM_TCFG3C_INTPIN_SHIFT))

#define  PCIE_DM_TCFG3C_INTLINE_MASK 0xff
#define  PCIE_DM_TCFG3C_INTLINE_SHIFT 0
#define  PCIE_DM_TCFG3C_INTLINE_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCFG3C_INTLINE_MASK) >> PCIE_DM_TCFG3C_INTLINE_SHIFT)
#define  PCIE_DM_TCFG3C_INTLINE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCFG3C_INTLINE_MASK) | (((uint32_t)val) << PCIE_DM_TCFG3C_INTLINE_SHIFT))

//====================================================================
//Register: DM Core Target Power Management Capabilities Register (TPMCap)
/** \brief Power Management Capabilities Register.*/
//====================================================================

#define  PCIE_DM_TPMCAP_PMCAP_MASK 0xffff0000
#define  PCIE_DM_TPMCAP_PMCAP_SHIFT 16
#define  PCIE_DM_TPMCAP_PMCAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCAP_PMCAP_MASK) >> PCIE_DM_TPMCAP_PMCAP_SHIFT)
#define  PCIE_DM_TPMCAP_PMCAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCAP_PMCAP_MASK) | (((uint32_t)val) << PCIE_DM_TPMCAP_PMCAP_SHIFT))

#define  PCIE_DM_TPMCAP_NEXTCAPPTR_MASK 0xff00
#define  PCIE_DM_TPMCAP_NEXTCAPPTR_SHIFT 8
#define  PCIE_DM_TPMCAP_NEXTCAPPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCAP_NEXTCAPPTR_MASK) >> PCIE_DM_TPMCAP_NEXTCAPPTR_SHIFT)
#define  PCIE_DM_TPMCAP_NEXTCAPPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCAP_NEXTCAPPTR_MASK) | (((uint32_t)val) << PCIE_DM_TPMCAP_NEXTCAPPTR_SHIFT))

#define  PCIE_DM_TPMCAP_CAPABID_MASK 0xff
#define  PCIE_DM_TPMCAP_CAPABID_SHIFT 0
#define  PCIE_DM_TPMCAP_CAPABID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCAP_CAPABID_MASK) >> PCIE_DM_TPMCAP_CAPABID_SHIFT)
#define  PCIE_DM_TPMCAP_CAPABID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCAP_CAPABID_MASK) | (((uint32_t)val) << PCIE_DM_TPMCAP_CAPABID_SHIFT))

//====================================================================
//Register: DM Core Target Power Management Control and Status Register (TPMCSR)
/** \brief Power Management Control and Status Register.*/
//====================================================================

#define  PCIE_DM_TPMCSR_RESERVED1_MASK 0xffff0000
#define  PCIE_DM_TPMCSR_RESERVED1_SHIFT 16
#define  PCIE_DM_TPMCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCSR_RESERVED1_MASK) >> PCIE_DM_TPMCSR_RESERVED1_SHIFT)
#define  PCIE_DM_TPMCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCSR_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_TPMCSR_RESERVED1_SHIFT))

#define  PCIE_DM_TPMCSR_PMESTAT_MASK 0x8000
#define  PCIE_DM_TPMCSR_PMESTAT_SHIFT 15
#define  PCIE_DM_TPMCSR_PMESTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCSR_PMESTAT_MASK) >> PCIE_DM_TPMCSR_PMESTAT_SHIFT)
#define  PCIE_DM_TPMCSR_PMESTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCSR_PMESTAT_MASK) | (((uint32_t)val) << PCIE_DM_TPMCSR_PMESTAT_SHIFT))

#define  PCIE_DM_TPMCSR_RESERVED2_MASK 0x7e00
#define  PCIE_DM_TPMCSR_RESERVED2_SHIFT 9
#define  PCIE_DM_TPMCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCSR_RESERVED2_MASK) >> PCIE_DM_TPMCSR_RESERVED2_SHIFT)
#define  PCIE_DM_TPMCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCSR_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_TPMCSR_RESERVED2_SHIFT))

#define  PCIE_DM_TPMCSR_PME_EN_MASK 0x100
#define  PCIE_DM_TPMCSR_PME_EN_SHIFT 8
#define  PCIE_DM_TPMCSR_PME_EN_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCSR_PME_EN_MASK) >> PCIE_DM_TPMCSR_PME_EN_SHIFT)
#define  PCIE_DM_TPMCSR_PME_EN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCSR_PME_EN_MASK) | (((uint32_t)val) << PCIE_DM_TPMCSR_PME_EN_SHIFT))

#define  PCIE_DM_TPMCSR_RESERVED3_MASK 0xfc
#define  PCIE_DM_TPMCSR_RESERVED3_SHIFT 2
#define  PCIE_DM_TPMCSR_RESERVED3_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCSR_RESERVED3_MASK) >> PCIE_DM_TPMCSR_RESERVED3_SHIFT)
#define  PCIE_DM_TPMCSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCSR_RESERVED3_MASK) | (((uint32_t)val) << PCIE_DM_TPMCSR_RESERVED3_SHIFT))

#define  PCIE_DM_TPMCSR_PWRSTATE_MASK 0x3
#define  PCIE_DM_TPMCSR_PWRSTATE_SHIFT 0
#define  PCIE_DM_TPMCSR_PWRSTATE_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPMCSR_PWRSTATE_MASK) >> PCIE_DM_TPMCSR_PWRSTATE_SHIFT)
#define  PCIE_DM_TPMCSR_PWRSTATE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPMCSR_PWRSTATE_MASK) | (((uint32_t)val) << PCIE_DM_TPMCSR_PWRSTATE_SHIFT))

//====================================================================
//Register: DM Core Target MSI Capabilities Control Register (TMsiCtrl)
/** \brief MSI Capabilities Control Register.*/
//====================================================================

#define  PCIE_DM_TMSICTRL_MISCCONTROL_MASK 0xfffe0000
#define  PCIE_DM_TMSICTRL_MISCCONTROL_SHIFT 17
#define  PCIE_DM_TMSICTRL_MISCCONTROL_MASK_SHIFT(reg) (((reg) & PCIE_DM_TMSICTRL_MISCCONTROL_MASK) >> PCIE_DM_TMSICTRL_MISCCONTROL_SHIFT)
#define  PCIE_DM_TMSICTRL_MISCCONTROL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TMSICTRL_MISCCONTROL_MASK) | (((uint32_t)val) << PCIE_DM_TMSICTRL_MISCCONTROL_SHIFT))

#define  PCIE_DM_TMSICTRL_MSIEN_MASK 0x10000
#define  PCIE_DM_TMSICTRL_MSIEN_SHIFT 16
#define  PCIE_DM_TMSICTRL_MSIEN_MASK_SHIFT(reg) (((reg) & PCIE_DM_TMSICTRL_MSIEN_MASK) >> PCIE_DM_TMSICTRL_MSIEN_SHIFT)
#define  PCIE_DM_TMSICTRL_MSIEN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TMSICTRL_MSIEN_MASK) | (((uint32_t)val) << PCIE_DM_TMSICTRL_MSIEN_SHIFT))

#define  PCIE_DM_TMSICTRL_NEXTCAPPTR_MASK 0xff00
#define  PCIE_DM_TMSICTRL_NEXTCAPPTR_SHIFT 8
#define  PCIE_DM_TMSICTRL_NEXTCAPPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_TMSICTRL_NEXTCAPPTR_MASK) >> PCIE_DM_TMSICTRL_NEXTCAPPTR_SHIFT)
#define  PCIE_DM_TMSICTRL_NEXTCAPPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TMSICTRL_NEXTCAPPTR_MASK) | (((uint32_t)val) << PCIE_DM_TMSICTRL_NEXTCAPPTR_SHIFT))

#define  PCIE_DM_TMSICTRL_CAPABID_MASK 0xff
#define  PCIE_DM_TMSICTRL_CAPABID_SHIFT 0
#define  PCIE_DM_TMSICTRL_CAPABID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TMSICTRL_CAPABID_MASK) >> PCIE_DM_TMSICTRL_CAPABID_SHIFT)
#define  PCIE_DM_TMSICTRL_CAPABID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TMSICTRL_CAPABID_MASK) | (((uint32_t)val) << PCIE_DM_TMSICTRL_CAPABID_SHIFT))

//====================================================================
//Register: DM Core Target MSI Capabilities  Address Register (TMsiAddr)
/** \brief MSI Capabilities Message Address.*/
//====================================================================

#define  PCIE_DM_TMSIADDR_ADDRESS_MASK 0xffffffff
#define  PCIE_DM_TMSIADDR_ADDRESS_SHIFT 0
#define  PCIE_DM_TMSIADDR_ADDRESS_MASK_SHIFT(reg) (((reg) & PCIE_DM_TMSIADDR_ADDRESS_MASK) >> PCIE_DM_TMSIADDR_ADDRESS_SHIFT)
#define  PCIE_DM_TMSIADDR_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TMSIADDR_ADDRESS_MASK) | (((uint32_t)val) << PCIE_DM_TMSIADDR_ADDRESS_SHIFT))

//====================================================================
//Register: DM Core Target MSI Capabilities Data  Register (TMsiData)
/** \brief MSI Capabilities Message Data*/
//====================================================================

#define  PCIE_DM_TMSIDATA_RESERVED1_MASK 0xffff0000
#define  PCIE_DM_TMSIDATA_RESERVED1_SHIFT 16
#define  PCIE_DM_TMSIDATA_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_TMSIDATA_RESERVED1_MASK) >> PCIE_DM_TMSIDATA_RESERVED1_SHIFT)
#define  PCIE_DM_TMSIDATA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TMSIDATA_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_TMSIDATA_RESERVED1_SHIFT))

#define  PCIE_DM_TMSIDATA_DATA_MASK 0xffff
#define  PCIE_DM_TMSIDATA_DATA_SHIFT 0
#define  PCIE_DM_TMSIDATA_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_TMSIDATA_DATA_MASK) >> PCIE_DM_TMSIDATA_DATA_SHIFT)
#define  PCIE_DM_TMSIDATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TMSIDATA_DATA_MASK) | (((uint32_t)val) << PCIE_DM_TMSIDATA_DATA_SHIFT))

//====================================================================
//Register: DM Core Target PCIE Capabilities Register (TPCIECap)
/** \brief PCI Express Capabilities Register.*/
//====================================================================

#define  PCIE_DM_TPCIECAP_PCIECAP_MASK 0xffff0000
#define  PCIE_DM_TPCIECAP_PCIECAP_SHIFT 16
#define  PCIE_DM_TPCIECAP_PCIECAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPCIECAP_PCIECAP_MASK) >> PCIE_DM_TPCIECAP_PCIECAP_SHIFT)
#define  PCIE_DM_TPCIECAP_PCIECAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPCIECAP_PCIECAP_MASK) | (((uint32_t)val) << PCIE_DM_TPCIECAP_PCIECAP_SHIFT))

#define  PCIE_DM_TPCIECAP_NEXTCAPPTR_MASK 0xff00
#define  PCIE_DM_TPCIECAP_NEXTCAPPTR_SHIFT 8
#define  PCIE_DM_TPCIECAP_NEXTCAPPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPCIECAP_NEXTCAPPTR_MASK) >> PCIE_DM_TPCIECAP_NEXTCAPPTR_SHIFT)
#define  PCIE_DM_TPCIECAP_NEXTCAPPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPCIECAP_NEXTCAPPTR_MASK) | (((uint32_t)val) << PCIE_DM_TPCIECAP_NEXTCAPPTR_SHIFT))

#define  PCIE_DM_TPCIECAP_CAPABID_MASK 0xff
#define  PCIE_DM_TPCIECAP_CAPABID_SHIFT 0
#define  PCIE_DM_TPCIECAP_CAPABID_MASK_SHIFT(reg) (((reg) & PCIE_DM_TPCIECAP_CAPABID_MASK) >> PCIE_DM_TPCIECAP_CAPABID_SHIFT)
#define  PCIE_DM_TPCIECAP_CAPABID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TPCIECAP_CAPABID_MASK) | (((uint32_t)val) << PCIE_DM_TPCIECAP_CAPABID_SHIFT))

//====================================================================
//Register: DM Core Target Device Capability Register (TDevCap)
/** \brief Device Capability Register.*/
//====================================================================

#define  PCIE_DM_TDEVCAP_DEVCAP_MASK 0xffffffff
#define  PCIE_DM_TDEVCAP_DEVCAP_SHIFT 0
#define  PCIE_DM_TDEVCAP_DEVCAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVCAP_DEVCAP_MASK) >> PCIE_DM_TDEVCAP_DEVCAP_SHIFT)
#define  PCIE_DM_TDEVCAP_DEVCAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVCAP_DEVCAP_MASK) | (((uint32_t)val) << PCIE_DM_TDEVCAP_DEVCAP_SHIFT))

//====================================================================
//Register: DM Core Target Device Control and Status Register (TDevContStatus)
/** \brief PCI Express Capability Device Control and Status Register.*/
//====================================================================

#define  PCIE_DM_TDEVCONTSTATUS_STATUS_MASK 0xffff0000
#define  PCIE_DM_TDEVCONTSTATUS_STATUS_SHIFT 16
#define  PCIE_DM_TDEVCONTSTATUS_STATUS_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVCONTSTATUS_STATUS_MASK) >> PCIE_DM_TDEVCONTSTATUS_STATUS_SHIFT)
#define  PCIE_DM_TDEVCONTSTATUS_STATUS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVCONTSTATUS_STATUS_MASK) | (((uint32_t)val) << PCIE_DM_TDEVCONTSTATUS_STATUS_SHIFT))

#define  PCIE_DM_TDEVCONTSTATUS_CONTROL_MASK 0xffff
#define  PCIE_DM_TDEVCONTSTATUS_CONTROL_SHIFT 0
#define  PCIE_DM_TDEVCONTSTATUS_CONTROL_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVCONTSTATUS_CONTROL_MASK) >> PCIE_DM_TDEVCONTSTATUS_CONTROL_SHIFT)
#define  PCIE_DM_TDEVCONTSTATUS_CONTROL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVCONTSTATUS_CONTROL_MASK) | (((uint32_t)val) << PCIE_DM_TDEVCONTSTATUS_CONTROL_SHIFT))

//====================================================================
//Register: DM Core Target Device Link Capabilities Register (TDevLinkCap)
/** \brief PCI Express Link Capabilities Register.*/
//====================================================================

#define  PCIE_DM_TDEVLINKCAP_DEVLINKCAP_MASK 0xffffffff
#define  PCIE_DM_TDEVLINKCAP_DEVLINKCAP_SHIFT 0
#define  PCIE_DM_TDEVLINKCAP_DEVLINKCAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVLINKCAP_DEVLINKCAP_MASK) >> PCIE_DM_TDEVLINKCAP_DEVLINKCAP_SHIFT)
#define  PCIE_DM_TDEVLINKCAP_DEVLINKCAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVLINKCAP_DEVLINKCAP_MASK) | (((uint32_t)val) << PCIE_DM_TDEVLINKCAP_DEVLINKCAP_SHIFT))

//====================================================================
//Register: DM Core Target Link Control and Status Register (TLinkContStatus)
/** \brief PCI Express Capability Device Control and Status Register.*/
//====================================================================

#define  PCIE_DM_TLINKCONTSTATUS_STATUS_MASK 0xffff0000
#define  PCIE_DM_TLINKCONTSTATUS_STATUS_SHIFT 16
#define  PCIE_DM_TLINKCONTSTATUS_STATUS_MASK_SHIFT(reg) (((reg) & PCIE_DM_TLINKCONTSTATUS_STATUS_MASK) >> PCIE_DM_TLINKCONTSTATUS_STATUS_SHIFT)
#define  PCIE_DM_TLINKCONTSTATUS_STATUS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TLINKCONTSTATUS_STATUS_MASK) | (((uint32_t)val) << PCIE_DM_TLINKCONTSTATUS_STATUS_SHIFT))

#define  PCIE_DM_TLINKCONTSTATUS_CONTROL_MASK 0xffff
#define  PCIE_DM_TLINKCONTSTATUS_CONTROL_SHIFT 0
#define  PCIE_DM_TLINKCONTSTATUS_CONTROL_MASK_SHIFT(reg) (((reg) & PCIE_DM_TLINKCONTSTATUS_CONTROL_MASK) >> PCIE_DM_TLINKCONTSTATUS_CONTROL_SHIFT)
#define  PCIE_DM_TLINKCONTSTATUS_CONTROL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TLINKCONTSTATUS_CONTROL_MASK) | (((uint32_t)val) << PCIE_DM_TLINKCONTSTATUS_CONTROL_SHIFT))

//====================================================================
//Register: DM Core Target Device Capability2 Register (TDevCap2)
/** \brief Device Capability2 Register.*/
//====================================================================

#define  PCIE_DM_TDEVCAP2_DEVCAP2_MASK 0xffffffff
#define  PCIE_DM_TDEVCAP2_DEVCAP2_SHIFT 0
#define  PCIE_DM_TDEVCAP2_DEVCAP2_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVCAP2_DEVCAP2_MASK) >> PCIE_DM_TDEVCAP2_DEVCAP2_SHIFT)
#define  PCIE_DM_TDEVCAP2_DEVCAP2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVCAP2_DEVCAP2_MASK) | (((uint32_t)val) << PCIE_DM_TDEVCAP2_DEVCAP2_SHIFT))

//====================================================================
//Register: DM Core Target Device Control and Status2 Register (TDevContStatus2)
/** \brief PCI Express Capability Device Control and Status2 Register.*/
//====================================================================

#define  PCIE_DM_TDEVCONTSTATUS2_STATUS2_MASK 0xffff0000
#define  PCIE_DM_TDEVCONTSTATUS2_STATUS2_SHIFT 16
#define  PCIE_DM_TDEVCONTSTATUS2_STATUS2_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVCONTSTATUS2_STATUS2_MASK) >> PCIE_DM_TDEVCONTSTATUS2_STATUS2_SHIFT)
#define  PCIE_DM_TDEVCONTSTATUS2_STATUS2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVCONTSTATUS2_STATUS2_MASK) | (((uint32_t)val) << PCIE_DM_TDEVCONTSTATUS2_STATUS2_SHIFT))

#define  PCIE_DM_TDEVCONTSTATUS2_CONTROL2_MASK 0xffff
#define  PCIE_DM_TDEVCONTSTATUS2_CONTROL2_SHIFT 0
#define  PCIE_DM_TDEVCONTSTATUS2_CONTROL2_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVCONTSTATUS2_CONTROL2_MASK) >> PCIE_DM_TDEVCONTSTATUS2_CONTROL2_SHIFT)
#define  PCIE_DM_TDEVCONTSTATUS2_CONTROL2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVCONTSTATUS2_CONTROL2_MASK) | (((uint32_t)val) << PCIE_DM_TDEVCONTSTATUS2_CONTROL2_SHIFT))

//====================================================================
//Register: DM Core Target Device Link Capabilities2 Register (TDevLinkCap2)
/** \brief PCI Express Link Capabilities2 Register.*/
//====================================================================

#define  PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_MASK 0xffffffff
#define  PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_SHIFT 0
#define  PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_MASK_SHIFT(reg) (((reg) & PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_MASK) >> PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_SHIFT)
#define  PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_MASK) | (((uint32_t)val) << PCIE_DM_TDEVLINKCAP2_DEVLINKCAP2_SHIFT))

//====================================================================
//Register: DM Core Target Link Control and Status2 Register (TLinkContStatus2)
/** \brief PCI Express Capability Device Control and Status2 Register.*/
//====================================================================

#define  PCIE_DM_TLINKCONTSTATUS2_STATUS2_MASK 0xffff0000
#define  PCIE_DM_TLINKCONTSTATUS2_STATUS2_SHIFT 16
#define  PCIE_DM_TLINKCONTSTATUS2_STATUS2_MASK_SHIFT(reg) (((reg) & PCIE_DM_TLINKCONTSTATUS2_STATUS2_MASK) >> PCIE_DM_TLINKCONTSTATUS2_STATUS2_SHIFT)
#define  PCIE_DM_TLINKCONTSTATUS2_STATUS2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TLINKCONTSTATUS2_STATUS2_MASK) | (((uint32_t)val) << PCIE_DM_TLINKCONTSTATUS2_STATUS2_SHIFT))

#define  PCIE_DM_TLINKCONTSTATUS2_CONTROL2_MASK 0xffff
#define  PCIE_DM_TLINKCONTSTATUS2_CONTROL2_SHIFT 0
#define  PCIE_DM_TLINKCONTSTATUS2_CONTROL2_MASK_SHIFT(reg) (((reg) & PCIE_DM_TLINKCONTSTATUS2_CONTROL2_MASK) >> PCIE_DM_TLINKCONTSTATUS2_CONTROL2_SHIFT)
#define  PCIE_DM_TLINKCONTSTATUS2_CONTROL2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TLINKCONTSTATUS2_CONTROL2_MASK) | (((uint32_t)val) << PCIE_DM_TLINKCONTSTATUS2_CONTROL2_SHIFT))

//====================================================================
//Register: DM Core Target AER Capabilities Register (TAERCap)
/** \brief PCI Express AER Enhanced Capability Header Register.*/
//====================================================================

#define  PCIE_DM_TAERCAP_AERHDR_MASK 0xffffffff
#define  PCIE_DM_TAERCAP_AERHDR_SHIFT 0
#define  PCIE_DM_TAERCAP_AERHDR_MASK_SHIFT(reg) (((reg) & PCIE_DM_TAERCAP_AERHDR_MASK) >> PCIE_DM_TAERCAP_AERHDR_SHIFT)
#define  PCIE_DM_TAERCAP_AERHDR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TAERCAP_AERHDR_MASK) | (((uint32_t)val) << PCIE_DM_TAERCAP_AERHDR_SHIFT))

//====================================================================
//Register: DM Core Target Uncorrectable Error Status Register (TUCorrStat)
/** \brief PCI Express AER Uncorrectable Error Status Register.*/
//====================================================================

#define  PCIE_DM_TUCORRSTAT_UCORRESTAT_MASK 0xffffffff
#define  PCIE_DM_TUCORRSTAT_UCORRESTAT_SHIFT 0
#define  PCIE_DM_TUCORRSTAT_UCORRESTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_TUCORRSTAT_UCORRESTAT_MASK) >> PCIE_DM_TUCORRSTAT_UCORRESTAT_SHIFT)
#define  PCIE_DM_TUCORRSTAT_UCORRESTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TUCORRSTAT_UCORRESTAT_MASK) | (((uint32_t)val) << PCIE_DM_TUCORRSTAT_UCORRESTAT_SHIFT))

//====================================================================
//Register: DM Core Target Uncorrectable Error Mask Register (TUCorrMask)
/** \brief PCI Express AER Uncorrectable Error Mask Register.*/
//====================================================================

#define  PCIE_DM_TUCORRMASK_UCORREMASK_MASK 0xffffffff
#define  PCIE_DM_TUCORRMASK_UCORREMASK_SHIFT 0
#define  PCIE_DM_TUCORRMASK_UCORREMASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_TUCORRMASK_UCORREMASK_MASK) >> PCIE_DM_TUCORRMASK_UCORREMASK_SHIFT)
#define  PCIE_DM_TUCORRMASK_UCORREMASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TUCORRMASK_UCORREMASK_MASK) | (((uint32_t)val) << PCIE_DM_TUCORRMASK_UCORREMASK_SHIFT))

//====================================================================
//Register: DM Core Target Uncorrectable Error Severity Register (TUCorrSev)
/** \brief PCI Express AER Uncorrectable Error Severity Register.*/
//====================================================================

#define  PCIE_DM_TUCORRSEV_UCORRESEV_MASK 0xffffffff
#define  PCIE_DM_TUCORRSEV_UCORRESEV_SHIFT 0
#define  PCIE_DM_TUCORRSEV_UCORRESEV_MASK_SHIFT(reg) (((reg) & PCIE_DM_TUCORRSEV_UCORRESEV_MASK) >> PCIE_DM_TUCORRSEV_UCORRESEV_SHIFT)
#define  PCIE_DM_TUCORRSEV_UCORRESEV_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TUCORRSEV_UCORRESEV_MASK) | (((uint32_t)val) << PCIE_DM_TUCORRSEV_UCORRESEV_SHIFT))

//====================================================================
//Register: DM Core Target Correctable Error Status Register (TCorrStat)
/** \brief PCI Express AER Correctable Error Status Register.*/
//====================================================================

#define  PCIE_DM_TCORRSTAT_CORRESTAT_MASK 0xffffffff
#define  PCIE_DM_TCORRSTAT_CORRESTAT_SHIFT 0
#define  PCIE_DM_TCORRSTAT_CORRESTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCORRSTAT_CORRESTAT_MASK) >> PCIE_DM_TCORRSTAT_CORRESTAT_SHIFT)
#define  PCIE_DM_TCORRSTAT_CORRESTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCORRSTAT_CORRESTAT_MASK) | (((uint32_t)val) << PCIE_DM_TCORRSTAT_CORRESTAT_SHIFT))

//====================================================================
//Register: DM Core Target Correctable Error Mask Register (TCorrMask)
/** \brief PCI Express AER Correctable Error Mask Register.*/
//====================================================================

#define  PCIE_DM_TCORRMASK_CORREMASK_MASK 0xffffffff
#define  PCIE_DM_TCORRMASK_CORREMASK_SHIFT 0
#define  PCIE_DM_TCORRMASK_CORREMASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_TCORRMASK_CORREMASK_MASK) >> PCIE_DM_TCORRMASK_CORREMASK_SHIFT)
#define  PCIE_DM_TCORRMASK_CORREMASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TCORRMASK_CORREMASK_MASK) | (((uint32_t)val) << PCIE_DM_TCORRMASK_CORREMASK_SHIFT))

//====================================================================
//Register: DM Core Target AER Capabilities and Control Register (TAERCapCtrl)
/** \brief PCI Express AER Capabilities and Control Register.*/
//====================================================================

#define  PCIE_DM_TAERCAPCTRL_CAPCTRL_MASK 0xffffffff
#define  PCIE_DM_TAERCAPCTRL_CAPCTRL_SHIFT 0
#define  PCIE_DM_TAERCAPCTRL_CAPCTRL_MASK_SHIFT(reg) (((reg) & PCIE_DM_TAERCAPCTRL_CAPCTRL_MASK) >> PCIE_DM_TAERCAPCTRL_CAPCTRL_SHIFT)
#define  PCIE_DM_TAERCAPCTRL_CAPCTRL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_TAERCAPCTRL_CAPCTRL_MASK) | (((uint32_t)val) << PCIE_DM_TAERCAPCTRL_CAPCTRL_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 00 (LCfg00)
/** \brief Start of the DM Core Local EP Cfg Header Space per PCIe 2.0 Specification for Type 0/1 Headers*/
//====================================================================

#define  PCIE_DM_LCFG00_DEVICEID_MASK 0xffff0000
#define  PCIE_DM_LCFG00_DEVICEID_SHIFT 16
#define  PCIE_DM_LCFG00_DEVICEID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG00_DEVICEID_MASK) >> PCIE_DM_LCFG00_DEVICEID_SHIFT)
#define  PCIE_DM_LCFG00_DEVICEID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG00_DEVICEID_MASK) | (((uint32_t)val) << PCIE_DM_LCFG00_DEVICEID_SHIFT))

#define  PCIE_DM_LCFG00_VENDORID_MASK 0xffff
#define  PCIE_DM_LCFG00_VENDORID_SHIFT 0
#define  PCIE_DM_LCFG00_VENDORID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG00_VENDORID_MASK) >> PCIE_DM_LCFG00_VENDORID_SHIFT)
#define  PCIE_DM_LCFG00_VENDORID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG00_VENDORID_MASK) | (((uint32_t)val) << PCIE_DM_LCFG00_VENDORID_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 04 (LCfg04)
/** \brief Status and Command Registers for Type 0/1 Header*/
//====================================================================

#define  PCIE_DM_LCFG04_STATUS_MASK 0xffff0000
#define  PCIE_DM_LCFG04_STATUS_SHIFT 16
#define  PCIE_DM_LCFG04_STATUS_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG04_STATUS_MASK) >> PCIE_DM_LCFG04_STATUS_SHIFT)
#define  PCIE_DM_LCFG04_STATUS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG04_STATUS_MASK) | (((uint32_t)val) << PCIE_DM_LCFG04_STATUS_SHIFT))

#define  PCIE_DM_LCFG04_COMMAND_MASK 0xffff
#define  PCIE_DM_LCFG04_COMMAND_SHIFT 0
#define  PCIE_DM_LCFG04_COMMAND_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG04_COMMAND_MASK) >> PCIE_DM_LCFG04_COMMAND_SHIFT)
#define  PCIE_DM_LCFG04_COMMAND_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG04_COMMAND_MASK) | (((uint32_t)val) << PCIE_DM_LCFG04_COMMAND_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 08 (LCfg08)
/** \brief Class Code and Revision ID for Type 0/1 Header*/
//====================================================================

#define  PCIE_DM_LCFG08_CLASSCODE_MASK 0xffffff00
#define  PCIE_DM_LCFG08_CLASSCODE_SHIFT 8
#define  PCIE_DM_LCFG08_CLASSCODE_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG08_CLASSCODE_MASK) >> PCIE_DM_LCFG08_CLASSCODE_SHIFT)
#define  PCIE_DM_LCFG08_CLASSCODE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG08_CLASSCODE_MASK) | (((uint32_t)val) << PCIE_DM_LCFG08_CLASSCODE_SHIFT))

#define  PCIE_DM_LCFG08_REVID_MASK 0xff
#define  PCIE_DM_LCFG08_REVID_SHIFT 0
#define  PCIE_DM_LCFG08_REVID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG08_REVID_MASK) >> PCIE_DM_LCFG08_REVID_SHIFT)
#define  PCIE_DM_LCFG08_REVID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG08_REVID_MASK) | (((uint32_t)val) << PCIE_DM_LCFG08_REVID_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 0C (LCfg0C)
/** \brief BIST, Header Type, Latency Timer and Cacheline Size for Type 0/1 Header*/
//====================================================================

#define  PCIE_DM_LCFG0C_BIST_MASK 0xff000000
#define  PCIE_DM_LCFG0C_BIST_SHIFT 24
#define  PCIE_DM_LCFG0C_BIST_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG0C_BIST_MASK) >> PCIE_DM_LCFG0C_BIST_SHIFT)
#define  PCIE_DM_LCFG0C_BIST_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG0C_BIST_MASK) | (((uint32_t)val) << PCIE_DM_LCFG0C_BIST_SHIFT))

#define  PCIE_DM_LCFG0C_HDRTYPE_MASK 0xff0000
#define  PCIE_DM_LCFG0C_HDRTYPE_SHIFT 16
#define  PCIE_DM_LCFG0C_HDRTYPE_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG0C_HDRTYPE_MASK) >> PCIE_DM_LCFG0C_HDRTYPE_SHIFT)
#define  PCIE_DM_LCFG0C_HDRTYPE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG0C_HDRTYPE_MASK) | (((uint32_t)val) << PCIE_DM_LCFG0C_HDRTYPE_SHIFT))

#define  PCIE_DM_LCFG0C_LATTIMER_MASK 0xff00
#define  PCIE_DM_LCFG0C_LATTIMER_SHIFT 8
#define  PCIE_DM_LCFG0C_LATTIMER_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG0C_LATTIMER_MASK) >> PCIE_DM_LCFG0C_LATTIMER_SHIFT)
#define  PCIE_DM_LCFG0C_LATTIMER_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG0C_LATTIMER_MASK) | (((uint32_t)val) << PCIE_DM_LCFG0C_LATTIMER_SHIFT))

#define  PCIE_DM_LCFG0C_CACHELINESIZE_MASK 0xff
#define  PCIE_DM_LCFG0C_CACHELINESIZE_SHIFT 0
#define  PCIE_DM_LCFG0C_CACHELINESIZE_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG0C_CACHELINESIZE_MASK) >> PCIE_DM_LCFG0C_CACHELINESIZE_SHIFT)
#define  PCIE_DM_LCFG0C_CACHELINESIZE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG0C_CACHELINESIZE_MASK) | (((uint32_t)val) << PCIE_DM_LCFG0C_CACHELINESIZE_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 10 is BAR0 (LCfg10)
/** \brief BAR0 Register*/
//====================================================================

#define  PCIE_DM_LCFG10_BAR0_MASK 0xffffffff
#define  PCIE_DM_LCFG10_BAR0_SHIFT 0
#define  PCIE_DM_LCFG10_BAR0_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG10_BAR0_MASK) >> PCIE_DM_LCFG10_BAR0_SHIFT)
#define  PCIE_DM_LCFG10_BAR0_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG10_BAR0_MASK) | (((uint32_t)val) << PCIE_DM_LCFG10_BAR0_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 14 is BAR1 (LCfg14)
/** \brief BAR0 Register*/
//====================================================================

#define  PCIE_DM_LCFG14_BAR1_MASK 0xffffffff
#define  PCIE_DM_LCFG14_BAR1_SHIFT 0
#define  PCIE_DM_LCFG14_BAR1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG14_BAR1_MASK) >> PCIE_DM_LCFG14_BAR1_SHIFT)
#define  PCIE_DM_LCFG14_BAR1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG14_BAR1_MASK) | (((uint32_t)val) << PCIE_DM_LCFG14_BAR1_SHIFT))

//====================================================================
//Register: DM Core Local Configuration BAR0 Mask Register (LCfg10M)
/** \brief BAR0 Mask Register*/
//====================================================================

#define  PCIE_DM_LCFG10M_BAR0MASK_MASK 0xffffffff
#define  PCIE_DM_LCFG10M_BAR0MASK_SHIFT 0
#define  PCIE_DM_LCFG10M_BAR0MASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG10M_BAR0MASK_MASK) >> PCIE_DM_LCFG10M_BAR0MASK_SHIFT)
#define  PCIE_DM_LCFG10M_BAR0MASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG10M_BAR0MASK_MASK) | (((uint32_t)val) << PCIE_DM_LCFG10M_BAR0MASK_SHIFT))

//====================================================================
//Register: DM Core Local Configuration BAR1 Mask Register (LCfg14M)
/** \brief BAR1 Mask Register*/
//====================================================================

#define  PCIE_DM_LCFG14M_BAR1MASK_MASK 0xffffffff
#define  PCIE_DM_LCFG14M_BAR1MASK_SHIFT 0
#define  PCIE_DM_LCFG14M_BAR1MASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG14M_BAR1MASK_MASK) >> PCIE_DM_LCFG14M_BAR1MASK_SHIFT)
#define  PCIE_DM_LCFG14M_BAR1MASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG14M_BAR1MASK_MASK) | (((uint32_t)val) << PCIE_DM_LCFG14M_BAR1MASK_SHIFT))

//====================================================================
//Register: Prefetchable Memory Base and Limit Register (LCfg24)
/** \brief Used only in RC Mode. Sets the Memory range that encompasses all the EP address ranges.
Any address outside the range is allowed for inbound packets.
Addresses inside the Memory Base and Limit range are rejected.*/
//====================================================================

#define  PCIE_DM_LCFG24_ENDADDR_MASK 0xfff00000
#define  PCIE_DM_LCFG24_ENDADDR_SHIFT 20
#define  PCIE_DM_LCFG24_ENDADDR_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG24_ENDADDR_MASK) >> PCIE_DM_LCFG24_ENDADDR_SHIFT)
#define  PCIE_DM_LCFG24_ENDADDR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG24_ENDADDR_MASK) | (((uint32_t)val) << PCIE_DM_LCFG24_ENDADDR_SHIFT))

#define  PCIE_DM_LCFG24_RESERVED1_MASK 0xf0000
#define  PCIE_DM_LCFG24_RESERVED1_SHIFT 16
#define  PCIE_DM_LCFG24_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG24_RESERVED1_MASK) >> PCIE_DM_LCFG24_RESERVED1_SHIFT)
#define  PCIE_DM_LCFG24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG24_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LCFG24_RESERVED1_SHIFT))

#define  PCIE_DM_LCFG24_STARTADDR_MASK 0xfff0
#define  PCIE_DM_LCFG24_STARTADDR_SHIFT 4
#define  PCIE_DM_LCFG24_STARTADDR_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG24_STARTADDR_MASK) >> PCIE_DM_LCFG24_STARTADDR_SHIFT)
#define  PCIE_DM_LCFG24_STARTADDR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG24_STARTADDR_MASK) | (((uint32_t)val) << PCIE_DM_LCFG24_STARTADDR_SHIFT))

#define  PCIE_DM_LCFG24_RESERVED2_MASK 0xf
#define  PCIE_DM_LCFG24_RESERVED2_SHIFT 0
#define  PCIE_DM_LCFG24_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG24_RESERVED2_MASK) >> PCIE_DM_LCFG24_RESERVED2_SHIFT)
#define  PCIE_DM_LCFG24_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG24_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_LCFG24_RESERVED2_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 2C, Subsystem ID and Subsystem Vendor ID (LCfg2C)
/** \brief Subsystem ID/Vendor ID - for Type 0 Headers only. Type 1 Header uses other definition but is not used for this implementation.*/
//====================================================================

#define  PCIE_DM_LCFG2C_SUBSYSID_MASK 0xffff0000
#define  PCIE_DM_LCFG2C_SUBSYSID_SHIFT 16
#define  PCIE_DM_LCFG2C_SUBSYSID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG2C_SUBSYSID_MASK) >> PCIE_DM_LCFG2C_SUBSYSID_SHIFT)
#define  PCIE_DM_LCFG2C_SUBSYSID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG2C_SUBSYSID_MASK) | (((uint32_t)val) << PCIE_DM_LCFG2C_SUBSYSID_SHIFT))

#define  PCIE_DM_LCFG2C_SUBSYSVID_MASK 0xffff
#define  PCIE_DM_LCFG2C_SUBSYSVID_SHIFT 0
#define  PCIE_DM_LCFG2C_SUBSYSVID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG2C_SUBSYSVID_MASK) >> PCIE_DM_LCFG2C_SUBSYSVID_SHIFT)
#define  PCIE_DM_LCFG2C_SUBSYSVID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG2C_SUBSYSVID_MASK) | (((uint32_t)val) << PCIE_DM_LCFG2C_SUBSYSVID_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 34 (LCfg34)
/** \brief PCIe Cfg Header Capabilities Pointer - contents point to Capabilities Registers.
Must be > 0x3F to be outside basic Type 0/1 Cfg space.*/
//====================================================================

#define  PCIE_DM_LCFG34_RESERVED1_MASK 0xffffff00
#define  PCIE_DM_LCFG34_RESERVED1_SHIFT 8
#define  PCIE_DM_LCFG34_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG34_RESERVED1_MASK) >> PCIE_DM_LCFG34_RESERVED1_SHIFT)
#define  PCIE_DM_LCFG34_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG34_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LCFG34_RESERVED1_SHIFT))

#define  PCIE_DM_LCFG34_CAPABPTR_MASK 0xff
#define  PCIE_DM_LCFG34_CAPABPTR_SHIFT 0
#define  PCIE_DM_LCFG34_CAPABPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG34_CAPABPTR_MASK) >> PCIE_DM_LCFG34_CAPABPTR_SHIFT)
#define  PCIE_DM_LCFG34_CAPABPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG34_CAPABPTR_MASK) | (((uint32_t)val) << PCIE_DM_LCFG34_CAPABPTR_SHIFT))

//====================================================================
//Register: DM Core Local Configuration Register 3C, Interrupt Pin  and Interrupt Line (LCfg3C)
/** \brief Interrupt Pin and Interrupt Line*/
//====================================================================

#define  PCIE_DM_LCFG3C_RESERVED1_MASK 0xffff0000
#define  PCIE_DM_LCFG3C_RESERVED1_SHIFT 16
#define  PCIE_DM_LCFG3C_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG3C_RESERVED1_MASK) >> PCIE_DM_LCFG3C_RESERVED1_SHIFT)
#define  PCIE_DM_LCFG3C_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG3C_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LCFG3C_RESERVED1_SHIFT))

#define  PCIE_DM_LCFG3C_INTPIN_MASK 0xff00
#define  PCIE_DM_LCFG3C_INTPIN_SHIFT 8
#define  PCIE_DM_LCFG3C_INTPIN_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG3C_INTPIN_MASK) >> PCIE_DM_LCFG3C_INTPIN_SHIFT)
#define  PCIE_DM_LCFG3C_INTPIN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG3C_INTPIN_MASK) | (((uint32_t)val) << PCIE_DM_LCFG3C_INTPIN_SHIFT))

#define  PCIE_DM_LCFG3C_INTLINE_MASK 0xff
#define  PCIE_DM_LCFG3C_INTLINE_SHIFT 0
#define  PCIE_DM_LCFG3C_INTLINE_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCFG3C_INTLINE_MASK) >> PCIE_DM_LCFG3C_INTLINE_SHIFT)
#define  PCIE_DM_LCFG3C_INTLINE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCFG3C_INTLINE_MASK) | (((uint32_t)val) << PCIE_DM_LCFG3C_INTLINE_SHIFT))

//====================================================================
//Register: DM Core Local Power Management Capabilities Register (LPMCap)
/** \brief Power Management Capabilities Register.*/
//====================================================================

#define  PCIE_DM_LPMCAP_PMCAP_MASK 0xffff0000
#define  PCIE_DM_LPMCAP_PMCAP_SHIFT 16
#define  PCIE_DM_LPMCAP_PMCAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCAP_PMCAP_MASK) >> PCIE_DM_LPMCAP_PMCAP_SHIFT)
#define  PCIE_DM_LPMCAP_PMCAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCAP_PMCAP_MASK) | (((uint32_t)val) << PCIE_DM_LPMCAP_PMCAP_SHIFT))

#define  PCIE_DM_LPMCAP_NEXTCAPPTR_MASK 0xff00
#define  PCIE_DM_LPMCAP_NEXTCAPPTR_SHIFT 8
#define  PCIE_DM_LPMCAP_NEXTCAPPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCAP_NEXTCAPPTR_MASK) >> PCIE_DM_LPMCAP_NEXTCAPPTR_SHIFT)
#define  PCIE_DM_LPMCAP_NEXTCAPPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCAP_NEXTCAPPTR_MASK) | (((uint32_t)val) << PCIE_DM_LPMCAP_NEXTCAPPTR_SHIFT))

#define  PCIE_DM_LPMCAP_CAPABID_MASK 0xff
#define  PCIE_DM_LPMCAP_CAPABID_SHIFT 0
#define  PCIE_DM_LPMCAP_CAPABID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCAP_CAPABID_MASK) >> PCIE_DM_LPMCAP_CAPABID_SHIFT)
#define  PCIE_DM_LPMCAP_CAPABID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCAP_CAPABID_MASK) | (((uint32_t)val) << PCIE_DM_LPMCAP_CAPABID_SHIFT))

//====================================================================
//Register: DM Core Local Power Management Control and Status Register (LPMCSR)
/** \brief Power Management Control and Status Register.*/
//====================================================================

#define  PCIE_DM_LPMCSR_RESERVED1_MASK 0xffff0000
#define  PCIE_DM_LPMCSR_RESERVED1_SHIFT 16
#define  PCIE_DM_LPMCSR_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCSR_RESERVED1_MASK) >> PCIE_DM_LPMCSR_RESERVED1_SHIFT)
#define  PCIE_DM_LPMCSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCSR_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LPMCSR_RESERVED1_SHIFT))

#define  PCIE_DM_LPMCSR_PMESTAT_MASK 0x8000
#define  PCIE_DM_LPMCSR_PMESTAT_SHIFT 15
#define  PCIE_DM_LPMCSR_PMESTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCSR_PMESTAT_MASK) >> PCIE_DM_LPMCSR_PMESTAT_SHIFT)
#define  PCIE_DM_LPMCSR_PMESTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCSR_PMESTAT_MASK) | (((uint32_t)val) << PCIE_DM_LPMCSR_PMESTAT_SHIFT))

#define  PCIE_DM_LPMCSR_RESERVED2_MASK 0x7e00
#define  PCIE_DM_LPMCSR_RESERVED2_SHIFT 9
#define  PCIE_DM_LPMCSR_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCSR_RESERVED2_MASK) >> PCIE_DM_LPMCSR_RESERVED2_SHIFT)
#define  PCIE_DM_LPMCSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCSR_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_LPMCSR_RESERVED2_SHIFT))

#define  PCIE_DM_LPMCSR_PME_EN_MASK 0x100
#define  PCIE_DM_LPMCSR_PME_EN_SHIFT 8
#define  PCIE_DM_LPMCSR_PME_EN_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCSR_PME_EN_MASK) >> PCIE_DM_LPMCSR_PME_EN_SHIFT)
#define  PCIE_DM_LPMCSR_PME_EN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCSR_PME_EN_MASK) | (((uint32_t)val) << PCIE_DM_LPMCSR_PME_EN_SHIFT))

#define  PCIE_DM_LPMCSR_RESERVED3_MASK 0xfc
#define  PCIE_DM_LPMCSR_RESERVED3_SHIFT 2
#define  PCIE_DM_LPMCSR_RESERVED3_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCSR_RESERVED3_MASK) >> PCIE_DM_LPMCSR_RESERVED3_SHIFT)
#define  PCIE_DM_LPMCSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCSR_RESERVED3_MASK) | (((uint32_t)val) << PCIE_DM_LPMCSR_RESERVED3_SHIFT))

#define  PCIE_DM_LPMCSR_PWRSTATE_MASK 0x3
#define  PCIE_DM_LPMCSR_PWRSTATE_SHIFT 0
#define  PCIE_DM_LPMCSR_PWRSTATE_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPMCSR_PWRSTATE_MASK) >> PCIE_DM_LPMCSR_PWRSTATE_SHIFT)
#define  PCIE_DM_LPMCSR_PWRSTATE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPMCSR_PWRSTATE_MASK) | (((uint32_t)val) << PCIE_DM_LPMCSR_PWRSTATE_SHIFT))

//====================================================================
//Register: DM Core Local MSI Capabilities Control Register (LMsiCtrl)
/** \brief MSI Capabilities Control Register.*/
//====================================================================

#define  PCIE_DM_LMSICTRL_MISCCONTROL_MASK 0xfffe0000
#define  PCIE_DM_LMSICTRL_MISCCONTROL_SHIFT 17
#define  PCIE_DM_LMSICTRL_MISCCONTROL_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSICTRL_MISCCONTROL_MASK) >> PCIE_DM_LMSICTRL_MISCCONTROL_SHIFT)
#define  PCIE_DM_LMSICTRL_MISCCONTROL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSICTRL_MISCCONTROL_MASK) | (((uint32_t)val) << PCIE_DM_LMSICTRL_MISCCONTROL_SHIFT))

#define  PCIE_DM_LMSICTRL_MSIEN_MASK 0x10000
#define  PCIE_DM_LMSICTRL_MSIEN_SHIFT 16
#define  PCIE_DM_LMSICTRL_MSIEN_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSICTRL_MSIEN_MASK) >> PCIE_DM_LMSICTRL_MSIEN_SHIFT)
#define  PCIE_DM_LMSICTRL_MSIEN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSICTRL_MSIEN_MASK) | (((uint32_t)val) << PCIE_DM_LMSICTRL_MSIEN_SHIFT))

#define  PCIE_DM_LMSICTRL_NEXTPTR_MASK 0xff00
#define  PCIE_DM_LMSICTRL_NEXTPTR_SHIFT 8
#define  PCIE_DM_LMSICTRL_NEXTPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSICTRL_NEXTPTR_MASK) >> PCIE_DM_LMSICTRL_NEXTPTR_SHIFT)
#define  PCIE_DM_LMSICTRL_NEXTPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSICTRL_NEXTPTR_MASK) | (((uint32_t)val) << PCIE_DM_LMSICTRL_NEXTPTR_SHIFT))

#define  PCIE_DM_LMSICTRL_CAPABID_MASK 0xff
#define  PCIE_DM_LMSICTRL_CAPABID_SHIFT 0
#define  PCIE_DM_LMSICTRL_CAPABID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSICTRL_CAPABID_MASK) >> PCIE_DM_LMSICTRL_CAPABID_SHIFT)
#define  PCIE_DM_LMSICTRL_CAPABID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSICTRL_CAPABID_MASK) | (((uint32_t)val) << PCIE_DM_LMSICTRL_CAPABID_SHIFT))

//====================================================================
//Register: DM Core Local MSI Capabilities  Address Register (LMsiAddr)
/** \brief MSI Capabilities Message Address.*/
//====================================================================

#define  PCIE_DM_LMSIADDR_ADDRESS_MASK 0xffffffff
#define  PCIE_DM_LMSIADDR_ADDRESS_SHIFT 0
#define  PCIE_DM_LMSIADDR_ADDRESS_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSIADDR_ADDRESS_MASK) >> PCIE_DM_LMSIADDR_ADDRESS_SHIFT)
#define  PCIE_DM_LMSIADDR_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSIADDR_ADDRESS_MASK) | (((uint32_t)val) << PCIE_DM_LMSIADDR_ADDRESS_SHIFT))

//====================================================================
//Register: DM Core Local MSI Capabilities Data  Register (LMsiData)
/** \brief MSI Capabilities Message Data*/
//====================================================================

#define  PCIE_DM_LMSIDATA_RESERVED1_MASK 0xffff0000
#define  PCIE_DM_LMSIDATA_RESERVED1_SHIFT 16
#define  PCIE_DM_LMSIDATA_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSIDATA_RESERVED1_MASK) >> PCIE_DM_LMSIDATA_RESERVED1_SHIFT)
#define  PCIE_DM_LMSIDATA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSIDATA_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LMSIDATA_RESERVED1_SHIFT))

#define  PCIE_DM_LMSIDATA_DATA_MASK 0xffff
#define  PCIE_DM_LMSIDATA_DATA_SHIFT 0
#define  PCIE_DM_LMSIDATA_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSIDATA_DATA_MASK) >> PCIE_DM_LMSIDATA_DATA_SHIFT)
#define  PCIE_DM_LMSIDATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSIDATA_DATA_MASK) | (((uint32_t)val) << PCIE_DM_LMSIDATA_DATA_SHIFT))

//====================================================================
//Register: DM Core Local PCIE Capabilities Register (LPCIECap)
/** \brief PCI Express Capabilities Register.*/
//====================================================================

#define  PCIE_DM_LPCIECAP_PCIECAP_MASK 0xffff0000
#define  PCIE_DM_LPCIECAP_PCIECAP_SHIFT 16
#define  PCIE_DM_LPCIECAP_PCIECAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPCIECAP_PCIECAP_MASK) >> PCIE_DM_LPCIECAP_PCIECAP_SHIFT)
#define  PCIE_DM_LPCIECAP_PCIECAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPCIECAP_PCIECAP_MASK) | (((uint32_t)val) << PCIE_DM_LPCIECAP_PCIECAP_SHIFT))

#define  PCIE_DM_LPCIECAP_NEXTCAPPTR_MASK 0xff00
#define  PCIE_DM_LPCIECAP_NEXTCAPPTR_SHIFT 8
#define  PCIE_DM_LPCIECAP_NEXTCAPPTR_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPCIECAP_NEXTCAPPTR_MASK) >> PCIE_DM_LPCIECAP_NEXTCAPPTR_SHIFT)
#define  PCIE_DM_LPCIECAP_NEXTCAPPTR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPCIECAP_NEXTCAPPTR_MASK) | (((uint32_t)val) << PCIE_DM_LPCIECAP_NEXTCAPPTR_SHIFT))

#define  PCIE_DM_LPCIECAP_CAPABID_MASK 0xff
#define  PCIE_DM_LPCIECAP_CAPABID_SHIFT 0
#define  PCIE_DM_LPCIECAP_CAPABID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPCIECAP_CAPABID_MASK) >> PCIE_DM_LPCIECAP_CAPABID_SHIFT)
#define  PCIE_DM_LPCIECAP_CAPABID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPCIECAP_CAPABID_MASK) | (((uint32_t)val) << PCIE_DM_LPCIECAP_CAPABID_SHIFT))

//====================================================================
//Register: DM Core Local Device Capability Register (LDevCap)
/** \brief Device Capability Register.*/
//====================================================================

#define  PCIE_DM_LDEVCAP_DEVCAP_MASK 0xffffffff
#define  PCIE_DM_LDEVCAP_DEVCAP_SHIFT 0
#define  PCIE_DM_LDEVCAP_DEVCAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVCAP_DEVCAP_MASK) >> PCIE_DM_LDEVCAP_DEVCAP_SHIFT)
#define  PCIE_DM_LDEVCAP_DEVCAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVCAP_DEVCAP_MASK) | (((uint32_t)val) << PCIE_DM_LDEVCAP_DEVCAP_SHIFT))

//====================================================================
//Register: DM Core Local Device Control and Status Register (LDevContStatus)
/** \brief PCI Express Capability Device Control and Status Register.*/
//====================================================================

#define  PCIE_DM_LDEVCONTSTATUS_STATUS_MASK 0xffff0000
#define  PCIE_DM_LDEVCONTSTATUS_STATUS_SHIFT 16
#define  PCIE_DM_LDEVCONTSTATUS_STATUS_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVCONTSTATUS_STATUS_MASK) >> PCIE_DM_LDEVCONTSTATUS_STATUS_SHIFT)
#define  PCIE_DM_LDEVCONTSTATUS_STATUS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVCONTSTATUS_STATUS_MASK) | (((uint32_t)val) << PCIE_DM_LDEVCONTSTATUS_STATUS_SHIFT))

#define  PCIE_DM_LDEVCONTSTATUS_CONTROL_MASK 0xffff
#define  PCIE_DM_LDEVCONTSTATUS_CONTROL_SHIFT 0
#define  PCIE_DM_LDEVCONTSTATUS_CONTROL_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVCONTSTATUS_CONTROL_MASK) >> PCIE_DM_LDEVCONTSTATUS_CONTROL_SHIFT)
#define  PCIE_DM_LDEVCONTSTATUS_CONTROL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVCONTSTATUS_CONTROL_MASK) | (((uint32_t)val) << PCIE_DM_LDEVCONTSTATUS_CONTROL_SHIFT))

//====================================================================
//Register: DM Core Local Device Link Capabilities Register (LDevLinkCap)
/** \brief PCI Express Link Capabilities Register.*/
//====================================================================

#define  PCIE_DM_LDEVLINKCAP_DEVLINKCAP_MASK 0xffffffff
#define  PCIE_DM_LDEVLINKCAP_DEVLINKCAP_SHIFT 0
#define  PCIE_DM_LDEVLINKCAP_DEVLINKCAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVLINKCAP_DEVLINKCAP_MASK) >> PCIE_DM_LDEVLINKCAP_DEVLINKCAP_SHIFT)
#define  PCIE_DM_LDEVLINKCAP_DEVLINKCAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVLINKCAP_DEVLINKCAP_MASK) | (((uint32_t)val) << PCIE_DM_LDEVLINKCAP_DEVLINKCAP_SHIFT))

//====================================================================
//Register: DM Core Local Link Control and Status Register (LLinkContStatus)
/** \brief PCI Express Capability Device Control and Status Register.*/
//====================================================================

#define  PCIE_DM_LLINKCONTSTATUS_STATUS_MASK 0xffff0000
#define  PCIE_DM_LLINKCONTSTATUS_STATUS_SHIFT 16
#define  PCIE_DM_LLINKCONTSTATUS_STATUS_MASK_SHIFT(reg) (((reg) & PCIE_DM_LLINKCONTSTATUS_STATUS_MASK) >> PCIE_DM_LLINKCONTSTATUS_STATUS_SHIFT)
#define  PCIE_DM_LLINKCONTSTATUS_STATUS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LLINKCONTSTATUS_STATUS_MASK) | (((uint32_t)val) << PCIE_DM_LLINKCONTSTATUS_STATUS_SHIFT))

#define  PCIE_DM_LLINKCONTSTATUS_CONTROL_MASK 0xffff
#define  PCIE_DM_LLINKCONTSTATUS_CONTROL_SHIFT 0
#define  PCIE_DM_LLINKCONTSTATUS_CONTROL_MASK_SHIFT(reg) (((reg) & PCIE_DM_LLINKCONTSTATUS_CONTROL_MASK) >> PCIE_DM_LLINKCONTSTATUS_CONTROL_SHIFT)
#define  PCIE_DM_LLINKCONTSTATUS_CONTROL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LLINKCONTSTATUS_CONTROL_MASK) | (((uint32_t)val) << PCIE_DM_LLINKCONTSTATUS_CONTROL_SHIFT))

//====================================================================
//Register: DM Core Local Root Control Register (LRootCtrl)
/** \brief PCI Express Root Control Register - RC only.*/
//====================================================================

#define  PCIE_DM_LROOTCTRL_RESERVED1_MASK 0xffff0000
#define  PCIE_DM_LROOTCTRL_RESERVED1_SHIFT 16
#define  PCIE_DM_LROOTCTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTCTRL_RESERVED1_MASK) >> PCIE_DM_LROOTCTRL_RESERVED1_SHIFT)
#define  PCIE_DM_LROOTCTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTCTRL_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LROOTCTRL_RESERVED1_SHIFT))

#define  PCIE_DM_LROOTCTRL_ROOTCTRL_MASK 0xffff
#define  PCIE_DM_LROOTCTRL_ROOTCTRL_SHIFT 0
#define  PCIE_DM_LROOTCTRL_ROOTCTRL_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTCTRL_ROOTCTRL_MASK) >> PCIE_DM_LROOTCTRL_ROOTCTRL_SHIFT)
#define  PCIE_DM_LROOTCTRL_ROOTCTRL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTCTRL_ROOTCTRL_MASK) | (((uint32_t)val) << PCIE_DM_LROOTCTRL_ROOTCTRL_SHIFT))

//====================================================================
//Register: DM Core Local Root Status Register (LRootStat)
/** \brief PCI Express Root Status Register - RC only.*/
//====================================================================

#define  PCIE_DM_LROOTSTAT_RESERVED1_MASK 0xfffc0000
#define  PCIE_DM_LROOTSTAT_RESERVED1_SHIFT 18
#define  PCIE_DM_LROOTSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTSTAT_RESERVED1_MASK) >> PCIE_DM_LROOTSTAT_RESERVED1_SHIFT)
#define  PCIE_DM_LROOTSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTSTAT_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LROOTSTAT_RESERVED1_SHIFT))

#define  PCIE_DM_LROOTSTAT_ROOTSTAT_MASK 0x3ffff
#define  PCIE_DM_LROOTSTAT_ROOTSTAT_SHIFT 0
#define  PCIE_DM_LROOTSTAT_ROOTSTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTSTAT_ROOTSTAT_MASK) >> PCIE_DM_LROOTSTAT_ROOTSTAT_SHIFT)
#define  PCIE_DM_LROOTSTAT_ROOTSTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTSTAT_ROOTSTAT_MASK) | (((uint32_t)val) << PCIE_DM_LROOTSTAT_ROOTSTAT_SHIFT))

//====================================================================
//Register: DM Core Local Device Capability2 Register (LDevCap2)
/** \brief Device Capability2 Register.*/
//====================================================================

#define  PCIE_DM_LDEVCAP2_DEVCAP2_MASK 0xffffffff
#define  PCIE_DM_LDEVCAP2_DEVCAP2_SHIFT 0
#define  PCIE_DM_LDEVCAP2_DEVCAP2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVCAP2_DEVCAP2_MASK) >> PCIE_DM_LDEVCAP2_DEVCAP2_SHIFT)
#define  PCIE_DM_LDEVCAP2_DEVCAP2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVCAP2_DEVCAP2_MASK) | (((uint32_t)val) << PCIE_DM_LDEVCAP2_DEVCAP2_SHIFT))

//====================================================================
//Register: DM Core Local Device Control and Status2 Register (LDevContStatus2)
/** \brief PCI Express Capability Device Control and Status2 Register.*/
//====================================================================

#define  PCIE_DM_LDEVCONTSTATUS2_STATUS2_MASK 0xffff0000
#define  PCIE_DM_LDEVCONTSTATUS2_STATUS2_SHIFT 16
#define  PCIE_DM_LDEVCONTSTATUS2_STATUS2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVCONTSTATUS2_STATUS2_MASK) >> PCIE_DM_LDEVCONTSTATUS2_STATUS2_SHIFT)
#define  PCIE_DM_LDEVCONTSTATUS2_STATUS2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVCONTSTATUS2_STATUS2_MASK) | (((uint32_t)val) << PCIE_DM_LDEVCONTSTATUS2_STATUS2_SHIFT))

#define  PCIE_DM_LDEVCONTSTATUS2_CONTROL2_MASK 0xffff
#define  PCIE_DM_LDEVCONTSTATUS2_CONTROL2_SHIFT 0
#define  PCIE_DM_LDEVCONTSTATUS2_CONTROL2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVCONTSTATUS2_CONTROL2_MASK) >> PCIE_DM_LDEVCONTSTATUS2_CONTROL2_SHIFT)
#define  PCIE_DM_LDEVCONTSTATUS2_CONTROL2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVCONTSTATUS2_CONTROL2_MASK) | (((uint32_t)val) << PCIE_DM_LDEVCONTSTATUS2_CONTROL2_SHIFT))

//====================================================================
//Register: DM Core Local Device Link Capabilities2 Register (LDevLinkCap2)
/** \brief PCI Express Link Capabilities2 Register.*/
//====================================================================

#define  PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_MASK 0xffffffff
#define  PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_SHIFT 0
#define  PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_MASK) >> PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_SHIFT)
#define  PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_MASK) | (((uint32_t)val) << PCIE_DM_LDEVLINKCAP2_DEVLINKCAP2_SHIFT))

//====================================================================
//Register: DM Core Local Link Control and Status2 Register (LLinkContStatus2)
/** \brief PCI Express Capability Device Control and Status2 Register.*/
//====================================================================

#define  PCIE_DM_LLINKCONTSTATUS2_STATUS2_MASK 0xffff0000
#define  PCIE_DM_LLINKCONTSTATUS2_STATUS2_SHIFT 16
#define  PCIE_DM_LLINKCONTSTATUS2_STATUS2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LLINKCONTSTATUS2_STATUS2_MASK) >> PCIE_DM_LLINKCONTSTATUS2_STATUS2_SHIFT)
#define  PCIE_DM_LLINKCONTSTATUS2_STATUS2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LLINKCONTSTATUS2_STATUS2_MASK) | (((uint32_t)val) << PCIE_DM_LLINKCONTSTATUS2_STATUS2_SHIFT))

#define  PCIE_DM_LLINKCONTSTATUS2_CONTROL2_MASK 0xffff
#define  PCIE_DM_LLINKCONTSTATUS2_CONTROL2_SHIFT 0
#define  PCIE_DM_LLINKCONTSTATUS2_CONTROL2_MASK_SHIFT(reg) (((reg) & PCIE_DM_LLINKCONTSTATUS2_CONTROL2_MASK) >> PCIE_DM_LLINKCONTSTATUS2_CONTROL2_SHIFT)
#define  PCIE_DM_LLINKCONTSTATUS2_CONTROL2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LLINKCONTSTATUS2_CONTROL2_MASK) | (((uint32_t)val) << PCIE_DM_LLINKCONTSTATUS2_CONTROL2_SHIFT))

//====================================================================
//Register: DM Core Local AER Capabilities Register (LAERCap)
/** \brief PCI Express AER Enhanced Capability Header Register.*/
//====================================================================

#define  PCIE_DM_LAERCAP_AERHDR_MASK 0xffffffff
#define  PCIE_DM_LAERCAP_AERHDR_SHIFT 0
#define  PCIE_DM_LAERCAP_AERHDR_MASK_SHIFT(reg) (((reg) & PCIE_DM_LAERCAP_AERHDR_MASK) >> PCIE_DM_LAERCAP_AERHDR_SHIFT)
#define  PCIE_DM_LAERCAP_AERHDR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LAERCAP_AERHDR_MASK) | (((uint32_t)val) << PCIE_DM_LAERCAP_AERHDR_SHIFT))

//====================================================================
//Register: DM Core Local Uncorrectable Error Status Register (LUCorrStat)
/** \brief PCI Express AER Uncorrectable Error Status Register.*/
//====================================================================

#define  PCIE_DM_LUCORRSTAT_UCORRESTAT_MASK 0xffffffff
#define  PCIE_DM_LUCORRSTAT_UCORRESTAT_SHIFT 0
#define  PCIE_DM_LUCORRSTAT_UCORRESTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_LUCORRSTAT_UCORRESTAT_MASK) >> PCIE_DM_LUCORRSTAT_UCORRESTAT_SHIFT)
#define  PCIE_DM_LUCORRSTAT_UCORRESTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LUCORRSTAT_UCORRESTAT_MASK) | (((uint32_t)val) << PCIE_DM_LUCORRSTAT_UCORRESTAT_SHIFT))

//====================================================================
//Register: DM Core Local Uncorrectable Error Mask Register (LUCorrMask)
/** \brief PCI Express AER Uncorrectable Error Mask Register.*/
//====================================================================

#define  PCIE_DM_LUCORRMASK_UCORREMASK_MASK 0xffffffff
#define  PCIE_DM_LUCORRMASK_UCORREMASK_SHIFT 0
#define  PCIE_DM_LUCORRMASK_UCORREMASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_LUCORRMASK_UCORREMASK_MASK) >> PCIE_DM_LUCORRMASK_UCORREMASK_SHIFT)
#define  PCIE_DM_LUCORRMASK_UCORREMASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LUCORRMASK_UCORREMASK_MASK) | (((uint32_t)val) << PCIE_DM_LUCORRMASK_UCORREMASK_SHIFT))

//====================================================================
//Register: DM Core Local Uncorrectable Error Severity Register (LUCorrSev)
/** \brief PCI Express AER Uncorrectable Error Severity Register.*/
//====================================================================

#define  PCIE_DM_LUCORRSEV_UCORRESEV_MASK 0xffffffff
#define  PCIE_DM_LUCORRSEV_UCORRESEV_SHIFT 0
#define  PCIE_DM_LUCORRSEV_UCORRESEV_MASK_SHIFT(reg) (((reg) & PCIE_DM_LUCORRSEV_UCORRESEV_MASK) >> PCIE_DM_LUCORRSEV_UCORRESEV_SHIFT)
#define  PCIE_DM_LUCORRSEV_UCORRESEV_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LUCORRSEV_UCORRESEV_MASK) | (((uint32_t)val) << PCIE_DM_LUCORRSEV_UCORRESEV_SHIFT))

//====================================================================
//Register: DM Core Local Correctable Error Status Register (LCorrStat)
/** \brief PCI Express AER Correctable Error Status Register.*/
//====================================================================

#define  PCIE_DM_LCORRSTAT_CORRESTAT_MASK 0xffffffff
#define  PCIE_DM_LCORRSTAT_CORRESTAT_SHIFT 0
#define  PCIE_DM_LCORRSTAT_CORRESTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCORRSTAT_CORRESTAT_MASK) >> PCIE_DM_LCORRSTAT_CORRESTAT_SHIFT)
#define  PCIE_DM_LCORRSTAT_CORRESTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCORRSTAT_CORRESTAT_MASK) | (((uint32_t)val) << PCIE_DM_LCORRSTAT_CORRESTAT_SHIFT))

//====================================================================
//Register: DM Core Local Correctable Error Mask Register (LCorrMask)
/** \brief PCI Express AER Correctable Error Mask Register.*/
//====================================================================

#define  PCIE_DM_LCORRMASK_CORREMASK_MASK 0xffffffff
#define  PCIE_DM_LCORRMASK_CORREMASK_SHIFT 0
#define  PCIE_DM_LCORRMASK_CORREMASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_LCORRMASK_CORREMASK_MASK) >> PCIE_DM_LCORRMASK_CORREMASK_SHIFT)
#define  PCIE_DM_LCORRMASK_CORREMASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LCORRMASK_CORREMASK_MASK) | (((uint32_t)val) << PCIE_DM_LCORRMASK_CORREMASK_SHIFT))

//====================================================================
//Register: DM Core Local AER Capabilities and Control Register (LAERCapCtrl)
/** \brief PCI Express AER Capabilities and Control Register.*/
//====================================================================

#define  PCIE_DM_LAERCAPCTRL_CAPCTRL_MASK 0xffffffff
#define  PCIE_DM_LAERCAPCTRL_CAPCTRL_SHIFT 0
#define  PCIE_DM_LAERCAPCTRL_CAPCTRL_MASK_SHIFT(reg) (((reg) & PCIE_DM_LAERCAPCTRL_CAPCTRL_MASK) >> PCIE_DM_LAERCAPCTRL_CAPCTRL_SHIFT)
#define  PCIE_DM_LAERCAPCTRL_CAPCTRL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LAERCAPCTRL_CAPCTRL_MASK) | (((uint32_t)val) << PCIE_DM_LAERCAPCTRL_CAPCTRL_SHIFT))

//====================================================================
//Register: DM Core Local Root Error Command Register (LRootErrCmd)
/** \brief PCI Express Root Error Command Register - RC only.*/
//====================================================================

#define  PCIE_DM_LROOTERRCMD_RESERVED1_MASK 0xfffffff8
#define  PCIE_DM_LROOTERRCMD_RESERVED1_SHIFT 3
#define  PCIE_DM_LROOTERRCMD_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTERRCMD_RESERVED1_MASK) >> PCIE_DM_LROOTERRCMD_RESERVED1_SHIFT)
#define  PCIE_DM_LROOTERRCMD_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTERRCMD_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LROOTERRCMD_RESERVED1_SHIFT))

#define  PCIE_DM_LROOTERRCMD_ERRCMD_MASK 0x7
#define  PCIE_DM_LROOTERRCMD_ERRCMD_SHIFT 0
#define  PCIE_DM_LROOTERRCMD_ERRCMD_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTERRCMD_ERRCMD_MASK) >> PCIE_DM_LROOTERRCMD_ERRCMD_SHIFT)
#define  PCIE_DM_LROOTERRCMD_ERRCMD_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTERRCMD_ERRCMD_MASK) | (((uint32_t)val) << PCIE_DM_LROOTERRCMD_ERRCMD_SHIFT))

//====================================================================
//Register: DM Core Local Root Error Status Register (LRootErrStat)
/** \brief PCI Express Root Error Status Register - RC only.*/
//====================================================================

#define  PCIE_DM_LROOTERRSTAT_MSGNUM_MASK 0xf8000000
#define  PCIE_DM_LROOTERRSTAT_MSGNUM_SHIFT 27
#define  PCIE_DM_LROOTERRSTAT_MSGNUM_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTERRSTAT_MSGNUM_MASK) >> PCIE_DM_LROOTERRSTAT_MSGNUM_SHIFT)
#define  PCIE_DM_LROOTERRSTAT_MSGNUM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTERRSTAT_MSGNUM_MASK) | (((uint32_t)val) << PCIE_DM_LROOTERRSTAT_MSGNUM_SHIFT))

#define  PCIE_DM_LROOTERRSTAT_RESERVED1_MASK 0x7ffff80
#define  PCIE_DM_LROOTERRSTAT_RESERVED1_SHIFT 7
#define  PCIE_DM_LROOTERRSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTERRSTAT_RESERVED1_MASK) >> PCIE_DM_LROOTERRSTAT_RESERVED1_SHIFT)
#define  PCIE_DM_LROOTERRSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTERRSTAT_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LROOTERRSTAT_RESERVED1_SHIFT))

#define  PCIE_DM_LROOTERRSTAT_ERRSTAT_MASK 0x7f
#define  PCIE_DM_LROOTERRSTAT_ERRSTAT_SHIFT 0
#define  PCIE_DM_LROOTERRSTAT_ERRSTAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_LROOTERRSTAT_ERRSTAT_MASK) >> PCIE_DM_LROOTERRSTAT_ERRSTAT_SHIFT)
#define  PCIE_DM_LROOTERRSTAT_ERRSTAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LROOTERRSTAT_ERRSTAT_MASK) | (((uint32_t)val) << PCIE_DM_LROOTERRSTAT_ERRSTAT_SHIFT))

//====================================================================
//Register: DM Core Local Error Source Identification Register (LErrSrcID)
/** \brief PCI Express Error Source Identification Register - RC only.*/
//====================================================================

#define  PCIE_DM_LERRSRCID_FATALID_MASK 0xffff0000
#define  PCIE_DM_LERRSRCID_FATALID_SHIFT 16
#define  PCIE_DM_LERRSRCID_FATALID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LERRSRCID_FATALID_MASK) >> PCIE_DM_LERRSRCID_FATALID_SHIFT)
#define  PCIE_DM_LERRSRCID_FATALID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LERRSRCID_FATALID_MASK) | (((uint32_t)val) << PCIE_DM_LERRSRCID_FATALID_SHIFT))

#define  PCIE_DM_LERRSRCID_CORRID_MASK 0xffff
#define  PCIE_DM_LERRSRCID_CORRID_SHIFT 0
#define  PCIE_DM_LERRSRCID_CORRID_MASK_SHIFT(reg) (((reg) & PCIE_DM_LERRSRCID_CORRID_MASK) >> PCIE_DM_LERRSRCID_CORRID_SHIFT)
#define  PCIE_DM_LERRSRCID_CORRID_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LERRSRCID_CORRID_MASK) | (((uint32_t)val) << PCIE_DM_LERRSRCID_CORRID_SHIFT))

//====================================================================
//Register: DM Core Local Port Logic Ack Latency and Replay Timer Register (LPortTimer)
/** \brief Port Logic Ack Latency and Replay Timer Register.
NOTE: This is the start of the Port Logic Registers, primarily used for changing default latencies and debug/test.
See Synopsys DM Core Reference Manual page 300 for detailed register and field descriptions.*/
//====================================================================

#define  PCIE_DM_LPORTTIMER_REPLAYTIMER_MASK 0xffff0000
#define  PCIE_DM_LPORTTIMER_REPLAYTIMER_SHIFT 16
#define  PCIE_DM_LPORTTIMER_REPLAYTIMER_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPORTTIMER_REPLAYTIMER_MASK) >> PCIE_DM_LPORTTIMER_REPLAYTIMER_SHIFT)
#define  PCIE_DM_LPORTTIMER_REPLAYTIMER_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPORTTIMER_REPLAYTIMER_MASK) | (((uint32_t)val) << PCIE_DM_LPORTTIMER_REPLAYTIMER_SHIFT))

#define  PCIE_DM_LPORTTIMER_ACKTIMER_MASK 0xffff
#define  PCIE_DM_LPORTTIMER_ACKTIMER_SHIFT 0
#define  PCIE_DM_LPORTTIMER_ACKTIMER_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPORTTIMER_ACKTIMER_MASK) >> PCIE_DM_LPORTTIMER_ACKTIMER_SHIFT)
#define  PCIE_DM_LPORTTIMER_ACKTIMER_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPORTTIMER_ACKTIMER_MASK) | (((uint32_t)val) << PCIE_DM_LPORTTIMER_ACKTIMER_SHIFT))

//====================================================================
//Register: DM Core Local Port Logic Message Register (LMsgReg)
/** \brief Port Logic Message Register.*/
//====================================================================

#define  PCIE_DM_LMSGREG_MSG_MASK 0xffffffff
#define  PCIE_DM_LMSGREG_MSG_SHIFT 0
#define  PCIE_DM_LMSGREG_MSG_MASK_SHIFT(reg) (((reg) & PCIE_DM_LMSGREG_MSG_MASK) >> PCIE_DM_LMSGREG_MSG_SHIFT)
#define  PCIE_DM_LMSGREG_MSG_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LMSGREG_MSG_MASK) | (((uint32_t)val) << PCIE_DM_LMSGREG_MSG_SHIFT))

//====================================================================
//Register: DM Core Local Port Logic Force Link Register (LForceLink)
/** \brief Port Logic Force Link Register.*/
//====================================================================

#define  PCIE_DM_LFORCELINK_FORCELINK_MASK 0xffffffff
#define  PCIE_DM_LFORCELINK_FORCELINK_SHIFT 0
#define  PCIE_DM_LFORCELINK_FORCELINK_MASK_SHIFT(reg) (((reg) & PCIE_DM_LFORCELINK_FORCELINK_MASK) >> PCIE_DM_LFORCELINK_FORCELINK_SHIFT)
#define  PCIE_DM_LFORCELINK_FORCELINK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LFORCELINK_FORCELINK_MASK) | (((uint32_t)val) << PCIE_DM_LFORCELINK_FORCELINK_SHIFT))

//====================================================================
//Register: DM Core Local Port Logic Ack Frequency Register (LAckFreq)
/** \brief PCI Express Port Logic Ack Frequency Register.*/
//====================================================================

#define  PCIE_DM_LACKFREQ_RESERVED1_MASK 0x80000000
#define  PCIE_DM_LACKFREQ_RESERVED1_SHIFT 31
#define  PCIE_DM_LACKFREQ_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LACKFREQ_RESERVED1_MASK) >> PCIE_DM_LACKFREQ_RESERVED1_SHIFT)
#define  PCIE_DM_LACKFREQ_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LACKFREQ_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LACKFREQ_RESERVED1_SHIFT))

#define  PCIE_DM_LACKFREQ_L1NOL0S_MASK 0x40000000
#define  PCIE_DM_LACKFREQ_L1NOL0S_SHIFT 30
#define  PCIE_DM_LACKFREQ_L1NOL0S_MASK_SHIFT(reg) (((reg) & PCIE_DM_LACKFREQ_L1NOL0S_MASK) >> PCIE_DM_LACKFREQ_L1NOL0S_SHIFT)
#define  PCIE_DM_LACKFREQ_L1NOL0S_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LACKFREQ_L1NOL0S_MASK) | (((uint32_t)val) << PCIE_DM_LACKFREQ_L1NOL0S_SHIFT))

#define  PCIE_DM_LACKFREQ_L1ENTRYLAT_MASK 0x38000000
#define  PCIE_DM_LACKFREQ_L1ENTRYLAT_SHIFT 27
#define  PCIE_DM_LACKFREQ_L1ENTRYLAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_LACKFREQ_L1ENTRYLAT_MASK) >> PCIE_DM_LACKFREQ_L1ENTRYLAT_SHIFT)
#define  PCIE_DM_LACKFREQ_L1ENTRYLAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LACKFREQ_L1ENTRYLAT_MASK) | (((uint32_t)val) << PCIE_DM_LACKFREQ_L1ENTRYLAT_SHIFT))

#define  PCIE_DM_LACKFREQ_L0SENTRYLAT_MASK 0x7000000
#define  PCIE_DM_LACKFREQ_L0SENTRYLAT_SHIFT 24
#define  PCIE_DM_LACKFREQ_L0SENTRYLAT_MASK_SHIFT(reg) (((reg) & PCIE_DM_LACKFREQ_L0SENTRYLAT_MASK) >> PCIE_DM_LACKFREQ_L0SENTRYLAT_SHIFT)
#define  PCIE_DM_LACKFREQ_L0SENTRYLAT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LACKFREQ_L0SENTRYLAT_MASK) | (((uint32_t)val) << PCIE_DM_LACKFREQ_L0SENTRYLAT_SHIFT))

#define  PCIE_DM_LACKFREQ_CCLKNFTS_MASK 0xff0000
#define  PCIE_DM_LACKFREQ_CCLKNFTS_SHIFT 16
#define  PCIE_DM_LACKFREQ_CCLKNFTS_MASK_SHIFT(reg) (((reg) & PCIE_DM_LACKFREQ_CCLKNFTS_MASK) >> PCIE_DM_LACKFREQ_CCLKNFTS_SHIFT)
#define  PCIE_DM_LACKFREQ_CCLKNFTS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LACKFREQ_CCLKNFTS_MASK) | (((uint32_t)val) << PCIE_DM_LACKFREQ_CCLKNFTS_SHIFT))

#define  PCIE_DM_LACKFREQ_NFTS_MASK 0xff00
#define  PCIE_DM_LACKFREQ_NFTS_SHIFT 8
#define  PCIE_DM_LACKFREQ_NFTS_MASK_SHIFT(reg) (((reg) & PCIE_DM_LACKFREQ_NFTS_MASK) >> PCIE_DM_LACKFREQ_NFTS_SHIFT)
#define  PCIE_DM_LACKFREQ_NFTS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LACKFREQ_NFTS_MASK) | (((uint32_t)val) << PCIE_DM_LACKFREQ_NFTS_SHIFT))

#define  PCIE_DM_LACKFREQ_ACKFREQ_MASK 0xff
#define  PCIE_DM_LACKFREQ_ACKFREQ_SHIFT 0
#define  PCIE_DM_LACKFREQ_ACKFREQ_MASK_SHIFT(reg) (((reg) & PCIE_DM_LACKFREQ_ACKFREQ_MASK) >> PCIE_DM_LACKFREQ_ACKFREQ_SHIFT)
#define  PCIE_DM_LACKFREQ_ACKFREQ_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LACKFREQ_ACKFREQ_MASK) | (((uint32_t)val) << PCIE_DM_LACKFREQ_ACKFREQ_SHIFT))

//====================================================================
//Register: DM Core Local Port Link Control Register (LPortLink)
/** \brief PCI Express Port Link Control Register. Bit 7 controls Fast Link Training*/
//====================================================================

#define  PCIE_DM_LPORTLINK_RESERVED1_MASK 0xffffff00
#define  PCIE_DM_LPORTLINK_RESERVED1_SHIFT 8
#define  PCIE_DM_LPORTLINK_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPORTLINK_RESERVED1_MASK) >> PCIE_DM_LPORTLINK_RESERVED1_SHIFT)
#define  PCIE_DM_LPORTLINK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPORTLINK_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_LPORTLINK_RESERVED1_SHIFT))

#define  PCIE_DM_LPORTLINK_FASTLINKTRNG_MASK 0x80
#define  PCIE_DM_LPORTLINK_FASTLINKTRNG_SHIFT 7
#define  PCIE_DM_LPORTLINK_FASTLINKTRNG_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPORTLINK_FASTLINKTRNG_MASK) >> PCIE_DM_LPORTLINK_FASTLINKTRNG_SHIFT)
#define  PCIE_DM_LPORTLINK_FASTLINKTRNG_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPORTLINK_FASTLINKTRNG_MASK) | (((uint32_t)val) << PCIE_DM_LPORTLINK_FASTLINKTRNG_SHIFT))

#define  PCIE_DM_LPORTLINK_MISCCONTROL_MASK 0x7f
#define  PCIE_DM_LPORTLINK_MISCCONTROL_SHIFT 0
#define  PCIE_DM_LPORTLINK_MISCCONTROL_MASK_SHIFT(reg) (((reg) & PCIE_DM_LPORTLINK_MISCCONTROL_MASK) >> PCIE_DM_LPORTLINK_MISCCONTROL_SHIFT)
#define  PCIE_DM_LPORTLINK_MISCCONTROL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_LPORTLINK_MISCCONTROL_MASK) | (((uint32_t)val) << PCIE_DM_LPORTLINK_MISCCONTROL_SHIFT))

//====================================================================
//Register: DM Application PHY Register Access Control via serial interface (PhyRegAccess)
/** \brief DM Core PHY Register Access Control. Programs the serial interface used to program the PHY.*/
//====================================================================

#define  PCIE_DM_PHYREGACCESS_RESERVED1_MASK 0xfffffff0
#define  PCIE_DM_PHYREGACCESS_RESERVED1_SHIFT 4
#define  PCIE_DM_PHYREGACCESS_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREGACCESS_RESERVED1_MASK) >> PCIE_DM_PHYREGACCESS_RESERVED1_SHIFT)
#define  PCIE_DM_PHYREGACCESS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREGACCESS_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_PHYREGACCESS_RESERVED1_SHIFT))

#define  PCIE_DM_PHYREGACCESS_CLKDIV_MASK 0xf
#define  PCIE_DM_PHYREGACCESS_CLKDIV_SHIFT 0
#define  PCIE_DM_PHYREGACCESS_CLKDIV_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREGACCESS_CLKDIV_MASK) >> PCIE_DM_PHYREGACCESS_CLKDIV_SHIFT)
#define  PCIE_DM_PHYREGACCESS_CLKDIV_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREGACCESS_CLKDIV_MASK) | (((uint32_t)val) << PCIE_DM_PHYREGACCESS_CLKDIV_SHIFT))

//====================================================================
//Register: PHY Read or Write Register (PhyReadWrite)
/** \brief Controls the Serial interface for read or write operation.
          Multiple read/*/
//====================================================================

#define  PCIE_DM_PHYREADWRITE_RESERVED1_MASK 0xfe000000
#define  PCIE_DM_PHYREADWRITE_RESERVED1_SHIFT 25
#define  PCIE_DM_PHYREADWRITE_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITE_RESERVED1_MASK) >> PCIE_DM_PHYREADWRITE_RESERVED1_SHIFT)
#define  PCIE_DM_PHYREADWRITE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITE_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITE_RESERVED1_SHIFT))

#define  PCIE_DM_PHYREADWRITE_WRNRD_MASK 0x1000000
#define  PCIE_DM_PHYREADWRITE_WRNRD_SHIFT 24
#define  PCIE_DM_PHYREADWRITE_WRNRD_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITE_WRNRD_MASK) >> PCIE_DM_PHYREADWRITE_WRNRD_SHIFT)
#define  PCIE_DM_PHYREADWRITE_WRNRD_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITE_WRNRD_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITE_WRNRD_SHIFT))

#define  PCIE_DM_PHYREADWRITE_ADDRESS_MASK 0xff0000
#define  PCIE_DM_PHYREADWRITE_ADDRESS_SHIFT 16
#define  PCIE_DM_PHYREADWRITE_ADDRESS_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITE_ADDRESS_MASK) >> PCIE_DM_PHYREADWRITE_ADDRESS_SHIFT)
#define  PCIE_DM_PHYREADWRITE_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITE_ADDRESS_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITE_ADDRESS_SHIFT))

#define  PCIE_DM_PHYREADWRITE_WRDATA_MASK 0xffff
#define  PCIE_DM_PHYREADWRITE_WRDATA_SHIFT 0
#define  PCIE_DM_PHYREADWRITE_WRDATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITE_WRDATA_MASK) >> PCIE_DM_PHYREADWRITE_WRDATA_SHIFT)
#define  PCIE_DM_PHYREADWRITE_WRDATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITE_WRDATA_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITE_WRDATA_SHIFT))

//====================================================================
//Register: PHY Read or Write Done Register (PhyReadWriteDone)
/** \brief Controls the Serial interface for read or write operation.
          Multiple read/*/
//====================================================================

#define  PCIE_DM_PHYREADWRITEDONE_RESERVED1_MASK 0xfffe0000
#define  PCIE_DM_PHYREADWRITEDONE_RESERVED1_SHIFT 17
#define  PCIE_DM_PHYREADWRITEDONE_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITEDONE_RESERVED1_MASK) >> PCIE_DM_PHYREADWRITEDONE_RESERVED1_SHIFT)
#define  PCIE_DM_PHYREADWRITEDONE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITEDONE_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITEDONE_RESERVED1_SHIFT))

#define  PCIE_DM_PHYREADWRITEDONE_DONE_MASK 0x10000
#define  PCIE_DM_PHYREADWRITEDONE_DONE_SHIFT 16
#define  PCIE_DM_PHYREADWRITEDONE_DONE_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITEDONE_DONE_MASK) >> PCIE_DM_PHYREADWRITEDONE_DONE_SHIFT)
#define  PCIE_DM_PHYREADWRITEDONE_DONE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITEDONE_DONE_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITEDONE_DONE_SHIFT))

#define  PCIE_DM_PHYREADWRITEDONE_RDDATA_MASK 0xffff
#define  PCIE_DM_PHYREADWRITEDONE_RDDATA_SHIFT 0
#define  PCIE_DM_PHYREADWRITEDONE_RDDATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITEDONE_RDDATA_MASK) >> PCIE_DM_PHYREADWRITEDONE_RDDATA_SHIFT)
#define  PCIE_DM_PHYREADWRITEDONE_RDDATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITEDONE_RDDATA_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITEDONE_RDDATA_SHIFT))

//====================================================================
//Register: PHY Read or Write Start Register (PhyReadWriteStart)
/** \brief Controls the Serial interface by indicating the start of read or write operation.*/
//====================================================================

#define  PCIE_DM_PHYREADWRITESTART_RESERVED1_MASK 0xfffffffe
#define  PCIE_DM_PHYREADWRITESTART_RESERVED1_SHIFT 1
#define  PCIE_DM_PHYREADWRITESTART_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITESTART_RESERVED1_MASK) >> PCIE_DM_PHYREADWRITESTART_RESERVED1_SHIFT)
#define  PCIE_DM_PHYREADWRITESTART_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITESTART_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITESTART_RESERVED1_SHIFT))

#define  PCIE_DM_PHYREADWRITESTART_START_MASK 0x1
#define  PCIE_DM_PHYREADWRITESTART_START_SHIFT 0
#define  PCIE_DM_PHYREADWRITESTART_START_MASK_SHIFT(reg) (((reg) & PCIE_DM_PHYREADWRITESTART_START_MASK) >> PCIE_DM_PHYREADWRITESTART_START_SHIFT)
#define  PCIE_DM_PHYREADWRITESTART_START_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_PHYREADWRITESTART_START_MASK) | (((uint32_t)val) << PCIE_DM_PHYREADWRITESTART_START_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 0 (OutAddrXlt0)
/** \brief DM Core Application Outbound Address Translation Register for Memory TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT0_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT0_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT0_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT0_MEM_MASK) >> PCIE_DM_OUTADDRXLT0_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT0_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT0_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT0_MEM_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 1 (OutAddrXlt1)
/** \brief DM Core Application Outbound Address Translation Register for Memory TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT1_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT1_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT1_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT1_MEM_MASK) >> PCIE_DM_OUTADDRXLT1_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT1_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT1_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT1_MEM_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 2 (OutAddrXlt2)
/** \brief DM Core Application Outbound Address Translation Register for Memory TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT2_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT2_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT2_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT2_MEM_MASK) >> PCIE_DM_OUTADDRXLT2_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT2_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT2_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT2_MEM_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 3 (OutAddrXlt3)
/** \brief DM Core Application Outbound Address Translation Register for Memory TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT3_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT3_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT3_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT3_MEM_MASK) >> PCIE_DM_OUTADDRXLT3_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT3_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT3_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT3_MEM_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 4 (OutAddrXlt4)
/** \brief DM Core Application Outbound Address Translation Register for Memory TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT4_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT4_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT4_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT4_MEM_MASK) >> PCIE_DM_OUTADDRXLT4_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT4_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT4_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT4_MEM_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 5 (OutAddrXlt5)
/** \brief DM Core Application Outbound Address Translation Register for Memory TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT5_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT5_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT5_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT5_MEM_MASK) >> PCIE_DM_OUTADDRXLT5_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT5_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT5_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT5_MEM_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 6 (OutAddrXlt6)
/** \brief DM Core Application Outbound Address Translation Register for Memory TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT6_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT6_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT6_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT6_MEM_MASK) >> PCIE_DM_OUTADDRXLT6_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT6_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT6_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT6_MEM_SHIFT))

//====================================================================
//Register: DM Application Outbound Address Translation Register 7 (OutAddrXlt7)
/** \brief DM Core Application Outbound Address Translation Register for I/O TLP's.*/
//====================================================================

#define  PCIE_DM_OUTADDRXLT7_MEM_MASK 0xffffffff
#define  PCIE_DM_OUTADDRXLT7_MEM_SHIFT 0
#define  PCIE_DM_OUTADDRXLT7_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_OUTADDRXLT7_MEM_MASK) >> PCIE_DM_OUTADDRXLT7_MEM_SHIFT)
#define  PCIE_DM_OUTADDRXLT7_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_OUTADDRXLT7_MEM_MASK) | (((uint32_t)val) << PCIE_DM_OUTADDRXLT7_MEM_SHIFT))

//====================================================================
//Register: DM Application Configuration TLP Settings (CfgControl)
/** \brief DM Core Application Outbound Address Translation Register for Configuration TLP's.  
Configuration TLP's require that the Address[31:16] in the outbound TLP header contain the 
{Bus Number[7:0], Device Number[4:0], Funtion Number[2:0]} fields.
This register provides control of those fields in the Cfg TLP Header.*/
//====================================================================

#define  PCIE_DM_CFGCONTROL_RESERVED1_MASK 0xfffe0000
#define  PCIE_DM_CFGCONTROL_RESERVED1_SHIFT 17
#define  PCIE_DM_CFGCONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_CFGCONTROL_RESERVED1_MASK) >> PCIE_DM_CFGCONTROL_RESERVED1_SHIFT)
#define  PCIE_DM_CFGCONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_CFGCONTROL_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_CFGCONTROL_RESERVED1_SHIFT))

#define  PCIE_DM_CFGCONTROL_TYPE1_MASK 0x10000
#define  PCIE_DM_CFGCONTROL_TYPE1_SHIFT 16
#define  PCIE_DM_CFGCONTROL_TYPE1_MASK_SHIFT(reg) (((reg) & PCIE_DM_CFGCONTROL_TYPE1_MASK) >> PCIE_DM_CFGCONTROL_TYPE1_SHIFT)
#define  PCIE_DM_CFGCONTROL_TYPE1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_CFGCONTROL_TYPE1_MASK) | (((uint32_t)val) << PCIE_DM_CFGCONTROL_TYPE1_SHIFT))

#define  PCIE_DM_CFGCONTROL_BUSNUMBER_MASK 0xff00
#define  PCIE_DM_CFGCONTROL_BUSNUMBER_SHIFT 8
#define  PCIE_DM_CFGCONTROL_BUSNUMBER_MASK_SHIFT(reg) (((reg) & PCIE_DM_CFGCONTROL_BUSNUMBER_MASK) >> PCIE_DM_CFGCONTROL_BUSNUMBER_SHIFT)
#define  PCIE_DM_CFGCONTROL_BUSNUMBER_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_CFGCONTROL_BUSNUMBER_MASK) | (((uint32_t)val) << PCIE_DM_CFGCONTROL_BUSNUMBER_SHIFT))

#define  PCIE_DM_CFGCONTROL_DEVNUMBER_MASK 0xf8
#define  PCIE_DM_CFGCONTROL_DEVNUMBER_SHIFT 3
#define  PCIE_DM_CFGCONTROL_DEVNUMBER_MASK_SHIFT(reg) (((reg) & PCIE_DM_CFGCONTROL_DEVNUMBER_MASK) >> PCIE_DM_CFGCONTROL_DEVNUMBER_SHIFT)
#define  PCIE_DM_CFGCONTROL_DEVNUMBER_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_CFGCONTROL_DEVNUMBER_MASK) | (((uint32_t)val) << PCIE_DM_CFGCONTROL_DEVNUMBER_SHIFT))

#define  PCIE_DM_CFGCONTROL_FUNCNUMBER_MASK 0x7
#define  PCIE_DM_CFGCONTROL_FUNCNUMBER_SHIFT 0
#define  PCIE_DM_CFGCONTROL_FUNCNUMBER_MASK_SHIFT(reg) (((reg) & PCIE_DM_CFGCONTROL_FUNCNUMBER_MASK) >> PCIE_DM_CFGCONTROL_FUNCNUMBER_SHIFT)
#define  PCIE_DM_CFGCONTROL_FUNCNUMBER_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_CFGCONTROL_FUNCNUMBER_MASK) | (((uint32_t)val) << PCIE_DM_CFGCONTROL_FUNCNUMBER_SHIFT))

//====================================================================
//Register: DM Application Control Register (ACtrl)
/** \brief Start of the DM Core Application Registers - Control Register*/
//====================================================================

#define  PCIE_DM_ACTRL_INTP_DIAG_MASK 0x80000000
#define  PCIE_DM_ACTRL_INTP_DIAG_SHIFT 31
#define  PCIE_DM_ACTRL_INTP_DIAG_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_INTP_DIAG_MASK) >> PCIE_DM_ACTRL_INTP_DIAG_SHIFT)
#define  PCIE_DM_ACTRL_INTP_DIAG_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_INTP_DIAG_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_INTP_DIAG_SHIFT))

#define  PCIE_DM_ACTRL_RESERVED1_MASK 0x7fff0000
#define  PCIE_DM_ACTRL_RESERVED1_SHIFT 16
#define  PCIE_DM_ACTRL_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_RESERVED1_MASK) >> PCIE_DM_ACTRL_RESERVED1_SHIFT)
#define  PCIE_DM_ACTRL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_RESERVED1_SHIFT))

#define  PCIE_DM_ACTRL_LTSSM_EN_MASK 0x8000
#define  PCIE_DM_ACTRL_LTSSM_EN_SHIFT 15
#define  PCIE_DM_ACTRL_LTSSM_EN_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_LTSSM_EN_MASK) >> PCIE_DM_ACTRL_LTSSM_EN_SHIFT)
#define  PCIE_DM_ACTRL_LTSSM_EN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_LTSSM_EN_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_LTSSM_EN_SHIFT))

#define  PCIE_DM_ACTRL_L1EXIT_MASK 0x4000
#define  PCIE_DM_ACTRL_L1EXIT_SHIFT 14
#define  PCIE_DM_ACTRL_L1EXIT_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_L1EXIT_MASK) >> PCIE_DM_ACTRL_L1EXIT_SHIFT)
#define  PCIE_DM_ACTRL_L1EXIT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_L1EXIT_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_L1EXIT_SHIFT))

#define  PCIE_DM_ACTRL_RESERVED2_MASK 0x3c00
#define  PCIE_DM_ACTRL_RESERVED2_SHIFT 10
#define  PCIE_DM_ACTRL_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_RESERVED2_MASK) >> PCIE_DM_ACTRL_RESERVED2_SHIFT)
#define  PCIE_DM_ACTRL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_RESERVED2_SHIFT))

#define  PCIE_DM_ACTRL_MSTRRESPERRMAP_MASK 0x300
#define  PCIE_DM_ACTRL_MSTRRESPERRMAP_SHIFT 8
#define  PCIE_DM_ACTRL_MSTRRESPERRMAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_MSTRRESPERRMAP_MASK) >> PCIE_DM_ACTRL_MSTRRESPERRMAP_SHIFT)
#define  PCIE_DM_ACTRL_MSTRRESPERRMAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_MSTRRESPERRMAP_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_MSTRRESPERRMAP_SHIFT))

#define  PCIE_DM_ACTRL_RESERVED3_MASK 0xc0
#define  PCIE_DM_ACTRL_RESERVED3_SHIFT 6
#define  PCIE_DM_ACTRL_RESERVED3_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_RESERVED3_MASK) >> PCIE_DM_ACTRL_RESERVED3_SHIFT)
#define  PCIE_DM_ACTRL_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_RESERVED3_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_RESERVED3_SHIFT))

#define  PCIE_DM_ACTRL_SLVRESPERRMAP_MASK 0x3f
#define  PCIE_DM_ACTRL_SLVRESPERRMAP_SHIFT 0
#define  PCIE_DM_ACTRL_SLVRESPERRMAP_MASK_SHIFT(reg) (((reg) & PCIE_DM_ACTRL_SLVRESPERRMAP_MASK) >> PCIE_DM_ACTRL_SLVRESPERRMAP_SHIFT)
#define  PCIE_DM_ACTRL_SLVRESPERRMAP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ACTRL_SLVRESPERRMAP_MASK) | (((uint32_t)val) << PCIE_DM_ACTRL_SLVRESPERRMAP_SHIFT))

//====================================================================
//Register: DM Application Status Register (AStat)
/** \brief Collects Non-Interrupt type Status bits*/
//====================================================================

#define  PCIE_DM_ASTAT_RESERVED1_MASK 0xffffff00
#define  PCIE_DM_ASTAT_RESERVED1_SHIFT 8
#define  PCIE_DM_ASTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_ASTAT_RESERVED1_MASK) >> PCIE_DM_ASTAT_RESERVED1_SHIFT)
#define  PCIE_DM_ASTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ASTAT_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_ASTAT_RESERVED1_SHIFT))

#define  PCIE_DM_ASTAT_LTSSMLINKUP_MASK 0x80
#define  PCIE_DM_ASTAT_LTSSMLINKUP_SHIFT 7
#define  PCIE_DM_ASTAT_LTSSMLINKUP_MASK_SHIFT(reg) (((reg) & PCIE_DM_ASTAT_LTSSMLINKUP_MASK) >> PCIE_DM_ASTAT_LTSSMLINKUP_SHIFT)
#define  PCIE_DM_ASTAT_LTSSMLINKUP_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ASTAT_LTSSMLINKUP_MASK) | (((uint32_t)val) << PCIE_DM_ASTAT_LTSSMLINKUP_SHIFT))

#define  PCIE_DM_ASTAT_RESERVED2_MASK 0x40
#define  PCIE_DM_ASTAT_RESERVED2_SHIFT 6
#define  PCIE_DM_ASTAT_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_ASTAT_RESERVED2_MASK) >> PCIE_DM_ASTAT_RESERVED2_SHIFT)
#define  PCIE_DM_ASTAT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ASTAT_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_ASTAT_RESERVED2_SHIFT))

#define  PCIE_DM_ASTAT_LTSSMLINKSTATUS_MASK 0x3f
#define  PCIE_DM_ASTAT_LTSSMLINKSTATUS_SHIFT 0
#define  PCIE_DM_ASTAT_LTSSMLINKSTATUS_MASK_SHIFT(reg) (((reg) & PCIE_DM_ASTAT_LTSSMLINKSTATUS_MASK) >> PCIE_DM_ASTAT_LTSSMLINKSTATUS_SHIFT)
#define  PCIE_DM_ASTAT_LTSSMLINKSTATUS_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_ASTAT_LTSSMLINKSTATUS_MASK) | (((uint32_t)val) << PCIE_DM_ASTAT_LTSSMLINKSTATUS_SHIFT))

//====================================================================
//Register: DM Application Interrupt Pending Register (AIntP)
/** \brief Collects Interrupt Pending bits*/
//====================================================================

#define  PCIE_DM_AINTP_GMCOMPLOOKUPERR_MASK 0x80000000
#define  PCIE_DM_AINTP_GMCOMPLOOKUPERR_SHIFT 31
#define  PCIE_DM_AINTP_GMCOMPLOOKUPERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_GMCOMPLOOKUPERR_MASK) >> PCIE_DM_AINTP_GMCOMPLOOKUPERR_SHIFT)
#define  PCIE_DM_AINTP_GMCOMPLOOKUPERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_GMCOMPLOOKUPERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_GMCOMPLOOKUPERR_SHIFT))

#define  PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_MASK 0x40000000
#define  PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_SHIFT 30
#define  PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_MASK) >> PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_SHIFT)
#define  PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMXCOMPLOOKUPERR_SHIFT))

#define  PCIE_DM_AINTP_CFGSYSERRRC_MASK 0x20000000
#define  PCIE_DM_AINTP_CFGSYSERRRC_SHIFT 29
#define  PCIE_DM_AINTP_CFGSYSERRRC_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_CFGSYSERRRC_MASK) >> PCIE_DM_AINTP_CFGSYSERRRC_SHIFT)
#define  PCIE_DM_AINTP_CFGSYSERRRC_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_CFGSYSERRRC_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_CFGSYSERRRC_SHIFT))

#define  PCIE_DM_AINTP_INVINTLPTYPE_MASK 0x10000000
#define  PCIE_DM_AINTP_INVINTLPTYPE_SHIFT 28
#define  PCIE_DM_AINTP_INVINTLPTYPE_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_INVINTLPTYPE_MASK) >> PCIE_DM_AINTP_INVINTLPTYPE_SHIFT)
#define  PCIE_DM_AINTP_INVINTLPTYPE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_INVINTLPTYPE_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_INVINTLPTYPE_SHIFT))

#define  PCIE_DM_AINTP_CFGPMEINT_MASK 0x8000000
#define  PCIE_DM_AINTP_CFGPMEINT_SHIFT 27
#define  PCIE_DM_AINTP_CFGPMEINT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_CFGPMEINT_MASK) >> PCIE_DM_AINTP_CFGPMEINT_SHIFT)
#define  PCIE_DM_AINTP_CFGPMEINT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_CFGPMEINT_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_CFGPMEINT_SHIFT))

#define  PCIE_DM_AINTP_CFGPMEMSI_MASK 0x4000000
#define  PCIE_DM_AINTP_CFGPMEMSI_SHIFT 26
#define  PCIE_DM_AINTP_CFGPMEMSI_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_CFGPMEMSI_MASK) >> PCIE_DM_AINTP_CFGPMEMSI_SHIFT)
#define  PCIE_DM_AINTP_CFGPMEMSI_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_CFGPMEMSI_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_CFGPMEMSI_SHIFT))

#define  PCIE_DM_AINTP_RESERVED1_MASK 0x2000000
#define  PCIE_DM_AINTP_RESERVED1_SHIFT 25
#define  PCIE_DM_AINTP_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RESERVED1_MASK) >> PCIE_DM_AINTP_RESERVED1_SHIFT)
#define  PCIE_DM_AINTP_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RESERVED1_SHIFT))

#define  PCIE_DM_AINTP_LINKREQRST_MASK 0x1000000
#define  PCIE_DM_AINTP_LINKREQRST_SHIFT 24
#define  PCIE_DM_AINTP_LINKREQRST_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_LINKREQRST_MASK) >> PCIE_DM_AINTP_LINKREQRST_SHIFT)
#define  PCIE_DM_AINTP_LINKREQRST_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_LINKREQRST_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_LINKREQRST_SHIFT))

#define  PCIE_DM_AINTP_CFGAERERRINT_MASK 0x800000
#define  PCIE_DM_AINTP_CFGAERERRINT_SHIFT 23
#define  PCIE_DM_AINTP_CFGAERERRINT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_CFGAERERRINT_MASK) >> PCIE_DM_AINTP_CFGAERERRINT_SHIFT)
#define  PCIE_DM_AINTP_CFGAERERRINT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_CFGAERERRINT_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_CFGAERERRINT_SHIFT))

#define  PCIE_DM_AINTP_CFGAERERRMSI_MASK 0x400000
#define  PCIE_DM_AINTP_CFGAERERRMSI_SHIFT 22
#define  PCIE_DM_AINTP_CFGAERERRMSI_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_CFGAERERRMSI_MASK) >> PCIE_DM_AINTP_CFGAERERRMSI_SHIFT)
#define  PCIE_DM_AINTP_CFGAERERRMSI_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_CFGAERERRMSI_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_CFGAERERRMSI_SHIFT))

#define  PCIE_DM_AINTP_RESERVED2_MASK 0x200000
#define  PCIE_DM_AINTP_RESERVED2_SHIFT 21
#define  PCIE_DM_AINTP_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RESERVED2_MASK) >> PCIE_DM_AINTP_RESERVED2_SHIFT)
#define  PCIE_DM_AINTP_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RESERVED2_SHIFT))

#define  PCIE_DM_AINTP_CFGAERINTMSGNUM_MASK 0x1f0000
#define  PCIE_DM_AINTP_CFGAERINTMSGNUM_SHIFT 16
#define  PCIE_DM_AINTP_CFGAERINTMSGNUM_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_CFGAERINTMSGNUM_MASK) >> PCIE_DM_AINTP_CFGAERINTMSGNUM_SHIFT)
#define  PCIE_DM_AINTP_CFGAERINTMSGNUM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_CFGAERINTMSGNUM_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_CFGAERINTMSGNUM_SHIFT))

#define  PCIE_DM_AINTP_RADMFATAL_MASK 0x8000
#define  PCIE_DM_AINTP_RADMFATAL_SHIFT 15
#define  PCIE_DM_AINTP_RADMFATAL_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMFATAL_MASK) >> PCIE_DM_AINTP_RADMFATAL_SHIFT)
#define  PCIE_DM_AINTP_RADMFATAL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMFATAL_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMFATAL_SHIFT))

#define  PCIE_DM_AINTP_RADMNONFATAL_MASK 0x4000
#define  PCIE_DM_AINTP_RADMNONFATAL_SHIFT 14
#define  PCIE_DM_AINTP_RADMNONFATAL_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMNONFATAL_MASK) >> PCIE_DM_AINTP_RADMNONFATAL_SHIFT)
#define  PCIE_DM_AINTP_RADMNONFATAL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMNONFATAL_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMNONFATAL_SHIFT))

#define  PCIE_DM_AINTP_RADMCORRERR_MASK 0x2000
#define  PCIE_DM_AINTP_RADMCORRERR_SHIFT 13
#define  PCIE_DM_AINTP_RADMCORRERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMCORRERR_MASK) >> PCIE_DM_AINTP_RADMCORRERR_SHIFT)
#define  PCIE_DM_AINTP_RADMCORRERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMCORRERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMCORRERR_SHIFT))

#define  PCIE_DM_AINTP_RADMPME_MASK 0x1000
#define  PCIE_DM_AINTP_RADMPME_SHIFT 12
#define  PCIE_DM_AINTP_RADMPME_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMPME_MASK) >> PCIE_DM_AINTP_RADMPME_SHIFT)
#define  PCIE_DM_AINTP_RADMPME_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMPME_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMPME_SHIFT))

#define  PCIE_DM_AINTP_RADMCPLTIMEOUT_MASK 0x800
#define  PCIE_DM_AINTP_RADMCPLTIMEOUT_SHIFT 11
#define  PCIE_DM_AINTP_RADMCPLTIMEOUT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMCPLTIMEOUT_MASK) >> PCIE_DM_AINTP_RADMCPLTIMEOUT_SHIFT)
#define  PCIE_DM_AINTP_RADMCPLTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMCPLTIMEOUT_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMCPLTIMEOUT_SHIFT))

#define  PCIE_DM_AINTP_RESERVED3_MASK 0x400
#define  PCIE_DM_AINTP_RESERVED3_SHIFT 10
#define  PCIE_DM_AINTP_RESERVED3_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RESERVED3_MASK) >> PCIE_DM_AINTP_RESERVED3_SHIFT)
#define  PCIE_DM_AINTP_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RESERVED3_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RESERVED3_SHIFT))

#define  PCIE_DM_AINTP_RADMCPLUR_MASK 0x200
#define  PCIE_DM_AINTP_RADMCPLUR_SHIFT 9
#define  PCIE_DM_AINTP_RADMCPLUR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMCPLUR_MASK) >> PCIE_DM_AINTP_RADMCPLUR_SHIFT)
#define  PCIE_DM_AINTP_RADMCPLUR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMCPLUR_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMCPLUR_SHIFT))

#define  PCIE_DM_AINTP_RADMCA_MASK 0x100
#define  PCIE_DM_AINTP_RADMCA_SHIFT 8
#define  PCIE_DM_AINTP_RADMCA_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RADMCA_MASK) >> PCIE_DM_AINTP_RADMCA_SHIFT)
#define  PCIE_DM_AINTP_RADMCA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RADMCA_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RADMCA_SHIFT))

#define  PCIE_DM_AINTP_RESERVED4_MASK 0xe0
#define  PCIE_DM_AINTP_RESERVED4_SHIFT 5
#define  PCIE_DM_AINTP_RESERVED4_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RESERVED4_MASK) >> PCIE_DM_AINTP_RESERVED4_SHIFT)
#define  PCIE_DM_AINTP_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RESERVED4_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RESERVED4_SHIFT))

#define  PCIE_DM_AINTP_TRGTCPLTIMEOUT_MASK 0x10
#define  PCIE_DM_AINTP_TRGTCPLTIMEOUT_SHIFT 4
#define  PCIE_DM_AINTP_TRGTCPLTIMEOUT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_TRGTCPLTIMEOUT_MASK) >> PCIE_DM_AINTP_TRGTCPLTIMEOUT_SHIFT)
#define  PCIE_DM_AINTP_TRGTCPLTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_TRGTCPLTIMEOUT_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_TRGTCPLTIMEOUT_SHIFT))

#define  PCIE_DM_AINTP_TRGTLOOKUPEMPTY_MASK 0x8
#define  PCIE_DM_AINTP_TRGTLOOKUPEMPTY_SHIFT 3
#define  PCIE_DM_AINTP_TRGTLOOKUPEMPTY_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_TRGTLOOKUPEMPTY_MASK) >> PCIE_DM_AINTP_TRGTLOOKUPEMPTY_SHIFT)
#define  PCIE_DM_AINTP_TRGTLOOKUPEMPTY_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_TRGTLOOKUPEMPTY_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_TRGTLOOKUPEMPTY_SHIFT))

#define  PCIE_DM_AINTP_RESERVED5_MASK 0x6
#define  PCIE_DM_AINTP_RESERVED5_SHIFT 1
#define  PCIE_DM_AINTP_RESERVED5_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_RESERVED5_MASK) >> PCIE_DM_AINTP_RESERVED5_SHIFT)
#define  PCIE_DM_AINTP_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_RESERVED5_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_RESERVED5_SHIFT))

#define  PCIE_DM_AINTP_TRNGRESET_MASK 0x1
#define  PCIE_DM_AINTP_TRNGRESET_SHIFT 0
#define  PCIE_DM_AINTP_TRNGRESET_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTP_TRNGRESET_MASK) >> PCIE_DM_AINTP_TRNGRESET_SHIFT)
#define  PCIE_DM_AINTP_TRNGRESET_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTP_TRNGRESET_MASK) | (((uint32_t)val) << PCIE_DM_AINTP_TRNGRESET_SHIFT))

//====================================================================
//Register: DM Application Interrupt Enable Register (AIntEn)
/** \brief Enable bits corresonding to each Interrupt Pending Register bit, excepting the
AER Interrupt Message Number status bits.*/
//====================================================================

#define  PCIE_DM_AINTEN_GMCOMPLOOKUPERR_MASK 0x80000000
#define  PCIE_DM_AINTEN_GMCOMPLOOKUPERR_SHIFT 31
#define  PCIE_DM_AINTEN_GMCOMPLOOKUPERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_GMCOMPLOOKUPERR_MASK) >> PCIE_DM_AINTEN_GMCOMPLOOKUPERR_SHIFT)
#define  PCIE_DM_AINTEN_GMCOMPLOOKUPERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_GMCOMPLOOKUPERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_GMCOMPLOOKUPERR_SHIFT))

#define  PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_MASK 0x40000000
#define  PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_SHIFT 30
#define  PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_MASK) >> PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_SHIFT)
#define  PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMXCOMPLOOKUPERR_SHIFT))

#define  PCIE_DM_AINTEN_CFGSYSERRRC_MASK 0x20000000
#define  PCIE_DM_AINTEN_CFGSYSERRRC_SHIFT 29
#define  PCIE_DM_AINTEN_CFGSYSERRRC_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_CFGSYSERRRC_MASK) >> PCIE_DM_AINTEN_CFGSYSERRRC_SHIFT)
#define  PCIE_DM_AINTEN_CFGSYSERRRC_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_CFGSYSERRRC_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_CFGSYSERRRC_SHIFT))

#define  PCIE_DM_AINTEN_INVINTLPTYPE_MASK 0x10000000
#define  PCIE_DM_AINTEN_INVINTLPTYPE_SHIFT 28
#define  PCIE_DM_AINTEN_INVINTLPTYPE_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_INVINTLPTYPE_MASK) >> PCIE_DM_AINTEN_INVINTLPTYPE_SHIFT)
#define  PCIE_DM_AINTEN_INVINTLPTYPE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_INVINTLPTYPE_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_INVINTLPTYPE_SHIFT))

#define  PCIE_DM_AINTEN_CFGPMEINT_MASK 0x8000000
#define  PCIE_DM_AINTEN_CFGPMEINT_SHIFT 27
#define  PCIE_DM_AINTEN_CFGPMEINT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_CFGPMEINT_MASK) >> PCIE_DM_AINTEN_CFGPMEINT_SHIFT)
#define  PCIE_DM_AINTEN_CFGPMEINT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_CFGPMEINT_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_CFGPMEINT_SHIFT))

#define  PCIE_DM_AINTEN_CFGPMEMSI_MASK 0x4000000
#define  PCIE_DM_AINTEN_CFGPMEMSI_SHIFT 26
#define  PCIE_DM_AINTEN_CFGPMEMSI_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_CFGPMEMSI_MASK) >> PCIE_DM_AINTEN_CFGPMEMSI_SHIFT)
#define  PCIE_DM_AINTEN_CFGPMEMSI_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_CFGPMEMSI_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_CFGPMEMSI_SHIFT))

#define  PCIE_DM_AINTEN_RESERVED1_MASK 0x2000000
#define  PCIE_DM_AINTEN_RESERVED1_SHIFT 25
#define  PCIE_DM_AINTEN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RESERVED1_MASK) >> PCIE_DM_AINTEN_RESERVED1_SHIFT)
#define  PCIE_DM_AINTEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RESERVED1_SHIFT))

#define  PCIE_DM_AINTEN_LINKREQRST_MASK 0x1000000
#define  PCIE_DM_AINTEN_LINKREQRST_SHIFT 24
#define  PCIE_DM_AINTEN_LINKREQRST_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_LINKREQRST_MASK) >> PCIE_DM_AINTEN_LINKREQRST_SHIFT)
#define  PCIE_DM_AINTEN_LINKREQRST_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_LINKREQRST_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_LINKREQRST_SHIFT))

#define  PCIE_DM_AINTEN_CFGAERERRINT_MASK 0x800000
#define  PCIE_DM_AINTEN_CFGAERERRINT_SHIFT 23
#define  PCIE_DM_AINTEN_CFGAERERRINT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_CFGAERERRINT_MASK) >> PCIE_DM_AINTEN_CFGAERERRINT_SHIFT)
#define  PCIE_DM_AINTEN_CFGAERERRINT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_CFGAERERRINT_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_CFGAERERRINT_SHIFT))

#define  PCIE_DM_AINTEN_CFGAERERRMSI_MASK 0x400000
#define  PCIE_DM_AINTEN_CFGAERERRMSI_SHIFT 22
#define  PCIE_DM_AINTEN_CFGAERERRMSI_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_CFGAERERRMSI_MASK) >> PCIE_DM_AINTEN_CFGAERERRMSI_SHIFT)
#define  PCIE_DM_AINTEN_CFGAERERRMSI_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_CFGAERERRMSI_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_CFGAERERRMSI_SHIFT))

#define  PCIE_DM_AINTEN_RESERVED2_MASK 0x3f0000
#define  PCIE_DM_AINTEN_RESERVED2_SHIFT 16
#define  PCIE_DM_AINTEN_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RESERVED2_MASK) >> PCIE_DM_AINTEN_RESERVED2_SHIFT)
#define  PCIE_DM_AINTEN_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RESERVED2_SHIFT))

#define  PCIE_DM_AINTEN_RADMFATAL_MASK 0x8000
#define  PCIE_DM_AINTEN_RADMFATAL_SHIFT 15
#define  PCIE_DM_AINTEN_RADMFATAL_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMFATAL_MASK) >> PCIE_DM_AINTEN_RADMFATAL_SHIFT)
#define  PCIE_DM_AINTEN_RADMFATAL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMFATAL_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMFATAL_SHIFT))

#define  PCIE_DM_AINTEN_RADMNONFATAL_MASK 0x4000
#define  PCIE_DM_AINTEN_RADMNONFATAL_SHIFT 14
#define  PCIE_DM_AINTEN_RADMNONFATAL_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMNONFATAL_MASK) >> PCIE_DM_AINTEN_RADMNONFATAL_SHIFT)
#define  PCIE_DM_AINTEN_RADMNONFATAL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMNONFATAL_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMNONFATAL_SHIFT))

#define  PCIE_DM_AINTEN_RADMCORRERR_MASK 0x2000
#define  PCIE_DM_AINTEN_RADMCORRERR_SHIFT 13
#define  PCIE_DM_AINTEN_RADMCORRERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMCORRERR_MASK) >> PCIE_DM_AINTEN_RADMCORRERR_SHIFT)
#define  PCIE_DM_AINTEN_RADMCORRERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMCORRERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMCORRERR_SHIFT))

#define  PCIE_DM_AINTEN_RADMPME_MASK 0x1000
#define  PCIE_DM_AINTEN_RADMPME_SHIFT 12
#define  PCIE_DM_AINTEN_RADMPME_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMPME_MASK) >> PCIE_DM_AINTEN_RADMPME_SHIFT)
#define  PCIE_DM_AINTEN_RADMPME_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMPME_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMPME_SHIFT))

#define  PCIE_DM_AINTEN_RADMCPLTIMEOUT_MASK 0x800
#define  PCIE_DM_AINTEN_RADMCPLTIMEOUT_SHIFT 11
#define  PCIE_DM_AINTEN_RADMCPLTIMEOUT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMCPLTIMEOUT_MASK) >> PCIE_DM_AINTEN_RADMCPLTIMEOUT_SHIFT)
#define  PCIE_DM_AINTEN_RADMCPLTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMCPLTIMEOUT_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMCPLTIMEOUT_SHIFT))

#define  PCIE_DM_AINTEN_RESERVED3_MASK 0x400
#define  PCIE_DM_AINTEN_RESERVED3_SHIFT 10
#define  PCIE_DM_AINTEN_RESERVED3_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RESERVED3_MASK) >> PCIE_DM_AINTEN_RESERVED3_SHIFT)
#define  PCIE_DM_AINTEN_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RESERVED3_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RESERVED3_SHIFT))

#define  PCIE_DM_AINTEN_RADMCPLUR_MASK 0x200
#define  PCIE_DM_AINTEN_RADMCPLUR_SHIFT 9
#define  PCIE_DM_AINTEN_RADMCPLUR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMCPLUR_MASK) >> PCIE_DM_AINTEN_RADMCPLUR_SHIFT)
#define  PCIE_DM_AINTEN_RADMCPLUR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMCPLUR_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMCPLUR_SHIFT))

#define  PCIE_DM_AINTEN_RADMCA_MASK 0x100
#define  PCIE_DM_AINTEN_RADMCA_SHIFT 8
#define  PCIE_DM_AINTEN_RADMCA_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RADMCA_MASK) >> PCIE_DM_AINTEN_RADMCA_SHIFT)
#define  PCIE_DM_AINTEN_RADMCA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RADMCA_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RADMCA_SHIFT))

#define  PCIE_DM_AINTEN_RESERVED4_MASK 0xe0
#define  PCIE_DM_AINTEN_RESERVED4_SHIFT 5
#define  PCIE_DM_AINTEN_RESERVED4_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RESERVED4_MASK) >> PCIE_DM_AINTEN_RESERVED4_SHIFT)
#define  PCIE_DM_AINTEN_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RESERVED4_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RESERVED4_SHIFT))

#define  PCIE_DM_AINTEN_TRGTCPLTIMEOUT_MASK 0x10
#define  PCIE_DM_AINTEN_TRGTCPLTIMEOUT_SHIFT 4
#define  PCIE_DM_AINTEN_TRGTCPLTIMEOUT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_TRGTCPLTIMEOUT_MASK) >> PCIE_DM_AINTEN_TRGTCPLTIMEOUT_SHIFT)
#define  PCIE_DM_AINTEN_TRGTCPLTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_TRGTCPLTIMEOUT_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_TRGTCPLTIMEOUT_SHIFT))

#define  PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_MASK 0x8
#define  PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_SHIFT 3
#define  PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_MASK) >> PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_SHIFT)
#define  PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_TRGTLOOKUPEMPTY_SHIFT))

#define  PCIE_DM_AINTEN_RESERVED5_MASK 0x6
#define  PCIE_DM_AINTEN_RESERVED5_SHIFT 1
#define  PCIE_DM_AINTEN_RESERVED5_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_RESERVED5_MASK) >> PCIE_DM_AINTEN_RESERVED5_SHIFT)
#define  PCIE_DM_AINTEN_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_RESERVED5_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_RESERVED5_SHIFT))

#define  PCIE_DM_AINTEN_TRNGRESET_MASK 0x1
#define  PCIE_DM_AINTEN_TRNGRESET_SHIFT 0
#define  PCIE_DM_AINTEN_TRNGRESET_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTEN_TRNGRESET_MASK) >> PCIE_DM_AINTEN_TRNGRESET_SHIFT)
#define  PCIE_DM_AINTEN_TRNGRESET_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTEN_TRNGRESET_MASK) | (((uint32_t)val) << PCIE_DM_AINTEN_TRNGRESET_SHIFT))

//====================================================================
//Register: DM Application Interrupt Acknowledge Register (AIntAck)
/** \brief Interrupt Acknowledge bits corresonding to each Interrupt Pending Register bit.
Writing a "1" to a bit will clear the corresponding Interrupt Pending bit,
excepting the AER Interrupt Message Number status bits.
Note that bit 16 is a diagnostic bit that when written, will set all the interrupt pending bits from the PCIe Core.*/
//====================================================================

#define  PCIE_DM_AINTACK_GMCOMPLOOKUPERR_MASK 0x80000000
#define  PCIE_DM_AINTACK_GMCOMPLOOKUPERR_SHIFT 31
#define  PCIE_DM_AINTACK_GMCOMPLOOKUPERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_GMCOMPLOOKUPERR_MASK) >> PCIE_DM_AINTACK_GMCOMPLOOKUPERR_SHIFT)
#define  PCIE_DM_AINTACK_GMCOMPLOOKUPERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_GMCOMPLOOKUPERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_GMCOMPLOOKUPERR_SHIFT))

#define  PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_MASK 0x40000000
#define  PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_SHIFT 30
#define  PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_MASK) >> PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_SHIFT)
#define  PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMXCOMPLOOKUPERR_SHIFT))

#define  PCIE_DM_AINTACK_CFGSYSERRRC_MASK 0x20000000
#define  PCIE_DM_AINTACK_CFGSYSERRRC_SHIFT 29
#define  PCIE_DM_AINTACK_CFGSYSERRRC_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_CFGSYSERRRC_MASK) >> PCIE_DM_AINTACK_CFGSYSERRRC_SHIFT)
#define  PCIE_DM_AINTACK_CFGSYSERRRC_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_CFGSYSERRRC_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_CFGSYSERRRC_SHIFT))

#define  PCIE_DM_AINTACK_INVINTLPTYPE_MASK 0x10000000
#define  PCIE_DM_AINTACK_INVINTLPTYPE_SHIFT 28
#define  PCIE_DM_AINTACK_INVINTLPTYPE_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_INVINTLPTYPE_MASK) >> PCIE_DM_AINTACK_INVINTLPTYPE_SHIFT)
#define  PCIE_DM_AINTACK_INVINTLPTYPE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_INVINTLPTYPE_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_INVINTLPTYPE_SHIFT))

#define  PCIE_DM_AINTACK_CFGPMEINT_MASK 0x8000000
#define  PCIE_DM_AINTACK_CFGPMEINT_SHIFT 27
#define  PCIE_DM_AINTACK_CFGPMEINT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_CFGPMEINT_MASK) >> PCIE_DM_AINTACK_CFGPMEINT_SHIFT)
#define  PCIE_DM_AINTACK_CFGPMEINT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_CFGPMEINT_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_CFGPMEINT_SHIFT))

#define  PCIE_DM_AINTACK_CFGPMEMSI_MASK 0x4000000
#define  PCIE_DM_AINTACK_CFGPMEMSI_SHIFT 26
#define  PCIE_DM_AINTACK_CFGPMEMSI_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_CFGPMEMSI_MASK) >> PCIE_DM_AINTACK_CFGPMEMSI_SHIFT)
#define  PCIE_DM_AINTACK_CFGPMEMSI_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_CFGPMEMSI_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_CFGPMEMSI_SHIFT))

#define  PCIE_DM_AINTACK_RESERVED1_MASK 0x2000000
#define  PCIE_DM_AINTACK_RESERVED1_SHIFT 25
#define  PCIE_DM_AINTACK_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RESERVED1_MASK) >> PCIE_DM_AINTACK_RESERVED1_SHIFT)
#define  PCIE_DM_AINTACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RESERVED1_SHIFT))

#define  PCIE_DM_AINTACK_LINKREQRST_MASK 0x1000000
#define  PCIE_DM_AINTACK_LINKREQRST_SHIFT 24
#define  PCIE_DM_AINTACK_LINKREQRST_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_LINKREQRST_MASK) >> PCIE_DM_AINTACK_LINKREQRST_SHIFT)
#define  PCIE_DM_AINTACK_LINKREQRST_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_LINKREQRST_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_LINKREQRST_SHIFT))

#define  PCIE_DM_AINTACK_CFGAERERRINT_MASK 0x800000
#define  PCIE_DM_AINTACK_CFGAERERRINT_SHIFT 23
#define  PCIE_DM_AINTACK_CFGAERERRINT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_CFGAERERRINT_MASK) >> PCIE_DM_AINTACK_CFGAERERRINT_SHIFT)
#define  PCIE_DM_AINTACK_CFGAERERRINT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_CFGAERERRINT_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_CFGAERERRINT_SHIFT))

#define  PCIE_DM_AINTACK_CFGAERERRMSI_MASK 0x400000
#define  PCIE_DM_AINTACK_CFGAERERRMSI_SHIFT 22
#define  PCIE_DM_AINTACK_CFGAERERRMSI_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_CFGAERERRMSI_MASK) >> PCIE_DM_AINTACK_CFGAERERRMSI_SHIFT)
#define  PCIE_DM_AINTACK_CFGAERERRMSI_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_CFGAERERRMSI_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_CFGAERERRMSI_SHIFT))

#define  PCIE_DM_AINTACK_RESERVED2_MASK 0x3e0000
#define  PCIE_DM_AINTACK_RESERVED2_SHIFT 17
#define  PCIE_DM_AINTACK_RESERVED2_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RESERVED2_MASK) >> PCIE_DM_AINTACK_RESERVED2_SHIFT)
#define  PCIE_DM_AINTACK_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RESERVED2_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RESERVED2_SHIFT))

#define  PCIE_DM_AINTACK_INTP_DIAG_MASK 0x10000
#define  PCIE_DM_AINTACK_INTP_DIAG_SHIFT 16
#define  PCIE_DM_AINTACK_INTP_DIAG_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_INTP_DIAG_MASK) >> PCIE_DM_AINTACK_INTP_DIAG_SHIFT)
#define  PCIE_DM_AINTACK_INTP_DIAG_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_INTP_DIAG_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_INTP_DIAG_SHIFT))

#define  PCIE_DM_AINTACK_RADMFATAL_MASK 0x8000
#define  PCIE_DM_AINTACK_RADMFATAL_SHIFT 15
#define  PCIE_DM_AINTACK_RADMFATAL_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMFATAL_MASK) >> PCIE_DM_AINTACK_RADMFATAL_SHIFT)
#define  PCIE_DM_AINTACK_RADMFATAL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMFATAL_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMFATAL_SHIFT))

#define  PCIE_DM_AINTACK_RADMNONFATAL_MASK 0x4000
#define  PCIE_DM_AINTACK_RADMNONFATAL_SHIFT 14
#define  PCIE_DM_AINTACK_RADMNONFATAL_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMNONFATAL_MASK) >> PCIE_DM_AINTACK_RADMNONFATAL_SHIFT)
#define  PCIE_DM_AINTACK_RADMNONFATAL_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMNONFATAL_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMNONFATAL_SHIFT))

#define  PCIE_DM_AINTACK_RADMCORRERR_MASK 0x2000
#define  PCIE_DM_AINTACK_RADMCORRERR_SHIFT 13
#define  PCIE_DM_AINTACK_RADMCORRERR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMCORRERR_MASK) >> PCIE_DM_AINTACK_RADMCORRERR_SHIFT)
#define  PCIE_DM_AINTACK_RADMCORRERR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMCORRERR_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMCORRERR_SHIFT))

#define  PCIE_DM_AINTACK_RADMPME_MASK 0x1000
#define  PCIE_DM_AINTACK_RADMPME_SHIFT 12
#define  PCIE_DM_AINTACK_RADMPME_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMPME_MASK) >> PCIE_DM_AINTACK_RADMPME_SHIFT)
#define  PCIE_DM_AINTACK_RADMPME_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMPME_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMPME_SHIFT))

#define  PCIE_DM_AINTACK_RADMCPLTIMEOUT_MASK 0x800
#define  PCIE_DM_AINTACK_RADMCPLTIMEOUT_SHIFT 11
#define  PCIE_DM_AINTACK_RADMCPLTIMEOUT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMCPLTIMEOUT_MASK) >> PCIE_DM_AINTACK_RADMCPLTIMEOUT_SHIFT)
#define  PCIE_DM_AINTACK_RADMCPLTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMCPLTIMEOUT_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMCPLTIMEOUT_SHIFT))

#define  PCIE_DM_AINTACK_RESERVED3_MASK 0x400
#define  PCIE_DM_AINTACK_RESERVED3_SHIFT 10
#define  PCIE_DM_AINTACK_RESERVED3_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RESERVED3_MASK) >> PCIE_DM_AINTACK_RESERVED3_SHIFT)
#define  PCIE_DM_AINTACK_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RESERVED3_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RESERVED3_SHIFT))

#define  PCIE_DM_AINTACK_RADMCPLUR_MASK 0x200
#define  PCIE_DM_AINTACK_RADMCPLUR_SHIFT 9
#define  PCIE_DM_AINTACK_RADMCPLUR_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMCPLUR_MASK) >> PCIE_DM_AINTACK_RADMCPLUR_SHIFT)
#define  PCIE_DM_AINTACK_RADMCPLUR_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMCPLUR_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMCPLUR_SHIFT))

#define  PCIE_DM_AINTACK_RADMCA_MASK 0x100
#define  PCIE_DM_AINTACK_RADMCA_SHIFT 8
#define  PCIE_DM_AINTACK_RADMCA_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RADMCA_MASK) >> PCIE_DM_AINTACK_RADMCA_SHIFT)
#define  PCIE_DM_AINTACK_RADMCA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RADMCA_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RADMCA_SHIFT))

#define  PCIE_DM_AINTACK_RESERVED4_MASK 0xe0
#define  PCIE_DM_AINTACK_RESERVED4_SHIFT 5
#define  PCIE_DM_AINTACK_RESERVED4_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RESERVED4_MASK) >> PCIE_DM_AINTACK_RESERVED4_SHIFT)
#define  PCIE_DM_AINTACK_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RESERVED4_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RESERVED4_SHIFT))

#define  PCIE_DM_AINTACK_TRGTCPLTIMEOUT_MASK 0x10
#define  PCIE_DM_AINTACK_TRGTCPLTIMEOUT_SHIFT 4
#define  PCIE_DM_AINTACK_TRGTCPLTIMEOUT_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_TRGTCPLTIMEOUT_MASK) >> PCIE_DM_AINTACK_TRGTCPLTIMEOUT_SHIFT)
#define  PCIE_DM_AINTACK_TRGTCPLTIMEOUT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_TRGTCPLTIMEOUT_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_TRGTCPLTIMEOUT_SHIFT))

#define  PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_MASK 0x8
#define  PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_SHIFT 3
#define  PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_MASK) >> PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_SHIFT)
#define  PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_TRGTLOOKUPEMPTY_SHIFT))

#define  PCIE_DM_AINTACK_RESERVED5_MASK 0x6
#define  PCIE_DM_AINTACK_RESERVED5_SHIFT 1
#define  PCIE_DM_AINTACK_RESERVED5_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_RESERVED5_MASK) >> PCIE_DM_AINTACK_RESERVED5_SHIFT)
#define  PCIE_DM_AINTACK_RESERVED5_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_RESERVED5_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_RESERVED5_SHIFT))

#define  PCIE_DM_AINTACK_TRNGRESET_MASK 0x1
#define  PCIE_DM_AINTACK_TRNGRESET_SHIFT 0
#define  PCIE_DM_AINTACK_TRNGRESET_MASK_SHIFT(reg) (((reg) & PCIE_DM_AINTACK_TRNGRESET_MASK) >> PCIE_DM_AINTACK_TRNGRESET_SHIFT)
#define  PCIE_DM_AINTACK_TRNGRESET_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_AINTACK_TRNGRESET_MASK) | (((uint32_t)val) << PCIE_DM_AINTACK_TRNGRESET_SHIFT))

//====================================================================
//Register: DM Application Inbound Address Translation Register 0 (InAddrXlt0)
/** \brief DM Core Application Inbound Address Translation Register.*/
//====================================================================

#define  PCIE_DM_INADDRXLT0_MEM_MASK 0xffffffff
#define  PCIE_DM_INADDRXLT0_MEM_SHIFT 0
#define  PCIE_DM_INADDRXLT0_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_INADDRXLT0_MEM_MASK) >> PCIE_DM_INADDRXLT0_MEM_SHIFT)
#define  PCIE_DM_INADDRXLT0_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_INADDRXLT0_MEM_MASK) | (((uint32_t)val) << PCIE_DM_INADDRXLT0_MEM_SHIFT))

//====================================================================
//Register: DM Application Inbound Address Translation Register 1 (InAddrXlt1)
/** \brief DM Core Application Inbound Address Translation Register.*/
//====================================================================

#define  PCIE_DM_INADDRXLT1_MEM_MASK 0xffffffff
#define  PCIE_DM_INADDRXLT1_MEM_SHIFT 0
#define  PCIE_DM_INADDRXLT1_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_INADDRXLT1_MEM_MASK) >> PCIE_DM_INADDRXLT1_MEM_SHIFT)
#define  PCIE_DM_INADDRXLT1_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_INADDRXLT1_MEM_MASK) | (((uint32_t)val) << PCIE_DM_INADDRXLT1_MEM_SHIFT))

//====================================================================
//Register: DM Application Inbound Address Translation Register 2 (InAddrXlt2)
/** \brief DM Core Application Inbound Address Translation Register.*/
//====================================================================

#define  PCIE_DM_INADDRXLT2_MEM_MASK 0xffffffff
#define  PCIE_DM_INADDRXLT2_MEM_SHIFT 0
#define  PCIE_DM_INADDRXLT2_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_INADDRXLT2_MEM_MASK) >> PCIE_DM_INADDRXLT2_MEM_SHIFT)
#define  PCIE_DM_INADDRXLT2_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_INADDRXLT2_MEM_MASK) | (((uint32_t)val) << PCIE_DM_INADDRXLT2_MEM_SHIFT))

//====================================================================
//Register: DM Application BAR0 (BAR0)
/** \brief Used for Inbound Address Translation logic. Set to the same value as the PCIe Core BAR0 register.*/
//====================================================================

#define  PCIE_DM_BAR0_BASE_MASK 0xf8000000
#define  PCIE_DM_BAR0_BASE_SHIFT 27
#define  PCIE_DM_BAR0_BASE_MASK_SHIFT(reg) (((reg) & PCIE_DM_BAR0_BASE_MASK) >> PCIE_DM_BAR0_BASE_SHIFT)
#define  PCIE_DM_BAR0_BASE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_BAR0_BASE_MASK) | (((uint32_t)val) << PCIE_DM_BAR0_BASE_SHIFT))

#define  PCIE_DM_BAR0_RESERVED1_MASK 0x7ffffff
#define  PCIE_DM_BAR0_RESERVED1_SHIFT 0
#define  PCIE_DM_BAR0_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_BAR0_RESERVED1_MASK) >> PCIE_DM_BAR0_RESERVED1_SHIFT)
#define  PCIE_DM_BAR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_BAR0_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_BAR0_RESERVED1_SHIFT))

//====================================================================
//Register: DM Application Window Miss Register (WinMiss)
//====================================================================

#define  PCIE_DM_WINMISS_MEM_MASK 0xffffffff
#define  PCIE_DM_WINMISS_MEM_SHIFT 0
#define  PCIE_DM_WINMISS_MEM_MASK_SHIFT(reg) (((reg) & PCIE_DM_WINMISS_MEM_MASK) >> PCIE_DM_WINMISS_MEM_SHIFT)
#define  PCIE_DM_WINMISS_MEM_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_WINMISS_MEM_MASK) | (((uint32_t)val) << PCIE_DM_WINMISS_MEM_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt0 Mask  (RC_MSI_IRQ0_Mask)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ0_MASK_MASK_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ0_MASK_MASK_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ0_MASK_MASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ0_MASK_MASK_MASK) >> PCIE_DM_RC_MSI_IRQ0_MASK_MASK_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ0_MASK_MASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ0_MASK_MASK_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ0_MASK_MASK_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt0 (RC_MSI_IRQ0)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ0_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ0_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ0_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ0_INT_MASK) >> PCIE_DM_RC_MSI_IRQ0_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ0_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ0_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ0_INT_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt0 Clear (RC_MSI_IRQ0_Clr)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ0_CLR_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ0_CLR_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ0_CLR_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ0_CLR_INT_MASK) >> PCIE_DM_RC_MSI_IRQ0_CLR_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ0_CLR_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ0_CLR_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ0_CLR_INT_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt1 Mask  (RC_MSI_IRQ1_Mask)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ1_MASK_MASK_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ1_MASK_MASK_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ1_MASK_MASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ1_MASK_MASK_MASK) >> PCIE_DM_RC_MSI_IRQ1_MASK_MASK_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ1_MASK_MASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ1_MASK_MASK_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ1_MASK_MASK_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt1 (RC_MSI_IRQ1)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ1_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ1_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ1_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ1_INT_MASK) >> PCIE_DM_RC_MSI_IRQ1_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ1_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ1_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ1_INT_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt1 Clear (RC_MSI_IRQ1_Clr)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ1_CLR_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ1_CLR_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ1_CLR_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ1_CLR_INT_MASK) >> PCIE_DM_RC_MSI_IRQ1_CLR_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ1_CLR_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ1_CLR_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ1_CLR_INT_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt2 Mask  (RC_MSI_IRQ2_Mask)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ2_MASK_MASK_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ2_MASK_MASK_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ2_MASK_MASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ2_MASK_MASK_MASK) >> PCIE_DM_RC_MSI_IRQ2_MASK_MASK_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ2_MASK_MASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ2_MASK_MASK_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ2_MASK_MASK_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt2 (RC_MSI_IRQ2)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ2_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ2_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ2_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ2_INT_MASK) >> PCIE_DM_RC_MSI_IRQ2_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ2_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ2_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ2_INT_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt2 Clear (RC_MSI_IRQ2_Clr)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ2_CLR_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ2_CLR_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ2_CLR_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ2_CLR_INT_MASK) >> PCIE_DM_RC_MSI_IRQ2_CLR_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ2_CLR_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ2_CLR_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ2_CLR_INT_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt3 Mask  (RC_MSI_IRQ3_Mask)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ3_MASK_MASK_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ3_MASK_MASK_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ3_MASK_MASK_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ3_MASK_MASK_MASK) >> PCIE_DM_RC_MSI_IRQ3_MASK_MASK_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ3_MASK_MASK_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ3_MASK_MASK_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ3_MASK_MASK_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt3 (RC_MSI_IRQ3)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ3_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ3_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ3_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ3_INT_MASK) >> PCIE_DM_RC_MSI_IRQ3_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ3_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ3_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ3_INT_SHIFT))

//====================================================================
//Register: DM Application RC MSI Interrupt3 Clear (RC_MSI_IRQ3_Clr)
//====================================================================

#define  PCIE_DM_RC_MSI_IRQ3_CLR_INT_MASK 0xffffffff
#define  PCIE_DM_RC_MSI_IRQ3_CLR_INT_SHIFT 0
#define  PCIE_DM_RC_MSI_IRQ3_CLR_INT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_MSI_IRQ3_CLR_INT_MASK) >> PCIE_DM_RC_MSI_IRQ3_CLR_INT_SHIFT)
#define  PCIE_DM_RC_MSI_IRQ3_CLR_INT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_MSI_IRQ3_CLR_INT_MASK) | (((uint32_t)val) << PCIE_DM_RC_MSI_IRQ3_CLR_INT_SHIFT))

//====================================================================
//Register: DM Application INTA Interrupt Enable (RC_INTA_IRQ_En)
//====================================================================

#define  PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_MASK 0xfffffffe
#define  PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_SHIFT 1
#define  PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_MASK) >> PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTA_IRQ_EN_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTA_IRQ_EN_EN_MASK 0x1
#define  PCIE_DM_RC_INTA_IRQ_EN_EN_SHIFT 0
#define  PCIE_DM_RC_INTA_IRQ_EN_EN_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTA_IRQ_EN_EN_MASK) >> PCIE_DM_RC_INTA_IRQ_EN_EN_SHIFT)
#define  PCIE_DM_RC_INTA_IRQ_EN_EN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTA_IRQ_EN_EN_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTA_IRQ_EN_EN_SHIFT))

//====================================================================
//Register: DM Application INTA Count Value (RC_INTA_IRQ_Cnt)
//====================================================================

#define  PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_MASK 0xfffffff0
#define  PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_SHIFT 4
#define  PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_MASK) >> PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTA_IRQ_CNT_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTA_IRQ_CNT_CNT_MASK 0xf
#define  PCIE_DM_RC_INTA_IRQ_CNT_CNT_SHIFT 0
#define  PCIE_DM_RC_INTA_IRQ_CNT_CNT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTA_IRQ_CNT_CNT_MASK) >> PCIE_DM_RC_INTA_IRQ_CNT_CNT_SHIFT)
#define  PCIE_DM_RC_INTA_IRQ_CNT_CNT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTA_IRQ_CNT_CNT_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTA_IRQ_CNT_CNT_SHIFT))

//====================================================================
//Register: DM Application INTB Interrupt Enable (RC_INTB_IRQ_En)
//====================================================================

#define  PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_MASK 0xfffffffe
#define  PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_SHIFT 1
#define  PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_MASK) >> PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTB_IRQ_EN_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTB_IRQ_EN_EN_MASK 0x1
#define  PCIE_DM_RC_INTB_IRQ_EN_EN_SHIFT 0
#define  PCIE_DM_RC_INTB_IRQ_EN_EN_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTB_IRQ_EN_EN_MASK) >> PCIE_DM_RC_INTB_IRQ_EN_EN_SHIFT)
#define  PCIE_DM_RC_INTB_IRQ_EN_EN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTB_IRQ_EN_EN_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTB_IRQ_EN_EN_SHIFT))

//====================================================================
//Register: DM Application INTB Count Value (RC_INTB_IRQ_Cnt)
//====================================================================

#define  PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_MASK 0xfffffff0
#define  PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_SHIFT 4
#define  PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_MASK) >> PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTB_IRQ_CNT_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTB_IRQ_CNT_CNT_MASK 0xf
#define  PCIE_DM_RC_INTB_IRQ_CNT_CNT_SHIFT 0
#define  PCIE_DM_RC_INTB_IRQ_CNT_CNT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTB_IRQ_CNT_CNT_MASK) >> PCIE_DM_RC_INTB_IRQ_CNT_CNT_SHIFT)
#define  PCIE_DM_RC_INTB_IRQ_CNT_CNT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTB_IRQ_CNT_CNT_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTB_IRQ_CNT_CNT_SHIFT))

//====================================================================
//Register: DM Application INTC Interrupt Enable (RC_INTC_IRQ_En)
//====================================================================

#define  PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_MASK 0xfffffffe
#define  PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_SHIFT 1
#define  PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_MASK) >> PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTC_IRQ_EN_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTC_IRQ_EN_EN_MASK 0x1
#define  PCIE_DM_RC_INTC_IRQ_EN_EN_SHIFT 0
#define  PCIE_DM_RC_INTC_IRQ_EN_EN_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTC_IRQ_EN_EN_MASK) >> PCIE_DM_RC_INTC_IRQ_EN_EN_SHIFT)
#define  PCIE_DM_RC_INTC_IRQ_EN_EN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTC_IRQ_EN_EN_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTC_IRQ_EN_EN_SHIFT))

//====================================================================
//Register: DM Application INTC Count Value (RC_INTC_IRQ_Cnt)
//====================================================================

#define  PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_MASK 0xfffffff0
#define  PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_SHIFT 4
#define  PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_MASK) >> PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTC_IRQ_CNT_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTC_IRQ_CNT_CNT_MASK 0xf
#define  PCIE_DM_RC_INTC_IRQ_CNT_CNT_SHIFT 0
#define  PCIE_DM_RC_INTC_IRQ_CNT_CNT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTC_IRQ_CNT_CNT_MASK) >> PCIE_DM_RC_INTC_IRQ_CNT_CNT_SHIFT)
#define  PCIE_DM_RC_INTC_IRQ_CNT_CNT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTC_IRQ_CNT_CNT_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTC_IRQ_CNT_CNT_SHIFT))

//====================================================================
//Register: DM Application INTD Interrupt Enable (RC_INTD_IRQ_En)
//====================================================================

#define  PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_MASK 0xfffffffe
#define  PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_SHIFT 1
#define  PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_MASK) >> PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTD_IRQ_EN_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTD_IRQ_EN_EN_MASK 0x1
#define  PCIE_DM_RC_INTD_IRQ_EN_EN_SHIFT 0
#define  PCIE_DM_RC_INTD_IRQ_EN_EN_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTD_IRQ_EN_EN_MASK) >> PCIE_DM_RC_INTD_IRQ_EN_EN_SHIFT)
#define  PCIE_DM_RC_INTD_IRQ_EN_EN_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTD_IRQ_EN_EN_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTD_IRQ_EN_EN_SHIFT))

//====================================================================
//Register: DM Application INTD Count Value (RC_INTD_IRQ_Cnt)
//====================================================================

#define  PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_MASK 0xfffffff0
#define  PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_SHIFT 4
#define  PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_MASK) >> PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_SHIFT)
#define  PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTD_IRQ_CNT_RESERVED1_SHIFT))

#define  PCIE_DM_RC_INTD_IRQ_CNT_CNT_MASK 0xf
#define  PCIE_DM_RC_INTD_IRQ_CNT_CNT_SHIFT 0
#define  PCIE_DM_RC_INTD_IRQ_CNT_CNT_MASK_SHIFT(reg) (((reg) & PCIE_DM_RC_INTD_IRQ_CNT_CNT_MASK) >> PCIE_DM_RC_INTD_IRQ_CNT_CNT_SHIFT)
#define  PCIE_DM_RC_INTD_IRQ_CNT_CNT_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_RC_INTD_IRQ_CNT_CNT_MASK) | (((uint32_t)val) << PCIE_DM_RC_INTD_IRQ_CNT_CNT_SHIFT))

//====================================================================
//Register: DM Application - EP INTA Generate (EP_INTA_Gen)
//====================================================================

#define  PCIE_DM_EP_INTA_GEN_RESERVED1_MASK 0xfffffffe
#define  PCIE_DM_EP_INTA_GEN_RESERVED1_SHIFT 1
#define  PCIE_DM_EP_INTA_GEN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_EP_INTA_GEN_RESERVED1_MASK) >> PCIE_DM_EP_INTA_GEN_RESERVED1_SHIFT)
#define  PCIE_DM_EP_INTA_GEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_EP_INTA_GEN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_EP_INTA_GEN_RESERVED1_SHIFT))

#define  PCIE_DM_EP_INTA_GEN_TOGGLE_MASK 0x1
#define  PCIE_DM_EP_INTA_GEN_TOGGLE_SHIFT 0
#define  PCIE_DM_EP_INTA_GEN_TOGGLE_MASK_SHIFT(reg) (((reg) & PCIE_DM_EP_INTA_GEN_TOGGLE_MASK) >> PCIE_DM_EP_INTA_GEN_TOGGLE_SHIFT)
#define  PCIE_DM_EP_INTA_GEN_TOGGLE_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_EP_INTA_GEN_TOGGLE_MASK) | (((uint32_t)val) << PCIE_DM_EP_INTA_GEN_TOGGLE_SHIFT))

//====================================================================
//Register: DM Application - EP MSI Generate (EP_MSI_Gen)
//====================================================================

#define  PCIE_DM_EP_MSI_GEN_RESERVED1_MASK 0xffffffe0
#define  PCIE_DM_EP_MSI_GEN_RESERVED1_SHIFT 5
#define  PCIE_DM_EP_MSI_GEN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_EP_MSI_GEN_RESERVED1_MASK) >> PCIE_DM_EP_MSI_GEN_RESERVED1_SHIFT)
#define  PCIE_DM_EP_MSI_GEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_EP_MSI_GEN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_EP_MSI_GEN_RESERVED1_SHIFT))

#define  PCIE_DM_EP_MSI_GEN_DATA_MASK 0x1f
#define  PCIE_DM_EP_MSI_GEN_DATA_SHIFT 0
#define  PCIE_DM_EP_MSI_GEN_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_EP_MSI_GEN_DATA_MASK) >> PCIE_DM_EP_MSI_GEN_DATA_SHIFT)
#define  PCIE_DM_EP_MSI_GEN_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_EP_MSI_GEN_DATA_MASK) | (((uint32_t)val) << PCIE_DM_EP_MSI_GEN_DATA_SHIFT))

//====================================================================
//Register: DM Application - EP MSI Generate Enabled (EP_MSI_Gen_En)
//====================================================================

#define  PCIE_DM_EP_MSI_GEN_EN_RESERVED1_MASK 0xfffffffe
#define  PCIE_DM_EP_MSI_GEN_EN_RESERVED1_SHIFT 1
#define  PCIE_DM_EP_MSI_GEN_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & PCIE_DM_EP_MSI_GEN_EN_RESERVED1_MASK) >> PCIE_DM_EP_MSI_GEN_EN_RESERVED1_SHIFT)
#define  PCIE_DM_EP_MSI_GEN_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_EP_MSI_GEN_EN_RESERVED1_MASK) | (((uint32_t)val) << PCIE_DM_EP_MSI_GEN_EN_RESERVED1_SHIFT))

#define  PCIE_DM_EP_MSI_GEN_EN_DATA_MASK 0x1
#define  PCIE_DM_EP_MSI_GEN_EN_DATA_SHIFT 0
#define  PCIE_DM_EP_MSI_GEN_EN_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_EP_MSI_GEN_EN_DATA_MASK) >> PCIE_DM_EP_MSI_GEN_EN_DATA_SHIFT)
#define  PCIE_DM_EP_MSI_GEN_EN_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_EP_MSI_GEN_EN_DATA_MASK) | (((uint32_t)val) << PCIE_DM_EP_MSI_GEN_EN_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 1 of 8) (MB_WR_En0)
/** \brief  (Instance 1 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN0_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN0_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN0_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN0_DATA_MASK) >> PCIE_DM_MB_WR_EN0_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN0_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN0_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN0_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 2 of 8) (MB_WR_En1)
/** \brief  (Instance 2 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN1_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN1_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN1_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN1_DATA_MASK) >> PCIE_DM_MB_WR_EN1_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN1_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN1_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN1_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 3 of 8) (MB_WR_En2)
/** \brief  (Instance 3 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN2_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN2_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN2_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN2_DATA_MASK) >> PCIE_DM_MB_WR_EN2_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN2_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN2_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN2_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 4 of 8) (MB_WR_En3)
/** \brief  (Instance 4 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN3_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN3_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN3_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN3_DATA_MASK) >> PCIE_DM_MB_WR_EN3_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN3_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN3_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN3_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 5 of 8) (MB_WR_En4)
/** \brief  (Instance 5 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN4_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN4_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN4_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN4_DATA_MASK) >> PCIE_DM_MB_WR_EN4_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN4_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN4_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN4_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 6 of 8) (MB_WR_En5)
/** \brief  (Instance 6 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN5_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN5_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN5_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN5_DATA_MASK) >> PCIE_DM_MB_WR_EN5_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN5_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN5_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN5_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 7 of 8) (MB_WR_En6)
/** \brief  (Instance 7 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN6_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN6_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN6_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN6_DATA_MASK) >> PCIE_DM_MB_WR_EN6_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN6_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN6_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN6_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Enable Register 0-7 (Instance 8 of 8) (MB_WR_En7)
/** \brief  (Instance 8 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_EN7_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_EN7_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_EN7_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_EN7_DATA_MASK) >> PCIE_DM_MB_WR_EN7_DATA_SHIFT)
#define  PCIE_DM_MB_WR_EN7_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_EN7_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_EN7_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 1 of 8) (MB_WR0)
/** \brief  (Instance 1 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR0_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR0_DATA_SHIFT 0
#define  PCIE_DM_MB_WR0_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR0_DATA_MASK) >> PCIE_DM_MB_WR0_DATA_SHIFT)
#define  PCIE_DM_MB_WR0_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR0_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR0_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 2 of 8) (MB_WR1)
/** \brief  (Instance 2 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR1_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR1_DATA_SHIFT 0
#define  PCIE_DM_MB_WR1_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR1_DATA_MASK) >> PCIE_DM_MB_WR1_DATA_SHIFT)
#define  PCIE_DM_MB_WR1_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR1_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR1_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 3 of 8) (MB_WR2)
/** \brief  (Instance 3 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR2_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR2_DATA_SHIFT 0
#define  PCIE_DM_MB_WR2_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR2_DATA_MASK) >> PCIE_DM_MB_WR2_DATA_SHIFT)
#define  PCIE_DM_MB_WR2_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR2_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR2_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 4 of 8) (MB_WR3)
/** \brief  (Instance 4 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR3_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR3_DATA_SHIFT 0
#define  PCIE_DM_MB_WR3_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR3_DATA_MASK) >> PCIE_DM_MB_WR3_DATA_SHIFT)
#define  PCIE_DM_MB_WR3_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR3_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR3_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 5 of 8) (MB_WR4)
/** \brief  (Instance 5 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR4_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR4_DATA_SHIFT 0
#define  PCIE_DM_MB_WR4_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR4_DATA_MASK) >> PCIE_DM_MB_WR4_DATA_SHIFT)
#define  PCIE_DM_MB_WR4_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR4_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR4_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 6 of 8) (MB_WR5)
/** \brief  (Instance 6 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR5_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR5_DATA_SHIFT 0
#define  PCIE_DM_MB_WR5_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR5_DATA_MASK) >> PCIE_DM_MB_WR5_DATA_SHIFT)
#define  PCIE_DM_MB_WR5_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR5_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR5_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 7 of 8) (MB_WR6)
/** \brief  (Instance 7 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR6_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR6_DATA_SHIFT 0
#define  PCIE_DM_MB_WR6_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR6_DATA_MASK) >> PCIE_DM_MB_WR6_DATA_SHIFT)
#define  PCIE_DM_MB_WR6_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR6_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR6_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register 0-7 (Instance 8 of 8) (MB_WR7)
/** \brief  (Instance 8 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR7_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR7_DATA_SHIFT 0
#define  PCIE_DM_MB_WR7_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR7_DATA_MASK) >> PCIE_DM_MB_WR7_DATA_SHIFT)
#define  PCIE_DM_MB_WR7_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR7_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR7_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 1 of 8) (MB_WR_Stat0)
/** \brief  (Instance 1 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT0_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT0_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT0_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT0_DATA_MASK) >> PCIE_DM_MB_WR_STAT0_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT0_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT0_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT0_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 2 of 8) (MB_WR_Stat1)
/** \brief  (Instance 2 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT1_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT1_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT1_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT1_DATA_MASK) >> PCIE_DM_MB_WR_STAT1_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT1_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT1_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT1_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 3 of 8) (MB_WR_Stat2)
/** \brief  (Instance 3 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT2_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT2_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT2_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT2_DATA_MASK) >> PCIE_DM_MB_WR_STAT2_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT2_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT2_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT2_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 4 of 8) (MB_WR_Stat3)
/** \brief  (Instance 4 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT3_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT3_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT3_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT3_DATA_MASK) >> PCIE_DM_MB_WR_STAT3_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT3_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT3_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT3_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 5 of 8) (MB_WR_Stat4)
/** \brief  (Instance 5 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT4_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT4_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT4_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT4_DATA_MASK) >> PCIE_DM_MB_WR_STAT4_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT4_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT4_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT4_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 6 of 8) (MB_WR_Stat5)
/** \brief  (Instance 6 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT5_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT5_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT5_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT5_DATA_MASK) >> PCIE_DM_MB_WR_STAT5_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT5_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT5_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT5_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 7 of 8) (MB_WR_Stat6)
/** \brief  (Instance 7 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT6_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT6_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT6_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT6_DATA_MASK) >> PCIE_DM_MB_WR_STAT6_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT6_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT6_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT6_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Register Status 0-7 (Instance 8 of 8) (MB_WR_Stat7)
/** \brief  (Instance 8 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_STAT7_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_STAT7_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_STAT7_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_STAT7_DATA_MASK) >> PCIE_DM_MB_WR_STAT7_DATA_SHIFT)
#define  PCIE_DM_MB_WR_STAT7_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_STAT7_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_STAT7_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 1 of 8) (MB_WR_Ack0)
/** \brief  (Instance 1 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK0_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK0_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK0_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK0_DATA_MASK) >> PCIE_DM_MB_WR_ACK0_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK0_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK0_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK0_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 2 of 8) (MB_WR_Ack1)
/** \brief  (Instance 2 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK1_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK1_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK1_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK1_DATA_MASK) >> PCIE_DM_MB_WR_ACK1_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK1_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK1_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK1_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 3 of 8) (MB_WR_Ack2)
/** \brief  (Instance 3 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK2_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK2_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK2_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK2_DATA_MASK) >> PCIE_DM_MB_WR_ACK2_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK2_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK2_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK2_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 4 of 8) (MB_WR_Ack3)
/** \brief  (Instance 4 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK3_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK3_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK3_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK3_DATA_MASK) >> PCIE_DM_MB_WR_ACK3_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK3_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK3_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK3_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 5 of 8) (MB_WR_Ack4)
/** \brief  (Instance 5 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK4_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK4_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK4_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK4_DATA_MASK) >> PCIE_DM_MB_WR_ACK4_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK4_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK4_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK4_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 6 of 8) (MB_WR_Ack5)
/** \brief  (Instance 6 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK5_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK5_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK5_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK5_DATA_MASK) >> PCIE_DM_MB_WR_ACK5_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK5_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK5_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK5_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 7 of 8) (MB_WR_Ack6)
/** \brief  (Instance 7 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK6_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK6_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK6_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK6_DATA_MASK) >> PCIE_DM_MB_WR_ACK6_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK6_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK6_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK6_DATA_SHIFT))

//====================================================================
//Register: DM Application - Mailbox Write Interrupt Acknowledge Register 0-7 (Instance 8 of 8) (MB_WR_Ack7)
/** \brief  (Instance 8 of 8)*/
//====================================================================

#define  PCIE_DM_MB_WR_ACK7_DATA_MASK 0xffffffff
#define  PCIE_DM_MB_WR_ACK7_DATA_SHIFT 0
#define  PCIE_DM_MB_WR_ACK7_DATA_MASK_SHIFT(reg) (((reg) & PCIE_DM_MB_WR_ACK7_DATA_MASK) >> PCIE_DM_MB_WR_ACK7_DATA_SHIFT)
#define  PCIE_DM_MB_WR_ACK7_DATA_REPLACE_VAL(reg,val) (((reg) & ~PCIE_DM_MB_WR_ACK7_DATA_MASK) | (((uint32_t)val) << PCIE_DM_MB_WR_ACK7_DATA_SHIFT))

#endif // PCIE
