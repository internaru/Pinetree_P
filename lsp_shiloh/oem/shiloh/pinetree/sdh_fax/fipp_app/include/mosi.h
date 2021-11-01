/* 
--------------------------------------------------------------------------------
Copyright 2004 Conexant Systems, Inc.
4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
All Rights Reserved Worldwide.
Information Contained Herein is Proprietary to Conexant Systems, Inc.
--------------------------------------------------------------------------------

$Header: /home/cvsref/rd/linux/package/digicolor/reference_code/fax/src/mosi.h,v 1.1 2009/07/30 21:03:06 yooh1 Exp $

Module Name: 

Description:

Tab size:
	4 spaces
--------------------------------------------------------------------------------
*/
/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/

/******************************************************************************
*  2. DEFINES
******************************************************************************/

/******************************************************************************
*  3. STRUCTURES
******************************************************************************/

/******************************************************************************
*  4. ALLOCS
******************************************************************************/

/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/

#ifndef _MOSI_H
#define _MOSI_H

#include "dc_return_codes.h"

/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/

/******************************************************************************
*  2. DEFINES
******************************************************************************/
#define RTOS_LINUX 

#if defined(RTOS_THREADX)
#include "Mosi_ThreadX.h"
//#define ASSERT(x)	if(x == FALSE) dc_log_printf("!!assert error !!\n");  
#elif defined(RTOS_LINUX)	
#include "mosi_linux.h"
#include <stdio.h>
#include <assert.h>
//#define ASSERT(x)	if(x == FALSE) {/*dc_log_printf("!!assert error !!\n");*/ assert(x);}
#endif

#define TRACE_ALWAYS	printf
#define TRACE_RELEASE	printf
#define FAX_TRACE		printf
#define TRACE			printf
#define TRACE_ERROR     printf

/******************************************************************************
*  3. STRUCTURES/TYPEDEF
******************************************************************************/
typedef void *OS_TASK;
typedef void *OS_TASKID;
typedef void *OS_QUEUE;
typedef void *OS_QUEUEID;
typedef void *OS_SEMAPHORE;
typedef void *OS_MUTEX;
typedef void *OS_TIMER;
typedef void *OS_TIMERID;
typedef void *OS_MEMORY_POOL;
typedef void *OS_PARTITION_POOL;
typedef void *OS_MEM_PTR;
typedef UINT32 OS_THREAD_PARAM;
typedef void (*OS_THREAD_ENTRY)(OS_THREAD_PARAM);
typedef void (*OS_TIMERCALLBACK)( OS_TIMERID, void *, UINT32, UINT32);
typedef void (*OS_IRQCALLBACK)( void *);

/******************************************************************************
*  4. ALLOCS
******************************************************************************/

/******************************************************************************
*  5. FUNCTIONS
******************************************************************************/







#if 0		// THY

#include <stddef.h>	// size_t
#include "Defs.h"

#ifndef MOSI_NO_MACRO_API
#define MOSI_NO_MACRO_API 1
#endif

#if !MOSI_NO_MACRO_API
// =======================================================================================
// Choose OS option
// =======================================================================================
#if defined(RTOS_OSC)
#include "Mosi_OSC.h"
#elif defined(RTOS_NUCLEUS)
#include "Mosi_Nucleus.h"
#elif defined(RTOS_THREADX)
#include "Mosi_ThreadX.h"
#elif defined(RTOS_ITRON)
#include "Mosi_Itron.h"
#elif defined(RTOS_MQX)
#include "Mosi_MQX.h"
#elif defined(RTOS_LINUX)
#include "Mosi_Linux.h"
#elif defined(RTOS_NONE)
#include "Mosi_NoRTOS.h"
#else
#error No OS Specified!
#endif
// =======================================================================================

#else //MOSI_NO_MACRO_API

#if defined(RTOS_ITRON)
	#define RTOS_STR	"uiTron"
	#define RTOS_VER	"4.0"
	#ifdef BOOL
	#undef BOOL
	#endif
	#include   "t_services.h"
	#include   "kernel_id.h"
	#if !DIAMONDBACK
	#include   "psptypes.h"
	#include   "arca6.h"
	#endif
	#define ITRON_VER	40
	#define DEFAULT_TIMER_STACK_SIZE	OSC_DEFAULT_STACK_SIZE
