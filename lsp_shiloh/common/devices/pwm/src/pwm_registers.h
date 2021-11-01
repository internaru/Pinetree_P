/*
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file pwm_registers.h
 *
 * \brief Contains the register structures and masks for the PWM hardware
 * block.
 *
 */

#ifndef PWM_REGISTERS_H
#define PWM_REGISTERS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//====================================================================
//
//Register File: DEC PWM (PWM)
//
//====================================================================

typedef struct pwm_regs_s
{
  volatile uint32_t enable;  ///< 0x0 [R/W]: PWM Enable register
  volatile uint32_t int_pend;  ///< 0x4 [R]: PWM Interrupt Pending register
} pwm_regs_t;

//====================================================================
//
//Register File: DPWM (PWM_dpwm_regs)
//
//====================================================================

typedef struct pwm_dpwm_regs_s
{
  volatile uint32_t config;  ///< 0x0 [R/W]: Configuration register
  volatile uint32_t count_0;  ///< 0x4 [R/W]: Initial delay count register
  volatile uint32_t count_1;  ///< 0x8 [R/W]: First semicycle count register
  volatile uint32_t count_2;  ///< 0xc [R/W]: Second semicycle count register
  volatile uint32_t int_enable;  ///< 0x10 [R/W]: Interrupt Enable Register
  volatile uint32_t int_pend;  ///< 0x14 [R]: Interrupt Pending Register
  volatile uint32_t int_ack;  ///< 0x18 [W]: Interrupt Acknowledge Register
  volatile uint32_t int_force;  ///< 0x1c [W]: Interrupt Force Register
  volatile uint32_t status;  ///< 0x20 [R]: Status Register
} pwm_dpwm_regs_t;

//====================================================================
//
//Register File: DEC PWM (PWM)
//
//====================================================================

//====================================================================
//Register: PWM Enable register (enable)
//====================================================================

#define  PWM_ENABLE_ENABLE_MASK 0xffffffff
#define  PWM_ENABLE_ENABLE_SHIFT 0
#define  PWM_ENABLE_ENABLE_MASK_SHIFT(reg) (((reg) & PWM_ENABLE_ENABLE_MASK) >> PWM_ENABLE_ENABLE_SHIFT)
#define  PWM_ENABLE_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~PWM_ENABLE_ENABLE_MASK) | (((uint32_t)val) << PWM_ENABLE_ENABLE_SHIFT))

//====================================================================
//Register: PWM Interrupt Pending register (intp)
//====================================================================

#define  PWM_INTP_INTP_MASK 0xffffffff
#define  PWM_INTP_INTP_SHIFT 0
#define  PWM_INTP_INTP_MASK_SHIFT(reg) (((reg) & PWM_INTP_INTP_MASK) >> PWM_INTP_INTP_SHIFT)
#define  PWM_INTP_INTP_REPLACE_VAL(reg,val) (((reg) & ~PWM_INTP_INTP_MASK) | (((uint32_t)val) << PWM_INTP_INTP_SHIFT))


//====================================================================
//
//Register File: DPWM (Instance 1 of 32) (PWM_dpwm_regs0)
//
//====================================================================

//====================================================================
//Register: Configuration register (config)
//====================================================================

#define  PWM_DPWM_CFG_RESERVED1_MASK 0xffffff00
#define  PWM_DPWM_CFG_RESERVED1_SHIFT 8
#define  PWM_DPWM_CFG_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_CFG_RESERVED1_MASK) >> PWM_DPWM_CFG_RESERVED1_SHIFT)
#define  PWM_DPWM_CFG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_CFG_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_CFG_RESERVED1_SHIFT))

