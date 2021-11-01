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
 * \file HiPWM_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _HIPWM_REGMASKS_H_
#define _HIPWM_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: HiPWM (HiPWM)
/** \brief Top-level register file for HiPWM block*/
//
//====================================================================

//====================================================================
//Register: HiPWM Configuration Register (HCR)
/** \brief This register configures the major features of the HiPWM module*/
//====================================================================

#define  HIPWM_HCR_RESERVED1_MASK 0xffe00000
#define  HIPWM_HCR_RESERVED1_SHIFT 21
#define  HIPWM_HCR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_RESERVED1_MASK) >> HIPWM_HCR_RESERVED1_SHIFT)
#define  HIPWM_HCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HCR_RESERVED1_SHIFT))

#define  HIPWM_HCR_EDGESEL_MASK 0x100000
#define  HIPWM_HCR_EDGESEL_SHIFT 20
#define  HIPWM_HCR_EDGESEL_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_EDGESEL_MASK) >> HIPWM_HCR_EDGESEL_SHIFT)
#define  HIPWM_HCR_EDGESEL_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_EDGESEL_MASK) | (((uint32_t)val) << HIPWM_HCR_EDGESEL_SHIFT))

#define  HIPWM_HCR_NUMEDGE_MASK 0x80000
#define  HIPWM_HCR_NUMEDGE_SHIFT 19
#define  HIPWM_HCR_NUMEDGE_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_NUMEDGE_MASK) >> HIPWM_HCR_NUMEDGE_SHIFT)
#define  HIPWM_HCR_NUMEDGE_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_NUMEDGE_MASK) | (((uint32_t)val) << HIPWM_HCR_NUMEDGE_SHIFT))

#define  HIPWM_HCR_CFGDECVIDEOPIGEN_MASK 0x40000
#define  HIPWM_HCR_CFGDECVIDEOPIGEN_SHIFT 18
#define  HIPWM_HCR_CFGDECVIDEOPIGEN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_CFGDECVIDEOPIGEN_MASK) >> HIPWM_HCR_CFGDECVIDEOPIGEN_SHIFT)
#define  HIPWM_HCR_CFGDECVIDEOPIGEN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_CFGDECVIDEOPIGEN_MASK) | (((uint32_t)val) << HIPWM_HCR_CFGDECVIDEOPIGEN_SHIFT))

#define  HIPWM_HCR_CFGDECVIDEODATAEN_MASK 0x20000
#define  HIPWM_HCR_CFGDECVIDEODATAEN_SHIFT 17
#define  HIPWM_HCR_CFGDECVIDEODATAEN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_CFGDECVIDEODATAEN_MASK) >> HIPWM_HCR_CFGDECVIDEODATAEN_SHIFT)
#define  HIPWM_HCR_CFGDECVIDEODATAEN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_CFGDECVIDEODATAEN_MASK) | (((uint32_t)val) << HIPWM_HCR_CFGDECVIDEODATAEN_SHIFT))

#define  HIPWM_HCR_CFGDECFORCELASERONEN_MASK 0x10000
#define  HIPWM_HCR_CFGDECFORCELASERONEN_SHIFT 16
#define  HIPWM_HCR_CFGDECFORCELASERONEN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_CFGDECFORCELASERONEN_MASK) >> HIPWM_HCR_CFGDECFORCELASERONEN_SHIFT)
#define  HIPWM_HCR_CFGDECFORCELASERONEN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_CFGDECFORCELASERONEN_MASK) | (((uint32_t)val) << HIPWM_HCR_CFGDECFORCELASERONEN_SHIFT))

#define  HIPWM_HCR_NOBATLAST_MASK 0x8000
#define  HIPWM_HCR_NOBATLAST_SHIFT 15
#define  HIPWM_HCR_NOBATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_NOBATLAST_MASK) >> HIPWM_HCR_NOBATLAST_SHIFT)
#define  HIPWM_HCR_NOBATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_NOBATLAST_MASK) | (((uint32_t)val) << HIPWM_HCR_NOBATLAST_SHIFT))

#define  HIPWM_HCR_DUALMARGIN_MASK 0x4000
#define  HIPWM_HCR_DUALMARGIN_SHIFT 14
#define  HIPWM_HCR_DUALMARGIN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_DUALMARGIN_MASK) >> HIPWM_HCR_DUALMARGIN_SHIFT)
#define  HIPWM_HCR_DUALMARGIN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_DUALMARGIN_MASK) | (((uint32_t)val) << HIPWM_HCR_DUALMARGIN_SHIFT))

#define  HIPWM_HCR_FILTERPULSES_MASK 0x2000
#define  HIPWM_HCR_FILTERPULSES_SHIFT 13
#define  HIPWM_HCR_FILTERPULSES_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_FILTERPULSES_MASK) >> HIPWM_HCR_FILTERPULSES_SHIFT)
#define  HIPWM_HCR_FILTERPULSES_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_FILTERPULSES_MASK) | (((uint32_t)val) << HIPWM_HCR_FILTERPULSES_SHIFT))

#define  HIPWM_HCR_CRCRST_MASK 0x1000
#define  HIPWM_HCR_CRCRST_SHIFT 12
#define  HIPWM_HCR_CRCRST_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_CRCRST_MASK) >> HIPWM_HCR_CRCRST_SHIFT)
#define  HIPWM_HCR_CRCRST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_CRCRST_MASK) | (((uint32_t)val) << HIPWM_HCR_CRCRST_SHIFT))

#define  HIPWM_HCR_HIPSDIR_MASK 0x800
#define  HIPWM_HCR_HIPSDIR_SHIFT 11
#define  HIPWM_HCR_HIPSDIR_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_HIPSDIR_MASK) >> HIPWM_HCR_HIPSDIR_SHIFT)
#define  HIPWM_HCR_HIPSDIR_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_HIPSDIR_MASK) | (((uint32_t)val) << HIPWM_HCR_HIPSDIR_SHIFT))

#define  HIPWM_HCR_PTWIDTH_MASK 0x400
#define  HIPWM_HCR_PTWIDTH_SHIFT 10
#define  HIPWM_HCR_PTWIDTH_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_PTWIDTH_MASK) >> HIPWM_HCR_PTWIDTH_SHIFT)
#define  HIPWM_HCR_PTWIDTH_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_PTWIDTH_MASK) | (((uint32_t)val) << HIPWM_HCR_PTWIDTH_SHIFT))

#define  HIPWM_HCR_TESTMODE_MASK 0x200
#define  HIPWM_HCR_TESTMODE_SHIFT 9
#define  HIPWM_HCR_TESTMODE_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_TESTMODE_MASK) >> HIPWM_HCR_TESTMODE_SHIFT)
#define  HIPWM_HCR_TESTMODE_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_TESTMODE_MASK) | (((uint32_t)val) << HIPWM_HCR_TESTMODE_SHIFT))

#define  HIPWM_HCR_CHANZERO_MASK 0x100
#define  HIPWM_HCR_CHANZERO_SHIFT 8
#define  HIPWM_HCR_CHANZERO_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_CHANZERO_MASK) >> HIPWM_HCR_CHANZERO_SHIFT)
#define  HIPWM_HCR_CHANZERO_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_CHANZERO_MASK) | (((uint32_t)val) << HIPWM_HCR_CHANZERO_SHIFT))

#define  HIPWM_HCR_INVPT_MASK 0x80
#define  HIPWM_HCR_INVPT_SHIFT 7
#define  HIPWM_HCR_INVPT_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_INVPT_MASK) >> HIPWM_HCR_INVPT_SHIFT)
#define  HIPWM_HCR_INVPT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_INVPT_MASK) | (((uint32_t)val) << HIPWM_HCR_INVPT_SHIFT))

#define  HIPWM_HCR_CPUACCESS_MASK 0x40
#define  HIPWM_HCR_CPUACCESS_SHIFT 6
#define  HIPWM_HCR_CPUACCESS_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_CPUACCESS_MASK) >> HIPWM_HCR_CPUACCESS_SHIFT)
#define  HIPWM_HCR_CPUACCESS_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_CPUACCESS_MASK) | (((uint32_t)val) << HIPWM_HCR_CPUACCESS_SHIFT))

#define  HIPWM_HCR_PECEN_MASK 0x20
#define  HIPWM_HCR_PECEN_SHIFT 5
#define  HIPWM_HCR_PECEN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_PECEN_MASK) >> HIPWM_HCR_PECEN_SHIFT)
#define  HIPWM_HCR_PECEN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_PECEN_MASK) | (((uint32_t)val) << HIPWM_HCR_PECEN_SHIFT))

#define  HIPWM_HCR_PIGTOPHSYNC_MASK 0x10
#define  HIPWM_HCR_PIGTOPHSYNC_SHIFT 4
#define  HIPWM_HCR_PIGTOPHSYNC_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_PIGTOPHSYNC_MASK) >> HIPWM_HCR_PIGTOPHSYNC_SHIFT)
#define  HIPWM_HCR_PIGTOPHSYNC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_PIGTOPHSYNC_MASK) | (((uint32_t)val) << HIPWM_HCR_PIGTOPHSYNC_SHIFT))

#define  HIPWM_HCR_PIGEN_MASK 0x8
#define  HIPWM_HCR_PIGEN_SHIFT 3
#define  HIPWM_HCR_PIGEN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_PIGEN_MASK) >> HIPWM_HCR_PIGEN_SHIFT)
#define  HIPWM_HCR_PIGEN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_PIGEN_MASK) | (((uint32_t)val) << HIPWM_HCR_PIGEN_SHIFT))

#define  HIPWM_HCR_BENDEREN_MASK 0x4
#define  HIPWM_HCR_BENDEREN_SHIFT 2
#define  HIPWM_HCR_BENDEREN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_BENDEREN_MASK) >> HIPWM_HCR_BENDEREN_SHIFT)
#define  HIPWM_HCR_BENDEREN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_BENDEREN_MASK) | (((uint32_t)val) << HIPWM_HCR_BENDEREN_SHIFT))

#define  HIPWM_HCR_SYNTHHSYNCEN_MASK 0x2
#define  HIPWM_HCR_SYNTHHSYNCEN_SHIFT 1
#define  HIPWM_HCR_SYNTHHSYNCEN_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_SYNTHHSYNCEN_MASK) >> HIPWM_HCR_SYNTHHSYNCEN_SHIFT)
#define  HIPWM_HCR_SYNTHHSYNCEN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_SYNTHHSYNCEN_MASK) | (((uint32_t)val) << HIPWM_HCR_SYNTHHSYNCEN_SHIFT))

#define  HIPWM_HCR_ENABLE_MASK 0x1
#define  HIPWM_HCR_ENABLE_SHIFT 0
#define  HIPWM_HCR_ENABLE_MASK_SHIFT(reg) (((reg) & HIPWM_HCR_ENABLE_MASK) >> HIPWM_HCR_ENABLE_SHIFT)
#define  HIPWM_HCR_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCR_ENABLE_MASK) | (((uint32_t)val) << HIPWM_HCR_ENABLE_SHIFT))

//====================================================================
//Register: HiPWM Side Margins Register (HSMR)
/** \brief HiPWM Side Margin*/
//====================================================================

#define  HIPWM_HSMR_OPPSIDEMARGIN_MASK 0xffff0000
#define  HIPWM_HSMR_OPPSIDEMARGIN_SHIFT 16
#define  HIPWM_HSMR_OPPSIDEMARGIN_MASK_SHIFT(reg) (((reg) & HIPWM_HSMR_OPPSIDEMARGIN_MASK) >> HIPWM_HSMR_OPPSIDEMARGIN_SHIFT)
#define  HIPWM_HSMR_OPPSIDEMARGIN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSMR_OPPSIDEMARGIN_MASK) | (((uint32_t)val) << HIPWM_HSMR_OPPSIDEMARGIN_SHIFT))

#define  HIPWM_HSMR_SIDEMARGIN_MASK 0xffff
#define  HIPWM_HSMR_SIDEMARGIN_SHIFT 0
#define  HIPWM_HSMR_SIDEMARGIN_MASK_SHIFT(reg) (((reg) & HIPWM_HSMR_SIDEMARGIN_MASK) >> HIPWM_HSMR_SIDEMARGIN_SHIFT)
#define  HIPWM_HSMR_SIDEMARGIN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSMR_SIDEMARGIN_MASK) | (((uint32_t)val) << HIPWM_HSMR_SIDEMARGIN_SHIFT))

//====================================================================
//Register: HiPWM Top Skew Register (HTSR)
/** \brief HiPWM Top Skew Register is used to allow small adjustments (up to 63 lines) to the Top Margin for
        this video channel. The value in this register may be modified while the HiPWM is enabled.*/
//====================================================================

#define  HIPWM_HTSR_RESERVED1_MASK 0xffffff00
#define  HIPWM_HTSR_RESERVED1_SHIFT 8
#define  HIPWM_HTSR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HTSR_RESERVED1_MASK) >> HIPWM_HTSR_RESERVED1_SHIFT)
#define  HIPWM_HTSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HTSR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HTSR_RESERVED1_SHIFT))

#define  HIPWM_HTSR_TOPSKEW_MASK 0xff
#define  HIPWM_HTSR_TOPSKEW_SHIFT 0
#define  HIPWM_HTSR_TOPSKEW_MASK_SHIFT(reg) (((reg) & HIPWM_HTSR_TOPSKEW_MASK) >> HIPWM_HTSR_TOPSKEW_SHIFT)
#define  HIPWM_HTSR_TOPSKEW_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HTSR_TOPSKEW_MASK) | (((uint32_t)val) << HIPWM_HTSR_TOPSKEW_SHIFT))

//====================================================================
//Register: HiPWM Status Register (HSR)
//====================================================================

#define  HIPWM_HSR_RESERVED1_MASK 0xfffff000
#define  HIPWM_HSR_RESERVED1_SHIFT 12
#define  HIPWM_HSR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_RESERVED1_MASK) >> HIPWM_HSR_RESERVED1_SHIFT)
#define  HIPWM_HSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HSR_RESERVED1_SHIFT))

#define  HIPWM_HSR_INDEX_MASK 0xf00
#define  HIPWM_HSR_INDEX_SHIFT 8
#define  HIPWM_HSR_INDEX_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_INDEX_MASK) >> HIPWM_HSR_INDEX_SHIFT)
#define  HIPWM_HSR_INDEX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_INDEX_MASK) | (((uint32_t)val) << HIPWM_HSR_INDEX_SHIFT))

#define  HIPWM_HSR_DIRRTOL_MASK 0x80
#define  HIPWM_HSR_DIRRTOL_SHIFT 7
#define  HIPWM_HSR_DIRRTOL_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_DIRRTOL_MASK) >> HIPWM_HSR_DIRRTOL_SHIFT)
#define  HIPWM_HSR_DIRRTOL_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_DIRRTOL_MASK) | (((uint32_t)val) << HIPWM_HSR_DIRRTOL_SHIFT))

#define  HIPWM_HSR_PAGE_STATE_MASK 0x70
#define  HIPWM_HSR_PAGE_STATE_SHIFT 4
#define  HIPWM_HSR_PAGE_STATE_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_PAGE_STATE_MASK) >> HIPWM_HSR_PAGE_STATE_SHIFT)
#define  HIPWM_HSR_PAGE_STATE_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_PAGE_STATE_MASK) | (((uint32_t)val) << HIPWM_HSR_PAGE_STATE_SHIFT))

#define  HIPWM_HSR_RESERVED2_MASK 0x8
#define  HIPWM_HSR_RESERVED2_SHIFT 3
#define  HIPWM_HSR_RESERVED2_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_RESERVED2_MASK) >> HIPWM_HSR_RESERVED2_SHIFT)
#define  HIPWM_HSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_RESERVED2_MASK) | (((uint32_t)val) << HIPWM_HSR_RESERVED2_SHIFT))

#define  HIPWM_HSR_READY_IN_MASK 0x4
#define  HIPWM_HSR_READY_IN_SHIFT 2
#define  HIPWM_HSR_READY_IN_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_READY_IN_MASK) >> HIPWM_HSR_READY_IN_SHIFT)
#define  HIPWM_HSR_READY_IN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_READY_IN_MASK) | (((uint32_t)val) << HIPWM_HSR_READY_IN_SHIFT))

#define  HIPWM_HSR_VALID_IN_MASK 0x2
#define  HIPWM_HSR_VALID_IN_SHIFT 1
#define  HIPWM_HSR_VALID_IN_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_VALID_IN_MASK) >> HIPWM_HSR_VALID_IN_SHIFT)
#define  HIPWM_HSR_VALID_IN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_VALID_IN_MASK) | (((uint32_t)val) << HIPWM_HSR_VALID_IN_SHIFT))

#define  HIPWM_HSR_BAT_TABLE_ERROR_MASK 0x1
#define  HIPWM_HSR_BAT_TABLE_ERROR_SHIFT 0
#define  HIPWM_HSR_BAT_TABLE_ERROR_MASK_SHIFT(reg) (((reg) & HIPWM_HSR_BAT_TABLE_ERROR_MASK) >> HIPWM_HSR_BAT_TABLE_ERROR_SHIFT)
#define  HIPWM_HSR_BAT_TABLE_ERROR_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSR_BAT_TABLE_ERROR_MASK) | (((uint32_t)val) << HIPWM_HSR_BAT_TABLE_ERROR_SHIFT))

//====================================================================
//Register: HiPWM Synth HSYNC Pulse Width Register (HSPWR)
/** \brief Sets the pulse width of the HiPWM Synthetic HSYNC*/
//====================================================================

#define  HIPWM_HSPWR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HSPWR_RESERVED1_SHIFT 16
#define  HIPWM_HSPWR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HSPWR_RESERVED1_MASK) >> HIPWM_HSPWR_RESERVED1_SHIFT)
#define  HIPWM_HSPWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSPWR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HSPWR_RESERVED1_SHIFT))

#define  HIPWM_HSPWR_SYNTHPULSEWID_MASK 0xffff
#define  HIPWM_HSPWR_SYNTHPULSEWID_SHIFT 0
#define  HIPWM_HSPWR_SYNTHPULSEWID_MASK_SHIFT(reg) (((reg) & HIPWM_HSPWR_SYNTHPULSEWID_MASK) >> HIPWM_HSPWR_SYNTHPULSEWID_SHIFT)
#define  HIPWM_HSPWR_SYNTHPULSEWID_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSPWR_SYNTHPULSEWID_MASK) | (((uint32_t)val) << HIPWM_HSPWR_SYNTHPULSEWID_SHIFT))

//====================================================================
//Register: HiPWM Synth HSYNC Quiescent Register (HSQR)
/** \brief Sets the Quiescent Time width of the HiPWM Synthetic HSYNC*/
//====================================================================

#define  HIPWM_HSQR_RESERVED1_MASK 0xff000000
#define  HIPWM_HSQR_RESERVED1_SHIFT 24
#define  HIPWM_HSQR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HSQR_RESERVED1_MASK) >> HIPWM_HSQR_RESERVED1_SHIFT)
#define  HIPWM_HSQR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSQR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HSQR_RESERVED1_SHIFT))

#define  HIPWM_HSQR_QUIWIDTH_MASK 0xffffff
#define  HIPWM_HSQR_QUIWIDTH_SHIFT 0
#define  HIPWM_HSQR_QUIWIDTH_MASK_SHIFT(reg) (((reg) & HIPWM_HSQR_QUIWIDTH_MASK) >> HIPWM_HSQR_QUIWIDTH_SHIFT)
#define  HIPWM_HSQR_QUIWIDTH_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HSQR_QUIWIDTH_MASK) | (((uint32_t)val) << HIPWM_HSQR_QUIWIDTH_SHIFT))

//====================================================================
//Register: HiPWM PEC Results Register (PRRA)
/** \brief Pixel Edge Count valid at end of image interrupt*/
//====================================================================

