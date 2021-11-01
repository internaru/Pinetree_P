/*
 * ============================================================================
 * (C) Copyright 2009-2010   Marvell International Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef OID_MODULE_CONSUMABLE_H
#define OID_MODULE_CONSUMABLE_H

#if defined HAVE_COMMON_CONSUMABLES_API || defined HAVE_OEM_CONSUMABLES_API

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 
 * \file oid_module_consumable.h
 *
 * \brief This is an internal OID API for registration of back-end OID
 * callback functions related to the consumable API.
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 * \brief register the consumable API oids with the OID backend
 */
void oid_register_module_consumable( void );


#ifdef __cplusplus
}
#endif

#else

// void function
#define oid_register_module_consumable() { (void)0; }

#endif
#endif
