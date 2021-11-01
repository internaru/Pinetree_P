/*
thread.h
*/

#ifndef __THREAD_H__
#define __THREAD_H__

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

#define USE_THREAD_LIST     (0)
#define ENABLE_PAGEFAULT_TRACING     (0)
#define USE_THREAD_LIST     (0)
#define USE_PTHREAD_CANCEL     (1)

#define OSPL_NAME_MAX_LEN	(32)

#define DECLARE_SIGNATURE				unsigned int signature

#define CHECK_SIGNATURE(s, sign, retcode)		if (s->signature != sign) \
										{ \
											/* printf("%s: invalid signature in %p: %#x instead of %#x\n", \
												__func__, s, s->signature, sign); */ \
												return retcode; \
										}
										
#define container_of(list_field, struct_type, ptr) \
       (struct_type*)((void*)ptr-(void*)(&((struct_type*)0)->list_field))

#define is_before(a, b) (((long)b - (long)a) > 0)

#define INIT_SIGNATURE(s, sign)			s->signature = sign

#define	TH_SIGNATURE		0xAABADBAD

#define	MX_SIGNATURE	0xCCBADBAD

#define SEM_SIGNATURE	0xBBBADBAD

#define EF_SIGNATURE	0xEFBADBAD

#define configure_malloc_behavior(...); // dcapi.h

#define HZ		1000
#define MSEC_PER_TICK	(1000/HZ)
#define USEC_PER_TICK	(MSEC_PER_TICK*1000)

#define TICKS_PER_SECOND	(1000000UL/USEC_PER_TICK)

#define TIMER_SIGNAL		SIGUSR1
#define TIMER_RESOLUTION	10

#define RELEASE_STATIC 

#define MAX_MSG_QUEUE_NAME_LEN     (64)


struct osplu_msg_hdr_s;

typedef void* (*pthread_start_routine)(void*);

typedef struct list_head_s
{
	struct list_head_s* next;
	struct list_head_s* prev;
} list_head_t;


typedef struct osplu_thread_s
{
	pthread_t				thread;
    char					name[OSPL_NAME_MAX_LEN + 1];
	unsigned int			priority;
	pthread_start_routine	func;
	ULONG					arg;
	OSPL_THREAD*			cb;
	pid_t					pid;
	struct osplu_thread_s*	next;		// linked list to support ospl_thread_identify
	DECLARE_SIGNATURE;
} osplu_thread_t;


typedef struct osplu_mutex_s
{
	pthread_mutex_t		mutex;
    char				name[OSPL_NAME_MAX_LEN + 1];
	pthread_mutexattr_t	attr;
	unsigned int		inherit;
	int					deleting;
	DECLARE_SIGNATURE;
} osplu_mutex_t;


typedef struct osplu_semaphore_s
{
	pthread_mutex_t		mutex;
    char				name[OSPL_NAME_MAX_LEN + 1];
	pthread_cond_t		condition;
	unsigned long		count;
	int					deleting;
	int					refcount;
	DECLARE_SIGNATURE;
} osplu_semaphore_t;


typedef struct osplu_event_flags_s
{
	pthread_mutex_t		mutex;
	pthread_cond_t		condition;
	unsigned long		flags;
	char*				name;
	int					deleting;
	int					refcount;
	DECLARE_SIGNATURE;
} osplu_event_flags_t;


typedef struct osplu_msg_hdr_s
{
	struct list_head_s		list;
	unsigned int			size;
	void*					payload;
} osplu_msg_hdr_t;

#if 0
typedef struct osplu_msg_queue_s
{
	pthread_mutex_t			mutex;
	pthread_cond_t			condition;
	void					*messages;		// message storage
	struct osplu_msg_hdr_s	*headers;		// header storage
	struct list_head_s		free;
	struct list_head_s		active;	
	unsigned int			nr;				// current number of messages in the queue
	int						mnum;			// max number of messages in the queue
	int						running;
    char					name[OSPL_NAME_MAX_LEN + 1];
	DECLARE_SIGNATURE;
} osplu_msg_queue_t;
#else
typedef struct osplu_msg_queue_s
{
	struct mq_attr			attributes;
	char					name[MAX_MSG_QUEUE_NAME_LEN];
	unsigned int			msg_size;		// max size of the message
	unsigned int			msg_num;		// max number of messages in the queue
	pthread_key_t			key;
} osplu_msg_queue_t;
#endif


typedef struct ospl_timer_s
{
	struct list_head_s	list;
    char				name[OSPL_NAME_MAX_LEN + 1];
	void				(*expiration_function)(ULONG);
	ULONG				expiration_input;
	ULONG				initial_ticks;
	ULONG				reschedule_ticks;
	ULONG				current_ticks;
	int					active;
	DECLARE_SIGNATURE;
} ospl_timer_t;


typedef struct ospl_timers_s
{
	struct list_head_s  list_head;			// head of timer list 
	pthread_mutex_t		mutex;				// access mutex
	timer_t				id;					// our one and only timer
	struct sigevent		timer_event_spec;	// signal event parameters
	struct itimerspec	timer_spec;			// timer parameters
	struct sigaction	oldact;				// old signal action 
	ULONG				overruns;			// current number of timer overruns, used for adjustment
	unsigned int		num;				// number of created timers
} ospl_timers_t;

#endif // #ifndef __THREAD_H_


