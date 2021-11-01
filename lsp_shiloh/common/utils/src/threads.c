/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "tx_api.h"
#include "tx_thr.h"
#include "threads_api.h"
#include "lassert.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#define _STACKBYTE (0xa5)
#define _STACKWORD (0xa5a5a5a5)

#ifdef HAVE_CMD
/*----------------------------------------------------------------------------*/
static const char * _state( unsigned state )
{
    switch( state )
    {
#define _CASE(state) case TX_ ## state: return #state
        _CASE(READY);
        _CASE(COMPLETED);
        _CASE(TERMINATED);
        _CASE(SUSPENDED);
        _CASE(SLEEP);
        _CASE(QUEUE_SUSP);
        _CASE(SEMAPHORE_SUSP);
        _CASE(EVENT_FLAG);
        _CASE(BLOCK_MEMORY);
        _CASE(BYTE_MEMORY);
        _CASE(IO_DRIVER);
        _CASE(FILE);
        _CASE(TCP_IP);
        _CASE(MUTEX_SUSP);
#undef _CASE
        default: return "UNKNOWN state";
    }
}

/*----------------------------------------------------------------------------*/
static unsigned _stackfree(TX_THREAD* t)
{
    int size=0;
    unsigned* ptr = t->tx_stack_start;
    while (*ptr == _STACKWORD) { size += 4; ptr++; }
    return size;
}

/*----------------------------------------------------------------------------*/
static signed _stackused(TX_THREAD* t)
{
  return t->tx_stack_size-_stackfree(t);
}

/*----------------------------------------------------------------------------*/
static unsigned _pc(TX_THREAD* t)
{
    /* There is variance in stack structure between debug and release threadx */
#ifndef DEBUG
    if( t->tx_state == TX_EVENT_FLAG )
        return *(((unsigned*)t->tx_stack_ptr)+14)-4;
    else if( t->tx_state == TX_SEMAPHORE_SUSP )
        return *(((unsigned*)t->tx_stack_ptr)+14)-4;
    else if( t->tx_state == TX_QUEUE_SUSP )
        return *(((unsigned*)t->tx_stack_ptr)+34)-4;
    else if( t->tx_state == TX_SLEEP )
        return *(((unsigned*)t->tx_stack_ptr)+14)-4;
#else
    if( t->tx_state == TX_EVENT_FLAG )
        return *(((unsigned*)t->tx_stack_ptr)+38)-4;
    else if( t->tx_state == TX_SEMAPHORE_SUSP )
        return *(((unsigned*)t->tx_stack_ptr)+32)-4;
    else if( t->tx_state == TX_QUEUE_SUSP )
        return *(((unsigned*)t->tx_stack_ptr)+36)-4;
    else if( t->tx_state == TX_SLEEP )
        return *(((unsigned*)t->tx_stack_ptr)+22)-4;
#endif
    return 0;
}

uint32_t threads_current_stack_highwater(void)
{
    TX_THREAD *current_thread;
    uint32_t highwater;

    current_thread = tx_thread_identify();
    ASSERT( NULL != current_thread );

    highwater = _stackused( current_thread );

    return highwater;
}

/*----------------------------------------------------------------------------*/
static void _show_thread(TX_THREAD* t, int highwater, int unused)
{
    UINT tx_res;
#define _NAME(x) ((x)?(x):"unknown")
    cmd_printf("%2d %-22.22s %10d %8.8x %6d",
               t->tx_priority, _NAME(t->tx_thread_name), (int)t->tx_run_count,
               (unsigned)t->tx_stack_ptr,
               (unsigned)t->tx_stack_size);
    if (highwater) cmd_printf(" %6d", _stackused(t));
    if (unused) cmd_printf(" %6d", _stackfree(t));
    cmd_printf(" %8.8x %-14s", _pc(t), _state(t->tx_state));

    if( t->tx_state == TX_EVENT_FLAG )
    {
        TX_EVENT_FLAGS_GROUP *group_ptr;
        CHAR *name;
        ULONG current_flags;
        TX_THREAD *first_suspended;
        ULONG suspended_count;
        TX_EVENT_FLAGS_GROUP *next_group;

        group_ptr = (TX_EVENT_FLAGS_GROUP *)t->tx_suspend_control_block;
        tx_res = tx_event_flags_info_get( group_ptr,
                                          &name,
                                          &current_flags,
                                          &first_suspended,
                                          &suspended_count,
                                          &next_group );


        cmd_printf(" evgroup %s(%p)=0x%x; waiting for 0x%x; option is 0x%x",
                   _NAME(name), group_ptr,
                   (unsigned)current_flags,
                   (unsigned)t->tx_suspend_info, (unsigned)t->tx_suspend_option);
    }
    else if( t->tx_state == TX_SEMAPHORE_SUSP )
    {
        TX_SEMAPHORE * pSem = (TX_SEMAPHORE *)t->tx_suspend_control_block;
        cmd_printf( " semaphore %s(%p)=%lu; %lu threads waiting",
                    _NAME(pSem->tx_semaphore_name), pSem,
                    pSem->tx_semaphore_count,
                    pSem->tx_semaphore_suspended_count);
    }
    else if( t->tx_state == TX_MUTEX_SUSP )
    {
        TX_MUTEX * pMutex = (TX_MUTEX *)t->tx_suspend_control_block;
        cmd_printf( " mutex %s(%p)=%lu; owned by %p",
                    _NAME(pMutex->tx_mutex_name), pMutex,
                    pMutex->tx_mutex_ownership_count,
                    pMutex->tx_mutex_owner);
    }
    else if( t->tx_state == TX_QUEUE_SUSP )
    {
        TX_QUEUE * pQueue = (TX_QUEUE *)t->tx_suspend_control_block;
        cmd_printf( " queue %s(%p)", _NAME(pQueue->tx_queue_name), pQueue);
    }
    else if( t->tx_state == TX_SLEEP )
    {
        cmd_printf( " %lu ticks", t->tx_thread_timer.tx_remaining_ticks);
    }
    cmd_printf( "\n");
#undef _NAME
}

