/*
 * ============================================================================
 * Copyright (c) 2008-2009 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file nodeCache.c
 *
 * \brief Partition/Block Node Cache Utility
 *
 * This file implements the partition manager API, please see the 
 * partitionMgrAPI.h file for detailed interface descriptions.
 *
 **/
  
#include <stdint.h>
#include <string.h>
#include "list.h"
#include "memAPI.h"
#include "deviceid.h"
#include "nodeCache.h"


/* FUNCTION NAME: AllocNewNode
 *
 * Please see nodeCache.h for function documentation.
 */
BLOCK_NODE_t *AllocNewNode(void)
{
    BLOCK_NODE_t *newNode;
    
    /* Allocate memory for a new node entry */
    newNode = (BLOCK_NODE_t *)MEM_MALLOC(sizeof(BLOCK_NODE_t));
    if (newNode != NULL)
    {
        memset(newNode, 0, sizeof(BLOCK_NODE_t));
    }

    return newNode;
}


/* FUNCTION NAME: FindNodeByIdAndDev
 *
 * Please see nodeCache.h for function documentation.
 */
BLOCK_NODE_t *FindNodeByIdTypeDev(uint32_t      ID,
                                  uint32_t      Type,
                                  MEMORY_DEVICE MemDev,
                                  ATLISTENTRY  *NodeList)
{
    ATLISTENTRY   *searchNode;
    BLOCK_NODE_t  *myNode;

    /* Make sure we were passed a valid node list */
    if (NodeList == NULL)
    {
        return NULL;        
    }

    /* Put searchNode at the first entry after the start (it will be the head
     * node if the list is empty).
     */
    searchNode = NodeList->nextEntry;

    /* Search until we find our node or we hit the head node */
    while (searchNode != NodeList)
    {
        myNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if ( (myNode != NULL) && (myNode->ID == ID) && 
             (myNode->Type == Type) && (myNode->MemDev == MemDev) )
        {
            return myNode;
        }
        searchNode = searchNode->nextEntry;
    }

    /* Didn't find a match */
    return NULL;
}


/* FUNCTION NAME: FindNodeByLabelAndDev
 *
 * Please see nodeCache.h for function documentation.
 */
BLOCK_NODE_t *FindNodeByLabelAndDev(char         *Label, 
                                    MEMORY_DEVICE MemDev,
                                    ATLISTENTRY  *NodeList)
{
    ATLISTENTRY   *searchNode;
    BLOCK_NODE_t  *myNode;

    /* Make sure we were passed valid parameters */
    if ((NodeList == NULL) || (Label == NULL))
    {
        return NULL;        
    }

    /* Put searchNode at the first entry after the start (it will be the head
     * node if the list is empty).
     */
    searchNode = NodeList->nextEntry;

    /* Search until we find our node or we hit the head node */
    while (searchNode != NodeList)
    {
        myNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if ((myNode != NULL) && (myNode->MemDev == MemDev) && (memcmp(&myNode->Label[0], Label, 4) == 0))
        {
            return myNode;
        }
        searchNode = searchNode->nextEntry;
    }

    /* Didn't find a match */
    return NULL;
}


/* FUNCTION NAME: DumpNodeList
 *
 * Please see nodeCache.h for function documentation.
 */
uint32_t DumpNodeList(ATLISTENTRY *theList)
{
    uint32_t       nodeCount = 0;
    ATLISTENTRY   *listNode;
    BLOCK_NODE_t  *myNode;

    if (theList == NULL)
    {
        return nodeCount;
    }

    while(!ATIsListEmpty(theList))
    {
        listNode = ATRemoveHeadList(theList);
        if (listNode != NULL)
        {
            /* The list nodes are embedded inside the container structure; we need
             * to free the container (because that's how we allocated the memory).
             */
            myNode = CONTAINING_RECORD(listNode, BLOCK_NODE_t, listNode);
            if (myNode != NULL)
            {
                MEM_FREE_AND_NULL(myNode);
            }
        }
        nodeCount++;
    }
    
    return nodeCount;
}


/* FUNCTION NAME: CloneNodeList
 *
 * Please see nodeCache.h for function documentation.
 */
ATLISTENTRY *CloneNodeList(ATLISTENTRY *theList)
{
    ATLISTENTRY  *searchNode;
    BLOCK_NODE_t *newNode;
    BLOCK_NODE_t *oldNode;
    ATLISTENTRY  *newList;
    
    if (theList == NULL)
    {
        /* List is missing */
        return NULL;
    }

    /* Allocate space for a new list head and initialize it */
    newList = (ATLISTENTRY *)MEM_MALLOC(sizeof(ATLISTENTRY));
    if (newList == NULL)
    {
        return NULL;
    }
    ATInitList(newList);
 
    /* Put searchNode at the first entry after the start (it will be the head
     * node if the list is empty).
     */
    searchNode = theList->nextEntry;

    /* Copy nodes until we hit the head node */
    while (searchNode != theList)
    {
        /* The list nodes are embedded in the block node structures, we need
         * to copy the entire containing structure.
         */
        oldNode = CONTAINING_RECORD(searchNode, BLOCK_NODE_t, listNode);
        if (oldNode != NULL)
        {
            /* Alloc the new node */
            newNode = AllocNewNode();
            if (newNode == NULL)
            {
                DumpNodeList(newList);
                MEM_FREE_AND_NULL(newList);
                return NULL;
            }
        
            /* Copy the existing node (list pointers will be over-written next) */
            memcpy((int8_t *)newNode, (int8_t *)oldNode, sizeof(BLOCK_NODE_t));
        
            /* Add it to the cloned list */
            ATInsertTailList(newList, &newNode->listNode);
        }
        searchNode = searchNode->nextEntry;
    }
    
    return newList;
}
