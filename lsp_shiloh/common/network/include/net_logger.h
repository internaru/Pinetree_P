/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef NET_LOGGER_H
#define NET_LOGGER_H

#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * register DEBUG_LOGGER_MODULE_NETWORK submodules here 
 *  
 * IMPORTANT: any modification of this list must be reflected in the submodule
 * index-to-name mappings defined in logger_common.c
 */


#define NET_LOGGER_SUBMOD_LINK          LOGGER_SUBMODULE_BIT(1)
#define NET_LOGGER_SUBMOD_NET_IO        LOGGER_SUBMODULE_BIT(2)
#define NET_LOGGER_SUBMOD_RAW_IO        LOGGER_SUBMODULE_BIT(3)
#define NET_LOGGER_SUBMOD_PRINT         LOGGER_SUBMODULE_BIT(4)
#define NET_LOGGER_SUBMOD_SCAN          LOGGER_SUBMODULE_BIT(5)
#define NET_LOGGER_SUBMOD_SM_JOB        LOGGER_SUBMODULE_BIT(6)
#define NET_LOGGER_SUBMOD_IPP           LOGGER_SUBMODULE_BIT(7)
#define NET_LOGGER_SUBMOD_GCPP          LOGGER_SUBMODULE_BIT(8)
#define NET_LOGGER_SUBMOD_WSD           LOGGER_SUBMODULE_BIT(9)
#define NET_LOGGER_SUBMOD_TELNET        LOGGER_SUBMODULE_BIT(10)
#define NET_LOGGER_SUBMOD_IFACE         LOGGER_SUBMODULE_BIT(11)
#define NET_LOGGER_SUBMOD_SERVICES      LOGGER_SUBMODULE_BIT(12)

#ifdef __cplusplus
}
#endif

#endif // NET_LOGGER_H

