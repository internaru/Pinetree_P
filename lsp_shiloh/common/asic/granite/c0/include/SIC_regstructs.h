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
 * \file SIC_regstructs.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SIC_REGSTRUCTS_H_
#define _SIC_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SIC (SIC)
//
//====================================================================

typedef struct SIC_REGS_s
{
  volatile uint32_t IRQENABLE;  ///< 0x0 [R/W]: SIC INTERRUPT ENABLE REGISTER IN LONG NAME
  volatile uint32_t FIQ_EN;  ///< 0x4 [R/W]: SIC FAST INTERRUPT ENABLE REGISTER
  volatile uint32_t IPR;  ///< 0x8 [R]: SIC PENDING REGISTER
  volatile uint32_t INT_ACK;  ///< 0xc [W]: SIC INTERRUPT ACKNOWLEDGE REGISTER
  volatile uint32_t FORCE;  ///< 0x10 [W]: SIC INTERRUPT FORCE REGISTER
  volatile uint32_t EN_CLEAR;  ///< 0x14 [W]: SIC INTERRUPT ENABLE CLEAR REGISTER
  volatile uint32_t ISR;  ///< 0x18 [R]: SIC INTERRUPT IRQ STATUS REGISTER
  volatile uint32_t FSR;  ///< 0x1c [R]: SIC INTERRUPT FIQ STATUS REGISTER
} SIC_REGS_t;

#endif // SIC
