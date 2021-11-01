/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2008-2010 Marvell International, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 *****************************************************************************/
/**
 * \file mlist.h
 *
 * \brief Mutexed Linked list using containing record.
 *
 */

#ifndef MLIST_H
#define MLIST_H

#include <list.h>               // requires list.c 
#include <posix_ostools.h>      // mutex

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This struct serves as the list head, one mutex per list.  The list nodes are
 * of type ATLISTENTRY.
 *
 * \note Removed items contain pointers that are not updated by the list.
 * these are stale and it is up to the caller to not use them.
 */
typedef struct MLISTENTRY_s
{
    ATLISTENTRY list;   ///< List as first element makes visual ptr is same much easier for humans.
    pthread_mutex_t mutex;
} MLISTENTRY;

void MInitList(MLISTENTRY* pListHead);

bool MIsListEmpty(MLISTENTRY* pListHead);
void MInsertHeadList(MLISTENTRY* pListHead, ATLISTENTRY* pNode);
void MInsertTailList(MLISTENTRY* pListHead, ATLISTENTRY* pNode);
ATLISTENTRY* MRemoveHeadList(MLISTENTRY* pListHead);
ATLISTENTRY* MRemoveTailList(MLISTENTRY* pListHead);

/**
 * \returns true on found, false on pNode not on this list.
 */
bool MRemoveEntryList(MLISTENTRY* pListHead, ATLISTENTRY* pNode);

// iterators 
ATLISTENTRY* MListHead(MLISTENTRY* pListhead);
ATLISTENTRY* MListNext(MLISTENTRY* pListhead, ATLISTENTRY* pNode);
ATLISTENTRY* MListTail(MLISTENTRY* pListhead);
ATLISTENTRY* MListPrev(MLISTENTRY* pListhead, ATLISTENTRY* pNode);

/**
 * \brief Insert element after this one.
 */
void MListInsertAfter(MLISTENTRY* pListhead, ATLISTENTRY *after, ATLISTENTRY *pNode);

/**
 * \brief Insert element before this one.
 */
void MListInsertBefore(MLISTENTRY* pListhead, ATLISTENTRY *before, ATLISTENTRY *pNode);

/**
 * \brief Return the number of nodes on the list.
 */
uint32_t MNumListNodes(MLISTENTRY* pListHead);

#if 1  // if inline doesn't work for some reason turn this on.
#  define MList_Lock( pListhead ) pthread_mutex_lock( &((pListhead)->mutex) ); 
#  define MList_UnLock( pListhead ) pthread_mutex_unlock( &((pListhead)->mutex) ); 
#  define MList_DeleteLock( pListhead ) pthread_mutex_destroy( &((pListhead)->mutex) );
#else 

/**
 * \brief Lock the entire list.
 *
 * Primary use is to allow atomic iteration.  If you lock you must unlock
 * preferably within the same function, and within a few lines of each other.
 */
inline void MList_Lock(MLISTENTRY* pListhead)
{
    pthread_mutex_lock( &pListhead->mutex ); 
}

/**
 * \brief Unlock the entire list.
 *
 * Primary use is to allow atomic iteration.  If you lock you must unlock
 * preferably within the same function, and within a few lines of each other.
 */
inline void MList_UnLock(MLISTENTRY* pListhead)
{
    pthread_mutex_unlock( &pListhead->mutex ); 
}

/**
 * \brief Delete the mutex from the OS, may now be freed or MInitList may be
 * called again.
 */
inline void MList_DeleteLock(MLISTENTRY* pListhead)
{
    pthread_mutex_destroy(&pListhead->mutex);
}

#endif // macro or inline

#ifdef __cplusplus
}
#endif
#endif // eof

