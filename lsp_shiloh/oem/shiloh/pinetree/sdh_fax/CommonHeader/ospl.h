/*
 * $Id: ospl.h,v 1.17 2010/04/19 21:26:20 let1 Exp $
 *
 * Copyright (C) 2009 Conexant Systems Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __OSPL_H__
#define __OSPL_H__

typedef unsigned long	ULONG;
typedef unsigned int	UINT;
typedef int				INT;
typedef char			CHAR;
typedef void			VOID;
typedef unsigned long	DWORD;

typedef void *                      VOID_PTR;
typedef char *                      CHAR_PTR;
typedef unsigned char *             UCHAR_PTR;

#define OSPL_SUCCESS          0x00
#define OSPL_DELETED          0x01
#define OSPL_NO_MEMORY        0x10
#define OSPL_POOL_ERROR       0x02
#define OSPL_PTR_ERROR        0x03
#define OSPL_WAIT_ERROR       0x04
#define OSPL_SIZE_ERROR       0x05
#define OSPL_GROUP_ERROR      0x06
#define OSPL_NO_EVENTS        0x07
#define OSPL_OPTION_ERROR     0x08
#define OSPL_QUEUE_ERROR      0x09
#define OSPL_QUEUE_EMPTY      0x0A
#define OSPL_QUEUE_FULL       0x0B
#define OSPL_SEMAPHORE_ERROR  0x0C
#define OSPL_NO_INSTANCE      0x0D
#define OSPL_THREAD_ERROR     0x0E
#define OSPL_PRIORITY_ERROR   0x0F
#define OSPL_START_ERROR      0x10
#define OSPL_DELETE_ERROR     0x11
#define OSPL_RESUME_ERROR     0x12
#define OSPL_CALLER_ERROR     0x13
#define OSPL_SUSPEND_ERROR    0x14
#define OSPL_TIMER_ERROR      0x15
#define OSPL_TICK_ERROR       0x16
#define OSPL_ACTIVATE_ERROR   0x17
#define OSPL_THRESH_ERROR     0x18
#define OSPL_SUSPEND_LIFTED   0x19
#define OSPL_WAIT_ABORTED     0x1A
#define OSPL_WAIT_ABORT_ERROR 0x1B
#define OSPL_MUTEX_ERROR      0x1C
#define OSPL_NOT_AVAILABLE    0x1D
#define OSPL_NOT_OWNED        0x1E
#define OSPL_INHERIT_ERROR    0x1F
#define OSPL_RESOURCE_ERROR	  0x20

#define CHIPID_DC2 0
#define CHIPID_VEGAS 1
#define CHIPID_COBRA 2
#define CHIPID_GILA 3
#define CHIPID_IGUANA 4
#define CHIPID_DINGO 5
#define CHIPID_CHUCKWALLA 6
#define CHIPID_JACKAL 7
#define CHIPID_CARACAL 8
#define CHIPID_NASHOBA 9

#define OSPL_NULL				NULL

/******************************************************************************
*  1. INCLUDES
******************************************************************************/
# include "dc_type.h"
# include "dc_return_codes.h"
# include "dc_int_api.h"


/******************************************************************************
//# define OSRUN_WithoutInterrupts dc_int_without_interrupts_1
//# define OSRUN_WithoutInterrupts2 dc_int_without_interrupts_2
//# define OSRUN_WithoutInterrupts3 dc_int_without_interrupts_3
//# define OSRUN_CPSR dc__user_mode_get
//# define OSRUN_DIE dc__die
******************************************************************************/
UINT32 ospl_call_without_interrupts_0(UINT32 (*fun)(void));
UINT32 ospl_call_without_interrupts_1(UINT32 (*fun)(UINT32 d0), UINT32 d0);
UINT32 ospl_call_without_interrupts_2(UINT32 (*fun)(UINT32 d0, UINT32 d1), UINT32 d0, UINT32 d1);
UINT32 ospl_is_user_mode(void);
void ospl_die(void);

