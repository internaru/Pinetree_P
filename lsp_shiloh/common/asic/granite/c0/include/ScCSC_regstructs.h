 /*
 *************************************************************************
 *
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *
 *                       Marvell Confidential
 *************************************************************************
 *************************************************************************
 * \file : ScCSC_regstructs.h
 *
 * NOTE: This file generated by RegBuild
 *
 * Generated on Friday, 07/10/09 at 02:17:17 PM PDT
 * using RegBuild 1.4
 *
 *************************************************************************
 */

#ifndef _SCCSC_REGSTRUCTS_H_
#define _SCCSC_REGSTRUCTS_H_

#include <stdint.h>

//====================================================================
//
//Register File: SubCube Color Space Conversion (ScCSC)
/** \brief Top-level register file for ScCSC*/
//
//====================================================================

typedef struct SCCSC_REGS_s
{
  volatile uint32_t STATUS;  ///< 0x0 [R]: ScCSC STATUS REG
  volatile uint32_t CNTRL;  ///< 0x4 [R/W]: ScCSC CONTROL REG
  volatile uint32_t MCClr;  ///< 0x8 [W]: Cache Miss Counters Clear
  volatile uint32_t LCClr;  ///< 0xc [W]: Cache Latency Counter Clear
  volatile uint32_t CACHE_INVLD;  ///< 0x10 [W]: INVALIDATE All Caches Register
  volatile uint32_t CACHE_CNTRL;  ///< 0x14 [R/W]: Cache Control Register
  volatile uint32_t IDX_MISS_COUNTER;  ///< 0x18 [R]: Cache Index Miss counter
  volatile uint32_t PIXEL_MISS_COUNTER;  ///< 0x1c [R]: Cache PIXEL Miss counter
  volatile uint32_t LATENCY_COUNTER;  ///< 0x20 [R]: Cache Latency counter
  volatile uint32_t BASE_ADDR;  ///< 0x24 [R/W]: Cached LUT Base Address
  volatile uint32_t WT;  ///< 0x28 [R/W]: White Threshold settings
  volatile uint32_t WR_REG0;  ///< 0x2c [R/W]: White Response reg 0 
  volatile uint32_t WR_REG1;  ///< 0x30 [R/W]: White Response reg 1 
  volatile uint32_t WR_REG2;  ///< 0x34 [R/W]: White Response reg 2 
  volatile uint32_t KR_REG0;  ///< 0x38 [R/W]: Black Response Reg 0 
  volatile uint32_t KR_REG1;  ///< 0x3c [R/W]: Black Response Reg 1 
  volatile uint32_t KR_REG2;  ///< 0x40 [R/W]: Black Response Reg 2 
  volatile uint32_t RAR;  ///< 0x44 [W]: SRAM Address 
  volatile uint32_t RDW0;  ///< 0x48 [R/W]: RAM Data Write 0
  volatile uint32_t RDW1;  ///< 0x4c [R/W]: RAM Data Write 1
  volatile uint32_t RDW2;  ///< 0x50 [R/W]: RAM Data Write 2
  volatile uint32_t RDR0;  ///< 0x54 [R]: RAM Data Read 0
  volatile uint32_t RDR1;  ///< 0x58 [R]: RAM Data Read 1
  volatile uint32_t RDR2;  ///< 0x5c [R]: RAM Data Read 2
} SCCSC_REGS_t;

#endif // SCCSC
