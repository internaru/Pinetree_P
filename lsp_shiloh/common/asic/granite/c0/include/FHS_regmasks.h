 /*
 *************************************************************************
 *
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *
 *                       Marvell Confidential
 *
 *************************************************************************
 */
/**
 * \file FHS_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _FHS_REGMASKS_H_
#define _FHS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Fractional Horizontal Scale (FHS)
/** \brief Register descriptions for the Fractional Horizontal Scale (FHS) block.
    FHS programming is simple, write the input the output and lastly the control registers inImageWidth, outImageWidth and control, respectively.
    FHS will apply the correct scale factor to get the desired output.
    The write only control register is read via the read only status register.
    FHS will handle four known error conditions:
    1. Bad initialization of the inImageWidth with a value less than two.
    2. Bad initialization of the inImageWidth with a larger value than outImageWidth. (FHS only upscales)
    3. Incorrect inImageWidth with respect to the actual data width, smaller value.
    4. Incorrect inImageWidth with respect to the actual data width, larger value.
    Error cases 1 and 2 are evaluated on the write of the control register, and result in a forced bypass of the scale function.
    Error cases 3 and 4 are determined algorithmically when expected end-of-line/image comes at the wrong time, and result in the FHS either consuming data or generating data to match the outImageWidth.
    For development, control register bit stallOnError may be set which will stall the pipe for error cases 3 and 4 stopping all data flow.
    Interrupts for the error conditions may be enabled in the int_en register, and acknowledged via int_ack register.
    Interrupt pending bits in register int_pend are set for errors regardless of whether or not interrupts are enabled.
    NOTE: The reset value is only valid on system reset or power-up, a soft reset from a higher level block will not affect the registers, as per design guidelines.*/
//
//====================================================================

//====================================================================
//Register: Input Image Pixel Width Register (inImageWidth)
/** \brief Contains the count of the input image pixels in the horizontal direction.*/
//====================================================================

#define  FHS_INIMAGEWIDTH_RESERVED1_MASK 0xffff0000
#define  FHS_INIMAGEWIDTH_RESERVED1_SHIFT 16
#define  FHS_INIMAGEWIDTH_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_INIMAGEWIDTH_RESERVED1_MASK) >> FHS_INIMAGEWIDTH_RESERVED1_SHIFT)
#define  FHS_INIMAGEWIDTH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_INIMAGEWIDTH_RESERVED1_MASK) | (((uint32_t)val) << FHS_INIMAGEWIDTH_RESERVED1_SHIFT))

#define  FHS_INIMAGEWIDTH_INPUTCOUNT_MASK 0xffff
#define  FHS_INIMAGEWIDTH_INPUTCOUNT_SHIFT 0
#define  FHS_INIMAGEWIDTH_INPUTCOUNT_MASK_SHIFT(reg) (((reg) & FHS_INIMAGEWIDTH_INPUTCOUNT_MASK) >> FHS_INIMAGEWIDTH_INPUTCOUNT_SHIFT)
#define  FHS_INIMAGEWIDTH_INPUTCOUNT_REPLACE_VAL(reg,val) (((reg) & ~FHS_INIMAGEWIDTH_INPUTCOUNT_MASK) | (((uint32_t)val) << FHS_INIMAGEWIDTH_INPUTCOUNT_SHIFT))

//====================================================================
//Register: Output Image Pixel Width Register (outImageWidth)
/** \brief Contains the count of the output image pixels in the horizontal direction.*/
//====================================================================

#define  FHS_OUTIMAGEWIDTH_RESERVED1_MASK 0xffff0000
#define  FHS_OUTIMAGEWIDTH_RESERVED1_SHIFT 16
#define  FHS_OUTIMAGEWIDTH_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_OUTIMAGEWIDTH_RESERVED1_MASK) >> FHS_OUTIMAGEWIDTH_RESERVED1_SHIFT)
#define  FHS_OUTIMAGEWIDTH_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_OUTIMAGEWIDTH_RESERVED1_MASK) | (((uint32_t)val) << FHS_OUTIMAGEWIDTH_RESERVED1_SHIFT))

#define  FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_MASK 0xffff
#define  FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_SHIFT 0
#define  FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_MASK_SHIFT(reg) (((reg) & FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_MASK) >> FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_SHIFT)
#define  FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_REPLACE_VAL(reg,val) (((reg) & ~FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_MASK) | (((uint32_t)val) << FHS_OUTIMAGEWIDTH_OUTPUTCOUNT_SHIFT))

