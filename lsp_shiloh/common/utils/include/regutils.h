/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file regutils.h
 *
 * \brief Register access utilities.
 *
 */

 #ifndef REGUTILS_H
#define REGUTILS_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief For polling functions
*/
typedef enum {
    NOT_EQUAL = 0,
    EQUALS = 1
} EQUALITY_CHK;


/**
 * \brief Poll a register until it has the requested value, or until time runs
 * out.
 *
 * \param[in] pollReg Address of the register to poll.
 * \param[in] mask Value to AND with the returned value from the register.
 * \param[in] result Value to compare against the masked register value.
 * \param[in] pollInterval Interval to wait before the next poll.
 * \param[in] count How many times to poll.
 * \param[in] equals If TRUE, poll until result == masked register value.
 *                   If FALSE, poll until result != masked register value.
 * 
 * \return error_type_t
 * \retval OK 0
 * \retval FAIL -1
 *
 */
error_type_t NVM_PollReg(volatile uint32_t *pollReg, uint32_t result, uint32_t mask, uint32_t pollInterval, uint32_t count, uint32_t equals);

/**
 * \brief Poll a function until it returns the requested value, or until time
 * runs out.
 *
 * \warning This only works for functions that take no parameters.
 *
 * \param pollFunc Pointer to the function to execute and poll on return value.
 * \param result Value to compare against the masked function return value.
 * \param mask Value to AND with the returned value from the function.
 * \param pollInterval Interval to wait before the next poll.
 * \param count How many times to poll.
 * \param equals
 *  If TRUE (EQUALS),  poll until result == masked function return value.
 *  If FALSE (NOT_EQUAL), poll until result != masked function return value.
 *  
 *  \return error_type_t
 *  \retval OK 0
 *  \retval FAIL -1 Timed out or Counted out
 *
 */
error_type_t NVM_PollFunc(uint8_t pollFunc(), uint32_t result, uint32_t mask, 
                        uint32_t pollInterval, uint32_t count, uint32_t equals);

#ifdef __cplusplus
}
#endif

#endif
