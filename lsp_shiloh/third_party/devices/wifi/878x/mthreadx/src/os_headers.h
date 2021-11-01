/* 
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2011
 */

#ifndef _OS_HEADERS_H
#define _OS_HEADERS_H

#ifndef __ATTRIB_ALIGN__
#define __ATTRIB_ALIGN__ __attribute__((aligned(4)))
#endif

// bqiu
#ifndef __ATTRIB_ALIGN16__
#define __ATTRIB_ALIGN16__ __attribute__((aligned(16)))
#endif

#ifndef __ATTRIB_PACK__
#define __ATTRIB_PACK__ __attribute__ ((packed))
#endif

#include    <stdio.h>
#include 	"tx_api.h"
#include    "ATypes.h"
//#include    "interrupt.h"
#include    "utils.h"
#include    "os_list.h"
#include    "os_defs.h"
#include    "os_wireless.h"
#include    "dprintf.h"
//#include    "agMemory.h"
#include    "memAPI.h"
#include    <ctype.h>
//#include    "endianSwap.h"
//#include    "agiLogger.h"
#include    "marvellTypeMap.h"
//#include	"os_net_device.h"

#define wait_queue_head_t	TX_EVENT_FLAGS_GROUP
#define spinlock_t		    TX_SEMAPHORE
#define SEMAPHORE           TX_SEMAPHORE
#define SPIN_LOCK_UNLOCKED  {0}
#define WLAN_DRV_TIMER		TX_TIMER
#define PWLAN_DRV_TIMER		TX_TIMER*

//-jrs#define sdspi_notifier_t	mmc_notifier_rec_t
//-jrs#define sdspi_notifier_fn_p	mmc_notifier_fn_t
//-jrs#define SDSPI_REG_TYPE_USER	MMC_REG_TYPE_USER	
//-jrs#define sdspi_card_p		mmc_card_t		
//-jrs#define register_user(x)	sdio_register(SDSPI_REG_TYPE_USER, x, 0)
//-jrs#define unregister_user(x)	sdio_unregister(SDSPI_REG_TYPE_USER, x)
//-jrs#define sdio_free_irq(x, y)	sdio_free_irq(x)
#define cpu_to_le16		    le16_to_cpu
#define cpu_to_le32		    le32_to_cpu

#define IW_EV_LCP_LEN		(sizeof(struct iw_event) - sizeof(union iwreq_data))
#define OS_INTERRUPT_SAVE_AREA	TX_INTERRUPT_SAVE_AREA
#define OS_INT_DISABLE		TX_DISABLE
#define OS_INT_RESTORE		TX_RESTORE

#define OS_INIT_SEMAPHORE(x)    	tx_semaphore_create(x, #x, 1)
#define OS_FREE_LOCK(x)		        tx_semaphore_delete(x)

#define sprintf(x...)       minSprintf(x)
#define snprintf(x...)      minNSprintf(x)

#ifdef DBG_PRINTF
#define	printk(x...)		DPRINTF((DBG_SOFT | DBG_OUTPUT), (x))
#else
#define printk(x...)        ((void)0)
#endif

#define HZ		100

#define kmalloc(x, y)	MEM_MALLOC((x))
#define kzalloc(x, y)	MEM_CALLOC(1, (x))
#define kfree		    MEM_FREE_AND_NULL

//#define udelay(x)	Delay(x)
#define udelay(x)	cpu_spin_delay(x)
#define mdelay(x);	{	\
				if((x)<10)	\
					tx_thread_sleep(x);	\
				else	\
					tx_thread_sleep((x)/10);	\
			}\


#define init_waitqueue_head(x)  tx_event_flags_create((x), #x)   
#define delete_waitqueue(x)     tx_event_flags_delete((x))

#define ConfigureThreadPriority();	{ 						\
	int temp; 									\
											\
	tx_thread_priority_change(&priv->MainThread.txThread, THR_PRI_NORMAL, &temp); 	\
}


#define free_netdev(x);	{ 							\
    MEM_FREE_AND_NULL(x->priv);                             \
	MEM_FREE_AND_NULL(x); 									\
	x = NULL; 									\
}

#define OS_EVENT_FLAGS_CLEAR(x) tx_event_flags_set((x), 0, TX_AND)

#define wake_up_interruptible(x) 							\
			tx_event_flags_set(x, KTHREAD_WAIT_FLAG, TX_OR)
	
#define interruptible_sleep_on_timeout(x, y) { 						\
	u32	flags; 									\
	tx_event_flags_get(x, KTHREAD_WAIT_FLAG, TX_OR_CLEAR, &flags, 1000); 		\
}
	
#define interruptible_sleep_on(x) { 							\
	u32 flags; 									\
	tx_event_flags_get(x, KTHREAD_WAIT_FLAG, TX_OR_CLEAR, &flags, 			\
							TX_WAIT_FOREVER); 		\
}

