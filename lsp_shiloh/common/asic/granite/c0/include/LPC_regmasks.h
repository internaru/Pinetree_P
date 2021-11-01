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
 * \file LPC_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _LPC_REGMASKS_H_
#define _LPC_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Linear Precision Control (LPC) (LPC)
/** \brief Top-level register file for LPC block*/
//
//====================================================================

//====================================================================
//Register: LPC Configuration Register (LCR)
/** \brief This register configures the major features of the LPC module*/
//====================================================================

#define  LPC_LCR_NAL_MASK 0x80000000
#define  LPC_LCR_NAL_SHIFT 31
#define  LPC_LCR_NAL_MASK_SHIFT(reg) (((reg) & LPC_LCR_NAL_MASK) >> LPC_LCR_NAL_SHIFT)
#define  LPC_LCR_NAL_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_NAL_MASK) | (((uint32_t)val) << LPC_LCR_NAL_SHIFT))

#define  LPC_LCR_SCANPATH_MASK 0x60000000
#define  LPC_LCR_SCANPATH_SHIFT 29
#define  LPC_LCR_SCANPATH_MASK_SHIFT(reg) (((reg) & LPC_LCR_SCANPATH_MASK) >> LPC_LCR_SCANPATH_SHIFT)
#define  LPC_LCR_SCANPATH_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_SCANPATH_MASK) | (((uint32_t)val) << LPC_LCR_SCANPATH_SHIFT))

#define  LPC_LCR_ROUNDMODE_MASK 0x18000000
#define  LPC_LCR_ROUNDMODE_SHIFT 27
#define  LPC_LCR_ROUNDMODE_MASK_SHIFT(reg) (((reg) & LPC_LCR_ROUNDMODE_MASK) >> LPC_LCR_ROUNDMODE_SHIFT)
#define  LPC_LCR_ROUNDMODE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_ROUNDMODE_MASK) | (((uint32_t)val) << LPC_LCR_ROUNDMODE_SHIFT))

#define  LPC_LCR_DIRALGMODE_MASK 0x7000000
#define  LPC_LCR_DIRALGMODE_SHIFT 24
#define  LPC_LCR_DIRALGMODE_MASK_SHIFT(reg) (((reg) & LPC_LCR_DIRALGMODE_MASK) >> LPC_LCR_DIRALGMODE_SHIFT)
#define  LPC_LCR_DIRALGMODE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_DIRALGMODE_MASK) | (((uint32_t)val) << LPC_LCR_DIRALGMODE_SHIFT))

#define  LPC_LCR_RESERVED1_MASK 0x800000
#define  LPC_LCR_RESERVED1_SHIFT 23
#define  LPC_LCR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LCR_RESERVED1_MASK) >> LPC_LCR_RESERVED1_SHIFT)
#define  LPC_LCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LCR_RESERVED1_SHIFT))

#define  LPC_LCR_OUTPUTFORMAT_MASK 0x700000
#define  LPC_LCR_OUTPUTFORMAT_SHIFT 20
#define  LPC_LCR_OUTPUTFORMAT_MASK_SHIFT(reg) (((reg) & LPC_LCR_OUTPUTFORMAT_MASK) >> LPC_LCR_OUTPUTFORMAT_SHIFT)
#define  LPC_LCR_OUTPUTFORMAT_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_OUTPUTFORMAT_MASK) | (((uint32_t)val) << LPC_LCR_OUTPUTFORMAT_SHIFT))

#define  LPC_LCR_RESERVED2_MASK 0x80000
#define  LPC_LCR_RESERVED2_SHIFT 19
#define  LPC_LCR_RESERVED2_MASK_SHIFT(reg) (((reg) & LPC_LCR_RESERVED2_MASK) >> LPC_LCR_RESERVED2_SHIFT)
#define  LPC_LCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_RESERVED2_MASK) | (((uint32_t)val) << LPC_LCR_RESERVED2_SHIFT))

#define  LPC_LCR_INPUTFORMAT_MASK 0x70000
#define  LPC_LCR_INPUTFORMAT_SHIFT 16
#define  LPC_LCR_INPUTFORMAT_MASK_SHIFT(reg) (((reg) & LPC_LCR_INPUTFORMAT_MASK) >> LPC_LCR_INPUTFORMAT_SHIFT)
#define  LPC_LCR_INPUTFORMAT_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_INPUTFORMAT_MASK) | (((uint32_t)val) << LPC_LCR_INPUTFORMAT_SHIFT))

#define  LPC_LCR_SLOPESF_MASK 0xf000
#define  LPC_LCR_SLOPESF_SHIFT 12
#define  LPC_LCR_SLOPESF_MASK_SHIFT(reg) (((reg) & LPC_LCR_SLOPESF_MASK) >> LPC_LCR_SLOPESF_SHIFT)
#define  LPC_LCR_SLOPESF_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_SLOPESF_MASK) | (((uint32_t)val) << LPC_LCR_SLOPESF_SHIFT))

