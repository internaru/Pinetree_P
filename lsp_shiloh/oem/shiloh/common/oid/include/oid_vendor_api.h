/*
 * ============================================================================
 * (C) Copyright 2013  Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file oid_vendor_api.h
 *
 * \brief The OID vendor API is an extension to the OID API allowing
 * the declaration of custom OIDS for a specific vendor
 *
 */


#ifndef OID_VENDOR_API_H
#define OID_VENDOR_API_H

#include "oid_api.h" // common/base oids

#ifdef __cplusplus
extern "C" {
#endif

/** PJL Module OIDS
 *
 */

#define OID_PJL_AUTHENTICATIONUSERID         oid_make(OID_NAMESPACE_VENDOR, OID_AREA_PJL, 0, OID_TYPE_STRING)
#define OID_PJL_AUTHENTICATIONPASSWORD   oid_make(OID_NAMESPACE_VENDOR, OID_AREA_PJL, 1, OID_TYPE_STRING)
#define OID_PJL_RESOLUTION         oid_make(OID_NAMESPACE_VENDOR, OID_AREA_PJL, 2, OID_TYPE_UINT32)

/** Network Modules OIDS
 *
 */
void oid_register_module_vendor( void );

#define OID_SINDOH_WIFI_CONNECT_SSID         oid_make(OID_NAMESPACE_VENDOR, OID_AREA_WIFI, 0, OID_TYPE_STRING)

#define OID_SINDOH_GET_IP                    oid_make(OID_NAMESPACE_VENDOR, OID_AREA_WIFI, 1, OID_TYPE_STRING)

#define OID_SINDOH_WIFI_SCAN_SSID            oid_make(OID_NAMESPACE_VENDOR, OID_AREA_WIFI, 2, OID_TYPE_STRING)


/** SNMP Module OIDS (via net-snmp sub-agentx)
 *    as defined in Printer-MIB.txt
 */

#ifdef __cplusplus
}
#endif

#endif
