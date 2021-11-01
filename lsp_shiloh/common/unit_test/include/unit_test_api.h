/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * 
 *
 */
 /** 
  * \file unit_test_api.h
  *
  * \brief This api provides a test harness for the system.  
  *
  * Tests are registered with the test harness and then are available for others
  * to run using the debug interface.
  *
  * These tests run from within the system so care must be taken to not destroy
  * important data or info in the system.  For example, a test of a nvram part
  * must insure that the area of the part that is used during the test is not
  * needed by the rest of the system.  All system resources and drivers are
  * available for use.  So the nvram test could use the cdma, memory manager,
  * interrupt api etc.
  *
  * <b>Test case format:</b><br>
  * The test case is a function that is included when "HAVE_UNIT_TEST" compile
  * switch is set and when the driver itself is included.  The test case is type
  * int with a void for parameters.  
  * The return values are:
  * -# 0 -- Test passed
  * -# number -- The line number where the test failed.
  *
  * The body of the test case is where the actual test is executed.  The test
  * case must clean up after itself so the system can continue to operate
  * without any issues.  
  *
  * The test case is not run under a job context.  Thus there is no protection
  * for the test case.  Avoid running anything else at the same time the test
  * case is executing.  When writing the test case insure that calls have a 
  * timeout function so the test case will exit when a problem is discovered.
  *
  */

#ifndef UNIT_TEST_H_
#define UNIT_TEST_H_

#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 *
 * \brief Initialize the unit test module
 * 
 */
void unit_test_init( void );


/**
 * \brief Use this to register a new test case
 *  
 * \param name        The name of test case
 * \param test_entry  The function pointer to the test case entry function
 *  
 * \return int
 * \retval OK Success
 * \retval FAIL failure
 * 
 */
error_type_t unit_test_register_testcase(char *name, int (*test_entry)(void));


#ifdef __cplusplus
}
#endif

#endif /* UNIT_TEST_H_ */
