/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */


#ifndef OID_MODULE_FEATURE_SUPPORT_H
#define OID_MODULE_FEATURE_SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 
 * \file oid_module_feature_support.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions related to the feature support API.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 * \brief register the feature support API oids with the OID
 * backend
 */
void oid_register_module_feature_support( void );



#ifdef __cplusplus
}
#endif

#endif
