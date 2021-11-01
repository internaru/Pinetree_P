/*
 *
 * ============================================================================
 * Copyright (c) 2009-2012   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file print_logger.h
 *
 */

#ifndef PRINT_LOGGER_H
#define PRINT_LOGGER_H

#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Module level portion (specific to each module) */
/* The print logger uses submodules, so we don't want to OR in DEBUG_LOGGER_MAIN_MODULE_ONLY */
#define LOGGER_MODULE_MASK  (DEBUG_LOGGER_MODULE_PRINT)

#define DBG_JOBMGR      LOGGER_SUBMODULE_BIT( 12 )
#define DBG_PRINTMGR    LOGGER_SUBMODULE_BIT( 13 )
#define DBG_EM          LOGGER_SUBMODULE_BIT( 14 )
#define DBG_CONSUMABLE  LOGGER_SUBMODULE_BIT( 15 )
#define DBG_MOTORS      LOGGER_SUBMODULE_BIT( 16 )
#define DBG_ZJPARSER    LOGGER_SUBMODULE_BIT( 17 )
#define DBG_ZXDRAWER    LOGGER_SUBMODULE_BIT( 19 )

#ifdef __cplusplus
}
#endif

#endif


