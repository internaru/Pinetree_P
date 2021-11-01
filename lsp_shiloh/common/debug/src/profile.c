/** 
 *
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Implement profile code if required.
 *
 **/
 
#include <stdint.h>
#include "agRouter.h"
#include "memAPI.h"
#include "dprintf.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif // #ifdef HAVE_CMD
#include "hwconfig_api.h"
#include "cpu_api.h"
#include <string.h>
#include "lassert.h"
#include <pthread.h>
#include <posix_ostools.h>
#ifndef __linux__
#include "tx_api.h"
#endif

#define PROFILE_SCALE_FACTOR (1<<PROFILE_SCALE_POWER)

#define TASK_COUNT  100


volatile uint32_t debug_profiler_StartTime, debug_profiler_TotalTime, debug_profiler_TaskNum;
uint32_t debug_profiler_Tasks[TASK_COUNT][2]; // Time for each task.

// interrupt time is slot 0
// idle loop is slot 1
//     ->NOTE: To prevent most/all of what would be "idle loop" time from being consumed by
//             the low priority PowerSaveCPU thread, for the purpose of task profiling it is
//             recommended that function powerSaveCPUTx in powerSave.c be modified such that
//             it exits immediately.
// tasks start at slot 2.

uint32_t debug_profiler_IntTasks[32]; // time for each vectored interrupt
uint32_t debug_profiler_IntNum;

//#define PROFILE_IDLE_ONLY    // turn on to only output idle usage
//#define PROFILE_AUTO_CAPTURE // turn on to do repetitive auto-capture & print
//#define PROFILE_LOG_TO_RAM   // turn on to log to RAM instead of serial debug

#ifdef PROFILE_IDLE_ONLY
#define HISTORY_SIZE 10      // # samples to accumulate before printing
static uint32_t IdleHistory[HISTORY_SIZE][2];
#else // #ifndef PROFILE_IDLE_ONLY
#define IDLE_THRESHOLD 100   // maximum idle percentage for which to generate a profile
#endif // #ifdef PROFILE_IDLE_ONLY

#ifdef PROFILE_AUTO_CAPTURE
static timer_t profileTimer;
#define PROFILER_MS 1000 // sample every 1.0 seconds
#endif // #ifdef PROFILE_AUTO_CAPTURE
 
#ifdef PROFILE_LOG_TO_RAM
// NOTE: The macro below is intended to be linked to the logger.c logString() function.  To
//       use the PROFILE_LOG_TO_RAM functionality, rename the "real" logString() function
//       to logStringProfile() and create a NOP function named logString() in logger.c and
//       logger.h, effectively bypassing existing usage of the RAM logger & giving the
//       profiler exclusive use.  Also note that for release builds, config.mk needs to be
//       modified to turn on the MEMORY_LOG flag.
#define PROFILE_LOG logStringProfile
#else // #ifndef PROFILE_LOG_TO_RAM
#define PROFILE_LOG dbg_printf
#endif // #ifdef PROFILE_LOG_TO_RAM

 
/* FUNCTION NAME: ProfileInit */
 
/** 
 * \brief Set all profile values back to zero.  Starts the profiling.
 * 
 * \retval None
 * 
 **/
void ProfileInit(void)
{
    uint32_t i;
    uint32_t Posture;

    // save current interrupt state & disable interrupts
    Posture = cpu_disable_interrupts();

    // intialize the data for the next accumulation window
    debug_profiler_TotalTime = 0;
    debug_profiler_TaskNum = 0;
    for(i = 0; i < TASK_COUNT; i++)
    {
        debug_profiler_Tasks[i][0] = 0;
        debug_profiler_Tasks[i][1] = 0;
    }
    debug_profiler_IntNum = 0;
    for(i = 0; i < 32; i++)
    {
        debug_profiler_IntTasks[i] = 0;
    }
    debug_profiler_StartTime = cpu_get_ccount();  // initialize the start time.

    // restore the interrupt state
    cpu_restore_interrupts(Posture);
}
 
 
/* FUNCTION NAME: PrintProfile */
 
/** 
 * \brief Prints the result of the current profile
 * 
 * \param argc standard argc
 * 
 * \param argv standard argv.
 * 
 * \retval int not used.
 * 
 **/
