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
 * \file EXP_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _EXP_REGS_REGSTRUCTS_H_
#define _EXP_REGS_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: exp_regs (exp_regs)
//
//====================================================================

typedef struct EXP_REGS_REGS_s
{
  volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t chaincfg;  ///< 0x4 [R/W]: Chain Configuration register
  volatile uint32_t reserved0[2];
  volatile uint32_t data0;  ///< 0x10 [R/W]: Data register (Instance 1 of 3)
  volatile uint32_t data1;  ///< 0x14 [R/W]: Data register (Instance 2 of 3)
  volatile uint32_t data2;  ///< 0x18 [R/W]: Data register (Instance 3 of 3)
  volatile uint32_t reserved1;
  volatile uint32_t data_in0;  ///< 0x20 [R]: Data In Register (Instance 1 of 3)
  volatile uint32_t data_in1;  ///< 0x24 [R]: Data In Register (Instance 2 of 3)
  volatile uint32_t data_in2;  ///< 0x28 [R]: Data In Register (Instance 3 of 3)
  volatile uint32_t reserved2;
  volatile uint32_t use_par_in0;  ///< 0x30 [R/W]: Use Parallel In Register (Instance 1 of 3)
  volatile uint32_t use_par_in1;  ///< 0x34 [R/W]: Use Parallel In Register (Instance 2 of 3)
  volatile uint32_t use_par_in2;  ///< 0x38 [R/W]: Use Parallel In Register (Instance 3 of 3)
  volatile uint32_t reserved3;
  volatile uint32_t rotate0;  ///< 0x40 [R/W]: Rotate Control Register (Instance 1 of 3)
  volatile uint32_t rotate1;  ///< 0x44 [R/W]: Rotate Control Register (Instance 2 of 3)
  volatile uint32_t rotate2;  ///< 0x48 [R/W]: Rotate Control Register (Instance 3 of 3)
} EXP_REGS_REGS_t;

#endif // EXP_REGS
