/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */


#ifndef OID_MODULE_STRING_MGR_H
#define OID_MODULE_STRING_MGR_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 
 * \file oid_module_string_mgr.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions related to the string mgr API.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 *
 * \brief Registers each string mgr API OID with the oid controller
 *
 */
void oid_register_module_string_mgr( void );



#ifdef __cplusplus
}
#endif

#endif
