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
 * \file TH_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _TH_REGMASKS_H_
#define _TH_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Thresholding Registers and SRAMs (TH)
/** \brief Register and SRAM descriptions for the Thresholding block*/
//
//====================================================================

//====================================================================
//
//Register File: CORE (CORE)
/** \brief Register descriptions for the Thresholding block*/
//
//====================================================================

//====================================================================
//Register: TH Configuration Register (TCR)
//====================================================================

#define  CORE_TCR_RESERVED1_MASK 0xfffc0000
#define  CORE_TCR_RESERVED1_SHIFT 18
#define  CORE_TCR_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TCR_RESERVED1_MASK) >> CORE_TCR_RESERVED1_SHIFT)
#define  CORE_TCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_RESERVED1_MASK) | (((uint32_t)val) << CORE_TCR_RESERVED1_SHIFT))

#define  CORE_TCR_DIRECTTHRESH_MASK 0x20000
#define  CORE_TCR_DIRECTTHRESH_SHIFT 17
#define  CORE_TCR_DIRECTTHRESH_MASK_SHIFT(reg) (((reg) & CORE_TCR_DIRECTTHRESH_MASK) >> CORE_TCR_DIRECTTHRESH_SHIFT)
#define  CORE_TCR_DIRECTTHRESH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_DIRECTTHRESH_MASK) | (((uint32_t)val) << CORE_TCR_DIRECTTHRESH_SHIFT))

#define  CORE_TCR_ALTLUTDBL_MASK 0x10000
#define  CORE_TCR_ALTLUTDBL_SHIFT 16
#define  CORE_TCR_ALTLUTDBL_MASK_SHIFT(reg) (((reg) & CORE_TCR_ALTLUTDBL_MASK) >> CORE_TCR_ALTLUTDBL_SHIFT)
#define  CORE_TCR_ALTLUTDBL_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_ALTLUTDBL_MASK) | (((uint32_t)val) << CORE_TCR_ALTLUTDBL_SHIFT))

#define  CORE_TCR_BYPASS_MASK 0x8000
#define  CORE_TCR_BYPASS_SHIFT 15
#define  CORE_TCR_BYPASS_MASK_SHIFT(reg) (((reg) & CORE_TCR_BYPASS_MASK) >> CORE_TCR_BYPASS_SHIFT)
#define  CORE_TCR_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_BYPASS_MASK) | (((uint32_t)val) << CORE_TCR_BYPASS_SHIFT))

#define  CORE_TCR_EDGEOVERRIDE_MASK 0x7000
#define  CORE_TCR_EDGEOVERRIDE_SHIFT 12
#define  CORE_TCR_EDGEOVERRIDE_MASK_SHIFT(reg) (((reg) & CORE_TCR_EDGEOVERRIDE_MASK) >> CORE_TCR_EDGEOVERRIDE_SHIFT)
#define  CORE_TCR_EDGEOVERRIDE_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_EDGEOVERRIDE_MASK) | (((uint32_t)val) << CORE_TCR_EDGEOVERRIDE_SHIFT))

#define  CORE_TCR_CPUACCESS_MASK 0x800
#define  CORE_TCR_CPUACCESS_SHIFT 11
#define  CORE_TCR_CPUACCESS_MASK_SHIFT(reg) (((reg) & CORE_TCR_CPUACCESS_MASK) >> CORE_TCR_CPUACCESS_SHIFT)
#define  CORE_TCR_CPUACCESS_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_CPUACCESS_MASK) | (((uint32_t)val) << CORE_TCR_CPUACCESS_SHIFT))

#define  CORE_TCR_USEOBJTYPE_MASK 0x400
#define  CORE_TCR_USEOBJTYPE_SHIFT 10
#define  CORE_TCR_USEOBJTYPE_MASK_SHIFT(reg) (((reg) & CORE_TCR_USEOBJTYPE_MASK) >> CORE_TCR_USEOBJTYPE_SHIFT)
#define  CORE_TCR_USEOBJTYPE_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_USEOBJTYPE_MASK) | (((uint32_t)val) << CORE_TCR_USEOBJTYPE_SHIFT))

#define  CORE_TCR_CFGCONCAT_MASK 0x380
#define  CORE_TCR_CFGCONCAT_SHIFT 7
#define  CORE_TCR_CFGCONCAT_MASK_SHIFT(reg) (((reg) & CORE_TCR_CFGCONCAT_MASK) >> CORE_TCR_CFGCONCAT_SHIFT)
#define  CORE_TCR_CFGCONCAT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_CFGCONCAT_MASK) | (((uint32_t)val) << CORE_TCR_CFGCONCAT_SHIFT))

#define  CORE_TCR_ALTLUTBPPCONV_MASK 0x60
#define  CORE_TCR_ALTLUTBPPCONV_SHIFT 5
#define  CORE_TCR_ALTLUTBPPCONV_MASK_SHIFT(reg) (((reg) & CORE_TCR_ALTLUTBPPCONV_MASK) >> CORE_TCR_ALTLUTBPPCONV_SHIFT)
#define  CORE_TCR_ALTLUTBPPCONV_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_ALTLUTBPPCONV_MASK) | (((uint32_t)val) << CORE_TCR_ALTLUTBPPCONV_SHIFT))

