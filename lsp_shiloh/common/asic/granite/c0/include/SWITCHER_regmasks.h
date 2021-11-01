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
 * \file SWITCHER_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SWITCHER_REGS_REGMASKS_H_
#define _SWITCHER_REGS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: Switching regulator control block registers (switcher_regs)
/** \brief Registers for the apb_switcher block, which allows writing to the control registers within the Marvell 88PG82XX family of programmable switching regulators across a PCM type serial interface.*/
//
//====================================================================

//====================================================================
//Register: Switcher data register (SData)
/** \brief The 16 bit data word written into this register will be converted into the appropriate bit stream and written to the switcher.*/
//====================================================================

#define  SWITCHER_REGS_SDATA_RESERVED1_MASK 0xffff0000
#define  SWITCHER_REGS_SDATA_RESERVED1_SHIFT 16
#define  SWITCHER_REGS_SDATA_RESERVED1_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_SDATA_RESERVED1_MASK) >> SWITCHER_REGS_SDATA_RESERVED1_SHIFT)
#define  SWITCHER_REGS_SDATA_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_SDATA_RESERVED1_MASK) | (((uint32_t)val) << SWITCHER_REGS_SDATA_RESERVED1_SHIFT))

#define  SWITCHER_REGS_SDATA_DATA_MASK 0xffff
#define  SWITCHER_REGS_SDATA_DATA_SHIFT 0
#define  SWITCHER_REGS_SDATA_DATA_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_SDATA_DATA_MASK) >> SWITCHER_REGS_SDATA_DATA_SHIFT)
#define  SWITCHER_REGS_SDATA_DATA_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_SDATA_DATA_MASK) | (((uint32_t)val) << SWITCHER_REGS_SDATA_DATA_SHIFT))

//====================================================================
//Register: Switcher status register (SStat)
//====================================================================

#define  SWITCHER_REGS_SSTAT_RESERVED1_MASK 0xfffffffe
#define  SWITCHER_REGS_SSTAT_RESERVED1_SHIFT 1
#define  SWITCHER_REGS_SSTAT_RESERVED1_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_SSTAT_RESERVED1_MASK) >> SWITCHER_REGS_SSTAT_RESERVED1_SHIFT)
#define  SWITCHER_REGS_SSTAT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_SSTAT_RESERVED1_MASK) | (((uint32_t)val) << SWITCHER_REGS_SSTAT_RESERVED1_SHIFT))

#define  SWITCHER_REGS_SSTAT_BUSY_MASK 0x1
#define  SWITCHER_REGS_SSTAT_BUSY_SHIFT 0
#define  SWITCHER_REGS_SSTAT_BUSY_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_SSTAT_BUSY_MASK) >> SWITCHER_REGS_SSTAT_BUSY_SHIFT)
#define  SWITCHER_REGS_SSTAT_BUSY_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_SSTAT_BUSY_MASK) | (((uint32_t)val) << SWITCHER_REGS_SSTAT_BUSY_SHIFT))

//====================================================================
//Register: High count for zero bit (HC0)
/** \brief The value in this register determines how long the serial data line will be high while transmitting a "0" bit. The switcher specification requires this time to be 250nsec +/-50nsec. This register should be programmed with a value equal to the ceiling of (250nsec * the bus clock frequency).*/
//====================================================================

#define  SWITCHER_REGS_HC0_RESERVED1_MASK 0xffffff00
#define  SWITCHER_REGS_HC0_RESERVED1_SHIFT 8
#define  SWITCHER_REGS_HC0_RESERVED1_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_HC0_RESERVED1_MASK) >> SWITCHER_REGS_HC0_RESERVED1_SHIFT)
#define  SWITCHER_REGS_HC0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_HC0_RESERVED1_MASK) | (((uint32_t)val) << SWITCHER_REGS_HC0_RESERVED1_SHIFT))

#define  SWITCHER_REGS_HC0_HC0_MASK 0xff
#define  SWITCHER_REGS_HC0_HC0_SHIFT 0
#define  SWITCHER_REGS_HC0_HC0_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_HC0_HC0_MASK) >> SWITCHER_REGS_HC0_HC0_SHIFT)
#define  SWITCHER_REGS_HC0_HC0_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_HC0_HC0_MASK) | (((uint32_t)val) << SWITCHER_REGS_HC0_HC0_SHIFT))

//====================================================================
//Register: HC1 (HC1)
/** \brief The value in this register determines how long the serial data line will be high while transmitting a "1" bit. The switcher specification requires this time to be 750nsec +/-50nsec. This register should be programmed with a value equal to the ceiling of (750nsec * the bus clock frequency).*/
//====================================================================

#define  SWITCHER_REGS_HC1_IDLE_MASK 0x80000000
#define  SWITCHER_REGS_HC1_IDLE_SHIFT 31
#define  SWITCHER_REGS_HC1_IDLE_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_HC1_IDLE_MASK) >> SWITCHER_REGS_HC1_IDLE_SHIFT)
#define  SWITCHER_REGS_HC1_IDLE_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_HC1_IDLE_MASK) | (((uint32_t)val) << SWITCHER_REGS_HC1_IDLE_SHIFT))

#define  SWITCHER_REGS_HC1_RESERVED1_MASK 0x7fffff00
#define  SWITCHER_REGS_HC1_RESERVED1_SHIFT 8
#define  SWITCHER_REGS_HC1_RESERVED1_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_HC1_RESERVED1_MASK) >> SWITCHER_REGS_HC1_RESERVED1_SHIFT)
#define  SWITCHER_REGS_HC1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_HC1_RESERVED1_MASK) | (((uint32_t)val) << SWITCHER_REGS_HC1_RESERVED1_SHIFT))

#define  SWITCHER_REGS_HC1_HC1_MASK 0xff
#define  SWITCHER_REGS_HC1_HC1_SHIFT 0
#define  SWITCHER_REGS_HC1_HC1_MASK_SHIFT(reg) (((reg) & SWITCHER_REGS_HC1_HC1_MASK) >> SWITCHER_REGS_HC1_HC1_SHIFT)
#define  SWITCHER_REGS_HC1_HC1_REPLACE_VAL(reg,val) (((reg) & ~SWITCHER_REGS_HC1_HC1_MASK) | (((uint32_t)val) << SWITCHER_REGS_HC1_HC1_SHIFT))

#endif // SWITCHER_REGS
