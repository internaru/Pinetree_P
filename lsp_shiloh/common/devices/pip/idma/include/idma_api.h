/****************************************************************************** 
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the IDMA_SLI block.
  * 
 **/

#ifndef __IDMA_FUNC_H_INCLUDED
#define __IDMA_FUNC_H_INCLUDED

#include <stdint.h>
#include "asic.h"
#include "regAddrs.h"
#include "logger.h"
#include "lassert.h"


/// lsptodo:
/// make base class for a descriptor
/// PA_IDMA and LPC_IDMA support
///


#ifndef idma_odma_descriptor_t
typedef struct idma_odma_descriptor_s
{
#define idma_odma_descriptor_T_BaseClassMembers \
    uint32_t config; \
    uint32_t src_hw; \
    uint32_t len; \
    uint32_t next_hw; 
    idma_odma_descriptor_T_BaseClassMembers;
} idma_odma_descriptor_t;


typedef struct lpc_idma_descriptor_s
{
    idma_odma_descriptor_T_BaseClassMembers;
    uint32_t lines; 
    uint32_t width;
    uint32_t lastline;
    uint32_t src_2; 
    uint32_t src_v; 
    uint32_t next_v; 
    uint32_t this_v; 
    uint32_t this_hw; 

} lpc_idma_descriptor_t;


#endif 

void idma_setup(PA_IDMA_REGS_t *idma_reg, 
                void *desc_addr_v, 
                /* add hw address ? */
                uint32_t data_addr, uint32_t xferCnt, uint32_t line_width);
void idma_setup_strip(PA_IDMA_REGS_t *idma_reg, 
                      void *desc_addr_v, //virtual address as void to allow inherited overlays of the registers. 
                      void *desc_addr_hw, // hw address, currently not used as no chains are used.
                      uint32_t data_addr, uint32_t xferCnt, uint32_t line_width, uint32_t first, uint32_t last);
void idma_setupinten(PA_IDMA_REGS_t *idma_reg, uint32_t rst_en, uint32_t eoi_en, uint32_t fin_en, uint32_t who_en);
void idma_start(PA_IDMA_REGS_t *idma_reg, uint32_t desc_addr_hw);
void idma_stop(PA_IDMA_REGS_t *idma_reg);
void idma_isr(PA_IDMA_REGS_t *idma_reg);
void idma_waitint(PA_IDMA_REGS_t *idma_reg, const uint32_t TIMEOUT);
void idma_printdescr(uint32_t desc_addr);

#endif


