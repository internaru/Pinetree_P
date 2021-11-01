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
 * \file SIC_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _SIC_REGMASKS_H_
#define _SIC_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SIC (SIC)
//
//====================================================================

//====================================================================
//Register: SIC INTERRUPT ENABLE REGISTER IN LONG NAME (IRQENABLE)
/** \brief SIC Interrupt Enable Register in Comment Section*/
//====================================================================

#define  SIC_IRQENABLE_EN_MASK 0xffffffff
#define  SIC_IRQENABLE_EN_SHIFT 0
#define  SIC_IRQENABLE_EN_MASK_SHIFT(reg) (((reg) & SIC_IRQENABLE_EN_MASK) >> SIC_IRQENABLE_EN_SHIFT)
#define  SIC_IRQENABLE_EN_REPLACE_VAL(reg,val) (((reg) & ~SIC_IRQENABLE_EN_MASK) | (((uint32_t)val) << SIC_IRQENABLE_EN_SHIFT))

//====================================================================
//Register: SIC FAST INTERRUPT ENABLE REGISTER (FIQ_EN)
/** \brief SIC FIQ Interrupt Enable Register*/
//====================================================================

#define  SIC_FIQ_EN_FIQ_EN_MASK 0xffffffff
#define  SIC_FIQ_EN_FIQ_EN_SHIFT 0
#define  SIC_FIQ_EN_FIQ_EN_MASK_SHIFT(reg) (((reg) & SIC_FIQ_EN_FIQ_EN_MASK) >> SIC_FIQ_EN_FIQ_EN_SHIFT)
#define  SIC_FIQ_EN_FIQ_EN_REPLACE_VAL(reg,val) (((reg) & ~SIC_FIQ_EN_FIQ_EN_MASK) | (((uint32_t)val) << SIC_FIQ_EN_FIQ_EN_SHIFT))

//====================================================================
//Register: SIC PENDING REGISTER (IPR)
/** \brief SIC Interrupt Pending Register.*/
//====================================================================

#define  SIC_IPR_STATUS_MASK 0xffffffff
#define  SIC_IPR_STATUS_SHIFT 0
#define  SIC_IPR_STATUS_MASK_SHIFT(reg) (((reg) & SIC_IPR_STATUS_MASK) >> SIC_IPR_STATUS_SHIFT)
#define  SIC_IPR_STATUS_REPLACE_VAL(reg,val) (((reg) & ~SIC_IPR_STATUS_MASK) | (((uint32_t)val) << SIC_IPR_STATUS_SHIFT))

//====================================================================
//Register: SIC INTERRUPT ACKNOWLEDGE REGISTER (INT_ACK)
/** \brief SIC Interrupt Acknowledge Register*/
//====================================================================

#define  SIC_INT_ACK_ACK_MASK 0xffffffff
#define  SIC_INT_ACK_ACK_SHIFT 0
#define  SIC_INT_ACK_ACK_MASK_SHIFT(reg) (((reg) & SIC_INT_ACK_ACK_MASK) >> SIC_INT_ACK_ACK_SHIFT)
#define  SIC_INT_ACK_ACK_REPLACE_VAL(reg,val) (((reg) & ~SIC_INT_ACK_ACK_MASK) | (((uint32_t)val) << SIC_INT_ACK_ACK_SHIFT))

//====================================================================
//Register: SIC INTERRUPT FORCE REGISTER (FORCE)
/** \brief SIC Interrupt Force Register*/
//====================================================================

#define  SIC_FORCE_INT_FORCE_MASK 0xffffffff
#define  SIC_FORCE_INT_FORCE_SHIFT 0
#define  SIC_FORCE_INT_FORCE_MASK_SHIFT(reg) (((reg) & SIC_FORCE_INT_FORCE_MASK) >> SIC_FORCE_INT_FORCE_SHIFT)
#define  SIC_FORCE_INT_FORCE_REPLACE_VAL(reg,val) (((reg) & ~SIC_FORCE_INT_FORCE_MASK) | (((uint32_t)val) << SIC_FORCE_INT_FORCE_SHIFT))

//====================================================================
//Register: SIC INTERRUPT ENABLE CLEAR REGISTER (EN_CLEAR)
/** \brief SIC Enable Clear Register*/
//====================================================================

#define  SIC_EN_CLEAR_EN_CLEAR_MASK 0xffffffff
#define  SIC_EN_CLEAR_EN_CLEAR_SHIFT 0
#define  SIC_EN_CLEAR_EN_CLEAR_MASK_SHIFT(reg) (((reg) & SIC_EN_CLEAR_EN_CLEAR_MASK) >> SIC_EN_CLEAR_EN_CLEAR_SHIFT)
#define  SIC_EN_CLEAR_EN_CLEAR_REPLACE_VAL(reg,val) (((reg) & ~SIC_EN_CLEAR_EN_CLEAR_MASK) | (((uint32_t)val) << SIC_EN_CLEAR_EN_CLEAR_SHIFT))

//====================================================================
//Register: SIC INTERRUPT IRQ STATUS REGISTER (ISR)
/** \brief SIC IRQ Interrupt Status Register*/
//====================================================================

#define  SIC_ISR_IRQ_STATUS_MASK 0xffffffff
#define  SIC_ISR_IRQ_STATUS_SHIFT 0
#define  SIC_ISR_IRQ_STATUS_MASK_SHIFT(reg) (((reg) & SIC_ISR_IRQ_STATUS_MASK) >> SIC_ISR_IRQ_STATUS_SHIFT)
#define  SIC_ISR_IRQ_STATUS_REPLACE_VAL(reg,val) (((reg) & ~SIC_ISR_IRQ_STATUS_MASK) | (((uint32_t)val) << SIC_ISR_IRQ_STATUS_SHIFT))

//====================================================================
//Register: SIC INTERRUPT FIQ STATUS REGISTER (FSR)
/** \brief SIC FIQ Interrupt Status Register*/
//====================================================================

#define  SIC_FSR_FIQ_STATUS_MASK 0xffffffff
#define  SIC_FSR_FIQ_STATUS_SHIFT 0
#define  SIC_FSR_FIQ_STATUS_MASK_SHIFT(reg) (((reg) & SIC_FSR_FIQ_STATUS_MASK) >> SIC_FSR_FIQ_STATUS_SHIFT)
#define  SIC_FSR_FIQ_STATUS_REPLACE_VAL(reg,val) (((reg) & ~SIC_FSR_FIQ_STATUS_MASK) | (((uint32_t)val) << SIC_FSR_FIQ_STATUS_SHIFT))

#endif // SIC
