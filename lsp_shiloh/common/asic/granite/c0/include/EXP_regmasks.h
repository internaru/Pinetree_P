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
 * \file EXP_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _EXP_REGS_REGMASKS_H_
#define _EXP_REGS_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: exp_regs (exp_regs)
//
//====================================================================

//====================================================================
//Register: Configuration register (cfg)
//====================================================================

#define  EXP_REGS_CFG_KICK_COUNT_MASK 0xfff00000
#define  EXP_REGS_CFG_KICK_COUNT_SHIFT 20
#define  EXP_REGS_CFG_KICK_COUNT_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_KICK_COUNT_MASK) >> EXP_REGS_CFG_KICK_COUNT_SHIFT)
#define  EXP_REGS_CFG_KICK_COUNT_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_KICK_COUNT_MASK) | (((uint32_t)val) << EXP_REGS_CFG_KICK_COUNT_SHIFT))

#define  EXP_REGS_CFG_PULSE_WIDTH_MASK 0xff000
#define  EXP_REGS_CFG_PULSE_WIDTH_SHIFT 12
#define  EXP_REGS_CFG_PULSE_WIDTH_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_PULSE_WIDTH_MASK) >> EXP_REGS_CFG_PULSE_WIDTH_SHIFT)
#define  EXP_REGS_CFG_PULSE_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_PULSE_WIDTH_MASK) | (((uint32_t)val) << EXP_REGS_CFG_PULSE_WIDTH_SHIFT))

#define  EXP_REGS_CFG_RESERVED1_MASK 0xc00
#define  EXP_REGS_CFG_RESERVED1_SHIFT 10
#define  EXP_REGS_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_RESERVED1_MASK) >> EXP_REGS_CFG_RESERVED1_SHIFT)
#define  EXP_REGS_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_RESERVED1_MASK) | (((uint32_t)val) << EXP_REGS_CFG_RESERVED1_SHIFT))

#define  EXP_REGS_CFG_TIMEBASE_SEL_MASK 0x300
#define  EXP_REGS_CFG_TIMEBASE_SEL_SHIFT 8
#define  EXP_REGS_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_TIMEBASE_SEL_MASK) >> EXP_REGS_CFG_TIMEBASE_SEL_SHIFT)
#define  EXP_REGS_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << EXP_REGS_CFG_TIMEBASE_SEL_SHIFT))

#define  EXP_REGS_CFG_EXP_WIDTH_MASK 0xf0
#define  EXP_REGS_CFG_EXP_WIDTH_SHIFT 4
#define  EXP_REGS_CFG_EXP_WIDTH_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_EXP_WIDTH_MASK) >> EXP_REGS_CFG_EXP_WIDTH_SHIFT)
#define  EXP_REGS_CFG_EXP_WIDTH_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_EXP_WIDTH_MASK) | (((uint32_t)val) << EXP_REGS_CFG_EXP_WIDTH_SHIFT))

#define  EXP_REGS_CFG_RESERVED2_MASK 0xc
#define  EXP_REGS_CFG_RESERVED2_SHIFT 2
#define  EXP_REGS_CFG_RESERVED2_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_RESERVED2_MASK) >> EXP_REGS_CFG_RESERVED2_SHIFT)
#define  EXP_REGS_CFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_RESERVED2_MASK) | (((uint32_t)val) << EXP_REGS_CFG_RESERVED2_SHIFT))

#define  EXP_REGS_CFG_KICK_MASK 0x2
#define  EXP_REGS_CFG_KICK_SHIFT 1
#define  EXP_REGS_CFG_KICK_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_KICK_MASK) >> EXP_REGS_CFG_KICK_SHIFT)
#define  EXP_REGS_CFG_KICK_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_KICK_MASK) | (((uint32_t)val) << EXP_REGS_CFG_KICK_SHIFT))

#define  EXP_REGS_CFG_ENABLE_MASK 0x1
#define  EXP_REGS_CFG_ENABLE_SHIFT 0
#define  EXP_REGS_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & EXP_REGS_CFG_ENABLE_MASK) >> EXP_REGS_CFG_ENABLE_SHIFT)
#define  EXP_REGS_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CFG_ENABLE_MASK) | (((uint32_t)val) << EXP_REGS_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Chain Configuration register (chaincfg)
//====================================================================

#define  EXP_REGS_CHAINCFG_RESERVED1_MASK 0xfff80000
#define  EXP_REGS_CHAINCFG_RESERVED1_SHIFT 19
#define  EXP_REGS_CHAINCFG_RESERVED1_MASK_SHIFT(reg) (((reg) & EXP_REGS_CHAINCFG_RESERVED1_MASK) >> EXP_REGS_CHAINCFG_RESERVED1_SHIFT)
#define  EXP_REGS_CHAINCFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CHAINCFG_RESERVED1_MASK) | (((uint32_t)val) << EXP_REGS_CHAINCFG_RESERVED1_SHIFT))

