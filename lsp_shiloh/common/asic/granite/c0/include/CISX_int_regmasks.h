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
 * \file CISX_int_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CISX_INT_REGMASKS_H_
#define _CISX_INT_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CISX_int (CISX_int)
//
//====================================================================

//====================================================================
//Register: CISX Configuration Register (CisxCfg)
//====================================================================

#define  CISX_INT_CISXCFG_RESERVED1_MASK 0xf0000000
#define  CISX_INT_CISXCFG_RESERVED1_SHIFT 28
#define  CISX_INT_CISXCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_RESERVED1_MASK) >> CISX_INT_CISXCFG_RESERVED1_SHIFT)
#define  CISX_INT_CISXCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_RESERVED1_SHIFT))

#define  CISX_INT_CISXCFG_CISX_TAG_MASK 0xe000000
#define  CISX_INT_CISXCFG_CISX_TAG_SHIFT 25
#define  CISX_INT_CISXCFG_CISX_TAG_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_CISX_TAG_MASK) >> CISX_INT_CISXCFG_CISX_TAG_SHIFT)
#define  CISX_INT_CISXCFG_CISX_TAG_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_CISX_TAG_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_CISX_TAG_SHIFT))

#define  CISX_INT_CISXCFG_BYPASS_MASK 0x1000000
#define  CISX_INT_CISXCFG_BYPASS_SHIFT 24
#define  CISX_INT_CISXCFG_BYPASS_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_BYPASS_MASK) >> CISX_INT_CISXCFG_BYPASS_SHIFT)
#define  CISX_INT_CISXCFG_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_BYPASS_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_BYPASS_SHIFT))

#define  CISX_INT_CISXCFG_BYPASS_MODE_MASK 0x800000
#define  CISX_INT_CISXCFG_BYPASS_MODE_SHIFT 23
#define  CISX_INT_CISXCFG_BYPASS_MODE_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_BYPASS_MODE_MASK) >> CISX_INT_CISXCFG_BYPASS_MODE_SHIFT)
#define  CISX_INT_CISXCFG_BYPASS_MODE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_BYPASS_MODE_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_BYPASS_MODE_SHIFT))

#define  CISX_INT_CISXCFG_MODE_MASK 0x700000
#define  CISX_INT_CISXCFG_MODE_SHIFT 20
#define  CISX_INT_CISXCFG_MODE_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_MODE_MASK) >> CISX_INT_CISXCFG_MODE_SHIFT)
#define  CISX_INT_CISXCFG_MODE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_MODE_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_MODE_SHIFT))

#define  CISX_INT_CISXCFG_CBIOUT_PACE_MASK 0xf0000
#define  CISX_INT_CISXCFG_CBIOUT_PACE_SHIFT 16
#define  CISX_INT_CISXCFG_CBIOUT_PACE_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_CBIOUT_PACE_MASK) >> CISX_INT_CISXCFG_CBIOUT_PACE_SHIFT)
#define  CISX_INT_CISXCFG_CBIOUT_PACE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_CBIOUT_PACE_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_CBIOUT_PACE_SHIFT))

#define  CISX_INT_CISXCFG_NUMSENSORS_MASK 0x8000
#define  CISX_INT_CISXCFG_NUMSENSORS_SHIFT 15
#define  CISX_INT_CISXCFG_NUMSENSORS_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_NUMSENSORS_MASK) >> CISX_INT_CISXCFG_NUMSENSORS_SHIFT)
#define  CISX_INT_CISXCFG_NUMSENSORS_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_NUMSENSORS_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_NUMSENSORS_SHIFT))

#define  CISX_INT_CISXCFG_RESERVED2_MASK 0x4000
#define  CISX_INT_CISXCFG_RESERVED2_SHIFT 14
#define  CISX_INT_CISXCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_RESERVED2_MASK) >> CISX_INT_CISXCFG_RESERVED2_SHIFT)
#define  CISX_INT_CISXCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_RESERVED2_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_RESERVED2_SHIFT))

#define  CISX_INT_CISXCFG_NUMCHANS_MASK 0x3000
#define  CISX_INT_CISXCFG_NUMCHANS_SHIFT 12
#define  CISX_INT_CISXCFG_NUMCHANS_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_NUMCHANS_MASK) >> CISX_INT_CISXCFG_NUMCHANS_SHIFT)
#define  CISX_INT_CISXCFG_NUMCHANS_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_NUMCHANS_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_NUMCHANS_SHIFT))

#define  CISX_INT_CISXCFG_CFG_2CH_ORDER_MASK 0x800
#define  CISX_INT_CISXCFG_CFG_2CH_ORDER_SHIFT 11
#define  CISX_INT_CISXCFG_CFG_2CH_ORDER_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_CFG_2CH_ORDER_MASK) >> CISX_INT_CISXCFG_CFG_2CH_ORDER_SHIFT)
#define  CISX_INT_CISXCFG_CFG_2CH_ORDER_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_CFG_2CH_ORDER_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_CFG_2CH_ORDER_SHIFT))

