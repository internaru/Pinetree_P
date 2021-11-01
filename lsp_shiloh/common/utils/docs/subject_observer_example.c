/********************************************************/
/***************   General Guidelines   *****************/
/********************************************************/
// A dialog can take place between the subject and the observer in the 
// context of the notification callback.  This dialog is via the exposed
// Get functions.  Events should be logically grouped so that this dialog
// will allow observers to quickly determine what has occurred.

// Processing in the notification callbacks needs to be minimal since
// this is done on the subject's thread

// The IDs used by the subjects and observers are simply cookies that verify
// that this is the correct subject/observer.  They are just a random number,
// and do not need to be coordinated.  The subject's ID does not need to match
// the corresponding observer's ID

// The example below is a sample of how the Subject and Observer model
// can work for the consumables.  

// The DECLARE_SINGLETON_SUBJECT_PRIVATE and DECLARE_SINGLETON_SUBJECT_PUBLIC
// macros can be used for very simple subjects, but for most subjects this 
// will not be sufficient.

// Basic functions that should be implemented for a subject:
//  Public Interface:
//  [component]_AttachObserver_[event] ( Observer_t *o );   
//      Connect an observer to this subject
//  [component]_DetachObserver_[event] ( Observer_t *o );
//      Disconnect  an observer from this subject
//  [component]_Get_[value] ( Subject_t *s );
//      Retrieves value from the provided subject
//      There can be multiple get functions for each subject
//  Private Interface:
//  [component]_Notify_[event] ( [ modules decide parameters ] );
//      This will call all of the registered observers' callback functions
//  static [component_[event]_t * [component_[event]( void );
//      Constructor and Get function for subjects data


/********************************************************/
/*******************   OBSERVERS    *********************/
/********************************************************/

// Random number that is used as a sanity check to validate this is observer 
// is your observer.  Pick your favorite number (does not need to match 
// subject's id)
#define CP_CONSUMABLE_CHANGE_ID  0xABCD1234

// Attach call Option #1 - Used if you want to be able to detach the observer
Observer_t *consumables_obs;
consumables_obs = Observer_Constructor( CP_CONSUMABLE_CHANGE_ID, consumable_cb )
Consumable_AttachObserver_Change( consumables_obs );
// The detach call
Consumable_DetachObserver_Change( consumables_obs );

// Attach call Option #2 - Used if you never need to detach the observer
Consumable_AttachObserver_Change( Observer_Constructor( CP_CONSUMABLE_CHANGE_ID, cp_consumable_cb ) );

// Callback function
void cp_consumable_cb(Observer_t *o, Subject_t *s)
{
    // Sanity check of the observer
    ASSERT( VALIDATE_OBSERVER( o, CP_CONSUMABLE_CHANGE_ID ) );

    // Perform the Get calls to find out what changed
    // Keep processing to a minimum, this is called on the Subject's thread
    dbg_printf( "Type: %d -- reason: %d\n", 
                Consumable_Get_Type(s), 
                Consumable_Get_Reason(s) );
}



/********************************************************/
/******************    SUBJECTS    **********************/
/********************************************************/

/****************    Header File   **********************/
#ifndef CONSUMABLES_EVENT_API_H
#define CONSUMABLES_EVENT_API_H
/** 
 * ============================================================================
 * Copyright (c) 2011   Marvell Semiconductor, Inc. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#include "event_observer.h"
#include "elabelAPI.h"

typedef enum consumable_change_type_e
{
    // Signifies a new cartridges has been added and the verification has been run
    CONSUMABLE_REASON_CARTRIDGE, 

    // An error has occured and user intervention is required
    CONSUMABLE_REASON_ERROR,

    // The percent remaining in the cartridge has been updated
    CONSUMABLE_REASON_LEVEL,
} consumable_change_type_t;

void Consumable_AttachObserver_Change( Observer_t *o );
void Consumable_DetachObserver_Change( Observer_t *o );

// The "Get" functions are to only be called in the context of a notification
// If the event needs a callback - a Get function should be exposed to 
// retrieve that callback
// Add as many Get functions as the subject/observers require
consumable_type_t Consumable_Get_Type( Subject_t *s );
consumable_change_type_t Consumable_Get_Reason( Subject_t *s );

#endif //CONSUMABLES_EVENT_API_H


/******************    Source File    ************************/
#include "event_observer.h"
// Random number that is used as a sanity check to validate this is subject is
// your subject.  Pick your favorite number (does not need to match observers id)
#define CONSUMABLE_CHANGE_ID 0x4567DECC

/** Consumable structure */
typedef struct Consumable_Subject_s
{
    // Common values added to all subjects
    Subject_T_BaseClassMembers;
    MLISTENTRY observers;
    // Custom fields to allow dialog about what event just happened
    // Add as many entries as the subject/observers require
    consumable_change_type_t consumable_reason;
    consumable_type_t   consumable_type; 
} Consumable_Subject_t;

/** Singleton Constructor, initializes, doubles as get struct */
static Consumable_Subject_t * Consumable_Change( void )
{
    /** Static structure in your C file */
    static Consumable_Subject_t *ptr = 0;
    if ( 0 == ptr)
    {
        // Construct a copy of the event and populate 
        // it with initial values
        ptr = (Consumable_Subject_t*) MEM_MALLOC(sizeof(Consumable_Subject_t));
        ptr->consumable_type = CM_UNKNOWN;
        ptr->consumable_reason = CONSUMABLE_REASON_ERROR;
        // Initialize the sanity check 
        ptr->subject_observer_oid_key = CONSUMABLE_CHANGE_ID;
        MInitList(&(ptr->observers));
    }
    return ptr;
}

/** Notify function */
static void Consumable_Notify_Change( consumable_type_t consumable_type, consumable_change_type_t consumable_reason )
{   
    // Set the subjects values BEFORE calling the NOTIFY_OBSERVERS
    // otherwise Get calls will have stale data
    Consumable_Change()->consumable_type = consumable_type;
    Consumable_Change()->consumable_reason = consumable_reason;
    // This will call each observers' callback
    NOTIFY_OBSERVERS(Consumable_Change(), observers);
}

/** Public attach */
// Alternate to this approach would be to expose the initialize/get function
// And allow observers to directly call the ATTACH_OBSERVER macro
void Consumable_AttachObserver_Change( Observer_t *o )
{
    ATTACH_OBSERVER(Consumable_Change()->observers, o );
}

/** Public detach */
void Consumable_DetachObserver_Change( Observer_t *o )
{
    DETACH_OBSERVER(Consumable_Change()->observers, o );
}

/** Public get - called from within the Notify callbacks */
consumable_type_t Consumable_Get_Type( Subject_t *s )
{
    // Validate the subject
    ASSERT(VALIDATE_OBJECT_KEY( s, Consumable_Subject_t, subject_observer_oid_key, CONSUMABLE_CHANGE_ID ) );
    // Fetch and return the value from the subject structure
    return Consumable_Change()->consumable_type;
}

/** Public get */ 
consumable_change_type_t Consumable_Get_Reason( Subject_t *s )
{
    // Validate the subject
    ASSERT(VALIDATE_OBJECT_KEY( s, Consumable_Subject_t, subject_observer_oid_key, CONSUMABLE_CHANGE_ID ) );
    // Fetch and return the value from the subject structure
    return Consumable_Change()->consumable_reason;
}


// Notify examples - 
// called from within other functions when a notification should be sent out
Consumable_Notify_Change( CM_CYAN, CONSUMABLE_REASON_LEVEL );
Consumable_Notify_Change( CM_BLACK, CONSUMABLE_REASON_CARTRIDGE );


