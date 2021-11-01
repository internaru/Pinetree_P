/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/


#include "pthread.h"
#include "logger.h"
#include "lassert.h"
#include "sys_init_api.h"

#define DBG_PRFX "SYSINIT: "
#define LOGGER_SUBMOD DEBUG_LOGGER_MAIN_MODULE_ONLY
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMOD
#define SYSINIT_LVL LOG_DEBUG_M // LOG_ERR to turn on during normal init

pthread_mutex_t sysinit_level1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   sysinit_level1_cond  = PTHREAD_COND_INITIALIZER;
bool sysinit_level1_run = false;    ///< true if level1 init waiters allowed to run; false else
int sysinit_level1_count = 0;       ///< count of threads that must ack level1 init

pthread_mutex_t sysinit_level2_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   sysinit_level2_cond  = PTHREAD_COND_INITIALIZER;
bool sysinit_level2_run = false;    ///< true if level2 init waiters allowed to run; false else
int sysinit_level2_count = 0;       ///< count of threads that must ack level2 init

pthread_mutex_t sysinit_level3_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t   sysinit_level3_cond  = PTHREAD_COND_INITIALIZER;
bool sysinit_level3_run = false;    ///< true if level3 init waiters allowed to run; false else

/**
 * \brief SysInit module init route; currently empty
 *
 * \author David Poole, Cory Atkin
 * \date 05-Oct-2005
 *
 */
void SysInitInit( void )
{
}


/******************************************************************************
 * Function Name: SysRegisterInitLevel
 * Created by: Cory Atkin 4/26/2004
 *
 *
 * Description: Called by level 1 and level 2 during their init function so
 *              the system knows they exist and will hold off level 3 tasks.
 *              This fixes a bug where level three tasks were being launched
 *              before the level 1 task had called SysWaitForInit
 *
 ******************************************************************************/
void SysRegisterInitLevel(INITLEVEL wait)
{
    // levels 1 and 2 are only valid wait levels
    XASSERT((wait == INITLEVEL_1 || wait == INITLEVEL_2), wait);

    if(wait == INITLEVEL_1)
    {
        pthread_mutex_lock(&sysinit_level1_mutex);
        sysinit_level1_count++;
        pthread_mutex_unlock(&sysinit_level1_mutex);
    }
    else if(wait == INITLEVEL_2)
    {
        pthread_mutex_lock(&sysinit_level2_mutex);
        sysinit_level2_count++;
        pthread_mutex_unlock(&sysinit_level2_mutex);
    }
}


/******************************************************************************
 * Function Name: SysWaitForInit
 * Created by: Cory Atkin 2/23/2004
 *
 *
 * Description: All tasks should call this function first. It suspends until
 *              system init is done. It prevents tasks from attempting to
 *              access system resources before they are initialized
 *
 ******************************************************************************/
//-jrsvoid SysWaitForInit()
//-jrs{
//-jrs    SysWaitForInitLevel(INITLEVEL_3);
//-jrs}


/******************************************************************************
 * Function Name: SysWaitForInitLevel
 * Created by: Cory Atkin 4/14/2004
 *
 *
 * Description: Suspend calling task until the appropriate init level
 *
 ******************************************************************************/
void SysWaitForInitLevel(INITLEVEL wait)
{
    //figure out what flag this task needs to wait on
    //based on the wait level
    if(wait == INITLEVEL_1)
    {
        DBG_PRINTF(SYSINIT_LVL, ("SYS: wait for level 1 init\n"));
        pthread_mutex_lock(&sysinit_level1_mutex);
        while(sysinit_level1_run == false)
        {
            pthread_cond_wait(&sysinit_level1_cond, &sysinit_level1_mutex);
        }
        pthread_mutex_unlock(&sysinit_level1_mutex);
    }
    if(wait == INITLEVEL_2)
    {
        DBG_PRINTF(SYSINIT_LVL, ("SYS: wait for level 2 init\n"));
        pthread_mutex_lock(&sysinit_level2_mutex);
        while(sysinit_level2_run == false)
        {
            pthread_cond_wait(&sysinit_level2_cond, &sysinit_level2_mutex);
        }
        pthread_mutex_unlock(&sysinit_level2_mutex);
    }
    if(wait == INITLEVEL_3)
    {
        DBG_PRINTF(SYSINIT_LVL, ("SYS: wait for level 3 init\n"));
        pthread_mutex_lock(&sysinit_level3_mutex);
        while(sysinit_level3_run == false)
        {
            pthread_cond_wait(&sysinit_level3_cond, &sysinit_level3_mutex);
        }
        pthread_mutex_unlock(&sysinit_level3_mutex);
    }
    DBG_PRINTF(SYSINIT_LVL, "SYS: level %ld task started\n", wait);
}