#define  CISX_INT_CISXCFG_CFG_3CH_ORDER_MASK 0x700
#define  CISX_INT_CISXCFG_CFG_3CH_ORDER_SHIFT 8
#define  CISX_INT_CISXCFG_CFG_3CH_ORDER_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_CFG_3CH_ORDER_MASK) >> CISX_INT_CISXCFG_CFG_3CH_ORDER_SHIFT)
#define  CISX_INT_CISXCFG_CFG_3CH_ORDER_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_CFG_3CH_ORDER_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_CFG_3CH_ORDER_SHIFT))

#define  CISX_INT_CISXCFG_FORCE_ODD_MASK 0x80
#define  CISX_INT_CISXCFG_FORCE_ODD_SHIFT 7
#define  CISX_INT_CISXCFG_FORCE_ODD_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_FORCE_ODD_MASK) >> CISX_INT_CISXCFG_FORCE_ODD_SHIFT)
#define  CISX_INT_CISXCFG_FORCE_ODD_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_FORCE_ODD_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_FORCE_ODD_SHIFT))

#define  CISX_INT_CISXCFG_CCD_ORDER_MASK 0x70
#define  CISX_INT_CISXCFG_CCD_ORDER_SHIFT 4
#define  CISX_INT_CISXCFG_CCD_ORDER_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_CCD_ORDER_MASK) >> CISX_INT_CISXCFG_CCD_ORDER_SHIFT)
#define  CISX_INT_CISXCFG_CCD_ORDER_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_CCD_ORDER_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_CCD_ORDER_SHIFT))

#define  CISX_INT_CISXCFG_RESERVED3_MASK 0x8
#define  CISX_INT_CISXCFG_RESERVED3_SHIFT 3
#define  CISX_INT_CISXCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_RESERVED3_MASK) >> CISX_INT_CISXCFG_RESERVED3_SHIFT)
#define  CISX_INT_CISXCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_RESERVED3_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_RESERVED3_SHIFT))

#define  CISX_INT_CISXCFG_APPCHAN2_MASK 0x4
#define  CISX_INT_CISXCFG_APPCHAN2_SHIFT 2
#define  CISX_INT_CISXCFG_APPCHAN2_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_APPCHAN2_MASK) >> CISX_INT_CISXCFG_APPCHAN2_SHIFT)
#define  CISX_INT_CISXCFG_APPCHAN2_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_APPCHAN2_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_APPCHAN2_SHIFT))

#define  CISX_INT_CISXCFG_APPCHAN1_MASK 0x2
#define  CISX_INT_CISXCFG_APPCHAN1_SHIFT 1
#define  CISX_INT_CISXCFG_APPCHAN1_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_APPCHAN1_MASK) >> CISX_INT_CISXCFG_APPCHAN1_SHIFT)
#define  CISX_INT_CISXCFG_APPCHAN1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_APPCHAN1_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_APPCHAN1_SHIFT))

#define  CISX_INT_CISXCFG_APPCHAN0_MASK 0x1
#define  CISX_INT_CISXCFG_APPCHAN0_SHIFT 0
#define  CISX_INT_CISXCFG_APPCHAN0_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXCFG_APPCHAN0_MASK) >> CISX_INT_CISXCFG_APPCHAN0_SHIFT)
#define  CISX_INT_CISXCFG_APPCHAN0_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXCFG_APPCHAN0_MASK) | (((uint32_t)val) << CISX_INT_CISXCFG_APPCHAN0_SHIFT))

//====================================================================
//Register: Status register (CisxStat)
//====================================================================

#define  CISX_INT_CISXSTAT_RESERVED1_MASK 0xffffff80
#define  CISX_INT_CISXSTAT_RESERVED1_SHIFT 7
#define  CISX_INT_CISXSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXSTAT_RESERVED1_MASK) >> CISX_INT_CISXSTAT_RESERVED1_SHIFT)
#define  CISX_INT_CISXSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXSTAT_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CISXSTAT_RESERVED1_SHIFT))

#define  CISX_INT_CISXSTAT_LINE_SM_MASK 0x70
#define  CISX_INT_CISXSTAT_LINE_SM_SHIFT 4
#define  CISX_INT_CISXSTAT_LINE_SM_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXSTAT_LINE_SM_MASK) >> CISX_INT_CISXSTAT_LINE_SM_SHIFT)
#define  CISX_INT_CISXSTAT_LINE_SM_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXSTAT_LINE_SM_MASK) | (((uint32_t)val) << CISX_INT_CISXSTAT_LINE_SM_SHIFT))

#define  CISX_INT_CISXSTAT_RESERVED2_MASK 0xc
#define  CISX_INT_CISXSTAT_RESERVED2_SHIFT 2
#define  CISX_INT_CISXSTAT_RESERVED2_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXSTAT_RESERVED2_MASK) >> CISX_INT_CISXSTAT_RESERVED2_SHIFT)
#define  CISX_INT_CISXSTAT_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXSTAT_RESERVED2_MASK) | (((uint32_t)val) << CISX_INT_CISXSTAT_RESERVED2_SHIFT))

