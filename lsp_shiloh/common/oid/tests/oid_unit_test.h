/*
 * ============================================================================
 * (C) Copyright 2009   Marvell International Ltd.
 *                          All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef OID_UNIT_TEST_H
#define OID_UNIT_TEST_H


#include <stdint.h> 


/**
 * 
 * \file oid_unit_test.h
 *
 * \brief Exposes the unit test function for the OID module
 *
 * This file should NOT be included by any module except the OID module
 *
 */


/**
 *
 * \brief OID Unit test function.  Tests OID API functionality
 *
 * \return int32_t 0 upon success, otherwise a negative number
 *
 */
int oid_unit_test( void );


#endif
