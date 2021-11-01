/*
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_NODECACHE_H
#define INC_NODECACHE_H

/**
 * \file nodeCache.h
 *
 * \brief External interface definitions for the node cache utility routines
 *
 * Several sections of the NVM Manager code create RAM based lists of blocks or
 * partitions.  This file defines a common 'block' node structure and methods 
 * to manipulate node lists.  Note that we use our standard ATLISTENTRY 
 * implementation which embeds the linked list nodes into our block node 
 * structure:
 *
 * List Head--+           +---------------+    +---------------+
 *            |           |   Block Node  |    |   Block Node  |
 *     +-----------+      | +-----------+ |    | +-----------+ |
 *  +->| List Node |------->| List Node |------->| List Node |------+
 *  |  +-----------+      | +-----------+ |    | +-----------+ |    |
 *  |                     +---------------+    +---------------+    |
 *  |                                                               |
 *  +---------------------------------------------------------------+
 *
 * When working with these lists it is important to differentiate between the
 * list nodes (ATLISTENTRY) and the containing block nodes (BLOCK_NODE_t).
 * 
 */

#include <stdint.h>
#include <stdbool.h>
#include "list.h"
#include "deviceid.h"

#ifdef __cplusplus
extern "C" {
#endif


 
/**
 *  \struct BLOCK_NODE_t
 *
 *  \brief Cached block/partition node
 *
 *  Used by NVM and partition manager to cache block and partition nodes in RAM.
 *  Not all structure fields may be used by each application.
 **/
typedef struct BLOCK_NODE_s
{
    MEMORY_DEVICE   MemDev;       /**< Memory device storing the object     */
    char            Label[4];     /**< 4 ASCII character label (dynamic ID) */
    uint32_t        ID;           /**< Object ID (Hashed label/sector ID)   */
    uint32_t        Type;         /**< Type of object                       */
    uint32_t        Version;      /**< Optional object version              */
    uint64_t        StartOffset;  /**< Object offset (address)              */
    uint64_t        Size;         /**< Object byte size                     */    
    uint32_t        StartBlock;   /**< Object starting block number         */
    uint32_t        BlockCount;   /**< Blocks used by object                */
    uint32_t        Flags;        /**< Object flags                         */
    bool            Committed;    /**< Data written flag                    */

    ATLISTENTRY     listNode;     /**< For the linked list pointers         */
} BLOCK_NODE_t;


/**
 *  \brief Allocates a new list node
 *
 *  This function allocates a new list node and zeros the memory.
 *
 *  \return *BLOCK_NODE_t 
 *  \retval  Pointer to the new node entry
 *  \retval  NULL if alloc error
 **/ 
BLOCK_NODE_t *AllocNewNode(void);


/**
 *  \brief Returns a pointer to a particular node entry
 *
 *  This function searches a node list using the supplied ID, Type, and device.
 *
 *  \param[in] ID        ID of the entry to find
 *  \param[in] Type      Type of the entry to find
 *  \param[in] MemDev    Memory device of entry to find
 *  \param[in] NodeList  Pointer to node list to be searched
 *
 *  \return *BLOCK_NODE_t 
 *  \retval  Pointer to the located node entry
 *  \retval  NULL if not found
 **/
BLOCK_NODE_t *FindNodeByIdTypeDev(uint32_t      ID,
                                  uint32_t      Type,
                                  MEMORY_DEVICE MemDev,
                                  ATLISTENTRY  *NodeList);


/**
 *  \brief Returns a pointer to a specified node entry
 *
 *  This function searches a node list using the supplied label and device.
 *
 *  \param[in] *Label   Pointer to 4 character ASCII label
 *  \param[in] MemDev   Memory device of entry to find
 *  \param[in] NodeList Pointer to node list to be searched
 *
 *  \return *BLOCK_NODE_t 
 *  \retval  Pointer to the located node entry if found
 *  \retval  NULL if not found
 **/
BLOCK_NODE_t *FindNodeByLabelAndDev(char         *Label, 
                                    MEMORY_DEVICE MemDev,
                                    ATLISTENTRY  *NodeList);


/**
 *  \brief Empty out the supplied node list
 *
 *  This function will free all of the nodes in the supplied list. Note that
 *  the head of the list will not be freed; it will be returned as an empty
 *  list.
 *
 *  \param[in] theList  Pointer to list to be emptied
 *
 *  \return uint32_t 
 *  \retval the number of node entries freed
 **/
uint32_t DumpNodeList(ATLISTENTRY *theList);


/**
 *  \brief Make a copy of the supplied node list
 *
 *  This function will copy all of the nodes in the supplied list and return
 *  a new list. An empty list will be returned if the original list was empty.
 *
 *  \param[in] theList  Pointer to the list to be copied
 *
 *  \return ATLISTENTRY * 
 *  \retval Pointer to new list
 *  \retval NULL if error
 **/
ATLISTENTRY *CloneNodeList(ATLISTENTRY *theList);


#ifdef __cplusplus
}
#endif

#endif /* INC_NODECACHE_H */
