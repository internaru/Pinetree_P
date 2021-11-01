/*
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file nvram_logger.h 
 *
 */

#ifndef NVM_LOGGER_H
#define NVM_LOGGER_H



#ifdef __cplusplus
extern "C" {
#endif

#include "logger.h"

#define LOGGER_MODULE_MASK ( DEBUG_LOGGER_MODULE_NVRAM )

#define DBG_PARTMGR          (1<<7)
#define DBG_NVRAM            (1<<8)
#define DBG_NVRAM_DEV_SPI    (1<<9)
#define DBG_NVRAM_DEV_SHIM   (1<<10)
#define DBG_NVRAM_DEV_EEPROM (1<<11)
#define DBG_IMAGEMAP         (1<<12)
#define DBG_MEMDEVICE        (1<<13)


#ifdef __cplusplus
}
#endif

#endif


