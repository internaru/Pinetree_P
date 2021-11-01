/******************************************************************************
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file logger_common.c
 *
 * \brief common registration of submodule names.
 *
 */
#include "logger.h"

// convert macro to register submodules
// usage: copy the line from the C file to this file to register the submodule by name.
#define SUBMODULE( module, submodule_name, submodule_id ) \
    logger_submodule_register( module, submodule_id, submodule_name )

void logger_common_register(void)
{
    SUBMODULE( DEBUG_LOGGER_MODULE_PRINT,      "main", 0 );
    {
	SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "parser", 7 ); // parser/common
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "printstat", 8 ); // printstat 
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "pjl", 9 ); // PJL 
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "passthru", 10 ); // pass through parser
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "printtools", 11 ); 
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "JM", 12 ); // JobMgr
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "PM", 13 ); // PrintMgr
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "EM", 14 ); // ??
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "consumable", 15 );
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "motors", 16 ); // move to devices?
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "zj", 17 ); // zj parser
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "pip", 18 ); // print image pipe
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "printmode", 19 );
        SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "eng", 20 );
	SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "zxdrawer", 21 );
	SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "ggs", 22 ); // global graphics parser
	SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "urf", 23 ); // urf parser
	SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "vcp", 24 ); // virtual control panel debug only.

	SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "pf", 25 ); // pf parser
	SUBMODULE( DEBUG_LOGGER_MODULE_PRINT, "pcfax", 26 ); // pcfax parser
    }

    SUBMODULE( DEBUG_LOGGER_MODULE_CNTRL_PANEL, "main", 0 ); // add kinoma sub modules?
    SUBMODULE( DEBUG_LOGGER_MODULE_USB_DEVICE,  "main", 0 ); // move to devices? 
    SUBMODULE( DEBUG_LOGGER_MODULE_NVRAM,       "main", 0 ); // move to devices?

    SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM,      "main", 0 );
    {
	    SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "ssl", 1 );
        SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "dma_alloc", 6 );
        SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "audio", 7 );
	SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "logger", 9 );
	SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "spi", 11 ); // move to devices?
        SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "sjm", 12 );
        SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "rm", 13 );
        SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "cm", 14 ); 
	SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "mlim", 24 );
	SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "mem", 25 );
	SUBMODULE( DEBUG_LOGGER_MODULE_SYSTEM, "statusmgr", 26 ); // isn't this dead?
    }

    SUBMODULE( DEBUG_LOGGER_MODULE_CONSUMABLES, "main", 0 );


    SUBMODULE( DEBUG_LOGGER_MODULE_GPIO,        "main", 0 ); // move to devices?
    SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,        "main", 0 ); // sub modules?
    {
        SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,     "ui", 	1 );	//scan app (UI Scan)
        SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,     "copy", 	2 );	//copy app
        SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,     "lib", 	3 );	//scan lib(app)
        SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,     "man", 	4 );	//scan man(app)
        SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,     "pc", 	5 );	//scan app (PC Scan)
        SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,     "task", 	6 );	//scan task
        SUBMODULE( DEBUG_LOGGER_MODULE_SCAN,     "mech", 	7 );	//scan mech_pinetree
    }
    SUBMODULE( DEBUG_LOGGER_MODULE_ENGINE,      "main", 0 ); // move to video?

    SUBMODULE( DEBUG_LOGGER_MODULE_DPRINTF,     "main", 0 ); // shouldn't this die?

    SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "main", 0 );
    {
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "gpio_led", 1 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "gpiologger", 2 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "dec_adc", 3 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "dec_laser", 4 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "dec_fuser", 5 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "dec_sensor", 6 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "dcmotor", 7 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "stepper", 8 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "pwm", 9 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "delay", 10 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "eth_mac", 11 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "eth_phy", 12 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "cdma", 13 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "jbig", 14 );
        SUBMODULE( DEBUG_LOGGER_MODULE_DEVICES,     "uio", 15 );
    }

    SUBMODULE( DEBUG_LOGGER_MODULE_VIDEO,       "main", 0 );
    {
        SUBMODULE( DEBUG_LOGGER_MODULE_VIDEO,       "video", 1 );
        SUBMODULE( DEBUG_LOGGER_MODULE_VIDEO,       "video_laser", 2 );
    }
    SUBMODULE( DEBUG_LOGGER_MODULE_JBIG,        "main", 0 ); // move to devices/jbig


    logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 0, "main" );
    {
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 1,  "link" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 2,  "net_io" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 3,  "raw_io" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 4,  "print" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 5,  "scan" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 6,  "sm_job" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 7,  "ipp" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 8,  "gcpp" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 9,  "wsd" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 10, "telnet" );
        logger_submodule_register( DEBUG_LOGGER_MODULE_NETWORK, 11,  "iface" );
    }



    SUBMODULE( DEBUG_LOGGER_MODULE_HTTP,        "main", 0 ); 
    {
        SUBMODULE( DEBUG_LOGGER_MODULE_HTTP,        "http", 26 );
        SUBMODULE( DEBUG_LOGGER_MODULE_HTTP,        "methods", 25 );
        SUBMODULE( DEBUG_LOGGER_MODULE_HTTP,        "ssi", 24 );
        SUBMODULE( DEBUG_LOGGER_MODULE_HTTP,        "oid", 23 );
        SUBMODULE( DEBUG_LOGGER_MODULE_HTTP,        "usb", 22 );
        SUBMODULE( DEBUG_LOGGER_MODULE_HTTP,        "net", 21 );
    }
    SUBMODULE( DEBUG_LOGGER_MODULE_FILESYSTEM,  "main", 0 ); // 

    SUBMODULE( DEBUG_LOGGER_MODULE_GENERATORS,  "main", 0 ); // move to video? 
    {
        SUBMODULE( DEBUG_LOGGER_MODULE_GENERATORS,  "gen_comp", 1 );
        SUBMODULE( DEBUG_LOGGER_MODULE_GENERATORS,  "gen_decomp", 2 );
        SUBMODULE( DEBUG_LOGGER_MODULE_GENERATORS,  "gen_mip", 3 );
        SUBMODULE( DEBUG_LOGGER_MODULE_GENERATORS,  "gen_pip", 4 );
        SUBMODULE( DEBUG_LOGGER_MODULE_GENERATORS,  "gen_nup", 5 );
    }
    SUBMODULE( DEBUG_LOGGER_MODULE_OEM1,  "main", 0 );
    SUBMODULE( DEBUG_LOGGER_MODULE_OEM2,  "main", 0 );
    SUBMODULE( DEBUG_LOGGER_MODULE_OEM3,  "main", 0 );
    SUBMODULE( DEBUG_LOGGER_MODULE_OEM4,  "main", 0 );
	SUBMODULE( DEBUG_LOGGER_MODULE_INTERNAL, "main", 0);
	{
        SUBMODULE( DEBUG_LOGGER_MODULE_INTERNAL, "report", 1 );
	}
#ifdef HAVE_IMAGELOG
	SUBMODULE( DEBUG_LOGGER_MODULE_IMAGELOG, "main", 0);
	{
        SUBMODULE( DEBUG_LOGGER_MODULE_IMAGELOG, "image_log", 1 );
	}
#endif
}