/******************************************************************************
// Debug
//# define OSRUN_DataPrint dc_debug_data_print
//# define OSRUN_CharPrint dc_debug_char_print
******************************************************************************/
void ospl_debug_data_print(char *what, UINT8 *s, int n);
void ospl_debug_char_print(char *txt, int numberofchars);
void ospl_debug_print_line(char fill);

/******************************************************************************
// Interrupt Control
//# define OSINTR_list dc_int_list
//# define OSINTR_init dc_int_init
//void dc__int_mask_convert(DC_RETURN_CODE (*dc_int_func)(DC_INT_ID id), 
//                         UINT32 mask, 
//                         UINT32 maskX);
//void dc__int_register_mask_convert(UINT32 mask,     
//                                  UINT32 maskX, 
//                                  void (*fun)(void), 
//                                  int priority);
//INLINE void OSRUN_FIRQEnable(UINT32 mask, UINT32 maskX)
//INLINE void OSRUN_IRQEnable(UINT32 mask, UINT32 maskX)
//INLINE void OSRUN_FIRQDisable(UINT32 mask, UINT32 maskX)
//INLINE void OSRUN_IRQDisable(UINT32 mask, UINT32 maskX)
//INLINE void OSSETUP_IRQ(UINT32 mask, UINT32 maskX, void (*fun)(void), int priority)
******************************************************************************/
#define OSPL_INT_PRIORITY_DEFAULT		0
#define OSPL_INT_PRIORITY_GPIO			12
#define OSPL_INT_PRIORITY_ET			10
#define OSPL_INT_PRIORITY_SPI			10
#define OSPL_INT_PRIORITY_TIMERB		8
#define OSPL_INT_PRIORTIY_FC			11			
#define OSPL_INT_PRIORITY_UART			0
#define OSPL_INT_PRIORITY_DT			10

#define OSPL_INT_DISABLE		0xC0
#define OSPL_INT_DISABLE_ALL	0x00
void ospl_int_list(void);
DC_RETURN_CODE ospl_int_init(void);
void ospl_int_mask_convert(DC_RETURN_CODE (*dc_int_func)(DC_INT_ID id), UINT32 mask, UINT32 maskX);
void ospl_int_register_mask_convert(UINT32 mask, UINT32 maskX, void (*fun)(void), int priority);
UINT32 ospl_interrupt_control(UINT32 control);
DC_RETURN_CODE ospl_int_register(DC_INT_ID id,void (*fun)(void), int priority);
DC_RETURN_CODE ospl_int_unregister(DC_INT_ID id, void (*fun)(void), int priority);
DC_RETURN_CODE ospl_int_set_priority(DC_INT_ID id, int priority);
DC_RETURN_CODE ospl_int_clear_priority(DC_INT_ID id, int priority);
void ospl_int_gp_dispatch(int status);
void ospl_set_gp_int_handler(DC_INT_GP_ID id, void (*handler)(DC_INT_GP_ID id), int priority);

static inline void ospl_firq_enable(UINT32 mask, UINT32 maskX)
{
    ospl_int_mask_convert(dc_int_firq_enable, mask, maskX);
}

static inline void ospl_irq_enable(UINT32 mask, UINT32 maskX)
{
    ospl_int_mask_convert(dc_int_irq_enable, mask, maskX);
}

static inline void ospl_firq_disable(UINT32 mask, UINT32 maskX)
{
    ospl_int_mask_convert(dc_int_disable, mask, maskX);
}

static inline void ospl_irq_disable(UINT32 mask, UINT32 maskX)
{
    ospl_int_mask_convert(dc_int_disable, mask, maskX);
}
  
static inline void ospl_register_irq(UINT32 mask, UINT32 maskX, void (*fun)(void), int priority)
{
    ospl_int_register_mask_convert(mask, maskX, fun, priority);
}