#define  PWM_DPWM_CFG_TIMEBASE_SEL_MASK 0xf0
#define  PWM_DPWM_CFG_TIMEBASE_SEL_SHIFT 4
#define  PWM_DPWM_CFG_TIMEBASE_SEL_MASK_SHIFT(reg) (((reg) & PWM_DPWM_CFG_TIMEBASE_SEL_MASK) >> PWM_DPWM_CFG_TIMEBASE_SEL_SHIFT)
#define  PWM_DPWM_CFG_TIMEBASE_SEL_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_CFG_TIMEBASE_SEL_MASK) | (((uint32_t)val) << PWM_DPWM_CFG_TIMEBASE_SEL_SHIFT))

#define  PWM_DPWM_CFG_HIGH_LOW_MASK 0x8
#define  PWM_DPWM_CFG_HIGH_LOW_SHIFT 3
#define  PWM_DPWM_CFG_HIGH_LOW_MASK_SHIFT(reg) (((reg) & PWM_DPWM_CFG_HIGH_LOW_MASK) >> PWM_DPWM_CFG_HIGH_LOW_SHIFT)
#define  PWM_DPWM_CFG_HIGH_LOW_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_CFG_HIGH_LOW_MASK) | (((uint32_t)val) << PWM_DPWM_CFG_HIGH_LOW_SHIFT))

#define  PWM_DPWM_CFG_OFF_STATE_MASK 0x4
#define  PWM_DPWM_CFG_OFF_STATE_SHIFT 2
#define  PWM_DPWM_CFG_OFF_STATE_MASK_SHIFT(reg) (((reg) & PWM_DPWM_CFG_OFF_STATE_MASK) >> PWM_DPWM_CFG_OFF_STATE_SHIFT)
#define  PWM_DPWM_CFG_OFF_STATE_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_CFG_OFF_STATE_MASK) | (((uint32_t)val) << PWM_DPWM_CFG_OFF_STATE_SHIFT))

#define  PWM_DPWM_CFG_SOFT_DISABLE_MASK 0x2
#define  PWM_DPWM_CFG_SOFT_DISABLE_SHIFT 1
#define  PWM_DPWM_CFG_SOFT_DISABLE_MASK_SHIFT(reg) (((reg) & PWM_DPWM_CFG_SOFT_DISABLE_MASK) >> PWM_DPWM_CFG_SOFT_DISABLE_SHIFT)
#define  PWM_DPWM_CFG_SOFT_DISABLE_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_CFG_SOFT_DISABLE_MASK) | (((uint32_t)val) << PWM_DPWM_CFG_SOFT_DISABLE_SHIFT))

#define  PWM_DPWM_CFG_ENABLE_MASK 0x1
#define  PWM_DPWM_CFG_ENABLE_SHIFT 0
#define  PWM_DPWM_CFG_ENABLE_MASK_SHIFT(reg) (((reg) & PWM_DPWM_CFG_ENABLE_MASK) >> PWM_DPWM_CFG_ENABLE_SHIFT)
#define  PWM_DPWM_CFG_ENABLE_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_CFG_ENABLE_MASK) | (((uint32_t)val) << PWM_DPWM_CFG_ENABLE_SHIFT))

//====================================================================
//Register: Initial delay count register (count0)
//====================================================================

#define  PWM_DPWM_COUNT0_RESERVED1_MASK 0xffff0000
#define  PWM_DPWM_COUNT0_RESERVED1_SHIFT 16
#define  PWM_DPWM_COUNT0_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_COUNT0_RESERVED1_MASK) >> PWM_DPWM_COUNT0_RESERVED1_SHIFT)
#define  PWM_DPWM_COUNT0_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_COUNT0_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_COUNT0_RESERVED1_SHIFT))

#define  PWM_DPWM_COUNT0_COUNT0_MASK 0xffff
#define  PWM_DPWM_COUNT0_COUNT0_SHIFT 0
#define  PWM_DPWM_COUNT0_COUNT0_MASK_SHIFT(reg) (((reg) & PWM_DPWM_COUNT0_COUNT0_MASK) >> PWM_DPWM_COUNT0_COUNT0_SHIFT)
#define  PWM_DPWM_COUNT0_COUNT0_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_COUNT0_COUNT0_MASK) | (((uint32_t)val) << PWM_DPWM_COUNT0_COUNT0_SHIFT))

