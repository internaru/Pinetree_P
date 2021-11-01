/*
 ***********************************************************
 *(c) Copyright Sindoh Co. Ltd. 
 *
 *               SINDOH Confidential
 * 
 ***********************************************************
 **/

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "SysPosixTimer.h"
#include "SysUI_Interface.h"
#include "agRouter.h"
#include "posix_ostools.h"
#include "logger.h"
#include "lassert.h"

static void SPTimerTimeoutHandler();

static sem_t g_tTimerSem;		///< Semaphore that's signaled if timer signal arrived
static bool g_bTimerStopped;	///< Non-zero if the timer is to be g_bTimerStopped

extern int g_nEnergySaveType;
int g_nTimeoutValue = 0; ///<In seconds time out value
static int g_nTimer = 0;	///< Timer used for timeout
bool g_bResetTimeout;	///< Reset Time out 
bool g_bPauseTimeout;	///< Pause Time out 
bool g_bDeleteTimer;	///< Delete Timer
sem_t semTimerFinished;	///< A semaphore to stop the timer when done

bool g_bDebugESMTimer = false;	///< Debug Energy Save Mode timer
bool g_bDebugAdaptiveTimer = false;	///<  Debug Adaptive Energy Save Mode timer
bool g_bDebugHourlyTimer = false;	///<  Debug Hourly timer
bool g_bDebugtimelyTimer = false;	///<  Debug Timely timer

#define ADAPTIVE_WEEKLY_TIMER
#ifdef ADAPTIVE_WEEKLY_TIMER
static int g_nAdaptiveTimeout = 120;	///< 2 Minutes = 2*60 = 120 Seconds Refresh timeout: Adeptive timeout
static int g_nAdaptiveTimer = 0;	///< Timer used for Adaptive timeout
#endif

#define HOURLY_CLOCK_TIMER
#ifdef HOURLY_CLOCK_TIMER
static int g_nHourlyTimeout = 3600;	///< 60 Minutes = 60*60 = 3600 Seconds timeout
static int g_nHourlyTimer = 0;	///< Timer used for Hourly clock update
static int g_nSysUpTimeInHour = 0;	///< Total system up time in hour
#endif

#define TIMELY_MSG_TIMER
#ifdef TIMELY_MSG_TIMER
int g_nTimelyTimeout = 0; ///< Set by SSM
static int g_nTimelyTimer = 0;	///< Timer used for Hourly clock update
#endif

#ifdef HAVE_IMAGELOG
#define IMAGE_LOG_TIMER
#ifdef IMAGE_LOG_TIMER
int g_nImageLogDeliveryTimeout = 0; ///< Set by ImageLogMonitor in Seconds
static int g_nImageLogDeliveryTimer = 0;	///< Timer used for Image Log delivery timer update
int g_nImageLogIntervalTimeout = 0; ///< Set by ImageLogMonitor in Seconds
static int g_nImageLogIntervalTimer = 0;	///< Timer used for Image Log interval timer update
#endif
#endif

#ifdef HAVE_SCANBOX
#define SCAN_BOX_TIMER
#ifdef SCAN_BOX_TIMER
int g_nScanBoxDeleteIntervalTimeout = 0; ///< Set by ScanBox in Seconds
static int g_nScanBoxDeleteIntervalTimer = 0;	///< Timer used for ScanBox delete interval
#endif
#endif

static ALIGN8 UINT8 SysPosixTimerStack[POSIX_MIN_STACK_SIZE];      ///< Stack for the Posix Timer 
static pthread_t SysPosixTimer_thd_id;	///< Thread ID in which user timer functions execute
pthread_mutex_t gSysPosixTimerMutex;

/**
 * 
 *\brief Timer signal handler.
 * On each timer signal, the signal handler will signal a semaphore.
 *
 */
static void TimerExpireSignalHandler() 
{
	/* called in signal handler context, we can only call async-signal-safe functions now!*/
	sem_post(&g_tTimerSem);	// the only async-signal-safe function pthreads defines
}

/**
 * 
 *\brief Timer thread waits for posts on the timer semaphore.
 * For each post, SPTimerTimeoutHandler() is called once.
 * 
 * This ensures that the SPTimerTimeoutHandler() is not called in a signal context.
 *
 **/ 