/******************************************************************************
// Memory Interface
//# define OSRUN_MICurrentMask dc__mi_current_mask_get
//DC_RETURN_CODE dc__mi_mask_convert(DC_RETURN_CODE (*fun)(DC_MI_ID), 
//                                  UINT16 activeMask, 
//                                  DC_RETURN_CODE (*un_fun)(DC_MI_ID));
//void dc__mi_current_mask_get(UINT16 *threadMask, 
//                            UINT16 *systemMask);
//INLINE int OSRUN_MIAdd(UINT32 activeMask)
//INLINE int OSRUN_MIDel(UINT32 activeMask)
//INLINE UINT16 OSRUN_MIRegister(UINT16 activeMask)
******************************************************************************/
# define OSPL_MIRD0 DC_MIRD0
# define OSPL_MIRD1 DC_MIRD1
# define OSPL_MIRD2 DC_MIRD2
# define OSPL_MIRD3 DC_MIRD3
# define OSPL_MIRD4 DC_MIRD4
# define OSPL_MIRD5 DC_MIRD5
# define OSPL_MIRD6 DC_MIRD6
# define OSPL_MIRD7 DC_MIRD7
# define OSPL_MIRD_ALL DC_MIRD_ALL

# define OSPL_MIWR0 DC_MIWR0
# define OSPL_MIWR1 DC_MIWR1
# define OSPL_MIWR2 DC_MIWR2
# define OSPL_MIWR3 DC_MIWR3
# define OSPL_MIWR4 DC_MIWR4
# define OSPL_MIWR5 DC_MIWR5
# define OSPL_MIWR6 DC_MIWR6
# define OSPL_MIWR7 DC_MIWR7
# define OSPL_MIWR_ALL DC_MIWR_ALL
void ospl_mi_current_mask_get(UINT16 *threadMask, UINT16 *systemMask);
DC_RETURN_CODE ospl_mi_mask_convert(DC_RETURN_CODE (*fun)(int), UINT16 activeMask, 
                                  DC_RETURN_CODE (*un_fun)(int));
void ospl_mi_current_mask_get(UINT16 *threadMask, UINT16 *systemMask);

/******************************************************************************
// Timer Management
******************************************************************************/
typedef unsigned int OSPL_TIMER;
VOID        ospl_timer_initialize(void);
//UINT        ospl_timer_activate(OSPL_INTERNAL_TIMER *timer_ptr);
UINT        ospl_timer_activate_api(OSPL_TIMER *timer_ptr);
UINT        ospl_timer_change(OSPL_TIMER *timer_ptr, ULONG initial_ticks, 
                                                    ULONG reschedule_ticks);
UINT        ospl_timer_create(OSPL_TIMER *timer_ptr, CHAR *name_ptr, 
                VOID (*expiration_function)(ULONG), ULONG expiration_input,
                ULONG initial_ticks, ULONG reschedule_ticks, UINT auto_activate);
//UINT        ospl_timer_deactivate(TX_INTERNAL_TIMER *timer_ptr);
UINT        ospl_timer_deactivate_api(OSPL_TIMER *timer_ptr);
UINT        ospl_timer_delete(OSPL_TIMER *timer_ptr);
UINT        ospl_timer_info_get(OSPL_TIMER *timer_ptr, CHAR **name, UINT *active, ULONG *remaining_ticks, 
                ULONG *reschedule_ticks, OSPL_TIMER **next_timer);
VOID        ospl_timer_thread_entry(ULONG timer_thread_input);
ULONG       ospl_time_get(void);
VOID        ospl_time_set(ULONG new_time);


/******************************************************************************
// Time Management
//# define OSRUN_WaitCycles dc_time_wait_cycles
//# define OSRUN_UTime dc_utime_get
//# define OSRUN_ElapsedUSec dc_utime_elapsed_usec
//# define OSRUN_DiffUSec dc_utime_diff_usec
//void OSRUN_Sleep(UINT32 milliSec);
//void OSRUN_Time(volatile UINT32 *time);
******************************************************************************/
void ospl_time_wait_cycles(UINT16 cycles);
void ospl_sleep(UINT32 milliSec);
void ospl_time(volatile UINT32 *time);