#define  LPC_LCR_LENGTHSF_MASK 0xc00
#define  LPC_LCR_LENGTHSF_SHIFT 10
#define  LPC_LCR_LENGTHSF_MASK_SHIFT(reg) (((reg) & LPC_LCR_LENGTHSF_MASK) >> LPC_LCR_LENGTHSF_SHIFT)
#define  LPC_LCR_LENGTHSF_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_LENGTHSF_MASK) | (((uint32_t)val) << LPC_LCR_LENGTHSF_SHIFT))

#define  LPC_LCR_MIXLUTMODE_MASK 0x300
#define  LPC_LCR_MIXLUTMODE_SHIFT 8
#define  LPC_LCR_MIXLUTMODE_MASK_SHIFT(reg) (((reg) & LPC_LCR_MIXLUTMODE_MASK) >> LPC_LCR_MIXLUTMODE_SHIFT)
#define  LPC_LCR_MIXLUTMODE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_MIXLUTMODE_MASK) | (((uint32_t)val) << LPC_LCR_MIXLUTMODE_SHIFT))

#define  LPC_LCR_SAVERESTOREMODE_MASK 0x80
#define  LPC_LCR_SAVERESTOREMODE_SHIFT 7
#define  LPC_LCR_SAVERESTOREMODE_MASK_SHIFT(reg) (((reg) & LPC_LCR_SAVERESTOREMODE_MASK) >> LPC_LCR_SAVERESTOREMODE_SHIFT)
#define  LPC_LCR_SAVERESTOREMODE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_SAVERESTOREMODE_MASK) | (((uint32_t)val) << LPC_LCR_SAVERESTOREMODE_SHIFT))

#define  LPC_LCR_LINEBUFACCESS_MASK 0x40
#define  LPC_LCR_LINEBUFACCESS_SHIFT 6
#define  LPC_LCR_LINEBUFACCESS_MASK_SHIFT(reg) (((reg) & LPC_LCR_LINEBUFACCESS_MASK) >> LPC_LCR_LINEBUFACCESS_SHIFT)
#define  LPC_LCR_LINEBUFACCESS_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_LINEBUFACCESS_MASK) | (((uint32_t)val) << LPC_LCR_LINEBUFACCESS_SHIFT))

#define  LPC_LCR_MIXLUTACCESS_MASK 0x20
#define  LPC_LCR_MIXLUTACCESS_SHIFT 5
#define  LPC_LCR_MIXLUTACCESS_MASK_SHIFT(reg) (((reg) & LPC_LCR_MIXLUTACCESS_MASK) >> LPC_LCR_MIXLUTACCESS_SHIFT)
#define  LPC_LCR_MIXLUTACCESS_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_MIXLUTACCESS_MASK) | (((uint32_t)val) << LPC_LCR_MIXLUTACCESS_SHIFT))

#define  LPC_LCR_FPTACCESS_MASK 0x10
#define  LPC_LCR_FPTACCESS_SHIFT 4
#define  LPC_LCR_FPTACCESS_MASK_SHIFT(reg) (((reg) & LPC_LCR_FPTACCESS_MASK) >> LPC_LCR_FPTACCESS_SHIFT)
#define  LPC_LCR_FPTACCESS_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_FPTACCESS_MASK) | (((uint32_t)val) << LPC_LCR_FPTACCESS_SHIFT))

#define  LPC_LCR_RESERVED3_MASK 0xc
#define  LPC_LCR_RESERVED3_SHIFT 2
#define  LPC_LCR_RESERVED3_MASK_SHIFT(reg) (((reg) & LPC_LCR_RESERVED3_MASK) >> LPC_LCR_RESERVED3_SHIFT)
#define  LPC_LCR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_RESERVED3_MASK) | (((uint32_t)val) << LPC_LCR_RESERVED3_SHIFT))

#define  LPC_LCR_SERP_MASK 0x2
#define  LPC_LCR_SERP_SHIFT 1
#define  LPC_LCR_SERP_MASK_SHIFT(reg) (((reg) & LPC_LCR_SERP_MASK) >> LPC_LCR_SERP_SHIFT)
#define  LPC_LCR_SERP_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_SERP_MASK) | (((uint32_t)val) << LPC_LCR_SERP_SHIFT))

#define  LPC_LCR_LPCBYPASS_MASK 0x1
#define  LPC_LCR_LPCBYPASS_SHIFT 0
#define  LPC_LCR_LPCBYPASS_MASK_SHIFT(reg) (((reg) & LPC_LCR_LPCBYPASS_MASK) >> LPC_LCR_LPCBYPASS_SHIFT)
#define  LPC_LCR_LPCBYPASS_REPLACE_VAL(reg,val) (((reg) & ~LPC_LCR_LPCBYPASS_MASK) | (((uint32_t)val) << LPC_LCR_LPCBYPASS_SHIFT))