//====================================================================
//Register: Control Register (control)
/** \brief Write only control of the Fractional Horizontal Scale (FHS) block. This register is written after changing the values in the inImageWidth or outImageWidth registers.*/
//====================================================================

#define  FHS_CONTROL_BYPASS_MASK 0x80000000
#define  FHS_CONTROL_BYPASS_SHIFT 31
#define  FHS_CONTROL_BYPASS_MASK_SHIFT(reg) (((reg) & FHS_CONTROL_BYPASS_MASK) >> FHS_CONTROL_BYPASS_SHIFT)
#define  FHS_CONTROL_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~FHS_CONTROL_BYPASS_MASK) | (((uint32_t)val) << FHS_CONTROL_BYPASS_SHIFT))

#define  FHS_CONTROL_RESERVED1_MASK 0x7e000000
#define  FHS_CONTROL_RESERVED1_SHIFT 25
#define  FHS_CONTROL_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_CONTROL_RESERVED1_MASK) >> FHS_CONTROL_RESERVED1_SHIFT)
#define  FHS_CONTROL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_CONTROL_RESERVED1_MASK) | (((uint32_t)val) << FHS_CONTROL_RESERVED1_SHIFT))

#define  FHS_CONTROL_STALLONERROR_MASK 0x1000000
#define  FHS_CONTROL_STALLONERROR_SHIFT 24
#define  FHS_CONTROL_STALLONERROR_MASK_SHIFT(reg) (((reg) & FHS_CONTROL_STALLONERROR_MASK) >> FHS_CONTROL_STALLONERROR_SHIFT)
#define  FHS_CONTROL_STALLONERROR_REPLACE_VAL(reg,val) (((reg) & ~FHS_CONTROL_STALLONERROR_MASK) | (((uint32_t)val) << FHS_CONTROL_STALLONERROR_SHIFT))

#define  FHS_CONTROL_RESERVED2_MASK 0xffffff
#define  FHS_CONTROL_RESERVED2_SHIFT 0
#define  FHS_CONTROL_RESERVED2_MASK_SHIFT(reg) (((reg) & FHS_CONTROL_RESERVED2_MASK) >> FHS_CONTROL_RESERVED2_SHIFT)
#define  FHS_CONTROL_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FHS_CONTROL_RESERVED2_MASK) | (((uint32_t)val) << FHS_CONTROL_RESERVED2_SHIFT))

//====================================================================
//Register: Status Register (status)
/** \brief Read only status of the Fractional Horizontal Scale (FHS) block.*/
//====================================================================

#define  FHS_STATUS_BYPASS_MASK 0x80000000
#define  FHS_STATUS_BYPASS_SHIFT 31
#define  FHS_STATUS_BYPASS_MASK_SHIFT(reg) (((reg) & FHS_STATUS_BYPASS_MASK) >> FHS_STATUS_BYPASS_SHIFT)
#define  FHS_STATUS_BYPASS_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_BYPASS_MASK) | (((uint32_t)val) << FHS_STATUS_BYPASS_SHIFT))

#define  FHS_STATUS_RESERVED1_MASK 0x7e000000
#define  FHS_STATUS_RESERVED1_SHIFT 25
#define  FHS_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_STATUS_RESERVED1_MASK) >> FHS_STATUS_RESERVED1_SHIFT)
#define  FHS_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_RESERVED1_MASK) | (((uint32_t)val) << FHS_STATUS_RESERVED1_SHIFT))

#define  FHS_STATUS_STALLONERROR_MASK 0x1000000
#define  FHS_STATUS_STALLONERROR_SHIFT 24
#define  FHS_STATUS_STALLONERROR_MASK_SHIFT(reg) (((reg) & FHS_STATUS_STALLONERROR_MASK) >> FHS_STATUS_STALLONERROR_SHIFT)
#define  FHS_STATUS_STALLONERROR_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_STALLONERROR_MASK) | (((uint32_t)val) << FHS_STATUS_STALLONERROR_SHIFT))

#define  FHS_STATUS_RESERVED2_MASK 0xfc0000
#define  FHS_STATUS_RESERVED2_SHIFT 18
#define  FHS_STATUS_RESERVED2_MASK_SHIFT(reg) (((reg) & FHS_STATUS_RESERVED2_MASK) >> FHS_STATUS_RESERVED2_SHIFT)
#define  FHS_STATUS_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_RESERVED2_MASK) | (((uint32_t)val) << FHS_STATUS_RESERVED2_SHIFT))