#define  CORE_TCR_OUTPUTJUSTCONCAT_MASK 0x18
#define  CORE_TCR_OUTPUTJUSTCONCAT_SHIFT 3
#define  CORE_TCR_OUTPUTJUSTCONCAT_MASK_SHIFT(reg) (((reg) & CORE_TCR_OUTPUTJUSTCONCAT_MASK) >> CORE_TCR_OUTPUTJUSTCONCAT_SHIFT)
#define  CORE_TCR_OUTPUTJUSTCONCAT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_OUTPUTJUSTCONCAT_MASK) | (((uint32_t)val) << CORE_TCR_OUTPUTJUSTCONCAT_SHIFT))

#define  CORE_TCR_PWMUXBPPCONV_MASK 0x6
#define  CORE_TCR_PWMUXBPPCONV_SHIFT 1
#define  CORE_TCR_PWMUXBPPCONV_MASK_SHIFT(reg) (((reg) & CORE_TCR_PWMUXBPPCONV_MASK) >> CORE_TCR_PWMUXBPPCONV_SHIFT)
#define  CORE_TCR_PWMUXBPPCONV_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_PWMUXBPPCONV_MASK) | (((uint32_t)val) << CORE_TCR_PWMUXBPPCONV_SHIFT))

#define  CORE_TCR_ENABLEPARTIALHALFTONE_MASK 0x1
#define  CORE_TCR_ENABLEPARTIALHALFTONE_SHIFT 0
#define  CORE_TCR_ENABLEPARTIALHALFTONE_MASK_SHIFT(reg) (((reg) & CORE_TCR_ENABLEPARTIALHALFTONE_MASK) >> CORE_TCR_ENABLEPARTIALHALFTONE_SHIFT)
#define  CORE_TCR_ENABLEPARTIALHALFTONE_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCR_ENABLEPARTIALHALFTONE_MASK) | (((uint32_t)val) << CORE_TCR_ENABLEPARTIALHALFTONE_SHIFT))

//====================================================================
//Register: TH Micro Cell Register (Instance 1 of 2) (TMCR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  CORE_TMCR0_RESERVED1_MASK 0xff000000
#define  CORE_TMCR0_RESERVED1_SHIFT 24
#define  CORE_TMCR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TMCR0_RESERVED1_MASK) >> CORE_TMCR0_RESERVED1_SHIFT)
#define  CORE_TMCR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR0_RESERVED1_MASK) | (((uint32_t)val) << CORE_TMCR0_RESERVED1_SHIFT))

#define  CORE_TMCR0_CELLOFFSET_MASK 0xff0000
#define  CORE_TMCR0_CELLOFFSET_SHIFT 16
#define  CORE_TMCR0_CELLOFFSET_MASK_SHIFT(reg) (((reg) & CORE_TMCR0_CELLOFFSET_MASK) >> CORE_TMCR0_CELLOFFSET_SHIFT)
#define  CORE_TMCR0_CELLOFFSET_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR0_CELLOFFSET_MASK) | (((uint32_t)val) << CORE_TMCR0_CELLOFFSET_SHIFT))

#define  CORE_TMCR0_CELLWIDTH_MASK 0xff00
#define  CORE_TMCR0_CELLWIDTH_SHIFT 8
#define  CORE_TMCR0_CELLWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TMCR0_CELLWIDTH_MASK) >> CORE_TMCR0_CELLWIDTH_SHIFT)
#define  CORE_TMCR0_CELLWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR0_CELLWIDTH_MASK) | (((uint32_t)val) << CORE_TMCR0_CELLWIDTH_SHIFT))

#define  CORE_TMCR0_RESERVED2_MASK 0xf0
#define  CORE_TMCR0_RESERVED2_SHIFT 4
#define  CORE_TMCR0_RESERVED2_MASK_SHIFT(reg) (((reg) & CORE_TMCR0_RESERVED2_MASK) >> CORE_TMCR0_RESERVED2_SHIFT)
#define  CORE_TMCR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR0_RESERVED2_MASK) | (((uint32_t)val) << CORE_TMCR0_RESERVED2_SHIFT))

#define  CORE_TMCR0_CELLHEIGHT_MASK 0xf
#define  CORE_TMCR0_CELLHEIGHT_SHIFT 0
#define  CORE_TMCR0_CELLHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TMCR0_CELLHEIGHT_MASK) >> CORE_TMCR0_CELLHEIGHT_SHIFT)
#define  CORE_TMCR0_CELLHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR0_CELLHEIGHT_MASK) | (((uint32_t)val) << CORE_TMCR0_CELLHEIGHT_SHIFT))

//====================================================================
//Register: TH Micro Cell Register (Instance 2 of 2) (TMCR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  CORE_TMCR1_RESERVED1_MASK 0xff000000
#define  CORE_TMCR1_RESERVED1_SHIFT 24
#define  CORE_TMCR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TMCR1_RESERVED1_MASK) >> CORE_TMCR1_RESERVED1_SHIFT)
#define  CORE_TMCR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR1_RESERVED1_MASK) | (((uint32_t)val) << CORE_TMCR1_RESERVED1_SHIFT))

#define  CORE_TMCR1_CELLOFFSET_MASK 0xff0000
#define  CORE_TMCR1_CELLOFFSET_SHIFT 16
#define  CORE_TMCR1_CELLOFFSET_MASK_SHIFT(reg) (((reg) & CORE_TMCR1_CELLOFFSET_MASK) >> CORE_TMCR1_CELLOFFSET_SHIFT)
#define  CORE_TMCR1_CELLOFFSET_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR1_CELLOFFSET_MASK) | (((uint32_t)val) << CORE_TMCR1_CELLOFFSET_SHIFT))

