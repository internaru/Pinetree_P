/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \file cmd_sys_api.h
 *
 * \brief This is the system interface to the cmd system.
 *
 **/

#ifndef CMD_SYS_API_H
#define CMD_SYS_API_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize the cmd system
 *
 * In case it is not obvious it should be known that this should be invoked
 * before any other cmd service including those exposed in other cmd API's
 */
void cmd_init(void);

#ifdef __cplusplus
}
#endif

#endif /* CMD_SYS_API_H */
