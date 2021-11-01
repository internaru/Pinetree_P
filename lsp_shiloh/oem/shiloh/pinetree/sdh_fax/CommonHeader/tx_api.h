#ifndef _TX_API_H_
#define _TX_API_H_

// Compatibility header
#include "ospl.h"

#define	TX_1_ULONG			1
#define TX_2_ULONG          2
#define TX_4_ULONG          4
#define TX_8_ULONG          8
#define TX_16_ULONG         16

#define	TX_NO_EVENTS				OSPL_NO_EVENTS
#define TX_SUCCESS					OSPL_SUCCESS
#define TX_NO_MEMORY				OSPL_NO_MEMORY
#define TX_PRIORITY_ERROR			OSPL_PRIORITY_ERROR

#define TX_EVENT_FLAGS_GROUP		OSPL_EVENT_FLAGS_GROUP
#define TX_THREAD					OSPL_THREAD
#define TX_MUTEX					OSPL_MUTEX
#define TX_QUEUE					OSPL_QUEUE

#define TX_OR_CLEAR					OSPL_OR_CLEAR
#define TX_OR						OSPL_OR
#define TX_AND_CLEAR				OSPL_AND_CLEAR
#define TX_AND						OSPL_AND

#define TX_WAIT_FOREVER				OSPL_WAIT_FOREVER
#define TX_NO_WAIT					OSPL_NO_WAIT
#define TX_NO_TIME_SLICE			OSPL_NO_TIME_SLICE
#define TX_AUTO_START				OSPL_AUTO_START
#define TX_NO_INHERIT				OSPL_NO_INHERIT
#define TX_QUEUE_FULL				OSPL_QUEUE_FULL

#define TX_INT_DISABLE         OSPL_INT_DISABLE



#define tx_thread_delete			ospl_thread_delete
#define tx_thread_sleep				ospl_thread_sleep
#define tx_event_flags_create		ospl_event_flags_create
#define tx_event_flags_delete		ospl_event_flags_delete
#define tx_event_flags_get			ospl_event_flags_get
#define tx_event_flags_set			ospl_event_flags_set
#define tx_time_get					ospl_time_get

#define tx_mutex_get				ospl_mutex_get
#define tx_mutex_put				ospl_mutex_put
#define tx_mutex_create				ospl_mutex_create
#define tx_mutex_delete				ospl_mutex_delete

#define tx_queue_receive			ospl_queue_receive
#define tx_queue_send				ospl_queue_send
#define tx_queue_create				ospl_queue_create
#define tx_queue_delete				ospl_queue_delete
#define tx_queue_info_get			ospl_queue_info_get

#define tx_semaphore_create			ospl_semaphore_create
#define tx_semaphore_delete			ospl_semaphore_delete
#define tx_semaphore_get			ospl_semaphore_get
#define tx_semaphore_put			ospl_semaphore_put

#define tx_interrupt_control     ospl_interrupt_control
#define tx_byte_pool_create      ospl_byte_pool_create
#define tx_thread_terminate      ospl_thread_terminate
#define tx_byte_pool_delete      ospl_byte_pool_delete
#define tx_byte_release          ospl_byte_release
#define tx_byte_allocate         ospl_byte_allocate
typedef struct TX_BYTE_POOL_STRUCT
{

    /* Define the byte pool ID used for error checking.  */
    ULONG       tx_byte_pool_id;

    /* Define the byte pool's name.  */
    CHAR_PTR    tx_byte_pool_name;

    /* Define the number of available bytes in the pool.  */
    ULONG       tx_byte_pool_available;

    /* Define the number of fragments in the pool.  */
    ULONG       tx_byte_pool_fragments;

    /* Define the head pointer of byte pool.  */
    CHAR_PTR    tx_byte_pool_list;

    /* Define the search pointer used for initial searching for memory
       in a byte pool.  */
    CHAR_PTR    tx_byte_pool_search;

    /* Save the start address of the byte pool's memory area.  */
    CHAR_PTR    tx_byte_pool_start;

    /* Save the byte pool's size in bytes.  */
    ULONG       tx_byte_pool_size;

    /* This is used to mark the owner of the byte memory pool during
       a search.  If this value changes during the search, the local search
       pointer must be reset.  */
    struct TX_THREAD_STRUCT  *tx_byte_pool_owner;

    /* Define the byte pool suspension list head along with a count of
       how many threads are suspended.  */
    struct TX_THREAD_STRUCT  *tx_byte_pool_suspension_list;
    ULONG                    tx_byte_pool_suspended_count;

    /* Define the created list next and previous pointers.  */
    struct TX_BYTE_POOL_STRUCT 
                *tx_byte_pool_created_next,    
                *tx_byte_pool_created_previous;

} TX_BYTE_POOL;

#endif
