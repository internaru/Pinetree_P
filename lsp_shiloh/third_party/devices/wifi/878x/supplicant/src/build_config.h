/*
 * wpa_supplicant/hostapd - Build time configuration defines
 * Copyright (c) 2005-2006, Jouni Malinen <jkmaline@cc.hut.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This header file can be used to define configuration defines that were
 * originally defined in Makefile. This is mainly meant for IDE use or for
 * systems that do not have suitable 'make' tool. In these cases, it may be
 * easier to have a single place for defining all the needed C pre-processor
 * defines.
 */

#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H

typedef enum _SUPPLICANT_THREAD_STATUS
{
    SUP_INITIALIZING,
    SUP_RUNNING,
    SUP_TERMINATING,
    SUP_TERMINATED
}SUPPLICANT_THREAD_STATUS;

/* Insert configuration defines, e.g., #define EAP_MD5, here, if needed. */

#define OS_NO_C_LIB_DEFINES
#define INTERNAL_SHA1
#define INTERNAL_MD5
#define INTERNAL_SHA256

#ifndef CONFIG_DRIVER_MARVELL
#define CONFIG_DRIVER_MARVELL
#endif

#endif /* BUILD_CONFIG_H */
