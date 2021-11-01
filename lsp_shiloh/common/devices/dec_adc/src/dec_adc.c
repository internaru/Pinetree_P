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
 * \file dec_adc.c
 *
 */

#include <stdlib.h> // atoi

#include "lassert.h"
#include "regAddrs.h"
#include "logger.h"
#include "debug.h"
#include "dec_adc_priv.h"
#include "dec_adc_api.h"
#include "cmd_proc_api.h"

// serial debug callback function.
int dec_adc_dbcallback( int argc, char *argv[] );

#define MAX_ADC_BITS 10


void dec_adc_get_scale_factor(const dec_adc_t * adc,
                              uint32_t * pin_input_max_mV,
                              uint32_t * external_voltage_max_mV)
{
    DBG_ASSERT(adc);

    *external_voltage_max_mV = adc->external_voltage_max_mV;
    *pin_input_max_mV = adc->pin_input_max_mV;
}

void dec_adc_set_scale_factor(dec_adc_t * adc,
                              uint32_t pin_input_max_mV,
                              uint32_t external_voltage_max_mV,
                              uint32_t reference_voltage)
{
    DBG_ASSERT(adc);
    DBG_ASSERT(external_voltage_max_mV != 0);
    DBG_ASSERT(pin_input_max_mV != 0);

    adc->external_voltage_max_mV = external_voltage_max_mV;
    adc->pin_input_max_mV = pin_input_max_mV;
    
    adc->scale_num = adc->external_voltage_max_mV * reference_voltage;
    adc->scale_denom = ((0x1 << MAX_ADC_BITS) -1) * adc->pin_input_max_mV;
    while (adc->scale_num > 0xfffff) /* 20 * 10 = two bits of wiggle room */
    {
        adc->scale_num >>= 1;
        adc->scale_denom >>= 1;
    }
    DBG_ASSERT(adc->scale_denom != 0);
    DBG_DEBUG(" dec_adc Pin %d, Scan num %d, Scale denom %d \n", adc->pin_number, adc->scale_num, adc->scale_denom);
    dbg_printf(" dec_adc Pin %d, Scan num %d, Scale denom %d \n", adc->pin_number, adc->scale_num, adc->scale_denom);
}

uint32_t dec_adc_raw_read_channel(const dec_adc_t *adc) 
{
    DBG_ASSERT(adc);
 
    if (adc) 
    {
        return ((*adc->reg) & ADC_REGS_RXDATA0_DATA_MASK );
    }
    return 0;
}

uint32_t dec_adc_channel_pin_number(dec_adc_t *adc) 
{
    DBG_ASSERT(adc);

    if (adc) 
    {
        return adc->pin_number;
    }
    return 0;
}

uint32_t dec_adc_scaled_read_channel_mV(const dec_adc_t *adc) 
{
    DBG_ASSERT(adc);

    if (adc && adc->scale_denom) 
    {
        return ((((*adc->reg) & ADC_REGS_RXDATA0_DATA_MASK) * adc->scale_num)/adc->scale_denom) ;
    }
    return 0;
}

void prepare_adc_for_programming(void)
{
    volatile ADC_REGS_REGS_t *adc;
    adc = dec_adc_get_regs_base();
    //  Clear Int & Int2 reg
    adc->IntEna = 0;
    adc->Int2Ena = 0;

    //  Clear trig Config 0-14 Trig edge bits
#if ASIC_REV >= ASIC_REV_Z1
    adc->TriggerConfig0 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig0, 0);
    adc->TriggerConfig1 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig1, 0);
    adc->TriggerConfig2 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig2, 0);
    adc->TriggerConfig3 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig3, 0);
    adc->TriggerConfig4 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig4, 0);
    adc->TriggerConfig5 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig5, 0);
    adc->TriggerConfig6 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig6, 0);
    adc->TriggerConfig7 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig7, 0);
    adc->TriggerConfig8 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig8, 0);
    adc->TriggerConfig9 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig9, 0);
    adc->TriggerConfig10 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig10, 0);
    adc->TriggerConfig11 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig11, 0);
    adc->TriggerConfig12 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig12, 0);
    adc->TriggerConfig13 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig13, 0);
    adc->TriggerConfig14 = ADC_REGS_TRIGGERCONFIG0_TRIG_EDGE_REPLACE_VAL(adc->TriggerConfig14, 0);
#endif

    //Clear priority register enable bits (and everything else)
    adc->Priority0 = 0;
    adc->Priority1 = 0;
    adc->Priority2 = 0;
    adc->Priority3 = 0;
    adc->Priority4 = 0;
    adc->Priority5 = 0;
    adc->Priority6 = 0;
    adc->Priority7 = 0;
    adc->Priority8 = 0;
    adc->Priority9 = 0;
    adc->Priority10 = 0;
    adc->Priority11 = 0;
    adc->Priority12 = 0;
    adc->Priority13 = 0;
    adc->Priority14 = 0;
    adc->Priority_15 = 0;
}