#define  EXP_REGS_CHAINCFG_PAR_OUT_EN_MASK 0x70000
#define  EXP_REGS_CHAINCFG_PAR_OUT_EN_SHIFT 16
#define  EXP_REGS_CHAINCFG_PAR_OUT_EN_MASK_SHIFT(reg) (((reg) & EXP_REGS_CHAINCFG_PAR_OUT_EN_MASK) >> EXP_REGS_CHAINCFG_PAR_OUT_EN_SHIFT)
#define  EXP_REGS_CHAINCFG_PAR_OUT_EN_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CHAINCFG_PAR_OUT_EN_MASK) | (((uint32_t)val) << EXP_REGS_CHAINCFG_PAR_OUT_EN_SHIFT))

#define  EXP_REGS_CHAINCFG_RESERVED2_MASK 0xff00
#define  EXP_REGS_CHAINCFG_RESERVED2_SHIFT 8
#define  EXP_REGS_CHAINCFG_RESERVED2_MASK_SHIFT(reg) (((reg) & EXP_REGS_CHAINCFG_RESERVED2_MASK) >> EXP_REGS_CHAINCFG_RESERVED2_SHIFT)
#define  EXP_REGS_CHAINCFG_RESERVED2_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CHAINCFG_RESERVED2_MASK) | (((uint32_t)val) << EXP_REGS_CHAINCFG_RESERVED2_SHIFT))

#define  EXP_REGS_CHAINCFG_RESERVED3_MASK 0xf8
#define  EXP_REGS_CHAINCFG_RESERVED3_SHIFT 3
#define  EXP_REGS_CHAINCFG_RESERVED3_MASK_SHIFT(reg) (((reg) & EXP_REGS_CHAINCFG_RESERVED3_MASK) >> EXP_REGS_CHAINCFG_RESERVED3_SHIFT)
#define  EXP_REGS_CHAINCFG_RESERVED3_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CHAINCFG_RESERVED3_MASK) | (((uint32_t)val) << EXP_REGS_CHAINCFG_RESERVED3_SHIFT))

#define  EXP_REGS_CHAINCFG_MODE_MASK 0x7
#define  EXP_REGS_CHAINCFG_MODE_SHIFT 0
#define  EXP_REGS_CHAINCFG_MODE_MASK_SHIFT(reg) (((reg) & EXP_REGS_CHAINCFG_MODE_MASK) >> EXP_REGS_CHAINCFG_MODE_SHIFT)
#define  EXP_REGS_CHAINCFG_MODE_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_CHAINCFG_MODE_MASK) | (((uint32_t)val) << EXP_REGS_CHAINCFG_MODE_SHIFT))

//====================================================================
//Register: Data register (Instance 1 of 3) (data0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  EXP_REGS_DATA0_DATA_MASK 0xffffffff
#define  EXP_REGS_DATA0_DATA_SHIFT 0
#define  EXP_REGS_DATA0_DATA_MASK_SHIFT(reg) (((reg) & EXP_REGS_DATA0_DATA_MASK) >> EXP_REGS_DATA0_DATA_SHIFT)
#define  EXP_REGS_DATA0_DATA_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_DATA0_DATA_MASK) | (((uint32_t)val) << EXP_REGS_DATA0_DATA_SHIFT))

//====================================================================
//Register: Data register (Instance 2 of 3) (data1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  EXP_REGS_DATA1_DATA_MASK 0xffffffff
#define  EXP_REGS_DATA1_DATA_SHIFT 0
#define  EXP_REGS_DATA1_DATA_MASK_SHIFT(reg) (((reg) & EXP_REGS_DATA1_DATA_MASK) >> EXP_REGS_DATA1_DATA_SHIFT)
#define  EXP_REGS_DATA1_DATA_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_DATA1_DATA_MASK) | (((uint32_t)val) << EXP_REGS_DATA1_DATA_SHIFT))

//====================================================================
//Register: Data register (Instance 3 of 3) (data2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  EXP_REGS_DATA2_DATA_MASK 0xffffffff
#define  EXP_REGS_DATA2_DATA_SHIFT 0
#define  EXP_REGS_DATA2_DATA_MASK_SHIFT(reg) (((reg) & EXP_REGS_DATA2_DATA_MASK) >> EXP_REGS_DATA2_DATA_SHIFT)
#define  EXP_REGS_DATA2_DATA_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_DATA2_DATA_MASK) | (((uint32_t)val) << EXP_REGS_DATA2_DATA_SHIFT))