UINT32 ospl_time_elapsed_msec(volatile UINT32 *t0, volatile UINT32 *t1);
UINT32 ospl_time_elapsed_sec(volatile UINT32 *t0, volatile UINT32 *t1);
UINT32 ospl_time_diff_msec(volatile UINT32 *t0);
UINT32 ospl_get_time(void);
UINT32 ospl_time_diff_sec(volatile UINT32 *t0);

/******************************************************************************
// Low-level Hardware Access
//# define OSRUN_reg_update dc_reg_update
******************************************************************************/
UINT8 ospl_reg_update(UINT32 address, UINT8 mask, char op);

/******************************************************************************
// Thread Management
//UINT32 OSSETUP_DefineProcess(void (*task)(UINT32 taskID), void *pstack, UINT32 stack_size, char *taskString);
******************************************************************************/
typedef unsigned int OSPL_THREAD;
#define OSPL_AUTO_START			1
#define OSPL_NO_TIME_SLICE		0
#define OSPL_PRIORITY_HIGHEST		DC_THREAD_PRIORITY_VERY_HIGH
#define OSPL_PRIORITY_HIGH			DC_THREAD_PRIORITY_HIGH
#define OSPL_PRIORITY_MEDIUM		DC_THREAD_PRIORITY_MEDIUM
#define OSPL_PRIORITY_LOW			DC_THREAD_PRIORITY_LOW
#define OSPL_PRIORITY_LOWEST		DC_THREAD_PRIORITY_VERY_LOW

UINT32 ospl_create_process(void (*task)(UINT32 taskID), void *pstack, UINT32 stack_size, char *taskString);
OSPL_THREAD* ospl_thread_identify(void);
char* ospl_thread_name(OSPL_THREAD* id);
UINT ospl_thread_delete(OSPL_THREAD *thread_ptr);
UINT ospl_thread_sleep(ULONG timer_ticks);
UINT ospl_thread_terminate(OSPL_THREAD *thread_ptr);

/******************************************************************************
// Thread Local Storage
******************************************************************************/
struct oa_tls_s;
void* ospl_tls_get_ptr(OSPL_THREAD* thr);
struct oa_tls_s* ospl_oa_tls(OSPL_THREAD* thr);

/******************************************************************************
// Synchronization Objects
// int OSRUN_SemaphoreLock(UINT8 semaphoreIndex);
// void OSRUN_SemaphoreUnlock(UINT8 semaphoreIndex);
// int OSRUN_SemaphoreWait(UINT8 semaphoreIndex);
// int OSRUN_SemaphoreWaitTime(UINT8 semaphoreIndex, UINT32 timeout_ms);
// void OSRUN_SemaphorePop(UINT8 semaphoreIndex, int ostate);
******************************************************************************/

typedef unsigned int			OSPL_MUTEX;
typedef OSPL_MUTEX				OSPL_SEMAPHORE;
#define OSPL_NO_WAIT			0
#define OSPL_WAIT_FOREVER		0xFFFFFFFFUL
#define OSPL_INHERIT			1
#define OSPL_NO_INHERIT			0
#define OSPL_AUTO_ACTIVATE		1

UINT ospl_mutex_create(OSPL_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit);
UINT ospl_mutex_delete(OSPL_MUTEX *mutex_ptr);
UINT ospl_mutex_get(OSPL_MUTEX *mutex_ptr, ULONG wait_option);
UINT ospl_mutex_prioritize(OSPL_MUTEX *mutex_ptr);
UINT ospl_mutex_put(OSPL_MUTEX *mutex_ptr);
void ospl_mutex_cleanup(OSPL_THREAD *thread_ptr);
void ospl_mutex_priority_change(OSPL_THREAD *thread_ptr, UINT priority, UINT threshold);

int   ospl_semaphore_count(OSPL_SEMAPHORE* sem);
UINT  ospl_semaphore_create(OSPL_SEMAPHORE *semaphore_ptr, CHAR *name_ptr, ULONG initial_count);
UINT  ospl_semaphore_delete(OSPL_SEMAPHORE *semaphore_ptr);
UINT  ospl_semaphore_get(OSPL_SEMAPHORE *semaphore_ptr, ULONG wait_option);
UINT  ospl_semaphore_put(OSPL_SEMAPHORE *semaphore_ptr);

