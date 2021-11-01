 /*
 *************************************************************************
 *
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                       Marvell Confidential
 *
 *************************************************************************
 */
/**
 * \file I2S_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _I2S_REGSTRUCTS_H_
#define _I2S_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: I2S (I2S)
//
//====================================================================

typedef struct I2S_REGS_s
{
  volatile uint32_t CONFIG;  ///< 0x0 [R/W]: I2S CONFIG REGISTER
  volatile uint32_t CLK_DIVIDE;  ///< 0x4 [R/W]: CLOCK DIVIDE REGISTER
  volatile uint32_t TXDATA;  ///< 0x8 [R/W]: TRANSMIT DATA REGISTER
  volatile uint32_t RXDATA;  ///< 0xc [R]: RECIEVE DATA REGISTER
  volatile uint32_t IPR;  ///< 0x10 [R]: INT PENDING REGISTER
  volatile uint32_t ABORT;  ///< 0x14 [W]: ABORT REGISTER
  volatile uint32_t FORCE;  ///< 0x18 [W]: INTERRUPT FORCE REGISTER
} I2S_REGS_t;

#endif // I2S