//====================================================================
//Register: First semicycle count register (count1)
//====================================================================

#define  PWM_DPWM_COUNT1_RESERVED1_MASK 0xffff0000
#define  PWM_DPWM_COUNT1_RESERVED1_SHIFT 16
#define  PWM_DPWM_COUNT1_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_COUNT1_RESERVED1_MASK) >> PWM_DPWM_COUNT1_RESERVED1_SHIFT)
#define  PWM_DPWM_COUNT1_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_COUNT1_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_COUNT1_RESERVED1_SHIFT))

#define  PWM_DPWM_COUNT1_COUNT1_MASK 0xffff
#define  PWM_DPWM_COUNT1_COUNT1_SHIFT 0
#define  PWM_DPWM_COUNT1_COUNT1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_COUNT1_COUNT1_MASK) >> PWM_DPWM_COUNT1_COUNT1_SHIFT)
#define  PWM_DPWM_COUNT1_COUNT1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_COUNT1_COUNT1_MASK) | (((uint32_t)val) << PWM_DPWM_COUNT1_COUNT1_SHIFT))

//====================================================================
//Register: Second semicycle count register (count2)
//====================================================================

#define  PWM_DPWM_COUNT2_RESERVED1_MASK 0xffff0000
#define  PWM_DPWM_COUNT2_RESERVED1_SHIFT 16
#define  PWM_DPWM_COUNT2_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_COUNT2_RESERVED1_MASK) >> PWM_DPWM_COUNT2_RESERVED1_SHIFT)
#define  PWM_DPWM_COUNT2_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_COUNT2_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_COUNT2_RESERVED1_SHIFT))

#define  PWM_DPWM_COUNT2_COUNT2_MASK 0xffff
#define  PWM_DPWM_COUNT2_COUNT2_SHIFT 0
#define  PWM_DPWM_COUNT2_COUNT2_MASK_SHIFT(reg) (((reg) & PWM_DPWM_COUNT2_COUNT2_MASK) >> PWM_DPWM_COUNT2_COUNT2_SHIFT)
#define  PWM_DPWM_COUNT2_COUNT2_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_COUNT2_COUNT2_MASK) | (((uint32_t)val) << PWM_DPWM_COUNT2_COUNT2_SHIFT))

//====================================================================
//Register: Interrupt Enable Register (int_en)
//====================================================================

#define  PWM_DPWM_INT_EN_RESERVED1_MASK 0xfffffff8
#define  PWM_DPWM_INT_EN_RESERVED1_SHIFT 3
#define  PWM_DPWM_INT_EN_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_EN_RESERVED1_MASK) >> PWM_DPWM_INT_EN_RESERVED1_SHIFT)
#define  PWM_DPWM_INT_EN_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_EN_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_INT_EN_RESERVED1_SHIFT))

#define  PWM_DPWM_INT_EN_DONE_INT_EN_MASK 0x4
#define  PWM_DPWM_INT_EN_DONE_INT_EN_SHIFT 2
#define  PWM_DPWM_INT_EN_DONE_INT_EN_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_EN_DONE_INT_EN_MASK) >> PWM_DPWM_INT_EN_DONE_INT_EN_SHIFT)
#define  PWM_DPWM_INT_EN_DONE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_EN_DONE_INT_EN_MASK) | (((uint32_t)val) << PWM_DPWM_INT_EN_DONE_INT_EN_SHIFT))

#define  PWM_DPWM_INT_EN_FALL_INT_EN_MASK 0x2
#define  PWM_DPWM_INT_EN_FALL_INT_EN_SHIFT 1
#define  PWM_DPWM_INT_EN_FALL_INT_EN_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_EN_FALL_INT_EN_MASK) >> PWM_DPWM_INT_EN_FALL_INT_EN_SHIFT)
#define  PWM_DPWM_INT_EN_FALL_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_EN_FALL_INT_EN_MASK) | (((uint32_t)val) << PWM_DPWM_INT_EN_FALL_INT_EN_SHIFT))

