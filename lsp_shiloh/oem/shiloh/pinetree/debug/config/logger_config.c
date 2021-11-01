/******************************************************************************
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file logger_config.c
 *
 * \brief 
 *
 */

/**
 *
 * \file logger_config.c
 *
 * \brief 
 *
 */

#include "logger.h"

/// in logger_common.c
extern void logger_common_register(void); 

/// in logger.c but should only be called from here.
extern int logger_submodule_register( int module_index, int submodule_id, const char * module_name );

// convert macro to register submodules
// usage: copy the line from the C file to this file to register the submodule by name.
#define SUBMODULE( module, submodule_name, submodule_id ) \
    logger_submodule_register( module, submodule_id, submodule_name )

void logger_oem_register()
{
    // add per project registrations here.
    SUBMODULE( DEBUG_LOGGER_MODULE_GENERATORS, "rangetest", 26 );  // not real but registered
}

void logger_local_init()
{
    logger_common_register();
    logger_oem_register();

    // CMD==> logger reset 
    // should output a program you can paste here :

#if 0  // ==> No Log
	logger_set_1( "off" );
    
    /*logger_set_2( "USB", "LOG_ERR" );
    logger_set_2( "PRINT", "LOG_ERR" );

    logger_set_2( "JBIG", "LOG_WARNING" );
    logger_set_2( "NETWORK", "LOG_ERR" );
    logger_set_2( "HTTP", "LOG_CRIT" );
    logger_set_2( "NVRAM", "LOG_ERR");

    //logger_set_3( "VIDEO", "video", "LOG_DEBUG" );    
    //logger_set_3( "VIDEO", "video_laser", "LOG_DEBUG" );   
    logger_set_3( "DEVICES", "jbig", "LOG_ERR" );   
    logger_set_3( "DPRINTF", "main", "LOG_ERR" );
    logger_set_3( "SYSTEM", "logger", "LOG_ERR" );
    logger_set_3( "SYSTEM", "mem", "off" );
    logger_set_3( "SYSTEM", "cm", "LOG_INFO" );
    //logger_set_3( "SYSTEM", "dma_alloc", "LOG_INFO" );

    logger_set_3( "PRINT", "ggs", "LOG_ERR" ); 
    logger_set_3( "PRINT", "pf", "LOG_INFO" ); 
    logger_set_3( "PRINT", "printmode", "LOG_ERR" );
    logger_set_3( "PRINT", "printtools", "LOG_ERR" );
    logger_set_3( "PRINT", "eng", "LOG_DEBUG" );
    logger_set_3( "PRINT", "pm", "LOG_DEBUG" );
    logger_set_3( "PRINT", "pip", "LOG_ERR" );

    logger_set_3( "DEVICES", "dcmotor", "LOG_ERR" );
    logger_set_3( "DEVICES", "dec_laser", "LOG_ERR" );
    logger_set_3( "DEVICES", "dec_fuser", "LOG_ERR" );
    logger_set_3( "DEVICES", "uio", "LOG_ERR" );*/
#endif
    logger_set_1( "LOG_ERR" );
   // logger_set_1( "LOG_DEBUG" );

   logger_set_2( "USB", "LOG_ERR" );
    logger_set_2( "PRINT", "LOG_ERR" );

    logger_set_2( "JBIG", "LOG_WARNING" );
    logger_set_2( "NETWORK", "LOG_ERR" );
    logger_set_2( "HTTP", "LOG_CRIT" );
    logger_set_2( "NVRAM", "LOG_ERR");

	logger_set_2( "INTERNAL", "LOG_ERR");

    //logger_set_3( "VIDEO", "video", "LOG_DEBUG" );    
    //logger_set_3( "VIDEO", "video_laser", "LOG_DEBUG" );   
    logger_set_3( "DEVICES", "jbig", "LOG_ERR" );   
    logger_set_3( "DPRINTF", "main", "LOG_ERR" );
    logger_set_3( "SYSTEM", "logger", "LOG_ERR" );
    logger_set_3( "SYSTEM", "mem", "off" );
    logger_set_3( "SYSTEM", "cm", "LOG_INFO" ); //LOG_ERR
    //logger_set_3( "SYSTEM", "dma_alloc", "LOG_INFO" );

    logger_set_3( "PRINT", "ggs", "LOG_ERR" ); 
    logger_set_3( "PRINT", "pf", "LOG_INFO" ); 
    logger_set_3( "PRINT", "pcfax", "LOG_ERR" );
    logger_set_3( "PRINT", "printmode", "LOG_ERR" );
    logger_set_3( "PRINT", "printtools", "LOG_ERR" );
    logger_set_3( "PRINT", "eng", "LOG_DEBUG" );
    logger_set_3( "PRINT", "pm", "LOG_DEBUG" );
    logger_set_3( "PRINT", "pip", "LOG_ERR" );
    
	logger_set_3( "SCAN", "ui",		"LOG_DEBUG" 	);
	logger_set_3( "SCAN", "lib",		"LOG_DEBUG" 	);
	logger_set_3( "SCAN", "man",		"LOG_DEBUG" 	);
	logger_set_3( "SCAN", "pc",		"LOG_DEBUG" 	);
	logger_set_3( "SCAN", "task",	"LOG_DEBUG" 	);
	logger_set_3( "SCAN", "mech",	"LOG_DEBUG" 	);

    logger_set_3( "DEVICES", "dcmotor", "LOG_ERR" );
    logger_set_3( "DEVICES", "dec_laser", "LOG_ERR" );
    logger_set_3( "DEVICES", "dec_fuser", "LOG_ERR" );
    logger_set_3( "DEVICES", "uio", "LOG_ERR" );

    /// PLEASE refrain from using the logger_enable_module_mask(...) interface.

    logger_timestamp_onOff_usecTick( true /* off */, true /* usec if on */ );
}


