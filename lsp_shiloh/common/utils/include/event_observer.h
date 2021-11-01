/* 
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/**
 * \file event_observer.h
 *
 * \brief Uses the GOF observer pattern to connect subjects with observers.
 * 
 * The intended use model is to set a callback allowing notification of an event
 * happening and the potential fetching of state information related to the 
 * event.
 * 
 * 
 * 
 * Usage: 
 *  Derive a new structure from Subject_t by adding the 
 *  Subject_T_BaseClassMembers macro to your structure you will also need some
 *  state variable(s) of interest and one or more lists of observers: 
 *  MLISTRENTRY's.  An observer will register with a subject using the 
 *  ATTACH_OBSERVER macro to insert itself into the event list.
 *  When the subject want to notify that an event has occured it will use the 
 *  NOTIFY_OBSERVERS macro to call each observer that has attached itself.  
 * 
 *  The observer will be called on the subjects thread and should return
 *  promptly if excessive cpu cycles are required the observer should send a
 *  message to itself with a copy of the pertinent state information. 
 *
 *  Subjects normally provide get interfaces with a base class Subject pointer,
 *  it is up to the subject to validate this pointer and at no time does the
 *  observer need to know what the derived type looks like.  
 *
 *  Why all the macros?
 *  The XXX_T_BasClassMembers macros allow for derivation in C without having
 *  nested structures
 *  
 *  The XXX_t_DEFINED is used to allow pointers to typedef'ed structures without
 *  knowing the contents of the structure.
 *
 *  ATTACH_OBSERVER, NOTIFY_OBSERVER, DETACH_OBSERVER are all templated methods
 *  they are using the c preprocesser to pass in information as a string
 *  building small code segment for you to do list manipulation or iteration for
 *  you.  
 *
 * Dynamic registration and unregistration at runtime to well defined events,
 * this might be the right hammer. Link time specialization of behavior? perhaps
 * a functional API would serve you better.
 *
 **/

#ifndef EVENT_OBSERVER_H
#define EVENT_OBSERVER_H

#include <stdint.h>
#include "mlist.h"
#include "dprintf.h"
#include "lassert.h"
#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DBG_EO
//#define DBG_EO ( DBG_LOUD | DBG_OUTPUT )
#define DBG_EO 0
#endif 

/**
 * Subject is an abstract base class that you should augment with one or more
 * observer lists and the subjects state information.  Typically get functions
 * are used by the observer to get the state of interest.  This implies that the
 * observer knows enough about the subject to know which get state calls to make
 * and where to register with.
 */
typedef struct Subject_s
{
#define Subject_T_BaseClassMembers \
    uint32_t subject_observer_oid_key /*!< sanity check identifier */

    Subject_T_BaseClassMembers;  

    /// a concrete subject would have observer list(s) and some state. 
    /// MLISTENTRY observers;  ///< not very usefull unless you derive from subject and add observers.

    /// Each list deserves good documentation describing what the event means so that clients 
    /// know what they are registering for.
   
} Subject_t;


/** Forward referenced typedef may exist. */
#ifndef Observer_t_DEFINED
#define Observer_t_DEFINED
typedef struct Observer_s Observer_t;
#endif

/**
 * Observer may only be in one subjects to notify list at a time.  If an
 * observer needs state derive from this by including the
 * Observer_T_BaseClassMembers macro in your structure.  The Update function is
 * the callback you want the subject to call when the event occurs.  In the
 * update function you call the subjects get state functions providing the
 * subject given to you in the update.
 * \code
 * int32_t time = Alarm_Subject_get_time_as_int32(s);
 * \endcode
 * The observer needs to know who to register with, and what public interface
 * get state functions the subject has. 
 */
struct Observer_s
{
#define Observer_T_BaseClassMembers \
    ATLISTENTRY observer_node; /*!< linked list node name may not be changed.*/ \
        void (*Update)(Observer_t *o, Subject_t *s); /*!< subject will call this on notify of event*/ \
        Observer_t * (*CopyConstructor)(Observer_t *src); /*!< deep copy rarely needed */ \
        void (*freeFunc)(void *o); /*!< memFree unless you derive and change this.*/ \
        uint32_t subject_observer_oid_key /*!< sanity check key */

    Observer_T_BaseClassMembers;
    
};



/**
 * \brief A state-less observer constructor.
 *
 * \returns null or a valid observer object.
 *
 * \param key This key is a sanity check that your getting the right observer
 * object type.
 *
 * (Update)(Observer_t *o, Subject_t *s) Update function will get called
 * with this, and the subject.
 */ 
Observer_t * Observer_Constructor( 
    uint32_t key,
    void (Update)(Observer_t *o, Subject_t *s)
    );