//====================================================================
//Register: LPC Image Configuration Register (LICR)
/** \brief This register provides LPC information about the input image. The LPC Line Buffer stores 2 lines and is organized into a 3584x32 SRAM, therefore pixel data from each line is packed into 16 bits.
<br>
LPC requires the image width to be 16-bit aligned.
<br>
The max image width supported will depend on the input bits per pixel (bpp).
<br>
<table align="center"> <tr><td>8 bpp</td><td>7168 pixels</td></tr> <tr><td>4 bpp</td><td>14336 pixels</td></tr> <tr><td>2 bpp</td><td>28672 pixels</td></tr> <tr><td>1 bpp</td><td>57344 pixels</td></tr></table>*/
//====================================================================

#define  LPC_LICR_RESERVED1_MASK 0xff000000
#define  LPC_LICR_RESERVED1_SHIFT 24
#define  LPC_LICR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LICR_RESERVED1_MASK) >> LPC_LICR_RESERVED1_SHIFT)
#define  LPC_LICR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LICR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LICR_RESERVED1_SHIFT))

#define  LPC_LICR_Y_R2L_MASK 0xf00000
#define  LPC_LICR_Y_R2L_SHIFT 20
#define  LPC_LICR_Y_R2L_MASK_SHIFT(reg) (((reg) & LPC_LICR_Y_R2L_MASK) >> LPC_LICR_Y_R2L_SHIFT)
#define  LPC_LICR_Y_R2L_REPLACE_VAL(reg,val) (((reg) & ~LPC_LICR_Y_R2L_MASK) | (((uint32_t)val) << LPC_LICR_Y_R2L_SHIFT))

#define  LPC_LICR_X_R2L_MASK 0xf0000
#define  LPC_LICR_X_R2L_SHIFT 16
#define  LPC_LICR_X_R2L_MASK_SHIFT(reg) (((reg) & LPC_LICR_X_R2L_MASK) >> LPC_LICR_X_R2L_SHIFT)
#define  LPC_LICR_X_R2L_REPLACE_VAL(reg,val) (((reg) & ~LPC_LICR_X_R2L_MASK) | (((uint32_t)val) << LPC_LICR_X_R2L_SHIFT))

#define  LPC_LICR_RESERVED2_MASK 0xf000
#define  LPC_LICR_RESERVED2_SHIFT 12
#define  LPC_LICR_RESERVED2_MASK_SHIFT(reg) (((reg) & LPC_LICR_RESERVED2_MASK) >> LPC_LICR_RESERVED2_SHIFT)
#define  LPC_LICR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LPC_LICR_RESERVED2_MASK) | (((uint32_t)val) << LPC_LICR_RESERVED2_SHIFT))

#define  LPC_LICR_LASTSRAMADDR_MASK 0xfff
#define  LPC_LICR_LASTSRAMADDR_SHIFT 0
#define  LPC_LICR_LASTSRAMADDR_MASK_SHIFT(reg) (((reg) & LPC_LICR_LASTSRAMADDR_MASK) >> LPC_LICR_LASTSRAMADDR_SHIFT)
#define  LPC_LICR_LASTSRAMADDR_REPLACE_VAL(reg,val) (((reg) & ~LPC_LICR_LASTSRAMADDR_MASK) | (((uint32_t)val) << LPC_LICR_LASTSRAMADDR_SHIFT))

//====================================================================
//Register: LPC Fractional Path Configuration Register (FPCR)
//====================================================================

#define  LPC_FPCR_RPOS1START_MASK 0xff000000
#define  LPC_FPCR_RPOS1START_SHIFT 24
#define  LPC_FPCR_RPOS1START_MASK_SHIFT(reg) (((reg) & LPC_FPCR_RPOS1START_MASK) >> LPC_FPCR_RPOS1START_SHIFT)
#define  LPC_FPCR_RPOS1START_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR_RPOS1START_MASK) | (((uint32_t)val) << LPC_FPCR_RPOS1START_SHIFT))

#define  LPC_FPCR_RPOS0START_MASK 0xff0000
#define  LPC_FPCR_RPOS0START_SHIFT 16
#define  LPC_FPCR_RPOS0START_MASK_SHIFT(reg) (((reg) & LPC_FPCR_RPOS0START_MASK) >> LPC_FPCR_RPOS0START_SHIFT)
#define  LPC_FPCR_RPOS0START_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR_RPOS0START_MASK) | (((uint32_t)val) << LPC_FPCR_RPOS0START_SHIFT))

#define  LPC_FPCR_VPOSR2LSTART_MASK 0xff00
#define  LPC_FPCR_VPOSR2LSTART_SHIFT 8
#define  LPC_FPCR_VPOSR2LSTART_MASK_SHIFT(reg) (((reg) & LPC_FPCR_VPOSR2LSTART_MASK) >> LPC_FPCR_VPOSR2LSTART_SHIFT)
#define  LPC_FPCR_VPOSR2LSTART_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR_VPOSR2LSTART_MASK) | (((uint32_t)val) << LPC_FPCR_VPOSR2LSTART_SHIFT))

#define  LPC_FPCR_VPOSL2RSTART_MASK 0xff
#define  LPC_FPCR_VPOSL2RSTART_SHIFT 0
#define  LPC_FPCR_VPOSL2RSTART_MASK_SHIFT(reg) (((reg) & LPC_FPCR_VPOSL2RSTART_MASK) >> LPC_FPCR_VPOSL2RSTART_SHIFT)
#define  LPC_FPCR_VPOSL2RSTART_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR_VPOSL2RSTART_MASK) | (((uint32_t)val) << LPC_FPCR_VPOSL2RSTART_SHIFT))

