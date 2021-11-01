/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include "logger.h"
#include "event_observer.h"
#include "PrintJobInfo.h"
#include "print_events_common_api.h"
#include "pjl_extra_api.h"
#include "agMessage.h"
#include "debug.h"
#include "cmd_proc_api.h"
#include "sjm_observers_api.h"

#define DBG_PRFX "vcp: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(24)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

/// temporary/local macro to save typing and temporary variables
/// note the use of the word cast in the definition to assist with grep'ing
#define pji_cast ((PrintJobInfo_t *) s)  

void vcp_jobstart_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO(" job_start name %s id %d\n", 
		    pji_cast->jobname, 
		    pji_cast->jobnum
	);
}
void vcp_jobend_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO(" job_end name %s id %d pages_printed %d jobstatus %x\n", 
		    pji_cast->jobname, 
		    pji_cast->jobnum,
		    pji_cast->pages_printed,
		    pji_cast->jobstatus
	);
}
void vcp_pagestart_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO(" pagestart %s id %d printed %d page %d\n", 
		    pji_cast->jobname, 
		    pji_cast->jobnum,
		    pji_cast->pages_printed,
		    pji_cast->job ? pji_cast->job->PAGECOUNT : -1
	);
}
void vcp_pageend_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO("  pageend name %s jobnum %d page %d\n", 
		    pji_cast->jobname, 
		    pji_cast->jobnum,
		    pji_cast->pages_printed
	);
}
void vcp_canceljobstart_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO(" canceljobstart name %s id %d\n", 
		    pji_cast->jobname, 
		    pji_cast->jobnum
	);
}
void vcp_canceljobend_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO(" canceljobend name %s id %d\n", 
		    pji_cast->jobname, 
		    pji_cast->jobnum
	);
}
void vcp_planedata_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_DEBUG(" planedata name %s numbytes %d\n", 
		    pji_cast->jobname, 
		    pji_cast->num_bytes
	);
}
#undef pji_cast // done with the macro
void vcp_pec_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO(" print system status %x %d %s job %x %d page %x %d\n", 
		    pec_get_printstatus( s ),  
		    map_print_status_to_pjl( pec_get_printstatus( s ) ), 
		    to_string_pjl_status( map_print_status_to_pjl( pec_get_printstatus( s ) ) ), 
		    pec_get_printjobstatus( s ), map_print_status_to_pjl( pec_get_printjobstatus( s ) ),  
		    pec_get_printpagestatus( s ), map_print_status_to_pjl( pec_get_printpagestatus( s ) )
	);
}
void vcp_sjm_update( Observer_t *o, Subject_t *s )
{
    DBG_PRINTF_INFO(" sjm type %d %s job %d status %d %s\n", 
		    SJM_Get_JobType(s),		    
		    to_string_job_types(SJM_Get_JobType(s)),
		    SJM_Get_JobId(s),
		    SJM_Get_JobStatus(s),
		    to_string_sjm_jobstate_enum(SJM_Get_JobStatus(s))
	);
}

int vcp_cancel_cb(int argc, char *argv[])
{
    MESSAGE TempMsg;
    TempMsg.msgType = MSG_CANCELJOB;
    TempMsg.param1 = SYS_REQUSER;
    SYMsgSend(SJMID, &TempMsg);
    return 0;
}


void virtual_control_panel_cmd_init()
{
    int i_res=0; UNUSED_VAR(i_res); // ignore cmd_register failure in release
    i_res = cmd_register( "vcp",
                          NULL,
                          NULL,
                          NULL,
                          NULL );
    XASSERT( i_res == CMD_OK, i_res );

    i_res = cmd_register_subcmd( "vcp",
				 "cancel",
				 "cancel job",
				 NULL,
				 NULL,
				 vcp_cancel_cb );
    XASSERT( i_res == CMD_OK, i_res );

}


void virtual_control_panel_register( )
{
    static bool oneshot = true;
    Observer_t *o;

    if (oneshot) {  // register once and never let go!
	oneshot = false;

	virtual_control_panel_cmd_init();

	o = Observer_Constructor( 0x12341001, vcp_jobstart_update );
	ATTACH_OBSERVER( system_pji->jobstart_observers, o );
	o = Observer_Constructor( 0x12341002, vcp_jobend_update );
	ATTACH_OBSERVER( system_pji->jobend_observers, o );
	o = Observer_Constructor( 0x12341003, vcp_pagestart_update );
	ATTACH_OBSERVER( system_pji->pagestart_observers, o );
	o = Observer_Constructor( 0x12341004, vcp_pageend_update );
	ATTACH_OBSERVER( system_pji->pageend_observers, o );
	o = Observer_Constructor( 0x12341005, vcp_canceljobstart_update );
	ATTACH_OBSERVER( system_pji->canceljobstart_observers, o );
	o = Observer_Constructor( 0x12341006, vcp_canceljobend_update );
	ATTACH_OBSERVER( system_pji->canceljobend_observers, o );
	o = Observer_Constructor( 0x12341007, vcp_planedata_update );
	ATTACH_OBSERVER( system_pji->planedata_observers, o );

	o = Observer_Constructor( 0x12341008, vcp_pec_update );
	pec_attachobserver_printstatus( o );

	o = Observer_Constructor( 0x12341009, vcp_sjm_update );
	SJM_AttachObserver_JobStatus( o );
    }
}

