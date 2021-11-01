/* 
 * **********************************************************
 * (c) Copyright 2009-2010 Marvell International Ltd.
 *
 *               Marvell Confidential
 * ==========================================================
 */
/**
 * \file sjm_observers_api.h
 *
 * \brief
 *
 */

#ifndef SJM_OBSERVERS_API_H
#define SJM_OBSERVERS_API_H


#ifdef __cplusplus
extern "C" {
#endif
#include "event_observer.h"
#include "ResourceMap.h" // JOB_TYPES
/**
 * This defines the observers for system job status.
 */
#define SJM_JobStatus_ID 0xfedb
typedef enum
{
    esjm_JobReceived,
    esjm_JobStart,
    esjm_JobDone,
    esjm_JobCancel_at_device,
    esjm_JobCancel_by_user,
    esjm_JobCancelDone,
    esjm_JobDropped
} sjm_jobstate_enum_t;

sjm_jobstate_enum_t SJM_Get_JobStatus( Subject_t *s);
JOB_TYPES SJM_Get_JobType(Subject_t *s);
uint32_t SJM_Get_JobId(Subject_t *s);
void SJM_AttachObserver_JobStatus(Observer_t *o);
void SJM_DetachObserver_JobStatus(Observer_t *o);
#ifdef HAVE_DBG_PRINTF
    const char* to_string_sjm_jobstate_enum(sjm_jobstate_enum_t enum_i);
#else
    static inline const char* to_string_sjm_jobstate_enum(sjm_jobstate_enum_t enum_i) {return 0;}
#endif
#ifdef __cplusplus
}
#endif

#endif
