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
 * \file I2S_regmasks.h
 * Generated using RegBuild 1.6.1
 *
 *************************************************************************
 */

#ifndef _I2S_REGMASKS_H_
#define _I2S_REGMASKS_H_

#include <stdint.h>

//====================================================================
//
//Register File: I2S (I2S)
//
//====================================================================

//====================================================================
//Register: I2S CONFIG REGISTER (CONFIG)
/** \brief I2S CONFIG Register*/
//====================================================================

#define  I2S_CONFIG_RESERVED1_MASK 0xffffff00
#define  I2S_CONFIG_RESERVED1_SHIFT 8
#define  I2S_CONFIG_RESERVED1_MASK_SHIFT(reg) (((reg) & I2S_CONFIG_RESERVED1_MASK) >> I2S_CONFIG_RESERVED1_SHIFT)
#define  I2S_CONFIG_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2S_CONFIG_RESERVED1_MASK) | (((uint32_t)val) << I2S_CONFIG_RESERVED1_SHIFT))

#define  I2S_CONFIG_WS_MASK 0x80
#define  I2S_CONFIG_WS_SHIFT 7
#define  I2S_CONFIG_WS_MASK_SHIFT(reg) (((reg) & I2S_CONFIG_WS_MASK) >> I2S_CONFIG_WS_SHIFT)
#define  I2S_CONFIG_WS_REPLACE_VAL(reg,val) (((reg) & ~I2S_CONFIG_WS_MASK) | (((uint32_t)val) << I2S_CONFIG_WS_SHIFT))

#define  I2S_CONFIG_TEN_MASK 0x40
#define  I2S_CONFIG_TEN_SHIFT 6
#define  I2S_CONFIG_TEN_MASK_SHIFT(reg) (((reg) & I2S_CONFIG_TEN_MASK) >> I2S_CONFIG_TEN_SHIFT)
#define  I2S_CONFIG_TEN_REPLACE_VAL(reg,val) (((reg) & ~I2S_CONFIG_TEN_MASK) | (((uint32_t)val) << I2S_CONFIG_TEN_SHIFT))

#define  I2S_CONFIG_REN_MASK 0x20
#define  I2S_CONFIG_REN_SHIFT 5
#define  I2S_CONFIG_REN_MASK_SHIFT(reg) (((reg) & I2S_CONFIG_REN_MASK) >> I2S_CONFIG_REN_SHIFT)
#define  I2S_CONFIG_REN_REPLACE_VAL(reg,val) (((reg) & ~I2S_CONFIG_REN_MASK) | (((uint32_t)val) << I2S_CONFIG_REN_SHIFT))

#define  I2S_CONFIG_D_MASK 0x10
#define  I2S_CONFIG_D_SHIFT 4
#define  I2S_CONFIG_D_MASK_SHIFT(reg) (((reg) & I2S_CONFIG_D_MASK) >> I2S_CONFIG_D_SHIFT)
#define  I2S_CONFIG_D_REPLACE_VAL(reg,val) (((reg) & ~I2S_CONFIG_D_MASK) | (((uint32_t)val) << I2S_CONFIG_D_SHIFT))

#define  I2S_CONFIG_M_MASK 0xc
#define  I2S_CONFIG_M_SHIFT 2
#define  I2S_CONFIG_M_MASK_SHIFT(reg) (((reg) & I2S_CONFIG_M_MASK) >> I2S_CONFIG_M_SHIFT)
#define  I2S_CONFIG_M_REPLACE_VAL(reg,val) (((reg) & ~I2S_CONFIG_M_MASK) | (((uint32_t)val) << I2S_CONFIG_M_SHIFT))

#define  I2S_CONFIG_DS_MASK 0x3
#define  I2S_CONFIG_DS_SHIFT 0
#define  I2S_CONFIG_DS_MASK_SHIFT(reg) (((reg) & I2S_CONFIG_DS_MASK) >> I2S_CONFIG_DS_SHIFT)
#define  I2S_CONFIG_DS_REPLACE_VAL(reg,val) (((reg) & ~I2S_CONFIG_DS_MASK) | (((uint32_t)val) << I2S_CONFIG_DS_SHIFT))