#elif defined(RTOS_MQX)
	#define RTOS_STR	"MQX"
	#define RTOS_VER	(void *)_mqx_version
	#include	"mqx.h"
	#include	"part.h"
	#include	"mutex.h"
	#include	"sem.h"
	#include	"message.h"
	#include    "arca6.h"
	#include	"timer.h"
#elif defined(RTOS_LINUX)
#include "Mosi_Linux.h"
#elif defined(RTOS_NONE)
#include "Mosi_NoRTOS.h"
#else
#error No OS Specified!
#endif

typedef void *OS_TASK;
typedef void *OS_TASKID;
typedef void *OS_QUEUE;
typedef void *OS_QUEUEID;
typedef void *OS_SEMAPHORE;
typedef void *OS_MUTEX;
typedef void *OS_TIMER;
typedef void *OS_TIMERID;
typedef void *OS_MEMORY_POOL;
typedef void *OS_PARTITION_POOL;
typedef void *OS_MEM_PTR;
typedef UINT32 OS_THREAD_PARAM;
typedef void (*OS_THREAD_ENTRY)(OS_THREAD_PARAM);
typedef void (*OS_TIMERCALLBACK)( OS_TIMERID, void *, UINT32, UINT32);
typedef void (*OS_IRQCALLBACK)( void *);

typedef enum
{
	OS_SUCCESS,
	OS_RCV_TIMEOUT,
	OS_SEM_TIMEOUT,
	OS_TIMEOUT,
	OS_INVALID_HANDLE,
	OS_OUT_OF_MEMORY,
	OS_INVALID_OBJ,
	OS_INVALID_STATE,
	OS_INVALID_PARAM,
	OS_NOT_SUPPORTED,
	OS_OVER_LIMIT,
	OS_FAILURE,
	OS_RETURN_LAST
} OS_RETURN;

#define OS_MSG_TIMED_OUT(x)	((x) == OS_RCV_TIMEOUT)
#define OS_SEM_TIMED_OUT(x)	((x) == OS_SEM_TIMEOUT)
#define OS_TIMED_OUT(x)		(OS_MSG_TIMED_OUT(x) || OS_SEM_TIMED_OUT(x))
#define OS_INVALID_QUEUE	(0)

typedef enum
{
	OS_NO_PREEMPT = 1,
	OS_PREEMPT,
	OS_NO_TIMESLICE,
	OS_FIFO_QUEUE,
	OS_PRIORITY_QUEUE,
	OS_BLOCKING_QUEUE,
	OS_NONBLOCKING_QUEUE,
	OS_TIMER_ELAPSED_TIME_MODE,
	OS_NO_START = 1 << 8,
	OS_START = 1 << 9,
	OS_DSP_TASK = 1 << 10,
	OS_XY_TASK = 1 << 11,
	OS_NO_SUSPEND = 0,
	OS_SUSPEND = 0xffffffff,
	OS_PARAM_LAST
} OS_PARAM;

typedef enum
{
	OS_PRIORITY_BUMP_LOWER,
	OS_PRIORITY_LOWEST,
	OS_PRIORITY_LOWER,
	OS_PRIORITY_LOW,
	OS_PRIORITY_NORMAL,
	OS_PRIORITY_HIGH,
	OS_PRIORITY_HIGHER,
	OS_PRIORITY_HIGHEST,
	OS_PRIORITY_BUMP_HIGHER
} OS_PRIORITY;

typedef enum 
{
	OS_TYPE_MEM_POOL, 
	OS_TYPE_PARTITION_POOL, 
	OS_TYPE_TASK, 
	OS_TYPE_SEM, 
	OS_TYPE_MUTEX, 
	OS_TYPE_QUEUE, 
	OS_TYPE_TIMER, 
	OS_TYPE_LAST
} OS_OBJ_TYPE;