static void *SysPosixTimerThread(void *notUsed) 
{
	signal(SIGALRM, TimerExpireSignalHandler);
	SPTSetPeriodicTimer(1,1);
	
	while (!g_bTimerStopped) {
		int rc = sem_wait(&g_tTimerSem);		// retry on EINTR
		if (rc == -1 && errno == EINTR){		//EINTR: The call was interrupted by a signal handler			
		    continue;
		}
		
		if (rc == -1) {
		    perror("sem_wait");
		    exit(-1);
		}
		
		SPTimerTimeoutHandler();	//Time out handler
	}
	return 0;
}

void SPTInitializeTimer(void) 
{
	int px_status;
	px_status = posix_mutex_init( &gSysPosixTimerMutex );
	XASSERT( px_status==0, px_status );

	pthread_mutex_lock( &gSysPosixTimerMutex );
	g_bResetTimeout = false;
	g_bPauseTimeout = false;
	pthread_mutex_unlock( &gSysPosixTimerMutex );

	g_bDeleteTimer = false;
	
	sem_init(&g_tTimerSem, /*not shared*/ 0, /*initial value*/0);
	//pthread_create(&SysPosixTimer_thd_id, (pthread_attr_t*)0, SysPosixTimerThread, (void*)0);	
	posix_create_thread(&SysPosixTimer_thd_id, SysPosixTimerThread, 0, "sysposixtimer", SysPosixTimerStack, POSIX_MIN_STACK_SIZE, POSIX_THR_PRI_NORMAL);
	//signal(SIGALRM, TimerExpireSignalHandler);

#if 0	
	struct sigaction sa;
	 /* Install timer_handler as the signal handler for SIGVTALRM. */
	 memset (&sa, 0, sizeof (sa));
	 sa.sa_handler = &TimerExpireSignalHandler;
	 sigaction (SIGVTALRM, &sa, NULL);
#endif
}

void SPTShutdownTimer() 
{
	g_bTimerStopped = true;
	sem_post(&g_tTimerSem);
	pthread_join(SysPosixTimer_thd_id, 0);
}

void SPTSetPeriodicTimer(long secDelay, long usecDelay) 
{
	struct itimerval stITimerVal = { 
		 .it_interval = { .tv_sec = secDelay, .tv_usec = usecDelay }, 
		 .it_value = { .tv_sec = secDelay, .tv_usec = usecDelay }};

	setitimer(ITIMER_REAL, &stITimerVal, (struct itimerval*)0);
	
	/* it_interval: subsequent firings
	    it_value: first firing 
	    ITIMER_REAL : decrements in real time, and delivers SIGALRM upon expiration.
	    ITIMER_VIRTUAL	: decrements only when the process is executing, and delivers SIGVTALRM upon expiration.
	    ITIMER_PROF : decrements both when the process executes and when the system is executing on behalf of the process.
	*/
}

//
static void SPTimerTimeoutHandler() 
{	
	static int i = 0;

	///TODO: MUTEX
	
	pthread_mutex_lock( &gSysPosixTimerMutex );
	
	if(!g_bPauseTimeout)	{
		++g_nTimer;
	}
	else	{
		g_nTimer = 0;
	}

	if(g_bResetTimeout){
		g_bPauseTimeout = false;
		g_bResetTimeout = false;
		g_nTimer = 0;
	}
	pthread_mutex_unlock( &gSysPosixTimerMutex );
	
	if(g_bDeleteTimer){
		g_bDeleteTimer = false;
		SPTShutdownTimer();
		sem_post(&semTimerFinished);	// signal main thread to exit
	}

	if(g_nTimeoutValue <= g_nTimer){
		//SEND Timeout message to System Service
		// reset and stop timer
		pthread_mutex_lock( &gSysPosixTimerMutex );
		g_nTimer = 0;
		pthread_mutex_unlock( &gSysPosixTimerMutex );
#if 1 //send message to System service
		//If Timeout Value is 0. Means Power/Energy save disabled by user, so don't send timeout message.
		if( g_nTimeoutValue != 0 ) {
			stWnd_EnergyMode stEnergyMode = {0};
			error_type_t err;
			SYS_UI_MESSAGE msg;
		
			stEnergyMode.mode = ESM_ENERGY_SAVE; 	// 0 : normal 	1 : energy save mode
			
			msg.msgSender = SYSTEM_ENERGY_SAVE_TIMER;
			msg.cmd = CMD_INT_ENERGY_MODE;
			memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);
			memcpy(msg.data, &stEnergyMode, sizeof(stWnd_EnergyMode));
			
			err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
			XASSERT( err==OK, err );
		}
#endif
	}
	