#define  HIPWM_PRRA_PEC_VALUE_MASK 0xffffffff
#define  HIPWM_PRRA_PEC_VALUE_SHIFT 0
#define  HIPWM_PRRA_PEC_VALUE_MASK_SHIFT(reg) (((reg) & HIPWM_PRRA_PEC_VALUE_MASK) >> HIPWM_PRRA_PEC_VALUE_SHIFT)
#define  HIPWM_PRRA_PEC_VALUE_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_PRRA_PEC_VALUE_MASK) | (((uint32_t)val) << HIPWM_PRRA_PEC_VALUE_SHIFT))

//====================================================================
//Register: HiPWM PIG Edge Pixel Value Register (HPPVR)
/** \brief Pixel Insertion Generator (PIG) leading edge pixel pulse train value*/
//====================================================================

#define  HIPWM_HPPVR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPPVR_RESERVED1_SHIFT 16
#define  HIPWM_HPPVR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPPVR_RESERVED1_MASK) >> HIPWM_HPPVR_RESERVED1_SHIFT)
#define  HIPWM_HPPVR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPVR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPPVR_RESERVED1_SHIFT))

#define  HIPWM_HPPVR_PIGPTVAL_MASK 0xffff
#define  HIPWM_HPPVR_PIGPTVAL_SHIFT 0
#define  HIPWM_HPPVR_PIGPTVAL_MASK_SHIFT(reg) (((reg) & HIPWM_HPPVR_PIGPTVAL_MASK) >> HIPWM_HPPVR_PIGPTVAL_SHIFT)
#define  HIPWM_HPPVR_PIGPTVAL_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPVR_PIGPTVAL_MASK) | (((uint32_t)val) << HIPWM_HPPVR_PIGPTVAL_SHIFT))

//====================================================================
//Register: HiPWM PIG Size Register (HPSR)
/** \brief Pixel Insertion Generation (PIG) Pixel Size*/
//====================================================================

#define  HIPWM_HPSR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPSR_RESERVED1_SHIFT 16
#define  HIPWM_HPSR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPSR_RESERVED1_MASK) >> HIPWM_HPSR_RESERVED1_SHIFT)
#define  HIPWM_HPSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPSR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPSR_RESERVED1_SHIFT))

#define  HIPWM_HPSR_PIGPIXSZ_MASK 0xffff
#define  HIPWM_HPSR_PIGPIXSZ_SHIFT 0
#define  HIPWM_HPSR_PIGPIXSZ_MASK_SHIFT(reg) (((reg) & HIPWM_HPSR_PIGPIXSZ_MASK) >> HIPWM_HPSR_PIGPIXSZ_SHIFT)
#define  HIPWM_HPSR_PIGPIXSZ_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPSR_PIGPIXSZ_MASK) | (((uint32_t)val) << HIPWM_HPSR_PIGPIXSZ_SHIFT))

//====================================================================
//Register: HiPWM PIG Pixel Count Register (HPCR)
/** \brief Pixel Insertion Generation (PIG) Pixel Count per line*/
//====================================================================

#define  HIPWM_HPCR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPCR_RESERVED1_SHIFT 16
#define  HIPWM_HPCR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPCR_RESERVED1_MASK) >> HIPWM_HPCR_RESERVED1_SHIFT)
#define  HIPWM_HPCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPCR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPCR_RESERVED1_SHIFT))

#define  HIPWM_HPCR_PIGPIXCNT_MASK 0xffff
#define  HIPWM_HPCR_PIGPIXCNT_SHIFT 0
#define  HIPWM_HPCR_PIGPIXCNT_MASK_SHIFT(reg) (((reg) & HIPWM_HPCR_PIGPIXCNT_MASK) >> HIPWM_HPCR_PIGPIXCNT_SHIFT)
#define  HIPWM_HPCR_PIGPIXCNT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPCR_PIGPIXCNT_MASK) | (((uint32_t)val) << HIPWM_HPCR_PIGPIXCNT_SHIFT))

//====================================================================
//Register: HiPWM PIG Pixel Spacing Register (HPPSR)
//====================================================================

#define  HIPWM_HPPSR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPPSR_RESERVED1_SHIFT 16
#define  HIPWM_HPPSR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPPSR_RESERVED1_MASK) >> HIPWM_HPPSR_RESERVED1_SHIFT)
#define  HIPWM_HPPSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPSR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPPSR_RESERVED1_SHIFT))

#define  HIPWM_HPPSR_PIGPIXSPACE_MASK 0xffff
#define  HIPWM_HPPSR_PIGPIXSPACE_SHIFT 0
#define  HIPWM_HPPSR_PIGPIXSPACE_MASK_SHIFT(reg) (((reg) & HIPWM_HPPSR_PIGPIXSPACE_MASK) >> HIPWM_HPPSR_PIGPIXSPACE_SHIFT)
#define  HIPWM_HPPSR_PIGPIXSPACE_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPSR_PIGPIXSPACE_MASK) | (((uint32_t)val) << HIPWM_HPPSR_PIGPIXSPACE_SHIFT))

//====================================================================
//Register: HiPWM PIG Line Spacing Register (HPLSR)
/** \brief Pixel Insertion Generation (PIG) spacing between pixels in lines*/
//====================================================================

#define  HIPWM_HPLSR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPLSR_RESERVED1_SHIFT 16
#define  HIPWM_HPLSR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPLSR_RESERVED1_MASK) >> HIPWM_HPLSR_RESERVED1_SHIFT)
#define  HIPWM_HPLSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPLSR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPLSR_RESERVED1_SHIFT))

#define  HIPWM_HPLSR_PIGLINESPACE_MASK 0xffff
#define  HIPWM_HPLSR_PIGLINESPACE_SHIFT 0
#define  HIPWM_HPLSR_PIGLINESPACE_MASK_SHIFT(reg) (((reg) & HIPWM_HPLSR_PIGLINESPACE_MASK) >> HIPWM_HPLSR_PIGLINESPACE_SHIFT)
#define  HIPWM_HPLSR_PIGLINESPACE_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPLSR_PIGLINESPACE_MASK) | (((uint32_t)val) << HIPWM_HPLSR_PIGLINESPACE_SHIFT))

//====================================================================
//Register: HiPWM PIG Pixel Offset Even Register (HPPOER)
/** \brief Pixel Insertion Generation (PIG) Pixel even line offset pixels in HiPWM clock cycles. This value will be used for the first PIG generated line of pixels and the odd line value will be used for the next generated line of pixels. The hardware will toggle between the even and odd line value.
        <br>
        Note that if the line spacing register is set to 1 the following sequence would occur: The first PIG generated line of pixels would use the even line offset value, the subsequent line would not generate any PIG pixels, and the next line would use the odd line offset value. This sequence would repeat for the length of the page.*/
//====================================================================

#define  HIPWM_HPPOER_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPPOER_RESERVED1_SHIFT 16
#define  HIPWM_HPPOER_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPPOER_RESERVED1_MASK) >> HIPWM_HPPOER_RESERVED1_SHIFT)
#define  HIPWM_HPPOER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPOER_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPPOER_RESERVED1_SHIFT))

#define  HIPWM_HPPOER_PIGOFFSETEVEN_MASK 0xffff
#define  HIPWM_HPPOER_PIGOFFSETEVEN_SHIFT 0
#define  HIPWM_HPPOER_PIGOFFSETEVEN_MASK_SHIFT(reg) (((reg) & HIPWM_HPPOER_PIGOFFSETEVEN_MASK) >> HIPWM_HPPOER_PIGOFFSETEVEN_SHIFT)
#define  HIPWM_HPPOER_PIGOFFSETEVEN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPOER_PIGOFFSETEVEN_MASK) | (((uint32_t)val) << HIPWM_HPPOER_PIGOFFSETEVEN_SHIFT))

//====================================================================
//Register: HiPWM PIG Pixel Offset Odd Register (HPPOOR)
/** \brief Pixel Insertion Generation (PIG) Pixel odd line offset pixels in HiPWM clock cycles. This value will be used for the second PIG generated line of pixels and the even line value will be used for the next generated line of pixels. The hardware will toggle between the even and odd line value.
        <br>
        Note that if the line spacing register is set to 1 the following sequence would occur: The first PIG generated line of pixels would use the even line offset value, the subsequent line would not generate any PIG pixels, and the next line would use the odd line offset value. This sequence would repeat for the length of the page.*/
//====================================================================

#define  HIPWM_HPPOOR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPPOOR_RESERVED1_SHIFT 16
#define  HIPWM_HPPOOR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPPOOR_RESERVED1_MASK) >> HIPWM_HPPOOR_RESERVED1_SHIFT)
#define  HIPWM_HPPOOR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPOOR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPPOOR_RESERVED1_SHIFT))

#define  HIPWM_HPPOOR_PIGOFFSETODD_MASK 0xffff
#define  HIPWM_HPPOOR_PIGOFFSETODD_SHIFT 0
#define  HIPWM_HPPOOR_PIGOFFSETODD_MASK_SHIFT(reg) (((reg) & HIPWM_HPPOOR_PIGOFFSETODD_MASK) >> HIPWM_HPPOOR_PIGOFFSETODD_SHIFT)
#define  HIPWM_HPPOOR_PIGOFFSETODD_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPOOR_PIGOFFSETODD_MASK) | (((uint32_t)val) << HIPWM_HPPOOR_PIGOFFSETODD_SHIFT))

//====================================================================
//Register: HiPWM PIG Line Offset Register (HPPLOR)
/** \brief Pixel Insertion Generation (PIG) Line offset pixels in lines*/
//====================================================================

#define  HIPWM_HPPLOR_RESERVED1_MASK 0xffff0000
#define  HIPWM_HPPLOR_RESERVED1_SHIFT 16
#define  HIPWM_HPPLOR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HPPLOR_RESERVED1_MASK) >> HIPWM_HPPLOR_RESERVED1_SHIFT)
#define  HIPWM_HPPLOR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPLOR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HPPLOR_RESERVED1_SHIFT))

#define  HIPWM_HPPLOR_PIGLINEOFFSET_MASK 0xffff
#define  HIPWM_HPPLOR_PIGLINEOFFSET_SHIFT 0
#define  HIPWM_HPPLOR_PIGLINEOFFSET_MASK_SHIFT(reg) (((reg) & HIPWM_HPPLOR_PIGLINEOFFSET_MASK) >> HIPWM_HPPLOR_PIGLINEOFFSET_SHIFT)
#define  HIPWM_HPPLOR_PIGLINEOFFSET_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HPPLOR_PIGLINEOFFSET_MASK) | (((uint32_t)val) << HIPWM_HPPLOR_PIGLINEOFFSET_SHIFT))

//====================================================================
//Register: HiPWM CRC Register (HCRC)
/** \brief Contents of the 32-bit CRC generator. The CRC generator uses the Ethernet 32-bit CRC algorithm.
        The CRC generator is reset at power up or by toggling the CrcRst bit in the HiPWM Configuration Register.
        This register should only be read when not actively printing in order to obtain a stable value.*/
//====================================================================

#define  HIPWM_HCRC_CRC_MASK 0xffffffff
#define  HIPWM_HCRC_CRC_SHIFT 0
#define  HIPWM_HCRC_CRC_MASK_SHIFT(reg) (((reg) & HIPWM_HCRC_CRC_MASK) >> HIPWM_HCRC_CRC_SHIFT)
#define  HIPWM_HCRC_CRC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HCRC_CRC_MASK) | (((uint32_t)val) << HIPWM_HCRC_CRC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Configuration Register (HBCR)
/** \brief The register sets the length, in HiPWM clock cycles, each HBAT entry is used. This length applies to all
        segments.*/
//====================================================================

#define  HIPWM_HBCR_RESERVED1_MASK 0xfffff000
#define  HIPWM_HBCR_RESERVED1_SHIFT 12
#define  HIPWM_HBCR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBCR_RESERVED1_MASK) >> HIPWM_HBCR_RESERVED1_SHIFT)
#define  HIPWM_HBCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBCR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBCR_RESERVED1_SHIFT))

#define  HIPWM_HBCR_SAMPLESPERSEG_MASK 0xfff
#define  HIPWM_HBCR_SAMPLESPERSEG_SHIFT 0
#define  HIPWM_HBCR_SAMPLESPERSEG_MASK_SHIFT(reg) (((reg) & HIPWM_HBCR_SAMPLESPERSEG_MASK) >> HIPWM_HBCR_SAMPLESPERSEG_SHIFT)
#define  HIPWM_HBCR_SAMPLESPERSEG_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBCR_SAMPLESPERSEG_MASK) | (((uint32_t)val) << HIPWM_HBCR_SAMPLESPERSEG_SHIFT))

//====================================================================
//Register: HiPWM Left Side BENDER Default Register (HLBDR)
/** \brief BENDER controls the pulse train increment value sent to HiPS Pulse Train Generator.
        The BENDER default value represents a positive 25-bit fractional number consisting of 19
        fractional and 6 integer bits.
        <br>
        The BENDER Default value is calculated as follows:
        <br>
        BENDER_default_value = (HiPS_Width x Pixel_freq x Num_taps) / HiPS_freq
        <br>
        where the HiPS_Width = 16, HiPS_freq = 2.5GHz, Pixel_freq is the desired video frequency, and Num_taps
        is either 32 or 64. Num_taps is controlled by the PTwidth bit in the HiPWM Configuration register.
        <br>
        Example: Pixel Frequency = 25.02MHz, Num_taps = 64
        <br>
        real BENDER_default_value = (16 x 25.020e6 x 64)/2.5e9 = 10.248192
        <br>
        hex BENDER_default_value = dec2hex(10.248192 x 2^19) = 0x51FC4C
        <br>
        The Left Side BENDER Default Register value is used when printing from left to right.
        <br>*/
//====================================================================

#define  HIPWM_HLBDR_RESERVED1_MASK 0xfe000000
#define  HIPWM_HLBDR_RESERVED1_SHIFT 25
#define  HIPWM_HLBDR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HLBDR_RESERVED1_MASK) >> HIPWM_HLBDR_RESERVED1_SHIFT)
#define  HIPWM_HLBDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HLBDR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HLBDR_RESERVED1_SHIFT))

#define  HIPWM_HLBDR_LBDRINT_MASK 0x1f80000
#define  HIPWM_HLBDR_LBDRINT_SHIFT 19
#define  HIPWM_HLBDR_LBDRINT_MASK_SHIFT(reg) (((reg) & HIPWM_HLBDR_LBDRINT_MASK) >> HIPWM_HLBDR_LBDRINT_SHIFT)
#define  HIPWM_HLBDR_LBDRINT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HLBDR_LBDRINT_MASK) | (((uint32_t)val) << HIPWM_HLBDR_LBDRINT_SHIFT))

#define  HIPWM_HLBDR_LBDRFRAC_MASK 0x7ffff
#define  HIPWM_HLBDR_LBDRFRAC_SHIFT 0
#define  HIPWM_HLBDR_LBDRFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HLBDR_LBDRFRAC_MASK) >> HIPWM_HLBDR_LBDRFRAC_SHIFT)
#define  HIPWM_HLBDR_LBDRFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HLBDR_LBDRFRAC_MASK) | (((uint32_t)val) << HIPWM_HLBDR_LBDRFRAC_SHIFT))

//====================================================================
//Register: HiPWM Right Side BENDER Default Register (HRBDR)
/** \brief BENDER controls the pulse train increment value sent to HiPS Pulse Train Generator.
        The BENDER default value represents a positive 25-bit fractional number consisting of 19
        fractional and 6 integer bits. It's value is calculated
        in the same manner as the HiPWM Left Side BENDER Default.
        <br>
        The Right Side BENDER Default Register value is used when printing from right to left.
        It is only used if the DualMargin bit is set in the HiPWM Configuration
        Register.*/
//====================================================================

#define  HIPWM_HRBDR_RESERVED1_MASK 0xfe000000
#define  HIPWM_HRBDR_RESERVED1_SHIFT 25
#define  HIPWM_HRBDR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HRBDR_RESERVED1_MASK) >> HIPWM_HRBDR_RESERVED1_SHIFT)
#define  HIPWM_HRBDR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HRBDR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HRBDR_RESERVED1_SHIFT))

#define  HIPWM_HRBDR_RBDRINT_MASK 0x1f80000
#define  HIPWM_HRBDR_RBDRINT_SHIFT 19
#define  HIPWM_HRBDR_RBDRINT_MASK_SHIFT(reg) (((reg) & HIPWM_HRBDR_RBDRINT_MASK) >> HIPWM_HRBDR_RBDRINT_SHIFT)
#define  HIPWM_HRBDR_RBDRINT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HRBDR_RBDRINT_MASK) | (((uint32_t)val) << HIPWM_HRBDR_RBDRINT_SHIFT))

#define  HIPWM_HRBDR_RBDRFRAC_MASK 0x7ffff
#define  HIPWM_HRBDR_RBDRFRAC_SHIFT 0
#define  HIPWM_HRBDR_RBDRFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HRBDR_RBDRFRAC_MASK) >> HIPWM_HRBDR_RBDRFRAC_SHIFT)
#define  HIPWM_HRBDR_RBDRFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HRBDR_RBDRFRAC_MASK) | (((uint32_t)val) << HIPWM_HRBDR_RBDRFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Normalization Configuration Register (HBNCR)
/** \brief It's possible for the HiPS Pulse Train quantization to cause similar errors in the same locations. 
        If they line up with some particular printed patterns it may become visible. To break up any patterning,
        injection of some noise with the LFSR could cause the errors to no longer be aligned or periodic.*/
//====================================================================

#define  HIPWM_HBNCR_RESERVED1_MASK 0xffffff80
#define  HIPWM_HBNCR_RESERVED1_SHIFT 7
#define  HIPWM_HBNCR_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBNCR_RESERVED1_MASK) >> HIPWM_HBNCR_RESERVED1_SHIFT)
#define  HIPWM_HBNCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNCR_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBNCR_RESERVED1_SHIFT))

#define  HIPWM_HBNCR_LFSRINITCNTL_MASK 0x40
#define  HIPWM_HBNCR_LFSRINITCNTL_SHIFT 6
#define  HIPWM_HBNCR_LFSRINITCNTL_MASK_SHIFT(reg) (((reg) & HIPWM_HBNCR_LFSRINITCNTL_MASK) >> HIPWM_HBNCR_LFSRINITCNTL_SHIFT)
#define  HIPWM_HBNCR_LFSRINITCNTL_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNCR_LFSRINITCNTL_MASK) | (((uint32_t)val) << HIPWM_HBNCR_LFSRINITCNTL_SHIFT))

#define  HIPWM_HBNCR_BENDERLFSREN_MASK 0x20
#define  HIPWM_HBNCR_BENDERLFSREN_SHIFT 5
#define  HIPWM_HBNCR_BENDERLFSREN_MASK_SHIFT(reg) (((reg) & HIPWM_HBNCR_BENDERLFSREN_MASK) >> HIPWM_HBNCR_BENDERLFSREN_SHIFT)
#define  HIPWM_HBNCR_BENDERLFSREN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNCR_BENDERLFSREN_MASK) | (((uint32_t)val) << HIPWM_HBNCR_BENDERLFSREN_SHIFT))

#define  HIPWM_HBNCR_BENDERLFSRMASK_MASK 0x1f
#define  HIPWM_HBNCR_BENDERLFSRMASK_SHIFT 0
#define  HIPWM_HBNCR_BENDERLFSRMASK_MASK_SHIFT(reg) (((reg) & HIPWM_HBNCR_BENDERLFSRMASK_MASK) >> HIPWM_HBNCR_BENDERLFSRMASK_SHIFT)
#define  HIPWM_HBNCR_BENDERLFSRMASK_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNCR_BENDERLFSRMASK_MASK) | (((uint32_t)val) << HIPWM_HBNCR_BENDERLFSRMASK_SHIFT))

//====================================================================
//Register: HiPWM BENDER LFSR Initialization Register (HBNINIT)
/** \brief Value used to initialize the BENDER LFSR at the first pixel of an image. This
        register should only be written when the HiPWM is disabled.*/