#define  CISX_INT_CISXSTAT_ODMA_BUSY_MASK 0x2
#define  CISX_INT_CISXSTAT_ODMA_BUSY_SHIFT 1
#define  CISX_INT_CISXSTAT_ODMA_BUSY_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXSTAT_ODMA_BUSY_MASK) >> CISX_INT_CISXSTAT_ODMA_BUSY_SHIFT)
#define  CISX_INT_CISXSTAT_ODMA_BUSY_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXSTAT_ODMA_BUSY_MASK) | (((uint32_t)val) << CISX_INT_CISXSTAT_ODMA_BUSY_SHIFT))

#define  CISX_INT_CISXSTAT_IDMA_BUSY_MASK 0x1
#define  CISX_INT_CISXSTAT_IDMA_BUSY_SHIFT 0
#define  CISX_INT_CISXSTAT_IDMA_BUSY_MASK_SHIFT(reg) (((reg) & CISX_INT_CISXSTAT_IDMA_BUSY_MASK) >> CISX_INT_CISXSTAT_IDMA_BUSY_SHIFT)
#define  CISX_INT_CISXSTAT_IDMA_BUSY_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CISXSTAT_IDMA_BUSY_MASK) | (((uint32_t)val) << CISX_INT_CISXSTAT_IDMA_BUSY_SHIFT))

//====================================================================
//Register: Channel 0 pixels (Chan0Pix)
//====================================================================

#define  CISX_INT_CHAN0PIX_RESERVED1_MASK 0xfc000000
#define  CISX_INT_CHAN0PIX_RESERVED1_SHIFT 26
#define  CISX_INT_CHAN0PIX_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN0PIX_RESERVED1_MASK) >> CISX_INT_CHAN0PIX_RESERVED1_SHIFT)
#define  CISX_INT_CHAN0PIX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN0PIX_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CHAN0PIX_RESERVED1_SHIFT))

#define  CISX_INT_CHAN0PIX_CHAN0DUM_MASK 0x3ff0000
#define  CISX_INT_CHAN0PIX_CHAN0DUM_SHIFT 16
#define  CISX_INT_CHAN0PIX_CHAN0DUM_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN0PIX_CHAN0DUM_MASK) >> CISX_INT_CHAN0PIX_CHAN0DUM_SHIFT)
#define  CISX_INT_CHAN0PIX_CHAN0DUM_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN0PIX_CHAN0DUM_MASK) | (((uint32_t)val) << CISX_INT_CHAN0PIX_CHAN0DUM_SHIFT))

#define  CISX_INT_CHAN0PIX_CHAN0DAT_MASK 0xffff
#define  CISX_INT_CHAN0PIX_CHAN0DAT_SHIFT 0
#define  CISX_INT_CHAN0PIX_CHAN0DAT_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN0PIX_CHAN0DAT_MASK) >> CISX_INT_CHAN0PIX_CHAN0DAT_SHIFT)
#define  CISX_INT_CHAN0PIX_CHAN0DAT_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN0PIX_CHAN0DAT_MASK) | (((uint32_t)val) << CISX_INT_CHAN0PIX_CHAN0DAT_SHIFT))

//====================================================================
//Register: Channel 1 pixels (Chan1Pix)
//====================================================================

#define  CISX_INT_CHAN1PIX_RESERVED1_MASK 0xfc000000
#define  CISX_INT_CHAN1PIX_RESERVED1_SHIFT 26
#define  CISX_INT_CHAN1PIX_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN1PIX_RESERVED1_MASK) >> CISX_INT_CHAN1PIX_RESERVED1_SHIFT)
#define  CISX_INT_CHAN1PIX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN1PIX_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CHAN1PIX_RESERVED1_SHIFT))

#define  CISX_INT_CHAN1PIX_CHAN1DUM_MASK 0x3ff0000
#define  CISX_INT_CHAN1PIX_CHAN1DUM_SHIFT 16
#define  CISX_INT_CHAN1PIX_CHAN1DUM_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN1PIX_CHAN1DUM_MASK) >> CISX_INT_CHAN1PIX_CHAN1DUM_SHIFT)
#define  CISX_INT_CHAN1PIX_CHAN1DUM_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN1PIX_CHAN1DUM_MASK) | (((uint32_t)val) << CISX_INT_CHAN1PIX_CHAN1DUM_SHIFT))

#define  CISX_INT_CHAN1PIX_CHAN1DAT_MASK 0xffff
#define  CISX_INT_CHAN1PIX_CHAN1DAT_SHIFT 0
#define  CISX_INT_CHAN1PIX_CHAN1DAT_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN1PIX_CHAN1DAT_MASK) >> CISX_INT_CHAN1PIX_CHAN1DAT_SHIFT)
#define  CISX_INT_CHAN1PIX_CHAN1DAT_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN1PIX_CHAN1DAT_MASK) | (((uint32_t)val) << CISX_INT_CHAN1PIX_CHAN1DAT_SHIFT))

//====================================================================
//Register: Channel 2 pixels (Chan2Pix)
//====================================================================