#define  CORE_TMCR1_CELLWIDTH_MASK 0xff00
#define  CORE_TMCR1_CELLWIDTH_SHIFT 8
#define  CORE_TMCR1_CELLWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TMCR1_CELLWIDTH_MASK) >> CORE_TMCR1_CELLWIDTH_SHIFT)
#define  CORE_TMCR1_CELLWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR1_CELLWIDTH_MASK) | (((uint32_t)val) << CORE_TMCR1_CELLWIDTH_SHIFT))

#define  CORE_TMCR1_RESERVED2_MASK 0xf0
#define  CORE_TMCR1_RESERVED2_SHIFT 4
#define  CORE_TMCR1_RESERVED2_MASK_SHIFT(reg) (((reg) & CORE_TMCR1_RESERVED2_MASK) >> CORE_TMCR1_RESERVED2_SHIFT)
#define  CORE_TMCR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR1_RESERVED2_MASK) | (((uint32_t)val) << CORE_TMCR1_RESERVED2_SHIFT))

#define  CORE_TMCR1_CELLHEIGHT_MASK 0xf
#define  CORE_TMCR1_CELLHEIGHT_SHIFT 0
#define  CORE_TMCR1_CELLHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TMCR1_CELLHEIGHT_MASK) >> CORE_TMCR1_CELLHEIGHT_SHIFT)
#define  CORE_TMCR1_CELLHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TMCR1_CELLHEIGHT_MASK) | (((uint32_t)val) << CORE_TMCR1_CELLHEIGHT_SHIFT))

//====================================================================
//Register: TH Cell Start Register (Instance 1 of 2) (TCSR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  CORE_TCSR0_RESERVED1_MASK 0xffff0000
#define  CORE_TCSR0_RESERVED1_SHIFT 16
#define  CORE_TCSR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TCSR0_RESERVED1_MASK) >> CORE_TCSR0_RESERVED1_SHIFT)
#define  CORE_TCSR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR0_RESERVED1_MASK) | (((uint32_t)val) << CORE_TCSR0_RESERVED1_SHIFT))

#define  CORE_TCSR0_STARTWIDTH_MASK 0xff00
#define  CORE_TCSR0_STARTWIDTH_SHIFT 8
#define  CORE_TCSR0_STARTWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TCSR0_STARTWIDTH_MASK) >> CORE_TCSR0_STARTWIDTH_SHIFT)
#define  CORE_TCSR0_STARTWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR0_STARTWIDTH_MASK) | (((uint32_t)val) << CORE_TCSR0_STARTWIDTH_SHIFT))

#define  CORE_TCSR0_RESERVED2_MASK 0xf0
#define  CORE_TCSR0_RESERVED2_SHIFT 4
#define  CORE_TCSR0_RESERVED2_MASK_SHIFT(reg) (((reg) & CORE_TCSR0_RESERVED2_MASK) >> CORE_TCSR0_RESERVED2_SHIFT)
#define  CORE_TCSR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR0_RESERVED2_MASK) | (((uint32_t)val) << CORE_TCSR0_RESERVED2_SHIFT))

#define  CORE_TCSR0_STARTHEIGHT_MASK 0xf
#define  CORE_TCSR0_STARTHEIGHT_SHIFT 0
#define  CORE_TCSR0_STARTHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TCSR0_STARTHEIGHT_MASK) >> CORE_TCSR0_STARTHEIGHT_SHIFT)
#define  CORE_TCSR0_STARTHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR0_STARTHEIGHT_MASK) | (((uint32_t)val) << CORE_TCSR0_STARTHEIGHT_SHIFT))

//====================================================================
//Register: TH Cell Start Register (Instance 2 of 2) (TCSR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  CORE_TCSR1_RESERVED1_MASK 0xffff0000
#define  CORE_TCSR1_RESERVED1_SHIFT 16
#define  CORE_TCSR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TCSR1_RESERVED1_MASK) >> CORE_TCSR1_RESERVED1_SHIFT)
#define  CORE_TCSR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR1_RESERVED1_MASK) | (((uint32_t)val) << CORE_TCSR1_RESERVED1_SHIFT))

#define  CORE_TCSR1_STARTWIDTH_MASK 0xff00
#define  CORE_TCSR1_STARTWIDTH_SHIFT 8
#define  CORE_TCSR1_STARTWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TCSR1_STARTWIDTH_MASK) >> CORE_TCSR1_STARTWIDTH_SHIFT)
#define  CORE_TCSR1_STARTWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR1_STARTWIDTH_MASK) | (((uint32_t)val) << CORE_TCSR1_STARTWIDTH_SHIFT))

#define  CORE_TCSR1_RESERVED2_MASK 0xf0
#define  CORE_TCSR1_RESERVED2_SHIFT 4
#define  CORE_TCSR1_RESERVED2_MASK_SHIFT(reg) (((reg) & CORE_TCSR1_RESERVED2_MASK) >> CORE_TCSR1_RESERVED2_SHIFT)
#define  CORE_TCSR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR1_RESERVED2_MASK) | (((uint32_t)val) << CORE_TCSR1_RESERVED2_SHIFT))

