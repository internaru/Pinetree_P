/**
 *
 * ============================================================================
 * Copyright (c) 2007-2012   Marvell International, Ltd. All Rights Reserved 
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 **/

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "dprintf.h"
#include "logger.h"
#include "lassert.h"
#include "debug.h"
#include "PrintMgr.h"
#include "JobMgr.h"
#include "error_types.h"
#include "printvars_api.h"
#include "print_events_common_api.h"

#define DBG_PRFX "PRINTSTAT: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 8 )

static Observer_t *PrintStatus=NULL;
static Observer_t *JobMgrStatus=NULL;

typedef struct print_system_status_s
{
    Subject_T_BaseClassMembers;
    MLISTENTRY print_status_observers;

    uint32_t status;
    bool     online; ///< ONLINE/offline status of print system
    // debug status info
    uint32_t print_mgr_status;
    uint32_t job_mgr_status;
    JOB_TYPES resourceJobType;
    uint32_t resourceJobID;

    // retry information for print observer
    TRY_AGAIN retry_callback;
    USER_CANCEL cancel_callback;
    Subject_t* retry_subject;
} print_system_status_t;

static print_system_status_t print_sys_status;

static void print_status_updater(Observer_t *observer, Subject_t *s);
static void jobmgr_status_updater(Observer_t *observer, Subject_t *s);

static Observer_t *PrintStatus;
static Observer_t *JobMgrStatus;

static void print_sys_status_constructor()
{
    memset(&print_sys_status, 0, sizeof(print_system_status_t));
    print_sys_status.online = OFFLINE;
    print_sys_status.status = STATUS_INVALID;
    print_sys_status.print_mgr_status = STATUS_INVALID;
    print_sys_status.job_mgr_status = STATUS_INVALID;
    print_sys_status.subject_observer_oid_key = PrintJobMgr_PrintStatus_ID;
    MInitList(&print_sys_status.print_status_observers);
}

bool print_sys_is_online()
{
	return print_sys_status.online;
}
error_type_t printvar_get_online(uint32_t *val)
{
	ASSERT(val); /// 
	return *val = print_sys_is_online();
}


static void print_sys_status_filtered_notify()
{
    uint32_t worst_status = STATGETWORSE(print_sys_status.job_mgr_status, print_sys_status.print_mgr_status);

    print_system_status_t* pstatus;

    DBG_PRINTF_NOTICE("%s jm status = %x pm status = %x worst = %x\n",__func__,print_sys_status.job_mgr_status, print_sys_status.print_mgr_status, worst_status);

    //if(worst_status != print_sys_status.status)
    {
        print_sys_status.status = worst_status;

	uint32_t online;
        if(ISOFFLINE(worst_status))
            online = OFFLINE;
        else
            online = ONLINE;
	if ( print_sys_status.online != online )
	{
		//printvar_set_online(&online); /// why would we allow the setting of online state independent of status?
		MESSAGE msg = {0};
		print_sys_status.online = online;
		msg.msgType = MSG_PRINTONLINE;
		msg.param2 = online;
		SYMsgSend(PRINTMGRID, &msg);
	}


        pstatus = &print_sys_status;
        NOTIFY_OBSERVERS(pstatus, print_status_observers );
        DBG_PRINTF_NOTICE("%s %x\n",__func__, worst_status);
    }
    /*
    else
    {
        DBG_PRINTF_NOTICE("%s %x - no change\n",__func__, worst_status);
    }*/
}

void pec_attachobserver_printstatus(Observer_t *o)
{
    DBG_PRINTF_NOTICE("%s %#x\n", __func__, o);
    ATTACH_OBSERVER( print_sys_status.print_status_observers, o );
}

void pec_detachobserver_printstatus(Observer_t *o)
{
    DBG_PRINTF_NOTICE("%s %#x\n", __func__, o);
    DETACH_OBSERVER( print_sys_status.print_status_observers, o );
}

uint32_t pec_get_current_printstatus()
{
    return print_sys_status.status;
}

uint32_t pec_get_printstatus( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t, 
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return print_sys_status.status;
}

uint32_t pec_get_printjobstatus( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t, 
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return print_sys_status.job_mgr_status;
}

uint32_t pec_get_printpagestatus( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t, 
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return print_sys_status.print_mgr_status;
}