static int threads_cmd_stackcheck_cb(int argc, char **argv)
{
    uint32_t stack_size;
    uint32_t high_water;
    TX_THREAD* thread;
    bool done_looping;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    thread = _tx_thread_created_ptr;

    done_looping = false;

    cmd_printf( "name,size,highwater,remaining,remaining_percent\n");
    while ( false == done_looping )
    {
        ASSERT( NULL != thread );

        stack_size = (uint32_t)thread->tx_stack_size;
        high_water = _stackused(thread);

        cmd_printf( "%s,%lu,%lu,%lu,%lu%%\n",
                    thread->tx_thread_name ? thread->tx_thread_name : "No Name",
                    stack_size,
                    high_water,
                    stack_size - high_water,
                    ( ( (stack_size - high_water) * 100 ) / stack_size ) );

        thread = thread->tx_created_next;

        if (thread == _tx_thread_created_ptr)
        {
            /* exit the loop when we wrap */
            done_looping = true;
        }
    }

    return CMD_OK;
}

/*----------------------------------------------------------------------------*/
static const char* _threads_usage="[--highwater=0|1] [--unused=0|1]";
static const char* _threads_notes="By default the amount of unused stack space "
  "is included in the list (--unused=1). The high water mark of the stack use "
  "can be shown by using the --highwater=1 option. Note that stack use will "
  "not work for stacks that have not been properly initialized.";
static int _threads_cmd(int argc, char **argv)
{
  int a, nextpri, pri=0, highwater=0, unused=1;

  for (a=1;a<argc;a++)
  {
    int error=1;
    if      (!strncmp("--highwater=", argv[a], 12))
    {
      if      (!argv[a][13] && (argv[a][12] == '0')) {error=0; highwater=0;}
      else if (!argv[a][13] && (argv[a][12] == '1')) {error=0; highwater=1;}
    }
    else if (!strncmp("--unused=", argv[a], 9))
    {
      if      (!argv[a][10] && (argv[a][9] == '0')) {error=0; unused=0;}
      else if (!argv[a][10] && (argv[a][9] == '1')) {error=0; unused=1;}
    }
    if (error)
    {
      cmd_printf("Ill formed option, %s, for command %s\n", argv[a], argv[0]);
      return CMD_USAGE_ERROR;
    }
  }

  cmd_printf("P  Name                     RunCount   StkPtr   Size");
  if (highwater) cmd_printf("   High");
  if (unused) cmd_printf("   Free");
  cmd_printf("    PC    State\n");

    while (pri < 32)
    {
        TX_THREAD* pThread = _tx_thread_created_ptr;
        nextpri = 32;
        while (pThread)
        {
            if (pThread->tx_priority == pri)
                _show_thread(pThread, highwater, unused);
            else if ((pThread->tx_priority > pri) && (pThread->tx_priority < nextpri))
                nextpri = pThread->tx_priority;
            pThread = pThread->tx_created_next;
            if (pThread == _tx_thread_created_ptr)
                break;
        }
        pri = nextpri;
    }
    return 0;
}
#endif

/*----------------------------------------------------------------------------*/
UINT _txs_thread_create(TX_THREAD *thread_ptr, CHAR *name, 
                        VOID (*entry_function)(ULONG), ULONG entry_input,
                        VOID *stack_start, ULONG stack_size, 
                        UINT priority, UINT preempt_threshold,
                        ULONG time_slice, UINT auto_start)
{
    ASSERT(!((unsigned)stack_start % 8)); /* Stacks must be aligned */
    memset(stack_start, _STACKBYTE, stack_size);
    return _tx_thread_create(thread_ptr, name, entry_function, entry_input,
                             stack_start, stack_size, priority,
                             preempt_threshold, time_slice, auto_start);
}

/*----------------------------------------------------------------------------*/
UINT _txs_thread_delete(TX_THREAD *thread_ptr)
{
    return _tx_thread_delete(thread_ptr);
}


/*----------------------------------------------------------------------------*/
int threads_set_priority(const char* name, int newpri)
{
    TX_THREAD* t = _tx_thread_created_ptr;
    while (t)
    {
        UINT old;
        char* tname = t->tx_thread_name;
        if (!strcmp(name, tname))
        {
            tx_thread_priority_change(t, newpri, &old);
            return old;
        }
        t = t->tx_created_next;
        if (t == _tx_thread_created_ptr) break;
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
void threads_init(void)
{
    static const char* _threads_desc = "Dump information about the OS threads";
    cmd_register("os", "Operating system commands, dump threads, check stacks, etc.", NULL, NULL, NULL);
    cmd_register("os threads", _threads_desc,
                  _threads_usage, _threads_notes, _threads_cmd);
    cmd_register("threads",    _threads_desc,
                  _threads_usage, _threads_notes, _threads_cmd);
    cmd_register("stack",      _threads_desc,
                  _threads_usage, _threads_notes, _threads_cmd);
    cmd_register("os stackcheck",
                 "Checks thread stacks for overflow.  Lists stacks that have reached within 1\% of their max",
                 NULL,
                 "This command is intended to be used in conjunction with a spreadsheet application.  The data is " \
                 "output in csv format with a header row and can be sorted by percent_remaining to find stacks " \
                 "that are close to zero percent remaining. Consider this shell command for a quick sort option:" \
                 "USBCmd os stackcheck | sort -n -r -t, -k5",
                 threads_cmd_stackcheck_cb);
}
