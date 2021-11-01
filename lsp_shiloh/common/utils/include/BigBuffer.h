/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/** 
 * \file BigBuffer.h
 * 
 * \brief The purpose of this class is to have a polymorphic free function.
 *
 * Handle to a big allocation allows subclasses to have different behaviors
 * caching, static buffers etc, base class does malloc free behavior.
 *
 */

#ifndef BIGBUFFER_H
#define BIGBUFFER_H

#include <stdint.h>
#include "list.h"  
#include "mlimiter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct BigBuffer_s BigBuffer_t;

#ifndef defined_dma_alloc_t
#define defined_dma_alloc_t
typedef struct dma_alloc_s  dma_alloc_t;
#endif

struct BigBuffer_s 
{   
/** Macro for members of the structure must be used by subclasses.  This way the
 * order of the members is preserved. */
#define BigBuffer_T_BaseClassMembers \
    uint8_t * data; /*!< Pointer to a big continous memory chunk, clients shouldn't change the address.*/\
    int32_t datalen; /*!< Length in bytes of the data, clients shouldn't change the length.*/ \
    ATLISTENTRY listNode; /*!< Linked list node for the client's use. Client must remove from lists prior to freeing.*/\
    BigBuffer_t *(*freeFunc)(BigBuffer_t *bigBuffer) /*!< Overloadable free function. */
    struct dma_alloc_s *dma_alloc; /*!< NULL on threadx, */
    /** Client is in no way obligated to use the listNode, but its there if they
     * want to insert this into a list. */
    BigBuffer_T_BaseClassMembers;
};

/** 
 * \brief Constructor does a malloc to get you a big block of memory with a
 * handle (BigBuffer_t) on top of it.
 *
 * \param[in] limiter 0 is default, non zero means a heap limiter is installed.
 * \param[in] len Number of bytes to allocate.
 *
 * \returns null on failure.
 * \returns Big Buffer pointer otherwise.
 */
BigBuffer_t * BigBuffer_Malloc( mlimiter_t *limiter, int32_t len );

/**
 * \brief Free the BigBuffer and the big chunk of ram it points to.
 *
 * Uses internal freeFunc so the behaviour may be from a derived class.
 *
 * Usage:   ptr = BigBuffer_Free( ptr );
 *
 * \param[in] bigBuffer Buffer to be freed.
 *
 * \return Always 0 / null.
 *
 */
BigBuffer_t * BigBuffer_Free( BigBuffer_t *bigBuffer );

/// Construct wrapping object adopting the deletion rights of the
/// passed in data.    
BigBuffer_t *BigBuffer_adopt(void *data, int32_t len);

/// realloc a big buffer 
/// normally used to shrink a decompression buffer
BigBuffer_t *BigBuffer_realloc( BigBuffer_t *bb, int32_t new_length );

// void BigBuffer_invalidate( BigBuffer_t *bb, int direction );

#ifdef __cplusplus
}
#endif

#endif // define BIGBUFFER_H
