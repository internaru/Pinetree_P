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
 * \file apb_ddac_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _APB_DDAC_REG_REGMASKS_H_
#define _APB_DDAC_REG_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Audio DDAC Registers (apb_ddac_reg)
/** \brief Registers for the audio DDAC module*/
//
//====================================================================

//====================================================================
//Register: Audio Output Configuration Register (AOCR)
/** \brief This register contains various bit fields to configure the audio output*/
//====================================================================

#define  APB_DDAC_REG_AOCR_FF_VOLUME_MASK 0xff000000
#define  APB_DDAC_REG_AOCR_FF_VOLUME_SHIFT 24
#define  APB_DDAC_REG_AOCR_FF_VOLUME_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR_FF_VOLUME_MASK) >> APB_DDAC_REG_AOCR_FF_VOLUME_SHIFT)
#define  APB_DDAC_REG_AOCR_FF_VOLUME_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR_FF_VOLUME_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR_FF_VOLUME_SHIFT))

#define  APB_DDAC_REG_AOCR_RESERVED1_MASK 0xc00000
#define  APB_DDAC_REG_AOCR_RESERVED1_SHIFT 22
#define  APB_DDAC_REG_AOCR_RESERVED1_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR_RESERVED1_MASK) >> APB_DDAC_REG_AOCR_RESERVED1_SHIFT)
#define  APB_DDAC_REG_AOCR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR_RESERVED1_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR_RESERVED1_SHIFT))

#define  APB_DDAC_REG_AOCR_DA_DATASHIFT_MASK 0x300000
#define  APB_DDAC_REG_AOCR_DA_DATASHIFT_SHIFT 20
#define  APB_DDAC_REG_AOCR_DA_DATASHIFT_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR_DA_DATASHIFT_MASK) >> APB_DDAC_REG_AOCR_DA_DATASHIFT_SHIFT)
#define  APB_DDAC_REG_AOCR_DA_DATASHIFT_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR_DA_DATASHIFT_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR_DA_DATASHIFT_SHIFT))

#define  APB_DDAC_REG_AOCR_DA_RESOLUTION_MASK 0x80000
#define  APB_DDAC_REG_AOCR_DA_RESOLUTION_SHIFT 19
#define  APB_DDAC_REG_AOCR_DA_RESOLUTION_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR_DA_RESOLUTION_MASK) >> APB_DDAC_REG_AOCR_DA_RESOLUTION_SHIFT)
#define  APB_DDAC_REG_AOCR_DA_RESOLUTION_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR_DA_RESOLUTION_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR_DA_RESOLUTION_SHIFT))

#define  APB_DDAC_REG_AOCR_OUTPUTSEL_MASK 0x70000
#define  APB_DDAC_REG_AOCR_OUTPUTSEL_SHIFT 16
#define  APB_DDAC_REG_AOCR_OUTPUTSEL_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR_OUTPUTSEL_MASK) >> APB_DDAC_REG_AOCR_OUTPUTSEL_SHIFT)
#define  APB_DDAC_REG_AOCR_OUTPUTSEL_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR_OUTPUTSEL_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR_OUTPUTSEL_SHIFT))

#define  APB_DDAC_REG_AOCR_RESERVED2_MASK 0xf000
#define  APB_DDAC_REG_AOCR_RESERVED2_SHIFT 12
#define  APB_DDAC_REG_AOCR_RESERVED2_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR_RESERVED2_MASK) >> APB_DDAC_REG_AOCR_RESERVED2_SHIFT)
#define  APB_DDAC_REG_AOCR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR_RESERVED2_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR_RESERVED2_SHIFT))

#define  APB_DDAC_REG_AOCR_SAMPDIVISOR_MASK 0xfff
#define  APB_DDAC_REG_AOCR_SAMPDIVISOR_SHIFT 0
#define  APB_DDAC_REG_AOCR_SAMPDIVISOR_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR_SAMPDIVISOR_MASK) >> APB_DDAC_REG_AOCR_SAMPDIVISOR_SHIFT)
#define  APB_DDAC_REG_AOCR_SAMPDIVISOR_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR_SAMPDIVISOR_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR_SAMPDIVISOR_SHIFT))