//====================================================================
//Register: CLOCK DIVIDE REGISTER (CLK_DIVIDE)
/** \brief I2S CLOCK DIVIDE Register*/
//====================================================================

#define  I2S_CLK_DIVIDE_RESERVED1_MASK 0xffffff80
#define  I2S_CLK_DIVIDE_RESERVED1_SHIFT 7
#define  I2S_CLK_DIVIDE_RESERVED1_MASK_SHIFT(reg) (((reg) & I2S_CLK_DIVIDE_RESERVED1_MASK) >> I2S_CLK_DIVIDE_RESERVED1_SHIFT)
#define  I2S_CLK_DIVIDE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2S_CLK_DIVIDE_RESERVED1_MASK) | (((uint32_t)val) << I2S_CLK_DIVIDE_RESERVED1_SHIFT))

#define  I2S_CLK_DIVIDE_D_MASK 0x7f
#define  I2S_CLK_DIVIDE_D_SHIFT 0
#define  I2S_CLK_DIVIDE_D_MASK_SHIFT(reg) (((reg) & I2S_CLK_DIVIDE_D_MASK) >> I2S_CLK_DIVIDE_D_SHIFT)
#define  I2S_CLK_DIVIDE_D_REPLACE_VAL(reg,val) (((reg) & ~I2S_CLK_DIVIDE_D_MASK) | (((uint32_t)val) << I2S_CLK_DIVIDE_D_SHIFT))

//====================================================================
//Register: TRANSMIT DATA REGISTER (TXDATA)
/** \brief I2S Transmit Data Register*/
//====================================================================

#define  I2S_TXDATA_T_MASK 0xffffffff
#define  I2S_TXDATA_T_SHIFT 0
#define  I2S_TXDATA_T_MASK_SHIFT(reg) (((reg) & I2S_TXDATA_T_MASK) >> I2S_TXDATA_T_SHIFT)
#define  I2S_TXDATA_T_REPLACE_VAL(reg,val) (((reg) & ~I2S_TXDATA_T_MASK) | (((uint32_t)val) << I2S_TXDATA_T_SHIFT))

//====================================================================
//Register: RECIEVE DATA REGISTER (RXDATA)
/** \brief I2S Recieve Data Register*/
//====================================================================

#define  I2S_RXDATA_R_MASK 0xffffffff
#define  I2S_RXDATA_R_SHIFT 0
#define  I2S_RXDATA_R_MASK_SHIFT(reg) (((reg) & I2S_RXDATA_R_MASK) >> I2S_RXDATA_R_SHIFT)
#define  I2S_RXDATA_R_REPLACE_VAL(reg,val) (((reg) & ~I2S_RXDATA_R_MASK) | (((uint32_t)val) << I2S_RXDATA_R_SHIFT))

//====================================================================
//Register: INT PENDING REGISTER (IPR)
/** \brief I2S INT PENDING Register*/
//====================================================================

#define  I2S_IPR_RESERVED1_MASK 0xfffffff8
#define  I2S_IPR_RESERVED1_SHIFT 3
#define  I2S_IPR_RESERVED1_MASK_SHIFT(reg) (((reg) & I2S_IPR_RESERVED1_MASK) >> I2S_IPR_RESERVED1_SHIFT)
#define  I2S_IPR_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2S_IPR_RESERVED1_MASK) | (((uint32_t)val) << I2S_IPR_RESERVED1_SHIFT))

#define  I2S_IPR_RDY_MASK 0x4
#define  I2S_IPR_RDY_SHIFT 2
#define  I2S_IPR_RDY_MASK_SHIFT(reg) (((reg) & I2S_IPR_RDY_MASK) >> I2S_IPR_RDY_SHIFT)
#define  I2S_IPR_RDY_REPLACE_VAL(reg,val) (((reg) & ~I2S_IPR_RDY_MASK) | (((uint32_t)val) << I2S_IPR_RDY_SHIFT))

