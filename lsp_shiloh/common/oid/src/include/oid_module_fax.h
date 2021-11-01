/* 
 *
 * ============================================================================
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef OID_MODULE_FAX_H
#define OID_MODULE_FAX_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 
 * \file oid_module_fax.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions related to the fax API.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 * \brief register the fax API oids with the OID backend
 */
void oid_register_module_fax( void );



#ifdef __cplusplus
}
#endif

#endif