//====================================================================
//Register: Audio Output Data Register (AODR)
/** \brief This register is enabled when the audio output is configured for D/A mode (AOCR.outputSel=001 or 010). This register is not used for the fixed-frequency output modes (AOCR.outputSel=100, 101 and 110)]. <BR>
      When enabled and configured for 8 bit resolution, writing a word to this register will load four D/A sample bytes; when configured for 10 bit resolution, writing a word to this register will load two 16 bit D/A samples, and hardware will use the upper 10 bits for delta-sigma or pwm averaging. <BR>
     Once the first 3 samples have been loaded into the FIFO (becomes full), the block starts reading byte samples from the FIFO and outputting audio, and continues to read from the FIFO until audio is disabled or switched to fixed-frequency output mode. <BR>
     This Output Data register should be updated when the Ready bit is set in the Audio Output Status Register (AOSR), or alternatively when the interrupt is asserted. <BR>
     When using DMA, this register address is programmed as the destination.*/
//====================================================================

#define  APB_DDAC_REG_AODR_DATA_MASK 0xffffffff
#define  APB_DDAC_REG_AODR_DATA_SHIFT 0
#define  APB_DDAC_REG_AODR_DATA_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AODR_DATA_MASK) >> APB_DDAC_REG_AODR_DATA_SHIFT)
#define  APB_DDAC_REG_AODR_DATA_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AODR_DATA_MASK) | (((uint32_t)val) << APB_DDAC_REG_AODR_DATA_SHIFT))

//====================================================================
//Register: Audio Output Status Register (AOSR)
/** \brief This register indicates when the D/A is ready for the next data value. <BR>
    The Ready bit becomes true (1) when the D/A data FIFO is ready to accept the next data value. When the Ready bit is true, the interrupt request is also asserted. <BR>
    The Ready bit is cleared when the Audio Output Data Register (AODR) is written.*/
//====================================================================

#define  APB_DDAC_REG_AOSR_RESERVED1_MASK 0xfffc0000
#define  APB_DDAC_REG_AOSR_RESERVED1_SHIFT 18
#define  APB_DDAC_REG_AOSR_RESERVED1_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_RESERVED1_MASK) >> APB_DDAC_REG_AOSR_RESERVED1_SHIFT)
#define  APB_DDAC_REG_AOSR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_RESERVED1_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_RESERVED1_SHIFT))

#define  APB_DDAC_REG_AOSR_DATAHOLD_MASK 0x3ff00
#define  APB_DDAC_REG_AOSR_DATAHOLD_SHIFT 8
#define  APB_DDAC_REG_AOSR_DATAHOLD_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_DATAHOLD_MASK) >> APB_DDAC_REG_AOSR_DATAHOLD_SHIFT)
#define  APB_DDAC_REG_AOSR_DATAHOLD_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_DATAHOLD_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_DATAHOLD_SHIFT))

#define  APB_DDAC_REG_AOSR_RESERVED2_MASK 0x80
#define  APB_DDAC_REG_AOSR_RESERVED2_SHIFT 7
#define  APB_DDAC_REG_AOSR_RESERVED2_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_RESERVED2_MASK) >> APB_DDAC_REG_AOSR_RESERVED2_SHIFT)
#define  APB_DDAC_REG_AOSR_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_RESERVED2_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_RESERVED2_SHIFT))

#define  APB_DDAC_REG_AOSR_FIFOREADEN_MASK 0x40
#define  APB_DDAC_REG_AOSR_FIFOREADEN_SHIFT 6
#define  APB_DDAC_REG_AOSR_FIFOREADEN_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_FIFOREADEN_MASK) >> APB_DDAC_REG_AOSR_FIFOREADEN_SHIFT)
#define  APB_DDAC_REG_AOSR_FIFOREADEN_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_FIFOREADEN_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_FIFOREADEN_SHIFT))

#define  APB_DDAC_REG_AOSR_FIFOCLEAR_MASK 0x20
#define  APB_DDAC_REG_AOSR_FIFOCLEAR_SHIFT 5
#define  APB_DDAC_REG_AOSR_FIFOCLEAR_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_FIFOCLEAR_MASK) >> APB_DDAC_REG_AOSR_FIFOCLEAR_SHIFT)
#define  APB_DDAC_REG_AOSR_FIFOCLEAR_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_FIFOCLEAR_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_FIFOCLEAR_SHIFT))