//====================================================================

#define  HIPWM_HBNINIT_RESERVED1_MASK 0xffff0000
#define  HIPWM_HBNINIT_RESERVED1_SHIFT 16
#define  HIPWM_HBNINIT_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBNINIT_RESERVED1_MASK) >> HIPWM_HBNINIT_RESERVED1_SHIFT)
#define  HIPWM_HBNINIT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNINIT_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBNINIT_RESERVED1_SHIFT))

#define  HIPWM_HBNINIT_BENDERLFSRINIT_MASK 0xffff
#define  HIPWM_HBNINIT_BENDERLFSRINIT_SHIFT 0
#define  HIPWM_HBNINIT_BENDERLFSRINIT_MASK_SHIFT(reg) (((reg) & HIPWM_HBNINIT_BENDERLFSRINIT_MASK) >> HIPWM_HBNINIT_BENDERLFSRINIT_SHIFT)
#define  HIPWM_HBNINIT_BENDERLFSRINIT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNINIT_BENDERLFSRINIT_MASK) | (((uint32_t)val) << HIPWM_HBNINIT_BENDERLFSRINIT_SHIFT))

//====================================================================
//Register: HiPWM BENDER LFSR Current Value Register (HBNCV)
//====================================================================

#define  HIPWM_HBNCV_RESERVED1_MASK 0xffff0000
#define  HIPWM_HBNCV_RESERVED1_SHIFT 16
#define  HIPWM_HBNCV_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBNCV_RESERVED1_MASK) >> HIPWM_HBNCV_RESERVED1_SHIFT)
#define  HIPWM_HBNCV_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNCV_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBNCV_RESERVED1_SHIFT))

#define  HIPWM_HBNCV_BENDERLFSRVAL_MASK 0xffff
#define  HIPWM_HBNCV_BENDERLFSRVAL_SHIFT 0
#define  HIPWM_HBNCV_BENDERLFSRVAL_MASK_SHIFT(reg) (((reg) & HIPWM_HBNCV_BENDERLFSRVAL_MASK) >> HIPWM_HBNCV_BENDERLFSRVAL_SHIFT)
#define  HIPWM_HBNCV_BENDERLFSRVAL_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBNCV_BENDERLFSRVAL_MASK) | (((uint32_t)val) << HIPWM_HBNCV_BENDERLFSRVAL_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 1 of 64) (HBAT0)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 1 of 64)*/
//====================================================================

#define  HIPWM_HBAT0_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT0_RESERVED1_SHIFT 20
#define  HIPWM_HBAT0_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT0_RESERVED1_MASK) >> HIPWM_HBAT0_RESERVED1_SHIFT)
#define  HIPWM_HBAT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT0_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT0_RESERVED1_SHIFT))

#define  HIPWM_HBAT0_BATLAST_MASK 0x80000
#define  HIPWM_HBAT0_BATLAST_SHIFT 19
#define  HIPWM_HBAT0_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT0_BATLAST_MASK) >> HIPWM_HBAT0_BATLAST_SHIFT)
#define  HIPWM_HBAT0_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT0_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT0_BATLAST_SHIFT))

#define  HIPWM_HBAT0_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT0_BATSIGN_SHIFT 18
#define  HIPWM_HBAT0_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT0_BATSIGN_MASK) >> HIPWM_HBAT0_BATSIGN_SHIFT)
#define  HIPWM_HBAT0_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT0_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT0_BATSIGN_SHIFT))

#define  HIPWM_HBAT0_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT0_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT0_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT0_BATSHIFT_MASK) >> HIPWM_HBAT0_BATSHIFT_SHIFT)
#define  HIPWM_HBAT0_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT0_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT0_BATSHIFT_SHIFT))

#define  HIPWM_HBAT0_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT0_BATFRAC_SHIFT 0
#define  HIPWM_HBAT0_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT0_BATFRAC_MASK) >> HIPWM_HBAT0_BATFRAC_SHIFT)
#define  HIPWM_HBAT0_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT0_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT0_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 2 of 64) (HBAT1)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 2 of 64)*/
//====================================================================

#define  HIPWM_HBAT1_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT1_RESERVED1_SHIFT 20
#define  HIPWM_HBAT1_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT1_RESERVED1_MASK) >> HIPWM_HBAT1_RESERVED1_SHIFT)
#define  HIPWM_HBAT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT1_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT1_RESERVED1_SHIFT))

#define  HIPWM_HBAT1_BATLAST_MASK 0x80000
#define  HIPWM_HBAT1_BATLAST_SHIFT 19
#define  HIPWM_HBAT1_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT1_BATLAST_MASK) >> HIPWM_HBAT1_BATLAST_SHIFT)
#define  HIPWM_HBAT1_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT1_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT1_BATLAST_SHIFT))

#define  HIPWM_HBAT1_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT1_BATSIGN_SHIFT 18
#define  HIPWM_HBAT1_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT1_BATSIGN_MASK) >> HIPWM_HBAT1_BATSIGN_SHIFT)
#define  HIPWM_HBAT1_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT1_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT1_BATSIGN_SHIFT))

#define  HIPWM_HBAT1_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT1_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT1_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT1_BATSHIFT_MASK) >> HIPWM_HBAT1_BATSHIFT_SHIFT)
#define  HIPWM_HBAT1_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT1_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT1_BATSHIFT_SHIFT))

#define  HIPWM_HBAT1_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT1_BATFRAC_SHIFT 0
#define  HIPWM_HBAT1_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT1_BATFRAC_MASK) >> HIPWM_HBAT1_BATFRAC_SHIFT)
#define  HIPWM_HBAT1_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT1_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT1_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 3 of 64) (HBAT2)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 3 of 64)*/
//====================================================================

#define  HIPWM_HBAT2_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT2_RESERVED1_SHIFT 20
#define  HIPWM_HBAT2_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT2_RESERVED1_MASK) >> HIPWM_HBAT2_RESERVED1_SHIFT)
#define  HIPWM_HBAT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT2_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT2_RESERVED1_SHIFT))

#define  HIPWM_HBAT2_BATLAST_MASK 0x80000
#define  HIPWM_HBAT2_BATLAST_SHIFT 19
#define  HIPWM_HBAT2_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT2_BATLAST_MASK) >> HIPWM_HBAT2_BATLAST_SHIFT)
#define  HIPWM_HBAT2_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT2_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT2_BATLAST_SHIFT))

#define  HIPWM_HBAT2_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT2_BATSIGN_SHIFT 18
#define  HIPWM_HBAT2_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT2_BATSIGN_MASK) >> HIPWM_HBAT2_BATSIGN_SHIFT)
#define  HIPWM_HBAT2_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT2_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT2_BATSIGN_SHIFT))

#define  HIPWM_HBAT2_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT2_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT2_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT2_BATSHIFT_MASK) >> HIPWM_HBAT2_BATSHIFT_SHIFT)
#define  HIPWM_HBAT2_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT2_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT2_BATSHIFT_SHIFT))

#define  HIPWM_HBAT2_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT2_BATFRAC_SHIFT 0
#define  HIPWM_HBAT2_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT2_BATFRAC_MASK) >> HIPWM_HBAT2_BATFRAC_SHIFT)
#define  HIPWM_HBAT2_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT2_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT2_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 4 of 64) (HBAT3)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 4 of 64)*/
//====================================================================

#define  HIPWM_HBAT3_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT3_RESERVED1_SHIFT 20
#define  HIPWM_HBAT3_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT3_RESERVED1_MASK) >> HIPWM_HBAT3_RESERVED1_SHIFT)
#define  HIPWM_HBAT3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT3_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT3_RESERVED1_SHIFT))

#define  HIPWM_HBAT3_BATLAST_MASK 0x80000
#define  HIPWM_HBAT3_BATLAST_SHIFT 19
#define  HIPWM_HBAT3_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT3_BATLAST_MASK) >> HIPWM_HBAT3_BATLAST_SHIFT)
#define  HIPWM_HBAT3_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT3_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT3_BATLAST_SHIFT))

#define  HIPWM_HBAT3_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT3_BATSIGN_SHIFT 18
#define  HIPWM_HBAT3_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT3_BATSIGN_MASK) >> HIPWM_HBAT3_BATSIGN_SHIFT)
#define  HIPWM_HBAT3_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT3_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT3_BATSIGN_SHIFT))

#define  HIPWM_HBAT3_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT3_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT3_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT3_BATSHIFT_MASK) >> HIPWM_HBAT3_BATSHIFT_SHIFT)
#define  HIPWM_HBAT3_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT3_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT3_BATSHIFT_SHIFT))

#define  HIPWM_HBAT3_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT3_BATFRAC_SHIFT 0
#define  HIPWM_HBAT3_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT3_BATFRAC_MASK) >> HIPWM_HBAT3_BATFRAC_SHIFT)
#define  HIPWM_HBAT3_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT3_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT3_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 5 of 64) (HBAT4)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 5 of 64)*/
//====================================================================

#define  HIPWM_HBAT4_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT4_RESERVED1_SHIFT 20
#define  HIPWM_HBAT4_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT4_RESERVED1_MASK) >> HIPWM_HBAT4_RESERVED1_SHIFT)
#define  HIPWM_HBAT4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT4_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT4_RESERVED1_SHIFT))

#define  HIPWM_HBAT4_BATLAST_MASK 0x80000
#define  HIPWM_HBAT4_BATLAST_SHIFT 19
#define  HIPWM_HBAT4_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT4_BATLAST_MASK) >> HIPWM_HBAT4_BATLAST_SHIFT)
#define  HIPWM_HBAT4_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT4_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT4_BATLAST_SHIFT))

#define  HIPWM_HBAT4_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT4_BATSIGN_SHIFT 18
#define  HIPWM_HBAT4_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT4_BATSIGN_MASK) >> HIPWM_HBAT4_BATSIGN_SHIFT)
#define  HIPWM_HBAT4_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT4_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT4_BATSIGN_SHIFT))

#define  HIPWM_HBAT4_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT4_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT4_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT4_BATSHIFT_MASK) >> HIPWM_HBAT4_BATSHIFT_SHIFT)
#define  HIPWM_HBAT4_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT4_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT4_BATSHIFT_SHIFT))

#define  HIPWM_HBAT4_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT4_BATFRAC_SHIFT 0
#define  HIPWM_HBAT4_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT4_BATFRAC_MASK) >> HIPWM_HBAT4_BATFRAC_SHIFT)
#define  HIPWM_HBAT4_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT4_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT4_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 6 of 64) (HBAT5)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 6 of 64)*/
//====================================================================

#define  HIPWM_HBAT5_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT5_RESERVED1_SHIFT 20
#define  HIPWM_HBAT5_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT5_RESERVED1_MASK) >> HIPWM_HBAT5_RESERVED1_SHIFT)
#define  HIPWM_HBAT5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT5_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT5_RESERVED1_SHIFT))

#define  HIPWM_HBAT5_BATLAST_MASK 0x80000
#define  HIPWM_HBAT5_BATLAST_SHIFT 19
#define  HIPWM_HBAT5_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT5_BATLAST_MASK) >> HIPWM_HBAT5_BATLAST_SHIFT)
#define  HIPWM_HBAT5_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT5_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT5_BATLAST_SHIFT))

#define  HIPWM_HBAT5_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT5_BATSIGN_SHIFT 18
#define  HIPWM_HBAT5_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT5_BATSIGN_MASK) >> HIPWM_HBAT5_BATSIGN_SHIFT)
#define  HIPWM_HBAT5_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT5_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT5_BATSIGN_SHIFT))

#define  HIPWM_HBAT5_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT5_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT5_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT5_BATSHIFT_MASK) >> HIPWM_HBAT5_BATSHIFT_SHIFT)
#define  HIPWM_HBAT5_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT5_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT5_BATSHIFT_SHIFT))

#define  HIPWM_HBAT5_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT5_BATFRAC_SHIFT 0
#define  HIPWM_HBAT5_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT5_BATFRAC_MASK) >> HIPWM_HBAT5_BATFRAC_SHIFT)
#define  HIPWM_HBAT5_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT5_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT5_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 7 of 64) (HBAT6)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 7 of 64)*/
//====================================================================

#define  HIPWM_HBAT6_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT6_RESERVED1_SHIFT 20
#define  HIPWM_HBAT6_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT6_RESERVED1_MASK) >> HIPWM_HBAT6_RESERVED1_SHIFT)
#define  HIPWM_HBAT6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT6_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT6_RESERVED1_SHIFT))

#define  HIPWM_HBAT6_BATLAST_MASK 0x80000
#define  HIPWM_HBAT6_BATLAST_SHIFT 19
#define  HIPWM_HBAT6_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT6_BATLAST_MASK) >> HIPWM_HBAT6_BATLAST_SHIFT)
#define  HIPWM_HBAT6_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT6_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT6_BATLAST_SHIFT))

#define  HIPWM_HBAT6_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT6_BATSIGN_SHIFT 18
#define  HIPWM_HBAT6_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT6_BATSIGN_MASK) >> HIPWM_HBAT6_BATSIGN_SHIFT)
#define  HIPWM_HBAT6_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT6_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT6_BATSIGN_SHIFT))

#define  HIPWM_HBAT6_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT6_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT6_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT6_BATSHIFT_MASK) >> HIPWM_HBAT6_BATSHIFT_SHIFT)
#define  HIPWM_HBAT6_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT6_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT6_BATSHIFT_SHIFT))

#define  HIPWM_HBAT6_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT6_BATFRAC_SHIFT 0
#define  HIPWM_HBAT6_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT6_BATFRAC_MASK) >> HIPWM_HBAT6_BATFRAC_SHIFT)
#define  HIPWM_HBAT6_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT6_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT6_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 8 of 64) (HBAT7)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 8 of 64)*/
//====================================================================

#define  HIPWM_HBAT7_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT7_RESERVED1_SHIFT 20
#define  HIPWM_HBAT7_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT7_RESERVED1_MASK) >> HIPWM_HBAT7_RESERVED1_SHIFT)
#define  HIPWM_HBAT7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT7_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT7_RESERVED1_SHIFT))

#define  HIPWM_HBAT7_BATLAST_MASK 0x80000
#define  HIPWM_HBAT7_BATLAST_SHIFT 19
#define  HIPWM_HBAT7_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT7_BATLAST_MASK) >> HIPWM_HBAT7_BATLAST_SHIFT)
#define  HIPWM_HBAT7_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT7_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT7_BATLAST_SHIFT))

#define  HIPWM_HBAT7_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT7_BATSIGN_SHIFT 18
#define  HIPWM_HBAT7_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT7_BATSIGN_MASK) >> HIPWM_HBAT7_BATSIGN_SHIFT)
#define  HIPWM_HBAT7_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT7_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT7_BATSIGN_SHIFT))

#define  HIPWM_HBAT7_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT7_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT7_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT7_BATSHIFT_MASK) >> HIPWM_HBAT7_BATSHIFT_SHIFT)
#define  HIPWM_HBAT7_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT7_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT7_BATSHIFT_SHIFT))

#define  HIPWM_HBAT7_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT7_BATFRAC_SHIFT 0
#define  HIPWM_HBAT7_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT7_BATFRAC_MASK) >> HIPWM_HBAT7_BATFRAC_SHIFT)
#define  HIPWM_HBAT7_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT7_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT7_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 9 of 64) (HBAT8)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 9 of 64)*/
//====================================================================

#define  HIPWM_HBAT8_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT8_RESERVED1_SHIFT 20
#define  HIPWM_HBAT8_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT8_RESERVED1_MASK) >> HIPWM_HBAT8_RESERVED1_SHIFT)
#define  HIPWM_HBAT8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT8_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT8_RESERVED1_SHIFT))

#define  HIPWM_HBAT8_BATLAST_MASK 0x80000
#define  HIPWM_HBAT8_BATLAST_SHIFT 19
#define  HIPWM_HBAT8_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT8_BATLAST_MASK) >> HIPWM_HBAT8_BATLAST_SHIFT)
#define  HIPWM_HBAT8_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT8_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT8_BATLAST_SHIFT))

#define  HIPWM_HBAT8_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT8_BATSIGN_SHIFT 18
#define  HIPWM_HBAT8_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT8_BATSIGN_MASK) >> HIPWM_HBAT8_BATSIGN_SHIFT)
#define  HIPWM_HBAT8_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT8_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT8_BATSIGN_SHIFT))

#define  HIPWM_HBAT8_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT8_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT8_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT8_BATSHIFT_MASK) >> HIPWM_HBAT8_BATSHIFT_SHIFT)
#define  HIPWM_HBAT8_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT8_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT8_BATSHIFT_SHIFT))

#define  HIPWM_HBAT8_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT8_BATFRAC_SHIFT 0
#define  HIPWM_HBAT8_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT8_BATFRAC_MASK) >> HIPWM_HBAT8_BATFRAC_SHIFT)
#define  HIPWM_HBAT8_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT8_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT8_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 10 of 64) (HBAT9)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 10 of 64)*/
//====================================================================

#define  HIPWM_HBAT9_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT9_RESERVED1_SHIFT 20
#define  HIPWM_HBAT9_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT9_RESERVED1_MASK) >> HIPWM_HBAT9_RESERVED1_SHIFT)
#define  HIPWM_HBAT9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT9_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT9_RESERVED1_SHIFT))

#define  HIPWM_HBAT9_BATLAST_MASK 0x80000
#define  HIPWM_HBAT9_BATLAST_SHIFT 19
#define  HIPWM_HBAT9_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT9_BATLAST_MASK) >> HIPWM_HBAT9_BATLAST_SHIFT)
#define  HIPWM_HBAT9_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT9_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT9_BATLAST_SHIFT))

#define  HIPWM_HBAT9_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT9_BATSIGN_SHIFT 18
#define  HIPWM_HBAT9_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT9_BATSIGN_MASK) >> HIPWM_HBAT9_BATSIGN_SHIFT)
#define  HIPWM_HBAT9_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT9_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT9_BATSIGN_SHIFT))

#define  HIPWM_HBAT9_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT9_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT9_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT9_BATSHIFT_MASK) >> HIPWM_HBAT9_BATSHIFT_SHIFT)
#define  HIPWM_HBAT9_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT9_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT9_BATSHIFT_SHIFT))

#define  HIPWM_HBAT9_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT9_BATFRAC_SHIFT 0
#define  HIPWM_HBAT9_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT9_BATFRAC_MASK) >> HIPWM_HBAT9_BATFRAC_SHIFT)
#define  HIPWM_HBAT9_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT9_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT9_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 11 of 64) (HBAT10)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 11 of 64)*/
//====================================================================

#define  HIPWM_HBAT10_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT10_RESERVED1_SHIFT 20
#define  HIPWM_HBAT10_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT10_RESERVED1_MASK) >> HIPWM_HBAT10_RESERVED1_SHIFT)
#define  HIPWM_HBAT10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT10_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT10_RESERVED1_SHIFT))

#define  HIPWM_HBAT10_BATLAST_MASK 0x80000
#define  HIPWM_HBAT10_BATLAST_SHIFT 19
#define  HIPWM_HBAT10_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT10_BATLAST_MASK) >> HIPWM_HBAT10_BATLAST_SHIFT)
#define  HIPWM_HBAT10_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT10_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT10_BATLAST_SHIFT))

#define  HIPWM_HBAT10_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT10_BATSIGN_SHIFT 18
#define  HIPWM_HBAT10_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT10_BATSIGN_MASK) >> HIPWM_HBAT10_BATSIGN_SHIFT)
#define  HIPWM_HBAT10_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT10_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT10_BATSIGN_SHIFT))

#define  HIPWM_HBAT10_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT10_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT10_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT10_BATSHIFT_MASK) >> HIPWM_HBAT10_BATSHIFT_SHIFT)
#define  HIPWM_HBAT10_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT10_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT10_BATSHIFT_SHIFT))

