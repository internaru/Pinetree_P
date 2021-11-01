#ifndef __MOSI_LINUX_H__
#define __MOSI_LINUX_H__

#include "tx_api.h"
/******************************************************************************
*  1. ENUMERATIONS
******************************************************************************/

/******************************************************************************
*  2. DEFINES
******************************************************************************/
#define OSC_TASK		int
#define OSC_TASKID		UINT32
#define OSC_QUEUE		OSPL_QUEUE
#define OSC_SEMAPHORE	OSPL_SEMAPHORE
#define TX_SEMAPHORE	OSPL_SEMAPHORE
#define OSC_MUTEX		OSPL_MUTEX
#define OSC_TIMERID		int*
#define OSC_TIMER		OSPL_TIMER

#define OSC_MEMORY_POOL		int
#define OSC_PARTITION_POOL	int

#define SECONDS(x)		((x) * 1000)
#define MILLISECONDS(x)	(x)

//#define OSC_SUCCESS				(0)
#define OSC_FAILURE				(-1)
//#define OSC_NO_SUSPEND			(-1)
//#define OSC_SUSPEND				(0)
#define OSC_SUCCESS				TX_SUCCESS
#define OSC_NO_SUSPEND			OSPL_NO_WAIT
#define OSC_SUSPEND				TX_WAIT_FOREVER
#define	OSC_SEM_TIMEOUT			(-2)
#define	OSC_RCV_TIMEOUT			(-3)

#define OSC_NO_PREEMPT			(0)
#define OSC_PREEMPT				(1)		
#define OSC_NO_TIMESLICE		(0)
#define OSC_START				(0)
#define OSC_NO_START			(0)
#define OSC_FIFO				(0)
#define OSC_PRIORITY			(1)
#define OSC_FIXED_SIZE			(0)
#define OSC_VARIABLE_SIZE		(1)

#define OSC_PRIORITY_LOWEST		(31)
#define OSC_PRIORITY_LOW		(22)
#define OSC_PRIORITY_NORMAL		(15)
#define OSC_PRIORITY_HIGH		(7)
#define OSC_PRIORITY_HIGHEST	(0)

#define OSC_DSP_TASK			(0)
#define OSC_XY_TASK				(0)

#define OSC_Sleep(x)		ospl_thread_sleep(x)
#define OSC_Sleep_Task(x)	ospl_thread_sleep(x)
#define OSRUN_Sleep(x)		ospl_thread_sleep(x)	

#define OSRUN_DebugString dc_log_printf
#define ORSUN_Dpf		dc_log_printf		

#define OSC_Create_Mutex(mutex, name, suspend)				ospl_mutex_create(mutex, name, TX_NO_INHERIT)
#define OSC_Obtain_Mutex(mutex, block)						ospl_mutex_get(mutex, block)
#define OSC_Release_Mutex(mutex)							ospl_mutex_put(mutex)

#define OSC_Create_Semaphore(sem, name, init, suspend)		ospl_semaphore_create(sem, name, init)
#define OSC_Take_Semaphore(sem, block)						ospl_semaphore_get(sem, block)
#define OSC_Give_Semaphore(sem)								ospl_semaphore_put(sem)
#define OSC_Delete_Semaphore(sem)							ospl_semaphore_delete(sem)


#define OSC_Release_Semaphore(sem)					ospl_semaphore_put(sem)
#define OSC_Obtain_Semaphore(sem, block)			ospl_semaphore_get(sem, block)

#define OSC_Send_To_Queue(qid, msg, size, suspend) ospl_queue_send((TX_QUEUE *)qid, msg, suspend)
#define OSC_Receive_From_Queue(qid, msg, sizeIn, sizeOut, suspend) ospl_queue_receive((TX_QUEUE *)qid, msg, suspend)

#define OSC_GetTickCount(iIndex)	(0)

#define OSC_DISABLE_IRQ()					
#define OSC_ENABLE_IRQ()

#define TIMER_ELAPSED_TIME_MODE	(0)

/******************************************************************************
*  3. STRUCTURES/TYPEDEF
******************************************************************************/
typedef void*			pointer;