#define  CISX_INT_CHAN2PIX_RESERVED1_MASK 0xfc000000
#define  CISX_INT_CHAN2PIX_RESERVED1_SHIFT 26
#define  CISX_INT_CHAN2PIX_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN2PIX_RESERVED1_MASK) >> CISX_INT_CHAN2PIX_RESERVED1_SHIFT)
#define  CISX_INT_CHAN2PIX_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN2PIX_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CHAN2PIX_RESERVED1_SHIFT))

#define  CISX_INT_CHAN2PIX_CHAN2DUM_MASK 0x3ff0000
#define  CISX_INT_CHAN2PIX_CHAN2DUM_SHIFT 16
#define  CISX_INT_CHAN2PIX_CHAN2DUM_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN2PIX_CHAN2DUM_MASK) >> CISX_INT_CHAN2PIX_CHAN2DUM_SHIFT)
#define  CISX_INT_CHAN2PIX_CHAN2DUM_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN2PIX_CHAN2DUM_MASK) | (((uint32_t)val) << CISX_INT_CHAN2PIX_CHAN2DUM_SHIFT))

#define  CISX_INT_CHAN2PIX_CHAN2DAT_MASK 0xffff
#define  CISX_INT_CHAN2PIX_CHAN2DAT_SHIFT 0
#define  CISX_INT_CHAN2PIX_CHAN2DAT_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN2PIX_CHAN2DAT_MASK) >> CISX_INT_CHAN2PIX_CHAN2DAT_SHIFT)
#define  CISX_INT_CHAN2PIX_CHAN2DAT_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN2PIX_CHAN2DAT_MASK) | (((uint32_t)val) << CISX_INT_CHAN2PIX_CHAN2DAT_SHIFT))

//====================================================================
//Register: Channel 0 Shuffle Separation Pixels (Chan0Shuffle)
//====================================================================

#define  CISX_INT_CHAN0SHUFFLE_RESERVED1_MASK 0xfffffc00
#define  CISX_INT_CHAN0SHUFFLE_RESERVED1_SHIFT 10
#define  CISX_INT_CHAN0SHUFFLE_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN0SHUFFLE_RESERVED1_MASK) >> CISX_INT_CHAN0SHUFFLE_RESERVED1_SHIFT)
#define  CISX_INT_CHAN0SHUFFLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN0SHUFFLE_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CHAN0SHUFFLE_RESERVED1_SHIFT))

#define  CISX_INT_CHAN0SHUFFLE_CHAN0SEP_MASK 0x3ff
#define  CISX_INT_CHAN0SHUFFLE_CHAN0SEP_SHIFT 0
#define  CISX_INT_CHAN0SHUFFLE_CHAN0SEP_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN0SHUFFLE_CHAN0SEP_MASK) >> CISX_INT_CHAN0SHUFFLE_CHAN0SEP_SHIFT)
#define  CISX_INT_CHAN0SHUFFLE_CHAN0SEP_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN0SHUFFLE_CHAN0SEP_MASK) | (((uint32_t)val) << CISX_INT_CHAN0SHUFFLE_CHAN0SEP_SHIFT))

//====================================================================
//Register: Channel 1 Shuffle Separation Pixels (Chan1Shuffle)
//====================================================================

#define  CISX_INT_CHAN1SHUFFLE_RESERVED1_MASK 0xfffffc00
#define  CISX_INT_CHAN1SHUFFLE_RESERVED1_SHIFT 10
#define  CISX_INT_CHAN1SHUFFLE_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN1SHUFFLE_RESERVED1_MASK) >> CISX_INT_CHAN1SHUFFLE_RESERVED1_SHIFT)
#define  CISX_INT_CHAN1SHUFFLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN1SHUFFLE_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CHAN1SHUFFLE_RESERVED1_SHIFT))

#define  CISX_INT_CHAN1SHUFFLE_CHAN1SEP_MASK 0x3ff
#define  CISX_INT_CHAN1SHUFFLE_CHAN1SEP_SHIFT 0
#define  CISX_INT_CHAN1SHUFFLE_CHAN1SEP_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN1SHUFFLE_CHAN1SEP_MASK) >> CISX_INT_CHAN1SHUFFLE_CHAN1SEP_SHIFT)
#define  CISX_INT_CHAN1SHUFFLE_CHAN1SEP_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN1SHUFFLE_CHAN1SEP_MASK) | (((uint32_t)val) << CISX_INT_CHAN1SHUFFLE_CHAN1SEP_SHIFT))

//====================================================================
//Register: Channel 2 Shuffle Separation Pixels (Chan2Shuffle)
//====================================================================

#define  CISX_INT_CHAN2SHUFFLE_RESERVED1_MASK 0xfffffc00
#define  CISX_INT_CHAN2SHUFFLE_RESERVED1_SHIFT 10
#define  CISX_INT_CHAN2SHUFFLE_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN2SHUFFLE_RESERVED1_MASK) >> CISX_INT_CHAN2SHUFFLE_RESERVED1_SHIFT)
#define  CISX_INT_CHAN2SHUFFLE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN2SHUFFLE_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_CHAN2SHUFFLE_RESERVED1_SHIFT))

