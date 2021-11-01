/*
 *
 * ============================================================================
 * Copyright (c) 2009-2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef ACL_BURN_FLASH_EVENT_API_H
#define ACL_BURN_FLASH_EVENT_API_H

#include "event_observer.h"

#if defined(HAVE_BOOTSPI) || defined(HAVE_NAND)
typedef enum acl_burn_flash_event_e
{
    BURN_FLASH_EVENT_UNKNOWN,
    BURN_FLASH_EVENT_ERASING,
    BURN_FLASH_EVENT_PROGRAMMING,
    BURN_FLASH_EVENT_UPGRADE_COMPLETE,
    BURN_FLASH_EVENT_UPGRADE_ERROR,

} acl_burn_flash_event_t;

void ACL_AttachObserver_burn_flash_event(Observer_t *o);
void ACL_DetachObserver_burn_flash_event(Observer_t *o);

acl_burn_flash_event_t ACL_Get_burn_flash_event(Subject_t *s);

void ACL_Notify_burn_flash_event(acl_burn_flash_event_t burn_flash_event);

#endif // #if defined(HAVE_BOOTSPI) || defined(HAVE_NAND)

#endif // ACL_BURN_FLASH_EVENT_API_H
