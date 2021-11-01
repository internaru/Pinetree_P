/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef GCPP_LOGGER_H
#define GCPP_LOGGER_H

#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#ifndef DBG_PRFX
  #define DBG_PRFX "GCPP: "
  #define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE_BIT( 8 ) 

  // General debug trace commands
  #define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
  #define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
  #define DBG_MSG_NOPRFX(...) DBG_PRINTF(LOG_NOTICE, __VA_ARGS__)
  #define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
  #define DBG_DUMP(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)
  #define DBG_CMD(...) cmd_printf(__VA_ARGS__)
#endif
  
// Debug levels for third-party code
#define GCPP_CURL_DEBUG_LEVEL		0L /* 1 verbose on, 0 verbose off */
#define GCPP_XMPP_DEBUG_LEVEL		XMPP_LEVEL_ERROR /* XMPP_LEVEL_DEBUG, XMPP_LEVEL_INFO, XMPP_LEVEL_WARN,XMPP_LEVEL_ERROR */


#ifdef __cplusplus
}
#endif

#endif // GCPP_LOGGER_H