/******************************************************************************
// Event flags
******************************************************************************/
typedef unsigned int	OSPL_EVENT_FLAGS_GROUP;
#define		OSPL_AND		(0x02)
#define		OSPL_AND_CLEAR	(0x03)
#define		OSPL_OR			(0x00)
#define		OSPL_OR_CLEAR	(0x01)
UINT ospl_event_flags_create(OSPL_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr);
UINT ospl_event_flags_delete(OSPL_EVENT_FLAGS_GROUP *group_ptr);
UINT ospl_event_flags_get(OSPL_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags, UINT get_option,
	ULONG *actual_flags_ptr, ULONG wait_option);
UINT ospl_event_flags_set(OSPL_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set,UINT set_option);

/******************************************************************************
// Message queues
******************************************************************************/
typedef unsigned int	OSPL_QUEUE;
UINT        ospl_queue_create(OSPL_QUEUE *queue_ptr, CHAR *name_ptr, UINT message_size, 
                        VOID *queue_start, ULONG queue_size);
UINT        ospl_queue_delete(OSPL_QUEUE *queue_ptr);
UINT        ospl_queue_flush(OSPL_QUEUE *queue_ptr);
UINT        ospl_queue_info_get(OSPL_QUEUE *queue_ptr, CHAR **name, ULONG *enqueued, ULONG *available_storage,
                    OSPL_THREAD **first_suspended, ULONG *suspended_count, OSPL_QUEUE **next_queue);
UINT        ospl_queue_prioritize(OSPL_QUEUE *queue_ptr);
UINT        ospl_queue_receive(OSPL_QUEUE *queue_ptr, VOID *destination_ptr, ULONG wait_option);
UINT        ospl_queue_send(OSPL_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);
UINT        ospl_queue_front_send(OSPL_QUEUE *queue_ptr, VOID *source_ptr, ULONG wait_option);
VOID        ospl_queue_cleanup(OSPL_THREAD *thread_ptr);

/******************************************************************************
// Byte pools
******************************************************************************/
#define ENABLE_BYTE_POOLS		0
typedef unsigned int OSPL_BYTE_POOL;
UINT ospl_byte_pool_create(OSPL_BYTE_POOL *pool_ptr, CHAR *name_ptr, VOID *pool_start, ULONG pool_size);
UINT ospl_byte_pool_delete(OSPL_BYTE_POOL *pool_ptr);
UINT ospl_byte_allocate(OSPL_BYTE_POOL *pool_ptr, VOID **memory_ptr, ULONG memory_size, ULONG wait_option);
UINT ospl_byte_release(VOID *memory_ptr);

/******************************************************************************
// Logging
******************************************************************************/
DC_RETURN_CODE ospl_log_string(char *str);
DC_RETURN_CODE ospl_log_printf(char *format, ...);

// These declarations and defines belong to OSPL transition layer
//#define OSRUN_WithoutInterrupts0		ospl_call_without_interrupts_0
//#define OSRUN_TextPrint(txt, lines)	ospl_log_string(txt)
//#define OSRUN_DebugString				ospl_log_string
//#define OSRUN_PrintLine				ospl_debug_print_line
//#define OSRUN_Dpf						ospl_log_printf
//#define OSRUN_ElapsedMSec(volatile UINT32 *t0, volatile UINT32 *t1)
//#define INLINE UINT32 OSRUN_ElapsedMSec(volatile UINT32 *t0, volatile UINT32 *t1)
//#define INLINE UINT32 OSRUN_ElapsedSec(volatile UINT32 *t0, volatile UINT32 *t1)
//#define INLINE UINT32 OSRUN_DiffMSec(volatile UINT32 *t0)
//#define INLINE UINT32 OSRUN_DiffSec(volatile UINT32 *t0)