#define  HIPWM_HBAT10_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT10_BATFRAC_SHIFT 0
#define  HIPWM_HBAT10_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT10_BATFRAC_MASK) >> HIPWM_HBAT10_BATFRAC_SHIFT)
#define  HIPWM_HBAT10_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT10_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT10_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 12 of 64) (HBAT11)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 12 of 64)*/
//====================================================================

#define  HIPWM_HBAT11_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT11_RESERVED1_SHIFT 20
#define  HIPWM_HBAT11_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT11_RESERVED1_MASK) >> HIPWM_HBAT11_RESERVED1_SHIFT)
#define  HIPWM_HBAT11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT11_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT11_RESERVED1_SHIFT))

#define  HIPWM_HBAT11_BATLAST_MASK 0x80000
#define  HIPWM_HBAT11_BATLAST_SHIFT 19
#define  HIPWM_HBAT11_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT11_BATLAST_MASK) >> HIPWM_HBAT11_BATLAST_SHIFT)
#define  HIPWM_HBAT11_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT11_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT11_BATLAST_SHIFT))

#define  HIPWM_HBAT11_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT11_BATSIGN_SHIFT 18
#define  HIPWM_HBAT11_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT11_BATSIGN_MASK) >> HIPWM_HBAT11_BATSIGN_SHIFT)
#define  HIPWM_HBAT11_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT11_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT11_BATSIGN_SHIFT))

#define  HIPWM_HBAT11_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT11_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT11_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT11_BATSHIFT_MASK) >> HIPWM_HBAT11_BATSHIFT_SHIFT)
#define  HIPWM_HBAT11_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT11_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT11_BATSHIFT_SHIFT))

#define  HIPWM_HBAT11_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT11_BATFRAC_SHIFT 0
#define  HIPWM_HBAT11_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT11_BATFRAC_MASK) >> HIPWM_HBAT11_BATFRAC_SHIFT)
#define  HIPWM_HBAT11_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT11_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT11_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 13 of 64) (HBAT12)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 13 of 64)*/
//====================================================================

#define  HIPWM_HBAT12_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT12_RESERVED1_SHIFT 20
#define  HIPWM_HBAT12_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT12_RESERVED1_MASK) >> HIPWM_HBAT12_RESERVED1_SHIFT)
#define  HIPWM_HBAT12_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT12_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT12_RESERVED1_SHIFT))

#define  HIPWM_HBAT12_BATLAST_MASK 0x80000
#define  HIPWM_HBAT12_BATLAST_SHIFT 19
#define  HIPWM_HBAT12_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT12_BATLAST_MASK) >> HIPWM_HBAT12_BATLAST_SHIFT)
#define  HIPWM_HBAT12_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT12_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT12_BATLAST_SHIFT))

#define  HIPWM_HBAT12_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT12_BATSIGN_SHIFT 18
#define  HIPWM_HBAT12_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT12_BATSIGN_MASK) >> HIPWM_HBAT12_BATSIGN_SHIFT)
#define  HIPWM_HBAT12_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT12_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT12_BATSIGN_SHIFT))

#define  HIPWM_HBAT12_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT12_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT12_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT12_BATSHIFT_MASK) >> HIPWM_HBAT12_BATSHIFT_SHIFT)
#define  HIPWM_HBAT12_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT12_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT12_BATSHIFT_SHIFT))

#define  HIPWM_HBAT12_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT12_BATFRAC_SHIFT 0
#define  HIPWM_HBAT12_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT12_BATFRAC_MASK) >> HIPWM_HBAT12_BATFRAC_SHIFT)
#define  HIPWM_HBAT12_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT12_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT12_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 14 of 64) (HBAT13)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 14 of 64)*/
//====================================================================

#define  HIPWM_HBAT13_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT13_RESERVED1_SHIFT 20
#define  HIPWM_HBAT13_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT13_RESERVED1_MASK) >> HIPWM_HBAT13_RESERVED1_SHIFT)
#define  HIPWM_HBAT13_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT13_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT13_RESERVED1_SHIFT))

#define  HIPWM_HBAT13_BATLAST_MASK 0x80000
#define  HIPWM_HBAT13_BATLAST_SHIFT 19
#define  HIPWM_HBAT13_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT13_BATLAST_MASK) >> HIPWM_HBAT13_BATLAST_SHIFT)
#define  HIPWM_HBAT13_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT13_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT13_BATLAST_SHIFT))

#define  HIPWM_HBAT13_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT13_BATSIGN_SHIFT 18
#define  HIPWM_HBAT13_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT13_BATSIGN_MASK) >> HIPWM_HBAT13_BATSIGN_SHIFT)
#define  HIPWM_HBAT13_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT13_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT13_BATSIGN_SHIFT))

#define  HIPWM_HBAT13_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT13_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT13_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT13_BATSHIFT_MASK) >> HIPWM_HBAT13_BATSHIFT_SHIFT)
#define  HIPWM_HBAT13_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT13_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT13_BATSHIFT_SHIFT))

#define  HIPWM_HBAT13_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT13_BATFRAC_SHIFT 0
#define  HIPWM_HBAT13_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT13_BATFRAC_MASK) >> HIPWM_HBAT13_BATFRAC_SHIFT)
#define  HIPWM_HBAT13_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT13_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT13_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 15 of 64) (HBAT14)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 15 of 64)*/
//====================================================================

#define  HIPWM_HBAT14_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT14_RESERVED1_SHIFT 20
#define  HIPWM_HBAT14_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT14_RESERVED1_MASK) >> HIPWM_HBAT14_RESERVED1_SHIFT)
#define  HIPWM_HBAT14_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT14_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT14_RESERVED1_SHIFT))

#define  HIPWM_HBAT14_BATLAST_MASK 0x80000
#define  HIPWM_HBAT14_BATLAST_SHIFT 19
#define  HIPWM_HBAT14_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT14_BATLAST_MASK) >> HIPWM_HBAT14_BATLAST_SHIFT)
#define  HIPWM_HBAT14_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT14_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT14_BATLAST_SHIFT))

#define  HIPWM_HBAT14_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT14_BATSIGN_SHIFT 18
#define  HIPWM_HBAT14_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT14_BATSIGN_MASK) >> HIPWM_HBAT14_BATSIGN_SHIFT)
#define  HIPWM_HBAT14_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT14_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT14_BATSIGN_SHIFT))

#define  HIPWM_HBAT14_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT14_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT14_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT14_BATSHIFT_MASK) >> HIPWM_HBAT14_BATSHIFT_SHIFT)
#define  HIPWM_HBAT14_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT14_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT14_BATSHIFT_SHIFT))

#define  HIPWM_HBAT14_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT14_BATFRAC_SHIFT 0
#define  HIPWM_HBAT14_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT14_BATFRAC_MASK) >> HIPWM_HBAT14_BATFRAC_SHIFT)
#define  HIPWM_HBAT14_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT14_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT14_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 16 of 64) (HBAT15)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 16 of 64)*/
//====================================================================

#define  HIPWM_HBAT15_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT15_RESERVED1_SHIFT 20
#define  HIPWM_HBAT15_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT15_RESERVED1_MASK) >> HIPWM_HBAT15_RESERVED1_SHIFT)
#define  HIPWM_HBAT15_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT15_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT15_RESERVED1_SHIFT))

#define  HIPWM_HBAT15_BATLAST_MASK 0x80000
#define  HIPWM_HBAT15_BATLAST_SHIFT 19
#define  HIPWM_HBAT15_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT15_BATLAST_MASK) >> HIPWM_HBAT15_BATLAST_SHIFT)
#define  HIPWM_HBAT15_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT15_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT15_BATLAST_SHIFT))

#define  HIPWM_HBAT15_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT15_BATSIGN_SHIFT 18
#define  HIPWM_HBAT15_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT15_BATSIGN_MASK) >> HIPWM_HBAT15_BATSIGN_SHIFT)
#define  HIPWM_HBAT15_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT15_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT15_BATSIGN_SHIFT))

#define  HIPWM_HBAT15_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT15_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT15_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT15_BATSHIFT_MASK) >> HIPWM_HBAT15_BATSHIFT_SHIFT)
#define  HIPWM_HBAT15_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT15_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT15_BATSHIFT_SHIFT))

#define  HIPWM_HBAT15_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT15_BATFRAC_SHIFT 0
#define  HIPWM_HBAT15_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT15_BATFRAC_MASK) >> HIPWM_HBAT15_BATFRAC_SHIFT)
#define  HIPWM_HBAT15_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT15_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT15_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 17 of 64) (HBAT16)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 17 of 64)*/
//====================================================================

#define  HIPWM_HBAT16_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT16_RESERVED1_SHIFT 20
#define  HIPWM_HBAT16_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT16_RESERVED1_MASK) >> HIPWM_HBAT16_RESERVED1_SHIFT)
#define  HIPWM_HBAT16_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT16_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT16_RESERVED1_SHIFT))

#define  HIPWM_HBAT16_BATLAST_MASK 0x80000
#define  HIPWM_HBAT16_BATLAST_SHIFT 19
#define  HIPWM_HBAT16_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT16_BATLAST_MASK) >> HIPWM_HBAT16_BATLAST_SHIFT)
#define  HIPWM_HBAT16_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT16_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT16_BATLAST_SHIFT))

#define  HIPWM_HBAT16_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT16_BATSIGN_SHIFT 18
#define  HIPWM_HBAT16_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT16_BATSIGN_MASK) >> HIPWM_HBAT16_BATSIGN_SHIFT)
#define  HIPWM_HBAT16_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT16_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT16_BATSIGN_SHIFT))

#define  HIPWM_HBAT16_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT16_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT16_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT16_BATSHIFT_MASK) >> HIPWM_HBAT16_BATSHIFT_SHIFT)
#define  HIPWM_HBAT16_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT16_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT16_BATSHIFT_SHIFT))

#define  HIPWM_HBAT16_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT16_BATFRAC_SHIFT 0
#define  HIPWM_HBAT16_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT16_BATFRAC_MASK) >> HIPWM_HBAT16_BATFRAC_SHIFT)
#define  HIPWM_HBAT16_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT16_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT16_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 18 of 64) (HBAT17)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 18 of 64)*/
//====================================================================

#define  HIPWM_HBAT17_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT17_RESERVED1_SHIFT 20
#define  HIPWM_HBAT17_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT17_RESERVED1_MASK) >> HIPWM_HBAT17_RESERVED1_SHIFT)
#define  HIPWM_HBAT17_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT17_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT17_RESERVED1_SHIFT))

#define  HIPWM_HBAT17_BATLAST_MASK 0x80000
#define  HIPWM_HBAT17_BATLAST_SHIFT 19
#define  HIPWM_HBAT17_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT17_BATLAST_MASK) >> HIPWM_HBAT17_BATLAST_SHIFT)
#define  HIPWM_HBAT17_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT17_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT17_BATLAST_SHIFT))

#define  HIPWM_HBAT17_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT17_BATSIGN_SHIFT 18
#define  HIPWM_HBAT17_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT17_BATSIGN_MASK) >> HIPWM_HBAT17_BATSIGN_SHIFT)
#define  HIPWM_HBAT17_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT17_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT17_BATSIGN_SHIFT))

#define  HIPWM_HBAT17_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT17_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT17_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT17_BATSHIFT_MASK) >> HIPWM_HBAT17_BATSHIFT_SHIFT)
#define  HIPWM_HBAT17_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT17_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT17_BATSHIFT_SHIFT))

#define  HIPWM_HBAT17_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT17_BATFRAC_SHIFT 0
#define  HIPWM_HBAT17_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT17_BATFRAC_MASK) >> HIPWM_HBAT17_BATFRAC_SHIFT)
#define  HIPWM_HBAT17_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT17_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT17_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 19 of 64) (HBAT18)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 19 of 64)*/
//====================================================================

#define  HIPWM_HBAT18_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT18_RESERVED1_SHIFT 20
#define  HIPWM_HBAT18_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT18_RESERVED1_MASK) >> HIPWM_HBAT18_RESERVED1_SHIFT)
#define  HIPWM_HBAT18_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT18_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT18_RESERVED1_SHIFT))

#define  HIPWM_HBAT18_BATLAST_MASK 0x80000
#define  HIPWM_HBAT18_BATLAST_SHIFT 19
#define  HIPWM_HBAT18_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT18_BATLAST_MASK) >> HIPWM_HBAT18_BATLAST_SHIFT)
#define  HIPWM_HBAT18_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT18_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT18_BATLAST_SHIFT))

#define  HIPWM_HBAT18_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT18_BATSIGN_SHIFT 18
#define  HIPWM_HBAT18_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT18_BATSIGN_MASK) >> HIPWM_HBAT18_BATSIGN_SHIFT)
#define  HIPWM_HBAT18_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT18_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT18_BATSIGN_SHIFT))

#define  HIPWM_HBAT18_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT18_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT18_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT18_BATSHIFT_MASK) >> HIPWM_HBAT18_BATSHIFT_SHIFT)
#define  HIPWM_HBAT18_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT18_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT18_BATSHIFT_SHIFT))

#define  HIPWM_HBAT18_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT18_BATFRAC_SHIFT 0
#define  HIPWM_HBAT18_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT18_BATFRAC_MASK) >> HIPWM_HBAT18_BATFRAC_SHIFT)
#define  HIPWM_HBAT18_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT18_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT18_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 20 of 64) (HBAT19)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 20 of 64)*/
//====================================================================

#define  HIPWM_HBAT19_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT19_RESERVED1_SHIFT 20
#define  HIPWM_HBAT19_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT19_RESERVED1_MASK) >> HIPWM_HBAT19_RESERVED1_SHIFT)
#define  HIPWM_HBAT19_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT19_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT19_RESERVED1_SHIFT))

#define  HIPWM_HBAT19_BATLAST_MASK 0x80000
#define  HIPWM_HBAT19_BATLAST_SHIFT 19
#define  HIPWM_HBAT19_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT19_BATLAST_MASK) >> HIPWM_HBAT19_BATLAST_SHIFT)
#define  HIPWM_HBAT19_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT19_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT19_BATLAST_SHIFT))

#define  HIPWM_HBAT19_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT19_BATSIGN_SHIFT 18
#define  HIPWM_HBAT19_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT19_BATSIGN_MASK) >> HIPWM_HBAT19_BATSIGN_SHIFT)
#define  HIPWM_HBAT19_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT19_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT19_BATSIGN_SHIFT))

#define  HIPWM_HBAT19_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT19_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT19_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT19_BATSHIFT_MASK) >> HIPWM_HBAT19_BATSHIFT_SHIFT)
#define  HIPWM_HBAT19_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT19_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT19_BATSHIFT_SHIFT))

#define  HIPWM_HBAT19_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT19_BATFRAC_SHIFT 0
#define  HIPWM_HBAT19_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT19_BATFRAC_MASK) >> HIPWM_HBAT19_BATFRAC_SHIFT)
#define  HIPWM_HBAT19_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT19_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT19_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 21 of 64) (HBAT20)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 21 of 64)*/
//====================================================================

#define  HIPWM_HBAT20_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT20_RESERVED1_SHIFT 20
#define  HIPWM_HBAT20_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT20_RESERVED1_MASK) >> HIPWM_HBAT20_RESERVED1_SHIFT)
#define  HIPWM_HBAT20_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT20_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT20_RESERVED1_SHIFT))

#define  HIPWM_HBAT20_BATLAST_MASK 0x80000
#define  HIPWM_HBAT20_BATLAST_SHIFT 19
#define  HIPWM_HBAT20_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT20_BATLAST_MASK) >> HIPWM_HBAT20_BATLAST_SHIFT)
#define  HIPWM_HBAT20_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT20_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT20_BATLAST_SHIFT))

#define  HIPWM_HBAT20_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT20_BATSIGN_SHIFT 18
#define  HIPWM_HBAT20_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT20_BATSIGN_MASK) >> HIPWM_HBAT20_BATSIGN_SHIFT)
#define  HIPWM_HBAT20_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT20_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT20_BATSIGN_SHIFT))

#define  HIPWM_HBAT20_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT20_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT20_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT20_BATSHIFT_MASK) >> HIPWM_HBAT20_BATSHIFT_SHIFT)
#define  HIPWM_HBAT20_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT20_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT20_BATSHIFT_SHIFT))

#define  HIPWM_HBAT20_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT20_BATFRAC_SHIFT 0
#define  HIPWM_HBAT20_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT20_BATFRAC_MASK) >> HIPWM_HBAT20_BATFRAC_SHIFT)
#define  HIPWM_HBAT20_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT20_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT20_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 22 of 64) (HBAT21)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 22 of 64)*/
//====================================================================

#define  HIPWM_HBAT21_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT21_RESERVED1_SHIFT 20
#define  HIPWM_HBAT21_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT21_RESERVED1_MASK) >> HIPWM_HBAT21_RESERVED1_SHIFT)
#define  HIPWM_HBAT21_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT21_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT21_RESERVED1_SHIFT))

#define  HIPWM_HBAT21_BATLAST_MASK 0x80000
#define  HIPWM_HBAT21_BATLAST_SHIFT 19
#define  HIPWM_HBAT21_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT21_BATLAST_MASK) >> HIPWM_HBAT21_BATLAST_SHIFT)
#define  HIPWM_HBAT21_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT21_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT21_BATLAST_SHIFT))

#define  HIPWM_HBAT21_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT21_BATSIGN_SHIFT 18
#define  HIPWM_HBAT21_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT21_BATSIGN_MASK) >> HIPWM_HBAT21_BATSIGN_SHIFT)
#define  HIPWM_HBAT21_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT21_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT21_BATSIGN_SHIFT))

#define  HIPWM_HBAT21_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT21_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT21_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT21_BATSHIFT_MASK) >> HIPWM_HBAT21_BATSHIFT_SHIFT)
#define  HIPWM_HBAT21_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT21_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT21_BATSHIFT_SHIFT))

#define  HIPWM_HBAT21_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT21_BATFRAC_SHIFT 0
#define  HIPWM_HBAT21_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT21_BATFRAC_MASK) >> HIPWM_HBAT21_BATFRAC_SHIFT)
#define  HIPWM_HBAT21_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT21_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT21_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 23 of 64) (HBAT22)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 23 of 64)*/
//====================================================================

#define  HIPWM_HBAT22_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT22_RESERVED1_SHIFT 20
#define  HIPWM_HBAT22_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT22_RESERVED1_MASK) >> HIPWM_HBAT22_RESERVED1_SHIFT)
#define  HIPWM_HBAT22_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT22_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT22_RESERVED1_SHIFT))

#define  HIPWM_HBAT22_BATLAST_MASK 0x80000
#define  HIPWM_HBAT22_BATLAST_SHIFT 19
#define  HIPWM_HBAT22_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT22_BATLAST_MASK) >> HIPWM_HBAT22_BATLAST_SHIFT)
#define  HIPWM_HBAT22_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT22_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT22_BATLAST_SHIFT))

#define  HIPWM_HBAT22_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT22_BATSIGN_SHIFT 18
#define  HIPWM_HBAT22_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT22_BATSIGN_MASK) >> HIPWM_HBAT22_BATSIGN_SHIFT)
#define  HIPWM_HBAT22_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT22_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT22_BATSIGN_SHIFT))

#define  HIPWM_HBAT22_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT22_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT22_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT22_BATSHIFT_MASK) >> HIPWM_HBAT22_BATSHIFT_SHIFT)
#define  HIPWM_HBAT22_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT22_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT22_BATSHIFT_SHIFT))

#define  HIPWM_HBAT22_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT22_BATFRAC_SHIFT 0
#define  HIPWM_HBAT22_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT22_BATFRAC_MASK) >> HIPWM_HBAT22_BATFRAC_SHIFT)
#define  HIPWM_HBAT22_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT22_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT22_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 24 of 64) (HBAT23)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 24 of 64)*/
//====================================================================

