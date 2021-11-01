/** 
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
#include "ATypes.h"
#include "event_observer.h"
#include "lassert.h"
#include "memAPI.h"

///< copy works because the base class can only be constructed state less! 
///< this function is hidden since derived classes must implement thier own copy constructors.
static Observer_t * ///< null or copied object.
Observer_copyconstructor(Observer_t *src)
{
    ASSERT(src);

    return  Observer_Constructor(src->subject_observer_oid_key, src->Update);
}


///< fully defines all parameters with the constructor arguments.
///< this must be a stateless base class and not a derived class.
Observer_t * Observer_Constructor( uint32_t key, void (*update)(Observer_t *o, Subject_t *s))
{
    Observer_t *dest = (Observer_t *)MEM_MALLOC(sizeof(Observer_t));
    
    //    DPRINTF(DBG_EO, ("---*** copy observer %x \n", dest));
    DBG_ASSERT(dest);
    if (dest)
    {
        ATInitNode(&dest->observer_node);
        dest->Update = update;
        dest->CopyConstructor = Observer_copyconstructor;
        dest->freeFunc = memFree;
        dest->subject_observer_oid_key = key;
    }
    return dest;
}

Observer_t *
Observer_find_by_update( 
    MLISTENTRY *list, 
    void (*update)(Observer_t *o, Subject_t *s) )
{
    DBG_ASSERT(update); // observer with null update function is illegal.

    if (list) // null list will not contain any observers.                  
    {                                                                   
        MList_Lock(list);                                             
        ATLISTENTRY *next = 0;                                           
        Observer_t *o = 0;                                               
        for (next = MListHead(list);                                  
             next;                                                       
             next = MListNext(list, next))                            
        {                                                               
            o = CONTAINING_RECORD(next, Observer_t, observer_node);     
            if (o->Update == update)                                    
            {                                                           
                MList_UnLock(list);                                  
                return o;      // found                                         
            }                                                           
        }                                                               
        MList_UnLock(list);                                          
    }
    return 0;                                                       
}