extern OSC_TIMERID OSC_Timer_Start(void (*notification_function_ptr)(ULONG), void *fax_timer, UINT32 mode, UINT32 milliseconds);
extern void OSC_Timer_Cancel(void *fax_timer);

extern int OSC_Allocate_Zero_Memory(OSC_MEMORY_POOL *pool, void **memptr, UINT32 size, int suspend);
extern int OSC_Allocate_Memory(OSC_MEMORY_POOL *pool, void **memptr, UINT32 size, int suspend);
extern void OSC_Deallocate_Memory(void *memptr);
extern int OSC_Create_Fax_Queue(OSC_QUEUE *queue, char *name, UINT16 num, int mtype, UINT16 msize, int suspend);


//=============================================================================
#if 0		// THY 6/24/09
#define RTOS_STR	"Linux"
#define MFS_SUPPORT (0)


#ifdef __cplusplus
extern "C" {
#endif

#undef TRACE
#undef TRACE_RELEASE
#undef TRACE_ALWAYS 

#if defined(__KERNEL__)
int my_printf(const char *s, ...);
int my_blank_printf(const char *s, ...);
int printk(const char *s, ...);

//*******************************************************************************************
///////////////////////////SCANNER RELATED TRACE MACRO  DEFINES//////////////////////////////
///////////////////////////SCANNER RELATED TRACE MACRO  DEFINES//////////////////////////////
#ifndef SCAN_TRACE
 #define  SCAN_TRACE	TRACE			//for Scanner driver and Scan JobNode, and Scanner Service
#endif

#ifndef SCAN_TRACE_ALWAYS
 #define  SCAN_TRACE_ALWAYS	printk	//my_printf	//for importants msgs from scanner driver, scan jobnode and scanner service
#endif
///////////////////////END SCANNER RELATED TRACE MACRO  DEFINES//////////////////////////////
///////////////////////END SCANNER RELATED TRACE MACRO  DEFINES//////////////////////////////
//*******************************************************************************************



//*******************************************************************************************
///////////////////////////////FAX RELATED TRACE MACRO  DEFINES//////////////////////////////
///////////////////////////////FAX RELATED TRACE MACRO  DEFINES//////////////////////////////
#define FAX_TRACE	printk	//my_printf		// enable debug printing only on fax
//#define FAX_TRACE	my_blank_printf		// enable debug printing only on fax

#ifdef FAX_TRACE
#define TRACE	my_blank_printf		// no debug msg printed	 
//#define TRACE	printk				// debug msg print using printk	 
#else
#define TRACE my_printf
#define FAX_TRACE	my_printf
//#define TRACE printk
#endif
//////////////////////////END FAX RELATED TRACE MACRO  DEFINES//////////////////////////////
//////////////////////////END FAX RELATED TRACE MACRO  DEFINES//////////////////////////////
//*******************************************************************************************

//*******************************************************************************************
///////////////////////////////PRINT PATH RELATED TRACE MACRO  DEFINES//////////////////////////////
///////////////////////////////PRINT PATH RELATED TRACE MACRO  DEFINES//////////////////////////////
#ifndef PRT_TRACE
 #define  PRT_TRACE	TRACE			//for Printer driver and Print JobNode, and Print Service
#endif

#ifndef PRT_TRACE_ALWAYS
 #define  PRT_TRACE_ALWAYS	my_printf	//for importants msgs from Printer driver, Print jobnode and Print service
#endif
//////////////////////////END PRINT PATH RELATED TRACE MACRO  DEFINES//////////////////////////////
//////////////////////////END PRINT PATH RELATED TRACE MACRO  DEFINES//////////////////////////////
//*******************************************************************************************

//*******************************************************************************************
///////////////////////////////MOTOR RELATED TRACE MACRO  DEFINES//////////////////////////////
///////////////////////////////MOTOR PATH RELATED TRACE MACRO  DEFINES//////////////////////////////
#ifndef MOTOR_TRACE
 #define	MOTOR_TRACE	TRACE			//for Motor driver and Motor IC Driver
#endif

#ifndef	MOTOR_TRACE_ALWAYS
 #define	MOTOR_TRACE_ALWAYS	my_printf	//for importants msgs from Motor driver and Motor IC Driver
#endif
//////////////////////////END MOTOR RELATED TRACE MACRO  DEFINES//////////////////////////////
//////////////////////////END MOTOR RELATED TRACE MACRO  DEFINES//////////////////////////////
//*******************************************************************************************


extern int snprintf(char * buf, size_t size, const char * fmt, ...);
extern long simple_strtol(const char*, char**, unsigned int);

#else
#include <stdio.h>
#define TRACE printf
#endif


#define EBDAF		9

extern int OSC_Create_Memory_Pool(OSC_MEMORY_POOL *pool, char *name, void * start, UINT32 size, UINT32 min, int suspend);
extern int OSC_Allocate_Memory(OSC_MEMORY_POOL *pool, void **memptr, UINT32 size, int suspend);
extern int OSC_Allocate_Zero_Memory(OSC_MEMORY_POOL *pool, void **memptr, UINT32 size, int suspend);
void* OSC_Allocate_Pages (unsigned siz);
extern int OSC_Deallocate_Memory(void *memptr);	
extern UINT32 OSC_Memory_Get_Size(void* memptr);
extern void* OSC_Allocate(int size);
extern void OSC_Free(void* ptr);

// non-cached allocations
extern void* OSC_Allocate_NC_Memory(unsigned int* dma_addr, unsigned int size);
extern int OSC_Free_NC_Memory(void* virt_address, unsigned int dma_addr, unsigned int size);
extern unsigned int OSC_Map_Memory(void* virt_address, unsigned int size, int to_device);
extern void OSC_Unmap_Memory(unsigned int phys_address, unsigned int size, int to_device);

#define OSC_Allocate_Stack_Memory	OSC_Allocate_Memory
#define OSC_Allocate_Queue_Memory	OSC_Allocate_Memory

extern int OSC_Create_Partition_Pool(OSC_PARTITION_POOL *pool, char *name, void *start, UINT32 size, UINT32 unit, int suspend);
extern int OSC_Allocate_Partition(OSC_PARTITION_POOL *pool, void **memptr, int block);
extern int OSC_Deallocate_Partition(OSC_PARTITION_POOL *pool, void *memptr);
extern int OSC_Get_Partition_Block_Size(OSC_PARTITION_POOL pool);

extern int OSC_Partition_Get_Total_Blocks(OSC_PARTITION_POOL pool);
extern int OSC_Partition_Get_Free_Blocks(OSC_PARTITION_POOL pool);
extern int OSC_Get_Partition_Block_Size(OSC_PARTITION_POOL pool);
extern int OSC_Partition_Get_Max_Used_Blocks(OSC_PARTITION_POOL pool);

extern int OSC_Create_Semaphore(OSC_SEMAPHORE *sem, char *name, int init, int suspend);
extern int OSC_Delete_Semaphore(OSC_SEMAPHORE *sem);
extern int OSC_Obtain_Semaphore(OSC_SEMAPHORE *sem, int block);
extern int OSC_Release_Semaphore(OSC_SEMAPHORE *sem);	
#define OSC_Take_Semaphore			OSC_Obtain_Semaphore		
#define OSC_Give_Semaphore			OSC_Release_Semaphore
extern int OSC_Create_Mutex(OSC_MUTEX *mutex, char *name, int suspend);
#define OSC_Obtain_Mutex(mutex, block)					OSC_Obtain_Semaphore(mutex, block)
#define OSC_Release_Mutex(mutex)						OSC_Release_Semaphore(mutex)
extern int tClearSemaphore(OSC_SEMAPHORE *pSem);

#define OSC_THREAD_PARAM	void*
typedef void (* OSC_THREAD_ENTRY)(OSC_THREAD_PARAM);

extern int OSC_Create_Task(OSC_TASK *thread, char *name, OSC_THREAD_ENTRY entry, UINT32 data, UINT32 ssize, int priority, UINT32 slice, int preempt, int autostart);
#define OSC_Delete_Current_Task()							return

extern int OSC_Block_Task(OSC_TASKID tid);
extern int OSC_Ready_Task(OSC_TASKID tid);									
extern int OSC_Relinquish_Task(void);
extern int OSC_Current_Task_Pointer(void);
extern int OSC_Destroy_task(OSC_TASKID tskid);

#define OSC_CURRENT_TASKID							OSC_Current_Task_Pointer()
#define OSC_Change_Priority(thread, priority)		(OSC_SUCCESS)
#define OSC_Id_Task(task)							(task)
#define OSC_Task_Resume								OSC_Ready_Task
#define OSC_Resume_Task								OSC_Ready_Task
#define OSC_Suspend_Task							OSC_Block_Task				

#define OSC_QUEUE_SIZE(num, size)	((num)*(size))

extern int OSC_Send_To_Queue(OSC_QUEUE *qid, void *msg, UINT32 size, int suspend);
extern int OSC_Receive_From_Queue(OSC_QUEUE *qid, void *msg, UINT32 sizeIn, UINT32 *sizeOut, int suspend);
						
extern int OSC_Sleep(int delay);
extern void OSC_Delay(int usdelay);

#define OSC_Sleep_Task(delay) OSC_Sleep(delay)

extern int OSC_Suspend_Scheduling(void);
extern int OSC_Resume_Scheduling(void);

#define OSC_Current_Task_Id			OSC_Current_Task_Pointer
#define OSC_Is_Current_Task(task)							(task == OSC_Current_Task_Id())

#define OSC_MSG_TIMED_OUT(x)	((x) == OSC_RCV_TIMEOUT)
#define OSC_SEM_TIMED_OUT(x)	((x) == OSC_SEM_TIMEOUT)
#define OSC_TIMED_OUT(x)		(OSC_MSG_TIMED_OUT(x) || OSC_SEM_TIMED_OUT(x))

extern UINT32 OSC_GetTickCount(int iIndex);
#define OSC_Time_Get_Ticks_per_Sec	(100)

typedef void (*TIMERCALLBACK)( OSC_TIMERID, void *, UINT32, UINT32);

#define TIMER_ELAPSED_TIME_MODE	(0)
extern OSC_TIMERID OSC_Timer_Start(TIMERCALLBACK, void *, UINT32, UINT32);
extern void OSC_Timer_Cancel(OSC_TIMERID);

extern char *GetRtosErrorText(int errCode);
extern char *CreateName(char *acPart1, char *acPart2);

#define DEV_WRITE_UINT8(DevAddr, Value)		OSC_DEV_WRITE_UINT8((volatile void*)(DevAddr), Value)	
#define DEV_READ_UINT8(DevAddr)				OSC_DEV_READ_UINT8((volatile void*)(DevAddr))			
#define DEV_WRITE_UINT16(DevAddr, Value)	OSC_DEV_WRITE_UINT16((volatile void*)(DevAddr), Value)
#define DEV_READ_UINT16(DevAddr)			OSC_DEV_READ_UINT16((volatile void*)(DevAddr))		
#define DEV_WRITE_UINT32(DevAddr, Value)	OSC_DEV_WRITE_UINT32((volatile void*)(DevAddr), Value)
#define DEV_READ_UINT32(DevAddr)			OSC_DEV_READ_UINT32((volatile void*)(DevAddr))		

extern void* memalloc(size_t size);
extern void memfree(void* p);
#if defined(__KERNEL__)
extern void * memcpy(void *d, const void *s, size_t count);
extern int memcmp(const void *d, const void *s, size_t count);

extern void OSC_ENABLE_IRQ(void);
extern void OSC_DISABLE_IRQ(void);
extern int OSC_HOOK_IRQ(int irq, IRQCALLBACK pfn, void * data);

extern void OSC_InvalidateCachedData(void *p, UINT32 size);
extern void OSC_FlushCachedData(void *p, UINT32 size);

#if 0
#define OSC_MemoryBarrier(DevAddr) __asm__ __volatile__ ("MCR p15, 0, %0, c7, c10, 4" \
                                   : : "r" (DevAddr) : "memory")