#define  CORE_TCSR1_STARTHEIGHT_MASK 0xf
#define  CORE_TCSR1_STARTHEIGHT_SHIFT 0
#define  CORE_TCSR1_STARTHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TCSR1_STARTHEIGHT_MASK) >> CORE_TCSR1_STARTHEIGHT_SHIFT)
#define  CORE_TCSR1_STARTHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCSR1_STARTHEIGHT_MASK) | (((uint32_t)val) << CORE_TCSR1_STARTHEIGHT_SHIFT))

//====================================================================
//Register: TH Stochastic Size Register (Instance 1 of 2) (TSSR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  CORE_TSSR0_RESERVED1_MASK 0xffff0000
#define  CORE_TSSR0_RESERVED1_SHIFT 16
#define  CORE_TSSR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TSSR0_RESERVED1_MASK) >> CORE_TSSR0_RESERVED1_SHIFT)
#define  CORE_TSSR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSSR0_RESERVED1_MASK) | (((uint32_t)val) << CORE_TSSR0_RESERVED1_SHIFT))

#define  CORE_TSSR0_STOCWIDTH_MASK 0xff00
#define  CORE_TSSR0_STOCWIDTH_SHIFT 8
#define  CORE_TSSR0_STOCWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TSSR0_STOCWIDTH_MASK) >> CORE_TSSR0_STOCWIDTH_SHIFT)
#define  CORE_TSSR0_STOCWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSSR0_STOCWIDTH_MASK) | (((uint32_t)val) << CORE_TSSR0_STOCWIDTH_SHIFT))

#define  CORE_TSSR0_STOCHEIGHT_MASK 0xff
#define  CORE_TSSR0_STOCHEIGHT_SHIFT 0
#define  CORE_TSSR0_STOCHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TSSR0_STOCHEIGHT_MASK) >> CORE_TSSR0_STOCHEIGHT_SHIFT)
#define  CORE_TSSR0_STOCHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSSR0_STOCHEIGHT_MASK) | (((uint32_t)val) << CORE_TSSR0_STOCHEIGHT_SHIFT))

//====================================================================
//Register: TH Stochastic Size Register (Instance 2 of 2) (TSSR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  CORE_TSSR1_RESERVED1_MASK 0xffff0000
#define  CORE_TSSR1_RESERVED1_SHIFT 16
#define  CORE_TSSR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TSSR1_RESERVED1_MASK) >> CORE_TSSR1_RESERVED1_SHIFT)
#define  CORE_TSSR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSSR1_RESERVED1_MASK) | (((uint32_t)val) << CORE_TSSR1_RESERVED1_SHIFT))

#define  CORE_TSSR1_STOCWIDTH_MASK 0xff00
#define  CORE_TSSR1_STOCWIDTH_SHIFT 8
#define  CORE_TSSR1_STOCWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TSSR1_STOCWIDTH_MASK) >> CORE_TSSR1_STOCWIDTH_SHIFT)
#define  CORE_TSSR1_STOCWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSSR1_STOCWIDTH_MASK) | (((uint32_t)val) << CORE_TSSR1_STOCWIDTH_SHIFT))

#define  CORE_TSSR1_STOCHEIGHT_MASK 0xff
#define  CORE_TSSR1_STOCHEIGHT_SHIFT 0
#define  CORE_TSSR1_STOCHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TSSR1_STOCHEIGHT_MASK) >> CORE_TSSR1_STOCHEIGHT_SHIFT)
#define  CORE_TSSR1_STOCHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSSR1_STOCHEIGHT_MASK) | (((uint32_t)val) << CORE_TSSR1_STOCHEIGHT_SHIFT))

//====================================================================
//Register: TH Thresholding Register (Instance 1 of 2) (TTR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  CORE_TTR0_THRESHHI_MASK 0xffff0000
#define  CORE_TTR0_THRESHHI_SHIFT 16
#define  CORE_TTR0_THRESHHI_MASK_SHIFT(reg) (((reg) & CORE_TTR0_THRESHHI_MASK) >> CORE_TTR0_THRESHHI_SHIFT)
#define  CORE_TTR0_THRESHHI_REPLACE_VAL(reg,val) (((reg) & ~CORE_TTR0_THRESHHI_MASK) | (((uint32_t)val) << CORE_TTR0_THRESHHI_SHIFT))

#define  CORE_TTR0_THRESHLO_MASK 0xffff
#define  CORE_TTR0_THRESHLO_SHIFT 0
#define  CORE_TTR0_THRESHLO_MASK_SHIFT(reg) (((reg) & CORE_TTR0_THRESHLO_MASK) >> CORE_TTR0_THRESHLO_SHIFT)
#define  CORE_TTR0_THRESHLO_REPLACE_VAL(reg,val) (((reg) & ~CORE_TTR0_THRESHLO_MASK) | (((uint32_t)val) << CORE_TTR0_THRESHLO_SHIFT))

//====================================================================
//Register: TH Thresholding Register (Instance 2 of 2) (TTR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  CORE_TTR1_THRESHHI_MASK 0xffff0000
#define  CORE_TTR1_THRESHHI_SHIFT 16
#define  CORE_TTR1_THRESHHI_MASK_SHIFT(reg) (((reg) & CORE_TTR1_THRESHHI_MASK) >> CORE_TTR1_THRESHHI_SHIFT)
#define  CORE_TTR1_THRESHHI_REPLACE_VAL(reg,val) (((reg) & ~CORE_TTR1_THRESHHI_MASK) | (((uint32_t)val) << CORE_TTR1_THRESHHI_SHIFT))

