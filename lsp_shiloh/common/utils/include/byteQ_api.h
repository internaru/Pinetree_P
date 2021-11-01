/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __INC_BYTEQ_API_H__
#define __INC_BYTEQ_API_H__

#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Creates a byteQ by allocating requested space
 *
 * The returned ptr will be used as a handle for future operations on this
 * queue. If NULL is returned an error occurred which could include a failure
 * to allocate memory.
 */
void* byteQ_create(const char* name, unsigned int memsize);

/**
 * \brief Destroys a previously created byteQ (and frees associated memory)
 *
 * Any currently blocked get or put operations will be immediately unblocked
 *   and will return with a FAIL result. Any future use of the byteQ must be
 *   avoided.
 */
error_type_t byteQ_destroy(void* hdl);

/**
 * \brief Places a byte in the queue (space permitting).
 *
 * When called from interrupt context timeout should be 0 (No wait)
 * When called from task context timeout may be <0 which indicates wait forever,
 *   if necessary, for space to become available. Timeout may also be >0 which
 *   indicates that the caller should be blocked for at most the specified
 *   number of OS ticks.
 */
error_type_t byteQ_putbyte(void* hdl, uint8_t  byte, int timeout);

/**
 * \brief Retrieve a byte from the queue (if available).
 *
 * When called from interrupt context timeout should be 0 (No wait)
 * When called from task context timeout may be <0 which indicates wait forever,
 *   if necessary, for a byte to become available. Timeout may also be >0 which
 *   indicates that the caller should be blocked for at most the specified
 *   number of OS ticks.
 */
error_type_t byteQ_getbyte(void* hdl, uint8_t* byte, int timeout);

#ifdef __cplusplus
}
#endif

#endif /* __INC_BYTEQ_API_H__ */