#define  CISX_INT_CHAN2SHUFFLE_CHAN2SEP_MASK 0x3ff
#define  CISX_INT_CHAN2SHUFFLE_CHAN2SEP_SHIFT 0
#define  CISX_INT_CHAN2SHUFFLE_CHAN2SEP_MASK_SHIFT(reg) (((reg) & CISX_INT_CHAN2SHUFFLE_CHAN2SEP_MASK) >> CISX_INT_CHAN2SHUFFLE_CHAN2SEP_SHIFT)
#define  CISX_INT_CHAN2SHUFFLE_CHAN2SEP_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_CHAN2SHUFFLE_CHAN2SEP_MASK) | (((uint32_t)val) << CISX_INT_CHAN2SHUFFLE_CHAN2SEP_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (IntEn)
//====================================================================

#define  CISX_INT_INTEN_RESERVED1_MASK 0xfffffc00
#define  CISX_INT_INTEN_RESERVED1_SHIFT 10
#define  CISX_INT_INTEN_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_RESERVED1_MASK) >> CISX_INT_INTEN_RESERVED1_SHIFT)
#define  CISX_INT_INTEN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_INTEN_RESERVED1_SHIFT))

#define  CISX_INT_INTEN_IMAGEDONE_EN_MASK 0x200
#define  CISX_INT_INTEN_IMAGEDONE_EN_SHIFT 9
#define  CISX_INT_INTEN_IMAGEDONE_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_IMAGEDONE_EN_MASK) >> CISX_INT_INTEN_IMAGEDONE_EN_SHIFT)
#define  CISX_INT_INTEN_IMAGEDONE_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_IMAGEDONE_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_IMAGEDONE_EN_SHIFT))

#define  CISX_INT_INTEN_ODMA2_IRQ_EN_MASK 0x100
#define  CISX_INT_INTEN_ODMA2_IRQ_EN_SHIFT 8
#define  CISX_INT_INTEN_ODMA2_IRQ_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_ODMA2_IRQ_EN_MASK) >> CISX_INT_INTEN_ODMA2_IRQ_EN_SHIFT)
#define  CISX_INT_INTEN_ODMA2_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_ODMA2_IRQ_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_ODMA2_IRQ_EN_SHIFT))

#define  CISX_INT_INTEN_ODMA1_IRQ_EN_MASK 0x80
#define  CISX_INT_INTEN_ODMA1_IRQ_EN_SHIFT 7
#define  CISX_INT_INTEN_ODMA1_IRQ_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_ODMA1_IRQ_EN_MASK) >> CISX_INT_INTEN_ODMA1_IRQ_EN_SHIFT)
#define  CISX_INT_INTEN_ODMA1_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_ODMA1_IRQ_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_ODMA1_IRQ_EN_SHIFT))

#define  CISX_INT_INTEN_ODMA0_IRQ_EN_MASK 0x40
#define  CISX_INT_INTEN_ODMA0_IRQ_EN_SHIFT 6
#define  CISX_INT_INTEN_ODMA0_IRQ_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_ODMA0_IRQ_EN_MASK) >> CISX_INT_INTEN_ODMA0_IRQ_EN_SHIFT)
#define  CISX_INT_INTEN_ODMA0_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_ODMA0_IRQ_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_ODMA0_IRQ_EN_SHIFT))

#define  CISX_INT_INTEN_IDMA1_IRQ_EN_MASK 0x20
#define  CISX_INT_INTEN_IDMA1_IRQ_EN_SHIFT 5
#define  CISX_INT_INTEN_IDMA1_IRQ_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_IDMA1_IRQ_EN_MASK) >> CISX_INT_INTEN_IDMA1_IRQ_EN_SHIFT)
#define  CISX_INT_INTEN_IDMA1_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_IDMA1_IRQ_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_IDMA1_IRQ_EN_SHIFT))

#define  CISX_INT_INTEN_IDMA0_IRQ_EN_MASK 0x10
#define  CISX_INT_INTEN_IDMA0_IRQ_EN_SHIFT 4
#define  CISX_INT_INTEN_IDMA0_IRQ_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_IDMA0_IRQ_EN_MASK) >> CISX_INT_INTEN_IDMA0_IRQ_EN_SHIFT)
#define  CISX_INT_INTEN_IDMA0_IRQ_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_IDMA0_IRQ_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_IDMA0_IRQ_EN_SHIFT))

#define  CISX_INT_INTEN_DMABUSERR_EN_MASK 0x8
#define  CISX_INT_INTEN_DMABUSERR_EN_SHIFT 3
#define  CISX_INT_INTEN_DMABUSERR_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_DMABUSERR_EN_MASK) >> CISX_INT_INTEN_DMABUSERR_EN_SHIFT)
#define  CISX_INT_INTEN_DMABUSERR_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_DMABUSERR_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_DMABUSERR_EN_SHIFT))

#define  CISX_INT_INTEN_ODMA2_OVERRUN_EN_MASK 0x4
#define  CISX_INT_INTEN_ODMA2_OVERRUN_EN_SHIFT 2
#define  CISX_INT_INTEN_ODMA2_OVERRUN_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_ODMA2_OVERRUN_EN_MASK) >> CISX_INT_INTEN_ODMA2_OVERRUN_EN_SHIFT)
#define  CISX_INT_INTEN_ODMA2_OVERRUN_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_ODMA2_OVERRUN_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_ODMA2_OVERRUN_EN_SHIFT))

