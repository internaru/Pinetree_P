/*
 * ============================================================================
 * Copyright (c) 2012   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/// delay_api implementation.


#include <poll.h>
#include "memAPI.h"
#include "sys_init_api.h"
#include "agRouter.h"
#include "agMessage.h"
#include <string.h>
#include <limits.h>
#include "dbg_api.h"
#ifdef PROCESSOR_ARM
// work around using old clib
#include "timerfd.h"
#else
#include <sys/timerfd.h>
#endif
#include <stdlib.h> // atoi

#include "delay_api.h"

#ifndef __linux__
#error linux only 
#endif


#define STACK_SIZE PTHREAD_STACK_MIN 
static ALIGN8 unsigned char Stack[STACK_SIZE]; ///< thread stack.
static pthread_t delay_linux_thd_id;

void    DelayMgrInit( void );
void    *DelayMgr( void *unused );

static int nfds = 0;
static int cmd_write_fd;
static struct pollfd fds[20];

typedef struct timer_list_s
{
  int repeat; // neg one shot, positive repeat, 0 == not running.
  struct itimerspec expire_time;
  void(*timer_complete_callback)(void *timer_complete_callback_data);
  void *timer_complete_callback_data;
  MODULE_ID  modID;
  UINT32     uiParam;
} timer_list_t;

static timer_list_t tlist[20]; 

#define HZ 100

static void delay_timer_read( int fd, int index )
{
    uint64_t expired;
    MESSAGE msg;
  

    REL_XASSERT( sizeof(uint64_t) == read(fd, &expired, sizeof(uint64_t)),
		 errno );
      

    if (tlist[index].timer_complete_callback != 0)  {
        tlist[index].timer_complete_callback( tlist[index].timer_complete_callback_data );
    } else {
        msg.msgType = MSG_DELAY_NOTIFY;
        msg.param1 = tlist[index].uiParam;    
        SYMsgSend( tlist[index].modID, &msg );
    }
    if ( tlist[index].repeat == -1 ) {
        tlist[index].repeat = 0;  // change from oneshot to unused.
        fds[index].events = 0;
    }
}
/// block 
void delay_msec_timer(uint32_t msec)
{
    posix_sleep_ms(msec);
}

uint32_t timer_get_time_usec(void)
{
    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now ) ;
    // dbg_printf("time %d %d \n", now.tv_sec, now.tv_nsec);
    return now.tv_sec * 1000000 + now.tv_nsec / 1000 ;  
}

bool DMNotifyRegister( MODULE_ID modID, uint32_t nTicks, bool bRepeat, uint32_t uiParam )
{
    timer_list_t new_timer;
    int sec = nTicks / HZ;
    int nsec = (nTicks % HZ) * 1000;

    new_timer.timer_complete_callback = 0;
    new_timer.timer_complete_callback_data = 0;
    new_timer.modID = modID;
    new_timer.uiParam = uiParam;
    new_timer.expire_time.it_value.tv_sec = sec;
    new_timer.expire_time.it_value.tv_nsec = nsec;

    if (bRepeat) {
        new_timer.repeat = 1;
        new_timer.expire_time.it_interval.tv_sec = sec;
        new_timer.expire_time.it_interval.tv_nsec = nsec;
    } else {
        new_timer.repeat = -1;
        new_timer.expire_time.it_interval.tv_sec = 0;
        new_timer.expire_time.it_interval.tv_nsec = 0;
    }
    write(cmd_write_fd, &new_timer, sizeof(timer_list_t));
    return true; // todo avail check return false
}

bool DMNotifyUnregister( MODULE_ID modID, uint32_t uiParam )
{
   timer_list_t new_timer;
   
   memset( &new_timer, 0, sizeof( new_timer )); 
   new_timer.repeat = 0;
   new_timer.modID = modID;
   new_timer.uiParam = uiParam;
   write(cmd_write_fd, &new_timer, sizeof(timer_list_t));
   return true; // lie to caller tell him it was valid and we removed it.
}

delay_msec_timer_t * delay_msec_timer_cancel(delay_msec_timer_t * Non_blocking_timer)
{
   timer_list_t new_timer;
   
   memset( &new_timer, 0, sizeof( new_timer )); 
   new_timer.repeat = 0;
   new_timer.modID = 0;
   new_timer.uiParam = (uintptr_t) Non_blocking_timer;
   write(cmd_write_fd, &new_timer, sizeof(timer_list_t));
  
  return 0;
}


delay_msec_timer_t * delay_msec_timer_non_blocking(   uint32_t msec,
                                                      void(*timer_complete_callback)(void *timer_complete_callback_data),
                                                      void *timer_complete_callback_data, 
                                                      bool repeating_if_true)
{
    static int delay_msec_handle = 0;
    timer_list_t new_timer;
    int sec = msec / 1000 ;
    int nsec = (msec % 1000) * 1000000;
    
    new_timer.timer_complete_callback = timer_complete_callback;
    new_timer.timer_complete_callback_data = timer_complete_callback_data;
    new_timer.modID = 0;
    new_timer.uiParam = ++delay_msec_handle;
    new_timer.expire_time.it_value.tv_sec = sec;
    new_timer.expire_time.it_value.tv_nsec = nsec;
    
    if (repeating_if_true) {
        new_timer.repeat = 1;
        new_timer.expire_time.it_interval.tv_sec = sec;
        new_timer.expire_time.it_interval.tv_nsec = nsec;
    } else {
        new_timer.repeat = -1;
        new_timer.expire_time.it_interval.tv_sec = 0;  //sec;
        new_timer.expire_time.it_interval.tv_nsec = 0; //nsec;
    }
    write(cmd_write_fd, &new_timer, sizeof(timer_list_t));
    return (delay_msec_timer_t *)(uintptr_t)delay_msec_handle;
}

void manage_list_message( int fd )
{
    timer_list_t new_timer;
    int ret, i;

    ret = read( fd, &new_timer, sizeof( timer_list_t ));
    XASSERT( ret == sizeof( timer_list_t ), errno );
    
    if (new_timer.repeat == 0) { // delete timer 
        for (i=1; i < nfds && ret > 0; i++) {
            if (( new_timer.modID && new_timer.modID   == tlist[i].modID 
                  && new_timer.uiParam == tlist[i].uiParam )
                || ( !new_timer.modID && new_timer.uiParam == tlist[i].uiParam )) {
                timerfd_settime(fds[i].fd, 0, &new_timer.expire_time, NULL);
                tlist[i].repeat = 0;
                fds[i].events = 0;
                break;
            }
        }
    } else { // add timer
      
       for (i=1; i < nfds; i++) {
           if (tlist[i].repeat == 0 ) {
               break;
           }
       }
       if (i == nfds)  {
           int fd = timerfd_create(CLOCK_MONOTONIC, 0);
           if (fd == -1)
               XASSERT( fd != -1, errno );
           fds[i].fd = fd;
           ++nfds;
       }
       memcpy(&tlist[i], &new_timer, sizeof(timer_list_t));
       fds[i].events = POLLIN;
       if (timerfd_settime(fds[i].fd, 0, &new_timer.expire_time, NULL) == -1)
           XASSERT( 0 == "timerfd_settime", errno);
    }
}

void *delay_list_thread( void *unused )
{
    int i, ret;
    int pipefd[2];

    dbg_printf("%s \n", __FUNCTION__);
    if (pipe2(pipefd, O_CLOEXEC) == -1) {
        XASSERT(0 == "pipecreate delaylist", errno);
    }
    fds[0].fd = pipefd[0];  // read commands.
    fds[0].events = POLLIN;
    cmd_write_fd = pipefd[1]; // write commands     
    nfds = 1; // wait for commands

    SysWaitForInit();

    while(1)
    {
        ret = poll( fds, nfds, -1 );
        for (i=0; i < nfds && ret > 0; i++)
        {
            if (!fds[i].revents) 
                continue;
            ret--;
            if ( i == 0 ) 
            { // REGISTER / UNREGISTER a timer 
                manage_list_message( fds[0].fd );
                break;
            }
            else 
            { // a timer has fired.
              delay_timer_read( fds[i].fd, i );
            }
        }
    }

    return 0;
}

/// sysinit function
void DelayMgrInit(void)
{
    //dbg_printf("%s \n", __FUNCTION__);
    posix_create_thread(&delay_linux_thd_id, delay_list_thread, 0, "delay_list", 
                        Stack, STACK_SIZE, POSIX_THR_PRI_REALTIME);

}


#if defined( HAVE_CMD ) && defined( INTERNAL_COMMANDS )


static void test_timer_complete_callback( void *timer_complete_callback_data)
{
    uint32_t begin_time = (uint32_t) timer_complete_callback_data;
    uint32_t end_time = 0;

    

    end_time = timer_get_time_usec();
    //DBG_LOG_DEBUG
    dbg_printf("timer callback (%d - %d) %d \n",begin_time, end_time, end_time - begin_time);
}

/* 
static const char *delay_cmd_desc = "test commands for the delay apis";
static const char *delay_cmd_usage = "<type> <millisecond(s)>";
static const char *delay_cmd_notes = "type is one of 0, 1, 2 or 3 and mean the following: 0 is a blocking delay, 1 is a non-blocking (non-repeating) timer, 2 is a non-blocking, (repeating) timer, 3 cancels 2 and the millisecond parameter must be zero in this final case.  In all other cases millisecond(s) is the number of milliseconds to delay or wait for the timer to expire.";
*/
static int delay_cmd_cb( int argc, char *argv[] )
{
    int type; 
    int msec;
    uint32_t begin_time;
    uint32_t end_time;
    static delay_msec_timer_t * repeatingTimer;
    
    if (argc != 3 )
    {
      return -1; //CMD_USAGE_ERROR;
    }

    type = atoi(argv[1]);
    msec = atoi(argv[2]);
     
    /// dest validates input
    
    if (type == 0) 
    {

        begin_time = timer_get_time_usec();
        //DBG_LOG_DEBUG("starting delay %10d \n",begin_time);
        delay_msec_timer( msec );
        end_time = timer_get_time_usec();
        //DBG_LOG_DEBUG("ending delay   %10d \n",end_time);
        //DBG_LOG_DEBUG
	dbg_printf("delay 0.1 msec  %d \n",end_time - begin_time);
    }
    else if (type == 1) 
    {
        begin_time = timer_get_time_usec();
        delay_msec_timer_non_blocking(msec,
                                      test_timer_complete_callback,
                                      (void *)begin_time, //< callback data on interrupt.
                                      false);
    }
    else if (type == 2) 
    {
        begin_time = timer_get_time_usec();
        repeatingTimer = delay_msec_timer_non_blocking(msec,
                                                       test_timer_complete_callback,
                                                       (void *)begin_time, //< callback data on interrupt.
                                                       true);
    }
    else if (type == 3)
    {
        if (repeatingTimer) 
        {
            repeatingTimer = delay_msec_timer_cancel(repeatingTimer);
        }
    }
    else
    {
      return -1; //CMD_USAGE_ERROR;
    }

    return 0; // CMD_OK;
}