//====================================================================
//Register: LPC Fractional Path Configuration Register 2 (FPCR2)
//====================================================================

#define  LPC_FPCR2_RESERVED1_MASK 0xfe000000
#define  LPC_FPCR2_RESERVED1_SHIFT 25
#define  LPC_FPCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_FPCR2_RESERVED1_MASK) >> LPC_FPCR2_RESERVED1_SHIFT)
#define  LPC_FPCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR2_RESERVED1_MASK) | (((uint32_t)val) << LPC_FPCR2_RESERVED1_SHIFT))

#define  LPC_FPCR2_RSLOPE1_MASK 0x1ff8000
#define  LPC_FPCR2_RSLOPE1_SHIFT 15
#define  LPC_FPCR2_RSLOPE1_MASK_SHIFT(reg) (((reg) & LPC_FPCR2_RSLOPE1_MASK) >> LPC_FPCR2_RSLOPE1_SHIFT)
#define  LPC_FPCR2_RSLOPE1_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR2_RSLOPE1_MASK) | (((uint32_t)val) << LPC_FPCR2_RSLOPE1_SHIFT))

#define  LPC_FPCR2_RESERVED2_MASK 0x7c00
#define  LPC_FPCR2_RESERVED2_SHIFT 10
#define  LPC_FPCR2_RESERVED2_MASK_SHIFT(reg) (((reg) & LPC_FPCR2_RESERVED2_MASK) >> LPC_FPCR2_RESERVED2_SHIFT)
#define  LPC_FPCR2_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR2_RESERVED2_MASK) | (((uint32_t)val) << LPC_FPCR2_RESERVED2_SHIFT))

#define  LPC_FPCR2_RSLOPE0_MASK 0x3ff
#define  LPC_FPCR2_RSLOPE0_SHIFT 0
#define  LPC_FPCR2_RSLOPE0_MASK_SHIFT(reg) (((reg) & LPC_FPCR2_RSLOPE0_MASK) >> LPC_FPCR2_RSLOPE0_SHIFT)
#define  LPC_FPCR2_RSLOPE0_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPCR2_RSLOPE0_MASK) | (((uint32_t)val) << LPC_FPCR2_RSLOPE0_SHIFT))

//====================================================================
//Register: LPC Window Manager Status Register (LWMSR)
/** \brief This register provides status of signals from the Window Manager for aid in debugging.*/
//====================================================================

#define  LPC_LWMSR_RESERVED1_MASK 0xffffc000
#define  LPC_LWMSR_RESERVED1_SHIFT 14
#define  LPC_LWMSR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_RESERVED1_MASK) >> LPC_LWMSR_RESERVED1_SHIFT)
#define  LPC_LWMSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LWMSR_RESERVED1_SHIFT))

#define  LPC_LWMSR_WRSTATE_MASK 0x3000
#define  LPC_LWMSR_WRSTATE_SHIFT 12
#define  LPC_LWMSR_WRSTATE_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_WRSTATE_MASK) >> LPC_LWMSR_WRSTATE_SHIFT)
#define  LPC_LWMSR_WRSTATE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_WRSTATE_MASK) | (((uint32_t)val) << LPC_LWMSR_WRSTATE_SHIFT))

#define  LPC_LWMSR_RESERVED2_MASK 0xc00
#define  LPC_LWMSR_RESERVED2_SHIFT 10
#define  LPC_LWMSR_RESERVED2_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_RESERVED2_MASK) >> LPC_LWMSR_RESERVED2_SHIFT)
#define  LPC_LWMSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_RESERVED2_MASK) | (((uint32_t)val) << LPC_LWMSR_RESERVED2_SHIFT))

#define  LPC_LWMSR_RDSTATE_MASK 0x300
#define  LPC_LWMSR_RDSTATE_SHIFT 8
#define  LPC_LWMSR_RDSTATE_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_RDSTATE_MASK) >> LPC_LWMSR_RDSTATE_SHIFT)
#define  LPC_LWMSR_RDSTATE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_RDSTATE_MASK) | (((uint32_t)val) << LPC_LWMSR_RDSTATE_SHIFT))

#define  LPC_LWMSR_RESERVED3_MASK 0x80
#define  LPC_LWMSR_RESERVED3_SHIFT 7
#define  LPC_LWMSR_RESERVED3_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_RESERVED3_MASK) >> LPC_LWMSR_RESERVED3_SHIFT)
#define  LPC_LWMSR_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_RESERVED3_MASK) | (((uint32_t)val) << LPC_LWMSR_RESERVED3_SHIFT))

#define  LPC_LWMSR_WMSTATE_MASK 0x70
#define  LPC_LWMSR_WMSTATE_SHIFT 4
#define  LPC_LWMSR_WMSTATE_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_WMSTATE_MASK) >> LPC_LWMSR_WMSTATE_SHIFT)
#define  LPC_LWMSR_WMSTATE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_WMSTATE_MASK) | (((uint32_t)val) << LPC_LWMSR_WMSTATE_SHIFT))

