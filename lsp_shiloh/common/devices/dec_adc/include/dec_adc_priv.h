#ifndef DEC_ADC_PRIV_H
#define DEC_ADC_PRIV_H
/* 
 *
 * ============================================================================
 * Copyright (c) 2010-2010  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/**
 *
 * \file dec_adc_priv.h
 *
 */

#include <stdint.h>
#include "adc_regstructs.h"
#include "adc_regmasks.h"
#include "logger.h"
#include "devices_logger.h"

#define DBG_PRFX "ADC: "         // simple string prepend to the front of all strings.
//#define LOGGER_SUBMOD 0x400  // (0x1<<10) DEVICES:ADC  submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_DEC_ADC

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)          // LOG_ERR is the level   
#define DBG_DEBUG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 
#define DBG_LOG_ERR(...) DBG_MEMLOG(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_LOG_DEBUG(...) DBG_MEMLOG(LOG_DEBUG, DBG_PRFX __VA_ARGS__) 

struct dec_adc_s
{
    volatile uint32_t *reg;   
    uint32_t external_voltage_max_mV;   
    uint32_t pin_input_max_mV;
    uint32_t scale_num;
    uint32_t scale_denom;
    uint32_t pin_number;
};

void dec_adc_local_init(void);


void initialize_SARADC_control(uint32_t strt_sel, 
                               uint32_t strt_done, 
                               uint32_t strt_low,
                               uint32_t ckscaler,
                               uint32_t clk_sel,
                               uint32_t rotate_sel,
                               uint32_t test_sar,
                               uint32_t vref_sel,
                               uint32_t pu_saradc
                               );

void initialize_ADC_control(uint32_t part_type,
                            uint32_t spi_desel_delay,
                            uint32_t clk_divisor,
                            uint32_t interface_mode);

uint32_t create_instruction_cfg_reg(uint32_t tag,
                                    uint32_t max,
                                    uint32_t min,
                                    uint32_t accum,
                                    uint32_t fifo,
                                    uint32_t cont,
                                    uint32_t N_samples,
                                    uint32_t buf_en,
                                    uint32_t extern_buf,
                                    uint32_t chan);

ADC_REGS_REGS_t* dec_adc_get_regs_base();

#endif // DEC_ADC_PRIV_H
