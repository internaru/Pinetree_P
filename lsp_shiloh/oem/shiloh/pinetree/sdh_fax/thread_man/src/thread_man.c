/*
thread_man.c
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include "cxshell.h"
#include "delay_api.h"
#include "pthread.h"
#include "posix_ostools.h"
#include "ospl.h"
#include "thread_man.h"
#include "SDHFaxCommonHeader.h"


RELEASE_STATIC ospl_timers_t ospl_timers;
RELEASE_STATIC pthread_once_t osplu_timer_once_control = PTHREAD_ONCE_INIT;

static void _ospl_semaphore_cleanup(void* arg);
static void _ospl_calc_timeout_value(ULONG ticks, struct timespec* ts, int relative);
static void _ospl_ticks_to_timeval(ULONG ticks, struct timeval* tv);
static void _ospl_event_flags_cleanup(void* arg);
static int _ospl_check_flags(osplu_event_flags_t *ef, ULONG requested_flags, UINT get_option);
static ULONG _ospl_ticks_2_usec(ULONG ticks);
void _osplu_list_init(struct list_head_s *list);
void _osplu_list_add_tail(struct list_head_s *list, struct list_head_s *elem);
void _osplu_dump_list(struct list_head_s *list, char* name);
int _osplu_is_list_empty(struct list_head_s *list);
void _osplu_list_remove(struct list_head_s* elem);

RELEASE_STATIC void _osplu_del_timer(ospl_timer_t* timer);
RELEASE_STATIC void _osplu_initialize_timers(void);
RELEASE_STATIC void _osplu_timer_notify_function(int signo);
RELEASE_STATIC void _osplu_add_timer(ospl_timer_t* timer);


ULONG ospl_time_get(void)
{
    struct timespec monotime;
    unsigned long mstime = 0;

    clock_gettime(CLOCK_MONOTONIC, &monotime);
    mstime = ((unsigned long)monotime.tv_sec)*TICKS_PER_SECOND + (unsigned long)monotime.tv_nsec/USEC_PER_TICK/USEC_PER_TICK;

    return (ULONG)mstime;
}


UINT ospl_event_flags_create(OSPL_EVENT_FLAGS_GROUP *group_ptr, CHAR *name_ptr)
{
	osplu_event_flags_t *ef;

    //TRACE_THREAD_MAN( "%s(%s)\n", __func__, name_ptr);
	ef = ospl_zalloc(sizeof(osplu_event_flags_t), 0, 0);

	if (!ef)
    {
        ospl_free(ef);
		return OSPL_NO_MEMORY;
    }

	ef->name = name_ptr;
	ef->flags = 0;
	ef->deleting = 0;
	ef->refcount = 0;

	pthread_mutex_init(&ef->mutex, NULL);
	pthread_cond_init(&ef->condition, NULL);

	*group_ptr = (OSPL_EVENT_FLAGS_GROUP)ef;

	INIT_SIGNATURE(ef, EF_SIGNATURE);
    //TRACE_THREAD_MAN("Created user event flags '%s' at %p\n", name_ptr ? name_ptr : "", ef);
	return OSPL_SUCCESS;
}


UINT ospl_event_flags_get(OSPL_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags, UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option)
{
	osplu_event_flags_t *ef = (osplu_event_flags_t*)*group_ptr;
	int retval = 0, retcode = OSPL_SUCCESS;

	CHECK_SIGNATURE(ef, EF_SIGNATURE, OSPL_GROUP_ERROR);

	pthread_mutex_lock(&ef->mutex);
	if (ef->deleting)
	{
		pthread_mutex_unlock(&ef->mutex);
		return OSPL_DELETED;
	}
	ef->refcount++;
	// waiting on a condition is a cancellation point, prepare cleanup handler
	pthread_cleanup_push(_ospl_event_flags_cleanup, ef);
	switch (wait_option)
	{
	case OSPL_NO_WAIT:
		if (!_ospl_check_flags(ef, requested_flags, get_option))
			retcode = OSPL_NO_EVENTS;
		break;
	case OSPL_WAIT_FOREVER:
		while (!_ospl_check_flags(ef, requested_flags, get_option))
		{
			retval = pthread_cond_wait(&ef->condition, &ef->mutex);
		}
		if (retval)
			retcode = OSPL_WAIT_ABORTED;
		break;
	default:
		{
			struct timespec ts;
			_ospl_calc_timeout_value(wait_option, &ts, 1);
			while (!_ospl_check_flags(ef, requested_flags, get_option))
			{
				retval = pthread_cond_timedwait(&ef->condition, &ef->mutex, &ts);
				if (retval) // timeout or any other type of error condition
				{
					// TODO - differentiate error codes
					retcode = OSPL_NO_EVENTS;
					break;
				}
			}
		}
		break;
	}

	if (OSPL_SUCCESS == retcode)
	{
		*actual_flags_ptr = ef->flags;
		if ((get_option == OSPL_AND_CLEAR) || (get_option == OSPL_OR_CLEAR))
		{
			ef->flags &= ~requested_flags;
		}
	}

	if  (ef->deleting)
	{
		retcode = OSPL_DELETED;
	}

	pthread_cleanup_pop(1); // will release the mutex and decrement refcount
	return retcode;
}


UINT ospl_event_flags_set(OSPL_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set, UINT set_option)
{
	osplu_event_flags_t *ef = (osplu_event_flags_t*)*group_ptr;

	CHECK_SIGNATURE(ef, EF_SIGNATURE, OSPL_GROUP_ERROR);

	pthread_mutex_lock(&ef->mutex);
	if (ef->deleting)
	{
		pthread_mutex_unlock(&ef->mutex);
		return OSPL_DELETED;
	}
	if (set_option == OSPL_AND)
	{
		ef->flags &= flags_to_set;
	}
	else if (set_option == OSPL_OR)
	{
		ef->flags |= flags_to_set;
	}
	else
	{
		pthread_mutex_unlock(&ef->mutex);
		return OSPL_OPTION_ERROR;
	}

	pthread_cond_broadcast(&ef->condition);

	pthread_mutex_unlock(&ef->mutex);

	return OSPL_SUCCESS;
}


UINT ospl_semaphore_create(OSPL_SEMAPHORE *semaphore_ptr, CHAR *name_ptr, ULONG initial_count)
{
	osplu_semaphore_t* sem;

    //TRACE_THREAD_MAN( "%s(%s)\n", __func__, name_ptr);
	sem = ospl_zalloc(sizeof(osplu_semaphore_t), 0, 0);
	if (!sem)
	{
		ospl_free(sem);
		return OSPL_NO_MEMORY;
    }

	if (name_ptr)
    {
        snprintf(sem->name, OSPL_NAME_MAX_LEN, "%s", name_ptr);
    }

	sem->count = initial_count;
	sem->deleting = 0;
	sem->refcount = 0;

	pthread_cond_init(&sem->condition, NULL);
	pthread_mutex_init(&sem->mutex, NULL);

	INIT_SIGNATURE(sem, SEM_SIGNATURE);

	*semaphore_ptr = (OSPL_SEMAPHORE)sem;
	return OSPL_SUCCESS;
}


UINT ospl_semaphore_get(OSPL_SEMAPHORE *semaphore_ptr, ULONG wait_option)
{
	osplu_semaphore_t* sem = (osplu_semaphore_t*)*semaphore_ptr;
	int retval = OSPL_SUCCESS;

	CHECK_SIGNATURE(sem, SEM_SIGNATURE, OSPL_SEMAPHORE_ERROR);

	pthread_mutex_lock(&sem->mutex);
	if (sem->deleting)
	{
		pthread_mutex_unlock(&sem->mutex);
		return OSPL_DELETED;
	}

	sem->refcount++;
	// waiting on a condition is a cancellation point, prepare cleanup handler
	pthread_cleanup_push(_ospl_semaphore_cleanup, sem);
	switch (wait_option)
	{
	case OSPL_WAIT_FOREVER:
		while (sem->count == 0 && !sem->deleting)
			pthread_cond_wait(&sem->condition, &sem->mutex);
		if (!sem->deleting)
			sem->count--;
		break;
	case OSPL_NO_WAIT:
	default:
		{
			struct timespec ts;
			_ospl_calc_timeout_value(wait_option, &ts, 1);

			if (sem->count == 0)
			{
				retval = pthread_cond_timedwait(&sem->condition, &sem->mutex, &ts);
				if (ETIMEDOUT == retval)
				{
					retval = OSPL_NO_INSTANCE;
				}
			}
			if (OSPL_SUCCESS == retval && !sem->deleting)
				sem->count--;
		}
		break;
	}

	if (sem->deleting)
	{
		retval = OSPL_DELETED;
	}

	pthread_cleanup_pop(1); // will release the mutex and decrement refcount

	return retval;
}


UINT ospl_semaphore_put(OSPL_SEMAPHORE *semaphore_ptr)
{
	osplu_semaphore_t* sem = (osplu_semaphore_t*)*semaphore_ptr;

	CHECK_SIGNATURE(sem, SEM_SIGNATURE, OSPL_SEMAPHORE_ERROR);

	if (sem->deleting)
	{
		pthread_mutex_unlock(&sem->mutex);
		return OSPL_DELETED;
	}

	pthread_mutex_lock(&sem->mutex);
	sem->refcount++;
	if (sem->count++ == 0)
		pthread_cond_signal(&sem->condition);
//		pthread_cond_broadcast(&sem->condition);
	sem->refcount--;
	pthread_mutex_unlock(&sem->mutex);

	return OSPL_SUCCESS;
}


UINT ospl_mutex_create(OSPL_MUTEX *mutex_ptr, CHAR *name_ptr, UINT inherit)
{
	osplu_mutex_t* mutex;

    //TRACE_THREAD_MAN( "%s(%s)\n", __func__, name_ptr);
	mutex = ospl_zalloc(sizeof(osplu_mutex_t), 0, 0);
	if (!mutex)
	{
		ospl_free(mutex);
		return OSPL_NO_MEMORY;
	}

	if (name_ptr)
    {
        snprintf(mutex->name, OSPL_NAME_MAX_LEN, "%s", name_ptr);
    }

	mutex->inherit = inherit;
	mutex->deleting = 0;
	pthread_mutexattr_settype(&mutex->attr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&mutex->mutex, &mutex->attr);

	INIT_SIGNATURE(mutex, MX_SIGNATURE);

	*mutex_ptr = (OSPL_MUTEX)mutex;
	return OSPL_SUCCESS;
}


UINT ospl_mutex_get(OSPL_MUTEX *mutex_ptr, ULONG wait_option)
{
	osplu_mutex_t* mutex = (osplu_mutex_t*)*mutex_ptr;
	int retval = 0;

	CHECK_SIGNATURE(mutex, MX_SIGNATURE, OSPL_MUTEX_ERROR);

	if (mutex->deleting)
	{
		return OSPL_DELETED;
	}

	switch (wait_option)
	{
	case OSPL_WAIT_FOREVER:
		retval = pthread_mutex_lock(&mutex->mutex);
		break;
	case OSPL_NO_WAIT:
		retval = pthread_mutex_trylock(&mutex->mutex);
		break;
	default:
		// with timeout
		while (wait_option--)
		{
			retval = pthread_mutex_trylock(&mutex->mutex);
			if (!retval)
				break;
#if 1 //mod.bong.2012-08-31
			usleep(1000); // sleep 1 ms
#else //mod.bong.
			delay_msec_timer(1); // sleep 1 ms
#endif //mod.bong.
		}
		break;
	}

	if (!retval)
		return OSPL_SUCCESS;

	return OSPL_NOT_AVAILABLE;
}


UINT ospl_mutex_put(OSPL_MUTEX *mutex_ptr)
{
	osplu_mutex_t* mutex = (osplu_mutex_t*)*mutex_ptr;
	int retval;

	CHECK_SIGNATURE(mutex, MX_SIGNATURE, OSPL_MUTEX_ERROR);

	if (mutex->deleting)
	{
		return OSPL_DELETED;
	}

	retval = pthread_mutex_unlock(&mutex->mutex);

	if (retval == EPERM)
		return OSPL_NOT_OWNED;

	return OSPL_SUCCESS;
}


UINT ospl_thread_delete(OSPL_THREAD *thread_ptr)
{
	int retval;
	osplu_thread_t*	t = NULL;
#if USE_THREAD_LIST
	osplu_thread_t	**iter;
	
	// remove the thread from the global list
	iter = &osplu_thread_list;
	while (*iter)
	{
		if ((*iter)->cb == thread_ptr)
		{
			t = *iter;
			break;
		}
		iter = &(*iter)->next;
	}
	if (*iter)
		*iter = (*iter)->next;
#else
	t = (osplu_thread_t*)*thread_ptr;
#endif

	if (t)
	{
		CHECK_SIGNATURE(t, TH_SIGNATURE, OSPL_THREAD_ERROR);

#if USE_PTHREAD_CANCEL
        retval = pthread_cancel(t->thread);
#else
        //should be implemented better:
        retval = pthread_kill(t->thread, SIGKILL);
#endif //USE_PTHREAD_CANCEL

		if (!retval)
		{
			retval = pthread_join(t->thread, NULL);

            //TRACE_THREAD_MAN("Destroyed user thread: name %s, entry %p, retval %d\n", t->name ? t->name : "unnamed", (void*)t->func, retval);
		}
		else
		{
            //TRACE_THREAD_MAN("Failed to cancel thread %s [%d] - already cancelled ?\n", t->name ? t->name : "unnamed", retval);
		}
		ospl_free(t);
		return OSPL_SUCCESS;
	}
	else
	{
        //TRACE_THREAD_MAN("Failed to cancel thread - unknown thread identifier\n");
	}
	return OSPL_THREAD_ERROR;
	
}


UINT ospl_thread_sleep(ULONG timer_ticks)
{
#if 1 //mod.bong.2012-08-27 2:17pm
#if 1 /* mod. for the exact time */
	UINT32 start_time = ospl_time_get();
	UINT32 stop_time;
	