#ifdef ADAPTIVE_WEEKLY_TIMER
	//Adaptive Energy Save mode: Refresh timeout value
	if(g_bDebugAdaptiveTimer){
		printf("g_nEnergySaveType = %d, g_nAdaptiveTimer=%d\n", g_nEnergySaveType, g_nAdaptiveTimer);
	}

	++g_nAdaptiveTimer;
	if(g_nAdaptiveTimeout <= g_nAdaptiveTimer){
		g_nAdaptiveTimer = 0; //reset timer
		
		stWnd_EnergyMode stEnergyMode = {0};
		error_type_t err;
		SYS_UI_MESSAGE msg;
	
		stEnergyMode.mode = ESM_ADAPTIVE_TIMEOUT;
		
		msg.msgSender = SYSTEM_ENERGY_SAVE_TIMER;
		msg.cmd = CMD_INT_ENERGY_MODE;
		memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);
		memcpy(msg.data, &stEnergyMode, sizeof(stWnd_EnergyMode));
		
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
		XASSERT( err==OK, err );		
	}
#endif

#ifdef HOURLY_CLOCK_TIMER
	//Hourly System Clock update
	if(g_bDebugHourlyTimer){
		printf ("System UP Time: Hours=%d, Minutes=%d Seconds=%d\n", g_nSysUpTimeInHour, g_nHourlyTimer/60, g_nHourlyTimer%60);
	}

	++g_nHourlyTimer;
	if(g_nHourlyTimeout <= g_nHourlyTimer){
		g_nHourlyTimer = 0; //reset hourly timer
		++g_nSysUpTimeInHour;
		
		stIC_HourlyAlarm stHourlyAlarm;
		error_type_t err;
		SYS_UI_MESSAGE msg;
	
		stHourlyAlarm.totalSysUpTime = g_nSysUpTimeInHour;
		
		msg.msgSender = SYSTEM_ENERGY_SAVE_TIMER;
		msg.cmd = CMD_INT_HOUR_ALARM;
		memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);
		memcpy(msg.data, &stHourlyAlarm, sizeof(stIC_HourlyAlarm));
		
		err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
		XASSERT( err==OK, err );		
	}
#endif

#ifdef TIMELY_MSG_TIMER
	//Timely message to SSM. Time will be set by SSM
	if(g_bDebugtimelyTimer){
		printf ("Timely Timer: Timeout=%d, Timer=%d\n", g_nTimelyTimeout, g_nTimelyTimer);
	}

	if(g_nTimelyTimeout){
		++g_nTimelyTimer;
		if(g_nTimelyTimeout <= g_nTimelyTimer){
			stIC_TimelyAlarm stTimelyAlarm;
			error_type_t err;
			SYS_UI_MESSAGE msg;

			stTimelyAlarm.totalTimeInSec = g_nTimelyTimer;

			msg.msgSender = SYSTEM_ENERGY_SAVE_TIMER;
			msg.cmd = CMD_INT_TIMELY_ALARM;
			memset(&msg.data, 0, SYS_UI_MESSAGE_DATA_LEN);
			memcpy(msg.data, &stTimelyAlarm, sizeof(stIC_TimelyAlarm));

			err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
			XASSERT( err==OK, err );

			//reset Timely timeout and timer
			pthread_mutex_lock( &gSysPosixTimerMutex );
			g_nTimelyTimeout = 0;
			pthread_mutex_unlock( &gSysPosixTimerMutex );
			g_nTimelyTimer = 0;
		}
	}
#endif