#define  I2S_IPR_TXRDY_MASK 0x2
#define  I2S_IPR_TXRDY_SHIFT 1
#define  I2S_IPR_TXRDY_MASK_SHIFT(reg) (((reg) & I2S_IPR_TXRDY_MASK) >> I2S_IPR_TXRDY_SHIFT)
#define  I2S_IPR_TXRDY_REPLACE_VAL(reg,val) (((reg) & ~I2S_IPR_TXRDY_MASK) | (((uint32_t)val) << I2S_IPR_TXRDY_SHIFT))

#define  I2S_IPR_RXDONE_MASK 0x1
#define  I2S_IPR_RXDONE_SHIFT 0
#define  I2S_IPR_RXDONE_MASK_SHIFT(reg) (((reg) & I2S_IPR_RXDONE_MASK) >> I2S_IPR_RXDONE_SHIFT)
#define  I2S_IPR_RXDONE_REPLACE_VAL(reg,val) (((reg) & ~I2S_IPR_RXDONE_MASK) | (((uint32_t)val) << I2S_IPR_RXDONE_SHIFT))

//====================================================================
//Register: ABORT REGISTER (ABORT)
/** \brief I2S ABORT REG.*/
//====================================================================

#define  I2S_ABORT_RESERVED1_MASK 0xfffffffe
#define  I2S_ABORT_RESERVED1_SHIFT 1
#define  I2S_ABORT_RESERVED1_MASK_SHIFT(reg) (((reg) & I2S_ABORT_RESERVED1_MASK) >> I2S_ABORT_RESERVED1_SHIFT)
#define  I2S_ABORT_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2S_ABORT_RESERVED1_MASK) | (((uint32_t)val) << I2S_ABORT_RESERVED1_SHIFT))

#define  I2S_ABORT_ST_MASK 0x1
#define  I2S_ABORT_ST_SHIFT 0
#define  I2S_ABORT_ST_MASK_SHIFT(reg) (((reg) & I2S_ABORT_ST_MASK) >> I2S_ABORT_ST_SHIFT)
#define  I2S_ABORT_ST_REPLACE_VAL(reg,val) (((reg) & ~I2S_ABORT_ST_MASK) | (((uint32_t)val) << I2S_ABORT_ST_SHIFT))

//====================================================================
//Register: INTERRUPT FORCE REGISTER (FORCE)
/** \brief I2S INTERRUPT FORCE REG.*/
//====================================================================

#define  I2S_FORCE_RESERVED1_MASK 0xfffffffc
#define  I2S_FORCE_RESERVED1_SHIFT 2
#define  I2S_FORCE_RESERVED1_MASK_SHIFT(reg) (((reg) & I2S_FORCE_RESERVED1_MASK) >> I2S_FORCE_RESERVED1_SHIFT)
#define  I2S_FORCE_RESERVED1_REPLACE_VAL(reg,val) (((reg) & ~I2S_FORCE_RESERVED1_MASK) | (((uint32_t)val) << I2S_FORCE_RESERVED1_SHIFT))

#define  I2S_FORCE_FT_MASK 0x2
#define  I2S_FORCE_FT_SHIFT 1
#define  I2S_FORCE_FT_MASK_SHIFT(reg) (((reg) & I2S_FORCE_FT_MASK) >> I2S_FORCE_FT_SHIFT)
#define  I2S_FORCE_FT_REPLACE_VAL(reg,val) (((reg) & ~I2S_FORCE_FT_MASK) | (((uint32_t)val) << I2S_FORCE_FT_SHIFT))

#define  I2S_FORCE_FR_MASK 0x1
#define  I2S_FORCE_FR_SHIFT 0
#define  I2S_FORCE_FR_MASK_SHIFT(reg) (((reg) & I2S_FORCE_FR_MASK) >> I2S_FORCE_FR_SHIFT)
#define  I2S_FORCE_FR_REPLACE_VAL(reg,val) (((reg) & ~I2S_FORCE_FR_MASK) | (((uint32_t)val) << I2S_FORCE_FR_SHIFT))

#endif // I2S