//====================================================================
//Register: Data In Register (Instance 1 of 3) (data_in0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  EXP_REGS_DATA_IN0_DATA_MASK 0xffffffff
#define  EXP_REGS_DATA_IN0_DATA_SHIFT 0
#define  EXP_REGS_DATA_IN0_DATA_MASK_SHIFT(reg) (((reg) & EXP_REGS_DATA_IN0_DATA_MASK) >> EXP_REGS_DATA_IN0_DATA_SHIFT)
#define  EXP_REGS_DATA_IN0_DATA_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_DATA_IN0_DATA_MASK) | (((uint32_t)val) << EXP_REGS_DATA_IN0_DATA_SHIFT))

//====================================================================
//Register: Data In Register (Instance 2 of 3) (data_in1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  EXP_REGS_DATA_IN1_DATA_MASK 0xffffffff
#define  EXP_REGS_DATA_IN1_DATA_SHIFT 0
#define  EXP_REGS_DATA_IN1_DATA_MASK_SHIFT(reg) (((reg) & EXP_REGS_DATA_IN1_DATA_MASK) >> EXP_REGS_DATA_IN1_DATA_SHIFT)
#define  EXP_REGS_DATA_IN1_DATA_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_DATA_IN1_DATA_MASK) | (((uint32_t)val) << EXP_REGS_DATA_IN1_DATA_SHIFT))

//====================================================================
//Register: Data In Register (Instance 3 of 3) (data_in2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  EXP_REGS_DATA_IN2_DATA_MASK 0xffffffff
#define  EXP_REGS_DATA_IN2_DATA_SHIFT 0
#define  EXP_REGS_DATA_IN2_DATA_MASK_SHIFT(reg) (((reg) & EXP_REGS_DATA_IN2_DATA_MASK) >> EXP_REGS_DATA_IN2_DATA_SHIFT)
#define  EXP_REGS_DATA_IN2_DATA_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_DATA_IN2_DATA_MASK) | (((uint32_t)val) << EXP_REGS_DATA_IN2_DATA_SHIFT))

//====================================================================
//Register: Use Parallel In Register (Instance 1 of 3) (use_par_in0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  EXP_REGS_USE_PAR_IN0_USE_PAR_IN_MASK 0xffffffff
#define  EXP_REGS_USE_PAR_IN0_USE_PAR_IN_SHIFT 0
#define  EXP_REGS_USE_PAR_IN0_USE_PAR_IN_MASK_SHIFT(reg) (((reg) & EXP_REGS_USE_PAR_IN0_USE_PAR_IN_MASK) >> EXP_REGS_USE_PAR_IN0_USE_PAR_IN_SHIFT)
#define  EXP_REGS_USE_PAR_IN0_USE_PAR_IN_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_USE_PAR_IN0_USE_PAR_IN_MASK) | (((uint32_t)val) << EXP_REGS_USE_PAR_IN0_USE_PAR_IN_SHIFT))

//====================================================================
//Register: Use Parallel In Register (Instance 2 of 3) (use_par_in1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  EXP_REGS_USE_PAR_IN1_USE_PAR_IN_MASK 0xffffffff
#define  EXP_REGS_USE_PAR_IN1_USE_PAR_IN_SHIFT 0
#define  EXP_REGS_USE_PAR_IN1_USE_PAR_IN_MASK_SHIFT(reg) (((reg) & EXP_REGS_USE_PAR_IN1_USE_PAR_IN_MASK) >> EXP_REGS_USE_PAR_IN1_USE_PAR_IN_SHIFT)
#define  EXP_REGS_USE_PAR_IN1_USE_PAR_IN_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_USE_PAR_IN1_USE_PAR_IN_MASK) | (((uint32_t)val) << EXP_REGS_USE_PAR_IN1_USE_PAR_IN_SHIFT))

//====================================================================
//Register: Use Parallel In Register (Instance 3 of 3) (use_par_in2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  EXP_REGS_USE_PAR_IN2_USE_PAR_IN_MASK 0xffffffff
#define  EXP_REGS_USE_PAR_IN2_USE_PAR_IN_SHIFT 0
#define  EXP_REGS_USE_PAR_IN2_USE_PAR_IN_MASK_SHIFT(reg) (((reg) & EXP_REGS_USE_PAR_IN2_USE_PAR_IN_MASK) >> EXP_REGS_USE_PAR_IN2_USE_PAR_IN_SHIFT)
#define  EXP_REGS_USE_PAR_IN2_USE_PAR_IN_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_USE_PAR_IN2_USE_PAR_IN_MASK) | (((uint32_t)val) << EXP_REGS_USE_PAR_IN2_USE_PAR_IN_SHIFT))

