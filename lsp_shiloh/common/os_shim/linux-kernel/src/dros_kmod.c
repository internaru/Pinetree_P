/*
 *
 * ============================================================================
 * Copyright (c) 2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <linux/module.h>
#include "dros.h"
#include "list.h"
#include "interrupt_api.h"
#include "memAPI.h"
#include "lassert.h"

// DrOS
EXPORT_SYMBOL( dros_sem_wait );
EXPORT_SYMBOL( dros_sem_trywait );
EXPORT_SYMBOL( dros_sem_post );
EXPORT_SYMBOL( dros_sem_init );
EXPORT_SYMBOL( dros_sem_getvalue );
//EXPORT_SYMBOL( dros_sem_timedwait );
EXPORT_SYMBOL( dros_sem_is_initialized );
EXPORT_SYMBOL( dros_create_thread );
EXPORT_SYMBOL( dros_destroy_thread );
EXPORT_SYMBOL( dros_mq_receive );
EXPORT_SYMBOL( dros_mq_send_nowait );
EXPORT_SYMBOL( dros_mq_init );
EXPORT_SYMBOL( dros_mq_free );
EXPORT_SYMBOL( dros_smplock_init );
EXPORT_SYMBOL( dros_smplock_acquire );
EXPORT_SYMBOL( dros_smplock_release );
EXPORT_SYMBOL( dros_smplock_acquire_isr );
EXPORT_SYMBOL( dros_smplock_release_isr );

//Lists
EXPORT_SYMBOL( ATInitNode );
EXPORT_SYMBOL( ATInitList );
EXPORT_SYMBOL( ATIsListEmpty );
EXPORT_SYMBOL( ATInsertHeadList );
EXPORT_SYMBOL( ATInsertTailList );
EXPORT_SYMBOL( ATRemoveHeadList );
EXPORT_SYMBOL( ATRemoveTailList );
EXPORT_SYMBOL( ATRemoveEntryList );
EXPORT_SYMBOL( ATListHead );
EXPORT_SYMBOL( ATListNext );
EXPORT_SYMBOL( ATListPrev );
EXPORT_SYMBOL( ATListTail );
EXPORT_SYMBOL( ATListInsertAfter );
EXPORT_SYMBOL( ATListInsertBefore );
EXPORT_SYMBOL( ATNumListNodes );

// Interrupts
EXPORT_SYMBOL( intAttach );
EXPORT_SYMBOL( intDetach );
EXPORT_SYMBOL( intEnable );
EXPORT_SYMBOL( intDisable );

// Memory
EXPORT_SYMBOL( memMalloc );
EXPORT_SYMBOL( memFree );

// Asserts
EXPORT_SYMBOL( _assert );
EXPORT_SYMBOL( XAssertFail );

static int __init dros_init(void)
{
    printk(KERN_INFO "dros_init\n");
    return 0;
}

static void __exit dros_exit(void)
{
    printk(KERN_INFO "dros_exit\n");
}

MODULE_AUTHOR("Marvell Semiconductor, Inc.");
MODULE_DESCRIPTION("DrOS");
MODULE_LICENSE("GPL");

module_init(dros_init);
module_exit(dros_exit);

