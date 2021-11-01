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
 * \file CBI2CDMA_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _CBI2CDMA_REGMASKS_H_
#define _CBI2CDMA_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: CBI2CDMA (CBI2CDMA)
//
//====================================================================

//====================================================================
//Register: Status Register (STATUS)
/** \brief Debug : Used to read the status of the FIFO and DMA handshake.*/
//====================================================================

#define  CBI2CDMA_STATUS_RESERVED1_MASK 0xffffffc0
#define  CBI2CDMA_STATUS_RESERVED1_SHIFT 6
#define  CBI2CDMA_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & CBI2CDMA_STATUS_RESERVED1_MASK) >> CBI2CDMA_STATUS_RESERVED1_SHIFT)
#define  CBI2CDMA_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_STATUS_RESERVED1_MASK) | (((uint32_t)val) << CBI2CDMA_STATUS_RESERVED1_SHIFT))

#define  CBI2CDMA_STATUS_DMA_CLR_MASK 0x20
#define  CBI2CDMA_STATUS_DMA_CLR_SHIFT 5
#define  CBI2CDMA_STATUS_DMA_CLR_MASK_SHIFT(reg) (((reg) & CBI2CDMA_STATUS_DMA_CLR_MASK) >> CBI2CDMA_STATUS_DMA_CLR_SHIFT)
#define  CBI2CDMA_STATUS_DMA_CLR_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_STATUS_DMA_CLR_MASK) | (((uint32_t)val) << CBI2CDMA_STATUS_DMA_CLR_SHIFT))

#define  CBI2CDMA_STATUS_DMA_SREQ_MASK 0x10
#define  CBI2CDMA_STATUS_DMA_SREQ_SHIFT 4
#define  CBI2CDMA_STATUS_DMA_SREQ_MASK_SHIFT(reg) (((reg) & CBI2CDMA_STATUS_DMA_SREQ_MASK) >> CBI2CDMA_STATUS_DMA_SREQ_SHIFT)
#define  CBI2CDMA_STATUS_DMA_SREQ_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_STATUS_DMA_SREQ_MASK) | (((uint32_t)val) << CBI2CDMA_STATUS_DMA_SREQ_SHIFT))

#define  CBI2CDMA_STATUS_QUEUE_READY_MASK 0x8
#define  CBI2CDMA_STATUS_QUEUE_READY_SHIFT 3
#define  CBI2CDMA_STATUS_QUEUE_READY_MASK_SHIFT(reg) (((reg) & CBI2CDMA_STATUS_QUEUE_READY_MASK) >> CBI2CDMA_STATUS_QUEUE_READY_SHIFT)
#define  CBI2CDMA_STATUS_QUEUE_READY_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_STATUS_QUEUE_READY_MASK) | (((uint32_t)val) << CBI2CDMA_STATUS_QUEUE_READY_SHIFT))

#define  CBI2CDMA_STATUS_QUEUE_VALID_MASK 0x4
#define  CBI2CDMA_STATUS_QUEUE_VALID_SHIFT 2
#define  CBI2CDMA_STATUS_QUEUE_VALID_MASK_SHIFT(reg) (((reg) & CBI2CDMA_STATUS_QUEUE_VALID_MASK) >> CBI2CDMA_STATUS_QUEUE_VALID_SHIFT)
#define  CBI2CDMA_STATUS_QUEUE_VALID_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_STATUS_QUEUE_VALID_MASK) | (((uint32_t)val) << CBI2CDMA_STATUS_QUEUE_VALID_SHIFT))

#define  CBI2CDMA_STATUS_IN_READY_MASK 0x2
#define  CBI2CDMA_STATUS_IN_READY_SHIFT 1
#define  CBI2CDMA_STATUS_IN_READY_MASK_SHIFT(reg) (((reg) & CBI2CDMA_STATUS_IN_READY_MASK) >> CBI2CDMA_STATUS_IN_READY_SHIFT)
#define  CBI2CDMA_STATUS_IN_READY_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_STATUS_IN_READY_MASK) | (((uint32_t)val) << CBI2CDMA_STATUS_IN_READY_SHIFT))

#define  CBI2CDMA_STATUS_IN_VALID_MASK 0x1
#define  CBI2CDMA_STATUS_IN_VALID_SHIFT 0
#define  CBI2CDMA_STATUS_IN_VALID_MASK_SHIFT(reg) (((reg) & CBI2CDMA_STATUS_IN_VALID_MASK) >> CBI2CDMA_STATUS_IN_VALID_SHIFT)
#define  CBI2CDMA_STATUS_IN_VALID_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_STATUS_IN_VALID_MASK) | (((uint32_t)val) << CBI2CDMA_STATUS_IN_VALID_SHIFT))

//====================================================================
//Register: DMA Data Register (DATA)
/** \brief This is the register used by CDMA to read the data.*/
//====================================================================

#define  CBI2CDMA_DATA_DATA_MASK 0xffffffff
#define  CBI2CDMA_DATA_DATA_SHIFT 0
#define  CBI2CDMA_DATA_DATA_MASK_SHIFT(reg) (((reg) & CBI2CDMA_DATA_DATA_MASK) >> CBI2CDMA_DATA_DATA_SHIFT)
#define  CBI2CDMA_DATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~CBI2CDMA_DATA_DATA_MASK) | (((uint32_t)val) << CBI2CDMA_DATA_DATA_SHIFT))

#endif // CBI2CDMA