//====================================================================
//Register: Rotate Control Register (Instance 1 of 3) (rotate0)
/** \brief  (Instance 1 of 3)*/
//====================================================================

#define  EXP_REGS_ROTATE0_RESERVED1_MASK 0xfffffffc
#define  EXP_REGS_ROTATE0_RESERVED1_SHIFT 2
#define  EXP_REGS_ROTATE0_RESERVED1_MASK_SHIFT(reg) (((reg) & EXP_REGS_ROTATE0_RESERVED1_MASK) >> EXP_REGS_ROTATE0_RESERVED1_SHIFT)
#define  EXP_REGS_ROTATE0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_ROTATE0_RESERVED1_MASK) | (((uint32_t)val) << EXP_REGS_ROTATE0_RESERVED1_SHIFT))

#define  EXP_REGS_ROTATE0_ROT_PAR_IN_MASK 0x3
#define  EXP_REGS_ROTATE0_ROT_PAR_IN_SHIFT 0
#define  EXP_REGS_ROTATE0_ROT_PAR_IN_MASK_SHIFT(reg) (((reg) & EXP_REGS_ROTATE0_ROT_PAR_IN_MASK) >> EXP_REGS_ROTATE0_ROT_PAR_IN_SHIFT)
#define  EXP_REGS_ROTATE0_ROT_PAR_IN_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_ROTATE0_ROT_PAR_IN_MASK) | (((uint32_t)val) << EXP_REGS_ROTATE0_ROT_PAR_IN_SHIFT))

//====================================================================
//Register: Rotate Control Register (Instance 2 of 3) (rotate1)
/** \brief  (Instance 2 of 3)*/
//====================================================================

#define  EXP_REGS_ROTATE1_RESERVED1_MASK 0xfffffffc
#define  EXP_REGS_ROTATE1_RESERVED1_SHIFT 2
#define  EXP_REGS_ROTATE1_RESERVED1_MASK_SHIFT(reg) (((reg) & EXP_REGS_ROTATE1_RESERVED1_MASK) >> EXP_REGS_ROTATE1_RESERVED1_SHIFT)
#define  EXP_REGS_ROTATE1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_ROTATE1_RESERVED1_MASK) | (((uint32_t)val) << EXP_REGS_ROTATE1_RESERVED1_SHIFT))

#define  EXP_REGS_ROTATE1_ROT_PAR_IN_MASK 0x3
#define  EXP_REGS_ROTATE1_ROT_PAR_IN_SHIFT 0
#define  EXP_REGS_ROTATE1_ROT_PAR_IN_MASK_SHIFT(reg) (((reg) & EXP_REGS_ROTATE1_ROT_PAR_IN_MASK) >> EXP_REGS_ROTATE1_ROT_PAR_IN_SHIFT)
#define  EXP_REGS_ROTATE1_ROT_PAR_IN_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_ROTATE1_ROT_PAR_IN_MASK) | (((uint32_t)val) << EXP_REGS_ROTATE1_ROT_PAR_IN_SHIFT))

//====================================================================
//Register: Rotate Control Register (Instance 3 of 3) (rotate2)
/** \brief  (Instance 3 of 3)*/
//====================================================================

#define  EXP_REGS_ROTATE2_RESERVED1_MASK 0xfffffffc
#define  EXP_REGS_ROTATE2_RESERVED1_SHIFT 2
#define  EXP_REGS_ROTATE2_RESERVED1_MASK_SHIFT(reg) (((reg) & EXP_REGS_ROTATE2_RESERVED1_MASK) >> EXP_REGS_ROTATE2_RESERVED1_SHIFT)
#define  EXP_REGS_ROTATE2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_ROTATE2_RESERVED1_MASK) | (((uint32_t)val) << EXP_REGS_ROTATE2_RESERVED1_SHIFT))

#define  EXP_REGS_ROTATE2_ROT_PAR_IN_MASK 0x3
#define  EXP_REGS_ROTATE2_ROT_PAR_IN_SHIFT 0
#define  EXP_REGS_ROTATE2_ROT_PAR_IN_MASK_SHIFT(reg) (((reg) & EXP_REGS_ROTATE2_ROT_PAR_IN_MASK) >> EXP_REGS_ROTATE2_ROT_PAR_IN_SHIFT)
#define  EXP_REGS_ROTATE2_ROT_PAR_IN_REPLACE_VAL(reg,val) (((reg) & ~EXP_REGS_ROTATE2_ROT_PAR_IN_MASK) | (((uint32_t)val) << EXP_REGS_ROTATE2_ROT_PAR_IN_SHIFT))

#endif // EXP_REGS