#define  CORE_TTR1_THRESHLO_MASK 0xffff
#define  CORE_TTR1_THRESHLO_SHIFT 0
#define  CORE_TTR1_THRESHLO_MASK_SHIFT(reg) (((reg) & CORE_TTR1_THRESHLO_MASK) >> CORE_TTR1_THRESHLO_SHIFT)
#define  CORE_TTR1_THRESHLO_REPLACE_VAL(reg,val) (((reg) & ~CORE_TTR1_THRESHLO_MASK) | (((uint32_t)val) << CORE_TTR1_THRESHLO_SHIFT))

//====================================================================
//Register: TH Thresholding Edge Register (TTER)
//====================================================================

#define  CORE_TTER_RESERVED1_MASK 0xffffff00
#define  CORE_TTER_RESERVED1_SHIFT 8
#define  CORE_TTER_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TTER_RESERVED1_MASK) >> CORE_TTER_RESERVED1_SHIFT)
#define  CORE_TTER_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TTER_RESERVED1_MASK) | (((uint32_t)val) << CORE_TTER_RESERVED1_SHIFT))

#define  CORE_TTER_THRESHEDGE_MASK 0xf0
#define  CORE_TTER_THRESHEDGE_SHIFT 4
#define  CORE_TTER_THRESHEDGE_MASK_SHIFT(reg) (((reg) & CORE_TTER_THRESHEDGE_MASK) >> CORE_TTER_THRESHEDGE_SHIFT)
#define  CORE_TTER_THRESHEDGE_REPLACE_VAL(reg,val) (((reg) & ~CORE_TTER_THRESHEDGE_MASK) | (((uint32_t)val) << CORE_TTER_THRESHEDGE_SHIFT))

#define  CORE_TTER_THRESHPW_MASK 0xf
#define  CORE_TTER_THRESHPW_SHIFT 0
#define  CORE_TTER_THRESHPW_MASK_SHIFT(reg) (((reg) & CORE_TTER_THRESHPW_MASK) >> CORE_TTER_THRESHPW_SHIFT)
#define  CORE_TTER_THRESHPW_REPLACE_VAL(reg,val) (((reg) & ~CORE_TTER_THRESHPW_MASK) | (((uint32_t)val) << CORE_TTER_THRESHPW_SHIFT))

//====================================================================
//Register: TH PosGen Save/Restore Register (Instance 1 of 2) (TPGR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  CORE_TPGR0_RESERVED1_MASK 0xff000000
#define  CORE_TPGR0_RESERVED1_SHIFT 24
#define  CORE_TPGR0_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TPGR0_RESERVED1_MASK) >> CORE_TPGR0_RESERVED1_SHIFT)
#define  CORE_TPGR0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR0_RESERVED1_MASK) | (((uint32_t)val) << CORE_TPGR0_RESERVED1_SHIFT))

#define  CORE_TPGR0_POSGENOFFSET_MASK 0xff0000
#define  CORE_TPGR0_POSGENOFFSET_SHIFT 16
#define  CORE_TPGR0_POSGENOFFSET_MASK_SHIFT(reg) (((reg) & CORE_TPGR0_POSGENOFFSET_MASK) >> CORE_TPGR0_POSGENOFFSET_SHIFT)
#define  CORE_TPGR0_POSGENOFFSET_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR0_POSGENOFFSET_MASK) | (((uint32_t)val) << CORE_TPGR0_POSGENOFFSET_SHIFT))

#define  CORE_TPGR0_POSGENWIDTH_MASK 0xff00
#define  CORE_TPGR0_POSGENWIDTH_SHIFT 8
#define  CORE_TPGR0_POSGENWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TPGR0_POSGENWIDTH_MASK) >> CORE_TPGR0_POSGENWIDTH_SHIFT)
#define  CORE_TPGR0_POSGENWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR0_POSGENWIDTH_MASK) | (((uint32_t)val) << CORE_TPGR0_POSGENWIDTH_SHIFT))

#define  CORE_TPGR0_RESERVED2_MASK 0xf0
#define  CORE_TPGR0_RESERVED2_SHIFT 4
#define  CORE_TPGR0_RESERVED2_MASK_SHIFT(reg) (((reg) & CORE_TPGR0_RESERVED2_MASK) >> CORE_TPGR0_RESERVED2_SHIFT)
#define  CORE_TPGR0_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR0_RESERVED2_MASK) | (((uint32_t)val) << CORE_TPGR0_RESERVED2_SHIFT))

#define  CORE_TPGR0_POSGENHEIGHT_MASK 0xf
#define  CORE_TPGR0_POSGENHEIGHT_SHIFT 0
#define  CORE_TPGR0_POSGENHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TPGR0_POSGENHEIGHT_MASK) >> CORE_TPGR0_POSGENHEIGHT_SHIFT)
#define  CORE_TPGR0_POSGENHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR0_POSGENHEIGHT_MASK) | (((uint32_t)val) << CORE_TPGR0_POSGENHEIGHT_SHIFT))

