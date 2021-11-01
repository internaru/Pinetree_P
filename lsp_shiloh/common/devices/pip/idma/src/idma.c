/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the IDMA_SLI block.
  * 
 **/

#ifndef __IDMA_FUNC_C_INCLUDED
#define __IDMA_FUNC_C_INCLUDED

#include <stdint.h>
#include "idma_api.h"
#include "logger.h"


#define DBG_PRFX "IDMA: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )


void idma_setup(PA_IDMA_REGS_t *idma_reg, void *desc_addr, uint32_t data_addr, uint32_t xferCnt, uint32_t line_width)
{
    // Setup Descriptor
    *(volatile uint32_t*)desc_addr = 0x000100C3;
    *(volatile uint32_t*)(desc_addr+4) = data_addr;
    *(volatile uint32_t*)(desc_addr+8) = xferCnt;
    *(volatile uint32_t*)(desc_addr+12) = 0x0;

    idma_reg->int_en = 0xFFFFFFFF;
    DBG_PRINTF_NOTICE("IDMA line width = 0x%08x\n\r", idma_reg->line_width);
    idma_reg->line_width = line_width;
    DBG_PRINTF_NOTICE("IDMA line width = 0x%08x\n\r", idma_reg->line_width);
    idma_reg->cfg = 0x00010500;  // 32-bit data / bursts of 4 lw
}

void idma_setup_strip(PA_IDMA_REGS_t *idma_reg, void *desc_addr, void *desc_addr_hw, uint32_t data_addr, uint32_t xferCnt, uint32_t line_width, uint32_t first, uint32_t last)
{
    // Setup Descriptor
    *(volatile uint32_t*)desc_addr = 0x00010003;
    *(volatile uint32_t*)desc_addr |= (first ? 0x40 : 0x00);
    *(volatile uint32_t*)desc_addr |= (last ? 0x80 : 0x00); 
    *(volatile uint32_t*)(desc_addr+4) = data_addr;
    *(volatile uint32_t*)(desc_addr+8) = xferCnt;
    *(volatile uint32_t*)(desc_addr+12) = 0x0;
    idma_reg->int_en = 0xFFFFFFFF;
    DBG_PRINTF_INFO("IDMA line width = 0x%08x\n\r", idma_reg->line_width);
    idma_reg->line_width = line_width;
    DBG_PRINTF_INFO("IDMA line width = 0x%08x\n\r", idma_reg->line_width);
    idma_reg->cfg = 0x00010500;  // 32-bit data / bursts of 4 lw
}

void idma_setupinten(PA_IDMA_REGS_t *idma_reg,
                     uint32_t rst_en,
                     uint32_t eoi_en,
                     uint32_t fin_en,
                     uint32_t who_en)
{
    unsigned int reg_value;

    // Write the INT_EN Register
    reg_value = idma_reg->int_en;
    reg_value = PA_IDMA_INT_EN_RST_INT_EN_REPLACE_VAL(reg_value,rst_en);
    reg_value = PA_IDMA_INT_EN_EOI_INT_EN_REPLACE_VAL(reg_value,eoi_en);
    reg_value = PA_IDMA_INT_EN_FIN_INT_EN_REPLACE_VAL(reg_value,fin_en);
    reg_value = PA_IDMA_INT_EN_WHO_INT_EN_REPLACE_VAL(reg_value,who_en);
    idma_reg->int_en = reg_value;
}

void idma_start(PA_IDMA_REGS_t *idma_reg, uint32_t desc_addr_hw)
{
    idma_reg->desc_write = desc_addr_hw;
}

#define IDMA_BUSY_VAL 0x01

void idma_stop(PA_IDMA_REGS_t *idma_reg)
{
    idma_reg->cfg = 0x00000000; // Take away IDMA enable
    // Acknowelege pending interrupts
    idma_reg->int_cl = idma_reg->int_st;
    // Wait for busy signal to go away
    while (idma_reg->status & IDMA_BUSY_VAL)
    ;
    // Acknowelege pending interrupts
    idma_reg->int_cl = idma_reg->int_st;
}

// called from pip_isr
void idma_isr(PA_IDMA_REGS_t *idma_reg)
{
    uint32_t reg_value;

    reg_value = (idma_reg->int_en & idma_reg->int_st);
    // Soft Reset Complete Interrupt
    if (reg_value & PA_IDMA_INT_EN_RST_INT_EN_MASK) 
    {
        DBG_PRINTF_INFO("IDMA ISR : Soft Reset Complete Interrupt Fired\n\r");
    }
    // EOI Interrupt
    if (reg_value & PA_IDMA_INT_EN_EOI_INT_EN_MASK) 
    {
        DBG_PRINTF_INFO("IDMA ISR : EOI Interrupt Fired\n\r");
    }
    // Finish Interrupt
    if (reg_value & PA_IDMA_INT_EN_FIN_INT_EN_MASK) 
    {
        DBG_PRINTF_INFO("IDMA ISR : Finish Interrupt Fired\n\r");
    }
    // Who Owns Interrupt
    if (reg_value & PA_IDMA_INT_EN_WHO_INT_EN_MASK) 
    {
        DBG_PRINTF_NOTICE("IDMA ISR : Ownership Error Interrupt Fired\n\r");
        ASSERT(0);
    }
}


void idma_waitint(PA_IDMA_REGS_t *idma_reg, const uint32_t TIMEOUT)
{
    ASSERT(0 == 1 ); // not ported linux
    /*
    uint32_t start_time = tx_time_get() * 10;
    uint32_t run_ISR = 1;

    while((idma_reg->int_st & idma_reg->int_en) == 0x00000000)
    {
        if((tx_time_get()*10 - start_time) > TIMEOUT)
        {
            DBG_PRINTF_NOTICE("ERROR: Timeout on idma_waitint\n\r");
            run_ISR = 0;
            break;
        }
    }
    if (run_ISR)
        idma_isr(idma_reg);
    */
}

void idma_printdescr(uint32_t desc_addr)
{
    DBG_PRINTF_NOTICE("Descr Addr: %08x\n\r", desc_addr);
    DBG_PRINTF_NOTICE("%08x\n\r", *(volatile unsigned int*)(desc_addr +0x0));
    DBG_PRINTF_NOTICE("%08x\n\r", *(volatile unsigned int*)(desc_addr +0x4));
    DBG_PRINTF_NOTICE("%08x\n\r", *(volatile unsigned int*)(desc_addr +0x8));
    DBG_PRINTF_NOTICE("%08x\n\r", *(volatile unsigned int*)(desc_addr +0xc));
}

#endif