/**
 * Sanity checking macro used inside assert or if statement 
 * true on valid, false on invalid, NULL is invalid.
 */
#define VALIDATE_OBJECT_KEY(obj, type, field, key) \
    (obj ? (((type*)obj)->field == key) : 0 )

/** True on valid, false on invalid sanity check macro. */
#define VALIDATE_OBSERVER(o, key) \
    VALIDATE_OBJECT_KEY(o, Observer_t, subject_observer_oid_key, key)

/**
 * Which list in the subject should the observer be attached too.
 * normally a function interface is used and this is the internal worker.
 * A subject that wants to hide it's internal representation would provide
 * an interface function and use this internally in it's register for 
 * event function.
 * Prevents multiple insertions of same object, remember an observer may only
 * be in one list at a time.
 */
#define ATTACH_OBSERVER(mlist, observer)                                \
    if ( observer ) {                                                   \
        ASSERT( (&(observer)->observer_node)->nextEntry == 0);          \
        ASSERT( (&(observer)->observer_node)->prevEntry == 0);          \
        MInsertTailList(&(mlist), &(observer)->observer_node);          \
    }                                                                   \
    (void)0

 /**
  * Returns null, or a node pointer, you already had the object pointer to begin
  * with.  Often hidden behind an interface function.
  */
#define DETACH_OBSERVER(list, observer)                         \
  (( observer ) ? MRemoveEntryList(&(list), &(observer)->observer_node) : 0 )
  
/**
 * \brief Notify calls all the registered observer(s) indicating an event has
 * occured.
 *
 * An observer must return quickly this will assert if you attempt to block.
 * Lock list prevents deletion of observer during iteration loop.
 */
#define NOTIFY_OBSERVERS( s, observers_list )                           \
    if (s) {                                                             \
        MList_Lock(&(s)->observers_list);                               \
        ATLISTENTRY *next = 0;                                          \
        for (next = MListHead(&(s)->observers_list);                    \
             next;                                                      \
             next = MListNext(&(s)->observers_list, next))              \
        {                                                               \
            Observer_t *o = CONTAINING_RECORD(next, Observer_t, observer_node); \
            o->Update(o, (Subject_t *)(s));                             \
        }                                                               \
        MList_UnLock(&(s)->observers_list);                             \
    }


/**
 * \brief This copys the list of observers from one list to another.
 *
 * In general this isn't required.  List must be initialized, this allows
 * additive copies.  Lock is only required on src, prevents observer deletion
 * while copying.
 */
#define DEEPCOPY_OBSERVERS( dest, src, observers )                \
  {                                                              \
      ATLISTENTRY *next = 0;                                     \
      Observer_t *o = 0;                                         \
      Observer_t *n = 0;                                         \
      DBG_ASSERT(dest && src);                                   \
      MList_Lock(&(src)->observers);                             \
      for (next = MListHead(&(src)->observers);                  \
           next;                                                 \
           next = MListNext(&(src)->observers, next))                   \
      {                                                                 \
          o = CONTAINING_RECORD(next, Observer_t, observer_node);       \
          ASSERT(o && o->CopyConstructor);                              \
          n = o->CopyConstructor(o);                                    \
          ASSERT(n);                                                    \
          ATTACH_OBSERVER( (dest)->observers, n );                      \
      }                                                                 \
      MList_UnLock(&(src)->observers);                                  \
  }

/** 
 * \brief This frees the list of observers from a list.
 *
 * In general this isn't required. normal use would be to detach an observer.
 * Gets used to free the copied observers in the printjobinfo field of the print
 * job.  List lock deliberately not used since it would be a programming error
 * to have distributed deletion of observers.
 */
#define FREE_OBSERVERS( s, list )                                       \
  {                                                                     \
      ASSERT(s && &((s)->list));                                        \
      ATLISTENTRY *next = 0;                                            \
      for (next = MRemoveHeadList(&(s)->list);                          \
           next;                                                        \
           next = MRemoveHeadList(&(s)->list))                          \
      {                                                                 \
          Observer_t *o = CONTAINING_RECORD(next, Observer_t, observer_node); \
          ASSERT(o);  \
          ASSERT(o->freeFunc);                                          \
          o->subject_observer_oid_key = 0;                              \
          o->freeFunc((void*)o);                                        \
      }                                                                 \
  }


/** 
 * \brief Returns pointer to observer in the given list if the update function is the same.
 *
 * The list is not modified; assumes that there aren't two observers
 * on the same list with the same update function, in this case you will need to find by object pointer.
 * 
 * 
 */
Observer_t *  ///< null or the found observer
Observer_find_by_update( 
    MLISTENTRY *list, 
    void (*update)(Observer_t *o, Subject_t *s) 
    );



