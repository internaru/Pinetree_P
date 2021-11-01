/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __JOBMGR_H__
#define __JOBMGR_H__

#include <stdint.h>
#include "mlimiter.h"
#include "event_observer.h"
#include "print_job_types.h"
#include "ResourceMap.h"

#ifdef __cplusplus
extern "C" {
#endif
#define JobMgr_myDeviceStatus_ID 0x43  
void JobMgr_AttachObserver_devicestatus(Observer_t *o);
void JobMgr_DetachObserver_devicestatus(Observer_t *o);
uint32_t JobMgr_Get_devicestatus( Subject_t *s);
JOB_TYPES JobMgr_Get_resourceJobType( Subject_t *s);
uint32_t JobMgr_Get_resourceJobID( Subject_t *s);

#if 0 
typedef enum
{
    e_JOB_IDLE,
    e_JOB_PAGE_START,
    e_JOB_PLANE_DATA
}JOB_EVENT_TYPE;
#define JobMgr_myPlaneDataEvent_ID 0x44 
void JobMgr_AttachObserver_plane_data_event(Observer_t *o);
void JobMgr_DetachObserver_plane_data_event(Observer_t *o);
PAGE_DATA * JobMgr_Get_plane_data_event_page( Subject_t *s);
PLANED * JobMgr_Get_plane_data_event_plane_data( Subject_t *s);
JOB_EVENT_TYPE JobMgr_Get_plane_data_event_type( Subject_t *s);
#endif

#if defined (DEBUG) || defined(HAVE_CMD)
int JobMgrPrintState(int argc, char* argv[]);
#endif

DECLARE_OR_DEFINE_FUNCTION_OF_TYPE_mlimiter_low_memory_strategy_t(print_low_memory_strategy);
#if 0
#define PRINT_JOBMGR_GLOBAL_PAGE_END_ID 0x44  
/**
 *
 * \brief Register for print job manager global page done (global means
 * anyone can register for this, vs. the print job info pipe only version)
 *
 * Modules who register will receive an event reporting callback.
 *
 * \param o An event reporting API Observer_t pointer, allocated by the caller
 *
 */
void print_jobmgr_global_page_end_observer_attach( Observer_t *o );


/**
 *
 * \brief unregister for print job manager global page done (global means
 * anyone can register for this, vs. the print job info pipe only version)
 *
 * \param o An event reporting API Observer_t pointer, allocated by the caller
 * also the same one used when registering
 *
 */
void print_jobmgr_global_page_end_observer_detach( Observer_t *o );



#define PRINT_JOBMGR_GLOBAL_JOB_END_ID 0x45  
/**
 *
 * \brief Register for print job manager global job done (global
 * means anyone can register for this, vs. the print job info 
 * pipe only version) 
 *
 * Modules who register will receive an event reporting callback.
 *
 * \param o An event reporting API Observer_t pointer, allocated by the caller
 *
 */
void print_jobmgr_global_job_end_observer_attach( Observer_t *o );


/**
 *
 * \brief unregister for print job manager global job done 
 * (global means anyone can register for this, vs. the print job 
 * info pipe only version) 
 *
 * \param o An event reporting API Observer_t pointer, allocated by the caller
 * also the same one used when registering
 *
 */
void print_jobmgr_global_job_end_observer_detach( Observer_t *o );
#endif

void job_mgr_free_memory( COLOR color );

void job_mgr_block_for_data_avail( uint32_t timeout );


#ifdef __cplusplus
}
#endif

#endif