int PrintProfile( int argc, char *argv[] )
{
    uint32_t j;
    uint32_t sum, sum_ints;
    bool needToLog;
#ifdef PROFILE_IDLE_ONLY
    char myLine[129];
    static int historyIndex = -1;
#else // #ifndef PROFILE_IDLE_ONLY
    char *taskName;
#endif // #ifdef PROFILE_IDLE_ONLY
    uint32_t rescale;
    uint32_t mask;
    uint32_t TaskTime_rescaled;
    uint32_t TotalTime_rescaled;

#ifdef PROFILE_IDLE_ONLY
    // clear our string buffer
    memset(myLine, 0, 129);

    // capture the current idle and title time sample
    historyIndex++;
    if (historyIndex >= HISTORY_SIZE)
    {
        historyIndex = 0;
    }
    IdleHistory[historyIndex][0] = debug_profiler_Tasks[1][1];
    IdleHistory[historyIndex][1] = debug_profiler_TotalTime;

    // take note if we have enough samples for the next display output
    needToLog = ((HISTORY_SIZE-1) == historyIndex);
#else // #ifndef PROFILE_IDLE_ONLY
    // only log if we are busy enough (not idle) to exceed the threshhold
    needToLog = (IDLE_THRESHOLD > ((debug_profiler_Tasks[1][1]*100)/debug_profiler_TotalTime));

    // output a label for the data
    PROFILE_LOG("FW Profile\n");
#endif // #ifdef PROFILE_IDLE_ONLY

    j = 0;
    sum = 0;

#ifdef PROFILE_IDLE_ONLY
    // output the history of idle loop cpu usage samples
    while (needToLog && (j<=(HISTORY_SIZE-1)))
#else // #ifndef PROFILE_IDLE_ONLY
    // loop through the interrupt and idle-loop buckets *and* all the actual threads
    while( needToLog && ((j<2) || (debug_profiler_Tasks[j][0] != 0)))
#endif // #ifdef PROFILE_IDLE_ONLY
    {
#ifndef PROFILE_IDLE_ONLY
        // for a sanity check, keep a running total of the individual thread cpu cycle counts
        sum += debug_profiler_Tasks[j][1];

        // select a display name for this "thread"
        if (0 == j)
        {
            taskName = "Interrupt";
        }
        else if (1 == j)
        {
            taskName = "IdleLoop";
        }
#ifndef __linux__
        else if (NULL == ((TX_THREAD *) debug_profiler_Tasks[j][0])->tx_thread_name)
        {
            taskName = "unknown";
        }
        else
        {
            taskName = ((TX_THREAD *) debug_profiler_Tasks[j][0])->tx_thread_name;
        }
#endif
        else
        {
            taskName = "unknown";
        }
#endif // #ifndef PROFILE_IDLE_ONLY

        // calculate a rescale factor and rescale the associated task and total time so
        // the percent usage doesn't overflow 32 bits
        mask = 0x80000000;
        rescale = 7;
        while (rescale)
        {
#ifdef PROFILE_IDLE_ONLY
            if (IdleHistory[j][1] & mask)
#else // #ifndef PROFILE_IDLE_ONLY
            if (debug_profiler_TotalTime & mask)
#endif // #ifdef PROFILE_IDLE_ONLY
            {
                break;
            }
            mask = mask >> 1;
            rescale--;
        }
#ifdef PROFILE_IDLE_ONLY
        TaskTime_rescaled = IdleHistory[j][0] >> rescale;
        TotalTime_rescaled = IdleHistory[j][1] >> rescale;
#else // #ifndef PROFILE_IDLE_ONLY
        TaskTime_rescaled = debug_profiler_Tasks[j][1] >> rescale;
        TotalTime_rescaled = debug_profiler_TotalTime >> rescale;
#endif // #ifdef PROFILE_IDLE_ONLY

#ifdef PROFILE_IDLE_ONLY
        // add the current idle loop cpu usage sample percentage to our output string
        minSprintf(&myLine[strlen(myLine)], "%3d.%02d ",
                  (TaskTime_rescaled*100)/TotalTime_rescaled,
                  (((TaskTime_rescaled*100) - (((TaskTime_rescaled*100)/TotalTime_rescaled)*TotalTime_rescaled))*100)/TotalTime_rescaled);
#else // #ifndef PROFILE_IDLE_ONLY
        // output the thread name and cpu usage (both in microseconds and as percent of total)
        PROFILE_LOG("Task Time %35s = %9d usec, %3d.%02d%%\n",
                 taskName,
                 (debug_profiler_Tasks[j][1]/hwGetProcSpeed())*PROFILE_SCALE_FACTOR,
                 (TaskTime_rescaled*100)/TotalTime_rescaled,
                 (((TaskTime_rescaled*100) - (((TaskTime_rescaled*100)/TotalTime_rescaled)*TotalTime_rescaled))*100)/TotalTime_rescaled);
#endif // #ifdef PROFILE_IDLE_ONLY
        j++;
    }

    if (needToLog) // we're outputting on this call
    {
#ifdef PROFILE_IDLE_ONLY
        // add a blank line & output our assembled string
        strcat(myLine, "\n");
        PROFILE_LOG(myLine);
#else // #ifndef PROFILE_IDLE_ONLY
        // show the total elapsed cpu time & compare (sanity check) to the sum for all "threads"
        // NOTE: The captured total time and the sum for all threads should be fairly close,
        // but will differ by the data that continues to accumulate while this function is running.
        PROFILE_LOG("Last captured total time is %d usec (%d msec)\n", (debug_profiler_TotalTime/hwGetProcSpeed())*PROFILE_SCALE_FACTOR, ((debug_profiler_TotalTime/hwGetProcSpeed())*PROFILE_SCALE_FACTOR)/1000);
        PROFILE_LOG("Sum of all task times is %d usec (%d msec)\n", (sum/hwGetProcSpeed())*PROFILE_SCALE_FACTOR, ((sum/hwGetProcSpeed())*PROFILE_SCALE_FACTOR)/1000);
        PROFILE_LOG("\n");

        // output a breakdown of the vectored interrupt cpu usage
        PROFILE_LOG("Interrupt Profile\n");
        sum_ints = 0;
        for (j=0; j<32; j++)
        {
            sum_ints += debug_profiler_IntTasks[j];
            PROFILE_LOG("VIC %2d ISR Time = %9d usec\n", j, (debug_profiler_IntTasks[j]/hwGetProcSpeed())*PROFILE_SCALE_FACTOR);
        }
        PROFILE_LOG("Sum of all VIC ISR times is %d usec\n", (sum_ints/hwGetProcSpeed())*PROFILE_SCALE_FACTOR);
#endif // #ifdef PROFILE_IDLE_ONLY
    }

    // clear out all previous profile info for the next accumulation window
    ProfileInit();

    return 0;
}