#define  LPC_LWMSR_RESERVED4_MASK 0xe
#define  LPC_LWMSR_RESERVED4_SHIFT 1
#define  LPC_LWMSR_RESERVED4_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_RESERVED4_MASK) >> LPC_LWMSR_RESERVED4_SHIFT)
#define  LPC_LWMSR_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_RESERVED4_MASK) | (((uint32_t)val) << LPC_LWMSR_RESERVED4_SHIFT))

#define  LPC_LWMSR_ARRAYVALID_MASK 0x1
#define  LPC_LWMSR_ARRAYVALID_SHIFT 0
#define  LPC_LWMSR_ARRAYVALID_MASK_SHIFT(reg) (((reg) & LPC_LWMSR_ARRAYVALID_MASK) >> LPC_LWMSR_ARRAYVALID_SHIFT)
#define  LPC_LWMSR_ARRAYVALID_REPLACE_VAL(reg,val) (((reg) & ~LPC_LWMSR_ARRAYVALID_MASK) | (((uint32_t)val) << LPC_LWMSR_ARRAYVALID_SHIFT))

//====================================================================
//Register: LPC Fractional Path Status Register (LFPSR)
/** \brief This register provides status of signals from the Fractional Path Generator to aid in debugging.*/
//====================================================================

#define  LPC_LFPSR_RESERVED1_MASK 0xff800000
#define  LPC_LFPSR_RESERVED1_SHIFT 23
#define  LPC_LFPSR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LFPSR_RESERVED1_MASK) >> LPC_LFPSR_RESERVED1_SHIFT)
#define  LPC_LFPSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LFPSR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LFPSR_RESERVED1_SHIFT))

#define  LPC_LFPSR_VERTPOS_MASK 0x7ffff0
#define  LPC_LFPSR_VERTPOS_SHIFT 4
#define  LPC_LFPSR_VERTPOS_MASK_SHIFT(reg) (((reg) & LPC_LFPSR_VERTPOS_MASK) >> LPC_LFPSR_VERTPOS_SHIFT)
#define  LPC_LFPSR_VERTPOS_REPLACE_VAL(reg,val) (((reg) & ~LPC_LFPSR_VERTPOS_MASK) | (((uint32_t)val) << LPC_LFPSR_VERTPOS_SHIFT))

#define  LPC_LFPSR_RESERVED2_MASK 0x8
#define  LPC_LFPSR_RESERVED2_SHIFT 3
#define  LPC_LFPSR_RESERVED2_MASK_SHIFT(reg) (((reg) & LPC_LFPSR_RESERVED2_MASK) >> LPC_LFPSR_RESERVED2_SHIFT)
#define  LPC_LFPSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~LPC_LFPSR_RESERVED2_MASK) | (((uint32_t)val) << LPC_LFPSR_RESERVED2_SHIFT))

#define  LPC_LFPSR_SEGLENERROR_MASK 0x4
#define  LPC_LFPSR_SEGLENERROR_SHIFT 2
#define  LPC_LFPSR_SEGLENERROR_MASK_SHIFT(reg) (((reg) & LPC_LFPSR_SEGLENERROR_MASK) >> LPC_LFPSR_SEGLENERROR_SHIFT)
#define  LPC_LFPSR_SEGLENERROR_REPLACE_VAL(reg,val) (((reg) & ~LPC_LFPSR_SEGLENERROR_MASK) | (((uint32_t)val) << LPC_LFPSR_SEGLENERROR_SHIFT))

#define  LPC_LFPSR_ACCUMCLAMP_MASK 0x2
#define  LPC_LFPSR_ACCUMCLAMP_SHIFT 1
#define  LPC_LFPSR_ACCUMCLAMP_MASK_SHIFT(reg) (((reg) & LPC_LFPSR_ACCUMCLAMP_MASK) >> LPC_LFPSR_ACCUMCLAMP_SHIFT)
#define  LPC_LFPSR_ACCUMCLAMP_REPLACE_VAL(reg,val) (((reg) & ~LPC_LFPSR_ACCUMCLAMP_MASK) | (((uint32_t)val) << LPC_LFPSR_ACCUMCLAMP_SHIFT))

#define  LPC_LFPSR_ACCUMCLIP_MASK 0x1
#define  LPC_LFPSR_ACCUMCLIP_SHIFT 0
#define  LPC_LFPSR_ACCUMCLIP_MASK_SHIFT(reg) (((reg) & LPC_LFPSR_ACCUMCLIP_MASK) >> LPC_LFPSR_ACCUMCLIP_SHIFT)
#define  LPC_LFPSR_ACCUMCLIP_REPLACE_VAL(reg,val) (((reg) & ~LPC_LFPSR_ACCUMCLIP_MASK) | (((uint32_t)val) << LPC_LFPSR_ACCUMCLIP_SHIFT))

//====================================================================
//Register: LPC LineBuffer Access Control Register (LBAR)
//====================================================================