//	if( (timer_ticks > 0) && ((timer_ticks % 10) == 0) ) {
//		timer_ticks = timer_ticks - 1;
//	}
	
	stop_time = start_time + timer_ticks;
	
	if( start_time > stop_time ) { /* overflow */
		usleep(timer_ticks * USEC_PER_TICK);
		return OSPL_SUCCESS;
	}

	while (is_before(start_time, stop_time))
	{
		if (!usleep((stop_time - start_time) * USEC_PER_TICK))
			break;
			
		if (errno != EINTR) // the only valid reason for us to be here is having been interrupted by a signal
			return OSPL_WAIT_ABORTED;
			
		start_time = ospl_time_get();
	}
#else
	if( timer_ticks > 1 ) {
		posix_sleep_ms((timer_ticks-1));
	}
	else {
		posix_sleep_ms(1);
	}
#endif
#else //mod.bong.
	delay_msec_timer(timer_ticks);
#endif //mod.bong.

	return OSPL_SUCCESS;
}


static void _ospl_semaphore_cleanup(void* arg)
{
	osplu_semaphore_t *sem = (osplu_semaphore_t*)arg;
	sem->refcount--;
    //TRACE_THREAD_MAN("%s: refcount %d\n", __func__, sem->refcount);
	pthread_mutex_unlock(&sem->mutex);
}


