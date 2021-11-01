
/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */


#ifndef OID_MODULE_WIFI_H
#define OID_MODULE_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 
 * \file oid_module_wifi.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions related to the wifi API.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 * \brief register the wifi API oids with the OID backend
 */
    void oid_register_module_wifi( void );

#ifdef __cplusplus
}
#endif

#endif