#define  LPC_LBAR_RESERVED1_MASK 0xfffffffc
#define  LPC_LBAR_RESERVED1_SHIFT 2
#define  LPC_LBAR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LBAR_RESERVED1_MASK) >> LPC_LBAR_RESERVED1_SHIFT)
#define  LPC_LBAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBAR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LBAR_RESERVED1_SHIFT))

#define  LPC_LBAR_INC_ADDR_MODE_MASK 0x2
#define  LPC_LBAR_INC_ADDR_MODE_SHIFT 1
#define  LPC_LBAR_INC_ADDR_MODE_MASK_SHIFT(reg) (((reg) & LPC_LBAR_INC_ADDR_MODE_MASK) >> LPC_LBAR_INC_ADDR_MODE_SHIFT)
#define  LPC_LBAR_INC_ADDR_MODE_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBAR_INC_ADDR_MODE_MASK) | (((uint32_t)val) << LPC_LBAR_INC_ADDR_MODE_SHIFT))

#define  LPC_LBAR_LB_RNW_MASK 0x1
#define  LPC_LBAR_LB_RNW_SHIFT 0
#define  LPC_LBAR_LB_RNW_MASK_SHIFT(reg) (((reg) & LPC_LBAR_LB_RNW_MASK) >> LPC_LBAR_LB_RNW_SHIFT)
#define  LPC_LBAR_LB_RNW_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBAR_LB_RNW_MASK) | (((uint32_t)val) << LPC_LBAR_LB_RNW_SHIFT))

//====================================================================
//Register: LPC LineBuffer Address Write Register (LBAWR)
//====================================================================

#define  LPC_LBAWR_RESERVED1_MASK 0xfffff000
#define  LPC_LBAWR_RESERVED1_SHIFT 12
#define  LPC_LBAWR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LBAWR_RESERVED1_MASK) >> LPC_LBAWR_RESERVED1_SHIFT)
#define  LPC_LBAWR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBAWR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LBAWR_RESERVED1_SHIFT))

#define  LPC_LBAWR_ADDRESS_MASK 0xfff
#define  LPC_LBAWR_ADDRESS_SHIFT 0
#define  LPC_LBAWR_ADDRESS_MASK_SHIFT(reg) (((reg) & LPC_LBAWR_ADDRESS_MASK) >> LPC_LBAWR_ADDRESS_SHIFT)
#define  LPC_LBAWR_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBAWR_ADDRESS_MASK) | (((uint32_t)val) << LPC_LBAWR_ADDRESS_SHIFT))

//====================================================================
//Register: LPC LineBuffer Address Status Register (LBSAR)
//====================================================================

#define  LPC_LBSAR_RESERVED1_MASK 0xfffff000
#define  LPC_LBSAR_RESERVED1_SHIFT 12
#define  LPC_LBSAR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LBSAR_RESERVED1_MASK) >> LPC_LBSAR_RESERVED1_SHIFT)
#define  LPC_LBSAR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBSAR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LBSAR_RESERVED1_SHIFT))

#define  LPC_LBSAR_ADDRESS_MASK 0xfff
#define  LPC_LBSAR_ADDRESS_SHIFT 0
#define  LPC_LBSAR_ADDRESS_MASK_SHIFT(reg) (((reg) & LPC_LBSAR_ADDRESS_MASK) >> LPC_LBSAR_ADDRESS_SHIFT)
#define  LPC_LBSAR_ADDRESS_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBSAR_ADDRESS_MASK) | (((uint32_t)val) << LPC_LBSAR_ADDRESS_SHIFT))

//====================================================================
//Register: LPC LineBuffer Data Write Register (LBDWR)
/** \brief This register writes the LPC line buffer SRAM.*/
//====================================================================

#define  LPC_LBDWR_DATA_MASK 0xffffffff
#define  LPC_LBDWR_DATA_SHIFT 0
#define  LPC_LBDWR_DATA_MASK_SHIFT(reg) (((reg) & LPC_LBDWR_DATA_MASK) >> LPC_LBDWR_DATA_SHIFT)
#define  LPC_LBDWR_DATA_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBDWR_DATA_MASK) | (((uint32_t)val) << LPC_LBDWR_DATA_SHIFT))

//====================================================================
//Register: LPC LineBuffer Data Read Register (LBDRR)
/** \brief This register reads the LPC line buffer SRAM.*/
//====================================================================

#define  LPC_LBDRR_DATA_MASK 0xffffffff
#define  LPC_LBDRR_DATA_SHIFT 0
#define  LPC_LBDRR_DATA_MASK_SHIFT(reg) (((reg) & LPC_LBDRR_DATA_MASK) >> LPC_LBDRR_DATA_SHIFT)
#define  LPC_LBDRR_DATA_REPLACE_VAL(reg,val) (((reg) & ~LPC_LBDRR_DATA_MASK) | (((uint32_t)val) << LPC_LBDRR_DATA_SHIFT))

//====================================================================
//Register: LPC LFSR Initialization Register (LLIR)
//====================================================================