static void _ospl_calc_timeout_value(ULONG ticks, struct timespec* ts, int relative)
{
	struct timeval tv, tc = {0};
	_ospl_ticks_to_timeval(ticks, &tv);

	if (relative)
	{
		gettimeofday(&tc, NULL);
		timeradd(&tv, &tc, &tv);				// absolute time for timeout
	}
	ts->tv_sec = tv.tv_sec;
	ts->tv_nsec = tv.tv_usec * 1000;
}


static void _ospl_ticks_to_timeval(ULONG ticks, struct timeval* tv)
{
	ULONG usec_timeout = _ospl_ticks_2_usec(ticks);

	tv->tv_sec = usec_timeout/1000000;			// seconds to wait
	tv->tv_usec = (usec_timeout%1000000);		// useconds to wait
}


static ULONG _ospl_ticks_2_usec(ULONG ticks)
{
	return ticks*USEC_PER_TICK;
}


void _osplu_list_init(struct list_head_s *list)
{
	list->next = list;
	list->prev = list;
}


void _osplu_list_add_tail(struct list_head_s *list, struct list_head_s *elem)
{
	assert(list && elem);
	elem->next = list;
	elem->prev = list->prev;
	list->prev->next = elem;
	list->prev = elem;
}


void _osplu_dump_list(struct list_head_s *list, char* name)
{
    //int count = 0;
	struct list_head_s* el = list->next;
    //TRACE_THREAD_MAN("List %s (%p): next=%p prev=%p\n", name, list, list->next, list->prev);
	if (_osplu_is_list_empty(list))
	{
        //TRACE_THREAD_MAN("EMPTY\n");
	}
	else
		while (el != list) 
		{
            //TRACE_THREAD_MAN("\t[%d] this: %p next: %p prev: %p\n", count++, el, el->next, el->prev);
			el = el->next;
		}
}


