/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** \file dec_sensor_config.c
 * example code for using dec_sensor 
 *
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "dec_sensor.h"
#include "dec_sensor_priv.h"

#include "lassert.h"
#include "logger.h"
#include "debug.h"

#include "uio_lib_api.h"
#include "regAddrs.h"
#include "apb_config_regmasks.h"
#include "apb_config_regstructs.h"
#include "DEC_CONFIG_regmasks.h"
#include "DEC_CONFIG_regstructs.h"
#include "GPIO_regmasks.h"
#include "GPIO_regstructs.h"

//// DEC_SENSOR_DEFINES

#define PAPER_MAIN_TRAY_DEC_SENSOR  0
#define PAPER_MPT_TRAY_DEC_SENSOR   1
#define PAPER_ENTRY_DEC_SENSOR      2
#define PAPER_EXIT_DEC_SENSOR       3


#define PAPER_OUTBIN_FULL_DEC_SENSOR 4
#define PAPER_FEED_DEC_SENSOR       5

#define MAX_SENSOR 4

static dec_sensor_t *mysensors[MAX_SENSOR] = {0,};

const const char * dec_sensor_key_table[] = 
{
    "paper_main_tray_sensor",   // EMPTY_S
    "paper_mpt_tray_sensor",    // MPT_EMPTY_S
    "paper_entry_sensor",       // INPUT
     "paper_exit_sensor",        // EXIT

    //"paper_outbin_full_sensor", // nSENS_OUTBIN_FULL
    // "paper_feed_sensor",        // nSENS_P_FEED




//-jrs    "cartridge_sensor",
//-jrs    "front_door_sensor",
//-jrs    "paper_available_sensor",
    0
};
KEY_TABLE_DEFINE_PRIVATE_3( dec, sensor, dec_sensor_t, dec_sensor_key_table, mysensors );


// INITIALIZE_DEC
    /// customer provided.
