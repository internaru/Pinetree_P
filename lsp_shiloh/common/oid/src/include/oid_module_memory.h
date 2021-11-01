/*
 * ============================================================================
 * (C) Copyright 2009-2010   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef OID_MODULE_MEMORY_H
#define OID_MODULE_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 
 * \file oid_module_memory.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions related to the memory manager.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 * \brief register the memory module oids with the OID backend
 */
void oid_register_module_memory( void );



#ifdef __cplusplus
}
#endif

#endif