#define  CISX_INT_INTEN_ODMA1_OVERRUN_EN_MASK 0x2
#define  CISX_INT_INTEN_ODMA1_OVERRUN_EN_SHIFT 1
#define  CISX_INT_INTEN_ODMA1_OVERRUN_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_ODMA1_OVERRUN_EN_MASK) >> CISX_INT_INTEN_ODMA1_OVERRUN_EN_SHIFT)
#define  CISX_INT_INTEN_ODMA1_OVERRUN_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_ODMA1_OVERRUN_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_ODMA1_OVERRUN_EN_SHIFT))

#define  CISX_INT_INTEN_ODMA0_OVERRUN_EN_MASK 0x1
#define  CISX_INT_INTEN_ODMA0_OVERRUN_EN_SHIFT 0
#define  CISX_INT_INTEN_ODMA0_OVERRUN_EN_MASK_SHIFT(reg) (((reg) & CISX_INT_INTEN_ODMA0_OVERRUN_EN_MASK) >> CISX_INT_INTEN_ODMA0_OVERRUN_EN_SHIFT)
#define  CISX_INT_INTEN_ODMA0_OVERRUN_EN_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTEN_ODMA0_OVERRUN_EN_MASK) | (((uint32_t)val) << CISX_INT_INTEN_ODMA0_OVERRUN_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (IntPend)
//====================================================================

#define  CISX_INT_INTPEND_RESERVED1_MASK 0xfffffc00
#define  CISX_INT_INTPEND_RESERVED1_SHIFT 10
#define  CISX_INT_INTPEND_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_RESERVED1_MASK) >> CISX_INT_INTPEND_RESERVED1_SHIFT)
#define  CISX_INT_INTPEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_RESERVED1_SHIFT))

#define  CISX_INT_INTPEND_IMAGEDONE_PEND_MASK 0x200
#define  CISX_INT_INTPEND_IMAGEDONE_PEND_SHIFT 9
#define  CISX_INT_INTPEND_IMAGEDONE_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_IMAGEDONE_PEND_MASK) >> CISX_INT_INTPEND_IMAGEDONE_PEND_SHIFT)
#define  CISX_INT_INTPEND_IMAGEDONE_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_IMAGEDONE_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_IMAGEDONE_PEND_SHIFT))

#define  CISX_INT_INTPEND_ODMA2_IRQ_PEND_MASK 0x100
#define  CISX_INT_INTPEND_ODMA2_IRQ_PEND_SHIFT 8
#define  CISX_INT_INTPEND_ODMA2_IRQ_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_ODMA2_IRQ_PEND_MASK) >> CISX_INT_INTPEND_ODMA2_IRQ_PEND_SHIFT)
#define  CISX_INT_INTPEND_ODMA2_IRQ_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_ODMA2_IRQ_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_ODMA2_IRQ_PEND_SHIFT))

#define  CISX_INT_INTPEND_ODMA1_IRQ_PEND_MASK 0x80
#define  CISX_INT_INTPEND_ODMA1_IRQ_PEND_SHIFT 7
#define  CISX_INT_INTPEND_ODMA1_IRQ_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_ODMA1_IRQ_PEND_MASK) >> CISX_INT_INTPEND_ODMA1_IRQ_PEND_SHIFT)
#define  CISX_INT_INTPEND_ODMA1_IRQ_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_ODMA1_IRQ_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_ODMA1_IRQ_PEND_SHIFT))

#define  CISX_INT_INTPEND_ODMA0_IRQ_PEND_MASK 0x40
#define  CISX_INT_INTPEND_ODMA0_IRQ_PEND_SHIFT 6
#define  CISX_INT_INTPEND_ODMA0_IRQ_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_ODMA0_IRQ_PEND_MASK) >> CISX_INT_INTPEND_ODMA0_IRQ_PEND_SHIFT)
#define  CISX_INT_INTPEND_ODMA0_IRQ_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_ODMA0_IRQ_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_ODMA0_IRQ_PEND_SHIFT))

#define  CISX_INT_INTPEND_IDMA1_IRQ_PEND_MASK 0x20
#define  CISX_INT_INTPEND_IDMA1_IRQ_PEND_SHIFT 5
#define  CISX_INT_INTPEND_IDMA1_IRQ_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_IDMA1_IRQ_PEND_MASK) >> CISX_INT_INTPEND_IDMA1_IRQ_PEND_SHIFT)
#define  CISX_INT_INTPEND_IDMA1_IRQ_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_IDMA1_IRQ_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_IDMA1_IRQ_PEND_SHIFT))

#define  CISX_INT_INTPEND_IDMA0_IRQ_PEND_MASK 0x10
#define  CISX_INT_INTPEND_IDMA0_IRQ_PEND_SHIFT 4
#define  CISX_INT_INTPEND_IDMA0_IRQ_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_IDMA0_IRQ_PEND_MASK) >> CISX_INT_INTPEND_IDMA0_IRQ_PEND_SHIFT)
#define  CISX_INT_INTPEND_IDMA0_IRQ_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_IDMA0_IRQ_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_IDMA0_IRQ_PEND_SHIFT))