void dec_sensor_local_init()
{
    /// customer provided, as each board will wire the sensors differently.
    uint32_t fcfg_active_low;
    uint32_t fcfg_active_high;
    uint32_t cfg_active_low;
    uint32_t cfg_active_high;
    uint32_t cfg_active_low_blip;

    // create the filter parameters configuration the sensor(s)
    // stefan foo: reverse the filters
    fcfg_active_low = sensor_filter_config( 10, 1, 1, 10 ); 
    fcfg_active_high = sensor_filter_config( 1, 10, 10, 1 ); 

    cfg_active_high = sensor_config( SENSOR_ALTIN_Normal, 
				     SENSOR_OutputSense_Normal, 
				     SENSOR_Timebase_1us, 
				     SENSOR_INTTYPE_Both, 
				     SENSOR_OUTTYPE_Normal);
    // create the rest of the sensor configuration parameters
    cfg_active_low = sensor_config( SENSOR_ALTIN_Normal, 
				    SENSOR_OutputSense_Inverted, // active low.
				    SENSOR_Timebase_1us, 
				    SENSOR_INTTYPE_Both, 
				    SENSOR_OUTTYPE_Normal); // can be polled
    // interrupt only no polling
    cfg_active_low_blip = sensor_config( SENSOR_ALTIN_Normal, 
					 SENSOR_OutputSense_Normal, 
					 SENSOR_Timebase_1us, 
					 SENSOR_INTTYPE_Both, 
					 SENSOR_OUTTYPE_Fall); 

    // configure the registers, creating an isr connection to the provided function,
    // that will fire the semaphone on triggered signal.


    mysensors[PAPER_MAIN_TRAY_DEC_SENSOR] = dec_sensor_create( 6, fcfg_active_low, 100, cfg_active_low, dec_sensor_unblock );
    //   mysensors[PAPER_MAIN_TRAY_DEC_SENSOR] = dec_sensor_create_gpio( 10, GPIO_BANK_F, 24, GPIO_LOGIC_SETTING_ACTIVE_LOW);
    ASSERT(mysensors[PAPER_MAIN_TRAY_DEC_SENSOR] != 0);

    mysensors[PAPER_MPT_TRAY_DEC_SENSOR] = dec_sensor_create( 5, fcfg_active_high, 100, cfg_active_high, dec_sensor_unblock );
    // mysensors[PAPER_MPT_TRAY_DEC_SENSOR] = dec_sensor_create_gpio( 11, GPIO_BANK_F, 23, GPIO_LOGIC_SETTING_ACTIVE_HIGH);
    ASSERT(mysensors[PAPER_MPT_TRAY_DEC_SENSOR] != 0);

    // mysensors[PAPER_ENTRY_DEC_SENSOR] = dec_sensor_create_gpio( 12, GPIO_BANK_F, 22, GPIO_LOGIC_SETTING_ACTIVE_LOW);
    mysensors[PAPER_ENTRY_DEC_SENSOR] = dec_sensor_create( 4, fcfg_active_low, 100, cfg_active_low_blip, dec_sensor_unblock );
    ASSERT(mysensors[PAPER_ENTRY_DEC_SENSOR] != 0);

    mysensors[PAPER_EXIT_DEC_SENSOR] = dec_sensor_create_gpio( 13, GPIO_BANK_F, 12, GPIO_LOGIC_SETTING_ACTIVE_LOW);
    ASSERT(mysensors[PAPER_EXIT_DEC_SENSOR] != 0);

#if 0 


    ASSERT(mysensors[PAPER_FEED_DEC_SENSOR] != 0);

#if defined(BOARD_TYPE_MECH)
    mysensors[PAPER_OUTBIN_FULL_DEC_SENSOR] = dec_sensor_create_gpio( 6, GPIO_BANK_E, 28, GPIO_LOGIC_SETTING_ACTIVE_LOW);

#elif defined(BOARD_TYPE_FORM_FACTOR)
    mysensors[PAPER_OUTBIN_FULL_DEC_SENSOR] = dec_sensor_create( 14,
                                                          fcfg, 100, cfg, 
                                                          dec_sensor_unblock );
#else
    #error "Unknown BOARD_TYPE"
#endif

    ASSERT(mysensors[PAPER_OUTBIN_FULL_DEC_SENSOR] != 0);


#endif
    // update the cfg parameter for no interrupts 
    //cfg = sensor_config( SENSOR_ALTIN_Normal, SENSOR_OutputSense_Normal, SENSOR_Timebase_1us, SENSOR_INTTYPE_NONE, SENSOR_OUTTYPE_Normal);
//    cfg = SENSOR_REGS_CFG0_INTTYPE_REPLACE_VAL(cfg,SENSOR_INTTYPE_NONE);
}

void *dec_sensor_get_dev()
{
    static void *sensor_dev = NULL;
    
    if(sensor_dev == NULL)
    {
        sensor_dev = uio_open_dev("dec_sensor");
        XASSERT(sensor_dev != NULL, errno);
    }
    
    return sensor_dev;
}

/**
 * \brief Get dec sensor registers memory mapped to user space using dec_sensor UIO kernel driver.
 * 
 */
DEC_SENSOR_REGS_t* dec_sensor_get_regs_base()
{
    static DEC_SENSOR_REGS_t* sensor_regs = NULL;

    if(sensor_regs == NULL)
    {
        // get dec_sensor map
        uio_map_t *map = uio_get_map(dec_sensor_get_dev(), 0);

        if (map != NULL)
        {
            sensor_regs = map->virt_addr;
        }
    }
    
    DBG_DEBUG("%s = %#x\n", __func__, sensor_regs);

    return sensor_regs;
}

// example code. 
//
//    dec_sensor_enable_isr( paper_entry_sensor );
//    int err = sem_wait(paper_entry_semaphore);
//    tx_semaphore_get(&paper_entry_semaphore, 0); // block forever
//    dec_sensor_poll_num(2);
//    if (dec_sensor_poll(paper_entry_sensor))
// 