#ifdef HAVE_IMAGELOG
#ifdef IMAGE_LOG_TIMER
	//Timely message to ImageLogMonitor. Time will be set by ImageLogMonitor
	if(g_bDebugtimelyTimer){
		printf ("Image Log Delivery Timer: Timeout=%d, Timer=%d\n Image Log Interval Timer: Timeout=%d, Timer=%d\n",
				g_nImageLogDeliveryTimeout, g_nImageLogDeliveryTimer, g_nImageLogIntervalTimeout, g_nImageLogIntervalTimer);
	}

	if(g_nImageLogDeliveryTimeout){
		++g_nImageLogDeliveryTimer;
		if(g_nImageLogDeliveryTimeout <= g_nImageLogDeliveryTimer){
			error_type_t err;
			MESSAGE msg;
			msg.msgType = MSG_IMAGE_LOG_TIMEOUT;
			msg.param1 = g_nImageLogDeliveryTimer;
			msg.param2 = 1;
			msg.param3 = NULL;
			err = SYMsgSend(IMAGE_LOG_MONITOR_ID, &msg);
			XASSERT( err==OK, err );

			//reset  Image Log interval timeout and timer
			pthread_mutex_lock( &gSysPosixTimerMutex );
			g_nImageLogDeliveryTimeout = 0;
			pthread_mutex_unlock( &gSysPosixTimerMutex );
			g_nImageLogDeliveryTimer = 0;
		}
	}

	if(g_nImageLogIntervalTimeout){
		++g_nImageLogIntervalTimer;
		if(g_nImageLogIntervalTimeout <= g_nImageLogIntervalTimer){
			error_type_t err;
			MESSAGE msg;
			msg.msgType = MSG_IMAGE_LOG_TIMEOUT;
			msg.param1 = g_nImageLogIntervalTimer;
			msg.param2 = 2;
			msg.param3 = NULL;
			err = SYMsgSend(IMAGE_LOG_MONITOR_ID, &msg);
			XASSERT( err==OK, err );

			//reset Image Log interval timeout and timer
			pthread_mutex_lock( &gSysPosixTimerMutex );
			g_nImageLogIntervalTimeout = 0;
			pthread_mutex_unlock( &gSysPosixTimerMutex );
			g_nImageLogIntervalTimer = 0;
		}
	}
#endif
#endif

#ifdef HAVE_SCANBOX
#ifdef SCAN_BOX_TIMER
	//Timely message to ScanBox. Time will be set by ScanBox.
	if(g_bDebugtimelyTimer){
		printf ("ScanBox Interval Timer: Timeout=%d, Timer=%d\n",
				g_nScanBoxDeleteIntervalTimeout, g_nScanBoxDeleteIntervalTimer);
	}

	if(g_nScanBoxDeleteIntervalTimeout){
		++g_nScanBoxDeleteIntervalTimer;
		if(g_nScanBoxDeleteIntervalTimeout <= g_nScanBoxDeleteIntervalTimer){
			error_type_t err;
			MESSAGE msg;
			msg.msgType = MSG_SCAN_BOX_TIMEOUT;
			msg.param1 = g_nScanBoxDeleteIntervalTimer;
			msg.param2 = 1;
			msg.param3 = NULL;
			err = SYMsgSend(SCAN_BOX_ID, &msg);
			XASSERT( err==OK, err );

			//reset  ScanBox interval timeout and timer
			pthread_mutex_lock( &gSysPosixTimerMutex );
			g_nScanBoxDeleteIntervalTimeout = 0;
			pthread_mutex_unlock( &gSysPosixTimerMutex );
			g_nScanBoxDeleteIntervalTimer = 0;
		}
	}
#endif
#endif

	if(i >= 600)
		i = 0;

	++i;
	if(g_bDebugESMTimer){
		struct itimerval getTimeVal;
		if (getitimer(ITIMER_REAL, &getTimeVal) == -1){
			printf("Failed to get real timer\n");
		}
		else{
			printf("%06ldSec.%06ld:" , getTimeVal.it_value.tv_sec, getTimeVal.it_value.tv_usec);
		}
		printf ("Timer called Paused = %d, Reset= %d, TimeOut= %d, Timer=%d  %d!\n", g_bPauseTimeout, g_bResetTimeout, g_nTimeoutValue, g_nTimer, i);
	}
}
//END