#define  CISX_INT_INTPEND_DMABUSERR_PEND_MASK 0x8
#define  CISX_INT_INTPEND_DMABUSERR_PEND_SHIFT 3
#define  CISX_INT_INTPEND_DMABUSERR_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_DMABUSERR_PEND_MASK) >> CISX_INT_INTPEND_DMABUSERR_PEND_SHIFT)
#define  CISX_INT_INTPEND_DMABUSERR_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_DMABUSERR_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_DMABUSERR_PEND_SHIFT))

#define  CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_MASK 0x4
#define  CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_SHIFT 2
#define  CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_MASK) >> CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_SHIFT)
#define  CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_ODMA2_OVERRUN_PEND_SHIFT))

#define  CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_MASK 0x2
#define  CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_SHIFT 1
#define  CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_MASK) >> CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_SHIFT)
#define  CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_ODMA1_OVERRUN_PEND_SHIFT))

#define  CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_MASK 0x1
#define  CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_SHIFT 0
#define  CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_MASK_SHIFT(reg) (((reg) & CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_MASK) >> CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_SHIFT)
#define  CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_MASK) | (((uint32_t)val) << CISX_INT_INTPEND_ODMA0_OVERRUN_PEND_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (IntAck)
//====================================================================

#define  CISX_INT_INTACK_RESERVED1_MASK 0xfffffc00
#define  CISX_INT_INTACK_RESERVED1_SHIFT 10
#define  CISX_INT_INTACK_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_RESERVED1_MASK) >> CISX_INT_INTACK_RESERVED1_SHIFT)
#define  CISX_INT_INTACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_INTACK_RESERVED1_SHIFT))

#define  CISX_INT_INTACK_IMAGEDONE_ACK_MASK 0x200
#define  CISX_INT_INTACK_IMAGEDONE_ACK_SHIFT 9
#define  CISX_INT_INTACK_IMAGEDONE_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_IMAGEDONE_ACK_MASK) >> CISX_INT_INTACK_IMAGEDONE_ACK_SHIFT)
#define  CISX_INT_INTACK_IMAGEDONE_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_IMAGEDONE_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_IMAGEDONE_ACK_SHIFT))

#define  CISX_INT_INTACK_ODMA2_IRQ_ACK_MASK 0x100
#define  CISX_INT_INTACK_ODMA2_IRQ_ACK_SHIFT 8
#define  CISX_INT_INTACK_ODMA2_IRQ_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_ODMA2_IRQ_ACK_MASK) >> CISX_INT_INTACK_ODMA2_IRQ_ACK_SHIFT)
#define  CISX_INT_INTACK_ODMA2_IRQ_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_ODMA2_IRQ_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_ODMA2_IRQ_ACK_SHIFT))

#define  CISX_INT_INTACK_ODMA1_IRQ_ACK_MASK 0x80
#define  CISX_INT_INTACK_ODMA1_IRQ_ACK_SHIFT 7
#define  CISX_INT_INTACK_ODMA1_IRQ_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_ODMA1_IRQ_ACK_MASK) >> CISX_INT_INTACK_ODMA1_IRQ_ACK_SHIFT)
#define  CISX_INT_INTACK_ODMA1_IRQ_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_ODMA1_IRQ_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_ODMA1_IRQ_ACK_SHIFT))

#define  CISX_INT_INTACK_ODMA0_IRQ_ACK_MASK 0x40
#define  CISX_INT_INTACK_ODMA0_IRQ_ACK_SHIFT 6
#define  CISX_INT_INTACK_ODMA0_IRQ_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_ODMA0_IRQ_ACK_MASK) >> CISX_INT_INTACK_ODMA0_IRQ_ACK_SHIFT)
#define  CISX_INT_INTACK_ODMA0_IRQ_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_ODMA0_IRQ_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_ODMA0_IRQ_ACK_SHIFT))

#define  CISX_INT_INTACK_IDMA1_IRQ_ACK_MASK 0x20
#define  CISX_INT_INTACK_IDMA1_IRQ_ACK_SHIFT 5
#define  CISX_INT_INTACK_IDMA1_IRQ_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_IDMA1_IRQ_ACK_MASK) >> CISX_INT_INTACK_IDMA1_IRQ_ACK_SHIFT)
#define  CISX_INT_INTACK_IDMA1_IRQ_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_IDMA1_IRQ_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_IDMA1_IRQ_ACK_SHIFT))

#define  CISX_INT_INTACK_IDMA0_IRQ_ACK_MASK 0x10
#define  CISX_INT_INTACK_IDMA0_IRQ_ACK_SHIFT 4
#define  CISX_INT_INTACK_IDMA0_IRQ_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_IDMA0_IRQ_ACK_MASK) >> CISX_INT_INTACK_IDMA0_IRQ_ACK_SHIFT)
#define  CISX_INT_INTACK_IDMA0_IRQ_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_IDMA0_IRQ_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_IDMA0_IRQ_ACK_SHIFT))

