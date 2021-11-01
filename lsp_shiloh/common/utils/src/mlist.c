

/****************************************************************************** 
 * $Header:  $
 *
 * ============================================================================
 * Copyright (c) 2003, 2008 Marvell Semiconductor, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 * Just like list.c but with mutexed operations.
 *
 * This file contains a simple circular link list implementation.  Linked
 * list are created using a MLISTENTRY node declared inside the structure
 * to be linked.  When a node is removed from a list the original struct is
 * recovered using the CONTAINING_RECORD macro.
 *
 * As a usage example, given the following:
 *
 * MLISTENTRY MyList;
 * typedef struct _MyData
 * {
 *     int x;
 *     int y;
 *     ATLISTENTRY ListNode;
 * }MyData;
 *
 * MyData Data1;
 * MyData Data2;
 *
 * We could use the list as follows:
 * main()
 * {
 *     MyData* pData;
 *     ATLISTENTRY* pNode;
 *     
 *     MInitList(&MyList);
 *     MInsertTailList(&MyList, &Data1.ListNode);
 *     MInsertTailList(&MyList, &Data2.ListNode);
 *
 *     pNode = MRemoveHeadList(&MyList);
 *     pData = CONTAINING_RECORD(pNode, MyData, ListNode);
 * }
 *     
 ***********************************************************************/
 
#include "ATypes.h"
#include "mlist.h"
#include "lassert.h"

/*
    Name:
      MInitList
    Description:
        This routine initializes a linked list.  The list MUST be
        initialized with this rouine before being used.
    Input:
        pListHead - The address of the head node in the list.
*/
void MInitList(MLISTENTRY* pListHead)
{
    posix_mutex_init( &pListHead->mutex );

    ATInitList(&pListHead->list);
}

/*
    Name:
      MIsListEmpty
    Description:
        This routine determines whether or not a linked list is empty.
    Input:
        pListHead - The address of the main list node.
    Output:
        TRUE if list is empty; FALSE else
*/
bool MIsListEmpty(MLISTENTRY* pListHead)
{
    bool result;

    pthread_mutex_lock( &pListHead->mutex ); 

    result = ATIsListEmpty(&pListHead->list);

    pthread_mutex_unlock( &pListHead->mutex );
    return result;
}

/*
    Name:
      MInsertHeadList
    Description:
        This routine inserts a node on the head of a list.
    Input:
        pListHead - The address of the main list node.
        pNode - The address of the node to be added.
*/
void MInsertHeadList(MLISTENTRY* pListHead, ATLISTENTRY* pNode)
{
    pthread_mutex_lock( &pListHead->mutex ); 
    
    ATInsertHeadList(&pListHead->list, pNode);
    
    pthread_mutex_unlock( &pListHead->mutex );
}

/*
    Name:
      MInsertTailList
    Description:
        This routine inserts a node on the tail of a list.
    Input:
        pListHead - The address of the main list node.
        pNode - The address of the node to be added.
*/
void MInsertTailList(MLISTENTRY* pListHead, ATLISTENTRY* pNode)
{
    pthread_mutex_lock( &pListHead->mutex ); 

    ATInsertTailList(&pListHead->list, pNode);

    pthread_mutex_unlock( &pListHead->mutex ); 
}

/*
    Name:
      MRemoveHeadList
    Description:
        This routine removes and returns the node at the
        head of the list.
    Input:
        pListHead - The address of the main list node.
    Output:
        Returns a pointer to the node at the head of the list 
        or NULL if the list is empty.
*/
ATLISTENTRY* MRemoveHeadList(MLISTENTRY* pListHead)
{
    ATLISTENTRY* pNode;
    // check for empty list
    pthread_mutex_lock( &pListHead->mutex ); 

    pNode = ATRemoveHeadList(&pListHead->list);

    pthread_mutex_unlock( &pListHead->mutex );
    return pNode;
}