/******************************************************************************
 * Function Name: SysInitDone
 * Created by: Cory Atkin 4/14/2004
 *
 *
 * Description: Called by level 1 and level 2 init tasks when done with
 *              their init
 *
 ******************************************************************************/
void SysInitDone(INITDONELEVEL done)
{
    DBG_PRINTF(SYSINIT_LVL, "SYS: level %ld init done\n", done);

    switch(done)
    {
        case INITDONE_0:
            //level 0 init is done. let next level tasks run based
            //on what levels have been requested
            pthread_mutex_lock(&sysinit_level1_mutex);
            pthread_mutex_lock(&sysinit_level2_mutex);
            pthread_mutex_lock(&sysinit_level3_mutex);
            if(sysinit_level1_count > 0)
            {
                DBG_PRINTF(SYSINIT_LVL, ("SYS: start level 1 tasks\n"));
                sysinit_level1_run = true;
                pthread_cond_broadcast(&sysinit_level1_cond);
            }
            else if(sysinit_level2_count > 0)
            {
                DBG_PRINTF(SYSINIT_LVL, ("SYS: start level 2 tasks\n"));
                sysinit_level2_run = true;
                pthread_cond_broadcast(&sysinit_level2_cond);
            }
            else
            {
                DBG_PRINTF(SYSINIT_LVL, ("SYS: start level 3 tasks\n"));
                sysinit_level3_run = true;
                pthread_cond_broadcast(&sysinit_level3_cond);
            }
            pthread_mutex_unlock(&sysinit_level3_mutex);
            pthread_mutex_unlock(&sysinit_level2_mutex);
            pthread_mutex_unlock(&sysinit_level1_mutex);
            break; 

        case INITDONE_1:
            pthread_mutex_lock(&sysinit_level1_mutex);
            pthread_mutex_lock(&sysinit_level2_mutex);
            pthread_mutex_lock(&sysinit_level3_mutex);
            ASSERT(sysinit_level1_count > 0);
            sysinit_level1_count--;
            if(sysinit_level1_count == 0)
            {
                if(sysinit_level2_count > 0)
                {
                    DBG_PRINTF(SYSINIT_LVL, ("SYS: start level 2 tasks\n"));
                    sysinit_level2_run = true;
                    pthread_cond_broadcast(&sysinit_level2_cond);
                }
                else
                {
                    DBG_PRINTF(SYSINIT_LVL, ("SYS: start level 3 tasks\n"));
                    sysinit_level3_run = true;
                    pthread_cond_broadcast(&sysinit_level3_cond);
                }
            }
            pthread_mutex_unlock(&sysinit_level3_mutex);
            pthread_mutex_unlock(&sysinit_level2_mutex);
            pthread_mutex_unlock(&sysinit_level1_mutex);
            break;

        case INITDONE_2:
            pthread_mutex_lock(&sysinit_level2_mutex);
            pthread_mutex_lock(&sysinit_level3_mutex);
            ASSERT(sysinit_level2_count > 0);
            sysinit_level2_count--;
            if(sysinit_level2_count == 0)
            {
                DBG_PRINTF(SYSINIT_LVL, ("SYS: start level 3 tasks\n"));
                sysinit_level3_run = true;
                pthread_cond_broadcast(&sysinit_level3_cond);
            }
            pthread_mutex_unlock(&sysinit_level3_mutex);
            pthread_mutex_unlock(&sysinit_level2_mutex);
            break;
    }
}

#ifndef __linux__
/******************************************************************************
 * Define the abort and _exit routines.  Library calls may call these in extreme
 * error conditions.  If they do, hop over to the assert handlers so we can run
 * the emergency shutdown routines (power down motors, etc) and try to get a
 * stack trace.
 ******************************************************************************/
void abort(void)
{
    _assert("System called abort", __FILE__, __LINE__);
    while(1);
}

void _exit(int code)
{
    XAssertFail("System called _exit", __FILE__, __LINE__, code);
    while(1);
}
#endif


