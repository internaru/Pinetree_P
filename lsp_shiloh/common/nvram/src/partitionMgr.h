/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_PARTITIONMGR_H
#define INC_PARTITIONMGR_H

#include "memdevice.h"
#include "nodeCache.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \file partitionMgr.h
 *
 * \brief Internal definitions for the Partition Manager API
 *
 * This header file contains internal/private partition manager API definitions.
 *
 **/

#define UNWRITTEN_DATA      0xffffffff   /**< Erased memory data               */


typedef struct partition_handle_s
{
    BLOCK_NODE_t   partition;
    MEMDEV_HANDLE *memDevH;
} PrivatePartitionHandle;


#ifdef __cplusplus
}
#endif

#endif /* INC_PARTITIONMGR_H */
