/****************************************************************************** 
 * $Header:  $
 *
 * ============================================================================
 * Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * \brief Semaphore limiting FIFO List.
 *     
 ***********************************************************************/
 
#include <pthread.h>
#include "ATypes.h"
#include "memAPI.h"
#include "fifolist.h"
#include "lassert.h"


/*
    Name:
      FListInit
    Description:
        This routine initializes a FIFO list.  The list MUST be
        initialized with this routine before being used.
    Input:
        pListHead - The address of the head node in the list.
        maxFifoCount - The max number of FIFO entries allowed - 0 if unlimited
*/
void FListInit(FIFOLIST* pListHead, uint32_t maxFifoCount)
{
    int px_res;
    
    px_res = posix_mutex_init( &pListHead->mutex );
    REL_ASSERT( px_res == 0 ); // this should always succeed
    
    ATInitList(&pListHead->list);
    
    pListHead->maxCount = maxFifoCount;
    sem_init(&pListHead->fill_sem, 0, 0);
    if (pListHead->maxCount)
    {
        sem_init(&pListHead->empty_sem, 0, pListHead->maxCount);
    }
}
/*
    Name:
      FListDestroy
    Description:
        This routine destroys a FIFO list.  The list MUST have been
        initialized before this routine is called.
    Input:
        pListHead - The address of the head node in the list.
*/
void FListDestroy(FIFOLIST* pListHead)
{
    // We should only destroy empty FIFO lists
    if (!FListIsEmpty(pListHead))
    {
        ASSERT(FALSE);
    }
    
    pthread_mutex_destroy(&pListHead->mutex);
    sem_destroy(&pListHead->fill_sem);
    if (pListHead->maxCount)
    {
        sem_destroy(&pListHead->empty_sem);
        pListHead->maxCount = 0;
    }
}

/*
    Name:
      FListIsEmpty
    Description:
        This routine determines whether or not a FIFO list is empty.
    Input:
        pListHead - The address of the main list node.
    Output:
        TRUE if list is empty; FALSE else
*/
bool FListIsEmpty(FIFOLIST* pListHead)
{
    int px_res;
    bool result = false;
    
    px_res = pthread_mutex_lock( &pListHead->mutex );  
    if ( 0 == px_res )
    {
        result = ATIsListEmpty(&pListHead->list);
        pthread_mutex_unlock( &pListHead->mutex );  
    }

    return result;
}

/*
    Name:
      FListAdd
    Description:
        This routine inserts a node on the head of a FIFO list.
    Input:
        pListHead - The address of the main list node.
        dataPtr - The address of the data to be added.
    Output:
        0 if success, else failed or timed out
*/
int FListAdd(FIFOLIST* pListHead, void* dataPtr, uint32_t mswait)
{
    int retcode = 0;

    ASSERT(dataPtr != NULL);

    if (pListHead->maxCount)
    {
        if (mswait == FIFO_WAIT_FOREVER)
        {
            retcode = sem_wait(&pListHead->empty_sem);
        }
        else
        {
            struct timespec sleep_time;
            
            while (mswait--)
            {
                retcode = sem_trywait(&pListHead->empty_sem);
                if (retcode == 0)
                {
                    break;
                }
                
                sleep_time.tv_sec = 0;
                sleep_time.tv_nsec = 1000000; // 1ms
                nanosleep(&sleep_time, NULL);
            }
        }
    }
    
    if (retcode == 0)
    {
        int px_res;
        FIFOENTRY* entry = (FIFOENTRY*)MEM_MALLOC(sizeof(FIFOENTRY));
        ASSERT(entry != NULL);
        
        entry->dataPtr = dataPtr;
        
        px_res = pthread_mutex_lock( &pListHead->mutex );  
        if ( 0 == px_res )
        {
            ATInsertHeadList(&pListHead->list, &entry->list_node);
            pthread_mutex_unlock( &pListHead->mutex );  
        }
        else
        {
            memFree(entry);
            retcode = 1;
        }

        sem_post(&pListHead->fill_sem);
    }
    
    return retcode;
}

/*
    Name:
      FListRemove
    Description:
        This routine removes and returns the node at the
        tail of the FIFO list.
    Input:
        pListHead - The address of the main list node.
    Output:
        Returns a data pointer from the tail of the FIFO list 
        or NULL if the list is empty or timed out.
*/
void* FListRemove(FIFOLIST* pListHead, uint32_t mswait)
{
    int retcode = 1;
    void* dataPtr = NULL;
    
    if (mswait == FIFO_WAIT_FOREVER)
    {
        retcode = sem_wait(&pListHead->fill_sem);
    }
    else
    {
        struct timespec sleep_time;

        do 
        {
            retcode = sem_trywait(&pListHead->fill_sem);
            if (retcode == 0)
            {
                break;
            }
            
            sleep_time.tv_sec = 0;
            sleep_time.tv_nsec = 1000000; // 1ms
            nanosleep(&sleep_time, NULL);
        }
        while (mswait--);
    }
    
    if (retcode == 0)
    {
        int px_res;
        FIFOENTRY* entry;
        ATLISTENTRY* pNode;
        
        px_res = pthread_mutex_lock( &pListHead->mutex );  
        if ( 0 == px_res )
        {
            pNode = ATRemoveTailList(&pListHead->list);

            pthread_mutex_unlock( &pListHead->mutex );  
        
            // check for empty list
            ASSERT(pNode != NULL);
            
            entry = CONTAINING_RECORD(pNode, FIFOENTRY, list_node);
            ASSERT(entry != NULL);
            
            dataPtr = entry->dataPtr;
            ASSERT(dataPtr != NULL);
            
            memFree(pNode);
            
            if (pListHead->maxCount)
            {
                sem_post(&pListHead->empty_sem);
            }
        }
        else
        {
            dataPtr = NULL;
        }
    }
    
    return dataPtr;
}