#define  PWM_DPWM_INT_EN_RISE_INT_EN_MASK 0x1
#define  PWM_DPWM_INT_EN_RISE_INT_EN_SHIFT 0
#define  PWM_DPWM_INT_EN_RISE_INT_EN_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_EN_RISE_INT_EN_MASK) >> PWM_DPWM_INT_EN_RISE_INT_EN_SHIFT)
#define  PWM_DPWM_INT_EN_RISE_INT_EN_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_EN_RISE_INT_EN_MASK) | (((uint32_t)val) << PWM_DPWM_INT_EN_RISE_INT_EN_SHIFT))

//====================================================================
//Register: Interrupt Pending Register (int_st)
//====================================================================

#define  PWM_DPWM_INT_ST_RESERVED1_MASK 0xfffffff8
#define  PWM_DPWM_INT_ST_RESERVED1_SHIFT 3
#define  PWM_DPWM_INT_ST_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_ST_RESERVED1_MASK) >> PWM_DPWM_INT_ST_RESERVED1_SHIFT)
#define  PWM_DPWM_INT_ST_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_ST_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_INT_ST_RESERVED1_SHIFT))

#define  PWM_DPWM_INT_ST_DONE_INT_ST_MASK 0x4
#define  PWM_DPWM_INT_ST_DONE_INT_ST_SHIFT 2
#define  PWM_DPWM_INT_ST_DONE_INT_ST_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_ST_DONE_INT_ST_MASK) >> PWM_DPWM_INT_ST_DONE_INT_ST_SHIFT)
#define  PWM_DPWM_INT_ST_DONE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_ST_DONE_INT_ST_MASK) | (((uint32_t)val) << PWM_DPWM_INT_ST_DONE_INT_ST_SHIFT))

#define  PWM_DPWM_INT_ST_FALL_INT_ST_MASK 0x2
#define  PWM_DPWM_INT_ST_FALL_INT_ST_SHIFT 1
#define  PWM_DPWM_INT_ST_FALL_INT_ST_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_ST_FALL_INT_ST_MASK) >> PWM_DPWM_INT_ST_FALL_INT_ST_SHIFT)
#define  PWM_DPWM_INT_ST_FALL_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_ST_FALL_INT_ST_MASK) | (((uint32_t)val) << PWM_DPWM_INT_ST_FALL_INT_ST_SHIFT))

#define  PWM_DPWM_INT_ST_RISE_INT_ST_MASK 0x1
#define  PWM_DPWM_INT_ST_RISE_INT_ST_SHIFT 0
#define  PWM_DPWM_INT_ST_RISE_INT_ST_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_ST_RISE_INT_ST_MASK) >> PWM_DPWM_INT_ST_RISE_INT_ST_SHIFT)
#define  PWM_DPWM_INT_ST_RISE_INT_ST_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_ST_RISE_INT_ST_MASK) | (((uint32_t)val) << PWM_DPWM_INT_ST_RISE_INT_ST_SHIFT))

//====================================================================
//Register: Interrupt Acknowledge Register (int_cl)
//====================================================================

#define  PWM_DPWM_INT_CL_RESERVED1_MASK 0xfffffff8
#define  PWM_DPWM_INT_CL_RESERVED1_SHIFT 3
#define  PWM_DPWM_INT_CL_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_CL_RESERVED1_MASK) >> PWM_DPWM_INT_CL_RESERVED1_SHIFT)
#define  PWM_DPWM_INT_CL_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_CL_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_INT_CL_RESERVED1_SHIFT))

#define  PWM_DPWM_INT_CL_DONE_INT_CL_MASK 0x4
#define  PWM_DPWM_INT_CL_DONE_INT_CL_SHIFT 2
#define  PWM_DPWM_INT_CL_DONE_INT_CL_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_CL_DONE_INT_CL_MASK) >> PWM_DPWM_INT_CL_DONE_INT_CL_SHIFT)
#define  PWM_DPWM_INT_CL_DONE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_CL_DONE_INT_CL_MASK) | (((uint32_t)val) << PWM_DPWM_INT_CL_DONE_INT_CL_SHIFT))