#define  FHS_STATUS_DATATYPE_MASK 0x30000
#define  FHS_STATUS_DATATYPE_SHIFT 16
#define  FHS_STATUS_DATATYPE_MASK_SHIFT(reg) (((reg) & FHS_STATUS_DATATYPE_MASK) >> FHS_STATUS_DATATYPE_SHIFT)
#define  FHS_STATUS_DATATYPE_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_DATATYPE_MASK) | (((uint32_t)val) << FHS_STATUS_DATATYPE_SHIFT))

#define  FHS_STATUS_RESERVED3_MASK 0xfc00
#define  FHS_STATUS_RESERVED3_SHIFT 10
#define  FHS_STATUS_RESERVED3_MASK_SHIFT(reg) (((reg) & FHS_STATUS_RESERVED3_MASK) >> FHS_STATUS_RESERVED3_SHIFT)
#define  FHS_STATUS_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_RESERVED3_MASK) | (((uint32_t)val) << FHS_STATUS_RESERVED3_SHIFT))

#define  FHS_STATUS_MODE_MASK 0x300
#define  FHS_STATUS_MODE_SHIFT 8
#define  FHS_STATUS_MODE_MASK_SHIFT(reg) (((reg) & FHS_STATUS_MODE_MASK) >> FHS_STATUS_MODE_SHIFT)
#define  FHS_STATUS_MODE_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_MODE_MASK) | (((uint32_t)val) << FHS_STATUS_MODE_SHIFT))

#define  FHS_STATUS_RESERVED4_MASK 0xff
#define  FHS_STATUS_RESERVED4_SHIFT 0
#define  FHS_STATUS_RESERVED4_MASK_SHIFT(reg) (((reg) & FHS_STATUS_RESERVED4_MASK) >> FHS_STATUS_RESERVED4_SHIFT)
#define  FHS_STATUS_RESERVED4_REPLACE_VAL(reg,val) (((reg) & ~FHS_STATUS_RESERVED4_MASK) | (((uint32_t)val) << FHS_STATUS_RESERVED4_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_pend)
/** \brief Indicates interrupt cause.*/
//====================================================================

#define  FHS_INT_PEND_RESERVED1_MASK 0xfffffff0
#define  FHS_INT_PEND_RESERVED1_SHIFT 4
#define  FHS_INT_PEND_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_INT_PEND_RESERVED1_MASK) >> FHS_INT_PEND_RESERVED1_SHIFT)
#define  FHS_INT_PEND_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_PEND_RESERVED1_MASK) | (((uint32_t)val) << FHS_INT_PEND_RESERVED1_SHIFT))

#define  FHS_INT_PEND_FORCEDBP_OLTI_MASK 0x8
#define  FHS_INT_PEND_FORCEDBP_OLTI_SHIFT 3
#define  FHS_INT_PEND_FORCEDBP_OLTI_MASK_SHIFT(reg) (((reg) & FHS_INT_PEND_FORCEDBP_OLTI_MASK) >> FHS_INT_PEND_FORCEDBP_OLTI_SHIFT)
#define  FHS_INT_PEND_FORCEDBP_OLTI_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_PEND_FORCEDBP_OLTI_MASK) | (((uint32_t)val) << FHS_INT_PEND_FORCEDBP_OLTI_SHIFT))

#define  FHS_INT_PEND_FORCEDBP_ILT2_MASK 0x4
#define  FHS_INT_PEND_FORCEDBP_ILT2_SHIFT 2
#define  FHS_INT_PEND_FORCEDBP_ILT2_MASK_SHIFT(reg) (((reg) & FHS_INT_PEND_FORCEDBP_ILT2_MASK) >> FHS_INT_PEND_FORCEDBP_ILT2_SHIFT)
#define  FHS_INT_PEND_FORCEDBP_ILT2_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_PEND_FORCEDBP_ILT2_MASK) | (((uint32_t)val) << FHS_INT_PEND_FORCEDBP_ILT2_SHIFT))

#define  FHS_INT_PEND_CHOKING_MASK 0x2
#define  FHS_INT_PEND_CHOKING_SHIFT 1
#define  FHS_INT_PEND_CHOKING_MASK_SHIFT(reg) (((reg) & FHS_INT_PEND_CHOKING_MASK) >> FHS_INT_PEND_CHOKING_SHIFT)
#define  FHS_INT_PEND_CHOKING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_PEND_CHOKING_MASK) | (((uint32_t)val) << FHS_INT_PEND_CHOKING_SHIFT))