int ospl_thread_register(OSPL_THREAD* thread);
int ospl_thread_unregister(OSPL_THREAD* thread);
void* ospl_thread_get_tls(OSPL_THREAD* thread);
UINT32 ospl_get_process_id(void);

typedef struct oa_tls_s {
    UINT  process_num;         //Oasis process number, for backwards compatibility, will
                               // eventually go.

    // The following 4 fields are for the older log buffer facility, and will eventually go.
    ULONG print_ptr;           //address in log buffer
    ULONG print_sync;          //Log message synchronization value
    ULONG print_base;          //Base address of log buffer
    ULONG print_size;          //Size of log buffer, in bytes

    ULONG mi_context[2 * 16];  //Address and control registers for 8 read and 8 write channels
    unsigned short  mi_mask;   //bitmask indicating which MI channels are registered.

    // This is also for the older log buffer facility
    char Dpf[64];              //temporary buffer for log messages

    /*
      This #if statement is here because of issues linking with
      object code compiled by a customer, leading to problems when
      this struct size changes.  Eventually the #ifdef should go
      away, and we should also have some sort of version/size checking
      for such black box code.
    */
#if defined(DC_TX_PORT_VERSION) && (DC_TX_PORT_VERSION > 0)
    UINT  log_id;               //id used by the newer log buffer facility
#endif
} oa_tls_t;

/******************************************************************************
// Generic Record Keeping
******************************************************************************/
#define USE_RECORD_KEEPING 0
typedef int (*cmp_record_func_t)(void* rec, void* param);
typedef void (*print_record_func_t)(void* rec);
#if USE_RECORD_KEEPING
void* ospl_init_record_keeping(char *name, 
							int size, 
							cmp_record_func_t compare_f,
							print_record_func_t print_f);
void ospl_close_record_keeping(void* rk);
void ospl_add_record(void* rk, void* ptr, int size);
void ospl_del_record(void* rk, void* ptr);
void ospl_print_records(void* rk);
#else
#define ospl_init_record_keeping(name, size, compare_f, print_f) (NULL)
#define ospl_close_record_keeping(a)
#define ospl_add_record(rk, ptr, size)
#define ospl_del_record(rk, ptr)
#define ospl_print_records(rk)
#endif
/******************************************************************************
// Managed memory allocations
******************************************************************************/
#define USE_MANAGED_MEMORY_ALLOCATIONS 0
#if USE_MANAGED_MEMORY_ALLOCATIONS
void ospl_mem_manager_init(void);
void* ospl_malloc(int size, int flags, char* name);
void* ospl_zalloc(int size, int flags, char* name);
void ospl_free(void* ptr);
void ospl_print_allocations(void);
#else
#define ospl_mem_manager_init()
#ifdef __KERNEL__
#define ospl_malloc(size, flags, name) kmalloc(size, flags)
#define ospl_zalloc(size, flags, name) kzalloc(size, flags)
#define ospl_free	kfree
#else
#include <stdlib.h>
#define ospl_malloc(size, flags, name) malloc(size)
#define ospl_zalloc(size, flags, name) calloc(size, 1)
#define ospl_free	free
#endif
#define ospl_print_allocations()
#endif

void* ospl_alloc_aligned(unsigned int size, unsigned int align);
void* ospl_alloc_aligned_min(unsigned int size);

#define is_address_in_range(address, start, size) \
	(((unsigned int)address >= (unsigned int)start) && ((unsigned int)address < ((unsigned int)start + size)))

#endif

/******************************************************************************
// Cache management
******************************************************************************/
void ospl_cache_flush_range(void* start, void* end);
void ospl_cache_invalidate_range(void* start, void* end);
void ospl_cache_clean_range(void* start, void* end);

/******************************************************************************
// Short term memory mapping
******************************************************************************/
void* ospl_create_temp_mapping(unsigned long phys_address, int size);
void ospl_delete_temp_mapping(void* virt_address);

/******************************************************************************
// Asynchronous signals
******************************************************************************/
void ospl_install_signal_handler(void);
int ospl_check_signal(void);
void ospl_reset_signal(void);
