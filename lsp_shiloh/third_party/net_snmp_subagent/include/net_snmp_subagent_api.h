/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Net-SNMP Sub-Agent for Printer MIBs
 * Marvell Printer Application Interface Header
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#ifndef NET_SNMP_SUBAGENT_API_H
#define NET_SNMP_SUBAGENT_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "io_device.h"

int32_t net_snmp_subagent_init(void);
void oid_register_module_printermib(void);

#ifdef __cplusplus
}
#endif

#endif // NET_SNMP_SUBAGENT_API_H