#define  LPC_LLIR_RESERVED1_MASK 0xfffff000
#define  LPC_LLIR_RESERVED1_SHIFT 12
#define  LPC_LLIR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LLIR_RESERVED1_MASK) >> LPC_LLIR_RESERVED1_SHIFT)
#define  LPC_LLIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LLIR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LLIR_RESERVED1_SHIFT))

#define  LPC_LLIR_LFSRINIT_MASK 0xfff
#define  LPC_LLIR_LFSRINIT_SHIFT 0
#define  LPC_LLIR_LFSRINIT_MASK_SHIFT(reg) (((reg) & LPC_LLIR_LFSRINIT_MASK) >> LPC_LLIR_LFSRINIT_SHIFT)
#define  LPC_LLIR_LFSRINIT_REPLACE_VAL(reg,val) (((reg) & ~LPC_LLIR_LFSRINIT_MASK) | (((uint32_t)val) << LPC_LLIR_LFSRINIT_SHIFT))

//====================================================================
//Register: LPC LFSR Current Value Register (LLCVR)
//====================================================================

#define  LPC_LLCVR_RESERVED1_MASK 0xfffff000
#define  LPC_LLCVR_RESERVED1_SHIFT 12
#define  LPC_LLCVR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LLCVR_RESERVED1_MASK) >> LPC_LLCVR_RESERVED1_SHIFT)
#define  LPC_LLCVR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LLCVR_RESERVED1_MASK) | (((uint32_t)val) << LPC_LLCVR_RESERVED1_SHIFT))

#define  LPC_LLCVR_LFSRVAL_MASK 0xfff
#define  LPC_LLCVR_LFSRVAL_SHIFT 0
#define  LPC_LLCVR_LFSRVAL_MASK_SHIFT(reg) (((reg) & LPC_LLCVR_LFSRVAL_MASK) >> LPC_LLCVR_LFSRVAL_SHIFT)
#define  LPC_LLCVR_LFSRVAL_REPLACE_VAL(reg,val) (((reg) & ~LPC_LLCVR_LFSRVAL_MASK) | (((uint32_t)val) << LPC_LLCVR_LFSRVAL_SHIFT))

//====================================================================
//Register: LPC FPT R2L Index Register (FRIR)
/** \brief If LPC is operating on serpentine data, it will traverse the Fractional Path Table in a serpentine manner. When scanning left to right it will begin on segment 0 and increment up to the last needed segment. When scanning right to left it will begin on the value programmed into R2LIndex and decrement down to the last needed segment.*/
//====================================================================

#define  LPC_FRIR_RESERVED1_MASK 0xffffff00
#define  LPC_FRIR_RESERVED1_SHIFT 8
#define  LPC_FRIR_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_FRIR_RESERVED1_MASK) >> LPC_FRIR_RESERVED1_SHIFT)
#define  LPC_FRIR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_FRIR_RESERVED1_MASK) | (((uint32_t)val) << LPC_FRIR_RESERVED1_SHIFT))

#define  LPC_FRIR_R2LINDEX_MASK 0xff
#define  LPC_FRIR_R2LINDEX_SHIFT 0
#define  LPC_FRIR_R2LINDEX_MASK_SHIFT(reg) (((reg) & LPC_FRIR_R2LINDEX_MASK) >> LPC_FRIR_R2LINDEX_SHIFT)
#define  LPC_FRIR_R2LINDEX_REPLACE_VAL(reg,val) (((reg) & ~LPC_FRIR_R2LINDEX_MASK) | (((uint32_t)val) << LPC_FRIR_R2LINDEX_SHIFT))

//====================================================================
//Register: LPC Save Restore Address (LSRA)
/** \brief This register is used to set the address of the save/restore register to access. There is currently 1 32-bit register that must be saved and restored, therefore valid addresses are 0x0.*/
//====================================================================

#define  LPC_LSRA_RESERVED1_MASK 0xfffffffe
#define  LPC_LSRA_RESERVED1_SHIFT 1
#define  LPC_LSRA_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_LSRA_RESERVED1_MASK) >> LPC_LSRA_RESERVED1_SHIFT)
#define  LPC_LSRA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_LSRA_RESERVED1_MASK) | (((uint32_t)val) << LPC_LSRA_RESERVED1_SHIFT))

#define  LPC_LSRA_SRADDR_MASK 0x1
#define  LPC_LSRA_SRADDR_SHIFT 0
#define  LPC_LSRA_SRADDR_MASK_SHIFT(reg) (((reg) & LPC_LSRA_SRADDR_MASK) >> LPC_LSRA_SRADDR_SHIFT)
#define  LPC_LSRA_SRADDR_REPLACE_VAL(reg,val) (((reg) & ~LPC_LSRA_SRADDR_MASK) | (((uint32_t)val) << LPC_LSRA_SRADDR_SHIFT))

//====================================================================
//Register: LPC Save Restore Read (LSRR)
/** \brief This read-only register is provided for the save portion of save/restore functionality. The "SaveRestoreMode" bit of the configuration register must be set to a 1 to access this register. The address for this value is controlled by the LSRA register.*/
//====================================================================