#define  HIPWM_HBAT23_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT23_RESERVED1_SHIFT 20
#define  HIPWM_HBAT23_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT23_RESERVED1_MASK) >> HIPWM_HBAT23_RESERVED1_SHIFT)
#define  HIPWM_HBAT23_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT23_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT23_RESERVED1_SHIFT))

#define  HIPWM_HBAT23_BATLAST_MASK 0x80000
#define  HIPWM_HBAT23_BATLAST_SHIFT 19
#define  HIPWM_HBAT23_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT23_BATLAST_MASK) >> HIPWM_HBAT23_BATLAST_SHIFT)
#define  HIPWM_HBAT23_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT23_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT23_BATLAST_SHIFT))

#define  HIPWM_HBAT23_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT23_BATSIGN_SHIFT 18
#define  HIPWM_HBAT23_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT23_BATSIGN_MASK) >> HIPWM_HBAT23_BATSIGN_SHIFT)
#define  HIPWM_HBAT23_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT23_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT23_BATSIGN_SHIFT))

#define  HIPWM_HBAT23_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT23_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT23_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT23_BATSHIFT_MASK) >> HIPWM_HBAT23_BATSHIFT_SHIFT)
#define  HIPWM_HBAT23_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT23_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT23_BATSHIFT_SHIFT))

#define  HIPWM_HBAT23_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT23_BATFRAC_SHIFT 0
#define  HIPWM_HBAT23_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT23_BATFRAC_MASK) >> HIPWM_HBAT23_BATFRAC_SHIFT)
#define  HIPWM_HBAT23_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT23_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT23_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 25 of 64) (HBAT24)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 25 of 64)*/
//====================================================================

#define  HIPWM_HBAT24_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT24_RESERVED1_SHIFT 20
#define  HIPWM_HBAT24_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT24_RESERVED1_MASK) >> HIPWM_HBAT24_RESERVED1_SHIFT)
#define  HIPWM_HBAT24_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT24_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT24_RESERVED1_SHIFT))

#define  HIPWM_HBAT24_BATLAST_MASK 0x80000
#define  HIPWM_HBAT24_BATLAST_SHIFT 19
#define  HIPWM_HBAT24_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT24_BATLAST_MASK) >> HIPWM_HBAT24_BATLAST_SHIFT)
#define  HIPWM_HBAT24_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT24_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT24_BATLAST_SHIFT))

#define  HIPWM_HBAT24_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT24_BATSIGN_SHIFT 18
#define  HIPWM_HBAT24_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT24_BATSIGN_MASK) >> HIPWM_HBAT24_BATSIGN_SHIFT)
#define  HIPWM_HBAT24_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT24_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT24_BATSIGN_SHIFT))

#define  HIPWM_HBAT24_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT24_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT24_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT24_BATSHIFT_MASK) >> HIPWM_HBAT24_BATSHIFT_SHIFT)
#define  HIPWM_HBAT24_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT24_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT24_BATSHIFT_SHIFT))

#define  HIPWM_HBAT24_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT24_BATFRAC_SHIFT 0
#define  HIPWM_HBAT24_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT24_BATFRAC_MASK) >> HIPWM_HBAT24_BATFRAC_SHIFT)
#define  HIPWM_HBAT24_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT24_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT24_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 26 of 64) (HBAT25)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 26 of 64)*/
//====================================================================

#define  HIPWM_HBAT25_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT25_RESERVED1_SHIFT 20
#define  HIPWM_HBAT25_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT25_RESERVED1_MASK) >> HIPWM_HBAT25_RESERVED1_SHIFT)
#define  HIPWM_HBAT25_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT25_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT25_RESERVED1_SHIFT))

#define  HIPWM_HBAT25_BATLAST_MASK 0x80000
#define  HIPWM_HBAT25_BATLAST_SHIFT 19
#define  HIPWM_HBAT25_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT25_BATLAST_MASK) >> HIPWM_HBAT25_BATLAST_SHIFT)
#define  HIPWM_HBAT25_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT25_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT25_BATLAST_SHIFT))

#define  HIPWM_HBAT25_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT25_BATSIGN_SHIFT 18
#define  HIPWM_HBAT25_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT25_BATSIGN_MASK) >> HIPWM_HBAT25_BATSIGN_SHIFT)
#define  HIPWM_HBAT25_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT25_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT25_BATSIGN_SHIFT))

#define  HIPWM_HBAT25_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT25_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT25_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT25_BATSHIFT_MASK) >> HIPWM_HBAT25_BATSHIFT_SHIFT)
#define  HIPWM_HBAT25_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT25_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT25_BATSHIFT_SHIFT))

#define  HIPWM_HBAT25_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT25_BATFRAC_SHIFT 0
#define  HIPWM_HBAT25_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT25_BATFRAC_MASK) >> HIPWM_HBAT25_BATFRAC_SHIFT)
#define  HIPWM_HBAT25_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT25_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT25_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 27 of 64) (HBAT26)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 27 of 64)*/
//====================================================================

#define  HIPWM_HBAT26_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT26_RESERVED1_SHIFT 20
#define  HIPWM_HBAT26_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT26_RESERVED1_MASK) >> HIPWM_HBAT26_RESERVED1_SHIFT)
#define  HIPWM_HBAT26_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT26_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT26_RESERVED1_SHIFT))

#define  HIPWM_HBAT26_BATLAST_MASK 0x80000
#define  HIPWM_HBAT26_BATLAST_SHIFT 19
#define  HIPWM_HBAT26_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT26_BATLAST_MASK) >> HIPWM_HBAT26_BATLAST_SHIFT)
#define  HIPWM_HBAT26_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT26_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT26_BATLAST_SHIFT))

#define  HIPWM_HBAT26_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT26_BATSIGN_SHIFT 18
#define  HIPWM_HBAT26_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT26_BATSIGN_MASK) >> HIPWM_HBAT26_BATSIGN_SHIFT)
#define  HIPWM_HBAT26_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT26_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT26_BATSIGN_SHIFT))

#define  HIPWM_HBAT26_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT26_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT26_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT26_BATSHIFT_MASK) >> HIPWM_HBAT26_BATSHIFT_SHIFT)
#define  HIPWM_HBAT26_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT26_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT26_BATSHIFT_SHIFT))

#define  HIPWM_HBAT26_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT26_BATFRAC_SHIFT 0
#define  HIPWM_HBAT26_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT26_BATFRAC_MASK) >> HIPWM_HBAT26_BATFRAC_SHIFT)
#define  HIPWM_HBAT26_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT26_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT26_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 28 of 64) (HBAT27)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 28 of 64)*/
//====================================================================

#define  HIPWM_HBAT27_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT27_RESERVED1_SHIFT 20
#define  HIPWM_HBAT27_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT27_RESERVED1_MASK) >> HIPWM_HBAT27_RESERVED1_SHIFT)
#define  HIPWM_HBAT27_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT27_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT27_RESERVED1_SHIFT))

#define  HIPWM_HBAT27_BATLAST_MASK 0x80000
#define  HIPWM_HBAT27_BATLAST_SHIFT 19
#define  HIPWM_HBAT27_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT27_BATLAST_MASK) >> HIPWM_HBAT27_BATLAST_SHIFT)
#define  HIPWM_HBAT27_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT27_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT27_BATLAST_SHIFT))

#define  HIPWM_HBAT27_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT27_BATSIGN_SHIFT 18
#define  HIPWM_HBAT27_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT27_BATSIGN_MASK) >> HIPWM_HBAT27_BATSIGN_SHIFT)
#define  HIPWM_HBAT27_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT27_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT27_BATSIGN_SHIFT))

#define  HIPWM_HBAT27_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT27_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT27_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT27_BATSHIFT_MASK) >> HIPWM_HBAT27_BATSHIFT_SHIFT)
#define  HIPWM_HBAT27_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT27_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT27_BATSHIFT_SHIFT))

#define  HIPWM_HBAT27_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT27_BATFRAC_SHIFT 0
#define  HIPWM_HBAT27_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT27_BATFRAC_MASK) >> HIPWM_HBAT27_BATFRAC_SHIFT)
#define  HIPWM_HBAT27_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT27_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT27_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 29 of 64) (HBAT28)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 29 of 64)*/
//====================================================================

#define  HIPWM_HBAT28_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT28_RESERVED1_SHIFT 20
#define  HIPWM_HBAT28_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT28_RESERVED1_MASK) >> HIPWM_HBAT28_RESERVED1_SHIFT)
#define  HIPWM_HBAT28_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT28_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT28_RESERVED1_SHIFT))

#define  HIPWM_HBAT28_BATLAST_MASK 0x80000
#define  HIPWM_HBAT28_BATLAST_SHIFT 19
#define  HIPWM_HBAT28_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT28_BATLAST_MASK) >> HIPWM_HBAT28_BATLAST_SHIFT)
#define  HIPWM_HBAT28_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT28_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT28_BATLAST_SHIFT))

#define  HIPWM_HBAT28_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT28_BATSIGN_SHIFT 18
#define  HIPWM_HBAT28_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT28_BATSIGN_MASK) >> HIPWM_HBAT28_BATSIGN_SHIFT)
#define  HIPWM_HBAT28_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT28_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT28_BATSIGN_SHIFT))

#define  HIPWM_HBAT28_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT28_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT28_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT28_BATSHIFT_MASK) >> HIPWM_HBAT28_BATSHIFT_SHIFT)
#define  HIPWM_HBAT28_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT28_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT28_BATSHIFT_SHIFT))

#define  HIPWM_HBAT28_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT28_BATFRAC_SHIFT 0
#define  HIPWM_HBAT28_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT28_BATFRAC_MASK) >> HIPWM_HBAT28_BATFRAC_SHIFT)
#define  HIPWM_HBAT28_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT28_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT28_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 30 of 64) (HBAT29)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 30 of 64)*/
//====================================================================

#define  HIPWM_HBAT29_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT29_RESERVED1_SHIFT 20
#define  HIPWM_HBAT29_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT29_RESERVED1_MASK) >> HIPWM_HBAT29_RESERVED1_SHIFT)
#define  HIPWM_HBAT29_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT29_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT29_RESERVED1_SHIFT))

#define  HIPWM_HBAT29_BATLAST_MASK 0x80000
#define  HIPWM_HBAT29_BATLAST_SHIFT 19
#define  HIPWM_HBAT29_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT29_BATLAST_MASK) >> HIPWM_HBAT29_BATLAST_SHIFT)
#define  HIPWM_HBAT29_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT29_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT29_BATLAST_SHIFT))

#define  HIPWM_HBAT29_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT29_BATSIGN_SHIFT 18
#define  HIPWM_HBAT29_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT29_BATSIGN_MASK) >> HIPWM_HBAT29_BATSIGN_SHIFT)
#define  HIPWM_HBAT29_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT29_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT29_BATSIGN_SHIFT))

#define  HIPWM_HBAT29_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT29_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT29_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT29_BATSHIFT_MASK) >> HIPWM_HBAT29_BATSHIFT_SHIFT)
#define  HIPWM_HBAT29_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT29_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT29_BATSHIFT_SHIFT))

#define  HIPWM_HBAT29_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT29_BATFRAC_SHIFT 0
#define  HIPWM_HBAT29_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT29_BATFRAC_MASK) >> HIPWM_HBAT29_BATFRAC_SHIFT)
#define  HIPWM_HBAT29_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT29_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT29_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 31 of 64) (HBAT30)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 31 of 64)*/
//====================================================================

#define  HIPWM_HBAT30_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT30_RESERVED1_SHIFT 20
#define  HIPWM_HBAT30_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT30_RESERVED1_MASK) >> HIPWM_HBAT30_RESERVED1_SHIFT)
#define  HIPWM_HBAT30_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT30_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT30_RESERVED1_SHIFT))

#define  HIPWM_HBAT30_BATLAST_MASK 0x80000
#define  HIPWM_HBAT30_BATLAST_SHIFT 19
#define  HIPWM_HBAT30_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT30_BATLAST_MASK) >> HIPWM_HBAT30_BATLAST_SHIFT)
#define  HIPWM_HBAT30_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT30_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT30_BATLAST_SHIFT))

#define  HIPWM_HBAT30_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT30_BATSIGN_SHIFT 18
#define  HIPWM_HBAT30_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT30_BATSIGN_MASK) >> HIPWM_HBAT30_BATSIGN_SHIFT)
#define  HIPWM_HBAT30_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT30_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT30_BATSIGN_SHIFT))

#define  HIPWM_HBAT30_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT30_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT30_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT30_BATSHIFT_MASK) >> HIPWM_HBAT30_BATSHIFT_SHIFT)
#define  HIPWM_HBAT30_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT30_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT30_BATSHIFT_SHIFT))

#define  HIPWM_HBAT30_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT30_BATFRAC_SHIFT 0
#define  HIPWM_HBAT30_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT30_BATFRAC_MASK) >> HIPWM_HBAT30_BATFRAC_SHIFT)
#define  HIPWM_HBAT30_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT30_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT30_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 32 of 64) (HBAT31)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 32 of 64)*/
//====================================================================

#define  HIPWM_HBAT31_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT31_RESERVED1_SHIFT 20
#define  HIPWM_HBAT31_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT31_RESERVED1_MASK) >> HIPWM_HBAT31_RESERVED1_SHIFT)
#define  HIPWM_HBAT31_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT31_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT31_RESERVED1_SHIFT))

#define  HIPWM_HBAT31_BATLAST_MASK 0x80000
#define  HIPWM_HBAT31_BATLAST_SHIFT 19
#define  HIPWM_HBAT31_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT31_BATLAST_MASK) >> HIPWM_HBAT31_BATLAST_SHIFT)
#define  HIPWM_HBAT31_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT31_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT31_BATLAST_SHIFT))

#define  HIPWM_HBAT31_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT31_BATSIGN_SHIFT 18
#define  HIPWM_HBAT31_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT31_BATSIGN_MASK) >> HIPWM_HBAT31_BATSIGN_SHIFT)
#define  HIPWM_HBAT31_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT31_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT31_BATSIGN_SHIFT))

#define  HIPWM_HBAT31_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT31_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT31_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT31_BATSHIFT_MASK) >> HIPWM_HBAT31_BATSHIFT_SHIFT)
#define  HIPWM_HBAT31_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT31_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT31_BATSHIFT_SHIFT))

#define  HIPWM_HBAT31_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT31_BATFRAC_SHIFT 0
#define  HIPWM_HBAT31_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT31_BATFRAC_MASK) >> HIPWM_HBAT31_BATFRAC_SHIFT)
#define  HIPWM_HBAT31_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT31_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT31_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 33 of 64) (HBAT32)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 33 of 64)*/
//====================================================================

#define  HIPWM_HBAT32_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT32_RESERVED1_SHIFT 20
#define  HIPWM_HBAT32_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT32_RESERVED1_MASK) >> HIPWM_HBAT32_RESERVED1_SHIFT)
#define  HIPWM_HBAT32_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT32_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT32_RESERVED1_SHIFT))

#define  HIPWM_HBAT32_BATLAST_MASK 0x80000
#define  HIPWM_HBAT32_BATLAST_SHIFT 19
#define  HIPWM_HBAT32_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT32_BATLAST_MASK) >> HIPWM_HBAT32_BATLAST_SHIFT)
#define  HIPWM_HBAT32_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT32_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT32_BATLAST_SHIFT))

#define  HIPWM_HBAT32_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT32_BATSIGN_SHIFT 18
#define  HIPWM_HBAT32_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT32_BATSIGN_MASK) >> HIPWM_HBAT32_BATSIGN_SHIFT)
#define  HIPWM_HBAT32_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT32_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT32_BATSIGN_SHIFT))

#define  HIPWM_HBAT32_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT32_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT32_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT32_BATSHIFT_MASK) >> HIPWM_HBAT32_BATSHIFT_SHIFT)
#define  HIPWM_HBAT32_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT32_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT32_BATSHIFT_SHIFT))

#define  HIPWM_HBAT32_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT32_BATFRAC_SHIFT 0
#define  HIPWM_HBAT32_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT32_BATFRAC_MASK) >> HIPWM_HBAT32_BATFRAC_SHIFT)
#define  HIPWM_HBAT32_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT32_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT32_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 34 of 64) (HBAT33)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 34 of 64)*/
//====================================================================

#define  HIPWM_HBAT33_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT33_RESERVED1_SHIFT 20
#define  HIPWM_HBAT33_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT33_RESERVED1_MASK) >> HIPWM_HBAT33_RESERVED1_SHIFT)
#define  HIPWM_HBAT33_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT33_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT33_RESERVED1_SHIFT))

#define  HIPWM_HBAT33_BATLAST_MASK 0x80000
#define  HIPWM_HBAT33_BATLAST_SHIFT 19
#define  HIPWM_HBAT33_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT33_BATLAST_MASK) >> HIPWM_HBAT33_BATLAST_SHIFT)
#define  HIPWM_HBAT33_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT33_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT33_BATLAST_SHIFT))

#define  HIPWM_HBAT33_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT33_BATSIGN_SHIFT 18
#define  HIPWM_HBAT33_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT33_BATSIGN_MASK) >> HIPWM_HBAT33_BATSIGN_SHIFT)
#define  HIPWM_HBAT33_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT33_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT33_BATSIGN_SHIFT))

#define  HIPWM_HBAT33_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT33_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT33_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT33_BATSHIFT_MASK) >> HIPWM_HBAT33_BATSHIFT_SHIFT)
#define  HIPWM_HBAT33_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT33_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT33_BATSHIFT_SHIFT))

#define  HIPWM_HBAT33_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT33_BATFRAC_SHIFT 0
#define  HIPWM_HBAT33_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT33_BATFRAC_MASK) >> HIPWM_HBAT33_BATFRAC_SHIFT)
#define  HIPWM_HBAT33_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT33_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT33_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 35 of 64) (HBAT34)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 35 of 64)*/
//====================================================================

#define  HIPWM_HBAT34_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT34_RESERVED1_SHIFT 20
#define  HIPWM_HBAT34_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT34_RESERVED1_MASK) >> HIPWM_HBAT34_RESERVED1_SHIFT)
#define  HIPWM_HBAT34_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT34_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT34_RESERVED1_SHIFT))

#define  HIPWM_HBAT34_BATLAST_MASK 0x80000
#define  HIPWM_HBAT34_BATLAST_SHIFT 19
#define  HIPWM_HBAT34_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT34_BATLAST_MASK) >> HIPWM_HBAT34_BATLAST_SHIFT)
#define  HIPWM_HBAT34_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT34_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT34_BATLAST_SHIFT))

#define  HIPWM_HBAT34_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT34_BATSIGN_SHIFT 18
#define  HIPWM_HBAT34_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT34_BATSIGN_MASK) >> HIPWM_HBAT34_BATSIGN_SHIFT)
#define  HIPWM_HBAT34_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT34_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT34_BATSIGN_SHIFT))

#define  HIPWM_HBAT34_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT34_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT34_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT34_BATSHIFT_MASK) >> HIPWM_HBAT34_BATSHIFT_SHIFT)
#define  HIPWM_HBAT34_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT34_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT34_BATSHIFT_SHIFT))

#define  HIPWM_HBAT34_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT34_BATFRAC_SHIFT 0
#define  HIPWM_HBAT34_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT34_BATFRAC_MASK) >> HIPWM_HBAT34_BATFRAC_SHIFT)
#define  HIPWM_HBAT34_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT34_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT34_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 36 of 64) (HBAT35)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 36 of 64)*/
//====================================================================

#define  HIPWM_HBAT35_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT35_RESERVED1_SHIFT 20
#define  HIPWM_HBAT35_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT35_RESERVED1_MASK) >> HIPWM_HBAT35_RESERVED1_SHIFT)
#define  HIPWM_HBAT35_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT35_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT35_RESERVED1_SHIFT))

