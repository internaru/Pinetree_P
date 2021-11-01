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
 * \file Zx_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ZX_REGSTRUCTS_H_
#define _ZX_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Zx Register Set (Zx)
/** \brief Top-level register file for Zx*/
//
//====================================================================

typedef struct ZX_REGS_s
{
  volatile uint32_t ZCR;  ///< 0x0 [R/W]: Zx Configuration Register
  volatile uint32_t ZSR;  ///< 0x4 [R]: Zx Status Register
  volatile uint32_t ZIER;  ///< 0x8 [R/W]: Zx Interrupt Enable Register
  volatile uint32_t ZIPR;  ///< 0xc [R]: Zx Interrupt Pending Register
  volatile uint32_t ZIAR;  ///< 0x10 [R]: Zx Interrupt Active Register
  volatile uint32_t ZICR;  ///< 0x14 [W]: Zx Interrupt Clear Register
  volatile uint32_t ZIFR;  ///< 0x18 [W]: Zx Interrupt Force Register
  volatile uint32_t ZDWR;  ///< 0x1c [W]: Zx Descriptor Write Register
  volatile uint32_t ZDRR;  ///< 0x20 [R]: Zx Descriptor Read Register
  volatile uint32_t ZLLR;  ///< 0x24 [R/W]: Zx Line Length Register
  volatile uint32_t ZBPR;  ///< 0x28 [R]: Zx Bytes Processed Register
  volatile uint32_t ZCSR;  ///< 0x2c [R]: Zx Cmd Source Register
  volatile uint32_t ZCBR;  ///< 0x30 [R]: Zx Cmd Bytes Register
  volatile uint32_t ZPSR;  ///< 0x34 [R]: Zx Pix Source Register
  volatile uint32_t reserved0;
  volatile uint32_t ZIHR;  ///< 0x3c [R/W]: Zx Image Height Register
} ZX_REGS_t;

#endif // ZX
