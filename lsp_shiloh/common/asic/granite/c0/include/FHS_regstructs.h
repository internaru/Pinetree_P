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
 * \file FHS_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _FHS_REGSTRUCTS_H_
#define _FHS_REGSTRUCTS_H_

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

typedef struct FHS_REGS_s
{
  volatile uint32_t inImageWidth;  ///< 0x0 [R/W]: Input Image Pixel Width Register
  volatile uint32_t outImageWidth;  ///< 0x4 [R/W]: Output Image Pixel Width Register
  volatile uint32_t control;  ///< 0x8 [W]: Control Register
  volatile uint32_t status;  ///< 0xc [R]: Status Register
  volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
  volatile uint32_t int_en;  ///< 0x1c [R/W]: Interrupt Enable Register
} FHS_REGS_t;

#endif // FHS