#define  HIPWM_HBAT35_BATLAST_MASK 0x80000
#define  HIPWM_HBAT35_BATLAST_SHIFT 19
#define  HIPWM_HBAT35_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT35_BATLAST_MASK) >> HIPWM_HBAT35_BATLAST_SHIFT)
#define  HIPWM_HBAT35_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT35_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT35_BATLAST_SHIFT))

#define  HIPWM_HBAT35_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT35_BATSIGN_SHIFT 18
#define  HIPWM_HBAT35_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT35_BATSIGN_MASK) >> HIPWM_HBAT35_BATSIGN_SHIFT)
#define  HIPWM_HBAT35_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT35_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT35_BATSIGN_SHIFT))

#define  HIPWM_HBAT35_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT35_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT35_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT35_BATSHIFT_MASK) >> HIPWM_HBAT35_BATSHIFT_SHIFT)
#define  HIPWM_HBAT35_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT35_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT35_BATSHIFT_SHIFT))

#define  HIPWM_HBAT35_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT35_BATFRAC_SHIFT 0
#define  HIPWM_HBAT35_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT35_BATFRAC_MASK) >> HIPWM_HBAT35_BATFRAC_SHIFT)
#define  HIPWM_HBAT35_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT35_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT35_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 37 of 64) (HBAT36)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 37 of 64)*/
//====================================================================

#define  HIPWM_HBAT36_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT36_RESERVED1_SHIFT 20
#define  HIPWM_HBAT36_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT36_RESERVED1_MASK) >> HIPWM_HBAT36_RESERVED1_SHIFT)
#define  HIPWM_HBAT36_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT36_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT36_RESERVED1_SHIFT))

#define  HIPWM_HBAT36_BATLAST_MASK 0x80000
#define  HIPWM_HBAT36_BATLAST_SHIFT 19
#define  HIPWM_HBAT36_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT36_BATLAST_MASK) >> HIPWM_HBAT36_BATLAST_SHIFT)
#define  HIPWM_HBAT36_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT36_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT36_BATLAST_SHIFT))

#define  HIPWM_HBAT36_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT36_BATSIGN_SHIFT 18
#define  HIPWM_HBAT36_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT36_BATSIGN_MASK) >> HIPWM_HBAT36_BATSIGN_SHIFT)
#define  HIPWM_HBAT36_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT36_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT36_BATSIGN_SHIFT))

#define  HIPWM_HBAT36_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT36_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT36_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT36_BATSHIFT_MASK) >> HIPWM_HBAT36_BATSHIFT_SHIFT)
#define  HIPWM_HBAT36_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT36_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT36_BATSHIFT_SHIFT))

#define  HIPWM_HBAT36_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT36_BATFRAC_SHIFT 0
#define  HIPWM_HBAT36_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT36_BATFRAC_MASK) >> HIPWM_HBAT36_BATFRAC_SHIFT)
#define  HIPWM_HBAT36_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT36_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT36_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 38 of 64) (HBAT37)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 38 of 64)*/
//====================================================================

#define  HIPWM_HBAT37_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT37_RESERVED1_SHIFT 20
#define  HIPWM_HBAT37_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT37_RESERVED1_MASK) >> HIPWM_HBAT37_RESERVED1_SHIFT)
#define  HIPWM_HBAT37_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT37_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT37_RESERVED1_SHIFT))

#define  HIPWM_HBAT37_BATLAST_MASK 0x80000
#define  HIPWM_HBAT37_BATLAST_SHIFT 19
#define  HIPWM_HBAT37_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT37_BATLAST_MASK) >> HIPWM_HBAT37_BATLAST_SHIFT)
#define  HIPWM_HBAT37_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT37_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT37_BATLAST_SHIFT))

#define  HIPWM_HBAT37_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT37_BATSIGN_SHIFT 18
#define  HIPWM_HBAT37_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT37_BATSIGN_MASK) >> HIPWM_HBAT37_BATSIGN_SHIFT)
#define  HIPWM_HBAT37_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT37_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT37_BATSIGN_SHIFT))

#define  HIPWM_HBAT37_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT37_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT37_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT37_BATSHIFT_MASK) >> HIPWM_HBAT37_BATSHIFT_SHIFT)
#define  HIPWM_HBAT37_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT37_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT37_BATSHIFT_SHIFT))

#define  HIPWM_HBAT37_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT37_BATFRAC_SHIFT 0
#define  HIPWM_HBAT37_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT37_BATFRAC_MASK) >> HIPWM_HBAT37_BATFRAC_SHIFT)
#define  HIPWM_HBAT37_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT37_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT37_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 39 of 64) (HBAT38)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 39 of 64)*/
//====================================================================

#define  HIPWM_HBAT38_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT38_RESERVED1_SHIFT 20
#define  HIPWM_HBAT38_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT38_RESERVED1_MASK) >> HIPWM_HBAT38_RESERVED1_SHIFT)
#define  HIPWM_HBAT38_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT38_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT38_RESERVED1_SHIFT))

#define  HIPWM_HBAT38_BATLAST_MASK 0x80000
#define  HIPWM_HBAT38_BATLAST_SHIFT 19
#define  HIPWM_HBAT38_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT38_BATLAST_MASK) >> HIPWM_HBAT38_BATLAST_SHIFT)
#define  HIPWM_HBAT38_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT38_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT38_BATLAST_SHIFT))

#define  HIPWM_HBAT38_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT38_BATSIGN_SHIFT 18
#define  HIPWM_HBAT38_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT38_BATSIGN_MASK) >> HIPWM_HBAT38_BATSIGN_SHIFT)
#define  HIPWM_HBAT38_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT38_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT38_BATSIGN_SHIFT))

#define  HIPWM_HBAT38_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT38_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT38_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT38_BATSHIFT_MASK) >> HIPWM_HBAT38_BATSHIFT_SHIFT)
#define  HIPWM_HBAT38_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT38_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT38_BATSHIFT_SHIFT))

#define  HIPWM_HBAT38_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT38_BATFRAC_SHIFT 0
#define  HIPWM_HBAT38_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT38_BATFRAC_MASK) >> HIPWM_HBAT38_BATFRAC_SHIFT)
#define  HIPWM_HBAT38_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT38_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT38_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 40 of 64) (HBAT39)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 40 of 64)*/
//====================================================================

#define  HIPWM_HBAT39_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT39_RESERVED1_SHIFT 20
#define  HIPWM_HBAT39_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT39_RESERVED1_MASK) >> HIPWM_HBAT39_RESERVED1_SHIFT)
#define  HIPWM_HBAT39_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT39_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT39_RESERVED1_SHIFT))

#define  HIPWM_HBAT39_BATLAST_MASK 0x80000
#define  HIPWM_HBAT39_BATLAST_SHIFT 19
#define  HIPWM_HBAT39_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT39_BATLAST_MASK) >> HIPWM_HBAT39_BATLAST_SHIFT)
#define  HIPWM_HBAT39_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT39_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT39_BATLAST_SHIFT))

#define  HIPWM_HBAT39_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT39_BATSIGN_SHIFT 18
#define  HIPWM_HBAT39_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT39_BATSIGN_MASK) >> HIPWM_HBAT39_BATSIGN_SHIFT)
#define  HIPWM_HBAT39_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT39_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT39_BATSIGN_SHIFT))

#define  HIPWM_HBAT39_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT39_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT39_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT39_BATSHIFT_MASK) >> HIPWM_HBAT39_BATSHIFT_SHIFT)
#define  HIPWM_HBAT39_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT39_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT39_BATSHIFT_SHIFT))

#define  HIPWM_HBAT39_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT39_BATFRAC_SHIFT 0
#define  HIPWM_HBAT39_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT39_BATFRAC_MASK) >> HIPWM_HBAT39_BATFRAC_SHIFT)
#define  HIPWM_HBAT39_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT39_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT39_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 41 of 64) (HBAT40)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 41 of 64)*/
//====================================================================

#define  HIPWM_HBAT40_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT40_RESERVED1_SHIFT 20
#define  HIPWM_HBAT40_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT40_RESERVED1_MASK) >> HIPWM_HBAT40_RESERVED1_SHIFT)
#define  HIPWM_HBAT40_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT40_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT40_RESERVED1_SHIFT))

#define  HIPWM_HBAT40_BATLAST_MASK 0x80000
#define  HIPWM_HBAT40_BATLAST_SHIFT 19
#define  HIPWM_HBAT40_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT40_BATLAST_MASK) >> HIPWM_HBAT40_BATLAST_SHIFT)
#define  HIPWM_HBAT40_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT40_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT40_BATLAST_SHIFT))

#define  HIPWM_HBAT40_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT40_BATSIGN_SHIFT 18
#define  HIPWM_HBAT40_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT40_BATSIGN_MASK) >> HIPWM_HBAT40_BATSIGN_SHIFT)
#define  HIPWM_HBAT40_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT40_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT40_BATSIGN_SHIFT))

#define  HIPWM_HBAT40_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT40_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT40_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT40_BATSHIFT_MASK) >> HIPWM_HBAT40_BATSHIFT_SHIFT)
#define  HIPWM_HBAT40_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT40_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT40_BATSHIFT_SHIFT))

#define  HIPWM_HBAT40_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT40_BATFRAC_SHIFT 0
#define  HIPWM_HBAT40_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT40_BATFRAC_MASK) >> HIPWM_HBAT40_BATFRAC_SHIFT)
#define  HIPWM_HBAT40_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT40_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT40_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 42 of 64) (HBAT41)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 42 of 64)*/
//====================================================================

#define  HIPWM_HBAT41_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT41_RESERVED1_SHIFT 20
#define  HIPWM_HBAT41_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT41_RESERVED1_MASK) >> HIPWM_HBAT41_RESERVED1_SHIFT)
#define  HIPWM_HBAT41_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT41_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT41_RESERVED1_SHIFT))

#define  HIPWM_HBAT41_BATLAST_MASK 0x80000
#define  HIPWM_HBAT41_BATLAST_SHIFT 19
#define  HIPWM_HBAT41_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT41_BATLAST_MASK) >> HIPWM_HBAT41_BATLAST_SHIFT)
#define  HIPWM_HBAT41_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT41_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT41_BATLAST_SHIFT))

#define  HIPWM_HBAT41_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT41_BATSIGN_SHIFT 18
#define  HIPWM_HBAT41_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT41_BATSIGN_MASK) >> HIPWM_HBAT41_BATSIGN_SHIFT)
#define  HIPWM_HBAT41_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT41_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT41_BATSIGN_SHIFT))

#define  HIPWM_HBAT41_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT41_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT41_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT41_BATSHIFT_MASK) >> HIPWM_HBAT41_BATSHIFT_SHIFT)
#define  HIPWM_HBAT41_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT41_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT41_BATSHIFT_SHIFT))

#define  HIPWM_HBAT41_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT41_BATFRAC_SHIFT 0
#define  HIPWM_HBAT41_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT41_BATFRAC_MASK) >> HIPWM_HBAT41_BATFRAC_SHIFT)
#define  HIPWM_HBAT41_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT41_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT41_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 43 of 64) (HBAT42)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 43 of 64)*/
//====================================================================

#define  HIPWM_HBAT42_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT42_RESERVED1_SHIFT 20
#define  HIPWM_HBAT42_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT42_RESERVED1_MASK) >> HIPWM_HBAT42_RESERVED1_SHIFT)
#define  HIPWM_HBAT42_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT42_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT42_RESERVED1_SHIFT))

#define  HIPWM_HBAT42_BATLAST_MASK 0x80000
#define  HIPWM_HBAT42_BATLAST_SHIFT 19
#define  HIPWM_HBAT42_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT42_BATLAST_MASK) >> HIPWM_HBAT42_BATLAST_SHIFT)
#define  HIPWM_HBAT42_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT42_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT42_BATLAST_SHIFT))

#define  HIPWM_HBAT42_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT42_BATSIGN_SHIFT 18
#define  HIPWM_HBAT42_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT42_BATSIGN_MASK) >> HIPWM_HBAT42_BATSIGN_SHIFT)
#define  HIPWM_HBAT42_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT42_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT42_BATSIGN_SHIFT))

#define  HIPWM_HBAT42_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT42_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT42_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT42_BATSHIFT_MASK) >> HIPWM_HBAT42_BATSHIFT_SHIFT)
#define  HIPWM_HBAT42_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT42_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT42_BATSHIFT_SHIFT))

#define  HIPWM_HBAT42_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT42_BATFRAC_SHIFT 0
#define  HIPWM_HBAT42_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT42_BATFRAC_MASK) >> HIPWM_HBAT42_BATFRAC_SHIFT)
#define  HIPWM_HBAT42_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT42_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT42_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 44 of 64) (HBAT43)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 44 of 64)*/
//====================================================================

#define  HIPWM_HBAT43_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT43_RESERVED1_SHIFT 20
#define  HIPWM_HBAT43_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT43_RESERVED1_MASK) >> HIPWM_HBAT43_RESERVED1_SHIFT)
#define  HIPWM_HBAT43_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT43_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT43_RESERVED1_SHIFT))

#define  HIPWM_HBAT43_BATLAST_MASK 0x80000
#define  HIPWM_HBAT43_BATLAST_SHIFT 19
#define  HIPWM_HBAT43_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT43_BATLAST_MASK) >> HIPWM_HBAT43_BATLAST_SHIFT)
#define  HIPWM_HBAT43_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT43_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT43_BATLAST_SHIFT))

#define  HIPWM_HBAT43_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT43_BATSIGN_SHIFT 18
#define  HIPWM_HBAT43_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT43_BATSIGN_MASK) >> HIPWM_HBAT43_BATSIGN_SHIFT)
#define  HIPWM_HBAT43_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT43_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT43_BATSIGN_SHIFT))

#define  HIPWM_HBAT43_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT43_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT43_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT43_BATSHIFT_MASK) >> HIPWM_HBAT43_BATSHIFT_SHIFT)
#define  HIPWM_HBAT43_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT43_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT43_BATSHIFT_SHIFT))

#define  HIPWM_HBAT43_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT43_BATFRAC_SHIFT 0
#define  HIPWM_HBAT43_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT43_BATFRAC_MASK) >> HIPWM_HBAT43_BATFRAC_SHIFT)
#define  HIPWM_HBAT43_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT43_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT43_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 45 of 64) (HBAT44)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 45 of 64)*/
//====================================================================

#define  HIPWM_HBAT44_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT44_RESERVED1_SHIFT 20
#define  HIPWM_HBAT44_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT44_RESERVED1_MASK) >> HIPWM_HBAT44_RESERVED1_SHIFT)
#define  HIPWM_HBAT44_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT44_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT44_RESERVED1_SHIFT))

#define  HIPWM_HBAT44_BATLAST_MASK 0x80000
#define  HIPWM_HBAT44_BATLAST_SHIFT 19
#define  HIPWM_HBAT44_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT44_BATLAST_MASK) >> HIPWM_HBAT44_BATLAST_SHIFT)
#define  HIPWM_HBAT44_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT44_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT44_BATLAST_SHIFT))

#define  HIPWM_HBAT44_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT44_BATSIGN_SHIFT 18
#define  HIPWM_HBAT44_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT44_BATSIGN_MASK) >> HIPWM_HBAT44_BATSIGN_SHIFT)
#define  HIPWM_HBAT44_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT44_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT44_BATSIGN_SHIFT))

#define  HIPWM_HBAT44_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT44_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT44_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT44_BATSHIFT_MASK) >> HIPWM_HBAT44_BATSHIFT_SHIFT)
#define  HIPWM_HBAT44_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT44_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT44_BATSHIFT_SHIFT))

#define  HIPWM_HBAT44_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT44_BATFRAC_SHIFT 0
#define  HIPWM_HBAT44_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT44_BATFRAC_MASK) >> HIPWM_HBAT44_BATFRAC_SHIFT)
#define  HIPWM_HBAT44_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT44_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT44_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 46 of 64) (HBAT45)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 46 of 64)*/
//====================================================================

#define  HIPWM_HBAT45_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT45_RESERVED1_SHIFT 20
#define  HIPWM_HBAT45_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT45_RESERVED1_MASK) >> HIPWM_HBAT45_RESERVED1_SHIFT)
#define  HIPWM_HBAT45_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT45_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT45_RESERVED1_SHIFT))

#define  HIPWM_HBAT45_BATLAST_MASK 0x80000
#define  HIPWM_HBAT45_BATLAST_SHIFT 19
#define  HIPWM_HBAT45_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT45_BATLAST_MASK) >> HIPWM_HBAT45_BATLAST_SHIFT)
#define  HIPWM_HBAT45_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT45_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT45_BATLAST_SHIFT))

#define  HIPWM_HBAT45_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT45_BATSIGN_SHIFT 18
#define  HIPWM_HBAT45_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT45_BATSIGN_MASK) >> HIPWM_HBAT45_BATSIGN_SHIFT)
#define  HIPWM_HBAT45_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT45_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT45_BATSIGN_SHIFT))

#define  HIPWM_HBAT45_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT45_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT45_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT45_BATSHIFT_MASK) >> HIPWM_HBAT45_BATSHIFT_SHIFT)
#define  HIPWM_HBAT45_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT45_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT45_BATSHIFT_SHIFT))

#define  HIPWM_HBAT45_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT45_BATFRAC_SHIFT 0
#define  HIPWM_HBAT45_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT45_BATFRAC_MASK) >> HIPWM_HBAT45_BATFRAC_SHIFT)
#define  HIPWM_HBAT45_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT45_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT45_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 47 of 64) (HBAT46)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 47 of 64)*/
//====================================================================

#define  HIPWM_HBAT46_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT46_RESERVED1_SHIFT 20
#define  HIPWM_HBAT46_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT46_RESERVED1_MASK) >> HIPWM_HBAT46_RESERVED1_SHIFT)
#define  HIPWM_HBAT46_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT46_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT46_RESERVED1_SHIFT))

#define  HIPWM_HBAT46_BATLAST_MASK 0x80000
#define  HIPWM_HBAT46_BATLAST_SHIFT 19
#define  HIPWM_HBAT46_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT46_BATLAST_MASK) >> HIPWM_HBAT46_BATLAST_SHIFT)
#define  HIPWM_HBAT46_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT46_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT46_BATLAST_SHIFT))

#define  HIPWM_HBAT46_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT46_BATSIGN_SHIFT 18
#define  HIPWM_HBAT46_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT46_BATSIGN_MASK) >> HIPWM_HBAT46_BATSIGN_SHIFT)
#define  HIPWM_HBAT46_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT46_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT46_BATSIGN_SHIFT))

#define  HIPWM_HBAT46_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT46_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT46_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT46_BATSHIFT_MASK) >> HIPWM_HBAT46_BATSHIFT_SHIFT)
#define  HIPWM_HBAT46_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT46_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT46_BATSHIFT_SHIFT))

#define  HIPWM_HBAT46_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT46_BATFRAC_SHIFT 0
#define  HIPWM_HBAT46_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT46_BATFRAC_MASK) >> HIPWM_HBAT46_BATFRAC_SHIFT)
#define  HIPWM_HBAT46_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT46_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT46_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 48 of 64) (HBAT47)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 48 of 64)*/
//====================================================================

#define  HIPWM_HBAT47_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT47_RESERVED1_SHIFT 20
#define  HIPWM_HBAT47_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT47_RESERVED1_MASK) >> HIPWM_HBAT47_RESERVED1_SHIFT)
#define  HIPWM_HBAT47_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT47_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT47_RESERVED1_SHIFT))

#define  HIPWM_HBAT47_BATLAST_MASK 0x80000
#define  HIPWM_HBAT47_BATLAST_SHIFT 19
#define  HIPWM_HBAT47_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT47_BATLAST_MASK) >> HIPWM_HBAT47_BATLAST_SHIFT)
#define  HIPWM_HBAT47_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT47_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT47_BATLAST_SHIFT))

#define  HIPWM_HBAT47_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT47_BATSIGN_SHIFT 18
#define  HIPWM_HBAT47_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT47_BATSIGN_MASK) >> HIPWM_HBAT47_BATSIGN_SHIFT)
#define  HIPWM_HBAT47_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT47_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT47_BATSIGN_SHIFT))

