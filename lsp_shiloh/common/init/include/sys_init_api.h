/*
 *
 * ============================================================================
 * Copyright (c) 2006-2014   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file sys_init_api.h
 *
 * \brief
 *
 */

#ifndef __SYS_INIT_API_H__
#define __SYS_INIT_API_H__

#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Startup initialization. This allows the tasks to come up in a
 * predefined order. 
 *
 * When a module's task first runs it must call SysWaitForInitLevel() with
 * its wait level. The system will suspend the task until the preceding level
 * of initialization has completed.
 *
 * These are the initializaton levels:
 * level 0 - Start OS, initialize system hardware, call all modules init
 *           functions.
 *           When this init is done SysInitDone() is called with INITDONE_0
 * level 1 - Run all tasks that called SysWaitForInitLevel() with INITLEVEL_1.
 *           These tasks are run after INITDONE_0
 *           When these tasks are done with their init, they call SysInitDone()
 *           with INITDONE_1
 * level 2 - Run all tasks that called SysWaitForInitLevel() with INITLEVEL_2.
 *           These tasks are run after INITDONE_1
 *           When these tasks have completed their init, they call SysInitDone()
 *           with INITDONE_2
 * level 3 - Run all tasks that call SysWaitForInit() or SysWaitForInitLevel()
 *           with INITLEVEL_3.
 *           These tasks are run after INITDONE_2.
 *           This is the last level, most tasks should wait for this level. They
 *           do not call SysInitDone()
 *
 * The system will keep track of how many tasks register for level 1 and level 2
 * and won't run tasks at the next level until all the preceding tasks have
 * indicated they are done.
 */
typedef enum
{
   INITLEVEL_1 = 1,    ///< Run during level 1 of init
   INITLEVEL_2 = 2,        ///< Run during level 2
   INITLEVEL_3 = 3,        ///< Run last
}INITLEVEL;

typedef enum
{
   INITDONE_0 = 0,    ///< Level 0 is done, run level 1
   INITDONE_1 = 1,        ///< Level 1 is done, run level 2
   INITDONE_2 = 2,        ///< Level 2 is done, run all others
}INITDONELEVEL;

void SysInitInit( void );

/**
 * \brief Called by tasks during their init function to register to run at the
 * given wait level.
 *
 * Any task that calls this function during init, must call SysWaitForInitLevel
 * with the same wait value.
 *
 * \b Example:
 * \code
 *    taskInit()
 *    {
 *       ...create task, message queue etc
 *       SysRegisterInitLevel(INITLEVEL_1);
 *    }
 *
 *    task()
 *    {
 *       SysWaitForInitLevel(INITLEVEL_1);
 *       ... do level 1 init
 *       SysInitDone(INITDONE_1);
 *    }
 * \endcode
 */
void SysRegisterInitLevel(INITLEVEL wait);

/**
 * \brief This function is called by all tasks that can run last.
 *
 * Currently it is the same as calling SysWaitForInitLevel(INITWAIT_3);
 * It is preferrable to call this function instead so that we can add
 * more levels later if needed and not have to go back and modify each
 * task.
 */
#define SysWaitForInit() \
    do \
    { \
        SysWaitForInitLevel(INITLEVEL_3); \
        posix_display_thread_sched_attr(__func__); \
    } while (0)


/**
 * \brief Run this task at the given level of init. See example below
 */
void SysWaitForInitLevel(INITLEVEL wait);

/**
 * \brief Called by tasks doing a given level of init to indicate they are done.
 *
 * \b Example:
 * \code
 *     taskThread()
 *     {
 *        SysWaitForInitLevel(INITWAIT_1);    //wait until level 0 is done
 *        DoInit();                           //do level one init
 *        SysInitDone(INITDONE_1);            //tell system done with level 1
 *        while(1)
 *          DoStuff();
 *     }
 * \endcode
 */
void SysInitDone(INITDONELEVEL done);

/**
 * \brief One task in the system is designated as the startup task. When it runs
 * it calls this function first to do the Level 0 initialization.
 *
 * \b Example:
 * \code
 *     taskThread()
 *     {
 *        SysInitSystem();           //do level 0 init
 *        SysInitDone(INITDONE_0);   //done with level 0
 *        SysWaitForInit();          //wait for all other init levels to finish
 *        while(1)
 *          DoStuff();
 *     }
 * \endcode
 */
void* SysInitSystem( void *unused );

/**
 * \brief Shutdown system modules.  This is blocking.
 */
void SysShutdownSystem(void);

/**
 * \brief Prototype for the platform init function.
 *
 * This function is called by main() to allow the platform to do any other
 * needed init. 
 *
 * \param[in] plat Used to pass in information to customize the the init.
 *
 * \return Success or failure values
 */
uint32_t PlatformInit(uint32_t plat);

/**
 * \brief Do soft reset of platform.
 */
void PlatformReset( void );

#ifdef __cplusplus
}
#endif

#endif