#define  FHS_INT_PEND_BARFING_MASK 0x1
#define  FHS_INT_PEND_BARFING_SHIFT 0
#define  FHS_INT_PEND_BARFING_MASK_SHIFT(reg) (((reg) & FHS_INT_PEND_BARFING_MASK) >> FHS_INT_PEND_BARFING_SHIFT)
#define  FHS_INT_PEND_BARFING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_PEND_BARFING_MASK) | (((uint32_t)val) << FHS_INT_PEND_BARFING_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_ack)
/** \brief Interrupt Acknowledge clears pending causes.*/
//====================================================================

#define  FHS_INT_ACK_RESERVED1_MASK 0xfffffff0
#define  FHS_INT_ACK_RESERVED1_SHIFT 4
#define  FHS_INT_ACK_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_INT_ACK_RESERVED1_MASK) >> FHS_INT_ACK_RESERVED1_SHIFT)
#define  FHS_INT_ACK_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_ACK_RESERVED1_MASK) | (((uint32_t)val) << FHS_INT_ACK_RESERVED1_SHIFT))

#define  FHS_INT_ACK_FORCEDBP_OLTI_MASK 0x8
#define  FHS_INT_ACK_FORCEDBP_OLTI_SHIFT 3
#define  FHS_INT_ACK_FORCEDBP_OLTI_MASK_SHIFT(reg) (((reg) & FHS_INT_ACK_FORCEDBP_OLTI_MASK) >> FHS_INT_ACK_FORCEDBP_OLTI_SHIFT)
#define  FHS_INT_ACK_FORCEDBP_OLTI_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_ACK_FORCEDBP_OLTI_MASK) | (((uint32_t)val) << FHS_INT_ACK_FORCEDBP_OLTI_SHIFT))

#define  FHS_INT_ACK_FORCEDBP_ILT2_MASK 0x4
#define  FHS_INT_ACK_FORCEDBP_ILT2_SHIFT 2
#define  FHS_INT_ACK_FORCEDBP_ILT2_MASK_SHIFT(reg) (((reg) & FHS_INT_ACK_FORCEDBP_ILT2_MASK) >> FHS_INT_ACK_FORCEDBP_ILT2_SHIFT)
#define  FHS_INT_ACK_FORCEDBP_ILT2_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_ACK_FORCEDBP_ILT2_MASK) | (((uint32_t)val) << FHS_INT_ACK_FORCEDBP_ILT2_SHIFT))

#define  FHS_INT_ACK_CHOKING_MASK 0x2
#define  FHS_INT_ACK_CHOKING_SHIFT 1
#define  FHS_INT_ACK_CHOKING_MASK_SHIFT(reg) (((reg) & FHS_INT_ACK_CHOKING_MASK) >> FHS_INT_ACK_CHOKING_SHIFT)
#define  FHS_INT_ACK_CHOKING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_ACK_CHOKING_MASK) | (((uint32_t)val) << FHS_INT_ACK_CHOKING_SHIFT))

#define  FHS_INT_ACK_BARFING_MASK 0x1
#define  FHS_INT_ACK_BARFING_SHIFT 0
#define  FHS_INT_ACK_BARFING_MASK_SHIFT(reg) (((reg) & FHS_INT_ACK_BARFING_MASK) >> FHS_INT_ACK_BARFING_SHIFT)
#define  FHS_INT_ACK_BARFING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_ACK_BARFING_MASK) | (((uint32_t)val) << FHS_INT_ACK_BARFING_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
/** \brief Force Interrupt causes.*/
//====================================================================

#define  FHS_INT_FO_RESERVED1_MASK 0xfffffff0
#define  FHS_INT_FO_RESERVED1_SHIFT 4
#define  FHS_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_INT_FO_RESERVED1_MASK) >> FHS_INT_FO_RESERVED1_SHIFT)
#define  FHS_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << FHS_INT_FO_RESERVED1_SHIFT))

#define  FHS_INT_FO_FORCEDBP_OLTI_MASK 0x8
#define  FHS_INT_FO_FORCEDBP_OLTI_SHIFT 3
#define  FHS_INT_FO_FORCEDBP_OLTI_MASK_SHIFT(reg) (((reg) & FHS_INT_FO_FORCEDBP_OLTI_MASK) >> FHS_INT_FO_FORCEDBP_OLTI_SHIFT)
#define  FHS_INT_FO_FORCEDBP_OLTI_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_FO_FORCEDBP_OLTI_MASK) | (((uint32_t)val) << FHS_INT_FO_FORCEDBP_OLTI_SHIFT))