#define  HIPWM_HBAT47_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT47_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT47_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT47_BATSHIFT_MASK) >> HIPWM_HBAT47_BATSHIFT_SHIFT)
#define  HIPWM_HBAT47_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT47_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT47_BATSHIFT_SHIFT))

#define  HIPWM_HBAT47_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT47_BATFRAC_SHIFT 0
#define  HIPWM_HBAT47_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT47_BATFRAC_MASK) >> HIPWM_HBAT47_BATFRAC_SHIFT)
#define  HIPWM_HBAT47_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT47_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT47_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 49 of 64) (HBAT48)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 49 of 64)*/
//====================================================================

#define  HIPWM_HBAT48_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT48_RESERVED1_SHIFT 20
#define  HIPWM_HBAT48_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT48_RESERVED1_MASK) >> HIPWM_HBAT48_RESERVED1_SHIFT)
#define  HIPWM_HBAT48_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT48_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT48_RESERVED1_SHIFT))

#define  HIPWM_HBAT48_BATLAST_MASK 0x80000
#define  HIPWM_HBAT48_BATLAST_SHIFT 19
#define  HIPWM_HBAT48_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT48_BATLAST_MASK) >> HIPWM_HBAT48_BATLAST_SHIFT)
#define  HIPWM_HBAT48_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT48_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT48_BATLAST_SHIFT))

#define  HIPWM_HBAT48_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT48_BATSIGN_SHIFT 18
#define  HIPWM_HBAT48_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT48_BATSIGN_MASK) >> HIPWM_HBAT48_BATSIGN_SHIFT)
#define  HIPWM_HBAT48_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT48_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT48_BATSIGN_SHIFT))

#define  HIPWM_HBAT48_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT48_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT48_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT48_BATSHIFT_MASK) >> HIPWM_HBAT48_BATSHIFT_SHIFT)
#define  HIPWM_HBAT48_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT48_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT48_BATSHIFT_SHIFT))

#define  HIPWM_HBAT48_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT48_BATFRAC_SHIFT 0
#define  HIPWM_HBAT48_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT48_BATFRAC_MASK) >> HIPWM_HBAT48_BATFRAC_SHIFT)
#define  HIPWM_HBAT48_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT48_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT48_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 50 of 64) (HBAT49)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 50 of 64)*/
//====================================================================

#define  HIPWM_HBAT49_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT49_RESERVED1_SHIFT 20
#define  HIPWM_HBAT49_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT49_RESERVED1_MASK) >> HIPWM_HBAT49_RESERVED1_SHIFT)
#define  HIPWM_HBAT49_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT49_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT49_RESERVED1_SHIFT))

#define  HIPWM_HBAT49_BATLAST_MASK 0x80000
#define  HIPWM_HBAT49_BATLAST_SHIFT 19
#define  HIPWM_HBAT49_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT49_BATLAST_MASK) >> HIPWM_HBAT49_BATLAST_SHIFT)
#define  HIPWM_HBAT49_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT49_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT49_BATLAST_SHIFT))

#define  HIPWM_HBAT49_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT49_BATSIGN_SHIFT 18
#define  HIPWM_HBAT49_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT49_BATSIGN_MASK) >> HIPWM_HBAT49_BATSIGN_SHIFT)
#define  HIPWM_HBAT49_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT49_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT49_BATSIGN_SHIFT))

#define  HIPWM_HBAT49_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT49_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT49_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT49_BATSHIFT_MASK) >> HIPWM_HBAT49_BATSHIFT_SHIFT)
#define  HIPWM_HBAT49_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT49_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT49_BATSHIFT_SHIFT))

#define  HIPWM_HBAT49_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT49_BATFRAC_SHIFT 0
#define  HIPWM_HBAT49_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT49_BATFRAC_MASK) >> HIPWM_HBAT49_BATFRAC_SHIFT)
#define  HIPWM_HBAT49_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT49_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT49_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 51 of 64) (HBAT50)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 51 of 64)*/
//====================================================================

#define  HIPWM_HBAT50_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT50_RESERVED1_SHIFT 20
#define  HIPWM_HBAT50_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT50_RESERVED1_MASK) >> HIPWM_HBAT50_RESERVED1_SHIFT)
#define  HIPWM_HBAT50_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT50_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT50_RESERVED1_SHIFT))

#define  HIPWM_HBAT50_BATLAST_MASK 0x80000
#define  HIPWM_HBAT50_BATLAST_SHIFT 19
#define  HIPWM_HBAT50_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT50_BATLAST_MASK) >> HIPWM_HBAT50_BATLAST_SHIFT)
#define  HIPWM_HBAT50_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT50_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT50_BATLAST_SHIFT))

#define  HIPWM_HBAT50_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT50_BATSIGN_SHIFT 18
#define  HIPWM_HBAT50_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT50_BATSIGN_MASK) >> HIPWM_HBAT50_BATSIGN_SHIFT)
#define  HIPWM_HBAT50_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT50_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT50_BATSIGN_SHIFT))

#define  HIPWM_HBAT50_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT50_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT50_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT50_BATSHIFT_MASK) >> HIPWM_HBAT50_BATSHIFT_SHIFT)
#define  HIPWM_HBAT50_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT50_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT50_BATSHIFT_SHIFT))

#define  HIPWM_HBAT50_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT50_BATFRAC_SHIFT 0
#define  HIPWM_HBAT50_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT50_BATFRAC_MASK) >> HIPWM_HBAT50_BATFRAC_SHIFT)
#define  HIPWM_HBAT50_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT50_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT50_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 52 of 64) (HBAT51)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 52 of 64)*/
//====================================================================

#define  HIPWM_HBAT51_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT51_RESERVED1_SHIFT 20
#define  HIPWM_HBAT51_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT51_RESERVED1_MASK) >> HIPWM_HBAT51_RESERVED1_SHIFT)
#define  HIPWM_HBAT51_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT51_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT51_RESERVED1_SHIFT))

#define  HIPWM_HBAT51_BATLAST_MASK 0x80000
#define  HIPWM_HBAT51_BATLAST_SHIFT 19
#define  HIPWM_HBAT51_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT51_BATLAST_MASK) >> HIPWM_HBAT51_BATLAST_SHIFT)
#define  HIPWM_HBAT51_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT51_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT51_BATLAST_SHIFT))

#define  HIPWM_HBAT51_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT51_BATSIGN_SHIFT 18
#define  HIPWM_HBAT51_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT51_BATSIGN_MASK) >> HIPWM_HBAT51_BATSIGN_SHIFT)
#define  HIPWM_HBAT51_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT51_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT51_BATSIGN_SHIFT))

#define  HIPWM_HBAT51_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT51_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT51_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT51_BATSHIFT_MASK) >> HIPWM_HBAT51_BATSHIFT_SHIFT)
#define  HIPWM_HBAT51_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT51_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT51_BATSHIFT_SHIFT))

#define  HIPWM_HBAT51_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT51_BATFRAC_SHIFT 0
#define  HIPWM_HBAT51_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT51_BATFRAC_MASK) >> HIPWM_HBAT51_BATFRAC_SHIFT)
#define  HIPWM_HBAT51_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT51_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT51_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 53 of 64) (HBAT52)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 53 of 64)*/
//====================================================================

#define  HIPWM_HBAT52_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT52_RESERVED1_SHIFT 20
#define  HIPWM_HBAT52_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT52_RESERVED1_MASK) >> HIPWM_HBAT52_RESERVED1_SHIFT)
#define  HIPWM_HBAT52_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT52_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT52_RESERVED1_SHIFT))

#define  HIPWM_HBAT52_BATLAST_MASK 0x80000
#define  HIPWM_HBAT52_BATLAST_SHIFT 19
#define  HIPWM_HBAT52_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT52_BATLAST_MASK) >> HIPWM_HBAT52_BATLAST_SHIFT)
#define  HIPWM_HBAT52_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT52_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT52_BATLAST_SHIFT))

#define  HIPWM_HBAT52_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT52_BATSIGN_SHIFT 18
#define  HIPWM_HBAT52_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT52_BATSIGN_MASK) >> HIPWM_HBAT52_BATSIGN_SHIFT)
#define  HIPWM_HBAT52_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT52_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT52_BATSIGN_SHIFT))

#define  HIPWM_HBAT52_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT52_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT52_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT52_BATSHIFT_MASK) >> HIPWM_HBAT52_BATSHIFT_SHIFT)
#define  HIPWM_HBAT52_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT52_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT52_BATSHIFT_SHIFT))

#define  HIPWM_HBAT52_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT52_BATFRAC_SHIFT 0
#define  HIPWM_HBAT52_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT52_BATFRAC_MASK) >> HIPWM_HBAT52_BATFRAC_SHIFT)
#define  HIPWM_HBAT52_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT52_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT52_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 54 of 64) (HBAT53)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 54 of 64)*/
//====================================================================

#define  HIPWM_HBAT53_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT53_RESERVED1_SHIFT 20
#define  HIPWM_HBAT53_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT53_RESERVED1_MASK) >> HIPWM_HBAT53_RESERVED1_SHIFT)
#define  HIPWM_HBAT53_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT53_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT53_RESERVED1_SHIFT))

#define  HIPWM_HBAT53_BATLAST_MASK 0x80000
#define  HIPWM_HBAT53_BATLAST_SHIFT 19
#define  HIPWM_HBAT53_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT53_BATLAST_MASK) >> HIPWM_HBAT53_BATLAST_SHIFT)
#define  HIPWM_HBAT53_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT53_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT53_BATLAST_SHIFT))

#define  HIPWM_HBAT53_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT53_BATSIGN_SHIFT 18
#define  HIPWM_HBAT53_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT53_BATSIGN_MASK) >> HIPWM_HBAT53_BATSIGN_SHIFT)
#define  HIPWM_HBAT53_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT53_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT53_BATSIGN_SHIFT))

#define  HIPWM_HBAT53_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT53_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT53_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT53_BATSHIFT_MASK) >> HIPWM_HBAT53_BATSHIFT_SHIFT)
#define  HIPWM_HBAT53_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT53_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT53_BATSHIFT_SHIFT))

#define  HIPWM_HBAT53_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT53_BATFRAC_SHIFT 0
#define  HIPWM_HBAT53_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT53_BATFRAC_MASK) >> HIPWM_HBAT53_BATFRAC_SHIFT)
#define  HIPWM_HBAT53_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT53_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT53_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 55 of 64) (HBAT54)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 55 of 64)*/
//====================================================================

#define  HIPWM_HBAT54_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT54_RESERVED1_SHIFT 20
#define  HIPWM_HBAT54_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT54_RESERVED1_MASK) >> HIPWM_HBAT54_RESERVED1_SHIFT)
#define  HIPWM_HBAT54_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT54_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT54_RESERVED1_SHIFT))

#define  HIPWM_HBAT54_BATLAST_MASK 0x80000
#define  HIPWM_HBAT54_BATLAST_SHIFT 19
#define  HIPWM_HBAT54_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT54_BATLAST_MASK) >> HIPWM_HBAT54_BATLAST_SHIFT)
#define  HIPWM_HBAT54_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT54_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT54_BATLAST_SHIFT))

#define  HIPWM_HBAT54_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT54_BATSIGN_SHIFT 18
#define  HIPWM_HBAT54_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT54_BATSIGN_MASK) >> HIPWM_HBAT54_BATSIGN_SHIFT)
#define  HIPWM_HBAT54_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT54_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT54_BATSIGN_SHIFT))

#define  HIPWM_HBAT54_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT54_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT54_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT54_BATSHIFT_MASK) >> HIPWM_HBAT54_BATSHIFT_SHIFT)
#define  HIPWM_HBAT54_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT54_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT54_BATSHIFT_SHIFT))

#define  HIPWM_HBAT54_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT54_BATFRAC_SHIFT 0
#define  HIPWM_HBAT54_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT54_BATFRAC_MASK) >> HIPWM_HBAT54_BATFRAC_SHIFT)
#define  HIPWM_HBAT54_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT54_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT54_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 56 of 64) (HBAT55)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 56 of 64)*/
//====================================================================

#define  HIPWM_HBAT55_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT55_RESERVED1_SHIFT 20
#define  HIPWM_HBAT55_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT55_RESERVED1_MASK) >> HIPWM_HBAT55_RESERVED1_SHIFT)
#define  HIPWM_HBAT55_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT55_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT55_RESERVED1_SHIFT))

#define  HIPWM_HBAT55_BATLAST_MASK 0x80000
#define  HIPWM_HBAT55_BATLAST_SHIFT 19
#define  HIPWM_HBAT55_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT55_BATLAST_MASK) >> HIPWM_HBAT55_BATLAST_SHIFT)
#define  HIPWM_HBAT55_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT55_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT55_BATLAST_SHIFT))

#define  HIPWM_HBAT55_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT55_BATSIGN_SHIFT 18
#define  HIPWM_HBAT55_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT55_BATSIGN_MASK) >> HIPWM_HBAT55_BATSIGN_SHIFT)
#define  HIPWM_HBAT55_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT55_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT55_BATSIGN_SHIFT))

#define  HIPWM_HBAT55_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT55_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT55_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT55_BATSHIFT_MASK) >> HIPWM_HBAT55_BATSHIFT_SHIFT)
#define  HIPWM_HBAT55_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT55_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT55_BATSHIFT_SHIFT))

#define  HIPWM_HBAT55_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT55_BATFRAC_SHIFT 0
#define  HIPWM_HBAT55_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT55_BATFRAC_MASK) >> HIPWM_HBAT55_BATFRAC_SHIFT)
#define  HIPWM_HBAT55_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT55_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT55_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 57 of 64) (HBAT56)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 57 of 64)*/
//====================================================================

#define  HIPWM_HBAT56_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT56_RESERVED1_SHIFT 20
#define  HIPWM_HBAT56_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT56_RESERVED1_MASK) >> HIPWM_HBAT56_RESERVED1_SHIFT)
#define  HIPWM_HBAT56_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT56_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT56_RESERVED1_SHIFT))

#define  HIPWM_HBAT56_BATLAST_MASK 0x80000
#define  HIPWM_HBAT56_BATLAST_SHIFT 19
#define  HIPWM_HBAT56_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT56_BATLAST_MASK) >> HIPWM_HBAT56_BATLAST_SHIFT)
#define  HIPWM_HBAT56_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT56_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT56_BATLAST_SHIFT))

#define  HIPWM_HBAT56_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT56_BATSIGN_SHIFT 18
#define  HIPWM_HBAT56_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT56_BATSIGN_MASK) >> HIPWM_HBAT56_BATSIGN_SHIFT)
#define  HIPWM_HBAT56_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT56_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT56_BATSIGN_SHIFT))

#define  HIPWM_HBAT56_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT56_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT56_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT56_BATSHIFT_MASK) >> HIPWM_HBAT56_BATSHIFT_SHIFT)
#define  HIPWM_HBAT56_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT56_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT56_BATSHIFT_SHIFT))

#define  HIPWM_HBAT56_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT56_BATFRAC_SHIFT 0
#define  HIPWM_HBAT56_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT56_BATFRAC_MASK) >> HIPWM_HBAT56_BATFRAC_SHIFT)
#define  HIPWM_HBAT56_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT56_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT56_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 58 of 64) (HBAT57)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 58 of 64)*/
//====================================================================

#define  HIPWM_HBAT57_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT57_RESERVED1_SHIFT 20
#define  HIPWM_HBAT57_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT57_RESERVED1_MASK) >> HIPWM_HBAT57_RESERVED1_SHIFT)
#define  HIPWM_HBAT57_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT57_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT57_RESERVED1_SHIFT))

#define  HIPWM_HBAT57_BATLAST_MASK 0x80000
#define  HIPWM_HBAT57_BATLAST_SHIFT 19
#define  HIPWM_HBAT57_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT57_BATLAST_MASK) >> HIPWM_HBAT57_BATLAST_SHIFT)
#define  HIPWM_HBAT57_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT57_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT57_BATLAST_SHIFT))

#define  HIPWM_HBAT57_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT57_BATSIGN_SHIFT 18
#define  HIPWM_HBAT57_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT57_BATSIGN_MASK) >> HIPWM_HBAT57_BATSIGN_SHIFT)
#define  HIPWM_HBAT57_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT57_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT57_BATSIGN_SHIFT))

#define  HIPWM_HBAT57_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT57_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT57_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT57_BATSHIFT_MASK) >> HIPWM_HBAT57_BATSHIFT_SHIFT)
#define  HIPWM_HBAT57_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT57_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT57_BATSHIFT_SHIFT))

#define  HIPWM_HBAT57_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT57_BATFRAC_SHIFT 0
#define  HIPWM_HBAT57_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT57_BATFRAC_MASK) >> HIPWM_HBAT57_BATFRAC_SHIFT)
#define  HIPWM_HBAT57_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT57_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT57_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 59 of 64) (HBAT58)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 59 of 64)*/
//====================================================================

#define  HIPWM_HBAT58_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT58_RESERVED1_SHIFT 20
#define  HIPWM_HBAT58_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT58_RESERVED1_MASK) >> HIPWM_HBAT58_RESERVED1_SHIFT)
#define  HIPWM_HBAT58_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT58_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT58_RESERVED1_SHIFT))

#define  HIPWM_HBAT58_BATLAST_MASK 0x80000
#define  HIPWM_HBAT58_BATLAST_SHIFT 19
#define  HIPWM_HBAT58_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT58_BATLAST_MASK) >> HIPWM_HBAT58_BATLAST_SHIFT)
#define  HIPWM_HBAT58_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT58_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT58_BATLAST_SHIFT))

#define  HIPWM_HBAT58_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT58_BATSIGN_SHIFT 18
#define  HIPWM_HBAT58_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT58_BATSIGN_MASK) >> HIPWM_HBAT58_BATSIGN_SHIFT)
#define  HIPWM_HBAT58_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT58_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT58_BATSIGN_SHIFT))

#define  HIPWM_HBAT58_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT58_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT58_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT58_BATSHIFT_MASK) >> HIPWM_HBAT58_BATSHIFT_SHIFT)
#define  HIPWM_HBAT58_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT58_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT58_BATSHIFT_SHIFT))

#define  HIPWM_HBAT58_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT58_BATFRAC_SHIFT 0
#define  HIPWM_HBAT58_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT58_BATFRAC_MASK) >> HIPWM_HBAT58_BATFRAC_SHIFT)
#define  HIPWM_HBAT58_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT58_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT58_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 60 of 64) (HBAT59)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 60 of 64)*/
//====================================================================

#define  HIPWM_HBAT59_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT59_RESERVED1_SHIFT 20
#define  HIPWM_HBAT59_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT59_RESERVED1_MASK) >> HIPWM_HBAT59_RESERVED1_SHIFT)
#define  HIPWM_HBAT59_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT59_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT59_RESERVED1_SHIFT))

#define  HIPWM_HBAT59_BATLAST_MASK 0x80000
#define  HIPWM_HBAT59_BATLAST_SHIFT 19
#define  HIPWM_HBAT59_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT59_BATLAST_MASK) >> HIPWM_HBAT59_BATLAST_SHIFT)
#define  HIPWM_HBAT59_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT59_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT59_BATLAST_SHIFT))

#define  HIPWM_HBAT59_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT59_BATSIGN_SHIFT 18
#define  HIPWM_HBAT59_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT59_BATSIGN_MASK) >> HIPWM_HBAT59_BATSIGN_SHIFT)
#define  HIPWM_HBAT59_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT59_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT59_BATSIGN_SHIFT))

#define  HIPWM_HBAT59_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT59_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT59_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT59_BATSHIFT_MASK) >> HIPWM_HBAT59_BATSHIFT_SHIFT)
#define  HIPWM_HBAT59_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT59_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT59_BATSHIFT_SHIFT))