JOB_TYPES pec_get_resourceJobType( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t,
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed.
    return print_sys_status.resourceJobType;
}

uint32_t pec_get_resourceJobID( Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t,
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed.
    return print_sys_status.resourceJobID;
}

TRY_AGAIN pec_get_retry_callback(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t, 
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return print_sys_status.retry_callback;
}

USER_CANCEL pec_get_user_cancel_callback(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t, 
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return print_sys_status.cancel_callback;
}
Subject_t* pec_get_retry_subject(Subject_t *s)
{
    ASSERT(VALIDATE_OBJECT_KEY( s, print_system_status_t, 
                                subject_observer_oid_key, PrintJobMgr_PrintStatus_ID) );

    // message causes notify causes get so no lock needed. 
    return print_sys_status.retry_subject;
}

bool pec_is_observing_printstatus(Observer_t *o)
{
    if (DETACH_OBSERVER( print_sys_status.print_status_observers, o ))
    {
        ATTACH_OBSERVER( print_sys_status.print_status_observers, o );
        return true;
    }
    return false;
}

/**
 * \brief print_status_init
 *
 **/
void print_status_init(void)
{  
    print_sys_status_constructor();
}

/**
 * \brief print_status_attach 
 * Note that attach and init are two different routines.  We init early so that others can attach 
 * to the print status observer, but we can't actually attach to the objects until the print 
 * system and job manager have finished their initialization. 
 *
 **/
void print_status_attach()
{
    if(!PrintStatus)
    {
        PrintStatus = Observer_Constructor(PrintMgr_myDeviceStatus_ID, print_status_updater);
        PrintMgr_AttachObserver_devicestatus(PrintStatus);
        JobMgrStatus = Observer_Constructor(JobMgr_myDeviceStatus_ID, jobmgr_status_updater);
        JobMgr_AttachObserver_devicestatus(JobMgrStatus);
    }

    ASSERT(PrintStatus != NULL);
    ASSERT(JobMgrStatus != NULL);
}

/**
 * \brief Callback when a system status is updated
 * When this happens, get the status, make a message and
 * forward it to the correct task
 */
static void print_status_updater(Observer_t *observer, Subject_t *s)
{
    uint32_t CurStatus;
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, PrintMgr_myDeviceStatus_ID));
    CurStatus = PrintMgr_Get_devicestatus(s);
    print_sys_status.print_mgr_status = CurStatus;
    print_sys_status.retry_callback = PrintMgr_Get_retry_callback(s);
    print_sys_status.cancel_callback = PrintMgr_Get_cancel_button_callback(s);

    if(print_sys_status.retry_callback != NULL)
    {
        print_sys_status.retry_subject = s;
    }
    print_sys_status_filtered_notify();
}

/**
 * \brief Callback when a system status is updated
 * When this happens, get the status, make a message and
 * forward it to the correct task
 */
static void jobmgr_status_updater(Observer_t *observer, Subject_t *s)
{
    uint32_t CurStatus;
    ASSERT(VALIDATE_OBJECT_KEY(observer, Observer_t, 
                               subject_observer_oid_key, JobMgr_myDeviceStatus_ID));
    CurStatus = JobMgr_Get_devicestatus(s);
    print_sys_status.job_mgr_status = CurStatus;
    print_sys_status.resourceJobType = JobMgr_Get_resourceJobType(s);
    print_sys_status.resourceJobID = JobMgr_Get_resourceJobID(s);
    print_sys_status.cancel_callback = NULL;
    print_sys_status_filtered_notify();
}

/**
 * \brief Debug routine to print the current status mgr state.
  */
void print_sys_status_dump(void)
{
    DBG_PRINTF_CRIT("print_sys_status: ---------- enter ----------\n");

    DBG_PRINTF_CRIT("print_sys_status: status = 0x%08X\n", print_sys_status.status);
    DBG_PRINTF_CRIT("print_sys_status: job_mgr_status = 0x%08X\n", print_sys_status.job_mgr_status);
    DBG_PRINTF_CRIT("print_sys_status: print_mgr_status = 0x%08X\n", print_sys_status.print_mgr_status);

    DBG_PRINTF_CRIT("print_sys_status: ---------- exit ----------\n");
}