#else
#define OSC_MemoryBarrier(DevAddr)
#endif


static inline void OSC_DEV_WRITE_UINT8(volatile void* DevAddr, UINT8 Value)
{
	*(volatile UINT8 *)(DevAddr) = (Value); 
	OSC_MemoryBarrier(DevAddr);
}

static inline UINT8 OSC_DEV_READ_UINT8(volatile void* DevAddr)
{
	OSC_MemoryBarrier(DevAddr);
	return *(volatile UINT8 *)(DevAddr);
}

static inline void OSC_DEV_WRITE_UINT16(volatile void* DevAddr, UINT16 Value)
{
	*(volatile UINT16 *)(DevAddr) = (Value); 
	OSC_MemoryBarrier(DevAddr);
}

static inline UINT16 OSC_DEV_READ_UINT16(volatile void* DevAddr)
{
	OSC_MemoryBarrier(DevAddr);
	return *(volatile UINT16 *)(DevAddr);
}

static inline void OSC_DEV_WRITE_UINT32(volatile void* DevAddr, UINT32 Value)
{
	*(volatile UINT32 *)(DevAddr) = (Value); 
	OSC_MemoryBarrier(DevAddr);
}

static inline UINT32 OSC_DEV_READ_UINT32(volatile void* DevAddr)
{
	OSC_MemoryBarrier(DevAddr);
	return *(volatile UINT32 *)(DevAddr);
}