/* Memory */
OS_RETURN OS_Create_Memory_Pool(OS_MEMORY_POOL *pool, char *name, void * start, UINT32 size, UINT32 min, OS_PARAM suspend);
OS_RETURN OS_Allocate_Memory(OS_MEMORY_POOL *pool, OS_MEM_PTR *pmemptr, UINT32 size, int suspend);
OS_RETURN OS_Deallocate_Memory(OS_MEM_PTR memptr);	
OS_MEM_PTR OS_Allocate_Pages (unsigned siz);
OS_RETURN OS_Allocate_Zero_Memory(OS_MEMORY_POOL *pool, OS_MEM_PTR *pmemptr, UINT32 size, int suspend);
OS_RETURN OS_Create_Partition_Pool(OS_PARTITION_POOL *pool, char *name, void *start, UINT32 size, UINT32 unit, OS_PARAM suspend);
OS_RETURN OS_Allocate_Partition(OS_PARTITION_POOL *pool, OS_MEM_PTR *pmemptr, int block);
OS_RETURN OS_Deallocate_Partition(OS_PARTITION_POOL *pool, OS_MEM_PTR memptr);
UINT32 OS_Get_Partition_Block_Size(OS_PARTITION_POOL pool);
UINT32 OS_Partition_Get_Total_Blocks(OS_PARTITION_POOL pool);
UINT32 OS_Partition_Get_Free_Blocks(OS_PARTITION_POOL pool);
UINT32 OS_Get_Partition_Block_Size(OS_PARTITION_POOL pool);
UINT32 OS_Partition_Get_Max_Used_Blocks(OS_PARTITION_POOL pool);
UINT32 OS_Memory_Get_Size(OS_MEM_PTR memptr);
#define OS_Allocate_Stack_Memory	OS_Allocate_Memory
#define OS_Allocate_Queue_Memory	OS_Allocate_Memory
#define OS_Partition_Get_Block_Size(pool_ptr) OS_Get_Partition_Block_Size(pool_ptr)

/* task */

OS_RETURN OS_Create_Task(OS_TASK *thread, char *name, OS_THREAD_ENTRY entry, UINT32 data, UINT32 ssize, OS_PRIORITY priority, UINT32 slice, OS_PARAM preempt, OS_PARAM autostart);
OS_RETURN OS_Destroy_Task(OS_TASK tskid);
OS_RETURN OS_Resume_Task(OS_TASK tskid);
OS_RETURN OS_Suspend_Task(OS_TASK tskid);
OS_RETURN OS_Yield_Task();
OS_RETURN OS_Sleep_Task(int delay);				
OS_TASKID OS_Current_Task_Id(void);
OS_RETURN OS_Delete_Current_Task(void);
OS_RETURN OS_Change_Task_Priority(OS_TASK task, OS_PRIORITY priority);
//#define OS_Id_Task(task)									(task)
//#define OS_Is_Current_Task(task)							(task == OS_Current_Task_Id())
//#define OS_Delete_Current_Task()							exd_tsk()
//OS_RETURN OS_Sleep(int delay);
//#define OS_Ready_Task			rsm_tsk
//#define OS_Current_Task_Id		OS_Current_Task_Pointer
//#define OS_Relinquish_Task		OS_Yield					
//#define OS_Relinquish			OS_Yield
//#define OS_Block_Task							OS_Suspend_Task
//#define OS_Is_Same_Task(task, taskid)		(OS_Id_Task(task) == taskid)

// These two must appear within the same nested brace level ({}).
void OS_Suspend_Scheduling();
void OS_Resume_Scheduling();

/* semaphore */
OS_RETURN OS_Create_Semaphore(OS_SEMAPHORE *sem, char *name, int init, OS_PARAM suspend);
OS_RETURN OS_Delete_Semaphore(OS_SEMAPHORE *sem);
OS_RETURN OS_Obtain_Semaphore(OS_SEMAPHORE *sem, int block);
OS_RETURN OS_Release_Semaphore(OS_SEMAPHORE *sem);	
OS_RETURN OS_Clear_Semaphore(OS_SEMAPHORE *sem);	
#define OS_Take_Semaphore		OS_Obtain_Semaphore		
#define OS_Give_Semaphore		OS_Release_Semaphore

/* mutex */
OS_RETURN OS_Create_Mutex(OS_MUTEX *mutex, char *name, OS_PARAM suspend);
OS_RETURN OS_Delete_Mutex(OS_MUTEX *mutex);
OS_RETURN OS_Obtain_Mutex(OS_MUTEX *mutex, int block);
OS_RETURN OS_Release_Mutex(OS_MUTEX *mutex);