void delay_test(void)
{
  char *argv[3] = { "delay", "3", "31" };
    
  argv[1] = "0"; argv[2] = "200";
  delay_cmd_cb( 3, argv);
  argv[1] = "1"; argv[2] = "300";
  delay_cmd_cb( 3, argv);

  argv[1] = "2"; argv[2] = "2000";
  delay_cmd_cb( 3, argv);
  sleep(10);
  
  argv[1] = "3"; argv[2] = "2000";
  delay_cmd_cb( 3, argv);


  argv[1] = "2"; argv[2] = "10";
  delay_cmd_cb( 3, argv);
  sleep(1);

  argv[1] = "3"; argv[2] = "10";
  delay_cmd_cb( 3, argv);


  argv[1] = "1"; argv[2] = "3";
  delay_cmd_cb( 3, argv);
  sleep(1);

  argv[1] = "1"; argv[2] = "3";
  delay_cmd_cb( 3, argv);
  sleep(1);

  argv[1] = "1"; argv[2] = "30";
  delay_cmd_cb( 3, argv);
  sleep(1);

  argv[1] = "1"; argv[2] = "30";
  delay_cmd_cb( 3, argv);
  sleep(1);

  argv[1] = "1"; argv[2] = "30";
  delay_cmd_cb( 3, argv);
  sleep(1);

  argv[1] = "1"; argv[2] = "501";
  delay_cmd_cb( 3, argv);
  sleep(1);

  argv[1] = "1"; argv[2] = "201";
  delay_cmd_cb( 3, argv);
  sleep(1);
  
  argv[1] = "1"; argv[2] = "301";
  delay_cmd_cb( 3, argv);
  sleep(1);
}

#endif