#define  APB_DDAC_REG_AOSR_FIFOFULL_MASK 0x10
#define  APB_DDAC_REG_AOSR_FIFOFULL_SHIFT 4
#define  APB_DDAC_REG_AOSR_FIFOFULL_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_FIFOFULL_MASK) >> APB_DDAC_REG_AOSR_FIFOFULL_SHIFT)
#define  APB_DDAC_REG_AOSR_FIFOFULL_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_FIFOFULL_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_FIFOFULL_SHIFT))

#define  APB_DDAC_REG_AOSR_FIFOEMPTY_MASK 0x8
#define  APB_DDAC_REG_AOSR_FIFOEMPTY_SHIFT 3
#define  APB_DDAC_REG_AOSR_FIFOEMPTY_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_FIFOEMPTY_MASK) >> APB_DDAC_REG_AOSR_FIFOEMPTY_SHIFT)
#define  APB_DDAC_REG_AOSR_FIFOEMPTY_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_FIFOEMPTY_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_FIFOEMPTY_SHIFT))

#define  APB_DDAC_REG_AOSR_FIFOOVERRUN_MASK 0x4
#define  APB_DDAC_REG_AOSR_FIFOOVERRUN_SHIFT 2
#define  APB_DDAC_REG_AOSR_FIFOOVERRUN_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_FIFOOVERRUN_MASK) >> APB_DDAC_REG_AOSR_FIFOOVERRUN_SHIFT)
#define  APB_DDAC_REG_AOSR_FIFOOVERRUN_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_FIFOOVERRUN_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_FIFOOVERRUN_SHIFT))

#define  APB_DDAC_REG_AOSR_FIFOUNDERRUN_MASK 0x2
#define  APB_DDAC_REG_AOSR_FIFOUNDERRUN_SHIFT 1
#define  APB_DDAC_REG_AOSR_FIFOUNDERRUN_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_FIFOUNDERRUN_MASK) >> APB_DDAC_REG_AOSR_FIFOUNDERRUN_SHIFT)
#define  APB_DDAC_REG_AOSR_FIFOUNDERRUN_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_FIFOUNDERRUN_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_FIFOUNDERRUN_SHIFT))

#define  APB_DDAC_REG_AOSR_READY_MASK 0x1
#define  APB_DDAC_REG_AOSR_READY_SHIFT 0
#define  APB_DDAC_REG_AOSR_READY_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOSR_READY_MASK) >> APB_DDAC_REG_AOSR_READY_SHIFT)
#define  APB_DDAC_REG_AOSR_READY_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOSR_READY_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOSR_READY_SHIFT))

//====================================================================
//Register: Audio Output Configuration2 Register (AOCR2)
/** \brief This register contains additional bit fields to configure the audio output*/
//====================================================================

#define  APB_DDAC_REG_AOCR2_RESERVED1_MASK 0xffffff00
#define  APB_DDAC_REG_AOCR2_RESERVED1_SHIFT 8
#define  APB_DDAC_REG_AOCR2_RESERVED1_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR2_RESERVED1_MASK) >> APB_DDAC_REG_AOCR2_RESERVED1_SHIFT)
#define  APB_DDAC_REG_AOCR2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR2_RESERVED1_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR2_RESERVED1_SHIFT))

#define  APB_DDAC_REG_AOCR2_DSCLKDIVISOR_MASK 0xff
#define  APB_DDAC_REG_AOCR2_DSCLKDIVISOR_SHIFT 0
#define  APB_DDAC_REG_AOCR2_DSCLKDIVISOR_MASK_SHIFT(reg) (((reg) & APB_DDAC_REG_AOCR2_DSCLKDIVISOR_MASK) >> APB_DDAC_REG_AOCR2_DSCLKDIVISOR_SHIFT)
#define  APB_DDAC_REG_AOCR2_DSCLKDIVISOR_REPLACE_VAL(reg,val) (((reg) & ~APB_DDAC_REG_AOCR2_DSCLKDIVISOR_MASK) | (((uint32_t)val) << APB_DDAC_REG_AOCR2_DSCLKDIVISOR_SHIFT))

#endif // APB_DDAC_REG