int _osplu_is_list_empty(struct list_head_s *list)
{
	return list->next == list;
}


RELEASE_STATIC void _osplu_del_timer(ospl_timer_t* timer)
{
	pthread_mutex_lock(&ospl_timers.mutex);
//	_osplu_dump_list(&ospl_timers.list_head, "before timer removed from list ");
	_osplu_list_remove(&timer->list);
//	_osplu_dump_list(&ospl_timers.list_head, "after timer removed from list ");
	pthread_mutex_unlock(&ospl_timers.mutex);
}


void _osplu_list_remove(struct list_head_s* elem)
{
	elem->prev->next = elem->next;
	elem->next->prev = elem->prev;
}


RELEASE_STATIC void _osplu_initialize_timers(void)
{
	struct sigaction act;
	int retval;

    //TRACE_THREAD_MAN("%s\n", __func__);
    act.sa_handler = _osplu_timer_notify_function;
    act.sa_flags=0;

	_osplu_list_init(&ospl_timers.list_head);
	pthread_mutex_init(&ospl_timers.mutex, NULL);

	if (sigemptyset(&act.sa_mask) == -1) 
	{ 
        //TRACE_THREAD_MAN("Error calling sigemptyset\n");
		return;
	} 
	if (sigaction(TIMER_SIGNAL, &act, &ospl_timers.oldact) == -1) 
	{ 
        //TRACE_THREAD_MAN("Error calling sigaction\n");
		return;
	} 	

	_ospl_calc_timeout_value(TIMER_RESOLUTION, &ospl_timers.timer_spec.it_value, 1);
	_ospl_calc_timeout_value(TIMER_RESOLUTION, &ospl_timers.timer_spec.it_interval, 0);

	ospl_timers.timer_event_spec.sigev_notify = SIGEV_SIGNAL;
	ospl_timers.timer_event_spec.sigev_signo = TIMER_SIGNAL;
	ospl_timers.overruns = 0;

	retval = timer_create(CLOCK_REALTIME, &ospl_timers.timer_event_spec, &ospl_timers.id);

	if (retval)
	{
        //TRACE_THREAD_MAN("timer_create failed, retval=%d\n", retval);
		return;
	}

	retval = timer_settime(ospl_timers.id, TIMER_ABSTIME, &ospl_timers.timer_spec, NULL);
	if (retval)
	{
        //TRACE_THREAD_MAN("timer_settime failed, retval=%d\n", retval);
		timer_delete(ospl_timers.id);
		return;
	}

}