#define  PWM_DPWM_INT_CL_FALL_INT_CL_MASK 0x2
#define  PWM_DPWM_INT_CL_FALL_INT_CL_SHIFT 1
#define  PWM_DPWM_INT_CL_FALL_INT_CL_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_CL_FALL_INT_CL_MASK) >> PWM_DPWM_INT_CL_FALL_INT_CL_SHIFT)
#define  PWM_DPWM_INT_CL_FALL_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_CL_FALL_INT_CL_MASK) | (((uint32_t)val) << PWM_DPWM_INT_CL_FALL_INT_CL_SHIFT))

#define  PWM_DPWM_INT_CL_RISE_INT_CL_MASK 0x1
#define  PWM_DPWM_INT_CL_RISE_INT_CL_SHIFT 0
#define  PWM_DPWM_INT_CL_RISE_INT_CL_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_CL_RISE_INT_CL_MASK) >> PWM_DPWM_INT_CL_RISE_INT_CL_SHIFT)
#define  PWM_DPWM_INT_CL_RISE_INT_CL_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_CL_RISE_INT_CL_MASK) | (((uint32_t)val) << PWM_DPWM_INT_CL_RISE_INT_CL_SHIFT))

//====================================================================
//Register: Interrupt Force Register (int_fo)
//====================================================================

#define  PWM_DPWM_INT_FO_RESERVED1_MASK 0xfffffff8
#define  PWM_DPWM_INT_FO_RESERVED1_SHIFT 3
#define  PWM_DPWM_INT_FO_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_FO_RESERVED1_MASK) >> PWM_DPWM_INT_FO_RESERVED1_SHIFT)
#define  PWM_DPWM_INT_FO_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_FO_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_INT_FO_RESERVED1_SHIFT))

#define  PWM_DPWM_INT_FO_DONE_INT_FO_MASK 0x4
#define  PWM_DPWM_INT_FO_DONE_INT_FO_SHIFT 2
#define  PWM_DPWM_INT_FO_DONE_INT_FO_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_FO_DONE_INT_FO_MASK) >> PWM_DPWM_INT_FO_DONE_INT_FO_SHIFT)
#define  PWM_DPWM_INT_FO_DONE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_FO_DONE_INT_FO_MASK) | (((uint32_t)val) << PWM_DPWM_INT_FO_DONE_INT_FO_SHIFT))

#define  PWM_DPWM_INT_FO_FALL_INT_FO_MASK 0x2
#define  PWM_DPWM_INT_FO_FALL_INT_FO_SHIFT 1
#define  PWM_DPWM_INT_FO_FALL_INT_FO_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_FO_FALL_INT_FO_MASK) >> PWM_DPWM_INT_FO_FALL_INT_FO_SHIFT)
#define  PWM_DPWM_INT_FO_FALL_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_FO_FALL_INT_FO_MASK) | (((uint32_t)val) << PWM_DPWM_INT_FO_FALL_INT_FO_SHIFT))

#define  PWM_DPWM_INT_FO_RISE_INT_FO_MASK 0x1
#define  PWM_DPWM_INT_FO_RISE_INT_FO_SHIFT 0
#define  PWM_DPWM_INT_FO_RISE_INT_FO_MASK_SHIFT(reg) (((reg) & PWM_DPWM_INT_FO_RISE_INT_FO_MASK) >> PWM_DPWM_INT_FO_RISE_INT_FO_SHIFT)
#define  PWM_DPWM_INT_FO_RISE_INT_FO_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_INT_FO_RISE_INT_FO_MASK) | (((uint32_t)val) << PWM_DPWM_INT_FO_RISE_INT_FO_SHIFT))