#define wait_event_interruptible(x, y) ({ 						\
	u32 flags = 0; 									\
	int ret; 									\
	ret = tx_event_flags_get(&x, KTHREAD_WAIT_FLAG, TX_OR_CLEAR, &flags, 		\
							100 * 30 /* time out after 30 seconds */); 		\
})

#define wait_event_interruptible_timeout(x, y, z) ({ 						\
	u32 flags = 0; 									\
	int ret; 									\
	ret = tx_event_flags_get(&x, KTHREAD_WAIT_FLAG, TX_OR_CLEAR, &flags, z); 		\
})


// bugbug: should be checking the cond here and in wait_event_interruptable -jrs
#define os_wait_interruptible_timeout(waitq, cond, timeout) wait_event_interruptible(waitq, timeout)

#define spin_lock_init(x) tx_semaphore_create(x, #x, 1)
#define spin_lock_free(x) tx_semaphore_delete(x)

#define schedule();	{	\
		u32 flags;	\
		tx_event_flags_get(&thread->wait_q, KTHREAD_WAIT_FLAG, 	\
					TX_AND_CLEAR, &flags, TX_WAIT_FOREVER);	\
	}

// the y=y below avoids an unused param warning
#define spin_lock_irqsave(x, y)		    {y=y; tx_semaphore_get(x, TX_WAIT_FOREVER);}
#define spin_unlock_irqrestore(x, y)	{y=y; tx_semaphore_put(x);}
#define OS_ACQ_SEMAPHORE_BLOCK(x)	    tx_semaphore_get((x), TX_WAIT_FOREVER)
#define OS_REL_SEMAPHORE(x) 		    tx_semaphore_put((x))
#define os_sched_timeout(x)		        tx_thread_sleep((x)/10)
#define cli()				            TX_DISABLE
#define sti()				            TX_RESTORE
#define enable_irq(x)                   intEnable((x))
#define disable_irq(x)                  intDisable((x))
#define wait_queue_t			int
#define netif_wake_queue(x)		/* defined to nothing */
#define netif_carrier_on(x)		/* defined to nothing */
#define netif_carrier_ok(x)		1  /* defined to 1 to pass */
#define netif_start_queue(x)		/* defined to nothing */
#define netif_stop_queue(x)		/* defined to nothing */
#define netif_device_detach(x)  /* defined to nothing */
#define netif_device_attach(x)  /* defined to nothing */
#define netif_carrier_off(x)		/* defined to nothing */
#define netif_rx(x)             os_upload_rx_packet(x)
#define register_netdev(x)      0  /* defined to 0 to pass if failure check */
#define unregister_netdev(x)          /* defined to nothing */
#define sema_init(X, Y)			/* defined to nothing */
#define set_current_state(x)    /* defined to nothing */
#define KERN_ERR			/* defined to nothing */
#define KERN_NOTICE			/* defined to nothing */
#define KERN_DEBUG			/* defined to nothing */
#define KERN_INFO			/* defined to nothing */
#define KERN_ALERT			/* defined to nothing */
#define init_waitqueue_entry(x,y)	*x=*x/* defined to nothing; assignment eliminates unused var warnings */
#define add_wait_queue(x,y)		/* defined to nothing */
#define remove_wait_queue(x,y)		/* defined to nothing */
#define kthread_should_stop() FALSE
#define MODULE_DESCRIPTION(x)		/* defined to nothing */
#define MODULE_LICENSE(x)		/* defined to nothing */
#define MODULE_AUTHOR(x)		/* defined to nothing */
#define SET_MODULE_OWNER(x)		/* defined to nothing */	
#define MOD_INC_USE_COUNT		/* defined to nothing */		
#define MOD_DEC_USE_COUNT		/* defined to nothing */		
#define module_init(x)			/* defined to nothing */
#define module_exit(x)			/* defined to nothing */
#define module_param(x...)      /* defined to nothing */         
#define MODULE_GET              /* defined to nothing */
#define MODULE_PUT              /* defined to nothing */
#define UpdateTransStart(x)		/* defined to nothing */

extern SEMAPHORE AtomicIncDecSem;
#define atomic_read(x)		    (*(x))
#define atomic_inc(x)		    ({tx_semaphore_get(&AtomicIncDecSem, TX_WAIT_FOREVER);\
                                ((*x)++);\
                                tx_semaphore_put(&AtomicIncDecSem);})
#define atomic_dec(x)		    ({tx_semaphore_get(&AtomicIncDecSem, TX_WAIT_FOREVER);\
                                ((*x)--);\
                                tx_semaphore_put(&AtomicIncDecSem);})

static inline atomic_t atomic_dec_return(atomic_t * x)
{
        atomic_dec(x); 
        return *x;
}

#define netif_queue_stopped(x)		0

#define TASK_INTERRUPTIBLE		1
#define TASK_RUNNING			2



#endif /* _OS_HEADERS_H */
