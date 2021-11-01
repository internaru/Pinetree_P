/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 **/

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>
#include "posix_ostools.h"
#include "memAPI.h"
#include "dprintf.h"
#include "faxer_api.h"
#include "dirent.h"
#ifdef HAVE_POWER_MGR
#include "fax_svc_pwr_drv.h"
#endif
#include "location_api.h" //for country ids
#include "platform_api.h"
#include "fax_svc_modem_status.h"
#include "sys_init_api.h"

typedef struct{
    uint32_t evt_id;
    uint32_t param;
    uint32_t param2;
    uint32_t filler;
}FAXER_MSG;

#define FAXER_CONTROLLER_QUEUE_SIZE 48              ///< Number of messages in this queue.

#define FAXER_CONTROLLER_STACK_SIZE (MAX((2048*5), POSIX_MIN_STACK_SIZE))   ///< The stack size for our task.

/* One and only context, it will be returned to the component calling start and passed down to the modem component */
FAXER_CONTEXT _FCtx; 

faxer_modem_state_t faxer_modem_state;
sem_t           faxer_modem_event_semaphore;

//static Observer_t *country_observer;

/*----------------------------------------------------------------------------*/
void _access_context(FAXER_CONTEXT *pFCtx, int lock_nLock)
{
}


uint32_t _faxer_post_fax_event(FAXER_CONTEXT *pFCtx, uint16_t evt_id, uint32_t param, uint32_t param2)
{
    uint32_t result = 0;
    return result;
}

// This needs to be in an include file.
void  *faxer_service_start(void (*app_cb)(uint16_t evt, void *data))
{
        return NULL;
}

/*
 *  Start a new fax receiving
 */
uint32_t faxer_start_receive(void *handle, fax_store_handle_t color_store, uint32_t *job_id)
{
    return 0;
}

/*
 * Start a fax polling receive
 *
 */
uint32_t faxer_start_polling(void *handle, char *phnnumber, uint32_t *job_id)
{
    return 0;
}

uint32_t faxer_group_send_add(void *handle, uint32_t lead_job_id, char *number, uint32_t *job_id)
{
    return 0;
}

uint32_t faxer_send_set_data(void *handle, uint32_t job_id, bool jpeg, void *data, 
                             uint16_t pages, bool data_complete)
{
    return(0);
}

uint32_t faxer_send_add_page(void *handle, uint32_t job_id, bool EOP)
{
    return 0;
}

uint32_t faxer_schedule_send(void *handle, struct tm *schdule_time, char *number, FAX_TYPES res,
                             char *fname, uint32_t *job_id)
{
    return(0);
}

 
uint32_t faxer_start_jpeg_send(void *handle, char *number, bool color, FAX_TYPES res, 
                               fax_store_handle_t store, uint32_t *job_id)
{
    return(0);
}


uint32_t _faxer_post_delay_event(FAXER_CONTEXT *pCtx, uint32_t delay_in_sec, uint16_t event, uint32_t param)
{
        return 1;
}


uint32_t faxer_terminate_fax(void *handle)
{
    return 1;
}


uint32_t faxer_get_session_data(void *handle, FAXER_SESSION_DATA id, void *buf)
{
    return 0;
}

/*
 * Dial digits
 * 
 */
uint32_t faxer_dial_digits(void *handle, char *dial_string)
{
    return 0;
}

uint32_t faxer_place_call(void *handle)
{
    return 0;
}

uint32_t faxer_end_call(void *handle)
{
     return 0;
}

uint32_t faxer_answer_call(void *handle)
{
    return 0;
}

#include "strfmt.h"

uint32_t faxer_free_storage(void *handle, int bytes_needed)
{
    return(0);
}

