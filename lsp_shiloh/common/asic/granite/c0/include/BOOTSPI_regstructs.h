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
 * \file BOOTSPI_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _BOOTSPI_REGSTRUCTS_H_
#define _BOOTSPI_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Bootspi module registers (BOOTSPI)
/** \brief Registers for the bootspi module.*/
//
//====================================================================

typedef struct BOOTSPI_REGS_s
{
  volatile uint32_t BSCR;  ///< 0x0 [R/W]: Bootspi configuration register
  volatile uint32_t reserved0[3];
  volatile uint32_t BSBR;  ///< 0x10 [R/W]: Bootspi Bypass Register
  volatile uint32_t reserved1[3];
  volatile uint32_t BSCMDR;  ///< 0x20 [W]: Bootspi Command Register
  volatile uint32_t reserved2[3];
  volatile uint32_t BSSR;  ///< 0x30 [R]: Bootspi Status Register
} BOOTSPI_REGS_t;

#endif // BOOTSPI