#ifdef PROFILE_AUTO_CAPTURE
/* FUNCTION NAME: profilerCallback */
 
/** 
 * \brief Timer callback to print the current profile.
 * 
 * \param unused Not used.
 * 
 * \retval none
 * 
 **/
static void profilerCallback( union sigval unused )
{
    // print the latest profile snapshot & reinitialize
    PrintProfile( 1, NULL );
}
#endif // #ifdef PROFILE_AUTO_CAPTURE


/* FUNCTION NAME: InitProfileCode */
 
/** 
 * \brief Initialize the profile code and add in the debug stuff.
 * 
 * \retval None
 * 
 **/
void InitProfileCode(void)
{
    int cmd_res;
    int px_status;

    ProfileInit();
#ifdef HAVE_CMD
    cmd_res = cmd_register_cmd("profile", 
                               NULL, 
                               "profile: capture per thread cpu usage profile",
                               NULL,
                               NULL,
                               PrintProfile);
    ASSERT(CMD_OK == cmd_res);
#endif // #ifdef HAVE_CMD

#ifdef PROFILE_AUTO_CAPTURE
    // start a timer to perform a repetitive auto-capture & print of the profile
    struct sigevent evp;
    struct itimerspec its;
    
    memset( &evp, 0, sizeof(struct sigevent) );
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = profilerCallback;
    evp.sigev_signo = SIGSTOP;

    px_status = timer_create( CLOCK_REALTIME, &evp, &profileTimer );
    XASSERT( px_status==0, px_status );

    /* 
     * Create periodic timer.
     */
    memset( &its, 0, sizeof(struct itimerspec) );
    its.it_interval.tv_sec = PROFILER_MS/MILLISEC_PER_SECOND;
    its.it_interval.tv_nsec = (PROFILER_MS%MILLISEC_PER_SECOND) * (NANOSEC_PER_USEC*USEC_PER_MILLISEC);
    its.it_value = its.it_interval;

    px_status = timer_settime( profileTimer, 0, &its, NULL );
    XASSERT( px_status==0, px_status );
    
#endif // #ifdef PROFILE_AUTO_CAPTURE
}