UINT32 OSC_MapBusToVirtual(void* BusAddress);
UINT32 OSC_MapVirtualToBus(void* VirtualAddress);

#define OSC_BusToVirtual(a)			OSC_MapBusToVirtual((void*)(a))
#define OSC_PhysicalToVirtual(a)	OSC_MapBusToVirtual((void*)(a))
#define OSC_VirtualToBus(a)			OSC_MapVirtualToBus((void*)(a))
#define OSC_VirtualToPhysical(a)	OSC_MapVirtualToBus((void*)(a))

// For debugging purposes to reduce call depth
// #include <asm/bug.h>
// extern void BUG(void);
// #define OSC_Breakpoint BUG
//#include <asm/atomic.h>
#define OSC_ATOMIC int*
extern void OSC_Atomic_Init(OSC_ATOMIC* a, int value);
extern void OSC_Atomic_Inc(OSC_ATOMIC* a);
extern void OSC_Atomic_Dec(OSC_ATOMIC* a);
extern OSC_ATOMIC OSC_Atomic_Read(OSC_ATOMIC* a);

#endif

#define TIMESTAMP	(OSC_GetTickCount(0))
#define SHORT_TS	(TIMESTAMP&0xffff)
extern void OSC_Breakpoint(void);
#undef BKPT
#define BKPT	OSC_Breakpoint()