// The following macro template is useful when
// the subject and the observer are singletons,
// the subject contains a simple data type.
// 
// afraid of macros?  gcc -E w


// concatination macros never use ## without a macro helper to expand input arguments.
#define CAT_2(a,b) a ## b
#define CAT_3(a,b,c) a ## b ## c
#define CAT_4(a,b,c,d) a ## b ## c ## d


/**
 * \brief Declare a simple subject with a simple data type like bool or uint32_t
 * that can be fetched in the notify function.
 *
 * This template creates a singleton.
 * \<component\>_AttachObserver_\<event\>( observerPtr )  allows attaching and
 * observer to the event
 * \<component\>_DetachObsever_\<event\>( observerPtr ) stop obsevering
 * \<var_type\> \<component\>_Get_\<event\>( SubjectPtr ) called from inside the
 * notify to get the state.
 * \<component\>_\<event\>() is a private structure accessor that constructs on
 * first use.
 * \<component\>_Notify_\<event\>( var_type ) private notify function.
 * 
 * define a unique id first.
 * \#define component_event_ID 1245314
 * Then declare the structure, the singlton object and the functions this should
 * happpen in a c file to hide the private structure and functions.
 *
 * The singleton is self initialized on first use rather than requiring the
 * constructor be called first.  This is useful when the registering party
 * initializes before the notifing party.
 * \<component\>_\<event\>() is a get the singleton accessor constructing  the
 * object on first use.
 *
 */
#define DECLARE_SINGLETON_SUBJECT_PRIVATE( component, event, id, var_type ) \
    /** Component structure with var_type and observer list */           \
    typedef struct CAT_4(component, _, event, _s)                       \
    {                                                                   \
        Subject_T_BaseClassMembers;                                     \
        MLISTENTRY observers;                                           \
        var_type val;                                                   \
    } CAT_4(component, _, event, _t) ;                                  \
    /** Singleton Constructor, initializes, doubles as get struct */     \
    static CAT_4(component, _, event, _t) * CAT_3(component, _, event)() \
    {                                                                   \
        /** Static structure in your C file */                           \
        static CAT_4(component, _, event, _t) *ptr = 0;                 \
        if ( 0 == ptr) {                                                \
            ptr = (CAT_4(component, _, event, _t)*)                     \
                MEM_MALLOC(sizeof(CAT_4(component, _, event, _t)));     \
            ASSERT( 0 != ptr );\
            ptr->val = 0;                                               \
            ptr->subject_observer_oid_key = id;                         \
            MInitList(&(ptr->observers));                               \
        }                                                               \
        return ptr;                                                     \
    }                                                                   \
    /** Notify function */                                               \
    static void CAT_3(component, _Notify_, event)( var_type val )       \
    {                                                                   \
        CAT_3(component, _, event)()->val = val;                        \
        NOTIFY_OBSERVERS(CAT_3(component, _, event)(), observers);      \
    }                                                                   \
    /** Public attach \<component\>_AttachObserver_\<event\>(observer*) */   \
    void CAT_3(component, _AttachObserver_, event)( Observer_t *o)      \
    {                                                                   \
        ATTACH_OBSERVER(CAT_3(component, _, event)()->observers, o );   \
    }                                                                   \
    /** Public detach \<component\>_DetachObserver_\<event\>(observer*) */   \
    void CAT_3(component, _DetachObserver_, event)( Observer_t *o)      \
    {                                                                   \
        DETACH_OBSERVER(CAT_3(component, _, event)()->observers, o );   \
    }                                                                   \
    /** Public get:  var_type \<component\>_Get_\<event\>(observer*) */      \
    var_type CAT_3(component, _Get_, event)( Subject_t *s)              \
    {                                                                   \
        ASSERT(VALIDATE_OBJECT_KEY( s, CAT_4(component, _, event, _t) , \
                                    subject_observer_oid_key,           \
                                    id ));                              \
        return CAT_3(component, _, event)()->val;                       \
    }                                                                   

/**
 * Public interface declarations of above interface(s) that belongs in a header
 * file.
 *
 * \<component\>_AttachObserver_\<event\>( observerPtr )  allows attaching an
 * observer to the event
 * \<component\>_DetachObsever_\<event\>( observerPtr ) stop observing
 * \<var_type\> \<component\>_Get_\<event\>( SubjectPtr ) called from inside the
 * notify to get the state.
 *
 */
#define DECLARE_SINGLETON_SUBJECT_PUBLIC( component, event, var_type )     \
    void CAT_3(component, _AttachObserver_, event)( Observer_t *o);     \
    void CAT_3(component, _DetachObserver_, event)( Observer_t *o);     \
    var_type CAT_3(component, _Get_, event)( Subject_t *s)
        


#ifdef __cplusplus
}
#endif


#endif // EVENT_OBSERVER_H
