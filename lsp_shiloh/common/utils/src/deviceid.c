/*
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 *  \file deviceid.c
 *
 *  \brief Device ID utility functions
 *
 *  Please see deviceid.h for interface/usage documentation.
 *
 **/
 
#include "deviceid.h"

 
/*
 * FUNCTION NAME: getDevIdStr 
 * 
 * Please see deviceid.h for function documentation.
 */
const char *getDevIdStr(MEMORY_DEVICE memDev)
{
    /* Note: these string must match the MEMORY_DEVICE enum! */
    static const char *mem_dev_msg[] = {
        "MEM_I2C",
        "MEM_RTC",
        "MEM_SPI0",
        "MEM_SPI1",
        "MEM_SPI2",
        "MEM_SPI3",
        "MEM_SPI4",
        "MEM_NAND0",
        "MEM_NAND1",
        "MEM_DEVICE_NOT_FOUND"
    };
    
    return mem_dev_msg[memDev];
} 
