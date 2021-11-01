/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */
/** Fax log related functions */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "dprintf.h"
#include "faxer_api.h"
//#include "prv_faxer.h"
#include "nvram_api.h"
#include "lassert.h"

/* update when size or types within fax_log changes */
#define FAXER_LOG_NVM_VER_ID (8)

void (*_init_log_storage)(void);
void (*_log_load)(void);
void (*_log_flush)(void);

/* This is the ram storage of the log entries, these entries are backed by 
   nv storage. The storage is a simple circular buffer. */
typedef struct
{
  uint8_t head;
  uint8_t tail;
//  FAX_LOG_ENTRY data[FAXER_MAX_LOG_ENTRIES_TO_KEEP];
  uint8_t  num_logs;
  uint32_t id_counter;
} _fax_log_t; 

pthread_mutex_t _mutex;

/*------------------------------------------------------------*/
void _access(int lock_nLock)
{
}

#if 0 /* simplify id by removing count base...not sure why this was 
         needed in the first place */
/*------------------------------------------------------------*/
static void _counter_init()
{
}
#endif


/*------------------------------------------------------------*/
uint32_t _faxer_get_new_job_id()
{
    return(0);
}

void _fs_init_storage()
{
}

/*---------------------------------------------------*/
void _faxer_log_init()
{
}

/*---------------------------------------------------*/
void _faxer_log_flush()
{
}

/**
 * Remove fax file associated with a fax job id
 */
uint32_t faxer_clear_fax(void *handle, uint32_t id, bool force)
{
     uint32_t ret = 1;
    return(ret);
}

/*
 * Get activity log
 */
uint32_t faxer_get_log(void *handle, uint32_t id, FAXER_ACTIVITY_ENTRY *entry)
{
    return 0;
}

/*
 *  Unused, changing to mode specific function for marking completed
 * 
 */
uint32_t faxer_pending_action(void *handle, uint32_t job_id, uint16_t *act, uint16_t act_mask)
{
      return 0;
}

/*
 * Find log entry
 */
uint32_t faxer_find_fax(void *handle, FAXER_ACT_TYPE type_to_match, uint16_t tag_to_match, 
                        uint32_t *index_set, uint16_t size)
{
     return 0;
}


uint32_t _faxer_clear_last_fax(void *handle)
{
  int ret = 1;
  return(ret);
}

/* WARNING: Designed for debug only, will cause issues if called when job
   is active.  Similar to an NVM reset for log only. */
void _faxer_log_clear_all()
{  
}