//====================================================================
//Register: Status Register (status)
//====================================================================

#define  PWM_DPWM_STATUS_RUNNING_MASK 0x80000000
#define  PWM_DPWM_STATUS_RUNNING_SHIFT 31
#define  PWM_DPWM_STATUS_RUNNING_MASK_SHIFT(reg) (((reg) & PWM_DPWM_STATUS_RUNNING_MASK) >> PWM_DPWM_STATUS_RUNNING_SHIFT)
#define  PWM_DPWM_STATUS_RUNNING_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_STATUS_RUNNING_MASK) | (((uint32_t)val) << PWM_DPWM_STATUS_RUNNING_SHIFT))

#define  PWM_DPWM_STATUS_DELAY_MASK 0x40000000
#define  PWM_DPWM_STATUS_DELAY_SHIFT 30
#define  PWM_DPWM_STATUS_DELAY_MASK_SHIFT(reg) (((reg) & PWM_DPWM_STATUS_DELAY_MASK) >> PWM_DPWM_STATUS_DELAY_SHIFT)
#define  PWM_DPWM_STATUS_DELAY_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_STATUS_DELAY_MASK) | (((uint32_t)val) << PWM_DPWM_STATUS_DELAY_SHIFT))

#define  PWM_DPWM_STATUS_SEMI_NUM_MASK 0x20000000
#define  PWM_DPWM_STATUS_SEMI_NUM_SHIFT 29
#define  PWM_DPWM_STATUS_SEMI_NUM_MASK_SHIFT(reg) (((reg) & PWM_DPWM_STATUS_SEMI_NUM_MASK) >> PWM_DPWM_STATUS_SEMI_NUM_SHIFT)
#define  PWM_DPWM_STATUS_SEMI_NUM_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_STATUS_SEMI_NUM_MASK) | (((uint32_t)val) << PWM_DPWM_STATUS_SEMI_NUM_SHIFT))

#define  PWM_DPWM_STATUS_PWM_OUT_MASK 0x10000000
#define  PWM_DPWM_STATUS_PWM_OUT_SHIFT 28
#define  PWM_DPWM_STATUS_PWM_OUT_MASK_SHIFT(reg) (((reg) & PWM_DPWM_STATUS_PWM_OUT_MASK) >> PWM_DPWM_STATUS_PWM_OUT_SHIFT)
#define  PWM_DPWM_STATUS_PWM_OUT_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_STATUS_PWM_OUT_MASK) | (((uint32_t)val) << PWM_DPWM_STATUS_PWM_OUT_SHIFT))

#define  PWM_DPWM_STATUS_RESERVED1_MASK 0xfff0000
#define  PWM_DPWM_STATUS_RESERVED1_SHIFT 16
#define  PWM_DPWM_STATUS_RESERVED1_MASK_SHIFT(reg) (((reg) & PWM_DPWM_STATUS_RESERVED1_MASK) >> PWM_DPWM_STATUS_RESERVED1_SHIFT)
#define  PWM_DPWM_STATUS_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_STATUS_RESERVED1_MASK) | (((uint32_t)val) << PWM_DPWM_STATUS_RESERVED1_SHIFT))

#define  PWM_DPWM_STATUS_CUR_COUNT_MASK 0xffff
#define  PWM_DPWM_STATUS_CUR_COUNT_SHIFT 0
#define  PWM_DPWM_STATUS_CUR_COUNT_MASK_SHIFT(reg) (((reg) & PWM_DPWM_STATUS_CUR_COUNT_MASK) >> PWM_DPWM_STATUS_CUR_COUNT_SHIFT)
#define  PWM_DPWM_STATUS_CUR_COUNT_REPLACE_VAL(reg,val) (((reg) & ~PWM_DPWM_STATUS_CUR_COUNT_MASK) | (((uint32_t)val) << PWM_DPWM_STATUS_CUR_COUNT_SHIFT))


#ifdef __cplusplus
}
#endif

#endif // PWM_REGISTERS_H

