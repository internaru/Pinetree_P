
/* 
 *
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */


#ifndef OID_MODULE_GCPP_H
#define OID_MODULE_GCPP_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 
 * \file oid_module_gcpp.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions related to the gcpp API.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 * \brief register the gcpp API oids with the OID backend
 */
    void oid_register_module_gcpp( void );



#ifdef __cplusplus
}
#endif

#endif