/*
    Name:
      MRemoveHeadList
    Description:
        This routine removes and returns the node at the
        tail of the list.
    Input:
        pListHead - The address of the main list node.
    Output:
        Returns a pointer to the node at the tail of the list 
        or NULL if the list is empty.
*/
ATLISTENTRY* MRemoveTailList(MLISTENTRY* pListHead)
{
    ATLISTENTRY* pNode;
    // check for empty list
    pthread_mutex_lock( &pListHead->mutex ); 

    pNode = ATRemoveTailList(&pListHead->list);

    pthread_mutex_unlock( &pListHead->mutex );
    return pNode;
}

/*
    Name:
      MRemoveEntryList
    Description:
      removes an entry from a list.
      return true if found and removed, false if not on the list.
    Input:
        pNode - The address of the node to be removed.
        pListHead - the list it is on.
*/
bool MRemoveEntryList(MLISTENTRY* pListHead, ATLISTENTRY* pNode)
{
    ATLISTENTRY* next;
    bool found = false;
    pthread_mutex_lock( &pListHead->mutex ); 
    ASSERT(pListHead && pNode);
    ASSERT(pListHead->list.nextEntry && pListHead->list.prevEntry);


    for (next = pListHead->list.nextEntry; 
         next != &pListHead->list; 
         next =  next->nextEntry)
    {
        ASSERT(next !=  next->nextEntry);
        if (next == pNode)
        {
            ASSERT(pNode->nextEntry && pNode->prevEntry); // if found it better be linked
            found = true; 
            pNode->nextEntry->prevEntry = pNode->prevEntry;
            pNode->prevEntry->nextEntry = pNode->nextEntry;
            pNode->prevEntry = pNode->nextEntry = 0;
            break;
        }
    }

    pthread_mutex_unlock( &pListHead->mutex );
    return found;
}


ATLISTENTRY* MListHead(MLISTENTRY* head)
{
    ATLISTENTRY* node;
    pthread_mutex_lock( &head->mutex ); 

    node = head->list.nextEntry == &head->list ? NULL : head->list.nextEntry; 

    pthread_mutex_unlock( &head->mutex ); 
    return node;
} 

ATLISTENTRY* MListNext(MLISTENTRY *head, ATLISTENTRY *pNode)
{
    ATLISTENTRY* node = NULL;
    ASSERT(head);

    pthread_mutex_lock( &head->mutex ); 

    if ( pNode && pNode != &head->list && pNode->nextEntry != &head->list)
        node = pNode->nextEntry;

    pthread_mutex_unlock( &head->mutex );  
    return node;
} 

ATLISTENTRY* MListTail(MLISTENTRY* head)
{
    ATLISTENTRY* node;
    pthread_mutex_lock( &head->mutex );

    node = head->list.prevEntry == &head->list ? NULL : head->list.prevEntry;

    pthread_mutex_unlock( &head->mutex );
    return node;
} 

ATLISTENTRY* MListPrev(MLISTENTRY *head, ATLISTENTRY *pNode)
{
    ATLISTENTRY* node = NULL;
    ASSERT(head);

    pthread_mutex_lock( &head->mutex );

    if ( pNode && pNode != &head->list && pNode->prevEntry != &head->list)
        node = pNode->prevEntry;

    pthread_mutex_unlock( &head->mutex );
    return node;
}

 
/// insert element after this one.
void MListInsertAfter(MLISTENTRY* pListHead, ATLISTENTRY *after, ATLISTENTRY *pNode)
{
    ASSERT(pListHead);
    pthread_mutex_lock( &pListHead->mutex );
    ATListInsertAfter(after, pNode);
    pthread_mutex_unlock( &pListHead->mutex );
}

/// insert element before this one.
void MListInsertBefore(MLISTENTRY* pListHead, ATLISTENTRY *before, ATLISTENTRY *pNode)
{   
    ASSERT(pListHead);
    pthread_mutex_lock( &pListHead->mutex );
    ATListInsertBefore(before, pNode);
    pthread_mutex_unlock( &pListHead->mutex );
}

// Calculate number of nodes on the list
uint32_t MNumListNodes(MLISTENTRY* pListHead)
{
    uint32_t num_nodes;

    ASSERT(pListHead);

    pthread_mutex_lock( &pListHead->mutex );
    num_nodes = ATNumListNodes(&pListHead->list);
    pthread_mutex_unlock( &pListHead->mutex );

    return num_nodes;
}