#define  LPC_LSRR_SAVEVAL_MASK 0xffffffff
#define  LPC_LSRR_SAVEVAL_SHIFT 0
#define  LPC_LSRR_SAVEVAL_MASK_SHIFT(reg) (((reg) & LPC_LSRR_SAVEVAL_MASK) >> LPC_LSRR_SAVEVAL_SHIFT)
#define  LPC_LSRR_SAVEVAL_REPLACE_VAL(reg,val) (((reg) & ~LPC_LSRR_SAVEVAL_MASK) | (((uint32_t)val) << LPC_LSRR_SAVEVAL_SHIFT))

//====================================================================
//Register: LPC Save Restore Write (LSRW)
/** \brief This write-only register is provided for the restore portion of save/restore functionality. The "SaveRestoreMode" bit of the configuration register must be set to a 1 to access this register. The address for this value is controlled by the LSRA register.*/
//====================================================================

#define  LPC_LSRW_RESTOREVAL_MASK 0xffffffff
#define  LPC_LSRW_RESTOREVAL_SHIFT 0
#define  LPC_LSRW_RESTOREVAL_MASK_SHIFT(reg) (((reg) & LPC_LSRW_RESTOREVAL_MASK) >> LPC_LSRW_RESTOREVAL_SHIFT)
#define  LPC_LSRW_RESTOREVAL_REPLACE_VAL(reg,val) (((reg) & ~LPC_LSRW_RESTOREVAL_MASK) | (((uint32_t)val) << LPC_LSRW_RESTOREVAL_SHIFT))

//====================================================================
//Register: Fractional Path Table (FPT)
/** \brief This table represents the 256x18 Fractional Path Table.*/
//====================================================================

#define  LPC_FPT_RESERVED1_MASK 0xfffc0000
#define  LPC_FPT_RESERVED1_SHIFT 18
#define  LPC_FPT_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_FPT_RESERVED1_MASK) >> LPC_FPT_RESERVED1_SHIFT)
#define  LPC_FPT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPT_RESERVED1_MASK) | (((uint32_t)val) << LPC_FPT_RESERVED1_SHIFT))

#define  LPC_FPT_CHANGELINE_MASK 0x30000
#define  LPC_FPT_CHANGELINE_SHIFT 16
#define  LPC_FPT_CHANGELINE_MASK_SHIFT(reg) (((reg) & LPC_FPT_CHANGELINE_MASK) >> LPC_FPT_CHANGELINE_SHIFT)
#define  LPC_FPT_CHANGELINE_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPT_CHANGELINE_MASK) | (((uint32_t)val) << LPC_FPT_CHANGELINE_SHIFT))

#define  LPC_FPT_LENGTH_MASK 0xff00
#define  LPC_FPT_LENGTH_SHIFT 8
#define  LPC_FPT_LENGTH_MASK_SHIFT(reg) (((reg) & LPC_FPT_LENGTH_MASK) >> LPC_FPT_LENGTH_SHIFT)
#define  LPC_FPT_LENGTH_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPT_LENGTH_MASK) | (((uint32_t)val) << LPC_FPT_LENGTH_SHIFT))

#define  LPC_FPT_SLOPE_MASK 0xff
#define  LPC_FPT_SLOPE_SHIFT 0
#define  LPC_FPT_SLOPE_MASK_SHIFT(reg) (((reg) & LPC_FPT_SLOPE_MASK) >> LPC_FPT_SLOPE_SHIFT)
#define  LPC_FPT_SLOPE_REPLACE_VAL(reg,val) (((reg) & ~LPC_FPT_SLOPE_MASK) | (((uint32_t)val) << LPC_FPT_SLOPE_SHIFT))

//====================================================================
//Register: Two Pixel Mixing LUT (MixLUT)
/** \brief This table represents the 256x16 LUT used for mixing two pixels.*/
//====================================================================

#define  LPC_MIXLUT_RESERVED1_MASK 0xffff0000
#define  LPC_MIXLUT_RESERVED1_SHIFT 16
#define  LPC_MIXLUT_RESERVED1_MASK_SHIFT(reg) (((reg) & LPC_MIXLUT_RESERVED1_MASK) >> LPC_MIXLUT_RESERVED1_SHIFT)
#define  LPC_MIXLUT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~LPC_MIXLUT_RESERVED1_MASK) | (((uint32_t)val) << LPC_MIXLUT_RESERVED1_SHIFT))

#define  LPC_MIXLUT_MIXDATA_MASK 0xffff
#define  LPC_MIXLUT_MIXDATA_SHIFT 0
#define  LPC_MIXLUT_MIXDATA_MASK_SHIFT(reg) (((reg) & LPC_MIXLUT_MIXDATA_MASK) >> LPC_MIXLUT_MIXDATA_SHIFT)
#define  LPC_MIXLUT_MIXDATA_REPLACE_VAL(reg,val) (((reg) & ~LPC_MIXLUT_MIXDATA_MASK) | (((uint32_t)val) << LPC_MIXLUT_MIXDATA_SHIFT))

#endif // LPC