#define  CISX_INT_INTACK_DMABUSERR_ACK_MASK 0x8
#define  CISX_INT_INTACK_DMABUSERR_ACK_SHIFT 3
#define  CISX_INT_INTACK_DMABUSERR_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_DMABUSERR_ACK_MASK) >> CISX_INT_INTACK_DMABUSERR_ACK_SHIFT)
#define  CISX_INT_INTACK_DMABUSERR_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_DMABUSERR_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_DMABUSERR_ACK_SHIFT))

#define  CISX_INT_INTACK_ODMA2_OVERRUN_ACK_MASK 0x4
#define  CISX_INT_INTACK_ODMA2_OVERRUN_ACK_SHIFT 2
#define  CISX_INT_INTACK_ODMA2_OVERRUN_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_ODMA2_OVERRUN_ACK_MASK) >> CISX_INT_INTACK_ODMA2_OVERRUN_ACK_SHIFT)
#define  CISX_INT_INTACK_ODMA2_OVERRUN_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_ODMA2_OVERRUN_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_ODMA2_OVERRUN_ACK_SHIFT))

#define  CISX_INT_INTACK_ODMA1_OVERRUN_ACK_MASK 0x2
#define  CISX_INT_INTACK_ODMA1_OVERRUN_ACK_SHIFT 1
#define  CISX_INT_INTACK_ODMA1_OVERRUN_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_ODMA1_OVERRUN_ACK_MASK) >> CISX_INT_INTACK_ODMA1_OVERRUN_ACK_SHIFT)
#define  CISX_INT_INTACK_ODMA1_OVERRUN_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_ODMA1_OVERRUN_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_ODMA1_OVERRUN_ACK_SHIFT))

#define  CISX_INT_INTACK_ODMA0_OVERRUN_ACK_MASK 0x1
#define  CISX_INT_INTACK_ODMA0_OVERRUN_ACK_SHIFT 0
#define  CISX_INT_INTACK_ODMA0_OVERRUN_ACK_MASK_SHIFT(reg) (((reg) & CISX_INT_INTACK_ODMA0_OVERRUN_ACK_MASK) >> CISX_INT_INTACK_ODMA0_OVERRUN_ACK_SHIFT)
#define  CISX_INT_INTACK_ODMA0_OVERRUN_ACK_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTACK_ODMA0_OVERRUN_ACK_MASK) | (((uint32_t)val) << CISX_INT_INTACK_ODMA0_OVERRUN_ACK_SHIFT))

//====================================================================
//Register: Interrupt Force Register (IntForce)
//====================================================================

#define  CISX_INT_INTFORCE_RESERVED1_MASK 0xfffffc00
#define  CISX_INT_INTFORCE_RESERVED1_SHIFT 10
#define  CISX_INT_INTFORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_RESERVED1_MASK) >> CISX_INT_INTFORCE_RESERVED1_SHIFT)
#define  CISX_INT_INTFORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_RESERVED1_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_RESERVED1_SHIFT))

#define  CISX_INT_INTFORCE_IMAGEDONE_FORCE_MASK 0x200
#define  CISX_INT_INTFORCE_IMAGEDONE_FORCE_SHIFT 9
#define  CISX_INT_INTFORCE_IMAGEDONE_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_IMAGEDONE_FORCE_MASK) >> CISX_INT_INTFORCE_IMAGEDONE_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_IMAGEDONE_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_IMAGEDONE_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_IMAGEDONE_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_MASK 0x100
#define  CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_SHIFT 8
#define  CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_MASK) >> CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_ODMA2_IRQ_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_MASK 0x80
#define  CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_SHIFT 7
#define  CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_MASK) >> CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_ODMA1_IRQ_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_MASK 0x40
#define  CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_SHIFT 6
#define  CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_MASK) >> CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_ODMA0_IRQ_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_MASK 0x20
#define  CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_SHIFT 5
#define  CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_MASK) >> CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_IDMA1_IRQ_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_MASK 0x10
#define  CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_SHIFT 4
#define  CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_MASK) >> CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_IDMA0_IRQ_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_DMABUSERR_FORCE_MASK 0x8
#define  CISX_INT_INTFORCE_DMABUSERR_FORCE_SHIFT 3
#define  CISX_INT_INTFORCE_DMABUSERR_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_DMABUSERR_FORCE_MASK) >> CISX_INT_INTFORCE_DMABUSERR_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_DMABUSERR_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_DMABUSERR_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_DMABUSERR_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_MASK 0x4
#define  CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_SHIFT 2
#define  CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_MASK) >> CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_ODMA2_OVERRUN_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_MASK 0x2
#define  CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_SHIFT 1
#define  CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_MASK) >> CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_ODMA1_OVERRUN_FORCE_SHIFT))

#define  CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_MASK 0x1
#define  CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_SHIFT 0
#define  CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_MASK_SHIFT(reg) (((reg) & CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_MASK) >> CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_SHIFT)
#define  CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_REPLACE_VAL(reg,val) (((reg) & ~CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_MASK) | (((uint32_t)val) << CISX_INT_INTFORCE_ODMA0_OVERRUN_FORCE_SHIFT))

#endif // CISX_INT