void initialize_SARADC_control(uint32_t strt_sel, 
                               uint32_t strt_done, 
                               uint32_t strt_low,
                               uint32_t ckscaler,
                               uint32_t clk_sel,
                               uint32_t rotate_sel,
                               uint32_t test_sar,
                               uint32_t vref_sel,
                               uint32_t pu_saradc
                               )
{
    volatile ADC_REGS_REGS_t *adc;
    adc = dec_adc_get_regs_base();
    uint32_t tmp = 0;

    tmp = ADC_REGS_SARADC_CONTROL_RESET_REPLACE_VAL(adc->SARADC_Control, 1); // hold in reset
    tmp = ADC_REGS_SARADC_CONTROL_T_STRT_SEL_REPLACE_VAL(tmp, strt_sel); //Bus dependent
    tmp = ADC_REGS_SARADC_CONTROL_T_STRT_DONE_REPLACE_VAL(tmp, strt_done); //Bus dependent
    tmp = ADC_REGS_SARADC_CONTROL_T_STRT_LOW_REPLACE_VAL(tmp, strt_low); //Bus dependent
    tmp = ADC_REGS_SARADC_CONTROL_CKSCALER_REPLACE_VAL(tmp, ckscaler); //Bus clk dependent
    tmp = ADC_REGS_SARADC_CONTROL_CKSEL_REPLACE_VAL(tmp, clk_sel); //Bus clk dependent
    tmp = ADC_REGS_SARADC_CONTROL_R_ROTATE_SEL_REPLACE_VAL(tmp, rotate_sel); //no bit rotation
    tmp = ADC_REGS_SARADC_CONTROL_TST_SAR_REPLACE_VAL(tmp, test_sar); //tst mode off
    tmp = ADC_REGS_SARADC_CONTROL_VREF_SEL_REPLACE_VAL(tmp, vref_sel); // use external Vref for RevZ
    tmp = ADC_REGS_SARADC_CONTROL_PU_SARADC_REPLACE_VAL(tmp, pu_saradc); // power up
    adc->SARADC_Control = ADC_REGS_SARADC_CONTROL_RESET_REPLACE_VAL(tmp, 0); // release reset   

}

void initialize_ADC_control(uint32_t part_type,
                            uint32_t spi_desel_delay,
                            uint32_t clk_divisor,
                            uint32_t interface_mode)
{
    volatile ADC_REGS_REGS_t *adc;
    adc = dec_adc_get_regs_base();
    uint32_t tmp;
    tmp = ADC_REGS_ADC_CONTROL_RXFIFO1_ENABLE_REPLACE_VAL(adc->ADC_Control, 0);
    tmp = ADC_REGS_ADC_CONTROL_RXFIFO2_ENABLE_REPLACE_VAL(tmp, 0);
    tmp = ADC_REGS_ADC_CONTROL_PART_TYPE_REPLACE_VAL(tmp, part_type);      // 0=SARADC 1=SPI
    tmp = ADC_REGS_ADC_CONTROL_SPI_DESELDELAY_REPLACE_VAL(tmp,spi_desel_delay);  // 150ns @ 240MHz
    tmp = ADC_REGS_ADC_CONTROL_SPI_CLKDIVISOR_REPLACE_VAL(tmp,clk_divisor);  // 320ns @ 240MHz
    tmp = ADC_REGS_ADC_CONTROL_PART_TYPE_REPLACE_VAL(tmp,part_type);      // 0=SARADC
    adc->ADC_Control = ADC_REGS_ADC_CONTROL_INTERFACE_MODE_REPLACE_VAL(tmp,interface_mode);     // 1=automatic 
}

uint32_t create_instruction_cfg_reg(uint32_t tag,
                                    uint32_t max,
                                    uint32_t min,
                                    uint32_t accum,
                                    uint32_t fifo,
                                    uint32_t cont,
                                    uint32_t N_samples,
                                    uint32_t buf_en,
                                    uint32_t extern_buf,
                                    uint32_t chan)
{
    uint32_t adc_instruct_reg = 0;

    adc_instruct_reg = ADC_REGS_INSTRUCTION0_TAG_REPLACE_VAL(adc_instruct_reg, tag);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_MAX_REPLACE_VAL(adc_instruct_reg, max);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_MIN_REPLACE_VAL(adc_instruct_reg, min);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_ACCUM_REPLACE_VAL(adc_instruct_reg, accum);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_FIFO_REPLACE_VAL(adc_instruct_reg, fifo);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_CONT_REPLACE_VAL(adc_instruct_reg, cont); 
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_N_SAMPLES_REPLACE_VAL(adc_instruct_reg, N_samples);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_BUF_EN_REPLACE_VAL(adc_instruct_reg, buf_en);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_EXTMUX_REPLACE_VAL(adc_instruct_reg, extern_buf);
    adc_instruct_reg = ADC_REGS_INSTRUCTION0_CHAN_REPLACE_VAL(adc_instruct_reg, chan);

    return adc_instruct_reg;
}

void dec_adc_init(void) 
{
    prepare_adc_for_programming();
    dec_adc_local_init();
    REL_ASSERT( CMD_OK == cmd_register("adc",
                                       NULL,
                                       NULL,
                                       NULL,
                                       dec_adc_dbcallback));
}

int dec_adc_dbcallback( int argc, char *argv[] )
{
    int test;
    int i;
    const char *str;
    //int temp;

    if (argc < 2 )
    {
        dbg_printf("debug:dec_adc <run test number x> \n");
        for (i = 0, str = dec_adc_name_by_index(i); 
             str ; 
             i++, str = dec_adc_name_by_index(i) )
        {
            dbg_printf(" dec_adc [%d] %s:%d %d\n",
                       i, 
                       str,
                       dec_adc_raw_read_channel(dec_adc_by_index(i)),
                       dec_adc_scaled_read_channel_mV(dec_adc_by_index(i)));
        }
        
        /* not enough parameters */
        return 0;
    }

    test = atoi(argv[1]);

     
    switch (test ) 
    {
    case 1:
    {
#if 0
         if (argc < 2 )
             dbg_printf("debug:adc 1 \n");
         else {
             BlackClaw_testADC(void);
         }
#endif
        break;
    }
    case 2:
        break;
    case 3:
        break;
    }
    return 0;
}


