/*
 * ============================================================================
 * Copyright (c) 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef _NET_CONF_FILES_H
#define _NET_CONF_FILES_H

// file system locations for configuration files, utilities and sh scripts

#define INITD             "/etc/init.d"
#define SBIN              "/sbin"
#define USRBIN            "/usr/bin"
#define USRSBIN           "/usr/sbin"
#define WIFI_BITS         "/8782"
#define WIFI_DEV_DRIVER   "/lib/modules" WIFI_BITS
#define WIFI_BIN          USRSBIN WIFI_BITS

// wpa_supplicant usage
#define VAR_RUN           "/var/run"
#define VAR_LIB           "/var/lib"

#endif // _NET_CONF_FILES_H

//eof