#define DECLARE_INSTANCE(Class, Instance) extern Class *Instance
#define GET_PINSTANCE(Instance)	(Instance)
#define CREATE_INSTANCE(Class, Instance) \
	Class *Instance;\
	extern  "C" int CreateInstance_##Class##Instance(void) \
	{ \
		Instance = new Class; \
		return (int)Instance; \
	}
#define CREATE_INSTANCE_1(Class, Instance, Param1) \
	Class *Instance;\
	extern  "C" int CreateInstance_##Class##Instance(void) \
	{ \
		Instance = new Class(Param1); \
		return (int)Instance; \
	}

#define CREATE_INSTANCE_2(Class, Instance, Param1, Param2) \
	Class *Instance;\
	extern  "C" int CreateInstance_##Class##Instance(void) \
	{ \
		Instance = new Class(Param1, Param2); \
		return (int)Instance; \
	}

#define CREATE_INSTANCE_3(Class, Instance, Param1, Param2, Param3) \
	Class *Instance;\
	extern  "C" int CreateInstance_##Class##Instance(void) \
	{ \
		Instance = new Class(Param1, Param2, Param3); \
		return (int)Instance; \
	}

#define CREATE_INSTANCE_4(Class, Instance, Param1, Param2, Param3, Param4) \
	Class *Instance;\
	extern  "C" int CreateInstance_##Class##Instance(void) \
	{ \
		Instance = new Class(Param1, Param2, Param3, Param4); \
		return (int)Instance; \
	}

#ifdef __cplusplus
}
#endif
#endif 		// if 0 	// THY 6/24/09
#endif