#define  HIPWM_HBAT59_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT59_BATFRAC_SHIFT 0
#define  HIPWM_HBAT59_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT59_BATFRAC_MASK) >> HIPWM_HBAT59_BATFRAC_SHIFT)
#define  HIPWM_HBAT59_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT59_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT59_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 61 of 64) (HBAT60)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 61 of 64)*/
//====================================================================

#define  HIPWM_HBAT60_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT60_RESERVED1_SHIFT 20
#define  HIPWM_HBAT60_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT60_RESERVED1_MASK) >> HIPWM_HBAT60_RESERVED1_SHIFT)
#define  HIPWM_HBAT60_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT60_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT60_RESERVED1_SHIFT))

#define  HIPWM_HBAT60_BATLAST_MASK 0x80000
#define  HIPWM_HBAT60_BATLAST_SHIFT 19
#define  HIPWM_HBAT60_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT60_BATLAST_MASK) >> HIPWM_HBAT60_BATLAST_SHIFT)
#define  HIPWM_HBAT60_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT60_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT60_BATLAST_SHIFT))

#define  HIPWM_HBAT60_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT60_BATSIGN_SHIFT 18
#define  HIPWM_HBAT60_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT60_BATSIGN_MASK) >> HIPWM_HBAT60_BATSIGN_SHIFT)
#define  HIPWM_HBAT60_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT60_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT60_BATSIGN_SHIFT))

#define  HIPWM_HBAT60_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT60_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT60_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT60_BATSHIFT_MASK) >> HIPWM_HBAT60_BATSHIFT_SHIFT)
#define  HIPWM_HBAT60_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT60_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT60_BATSHIFT_SHIFT))

#define  HIPWM_HBAT60_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT60_BATFRAC_SHIFT 0
#define  HIPWM_HBAT60_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT60_BATFRAC_MASK) >> HIPWM_HBAT60_BATFRAC_SHIFT)
#define  HIPWM_HBAT60_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT60_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT60_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 62 of 64) (HBAT61)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 62 of 64)*/
//====================================================================

#define  HIPWM_HBAT61_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT61_RESERVED1_SHIFT 20
#define  HIPWM_HBAT61_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT61_RESERVED1_MASK) >> HIPWM_HBAT61_RESERVED1_SHIFT)
#define  HIPWM_HBAT61_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT61_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT61_RESERVED1_SHIFT))

#define  HIPWM_HBAT61_BATLAST_MASK 0x80000
#define  HIPWM_HBAT61_BATLAST_SHIFT 19
#define  HIPWM_HBAT61_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT61_BATLAST_MASK) >> HIPWM_HBAT61_BATLAST_SHIFT)
#define  HIPWM_HBAT61_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT61_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT61_BATLAST_SHIFT))

#define  HIPWM_HBAT61_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT61_BATSIGN_SHIFT 18
#define  HIPWM_HBAT61_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT61_BATSIGN_MASK) >> HIPWM_HBAT61_BATSIGN_SHIFT)
#define  HIPWM_HBAT61_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT61_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT61_BATSIGN_SHIFT))

#define  HIPWM_HBAT61_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT61_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT61_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT61_BATSHIFT_MASK) >> HIPWM_HBAT61_BATSHIFT_SHIFT)
#define  HIPWM_HBAT61_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT61_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT61_BATSHIFT_SHIFT))

#define  HIPWM_HBAT61_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT61_BATFRAC_SHIFT 0
#define  HIPWM_HBAT61_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT61_BATFRAC_MASK) >> HIPWM_HBAT61_BATFRAC_SHIFT)
#define  HIPWM_HBAT61_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT61_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT61_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 63 of 64) (HBAT62)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 63 of 64)*/
//====================================================================

#define  HIPWM_HBAT62_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT62_RESERVED1_SHIFT 20
#define  HIPWM_HBAT62_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT62_RESERVED1_MASK) >> HIPWM_HBAT62_RESERVED1_SHIFT)
#define  HIPWM_HBAT62_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT62_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT62_RESERVED1_SHIFT))

#define  HIPWM_HBAT62_BATLAST_MASK 0x80000
#define  HIPWM_HBAT62_BATLAST_SHIFT 19
#define  HIPWM_HBAT62_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT62_BATLAST_MASK) >> HIPWM_HBAT62_BATLAST_SHIFT)
#define  HIPWM_HBAT62_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT62_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT62_BATLAST_SHIFT))

#define  HIPWM_HBAT62_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT62_BATSIGN_SHIFT 18
#define  HIPWM_HBAT62_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT62_BATSIGN_MASK) >> HIPWM_HBAT62_BATSIGN_SHIFT)
#define  HIPWM_HBAT62_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT62_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT62_BATSIGN_SHIFT))

#define  HIPWM_HBAT62_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT62_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT62_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT62_BATSHIFT_MASK) >> HIPWM_HBAT62_BATSHIFT_SHIFT)
#define  HIPWM_HBAT62_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT62_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT62_BATSHIFT_SHIFT))

#define  HIPWM_HBAT62_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT62_BATFRAC_SHIFT 0
#define  HIPWM_HBAT62_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT62_BATFRAC_MASK) >> HIPWM_HBAT62_BATFRAC_SHIFT)
#define  HIPWM_HBAT62_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT62_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT62_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM BENDER Acceleration Table (Instance 64 of 64) (HBAT63)
/** \brief The HiPWM BENDER Acceleration Table (HBAT) consists of 64 entries. (Instance 64 of 64)*/
//====================================================================

#define  HIPWM_HBAT63_RESERVED1_MASK 0xfff00000
#define  HIPWM_HBAT63_RESERVED1_SHIFT 20
#define  HIPWM_HBAT63_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT63_RESERVED1_MASK) >> HIPWM_HBAT63_RESERVED1_SHIFT)
#define  HIPWM_HBAT63_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT63_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HBAT63_RESERVED1_SHIFT))

#define  HIPWM_HBAT63_BATLAST_MASK 0x80000
#define  HIPWM_HBAT63_BATLAST_SHIFT 19
#define  HIPWM_HBAT63_BATLAST_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT63_BATLAST_MASK) >> HIPWM_HBAT63_BATLAST_SHIFT)
#define  HIPWM_HBAT63_BATLAST_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT63_BATLAST_MASK) | (((uint32_t)val) << HIPWM_HBAT63_BATLAST_SHIFT))

#define  HIPWM_HBAT63_BATSIGN_MASK 0x40000
#define  HIPWM_HBAT63_BATSIGN_SHIFT 18
#define  HIPWM_HBAT63_BATSIGN_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT63_BATSIGN_MASK) >> HIPWM_HBAT63_BATSIGN_SHIFT)
#define  HIPWM_HBAT63_BATSIGN_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT63_BATSIGN_MASK) | (((uint32_t)val) << HIPWM_HBAT63_BATSIGN_SHIFT))

#define  HIPWM_HBAT63_BATSHIFT_MASK 0x3c000
#define  HIPWM_HBAT63_BATSHIFT_SHIFT 14
#define  HIPWM_HBAT63_BATSHIFT_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT63_BATSHIFT_MASK) >> HIPWM_HBAT63_BATSHIFT_SHIFT)
#define  HIPWM_HBAT63_BATSHIFT_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT63_BATSHIFT_MASK) | (((uint32_t)val) << HIPWM_HBAT63_BATSHIFT_SHIFT))

#define  HIPWM_HBAT63_BATFRAC_MASK 0x3fff
#define  HIPWM_HBAT63_BATFRAC_SHIFT 0
#define  HIPWM_HBAT63_BATFRAC_MASK_SHIFT(reg) (((reg) & HIPWM_HBAT63_BATFRAC_MASK) >> HIPWM_HBAT63_BATFRAC_SHIFT)
#define  HIPWM_HBAT63_BATFRAC_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HBAT63_BATFRAC_MASK) | (((uint32_t)val) << HIPWM_HBAT63_BATFRAC_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 1 of 12) (HMIC0)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 1 of 12)*/
//====================================================================

#define  HIPWM_HMIC0_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC0_RESERVED1_SHIFT 12
#define  HIPWM_HMIC0_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC0_RESERVED1_MASK) >> HIPWM_HMIC0_RESERVED1_SHIFT)
#define  HIPWM_HMIC0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC0_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC0_RESERVED1_SHIFT))

#define  HIPWM_HMIC0_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC0_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC0_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC0_MICINTPIX_MASK) >> HIPWM_HMIC0_MICINTPIX_SHIFT)
#define  HIPWM_HMIC0_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC0_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC0_MICINTPIX_SHIFT))

#define  HIPWM_HMIC0_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC0_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC0_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC0_MICFRACPIX_MASK) >> HIPWM_HMIC0_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC0_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC0_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC0_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 2 of 12) (HMIC1)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 2 of 12)*/
//====================================================================

#define  HIPWM_HMIC1_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC1_RESERVED1_SHIFT 12
#define  HIPWM_HMIC1_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC1_RESERVED1_MASK) >> HIPWM_HMIC1_RESERVED1_SHIFT)
#define  HIPWM_HMIC1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC1_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC1_RESERVED1_SHIFT))

#define  HIPWM_HMIC1_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC1_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC1_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC1_MICINTPIX_MASK) >> HIPWM_HMIC1_MICINTPIX_SHIFT)
#define  HIPWM_HMIC1_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC1_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC1_MICINTPIX_SHIFT))

#define  HIPWM_HMIC1_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC1_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC1_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC1_MICFRACPIX_MASK) >> HIPWM_HMIC1_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC1_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC1_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC1_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 3 of 12) (HMIC2)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 3 of 12)*/
//====================================================================

#define  HIPWM_HMIC2_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC2_RESERVED1_SHIFT 12
#define  HIPWM_HMIC2_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC2_RESERVED1_MASK) >> HIPWM_HMIC2_RESERVED1_SHIFT)
#define  HIPWM_HMIC2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC2_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC2_RESERVED1_SHIFT))

#define  HIPWM_HMIC2_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC2_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC2_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC2_MICINTPIX_MASK) >> HIPWM_HMIC2_MICINTPIX_SHIFT)
#define  HIPWM_HMIC2_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC2_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC2_MICINTPIX_SHIFT))

#define  HIPWM_HMIC2_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC2_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC2_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC2_MICFRACPIX_MASK) >> HIPWM_HMIC2_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC2_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC2_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC2_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 4 of 12) (HMIC3)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 4 of 12)*/
//====================================================================

#define  HIPWM_HMIC3_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC3_RESERVED1_SHIFT 12
#define  HIPWM_HMIC3_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC3_RESERVED1_MASK) >> HIPWM_HMIC3_RESERVED1_SHIFT)
#define  HIPWM_HMIC3_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC3_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC3_RESERVED1_SHIFT))

#define  HIPWM_HMIC3_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC3_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC3_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC3_MICINTPIX_MASK) >> HIPWM_HMIC3_MICINTPIX_SHIFT)
#define  HIPWM_HMIC3_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC3_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC3_MICINTPIX_SHIFT))

#define  HIPWM_HMIC3_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC3_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC3_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC3_MICFRACPIX_MASK) >> HIPWM_HMIC3_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC3_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC3_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC3_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 5 of 12) (HMIC4)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 5 of 12)*/
//====================================================================

#define  HIPWM_HMIC4_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC4_RESERVED1_SHIFT 12
#define  HIPWM_HMIC4_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC4_RESERVED1_MASK) >> HIPWM_HMIC4_RESERVED1_SHIFT)
#define  HIPWM_HMIC4_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC4_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC4_RESERVED1_SHIFT))

#define  HIPWM_HMIC4_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC4_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC4_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC4_MICINTPIX_MASK) >> HIPWM_HMIC4_MICINTPIX_SHIFT)
#define  HIPWM_HMIC4_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC4_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC4_MICINTPIX_SHIFT))

#define  HIPWM_HMIC4_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC4_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC4_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC4_MICFRACPIX_MASK) >> HIPWM_HMIC4_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC4_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC4_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC4_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 6 of 12) (HMIC5)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 6 of 12)*/
//====================================================================

#define  HIPWM_HMIC5_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC5_RESERVED1_SHIFT 12
#define  HIPWM_HMIC5_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC5_RESERVED1_MASK) >> HIPWM_HMIC5_RESERVED1_SHIFT)
#define  HIPWM_HMIC5_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC5_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC5_RESERVED1_SHIFT))

#define  HIPWM_HMIC5_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC5_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC5_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC5_MICINTPIX_MASK) >> HIPWM_HMIC5_MICINTPIX_SHIFT)
#define  HIPWM_HMIC5_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC5_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC5_MICINTPIX_SHIFT))

#define  HIPWM_HMIC5_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC5_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC5_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC5_MICFRACPIX_MASK) >> HIPWM_HMIC5_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC5_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC5_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC5_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 7 of 12) (HMIC6)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 7 of 12)*/
//====================================================================

#define  HIPWM_HMIC6_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC6_RESERVED1_SHIFT 12
#define  HIPWM_HMIC6_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC6_RESERVED1_MASK) >> HIPWM_HMIC6_RESERVED1_SHIFT)
#define  HIPWM_HMIC6_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC6_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC6_RESERVED1_SHIFT))

#define  HIPWM_HMIC6_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC6_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC6_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC6_MICINTPIX_MASK) >> HIPWM_HMIC6_MICINTPIX_SHIFT)
#define  HIPWM_HMIC6_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC6_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC6_MICINTPIX_SHIFT))

#define  HIPWM_HMIC6_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC6_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC6_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC6_MICFRACPIX_MASK) >> HIPWM_HMIC6_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC6_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC6_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC6_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 8 of 12) (HMIC7)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 8 of 12)*/
//====================================================================

#define  HIPWM_HMIC7_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC7_RESERVED1_SHIFT 12
#define  HIPWM_HMIC7_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC7_RESERVED1_MASK) >> HIPWM_HMIC7_RESERVED1_SHIFT)
#define  HIPWM_HMIC7_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC7_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC7_RESERVED1_SHIFT))

#define  HIPWM_HMIC7_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC7_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC7_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC7_MICINTPIX_MASK) >> HIPWM_HMIC7_MICINTPIX_SHIFT)
#define  HIPWM_HMIC7_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC7_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC7_MICINTPIX_SHIFT))

#define  HIPWM_HMIC7_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC7_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC7_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC7_MICFRACPIX_MASK) >> HIPWM_HMIC7_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC7_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC7_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC7_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 9 of 12) (HMIC8)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 9 of 12)*/
//====================================================================

#define  HIPWM_HMIC8_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC8_RESERVED1_SHIFT 12
#define  HIPWM_HMIC8_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC8_RESERVED1_MASK) >> HIPWM_HMIC8_RESERVED1_SHIFT)
#define  HIPWM_HMIC8_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC8_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC8_RESERVED1_SHIFT))

#define  HIPWM_HMIC8_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC8_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC8_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC8_MICINTPIX_MASK) >> HIPWM_HMIC8_MICINTPIX_SHIFT)
#define  HIPWM_HMIC8_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC8_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC8_MICINTPIX_SHIFT))

#define  HIPWM_HMIC8_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC8_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC8_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC8_MICFRACPIX_MASK) >> HIPWM_HMIC8_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC8_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC8_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC8_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 10 of 12) (HMIC9)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 10 of 12)*/
//====================================================================

#define  HIPWM_HMIC9_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC9_RESERVED1_SHIFT 12
#define  HIPWM_HMIC9_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC9_RESERVED1_MASK) >> HIPWM_HMIC9_RESERVED1_SHIFT)
#define  HIPWM_HMIC9_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC9_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC9_RESERVED1_SHIFT))

#define  HIPWM_HMIC9_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC9_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC9_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC9_MICINTPIX_MASK) >> HIPWM_HMIC9_MICINTPIX_SHIFT)
#define  HIPWM_HMIC9_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC9_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC9_MICINTPIX_SHIFT))

#define  HIPWM_HMIC9_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC9_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC9_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC9_MICFRACPIX_MASK) >> HIPWM_HMIC9_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC9_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC9_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC9_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 11 of 12) (HMIC10)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 11 of 12)*/
//====================================================================

#define  HIPWM_HMIC10_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC10_RESERVED1_SHIFT 12
#define  HIPWM_HMIC10_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC10_RESERVED1_MASK) >> HIPWM_HMIC10_RESERVED1_SHIFT)
#define  HIPWM_HMIC10_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC10_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC10_RESERVED1_SHIFT))

#define  HIPWM_HMIC10_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC10_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC10_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC10_MICINTPIX_MASK) >> HIPWM_HMIC10_MICINTPIX_SHIFT)
#define  HIPWM_HMIC10_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC10_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC10_MICINTPIX_SHIFT))

#define  HIPWM_HMIC10_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC10_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC10_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC10_MICFRACPIX_MASK) >> HIPWM_HMIC10_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC10_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC10_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC10_MICFRACPIX_SHIFT))

//====================================================================
//Register: HiPWM Mirror Correction Margin Registers (Instance 12 of 12) (HMIC11)
/** \brief The HiPWM Mirror Correction Margin Registers are provided to allow for periodic line based image transformations.
        The number of the HMIC registers used is controlled by the numFacets field in the HVSync Configuration Register.
        For example if the numFacets field is set to 3, which indicates a 4 facet mirror, 4 HMIC registers will be used by
        the hardware.
        <br>
        If any sub-pixel or fractional margin is required, it's value is programmed into these registers. Again for the 
        case of a 4 facet mirror 4 HMIC registers will be used by the hardware. Even if the fractional margin is the same
        for every line, all 4 HMIC registers must be programmed with the desired sub-pixel margin. The 6-bit fraction allows
        for 1/64 pixel control if the PTwidth field in the HiPWM Configuration Register is set to 64 bits (a value of 0). If
        the PTwidth field is set to 32 bits (a value of 1) the least significant bit is ignored which allows for 1/32 pixel
        control.
        <br>
        The value in these registers is used in concert with the value in the Side Margins register. Due to the nature of
        the hardware, this register is subtracted from the value in the Side Margins Register.
        <br>
        Example: The Side Margins register has a value of 100 (integer pixels) and the HMiC Register has a value of 0x90.
        0x90 indicates an integer value of 2 and a fractional value of 24 which converts to 0.375 (24/2^6). Therefore the
        side margin would equal 100 - 2.375 = 97.625 pixels. (Instance 12 of 12)*/
//====================================================================

#define  HIPWM_HMIC11_RESERVED1_MASK 0xfffff000
#define  HIPWM_HMIC11_RESERVED1_SHIFT 12
#define  HIPWM_HMIC11_RESERVED1_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC11_RESERVED1_MASK) >> HIPWM_HMIC11_RESERVED1_SHIFT)
#define  HIPWM_HMIC11_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC11_RESERVED1_MASK) | (((uint32_t)val) << HIPWM_HMIC11_RESERVED1_SHIFT))

#define  HIPWM_HMIC11_MICINTPIX_MASK 0xfc0
#define  HIPWM_HMIC11_MICINTPIX_SHIFT 6
#define  HIPWM_HMIC11_MICINTPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC11_MICINTPIX_MASK) >> HIPWM_HMIC11_MICINTPIX_SHIFT)
#define  HIPWM_HMIC11_MICINTPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC11_MICINTPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC11_MICINTPIX_SHIFT))

#define  HIPWM_HMIC11_MICFRACPIX_MASK 0x3f
#define  HIPWM_HMIC11_MICFRACPIX_SHIFT 0
#define  HIPWM_HMIC11_MICFRACPIX_MASK_SHIFT(reg) (((reg) & HIPWM_HMIC11_MICFRACPIX_MASK) >> HIPWM_HMIC11_MICFRACPIX_SHIFT)
#define  HIPWM_HMIC11_MICFRACPIX_REPLACE_VAL(reg,val) (((reg) & ~HIPWM_HMIC11_MICFRACPIX_MASK) | (((uint32_t)val) << HIPWM_HMIC11_MICFRACPIX_SHIFT))

#endif // HIPWM
