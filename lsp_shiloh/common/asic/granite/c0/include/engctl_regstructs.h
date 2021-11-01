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
 * \file engctl_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _ENGCTL_REGSTRUCTS_H_
#define _ENGCTL_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Registers for the Engine Control module (engctl)
/** \brief Registers for the Engine Control module*/
//
//====================================================================

typedef struct ENGCTL_REGS_s
{
  volatile uint32_t ECFGR;  ///< 0x0 [R/W]: Engine Configuration Register
  volatile uint32_t ENGCTL0;  ///< 0x4 [R/W]: Engine Control 0 Register
  volatile uint32_t ENGCTL1;  ///< 0x8 [R/W]: Engine Control 1 Register
  volatile uint32_t ENGCTL2;  ///< 0xc [R/W]: Engine Control 2 Register
  volatile uint32_t reserved0[1020];
  volatile uint32_t ECCS[256];  ///< 0x1000 [R/W]: Engine Control Code Store
} ENGCTL_REGS_t;

#endif // ENGCTL