#define  FHS_INT_FO_FORCEDBP_ILT2_MASK 0x4
#define  FHS_INT_FO_FORCEDBP_ILT2_SHIFT 2
#define  FHS_INT_FO_FORCEDBP_ILT2_MASK_SHIFT(reg) (((reg) & FHS_INT_FO_FORCEDBP_ILT2_MASK) >> FHS_INT_FO_FORCEDBP_ILT2_SHIFT)
#define  FHS_INT_FO_FORCEDBP_ILT2_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_FO_FORCEDBP_ILT2_MASK) | (((uint32_t)val) << FHS_INT_FO_FORCEDBP_ILT2_SHIFT))

#define  FHS_INT_FO_CHOKING_MASK 0x2
#define  FHS_INT_FO_CHOKING_SHIFT 1
#define  FHS_INT_FO_CHOKING_MASK_SHIFT(reg) (((reg) & FHS_INT_FO_CHOKING_MASK) >> FHS_INT_FO_CHOKING_SHIFT)
#define  FHS_INT_FO_CHOKING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_FO_CHOKING_MASK) | (((uint32_t)val) << FHS_INT_FO_CHOKING_SHIFT))

#define  FHS_INT_FO_BARFING_MASK 0x1
#define  FHS_INT_FO_BARFING_SHIFT 0
#define  FHS_INT_FO_BARFING_MASK_SHIFT(reg) (((reg) & FHS_INT_FO_BARFING_MASK) >> FHS_INT_FO_BARFING_SHIFT)
#define  FHS_INT_FO_BARFING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_FO_BARFING_MASK) | (((uint32_t)val) << FHS_INT_FO_BARFING_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
/** \brief Enable Interrupt causes.*/
//====================================================================

#define  FHS_INT_EN_RESERVED1_MASK 0xfffffff0
#define  FHS_INT_EN_RESERVED1_SHIFT 4
#define  FHS_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & FHS_INT_EN_RESERVED1_MASK) >> FHS_INT_EN_RESERVED1_SHIFT)
#define  FHS_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << FHS_INT_EN_RESERVED1_SHIFT))

#define  FHS_INT_EN_FORCEDBP_OLTI_MASK 0x8
#define  FHS_INT_EN_FORCEDBP_OLTI_SHIFT 3
#define  FHS_INT_EN_FORCEDBP_OLTI_MASK_SHIFT(reg) (((reg) & FHS_INT_EN_FORCEDBP_OLTI_MASK) >> FHS_INT_EN_FORCEDBP_OLTI_SHIFT)
#define  FHS_INT_EN_FORCEDBP_OLTI_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_EN_FORCEDBP_OLTI_MASK) | (((uint32_t)val) << FHS_INT_EN_FORCEDBP_OLTI_SHIFT))

#define  FHS_INT_EN_FORCEDBP_ILT2_MASK 0x4
#define  FHS_INT_EN_FORCEDBP_ILT2_SHIFT 2
#define  FHS_INT_EN_FORCEDBP_ILT2_MASK_SHIFT(reg) (((reg) & FHS_INT_EN_FORCEDBP_ILT2_MASK) >> FHS_INT_EN_FORCEDBP_ILT2_SHIFT)
#define  FHS_INT_EN_FORCEDBP_ILT2_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_EN_FORCEDBP_ILT2_MASK) | (((uint32_t)val) << FHS_INT_EN_FORCEDBP_ILT2_SHIFT))

#define  FHS_INT_EN_CHOKING_MASK 0x2
#define  FHS_INT_EN_CHOKING_SHIFT 1
#define  FHS_INT_EN_CHOKING_MASK_SHIFT(reg) (((reg) & FHS_INT_EN_CHOKING_MASK) >> FHS_INT_EN_CHOKING_SHIFT)
#define  FHS_INT_EN_CHOKING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_EN_CHOKING_MASK) | (((uint32_t)val) << FHS_INT_EN_CHOKING_SHIFT))

#define  FHS_INT_EN_BARFING_MASK 0x1
#define  FHS_INT_EN_BARFING_SHIFT 0
#define  FHS_INT_EN_BARFING_MASK_SHIFT(reg) (((reg) & FHS_INT_EN_BARFING_MASK) >> FHS_INT_EN_BARFING_SHIFT)
#define  FHS_INT_EN_BARFING_REPLACE_VAL(reg,val) (((reg) & ~FHS_INT_EN_BARFING_MASK) | (((uint32_t)val) << FHS_INT_EN_BARFING_SHIFT))

#endif // FHS