RELEASE_STATIC void _osplu_timer_notify_function(int signo)
{
	struct list_head_s* el, *next_el;
	UINT32 overruns, inc_ticks = 0;
	overruns = timer_getoverrun(ospl_timers.id);
	if (overruns > ospl_timers.overruns)
	{
		inc_ticks = overruns - ospl_timers.overruns;
        //TRACE_THREAD_MAN("overruns: %lu => %u, inc=%u\n", ospl_timers.overruns, overruns, inc_ticks);
	}
	ospl_timers.overruns = overruns;
	pthread_mutex_lock(&ospl_timers.mutex);
	el = ospl_timers.list_head.next;
	while (el != &ospl_timers.list_head)
	{
		ospl_timer_t* timer = container_of(list, ospl_timer_t, el);
		next_el = el->next;
		if (timer->active)
		{
			int shoot = 0;
			if (timer->current_ticks > 0)
			{
				timer->current_ticks -= (TIMER_RESOLUTION + inc_ticks);
				if (timer->current_ticks <= 0) 
				{
					timer->current_ticks = timer->reschedule_ticks;
					shoot = 1;
				}
			}
			if (shoot && timer->expiration_function)
			{
				// release the lock so that callback function could schedule new timer too
				pthread_mutex_unlock(&ospl_timers.mutex);
				timer->expiration_function(timer->expiration_input);
				pthread_mutex_lock(&ospl_timers.mutex);
			}
		}
		el = next_el;
	}

	pthread_mutex_unlock(&ospl_timers.mutex);
}


RELEASE_STATIC void _osplu_add_timer(ospl_timer_t* timer)
{
	pthread_mutex_lock(&ospl_timers.mutex);
	_osplu_list_add_tail(&ospl_timers.list_head, &timer->list);
//	_osplu_dump_list(&ospl_timers.list_head, "added timer");
	pthread_mutex_unlock(&ospl_timers.mutex);
}


static void _ospl_event_flags_cleanup(void* arg)
{
	osplu_event_flags_t *ef = (osplu_event_flags_t*)arg;
	ef->refcount--;
    //TRACE_THREAD_MAN("%s: refcount %d\n", __func__, ef->refcount);
	pthread_mutex_unlock(&ef->mutex);
}


static int _ospl_check_flags(osplu_event_flags_t *ef, ULONG requested_flags, UINT get_option)
{
	int retval = 0;
	CHECK_SIGNATURE(ef, EF_SIGNATURE, OSPL_GROUP_ERROR);

	if (ef->deleting) 
		return 1;

	switch (get_option)
	{
	case OSPL_OR_CLEAR:
	case OSPL_OR:
		retval = (ef->flags & requested_flags) ? 1 : 0;
		break;
	case OSPL_AND_CLEAR:
	case OSPL_AND:
		retval = ((ef->flags & requested_flags) == requested_flags) ? 1 : 0;
		break;
	default:
		break;
	}

	return retval;
}

