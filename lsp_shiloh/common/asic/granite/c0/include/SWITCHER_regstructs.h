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
 * \file SWITCHER_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SWITCHER_REGS_REGSTRUCTS_H_
#define _SWITCHER_REGS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Switching regulator control block registers (switcher_regs)
/** \brief Registers for the apb_switcher block, which allows writing to the control registers within the Marvell 88PG82XX family of programmable switching regulators across a PCM type serial interface.*/
//
//====================================================================

typedef struct SWITCHER_REGS_REGS_s
{
  volatile uint32_t SData;  ///< 0x0 [R/W]: Switcher data register
  volatile uint32_t SStat;  ///< 0x4 [R]: Switcher status register
  volatile uint32_t HC0;  ///< 0x8 [R/W]: High count for zero bit
  volatile uint32_t HC1;  ///< 0xc [R/W]: HC1
} SWITCHER_REGS_REGS_t;

#endif // SWITCHER_REGS