/* queue */
OS_RETURN OS_Create_Queue(OS_QUEUE *queue, char *name, UINT16 num, int mtype, UINT16 msize, OS_PARAM suspend);
OS_RETURN OS_Delete_Queue(OS_QUEUE *queue);
OS_RETURN OS_Send_To_Queue(OS_QUEUE *qid, void *msg, UINT32 size, int suspend);
OS_RETURN OS_Receive_From_Queue(OS_QUEUE *qid, void *msg, UINT32 sizeIn, UINT32 *sizeOut, int suspend);
#define OS_QUEUE_SIZE(num, size)			((num)*(size))
//#define OS_Id_Queue(queue)					(queue)
//#define OS_Is_Same_Queue(queue, queueid)	(OS_Id_Queue(queue) == queueid)	


/* timer */
#define MILLISECONDS(x)			(x)
#define SECONDS(x)				(MILLISECONDS(x) * 1000)
OS_TIMERID OS_Timer_Start(OS_TIMERCALLBACK notification_function_ptr, void* notification_data_ptr,OS_PARAM mode,UINT32 msec);
OS_RETURN OS_Timer_Cancel(OS_TIMERID id);
UINT32 OS_GetTickCount(int iIndex);
UINT32 OS_Time_Get_Ticks_per_Sec();

/* IRQ */
OS_RETURN OS_HOOK_IRQ(int irq, OS_IRQCALLBACK pfn, void *data);
OS_RETURN OS_ENABLE_IRQ();
OS_RETURN OS_DISABLE_IRQ();

// Cache operations
void OS_InvalidateCachedData(const OS_MEM_PTR p, unsigned size);
void OS_FlushCachedData(OS_MEM_PTR p, UINT32 size);
UINT32 OS_MapBusToVirtual(OS_MEM_PTR BusAddress);
UINT32 OS_MapVirtualToBus(OS_MEM_PTR VirtualAddress);
//#define OS_VirtualToBus(a)		(a)
//#define OS_BusToVirtual(a)		(a)

/* general */
char *GetRtosErrorText(int errCode);
char *CreateName(char *acPart1, char *acPart2);
int OS_Store_Name(OS_OBJ_TYPE type, UINT32 id, char *name);
char * OS_Get_Name(OS_OBJ_TYPE type, UINT32 id);


// remove these line by line to complete OSC to OS porting job
#define OSC_Create_Memory_Pool			OS_Create_Memory_Pool
#define OSC_Allocate_Memory				OS_Allocate_Memory
#define OSC_Deallocate_Memory			OS_Deallocate_Memory	
#define OSC_Allocate_Pages				OS_Allocate_Pages
#define OSC_Allocate_Zero_Memory		OS_Allocate_Zero_Memory
#define OSC_Create_Partition_Pool		OS_Create_Partition_Pool
#define OSC_Allocate_Partition			OS_Allocate_Partition
#define OSC_Deallocate_Partition		OS_Deallocate_Partition
#define OSC_Get_Partition_Block_Size	OS_Get_Partition_Block_Size
#define OSC_Partition_Get_Total_Blocks	OS_Partition_Get_Total_Blocks
#define OSC_Partition_Get_Free_Blocks	OS_Partition_Get_Free_Blocks
#define OSC_Get_Partition_Block_Size	OS_Get_Partition_Block_Size
#define OSC_Partition_Get_Max_Used_Blocks OS_Partition_Get_Max_Used_Blocks
#define OSC_Memory_Get_Size				OS_Memory_Get_Size
#define OSC_Create_Task					OS_Create_Task
#define OSC_Destroy_task				OS_Destroy_Task
#define OSC_Delete_Current_Task			OS_Delete_Current_Task
#define OSC_Block_Task					OS_Suspend_Task
#define OSC_Suspend_Task				OS_Suspend_Task
#define OSC_Resume_Task					OS_Resume_Task
#define OSC_Ready_Task					OS_Resume_Task
#define OSC_Yield_Task					OS_Yield_Task
#define OSC_Yield						OS_Yield_Task
#define OSC_Relinquish_Task				OS_Yield_Task
#define OSC_Sleep						OS_Sleep_Task
#define OSC_Sleep_Task					OS_Sleep_Task
#define OSC_Current_Task_Pointer		OS_Current_Task_Id
#define OSC_Current_Task_Id				OS_Current_Task_Id
#define OSC_Change_Task_Priority		OS_Change_Task_Priority
#define OSC_Suspend_Scheduling			OS_Suspend_Scheduling
#define OSC_Resume_Scheduling			OS_Resume_Scheduling
#define OSC_Create_Semaphore			OS_Create_Semaphore
#define OSC_Delete_Semaphore			OS_Delete_Semaphore
#define OSC_Obtain_Semaphore			OS_Obtain_Semaphore
#define OSC_Release_Semaphore			OS_Release_Semaphore
#define OSC_Take_Semaphore				OS_Obtain_Semaphore		
#define OSC_Give_Semaphore				OS_Release_Semaphore
#define OSC_Create_Mutex				OS_Create_Mutex
#define OSC_Delete_Mutex				OS_Delete_Mutex
#define OSC_Obtain_Mutex				OS_Obtain_Mutex
#define OSC_Release_Mutex				OS_Release_Mutex
#define OSC_Create_Queue				OS_Create_Queue
#define OSC_Delete_Queue				OS_Delete_Queue
#define OSC_Send_To_Queue				OS_Send_To_Queue
#define OSC_Receive_From_Queue			OS_Receive_From_Queue
#define OSC_Timer_Start					OS_Timer_Start
#define OSC_Timer_Cancel				OS_Timer_Cancel
#define OSC_GetTickCount				OS_GetTickCount
#define OSC_Time_Get_Ticks_per_Sec		OS_Time_Get_Ticks_per_Sec
#define OSC_HOOK_IRQ					OS_HOOK_IRQ
#define OSC_ENABLE_IRQ					OS_ENABLE_IRQ
#define OSC_DISABLE_IRQ					OS_DISABLE_IRQ
#define OSC_InvalidateCachedData		OS_InvalidateCachedData
#define OSC_FlushCachedData				OS_FlushCachedData
#define OSC_MapBusToVirtual				OS_MapBusToVirtual
#define OSC_MapVirtualToBus				OS_MapVirtualToBus
#define OSC_BusToVirtual				OS_MapBusToVirtual
#define OSC_VirtualToBus(x)				OS_MapVirtualToBus((void *)(x))

