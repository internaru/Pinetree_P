/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 *****************************************************************************/
/**
 * \file fifolist.h
 *
 * \brief Semaphore limiting FIFO List.
 *
 */

#ifndef FIFOLIST_H
#define FIFOLIST_H

#include <pthread.h>
#include <list.h>               // requires list.c 
#include <posix_ostools.h>      // mutex

#ifdef __cplusplus
extern "C" {
#endif

#define FIFO_WAIT_FOREVER (-1)

typedef struct FIFOLIST_s
{
    ATLISTENTRY list;
    pthread_mutex_t mutex;
    sem_t fill_sem;
    sem_t empty_sem;
    uint32_t maxCount;
    
} FIFOLIST;

typedef struct FIFOENTRY_s
{
    ATLISTENTRY list_node;
    void* dataPtr;
}FIFOENTRY;

void FListInit(FIFOLIST* pListHead, uint32_t maxFifoCount);
void FListDestroy(FIFOLIST* pListHead);

int FListAdd(FIFOLIST* pListHead, void* dataPtr, uint32_t mswait);
void* FListRemove(FIFOLIST* pListHead, uint32_t mswait);

bool FListIsEmpty(FIFOLIST* pListHead);

#ifdef __cplusplus
}
#endif

#endif // eof

