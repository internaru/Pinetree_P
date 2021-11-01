/****************************************************************************** 
 * Copyright (c) 2009-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef RESPONSE_SUBJECT_H
#define RESPONSE_SUBJECT_H
#include "event_observer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*TRY_AGAIN)(Subject_t *s);
typedef void (*USER_CANCEL)(Subject_t *s);

typedef struct Menu_Subject_TryAgain_s
{
#define Response_Subject_T_BaseClassMembers \
    Subject_T_BaseClassMembers;  \
    MLISTENTRY observers; \
    TRY_AGAIN try_again; \
    USER_CANCEL cancel_button; 
    // void (*timeout)(Subject_t *s); 

    Response_Subject_T_BaseClassMembers;
    
    /// a concrete subject would have observer list(s) and some state. 
    /// MLISTENTRY observers;  ///< not very usefull unless you derive from subject and add observers.

    /// Each list deserves good documentation describing what the event means so that clients 
    /// know what they are registering for.
   
} Menu_Subject_TryAgain_t;

#ifdef __cplusplus
}
#endif

#endif
