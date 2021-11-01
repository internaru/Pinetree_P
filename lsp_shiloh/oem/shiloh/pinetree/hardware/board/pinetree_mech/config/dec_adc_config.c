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
 * \file dec_adc_4729.c
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "uio_lib_api.h"
#include "dec_adc_api.h"
#include "dec_adc_priv.h"
#include "key_table.h"
#include "regAddrs.h"
#include "lassert.h"
#include "logger.h"
#include "debug.h"

//#define VREF_SEL   0               // use internal vref on B0 or greater /* [KSF080902] HW new therm (0: internal , 1: external) *//* [KSF082602] for external & internal */
#define ADC_REF_mV 2000            // 1.7volt clamp on external ADC input voltage.
#define ADC_REF_mV_clippoint 2000  // max input voltage


#define MAX_ADC 1

static dec_adc_t my_adcs[MAX_ADC];

static dec_adc_t *myadcs[] = { 
    &my_adcs[0], 
    0
};

const const char * dec_adc_key_table[] = 
{
    "fuser_temperature",  ///Fuser temperature is sampled on adc pin 1
    0
};

KEY_TABLE_DEFINE_PRIVATE_2( dec_adc, dec_adc_t, dec_adc_key_table, myadcs );

void dec_adc_local_init(void)
{
  volatile ADC_REGS_REGS_t *adc;
  adc = dec_adc_get_regs_base();
  uint32_t adc_instruct_reg = 0;

#if 1 /* [KSF082602] for external & internal */
	FILE	*thermFP;
    
    thermFP = fopen("/app/thermistorExternalInput.txt", "r");

    if(thermFP <= 0)
    {	
		printf("[1].ThermistorInternalInput Mode (default)\n");
		  initialize_SARADC_control(7, /*strt_sel */
                            7, /*strt_done */
                            7, /*strt_low */
                            0, /*ckscaler */
                            3, /*clk_sel */
                            0, /*rotate_sel */
                            0, /*test_sar */
                            0, /*vref_sel */
                            1 /*pu_saradc */
                            );
	
	}
	else
	{		
		printf("[2].ThermistorExternalInput Mode\n");
		fclose(thermFP);
		  initialize_SARADC_control(7, /*strt_sel */
                            7, /*strt_done */
                            7, /*strt_low */
                            0, /*ckscaler */
                            3, /*clk_sel */
                            0, /*rotate_sel */
                            0, /*test_sar */
                            1, /*vref_sel */
                            1 /*pu_saradc */
                            );
	}
#endif

#if 0 /* [KSF082602] for external & internal */
  initialize_SARADC_control(7, /*strt_sel */
                            7, /*strt_done */
                            7, /*strt_low */
                            0, /*ckscaler */
                            3, /*clk_sel */
                            0, /*rotate_sel */
                            0, /*test_sar */
                            VREF_SEL, /*vref_sel */
                            1 /*pu_saradc */
                            );
 #endif
  initialize_ADC_control(0, /* part_type */
                         36,/* spi_desel_delay */
                         77,/* clk_divisor */
                         1  /* interface_mode */
                         );

  //configure ContTimebaseConfig

  adc->ContTimebaseConfig = ADC_REGS_CONTTIMEBASECONFIG_PERIOD_REPLACE_VAL(adc->ContTimebaseConfig, 25);

  //TriggerConfig would go here
  //threshold min/max would go here

  //program instruction table
  //reads SARADC channels (same as adc pin) 1 to 3 and puts results in RXData0-2 respectively
  //RXDataN is based on Instruction#, not channel.  So, data read in Instruction0 will go on RXData0.
 
  //read adc pin 0 and continue
  adc_instruct_reg = create_instruction_cfg_reg( 0, /* tag *//*cont*/
                                                 0, /* min */
                                                 0, /* max */
                                                 0, /* accum */
                                                 0, /* fifo */
                                                 0, /* cont */
                                                 0, /* N_samples */
                                                 0, /* buf_en */
                                                 0, /* ext_buf */
                                                 0 /* chan */
                                                 );
  adc->Instruction0 = adc_instruct_reg;

  // don't need this since we have continue off and are only using one channel.
  adc->Instruction1= ADC_REGS_INSTRUCTION0_CONT_REPLACE_VAL(adc_instruct_reg, 0);


  //program Priority Control Table
  // 
  // Priority_15 is continuous sampling. 
  // So, by programming instruction0 into the Priority_15 register and having the cont bit set on
  // instruction0 and instruction1, and turned off in Instruction2, this will cause the SARADC to 
  // run instruction0, sampling pin0 and putting the results in RXData0, 

  //disabled   // run instruction1, sampling pin2 and putting the results in RXData1, and 
  //disabled   // run instruction2, sampling pin3 and putting the results in RXData2, 
  // then stop and wait for the next sampling period (1 mSec) and start over again.
  
  adc->Priority_15 = ADC_REGS_PRIORITY_15_INSTR_ADDR_REPLACE_VAL(adc->Priority_15, 0);
  adc->Priority_15 = ADC_REGS_PRIORITY_15_TRIGGER_SRC_REPLACE_VAL(adc->Priority_15, 0xf);
  adc->Priority_15 = ADC_REGS_PRIORITY_15_ENABLE_REPLACE_VAL(adc->Priority_15, 1);

  //Fuser Temp is attached to ADC pin 0
  my_adcs[0].reg = &adc->RXData0;
  my_adcs[0].pin_number = 0;
  dec_adc_set_scale_factor(&my_adcs[0], ADC_REF_mV_clippoint, 2000, ADC_REF_mV);

}

/**
 * \brief Get dec adc registers memory mapped to user space using dec_adc UIO kernel driver.
 * 
 */
ADC_REGS_REGS_t* dec_adc_get_regs_base()
{
    static void *adc_dev = NULL;
    static ADC_REGS_REGS_t* adc_regs = NULL;

    if(adc_dev == NULL)
    {
        adc_dev = uio_open_dev("dec_adc");
        XASSERT(adc_dev != NULL, errno);
    }

    if(adc_regs == NULL)
    {
        // get dec_adc map
        uio_map_t *map = uio_get_map(adc_dev, 0);

        if (map != NULL)
        {
            adc_regs = map->virt_addr;
        }
    }
    
    DBG_ERR("%s = %#x\n", __func__, adc_regs);

    return adc_regs;
}