#define tClearSemaphore					OS_Clear_Semaphore

#define OSC_SEMAPHORE			OS_SEMAPHORE
#define OSC_TASK				OS_TASK
#define OSC_TASKID				OS_TASKID
#define OSC_CURRENT_TASKID		OS_CURRENT_TASKID
#define OSC_THREAD_PARAM		OS_THREAD_PARAM
#define OSC_THREAD_ENTRY		OS_THREAD_ENTRY
#define OSC_QUEUE				OS_QUEUE
#define OSC_QUEUEID				OS_QUEUEID
#define OSC_SEMAPHORE			OS_SEMAPHORE
#define OSC_MUTEX				OS_MUTEX
#define OSC_TIMER				OS_TIMER
#define OSC_TIMERID				OS_TIMERID
#define OSC_INVALID_QUEUE		OS_INVALID_QUEUE
#define OSC_MEMORY_POOL			OS_MEMORY_POOL	
#define OSC_PARTITION_POOL		OS_PARTITION_POOL
#define OSC_SUCCESS				OS_SUCCESS
#define OSC_RCV_TIMEOUT			OS_RCV_TIMEOUT
#define OSC_SEM_TIMEOUT			OS_SEM_TIMEOUT
#define OSC_SEM_TIMED_OUT		OS_SEM_TIMED_OUT
#define OSC_MSG_TIMED_OUT		OS_MSG_TIMED_OUT
#define OSC_TIMED_OUT			OS_TIMED_OUT
#define OSC_INVALID_HANDLE		OS_INVALID_HANDLE
#define OSC_OUT_OF_MEMORY		OS_OUT_OF_MEMORY
#define OSC_INVALID_OBJ			OS_INVALID_OBJ
#define OSC_INVALID_STATE		OS_INVALID_STATE
#define OSC_INVALID_PARAM		OS_INVALID_PARAM
#define OSC_FAILURE				OS_FAILURE
#define OSC_NO_SUSPEND			OS_NO_SUSPEND
#define OSC_SUSPEND				OS_SUSPEND
#define OSC_PRIORITY_LOWEST		OS_PRIORITY_LOWEST	
#define OSC_PRIORITY_LOWER		OS_PRIORITY_LOWER
#define OSC_PRIORITY_LOW		OS_PRIORITY_LOW
#define OSC_PRIORITY_NORMAL		OS_PRIORITY_NORMAL	
#define OSC_PRIORITY_HIGH		OS_PRIORITY_HIGH
#define OSC_PRIORITY_HIGHER		OS_PRIORITY_HIGHER
#define OSC_PRIORITY_HIGHEST	OS_PRIORITY_HIGHEST
#define OSC_PRIORITY_BUMP_LOWER OS_PRIORITY_BUMP_LOWER
#define OSC_PRIORITY_BUMP_HIGHER OS_PRIORITY_BUMP_HIGHER
#define OSC_NO_PREEMPT			OS_NO_PREEMPT
#define OSC_PREEMPT				OS_PREEMPT	
#define OSC_NO_TIMESLICE		OS_NO_TIMESLICE
#define OSC_START				OS_START
#define OSC_NO_START			OS_NO_START
#define OSC_FIFO				OS_FIFO_QUEUE
#define OSC_PRIORITY			OS_PRIORITY_QUEUE
#define OSC_FIXED_SIZE			OS_BLOCKING_QUEUE
#define OSC_VARIABLE_SIZE		OS_BLOCKING_QUEUE
#define OSC_DSP_TASK			OS_DSP_TASK
#define OSC_XY_TASK				OS_XY_TASK	