//====================================================================
//Register: TH PosGen Save/Restore Register (Instance 2 of 2) (TPGR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  CORE_TPGR1_RESERVED1_MASK 0xff000000
#define  CORE_TPGR1_RESERVED1_SHIFT 24
#define  CORE_TPGR1_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TPGR1_RESERVED1_MASK) >> CORE_TPGR1_RESERVED1_SHIFT)
#define  CORE_TPGR1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR1_RESERVED1_MASK) | (((uint32_t)val) << CORE_TPGR1_RESERVED1_SHIFT))

#define  CORE_TPGR1_POSGENOFFSET_MASK 0xff0000
#define  CORE_TPGR1_POSGENOFFSET_SHIFT 16
#define  CORE_TPGR1_POSGENOFFSET_MASK_SHIFT(reg) (((reg) & CORE_TPGR1_POSGENOFFSET_MASK) >> CORE_TPGR1_POSGENOFFSET_SHIFT)
#define  CORE_TPGR1_POSGENOFFSET_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR1_POSGENOFFSET_MASK) | (((uint32_t)val) << CORE_TPGR1_POSGENOFFSET_SHIFT))

#define  CORE_TPGR1_POSGENWIDTH_MASK 0xff00
#define  CORE_TPGR1_POSGENWIDTH_SHIFT 8
#define  CORE_TPGR1_POSGENWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TPGR1_POSGENWIDTH_MASK) >> CORE_TPGR1_POSGENWIDTH_SHIFT)
#define  CORE_TPGR1_POSGENWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR1_POSGENWIDTH_MASK) | (((uint32_t)val) << CORE_TPGR1_POSGENWIDTH_SHIFT))

#define  CORE_TPGR1_RESERVED2_MASK 0xf0
#define  CORE_TPGR1_RESERVED2_SHIFT 4
#define  CORE_TPGR1_RESERVED2_MASK_SHIFT(reg) (((reg) & CORE_TPGR1_RESERVED2_MASK) >> CORE_TPGR1_RESERVED2_SHIFT)
#define  CORE_TPGR1_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR1_RESERVED2_MASK) | (((uint32_t)val) << CORE_TPGR1_RESERVED2_SHIFT))

#define  CORE_TPGR1_POSGENHEIGHT_MASK 0xf
#define  CORE_TPGR1_POSGENHEIGHT_SHIFT 0
#define  CORE_TPGR1_POSGENHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TPGR1_POSGENHEIGHT_MASK) >> CORE_TPGR1_POSGENHEIGHT_SHIFT)
#define  CORE_TPGR1_POSGENHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TPGR1_POSGENHEIGHT_MASK) | (((uint32_t)val) << CORE_TPGR1_POSGENHEIGHT_SHIFT))

//====================================================================
//Register: TH CellGen Save/Restore Register (Instance 1 of 2) (TCGR0)
/** \brief  (Instance 1 of 2)*/
//====================================================================

#define  CORE_TCGR0_CELLGENVERTWIDTH_MASK 0xff000000
#define  CORE_TCGR0_CELLGENVERTWIDTH_SHIFT 24
#define  CORE_TCGR0_CELLGENVERTWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TCGR0_CELLGENVERTWIDTH_MASK) >> CORE_TCGR0_CELLGENVERTWIDTH_SHIFT)
#define  CORE_TCGR0_CELLGENVERTWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR0_CELLGENVERTWIDTH_MASK) | (((uint32_t)val) << CORE_TCGR0_CELLGENVERTWIDTH_SHIFT))

#define  CORE_TCGR0_CELLGENVERTHEIGHT_MASK 0xff0000
#define  CORE_TCGR0_CELLGENVERTHEIGHT_SHIFT 16
#define  CORE_TCGR0_CELLGENVERTHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TCGR0_CELLGENVERTHEIGHT_MASK) >> CORE_TCGR0_CELLGENVERTHEIGHT_SHIFT)
#define  CORE_TCGR0_CELLGENVERTHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR0_CELLGENVERTHEIGHT_MASK) | (((uint32_t)val) << CORE_TCGR0_CELLGENVERTHEIGHT_SHIFT))

#define  CORE_TCGR0_CELLGENWIDTH_MASK 0xff00
#define  CORE_TCGR0_CELLGENWIDTH_SHIFT 8
#define  CORE_TCGR0_CELLGENWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TCGR0_CELLGENWIDTH_MASK) >> CORE_TCGR0_CELLGENWIDTH_SHIFT)
#define  CORE_TCGR0_CELLGENWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR0_CELLGENWIDTH_MASK) | (((uint32_t)val) << CORE_TCGR0_CELLGENWIDTH_SHIFT))

#define  CORE_TCGR0_CELLGENHEIGHT_MASK 0xff
#define  CORE_TCGR0_CELLGENHEIGHT_SHIFT 0
#define  CORE_TCGR0_CELLGENHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TCGR0_CELLGENHEIGHT_MASK) >> CORE_TCGR0_CELLGENHEIGHT_SHIFT)
#define  CORE_TCGR0_CELLGENHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR0_CELLGENHEIGHT_MASK) | (((uint32_t)val) << CORE_TCGR0_CELLGENHEIGHT_SHIFT))

//====================================================================
//Register: TH CellGen Save/Restore Register (Instance 2 of 2) (TCGR1)
/** \brief  (Instance 2 of 2)*/
//====================================================================

#define  CORE_TCGR1_CELLGENVERTWIDTH_MASK 0xff000000
#define  CORE_TCGR1_CELLGENVERTWIDTH_SHIFT 24
#define  CORE_TCGR1_CELLGENVERTWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TCGR1_CELLGENVERTWIDTH_MASK) >> CORE_TCGR1_CELLGENVERTWIDTH_SHIFT)
#define  CORE_TCGR1_CELLGENVERTWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR1_CELLGENVERTWIDTH_MASK) | (((uint32_t)val) << CORE_TCGR1_CELLGENVERTWIDTH_SHIFT))

#define  CORE_TCGR1_CELLGENVERTHEIGHT_MASK 0xff0000
#define  CORE_TCGR1_CELLGENVERTHEIGHT_SHIFT 16
#define  CORE_TCGR1_CELLGENVERTHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TCGR1_CELLGENVERTHEIGHT_MASK) >> CORE_TCGR1_CELLGENVERTHEIGHT_SHIFT)
#define  CORE_TCGR1_CELLGENVERTHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR1_CELLGENVERTHEIGHT_MASK) | (((uint32_t)val) << CORE_TCGR1_CELLGENVERTHEIGHT_SHIFT))

#define  CORE_TCGR1_CELLGENWIDTH_MASK 0xff00
#define  CORE_TCGR1_CELLGENWIDTH_SHIFT 8
#define  CORE_TCGR1_CELLGENWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TCGR1_CELLGENWIDTH_MASK) >> CORE_TCGR1_CELLGENWIDTH_SHIFT)
#define  CORE_TCGR1_CELLGENWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR1_CELLGENWIDTH_MASK) | (((uint32_t)val) << CORE_TCGR1_CELLGENWIDTH_SHIFT))

#define  CORE_TCGR1_CELLGENHEIGHT_MASK 0xff
#define  CORE_TCGR1_CELLGENHEIGHT_SHIFT 0
#define  CORE_TCGR1_CELLGENHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TCGR1_CELLGENHEIGHT_MASK) >> CORE_TCGR1_CELLGENHEIGHT_SHIFT)
#define  CORE_TCGR1_CELLGENHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TCGR1_CELLGENHEIGHT_MASK) | (((uint32_t)val) << CORE_TCGR1_CELLGENHEIGHT_SHIFT))

//====================================================================
//Register: TH AltLut Save/Restore Register (TALR)
//====================================================================

#define  CORE_TALR_RESERVED1_MASK 0xfffff800
#define  CORE_TALR_RESERVED1_SHIFT 11
#define  CORE_TALR_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TALR_RESERVED1_MASK) >> CORE_TALR_RESERVED1_SHIFT)
#define  CORE_TALR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TALR_RESERVED1_MASK) | (((uint32_t)val) << CORE_TALR_RESERVED1_SHIFT))

#define  CORE_TALR_ALTLUTWIDTH_MASK 0x700
#define  CORE_TALR_ALTLUTWIDTH_SHIFT 8
#define  CORE_TALR_ALTLUTWIDTH_MASK_SHIFT(reg) (((reg) & CORE_TALR_ALTLUTWIDTH_MASK) >> CORE_TALR_ALTLUTWIDTH_SHIFT)
#define  CORE_TALR_ALTLUTWIDTH_REPLACE_VAL(reg,val) (((reg) & ~CORE_TALR_ALTLUTWIDTH_MASK) | (((uint32_t)val) << CORE_TALR_ALTLUTWIDTH_SHIFT))

#define  CORE_TALR_RESERVED2_MASK 0xf8
#define  CORE_TALR_RESERVED2_SHIFT 3
#define  CORE_TALR_RESERVED2_MASK_SHIFT(reg) (((reg) & CORE_TALR_RESERVED2_MASK) >> CORE_TALR_RESERVED2_SHIFT)
#define  CORE_TALR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CORE_TALR_RESERVED2_MASK) | (((uint32_t)val) << CORE_TALR_RESERVED2_SHIFT))

#define  CORE_TALR_ALTLUTHEIGHT_MASK 0x7
#define  CORE_TALR_ALTLUTHEIGHT_SHIFT 0
#define  CORE_TALR_ALTLUTHEIGHT_MASK_SHIFT(reg) (((reg) & CORE_TALR_ALTLUTHEIGHT_MASK) >> CORE_TALR_ALTLUTHEIGHT_SHIFT)
#define  CORE_TALR_ALTLUTHEIGHT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TALR_ALTLUTHEIGHT_MASK) | (((uint32_t)val) << CORE_TALR_ALTLUTHEIGHT_SHIFT))

//====================================================================
//Register: TH Save/Restore Register (TSRR)
//====================================================================

#define  CORE_TSRR_RESERVED1_MASK 0xfffffff0
#define  CORE_TSRR_RESERVED1_SHIFT 4
#define  CORE_TSRR_RESERVED1_MASK_SHIFT(reg) (((reg) & CORE_TSRR_RESERVED1_MASK) >> CORE_TSRR_RESERVED1_SHIFT)
#define  CORE_TSRR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSRR_RESERVED1_MASK) | (((uint32_t)val) << CORE_TSRR_RESERVED1_SHIFT))