#undef TIMER_ELAPSED_TIME_MODE
#define TIMER_ELAPSED_TIME_MODE	OS_TIMER_ELAPSED_TIME_MODE

#endif //MOSI_NO_MACRO_API

#ifdef RTOS_ITRON
#define OSC_DEFAULT_STACK_SIZE		4096
#else
#define OSC_DEFAULT_STACK_SIZE		2048
#endif

#define OSC_DEFAULT_QUEUE_SIZE		20

#ifndef RTOS_LINUX
#define CREATE_INSTANCE(Class, Instance) Class Instance
#define CREATE_INSTANCE_1(Class, Instance, Param1) Class Instance(Param1)
#define CREATE_INSTANCE_2(Class, Instance, Param1, Param2) Class Instance(Param1, Param2)
#define CREATE_INSTANCE_3(Class, Instance, Param1, Param2, Param3) Class Instance(Param1, Param2, Param3)
#define CREATE_INSTANCE_4(Class, Instance, Param1, Param2, Param3, Param4) Class Instance(Param1, Param2, Param3, Param4)
#define DECLARE_INSTANCE(Class, Instance) extern Class Instance
#define GET_PINSTANCE(Instance)	(&Instance)
#endif

#ifndef FILEHANDLE
#define FILEHANDLE	void *
#endif

#if MFS_SUPPORT
#define fopen _io_fopen
#define fclose _io_fclose
#define fseek _io_fseek
#define fread(ptr,so,no,f) (_io_read(f,ptr,(so)*(no))/(so))
#define fwrite(ptr,so,no,f) (_io_write(f,ptr,(so)*(no))/(so))
#endif

#define OSC_Enter_Critical_Section(mutex)			VERIFYOS(OSC_Obtain_Mutex(mutex, OSC_SUSPEND))
#define OSC_Leave_Critical_Section(mutex)			VERIFYOS(OSC_Release_Mutex(mutex))

extern OSC_MEMORY_POOL		byte_pool_0;
extern OSC_PARTITION_POOL	block_pool_0;

#if DIAMONDBACK
// Uncached memory pool. It is needed for EMAC and may be useful for some other peripherals
extern OSC_MEMORY_POOL		uncached_pool_0;
#endif

typedef struct tagIdleCount
{
	UINT32 idle_count[4];
} IDLE_COUNT_T;

void OSC_Get_Idle_Count(IDLE_COUNT_T *pcount, BOOL clear_count);

typedef struct tagMemCheck
{
	UINT32 kernel_size;
	UINT32 highest_kernel;
	UINT32 pool_size;
	UINT32 highest_pool;
	UINT32 used_buffer;
	UINT32 buffer_size;
	UINT32 highest_buffer;
} MEM_CHECK, *PMEM_CHECK;

int OSC_Memory_Check(PMEM_CHECK pMemCheck);

#if !MOSI_NO_MACRO_API
//extern int tClearSemaphore(OSC_SEMAPHORE *pSem);
#define tClearSemaphore					OSC_Clear_Semaphore
int OSC_Clear_Semaphore(OSC_SEMAPHORE *pSem);
#endif

extern void * memalloc(size_t amount);
extern void memfree(void *pMem);

#if !defined(USE_KERNEL_LOGGING) || !USE_KERNEL_LOGGING
#define _KLOGE2(fn,p1)
#define _KLOGX2(fn,p1)
#endif

#endif		// if 0		THY

#endif	// _MOSI_H