#define  CORE_TSRR_CELLGENDT_MASK 0xc
#define  CORE_TSRR_CELLGENDT_SHIFT 2
#define  CORE_TSRR_CELLGENDT_MASK_SHIFT(reg) (((reg) & CORE_TSRR_CELLGENDT_MASK) >> CORE_TSRR_CELLGENDT_SHIFT)
#define  CORE_TSRR_CELLGENDT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSRR_CELLGENDT_MASK) | (((uint32_t)val) << CORE_TSRR_CELLGENDT_SHIFT))

#define  CORE_TSRR_POSGENDT_MASK 0x3
#define  CORE_TSRR_POSGENDT_SHIFT 0
#define  CORE_TSRR_POSGENDT_MASK_SHIFT(reg) (((reg) & CORE_TSRR_POSGENDT_MASK) >> CORE_TSRR_POSGENDT_SHIFT)
#define  CORE_TSRR_POSGENDT_REPLACE_VAL(reg,val) (((reg) & ~CORE_TSRR_POSGENDT_MASK) | (((uint32_t)val) << CORE_TSRR_POSGENDT_SHIFT))

//====================================================================
//
//Register File: SRAM (SRAM)
/** \brief SRAM descriptions for the Thresholding block*/
//
//====================================================================

//====================================================================
//Register: InLUT SRAM (ILUT)
//====================================================================

#define  SRAM_ILUT_RESERVED1_MASK 0xffff0000
#define  SRAM_ILUT_RESERVED1_SHIFT 16
#define  SRAM_ILUT_RESERVED1_MASK_SHIFT(reg) (((reg) & SRAM_ILUT_RESERVED1_MASK) >> SRAM_ILUT_RESERVED1_SHIFT)
#define  SRAM_ILUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SRAM_ILUT_RESERVED1_MASK) | (((uint32_t)val) << SRAM_ILUT_RESERVED1_SHIFT))

#define  SRAM_ILUT_FIELD_MASK 0xffff
#define  SRAM_ILUT_FIELD_SHIFT 0
#define  SRAM_ILUT_FIELD_MASK_SHIFT(reg) (((reg) & SRAM_ILUT_FIELD_MASK) >> SRAM_ILUT_FIELD_SHIFT)
#define  SRAM_ILUT_FIELD_REPLACE_VAL(reg,val) (((reg) & ~SRAM_ILUT_FIELD_MASK) | (((uint32_t)val) << SRAM_ILUT_FIELD_SHIFT))

//====================================================================
//Register: uCell SRAM (CELL0)
//====================================================================

#define  SRAM_CELL0_FIELD_MASK 0xffffffff
#define  SRAM_CELL0_FIELD_SHIFT 0
#define  SRAM_CELL0_FIELD_MASK_SHIFT(reg) (((reg) & SRAM_CELL0_FIELD_MASK) >> SRAM_CELL0_FIELD_SHIFT)
#define  SRAM_CELL0_FIELD_REPLACE_VAL(reg,val) (((reg) & ~SRAM_CELL0_FIELD_MASK) | (((uint32_t)val) << SRAM_CELL0_FIELD_SHIFT))

//====================================================================
//Register: uCell SRAM (CELL1)
//====================================================================

#define  SRAM_CELL1_FIELD_MASK 0xffffffff
#define  SRAM_CELL1_FIELD_SHIFT 0
#define  SRAM_CELL1_FIELD_MASK_SHIFT(reg) (((reg) & SRAM_CELL1_FIELD_MASK) >> SRAM_CELL1_FIELD_SHIFT)
#define  SRAM_CELL1_FIELD_REPLACE_VAL(reg,val) (((reg) & ~SRAM_CELL1_FIELD_MASK) | (((uint32_t)val) << SRAM_CELL1_FIELD_SHIFT))

//====================================================================
//Register: AltLUT SRAM (ALUT)
//====================================================================

#define  SRAM_ALUT_RESERVED1_MASK 0xfffffff0
#define  SRAM_ALUT_RESERVED1_SHIFT 4
#define  SRAM_ALUT_RESERVED1_MASK_SHIFT(reg) (((reg) & SRAM_ALUT_RESERVED1_MASK) >> SRAM_ALUT_RESERVED1_SHIFT)
#define  SRAM_ALUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SRAM_ALUT_RESERVED1_MASK) | (((uint32_t)val) << SRAM_ALUT_RESERVED1_SHIFT))

#define  SRAM_ALUT_FIELD_MASK 0xf
#define  SRAM_ALUT_FIELD_SHIFT 0
#define  SRAM_ALUT_FIELD_MASK_SHIFT(reg) (((reg) & SRAM_ALUT_FIELD_MASK) >> SRAM_ALUT_FIELD_SHIFT)
#define  SRAM_ALUT_FIELD_REPLACE_VAL(reg,val) (((reg) & ~SRAM_ALUT_FIELD_MASK) | (((uint32_t)val) << SRAM_ALUT_FIELD_SHIFT))

//====================================================================
//Register: Stochastic SRAM (STOC)
//====================================================================

#define  SRAM_STOC_FIELD_MASK 0xffffffff
#define  SRAM_STOC_FIELD_SHIFT 0
#define  SRAM_STOC_FIELD_MASK_SHIFT(reg) (((reg) & SRAM_STOC_FIELD_MASK) >> SRAM_STOC_FIELD_SHIFT)
#define  SRAM_STOC_FIELD_REPLACE_VAL(reg,val) (((reg) & ~SRAM_STOC_FIELD_MASK) | (((uint32_t)val) << SRAM_STOC_FIELD_SHIFT))

#endif // TH
